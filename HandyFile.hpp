
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

#include <fcntl.h>
#include <cstdio>

#include "HandyDeque.hpp"
#include "HandyMemory.hpp"
#include "HandyThreadUtils.hpp"
#include "HandyUtils.hpp"


#ifdef IS_WINDOWS
	//#include <winsock2.h>
	#include <io.h>
#else
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h> 

	#define _lseeki64 lseek64
	#define  _telli64 tell64

	/// I'm not sure if tell64 is defined on linux, this could be a fix, by using lseek64:
	FORCEINLINE int64_t tell64(int filedes) { return lseek64(filedes, 0, SEEK_CUR); }
	/// You may need to define (https://www.ohse.de/uwe/articles/lfs.html https://users.suse.com/~aj/linux_lfs.html)
	///		_LARGEFILE_SOURCE=1
	///		_LARGEFILE64_SOURCE 
	///		_FILE_OFFSET_BITS=64
	/// (or some combination)

	#define O_BINARY 0
	#define _read  read
	#define _write write
	#define _close close
	#define _open  open
#endif



//#define SINGLETHREAD_CAF

namespace HANDY_NS {

	enum class FileExistsMode
	{
		if_exists_fail,
		if_exists_just_open,
		//if_exists_map_all,
		if_exists_truncate,
	};

	enum class FileNotExistMode
	{
		if_doesnt_exist_fail,
		if_doesnt_exist_create,
	};

	enum class AccessPosition
	{
		Read,
		Write,
	};

	class File
	{
		COPY_ASSIGN_MOVE_CTOR(File, delete, delete, delete)

		std::filesystem::path m_path;

		bool m_isReadOnly   = false;

		std::fstream m_handle;

		void errClearError();

	public:
		File() = default;
		~File();

		std::filesystem::path Path() const { return m_path; }

		bool Open        (std::filesystem::path path, bool resetFile = false);
		bool Open        (std::filesystem::path path, FileExistsMode exists_mode, FileNotExistMode doesnt_exist_mode);
		bool OpenReadOnly(std::filesystem::path path);
		void Close();

		bool IsOpen();

		int64_t Tell(AccessPosition acc);

		bool Flush();

		bool Seek       (AccessPosition acc, int64_t offset            = 0);
		bool SeekCurrent(AccessPosition acc, int64_t offsetFromCurrent = 0);
		bool SeekEnd    (AccessPosition acc, int64_t reverseOffset     = 0);

		bool     Read    (void       * buffer, uint64_t numBytes);
		uint64_t ReadUpTo(void       * buffer, uint64_t maxBytes);
		bool     Write   (void const * buffer, uint64_t numBytes);

		template <typename TDst> bool ReadTo(TDst & dDst, uint64_t numBytes); /// Should just be Read() but it ends up being recursive.
		template <typename TSrc> bool Write (TSrc & dSrc);
	};

	FORCEINLINE File::~File() { /* m_handle is dtor'd automatically */ }

	FORCEINLINE bool File::IsOpen() { return m_handle.is_open(); }

	FORCEINLINE int64_t File::Tell(AccessPosition acc) 
	{
		if (!m_handle.is_open())
			return -1;

		switch (acc)
		{
			case AccessPosition::Read:  return m_handle.tellg();
			case AccessPosition::Write: return m_handle.tellp();
			default: throw std::runtime_error("Not Implemented");
		}

		return 0;
	} 

	inline void File::errClearError()
	{
		if (!m_handle.is_open())
		{
			std::cout << std::endl
				<< m_path.string() << " : " << "File error, file not open." << std::endl
				<< "    " << "Eofbit:  " << m_handle.eof()  << std::endl
				<< "    " << "Failbit: " << m_handle.fail() << std::endl
				<< "    " << "Badbit:  " << m_handle.bad()  << std::endl;
			return;
		}

		std::cout << std::endl
			<< m_path.string() << " : " << "General file error." << std::endl
			<< "    " << "Eofbit:  " << m_handle.eof()  << std::endl
			<< "    " << "Failbit: " << m_handle.fail() << std::endl
			<< "    " << "Badbit:  " << m_handle.bad()  << std::endl;

		/// Bad bit is an indication that the stream is now poisoned ("When set, the integrity of the stream may have been affected.") 
		//m_handle.clear(std::ios::badbit);
		if (m_handle.bad())
		{
			m_handle.close();
			std::cout << m_path.u8string() << " : " << "File bad bit set; closing file. All subsequent file operations will fail." << std::endl;
			return;
		}

		m_handle.clear(std::ios::failbit);
		m_handle.clear(std::ios::eofbit);
	}

	inline bool File::Open(std::filesystem::path path, bool resetFile)
	{
		return Open(path, resetFile ? FileExistsMode::if_exists_truncate : FileExistsMode::if_exists_just_open, FileNotExistMode::if_doesnt_exist_create);
	}

	inline bool File::Open(std::filesystem::path path, FileExistsMode exists_mode, FileNotExistMode doesnt_exist_mode)
	{
		auto mode = std::ios::binary | std::ios::in | std::ios::out;

		if (std::filesystem::exists(path))
			switch (exists_mode)
			{
				case FileExistsMode::if_exists_fail:      return false;
				case FileExistsMode::if_exists_just_open: break;
				case FileExistsMode::if_exists_truncate:  mode |= std::ios::trunc; break;
				default: break;
			}
		else
			switch (doesnt_exist_mode)
			{
				case FileNotExistMode::if_doesnt_exist_fail: return false;
				case FileNotExistMode::if_doesnt_exist_create: { std::ofstream touchOFS; touchOFS.open(path, std::ios::binary); mode |= std::ios::trunc; } break;
				default: break;
			}

		m_handle.open(path, mode);
		m_path = path;

		if (m_handle.good())
		{
			m_isReadOnly = false;

			return true;
		}

		errClearError();

		return false;
	}

	inline bool File::OpenReadOnly(std::filesystem::path path)
	{
		m_handle.open(path, std::ios::binary | std::ios::in);
		m_path = path;

		if (m_handle.good())
		{
			m_isReadOnly = true;
			return true;
		}

		errClearError();

		return false;
	}

	inline void File::Close()
	{
		m_handle.close();
	}


	inline bool File::Flush()
	{
		if (!m_handle.is_open())
			return false;

		if (m_isReadOnly)
			return false;

		m_handle.flush();

		if (m_handle.good())
			return true;

		errClearError();
		return false;
	}

	inline bool File::Seek(AccessPosition acc, int64_t offset)
	{
		if (!m_handle.is_open())
			return false;

		if (m_handle.good())
		{
			switch (acc)
			{
				case AccessPosition::Read:  m_handle.seekg(offset, std::ios::beg); break;
				case AccessPosition::Write: m_handle.seekp(offset, std::ios::beg); break;
				default: throw std::runtime_error("Not Implemented");
			}
			return true;
		}

		errClearError();
		return false;
	}

	inline bool File::SeekCurrent(AccessPosition acc, int64_t offsetFromCurrent)
	{
		if (!m_handle.is_open())
			return false;

		if (m_handle.good())
		{
			switch (acc)
			{
				case AccessPosition::Read:  m_handle.seekg(offsetFromCurrent, std::ios::cur); break;
				case AccessPosition::Write: m_handle.seekp(offsetFromCurrent, std::ios::cur); break;
				default: throw std::runtime_error("Not Implemented");
			}
			return true;
		}

		errClearError();
		return false;
	}

	inline bool File::SeekEnd(AccessPosition acc, int64_t reverseOffset)
	{
		if (!m_handle.is_open())
			return false;

		if (m_handle.good())
		{
			switch (acc)
			{
				case AccessPosition::Read:  m_handle.seekg(reverseOffset, std::ios::end); break;
				case AccessPosition::Write: m_handle.seekp(reverseOffset, std::ios::end); break;
				default: throw std::runtime_error("Not Implemented");
			}
			return true;
		}

		errClearError();
		return false;
	}
	
	inline bool File::Read(void * buffer, uint64_t numBytes)
	{
		if (!m_handle.is_open())
			return false;

		if (numBytes == 0)
			return true;
		
		if (!m_handle.good())
		{
			errClearError();
			return false;
		}

		m_handle.read((char *)buffer, numBytes);

		if (m_handle.good() && m_handle.gcount() == (int64_t)numBytes)
			return true;

		errClearError();
		return false;
	}

	inline uint64_t File::ReadUpTo(void * buffer, uint64_t maxBytes)
	{
		if (!m_handle.is_open())
			return 0;

		if (maxBytes == 0)
			return 0;

		if (!m_handle.good())
		{
			errClearError();
			return false;
		}

		m_handle.read((char *)buffer, maxBytes);

		uint64_t bytesRead = m_handle.gcount();

		if (bytesRead < maxBytes)
		{
			m_handle.clear(std::ios::failbit);
			m_handle.clear(std::ios::eofbit);
		}

		if (m_handle.good())
			return bytesRead;

		errClearError();
		return 0;
	}

	inline bool File::Write(void const * buffer, uint64_t numBytes)
	{
		if (m_isReadOnly)
			return false;

		if (!m_handle.is_open())
			return false;

		if (numBytes == 0)
			return true;

		if (!m_handle.good())
		{
			errClearError();
			return false;
		}

		m_handle.write((char const *)buffer, numBytes);

		if (m_handle.good())
			return true;

		errClearError();
		return false;
	}
	 
	template <typename TDst>
	FORCEINLINE bool File::ReadTo(TDst & dDst, uint64_t numBytes)
	{
		if (!IsOpen())
			return false;

		if (numBytes == 0)
			return true;

		Deserializer<TDst> dstDeser;
		uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span((size_t)numBytes, dDst));

		if (!dst)
			return false;

		return Read(dst, numBytes);
	}

	template <typename TSrc>
	FORCEINLINE bool File::Write(TSrc & dSrc)
	{
		if (m_isReadOnly)
			return false;

		if (!IsOpen())
			return false;

		Serializer<TSrc>           srcSer;
		std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
		uint8_t const *            src = reinterpret_cast<uint8_t const *>(srcSpan.data());

		return Write(src, (uint64_t)srcSpan.size());
	}

	/// Returns true if successfully wrote source to file.
	template <typename TSrc>
	bool TryWriteFile(std::filesystem::path filePath, TSrc const & source)
	{
		File f;
		if (!f.Open(filePath, true))
			return false;

		return f.Write(source);
	}

	/// Returns true if successfully read file into destination.
	template <typename TDst>
	bool TryReadFile(std::filesystem::path filePath, TDst & destination)
	{
		if (!std::filesystem::exists(filePath))
			return false;

		File f;
		if (!f.OpenReadOnly(filePath))
			return false;

		if (!f.SeekEnd(AccessPosition::Read)) return false;
		uint64_t sz = f.Tell(AccessPosition::Read);
		if (sz < 0)       return false;
		if (!f.Seek(AccessPosition::Read))   return false;

		return f.ReadTo(destination, sz);
	} 

	#if !defined IS_CLI

	// VERY fast and compressed file that can be read or appended. Always append as much data at a time as possible, since
	// any data below the block size will temporarily be stored in a "-tl" file on Flush or Close.
	class CompressedAppendFile
	{
		class CABlock
		{
			COPY_ASSIGN_MOVE_CTOR(CABlock, delete, delete, delete);

			File & m_file;


		public:

			uint64_t FileOffset;  // Offset in the file to the beginning of this block (the start of the sizing section)

			uint64_t PayloadSize; // Size of the data payload for this block. SHOULD BE detail::BLOCK_SIZE
			uint64_t CompactSize; // Size of the data payload as compressed on disk.


			std::vector<std::byte> * Data = nullptr; // If currently loaded.

			void ReadInfo()
			{
				std::array<uint64_t, 2> sizes;
				m_file.Seek(AccessPosition::Read, FileOffset);
				m_file.Read(&sizes[0], sizeof(uint64_t) * 2);
				CompactSize = sizes[0];
				PayloadSize = sizes[1]; /// This should be detail::BLOCK_SIZE.
			}

			uint64_t NextBlockFileOffset() const { return FileOffset + 16 + CompactSize; }
			
			bool IsLoaded() const { return Data != nullptr; }

			void EnsureLoaded()
			{
				if (Data)
					return;

				m_file.Seek(AccessPosition::Read, FileOffset);
				std::vector<std::byte> compressedData;
				m_file.ReadTo(compressedData, CompactSize + 16);
				Data = new std::vector<std::byte>();
				Encoding<EncodingScheme::Shrinker>::Decode(*Data, compressedData);
			}

			void EnsureUnloaded()
			{
				SafeDelete(Data);
			}

			CABlock(File & file, uint64_t fileOffset)
				: m_file(file)
				, FileOffset(fileOffset)
			{
				ReadInfo();
			}

			/// Used when appending new blocks, avoids an unnecessary file read
			CABlock(File & file, uint64_t fileOffset, uint64_t payloadSize, uint64_t compactSize)
				: m_file(file)
				, FileOffset(fileOffset)
				, PayloadSize(payloadSize)
				, CompactSize(compactSize)
			{ }

			~CABlock() { EnsureUnloaded(); }
		};

	public:
		static constexpr uint64_t BlockSize       = detail::BLOCK_SIZE;
		//static constexpr uint64_t MaxBlocksCached = 64;
		static char const *       PartialFileExt() { return "-tl"; }

		COPY_ASSIGN_MOVE_CTOR(CompressedAppendFile, delete, delete, delete);

	private:
		std::filesystem::path m_path;
				
		File m_file;
		bool m_isOpen    = false;
		bool m_isFlushed = true;

		//htd::deque <CABlock*> m_cachedBlocks;
		std::vector<CABlock*> m_blocks;

		CABlock * m_lastCachedBlock = nullptr;

		uint64_t m_fileSize  = 0; /// Does NOT include m_partial, and is compressed!

		uint64_t m_position = 0; /// Read position

		std::vector<std::byte> m_partialData;

		/// Assumes m_partialData is EMPTY, or IS the data param
		void appendBasic(void * data, uint64_t size)
		{
			std::byte * dataStart = (std::byte *) data;
			std::byte * dataCurr  = dataStart;
			std::byte * dataEnd   = dataStart + size;

			struct BlockInfo
			{
				std::byte *          Src = nullptr;
				std::vector<uint8_t> CompressedData;

				BlockInfo() : CompressedData(BlockSize) { }
			};

			std::vector<BlockInfo> tbs;

			m_file.SeekEnd(AccessPosition::Write);

			while ((dataEnd - dataCurr) >= (ptrdiff_t)BlockSize)
			{
				auto & tb = tbs.emplace_back();
				tb.Src = (std::byte *)dataCurr;
				dataCurr += BlockSize;
			}
			
			Handy::WorkPool<BlockInfo *> wp;
			wp.SetThreadNames("Handy::CompressedAppendFile");
			wp.SetTask([](BlockInfo * tb)
			{
				/// This automatically adds size info at the front of the block!
				Encoding<EncodingScheme::Shrinker>::Encode(tb->CompressedData, std::span<std::byte>(tb->Src, BlockSize));
			});
			
			for (auto & tb : tbs)
				wp.AddJob(&tb);

			wp.Wait();

			for (auto & tb : tbs)
			{
				m_file.Write(&tb.CompressedData[0], tb.CompressedData.size());
				std::array<uint64_t, 2> sizes;
				memcpy(&sizes[0], &tb.CompressedData[0], 8 * 2);
				CABlock * nuBlock = new CABlock(m_file, m_fileSize, sizes[1], sizes[0]); /// Prespecify these values so we don't have to reread them from disk.
				m_blocks.push_back(nuBlock);
				m_fileSize += tb.CompressedData.size();
			}

			m_file.Flush();

			std::vector<std::byte> nuPData(dataCurr, dataEnd);
			m_partialData.clear();
			std::swap(nuPData, m_partialData);
		}


	public:
		bool IsOpen() { return m_isOpen; }

		bool Flush()
		{
			// if (m_partialData.size() > 0)
			if (!m_isFlushed)
			{
				auto ppath = m_path;
				ppath += PartialFileExt();
				if (TryWriteFile(ppath, m_partialData))
				{
					m_isFlushed = true;
					//m_partialData.clear();
					return true;
				}
				else
				{
					std::filesystem::remove(ppath);
					return false;
				}
			}

			return true;
		}

		void Close()
		{
			Flush();

			m_file.Close();
			//m_fileP.Close();
			m_isOpen = false;
			m_path = std::filesystem::path();
			m_isFlushed = true;
			//m_pathP = std::filesystem::path();

			for (auto iP : m_blocks)
				delete iP;

			m_blocks.clear();
		}

		 CompressedAppendFile() = default;
		~CompressedAppendFile() { Close(); }

		bool Open(std::filesystem::path path, bool resetFile = false)
		{
			Close();

			m_path = path;

			if (!m_file.Open(m_path, resetFile))        { Close(); return false; }
			if (!m_file.SeekEnd(AccessPosition::Write)) { Close(); return false; }

			m_fileSize = m_file.Tell(AccessPosition::Write);

			if (m_fileSize < 0) { Close(); return false; }

			/// Read the CABlocks:
			uint64_t position = 0;
			while (position < m_fileSize)
			{
				CABlock * nuBlock = new CABlock(m_file, position);
				m_blocks.push_back(nuBlock);
				position = nuBlock->NextBlockFileOffset();
			}
			
			m_isOpen = true;
			
			{
				auto ppath = m_path;
				ppath += PartialFileExt();
				if (resetFile)
				{
					std::error_code ignored;
					std::filesystem::remove(ppath, ignored);
					//TryWriteFile(ppath, std::vector<std::byte>());
				}
				else
					TryReadFile(ppath, m_partialData);
			}

			return true;
		}

		/// Append to the compressed file. Does NOT affect the Seeking position.
		void Append(void * data, uint64_t size)
		{
			m_isFlushed = false;

			int64_t oldPSize = (int64_t)m_partialData.size();

			//std::cout << "Append: " << data << " " << size << std::endl;

			if ((oldPSize + size) < BlockSize)
			{
				m_partialData.resize(m_partialData.size() + size);
				memcpy(&m_partialData[oldPSize], data, size);
			}
			else if (oldPSize == 0)
			{
				appendBasic(data, size);
			}
			else
			{
				int64_t difference = (int64_t)BlockSize - oldPSize;
				//std::cout << "    Size: " << size << " Diff: " << difference << "oldPSize: " << oldPSize;

				m_partialData.resize(BlockSize);
				memcpy(&m_partialData[oldPSize], data, difference);

				appendBasic(&m_partialData[0], BlockSize);
				//std::cout << "Done1 " << m_partialData.size();

				appendBasic(((std::byte *)data) + difference, size - difference);
				//std::cout << " Done2" << m_partialData.size() << std::endl;
			}

			//std::cout << "     Final m_partialData size: " << m_partialData.size() << std::endl;
		}

		uint64_t SizeBytes()           { return m_blocks.size() * BlockSize + (uint64_t)m_partialData.size(); }
		uint64_t SizeBytesCompressed()
		{
			uint64_t sbc = 0;
			for (CABlock * cab : m_blocks)
				sbc += cab->CompactSize;

			sbc += m_partialData.size();
			return sbc;
		}

		bool Seek       (int64_t offset            = 0) { m_position =  offset;                      return true; }
		bool SeekCurrent(int64_t offsetFromCurrent = 0) { m_position += offsetFromCurrent;           return true; }
		bool SeekEnd    (int64_t reverseOffset     = 0) { m_position =  SizeBytes() - reverseOffset; return true; }

		uint64_t ReadUpTo(void * dst, uint64_t numBytes)
		{
			uint64_t bytesWritten = 0;

			std::byte * dstCurr = (std::byte *)dst;

			while (numBytes > 0)
			{
				if (m_position > SizeBytes())
					return bytesWritten;

				uint64_t bIndex  = m_position / BlockSize;
				uint64_t bOffset = m_position % BlockSize;

				uint64_t numBlockBytesToCopy = 0;

				if (bIndex < m_blocks.size())
				{
					numBlockBytesToCopy = FastMin(numBytes, BlockSize - bOffset);

					CABlock * block = m_blocks.at(bIndex);
					if (block != m_lastCachedBlock)
					{
						if (m_lastCachedBlock)
							m_lastCachedBlock->EnsureUnloaded();
						block->EnsureLoaded();
						m_lastCachedBlock = block;
					}

					if (numBlockBytesToCopy > 0)
						memcpy(dstCurr, ((std::byte *)block->Data->data()) + bOffset, numBlockBytesToCopy);

				}
				else if (bIndex == m_blocks.size()) // Data needs to come out of m_partialData
				{
					if (bOffset >= m_partialData.size()) 
						return bytesWritten; // This should always be zero.

					numBlockBytesToCopy = FastMin(numBytes, m_partialData.size() - bOffset);
					memcpy(dstCurr, ((std::byte *)m_partialData.data()) + bOffset, numBlockBytesToCopy);
				}
				else
				{
					throw std::runtime_error("CompressedAppendFile - This line should NEVER be hit.");
					return bytesWritten;
				}

				numBytes     -= numBlockBytesToCopy;
				m_position   += numBlockBytesToCopy;
				dstCurr      += numBlockBytesToCopy;
				bytesWritten += numBlockBytesToCopy;
			}

			return bytesWritten;
		}

		bool Read(void * dst, uint64_t numBytes)
		{
			return ReadUpTo(dst, numBytes) == numBytes;
		}

		template <typename TDst>
		bool ReadTo(TDst & dDst, size_t numBytes)
		{
			Deserializer<TDst> dstDeser;
			uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(numBytes, dDst));

			if (!dst)
				return false;

			return Read(dst, numBytes);
		}

		template <typename TSrc>
		void Append(TSrc & dSrc)
		{
			Serializer<TSrc>           srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const *            src = reinterpret_cast<uint8_t const *>(srcSpan.data());
			Append((void *)src, (uint64_t)srcSpan.size());
		}
	};

	#endif
}

