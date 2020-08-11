
/// ========================================================================
/// UNLICENSE
/// 
/// This is free and unencumbered software released into the public domain.
/// Anyone is free to copy, modify, publish, use, compile, sell, or
/// distribute this software, either in source code form or as a compiled
/// binary, for any purpose, commercial or non-commercial, and by any
/// means.
///
/// In jurisdictions that recognize copyright laws, the author or authors
/// of this software dedicate any and all copyright interest in the
/// software to the public domain. We make this dedication for the benefit
/// of the public at large and to the detriment of our heirs and
/// successors. We intend this dedication to be an overt act of
/// relinquishment in perpetuity of all present and future rights to this
/// software under copyright law.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
/// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
/// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
/// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/// OTHER DEALINGS IN THE SOFTWARE.
///
/// For more information, please refer to <http://unlicense.org/>
/// ========================================================================

#pragma once

//System Includes
#include <type_traits>
#include <memory>
#include <cstring>
#include <cstddef>

//Handy Includes
#include "HandyBase.hpp"

#if !defined IS_CLI
	#include <mutex>
#endif

namespace HANDY_NS {

	template <typename T>
	FORCEINLINE void SafeDelete(T * &t)
	{
		if (t != nullptr)
		{
			delete t;
			t = nullptr;
		}
	}

	template <typename T>
	FORCEINLINE void SafeDeleteArray(T * &t)
	{
		if (t != nullptr)
		{
			delete[] t;
			t = nullptr;
		}
	}

	template<typename T, typename ... Args>
	FORCEINLINE T * SafeNew(Args &&... args)
	{
		return new (std::nothrow) T(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	FORCEINLINE T * SafeNew(T *&t, Args &&... args)
	{
		return t = new (std::nothrow) T(std::forward<Args>(args)...);
	}

	#ifndef IS_CLANG
	template<typename T, typename ... Args>
	FORCEINLINE T * SafeNewArray(size_t size, Args &&... args)
	{
		return new (std::nothrow) T[size](std::forward<Args>(args)...);
	}
	#endif

	template<typename T>
	FORCEINLINE T * SafeNewArrayRaw(size_t size)
	{
		return new (std::nothrow) T[size];
	}

	template <typename T>
	FORCEINLINE void Zero(T & data)
	{
		memset(&data, 0, sizeof(T));
	}

	// Does not accout for endianness!
	// For primitive types, resolves to 2 ops in most compilers, without violating strict aliasing rules.
	template <typename FROM_TYPE, typename TO_TYPE>
	TO_TYPE ReinterpretBitsByCopy(FROM_TYPE from)
	{
		STATIC_ASSERT(sizeof(FROM_TYPE) == sizeof(TO_TYPE));

		TO_TYPE to;
		char * fromPointer = (char *)&from;
		char * toPointer = (char *)&to;
		memcpy(toPointer, fromPointer, sizeof(from));
		return to;
	}

	// Does not accout for endianness!
	// For primitive types, resolves to 2 ops in most compilers, without violating strict aliasing rules.
	template <typename FROM_TYPE, typename TO_TYPE>
	TO_TYPE ReinterpretBitsByUnion(FROM_TYPE from)
	{
		STATIC_ASSERT(sizeof(FROM_TYPE) == sizeof(TO_TYPE));

		union F2T
		{
			FROM_TYPE from;
			TO_TYPE   to;
		};

		F2T combined;
		combined.from = from;

		return combined.to;
	}

	#if !defined IS_CLI

	///-----------------------------------------------
	/// Pool Allocator
	/// For O(1) allocation times.
	template <class TYPE, size_t ITEMSPERARENA = 64>
	class PoolAllocator final
	{
		union Slot
		{
			alignas(TYPE)
				char   Payload[sizeof(TYPE)];

			Slot * NextFree;
		};

		struct Arena
		{
			Slot          Data[ITEMSPERARENA];
			size_t        FirstFree;
			Arena * const PrevArena;

			Arena() noexcept : FirstFree(0), PrevArena(nullptr) {}
			Arena(Arena * prev) noexcept : FirstFree(1), PrevArena(prev) {}
			~Arena() noexcept {}
		};

		Arena * m_arena;
		Slot  * m_freeList;
		std::mutex m_mutex;

		PoolAllocator(const PoolAllocator &) = delete;
		PoolAllocator & operator=(const PoolAllocator &) = delete;

	public:
		PoolAllocator() : m_freeList(nullptr) 
		{
			Locked(m_mutex)
				m_arena = new Arena;
		}

		~PoolAllocator() noexcept
		{
			Locked(m_mutex)
			{
				while (m_arena)
				{
					Arena * arena = m_arena;
					m_arena = arena->PrevArena;
					delete arena;
				}
			}
		}

		FORCEINLINE TYPE * Alloc()
		{
			Locked(m_mutex)
			{
				TYPE * ret = nullptr;

				if (m_freeList)
				{
					Slot * slot = m_freeList;
					m_freeList = slot->NextFree;

					ret = reinterpret_cast<TYPE *>(slot->Payload);
				}
				else if (m_arena->FirstFree < ITEMSPERARENA)
				{
					ret = reinterpret_cast<TYPE *>(m_arena->Data[m_arena->FirstFree++].Payload);
				}
				else
				{
					m_arena = new Arena(m_arena);
					ret = reinterpret_cast<TYPE *>(m_arena->Data[0].Payload);
				}

				return ret;
			}
		}

		FORCEINLINE TYPE * Construct() /// TODO make a Construct(args) that passes all params over to TYPE ctor.
		{
			TYPE * ret = Alloc();

			//memset(ret, 0, sizeof(TYPE));

			return new (ret) TYPE();
		}

		FORCEINLINE void Free(TYPE * ptr) noexcept
		{
			Locked(m_mutex)
			{
				Slot * slot = reinterpret_cast<Slot *>(reinterpret_cast<char *>(ptr) - offsetof(Slot, Payload));
				slot->NextFree = m_freeList;
				m_freeList = slot;
			}
		}

		FORCEINLINE void SafeFree(TYPE * & ptrR) noexcept
		{
			if (ptrR == nullptr)
				return;

			Free(ptrR);

			ptrR = nullptr;
		}
	};
	///-----------------------------------------------

	/// Self-expanding memory pool that guarantees returned chunks will not move as long as the pool exists.
	/// Allocates memory in TypicalBlockSizeBytes size chunks, but if you request a size larger than this, it 
	/// will allocate a chunk of the requested size instead.
	class MemoryPool
	{
	public:
		size_t TypicalBlockSizeBytes; // One Mebibyte by default

		/// TODO: Figure out how to make std::swap for MemoryPool be a friend function so I can make all these fields private:
		std::mutex Mutex;

		std::vector<std::vector<std::byte> *> Blocks;

		size_t BlockIndex = 0;
		size_t ByteIndex  = 0;

		bool Inited = false;

		MemoryPool(size_t typicalBlockSizeBytes = 1'048'576) : TypicalBlockSizeBytes(typicalBlockSizeBytes) { }

		COPY_ASSIGN_MOVE_CTOR(MemoryPool, delete, delete, delete)

	private:
		void tryInit()
		{
			if (!Inited)
			{
				(Blocks.emplace_back() = new std::vector<std::byte>())->resize(TypicalBlockSizeBytes);
				Inited = true;
			}

		}

	public:

		void * Get(size_t numBytes)
		{
			std::scoped_lock sLock(Mutex);

			tryInit();

			if (numBytes <= (Blocks[BlockIndex]->size() - ByteIndex))
			{
				void * ret = &(*Blocks[BlockIndex])[ByteIndex];
				ByteIndex += numBytes;
				return ret;
			}

			auto & nuBlock = *(Blocks.emplace_back(new std::vector<std::byte>()));
			++BlockIndex;

			if (numBytes > TypicalBlockSizeBytes / 2)
			{
				nuBlock.resize(numBytes);
				std::swap(Blocks[BlockIndex], Blocks[BlockIndex -1]);
			}
			else
			{
				nuBlock.resize(TypicalBlockSizeBytes);
				ByteIndex = numBytes;
			}

			return &nuBlock[0];
		}

		//template <typename T, typename... Types>
		//T * Get(Types... args)
		//{
		//	void * buf = Get(sizeof(T));
		//	return new buf T(&args...);
		//}

		std::span<std::byte> GetCopy(std::span<std::byte> blob)
		{
			std::byte * ret = reinterpret_cast<std::byte *>(Get(blob.size()));
			memcpy(ret, blob.data(), blob.size());
			return std::span<std::byte>(ret, blob.size());
		}

		std::span<std::byte> GetCopy(std::span<std::byte const> blob)
		{
			std::byte * ret = reinterpret_cast<std::byte *>(Get(blob.size()));
			memcpy(ret, blob.data(), blob.size());
			return std::span<std::byte>(ret, blob.size());
		}

		void Reset()
		{
			std::scoped_lock sLock(Mutex);

			for (std::vector<std::byte> * item : Blocks)
				delete item;

			Blocks.clear();
			BlockIndex = 0;
			ByteIndex  = 0;

			Inited = false;
		}
	private:
		void adoptImpl(std::vector<std::byte> & block)
		{
			Blocks.emplace_back() = new std::vector<std::byte>();
			std::swap(*Blocks.back(), block);
			++BlockIndex;

			std::swap(Blocks[BlockIndex], Blocks[BlockIndex -1]);
		}

	public:
		void Adopt(std::vector<std::byte> & block)
		{
			std::scoped_lock sLock(Mutex);

			tryInit();

			adoptImpl(block);
		}

		void Adopt(MemoryPool & otherPool)
		{
			std::scoped_lock sLock(Mutex, otherPool.Mutex);

			tryInit();

			for (auto blockP : otherPool.Blocks)
				adoptImpl(*blockP);

			otherPool.Reset();
		}

		~MemoryPool()
		{
			Reset();
		}
		

		void swap(HANDY_NS::MemoryPool & rhs) noexcept
		{
			std::scoped_lock sLock(Mutex, rhs.Mutex);

			std::swap(Blocks,     rhs.Blocks);
			std::swap(BlockIndex, rhs.BlockIndex);
			std::swap(ByteIndex,  rhs.ByteIndex);
			std::swap(Inited,     rhs.Inited);
			std::swap(TypicalBlockSizeBytes, rhs.TypicalBlockSizeBytes);
		}

		/// ---------

		struct span
		{
			MemoryPool & Pool;
			std::span<std::byte> Span;

			explicit
			span(MemoryPool & pool) : Pool(pool) {}
			span(MemoryPool & pool, std::byte * p, std::ptrdiff_t count) : Pool(pool), Span(p, count) { }
		};
	};

	template <> 
	FORCEINLINE std::byte * Deserializer<MemoryPool::span>::prepare_span(size_t sizeBytes, MemoryPool::span & outItem)
	{
		if (outItem.Span.empty())
		{
			std::byte * p = reinterpret_cast<std::byte *>(outItem.Pool.Get(sizeBytes));
			outItem.Span = std::span<std::byte>(p, sizeBytes);
			return p;
		}

		if (outItem.Span.size() == (std::ptrdiff_t)sizeBytes)
			return outItem.Span.data();

		#if defined HANDY_SERDE_NOISY_NULLRET
			std::cerr << "Handy::Deserializer - arithmetic type T is the wrong size to be assigned this data.";
		#endif

		return nullptr;
	}

	template <> 
	FORCEINLINE std::span<std::byte const> Serializer<MemoryPool::span>::get_span(MemoryPool::span const & inItem)
	{
		return inItem.Span;
	}
	#endif


} // HANDY_NS


//namespace std
//{
//	template<>
//	void swap(HANDY_NS::MemoryPool & lhs, HANDY_NS::MemoryPool & rhs) noexcept
//	{
//		std::scoped_lock sLock(lhs.Mutex, rhs.Mutex);
//
//		std::swap(lhs.Blocks,     rhs.Blocks);
//		std::swap(lhs.BlockIndex, rhs.BlockIndex);
//		std::swap(lhs.ByteIndex,  rhs.ByteIndex);
//		std::swap(lhs.Inited,     rhs.Inited);
//	}
//
//}

