
/// =============================================================================
///	https://github.com/stbrumme/hash-library
///
/// LICENSE: ZLIB
/// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
///
/// This software is provided 'as-is', without any express or implied
/// warranty.  In no event will the authors be held liable for any damages
/// arising from the use of this software.
/// 
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
/// 
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software
///    in a product, an acknowledgment in the product documentation would be
///    appreciated but is not required.
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
/// 3. This notice may not be removed or altered from any source distribution.
/// =============================================================================

/// =============================================================================
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
/// =============================================================================

#pragma once

#include <string>

#include "HandyBase.hpp"
#include "HandyCompat.hpp"
#include "HandySerDe.hpp"
#include "HandyEncoding.hpp"

namespace HANDY_NS::Hash {


	/// algorithm variants
	enum class SHA3Bits : uint32_t
	{ 
		Bits224 = 224, 
		Bits256 = 256, 
		Bits384 = 384, 
		Bits512 = 512
	};

	template <SHA3Bits NUMBITS = SHA3Bits::Bits512>
	struct SHA3
	{
	private:
		static constexpr uint32_t NumBitsU32 = (uint32_t)NUMBITS;
		static constexpr uint32_t Rounds = 24;
		static constexpr uint64_t XorMasks[Rounds] =
		{
			0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
			0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
			0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
			0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
			0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
			0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
			0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
			0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
		};

		/// rotate left and wrap around to the right
		FORCEINLINE static uint64_t rotateLeft(uint64_t x, uint8_t numBits) { return (x << numBits) | (x >> (64 - numBits)); }

		/// return x % 5 for 0 <= x <= 9
		FORCEINLINE static uint32_t mod5(uint32_t x) { if (x < 5_u32) return x; return x - 5_u32; }

		/// 1600 bits, stored as 25x64 bit, BlockSize is no more than 1152 bits (Keccak224)
		enum {    StateSize = 1600 / (8 * 8),
			   MaxBlockSize =  200 - 2 * (224 / 8) };

		uint64_t m_hash[StateSize];      // hash
		uint64_t m_numBytes;             // size of processed data in bytes
		size_t   m_blockSize;            // block size (less or equal to MaxBlockSize)
		size_t   m_bufferSize;           // valid bytes in m_buffer
		uint8_t  m_buffer[MaxBlockSize]; // bytes not processed yet

		/// process a full block
		void processBlock(void const * data)
		{
			const uint64_t* data64 = (const uint64_t*) data;
			
			for (uint32_t i = 0; i < m_blockSize / 8; i++) // mix data into state
			{
				m_hash[i] ^= bswapi_b(data64[i]);
			}

			// re-compute state
			for (uint32_t round = 0; round < Rounds; round++)
			{
				// Theta
				uint64_t coefficients[5];
				for (uint32_t i = 0; i < 5; i++)
					coefficients[i] = m_hash[i] ^ m_hash[i + 5] ^ m_hash[i + 10] ^ m_hash[i + 15] ^ m_hash[i + 20];

				for (uint32_t i = 0; i < 5; i++)
				{
					uint64_t one = coefficients[mod5(i + 4)] ^ rotateLeft(coefficients[mod5(i + 1)], 1);
					m_hash[i     ] ^= one;
					m_hash[i +  5] ^= one;
					m_hash[i + 10] ^= one;
					m_hash[i + 15] ^= one;
					m_hash[i + 20] ^= one;
				}

				uint64_t one; // temporary

				// Rho Pi
				uint64_t last = m_hash[1];
				one = m_hash[10]; m_hash[10] = rotateLeft(last,  1); last = one;
				one = m_hash[ 7]; m_hash[ 7] = rotateLeft(last,  3); last = one;
				one = m_hash[11]; m_hash[11] = rotateLeft(last,  6); last = one;
				one = m_hash[17]; m_hash[17] = rotateLeft(last, 10); last = one;
				one = m_hash[18]; m_hash[18] = rotateLeft(last, 15); last = one;
				one = m_hash[ 3]; m_hash[ 3] = rotateLeft(last, 21); last = one;
				one = m_hash[ 5]; m_hash[ 5] = rotateLeft(last, 28); last = one;
				one = m_hash[16]; m_hash[16] = rotateLeft(last, 36); last = one;
				one = m_hash[ 8]; m_hash[ 8] = rotateLeft(last, 45); last = one;
				one = m_hash[21]; m_hash[21] = rotateLeft(last, 55); last = one;
				one = m_hash[24]; m_hash[24] = rotateLeft(last,  2); last = one;
				one = m_hash[ 4]; m_hash[ 4] = rotateLeft(last, 14); last = one;
				one = m_hash[15]; m_hash[15] = rotateLeft(last, 27); last = one;
				one = m_hash[23]; m_hash[23] = rotateLeft(last, 41); last = one;
				one = m_hash[19]; m_hash[19] = rotateLeft(last, 56); last = one;
				one = m_hash[13]; m_hash[13] = rotateLeft(last,  8); last = one;
				one = m_hash[12]; m_hash[12] = rotateLeft(last, 25); last = one;
				one = m_hash[ 2]; m_hash[ 2] = rotateLeft(last, 43); last = one;
				one = m_hash[20]; m_hash[20] = rotateLeft(last, 62); last = one;
				one = m_hash[14]; m_hash[14] = rotateLeft(last, 18); last = one;
				one = m_hash[22]; m_hash[22] = rotateLeft(last, 39); last = one;
				one = m_hash[ 9]; m_hash[ 9] = rotateLeft(last, 61); last = one;
				one = m_hash[ 6]; m_hash[ 6] = rotateLeft(last, 20); last = one;
				m_hash[ 1] = rotateLeft(last, 44);

				// Chi
				for (uint32_t j = 0; j < 25; j += 5)
				{
					// temporaries
					uint64_t one = m_hash[j];
					uint64_t two = m_hash[j + 1];

					m_hash[j]     ^= m_hash[j + 2] & ~two;
					m_hash[j + 1] ^= m_hash[j + 3] & ~m_hash[j + 2];
					m_hash[j + 2] ^= m_hash[j + 4] & ~m_hash[j + 3];
					m_hash[j + 3] ^=      one      & ~m_hash[j + 4];
					m_hash[j + 4] ^=      two      & ~one;
				}

				// Iota
				m_hash[0] ^= XorMasks[round];
			}
		}

		/// process everything left in the internal buffer
		void processBuffer()
		{
			size_t offset = m_bufferSize; // add padding
			m_buffer[offset++] = 0x06; // add a "1" byte
			while (offset < m_blockSize) // fill with zeros
				m_buffer[offset++] = 0;
			
			m_buffer[offset - 1] |= 0x80; // and add a single set bit

			processBlock(m_buffer);
		}




	public:

		void Reset()
		{
			for (size_t i = 0; i < StateSize; i++)
				m_hash[i] = 0;

			m_numBytes   = 0;
			m_bufferSize = 0;
		}

		SHA3() : m_blockSize(200 - 2 * ((uint32_t)NumBitsU32 / 8))
		{
			Reset();
		}

		void Add(const void* data, size_t numBytes)
		{
			const uint8_t* current = (const uint8_t*) data;

			// copy data to buffer
			if (m_bufferSize > 0)
			{
				while (numBytes > 0 && m_bufferSize < m_blockSize)
				{
					m_buffer[m_bufferSize++] = *current++;
					numBytes--;
				}
			}

			// full buffer
			if (m_bufferSize == m_blockSize)
			{
				processBlock((void*)m_buffer);
				m_numBytes  += m_blockSize;
				m_bufferSize = 0;
			}

			// no more data ?
			if (numBytes == 0)
				return;

			// process full blocks
			while (numBytes >= m_blockSize)
			{
				processBlock(current);
				current    += m_blockSize;
				m_numBytes += m_blockSize;
				numBytes   -= m_blockSize;
			}

			// keep remaining bytes in buffer
			while (numBytes > 0)
			{
				m_buffer[m_bufferSize++] = *current++;
				numBytes--;
			}
		}

		template <typename TSrc>
		bool Add(TSrc const & dSrc)
		{
			Serializer<TSrc> src;
			auto srcSpan = src.get_span(dSrc);
			Add(srcSpan.data(), srcSpan.size());

			return true;
		}

		size_t SizeBytes() const { return NumBitsU32/8; } 

		/// return latest hash as 16 hex characters
		std::array<uint8_t, NumBitsU32/8> Get()
		{
			processBuffer(); // process remaining bytes
			uint32_t hashLength = NumBitsU32 / 64; // number of significant elements in hash (uint64_t)

			std::array<uint8_t, NumBitsU32 / 8> result;

			int nthByte = 0;
			for (uint32_t i = 0; i < hashLength; i++)
				for (uint32_t j = 0; j < 8; j++) // 64 bits => 8 bytes
					result[nthByte++] = (uint8_t)(m_hash[i] >> (8 * j));

			// SHA3-224's last entry in m_hash provides only 32 bits instead of 64 bits
			uint32_t remainder = NumBitsU32 - hashLength * 64;
			uint32_t processed = 0;
			while (processed < remainder)
			{
				result[nthByte++] = (uint8_t)(m_hash[hashLength] >> processed);
				processed += 8;
			}

			return result;
		}

		template <typename TDst>
		bool Get(TDst & dDst)
		{
			Deserializer<TDst> dser;
			void * p = dser.prepare_span(SizeBytes(), dDst);
			if (!p)
				return false;

			auto result = Get();
			memcpy(p, &result[0], SizeBytes());

			return true;
		}
	};


	struct XXHash32
	{
	private:
		/// magic constants :-)
		static constexpr uint32_t Prime1 = 2654435761U;
		static constexpr uint32_t Prime2 = 2246822519U;
		static constexpr uint32_t Prime3 = 3266489917U;
		static constexpr uint32_t Prime4 =  668265263U;
		static constexpr uint32_t Prime5 =  374761393U;

		/// temporarily store up to 15 bytes between multiple add() calls
		static const uint32_t MaxBufferSize = 15 + 1;

		// internal state and temporary buffer
		uint32_t state[4]; // state[2] == seed if totalLength < MaxBufferSize
		uint8_t  buffer[MaxBufferSize];
		uint32_t bufferSize;
		uint64_t totalLength;

		/// rotate bits, should compile to a single CPU instruction (ROL)
		FORCEINLINE static uint32_t rotateLeft(uint32_t x, uint8_t bits)
		{
			return (x << bits) | (x >> (32 - bits));
		}

		/// process a block of 4x4 bytes, this is the main part of the XXHash32 algorithm
		FORCEINLINE static void process(const void* data, uint32_t& state0, uint32_t& state1, uint32_t& state2, uint32_t& state3)
		{
			const uint32_t* block = (const uint32_t*)data;
			state0 = rotateLeft(state0 + block[0] * Prime2, 13) * Prime1;
			state1 = rotateLeft(state1 + block[1] * Prime2, 13) * Prime1;
			state2 = rotateLeft(state2 + block[2] * Prime2, 13) * Prime1;
			state3 = rotateLeft(state3 + block[3] * Prime2, 13) * Prime1;
		}

		// get current hash
		uint32_t get() const
		{
			uint32_t result = (uint32_t)totalLength;

			// fold 128 bit state into one single 32 bit value
			if (totalLength >= MaxBufferSize)
				result += rotateLeft(state[0], 1) +
				rotateLeft(state[1], 7) +
				rotateLeft(state[2], 12) +
				rotateLeft(state[3], 18);
			else
				// internal state wasn't set in add(), therefore original seed is still stored in state2
				result += state[2] + Prime5;

			// process remaining bytes in temporary buffer
			uint8_t const * data = buffer;
			// point beyond last byte
			uint8_t const * stop = data + bufferSize;

			// at least 4 bytes left ? => eat 4 bytes per step
			for (; data + 4 <= stop; data += 4)
				result = rotateLeft(result + *(uint32_t*)data * Prime3, 17) * Prime4;

			// take care of remaining 0..3 bytes, eat 1 byte per step
			while (data != stop)
				result = rotateLeft(result + (*data++) * Prime5, 11) * Prime1;

			// mix bits
			result ^= result >> 15;
			result *= Prime2;
			result ^= result >> 13;
			result *= Prime3;
			result ^= result >> 16;
			return result;
		}

	public:

		void Reset(uint32_t seed = 0)
		{
			state[0] = seed + Prime1 + Prime2;
			state[1] = seed + Prime2;
			state[2] = seed;
			state[3] = seed - Prime1;
			bufferSize = 0;
			totalLength = 0;
		}

		explicit XXHash32(uint32_t seed = 0) { Reset(seed); }

		//@param  input  pointer to a continuous block of data
		//@param  length number of bytes
		//@return false if parameters are invalid / zero
		bool Add(void const * input, uint64_t length)
		{
			// no data ?
			if (!input || length == 0)
				return false;

			totalLength += length;
			// byte-wise access
			uint8_t const * data = (uint8_t const *)input;

			// unprocessed old data plus new data still fit in temporary buffer ?
			if (bufferSize + length < MaxBufferSize)
			{
				// just add new data
				while (length-- > 0)
					buffer[bufferSize++] = *data++;
				return true;
			}

			// point beyond last byte
			uint8_t const * stop = data + length;
			uint8_t const * stopBlock = stop - MaxBufferSize;

			// some data left from previous update ?
			if (bufferSize > 0)
			{
				// make sure temporary buffer is full (16 bytes)
				while (bufferSize < MaxBufferSize)
					buffer[bufferSize++] = *data++;

				// process these 16 bytes (4x4)
				process(buffer, state[0], state[1], state[2], state[3]);
			}

			// copying state to local variables helps optimizer A LOT
			uint32_t s0 = state[0], s1 = state[1], s2 = state[2], s3 = state[3];
			// 16 bytes at once
			while (data <= stopBlock)
			{
				// local variables s0..s3 instead of state[0]..state[3] are much faster
				process(data, s0, s1, s2, s3);
				data += 16;
			}
			// copy back
			state[0] = s0; state[1] = s1; state[2] = s2; state[3] = s3;

			// copy remainder to temporary buffer
			bufferSize = (uint32_t)(stop - data);
			for (uint32_t i = 0; i < bufferSize; i++)
				buffer[i] = data[i];

			// done
			return true;
		}

		template <typename TSrc>
		bool Add(TSrc const & dSrc)
		{
			Serializer<TSrc> src;
			auto srcSpan = src.get_span(dSrc);
			Add(srcSpan.data(), srcSpan.size());

			return true;
		}

		template <typename TDst>
		bool Get(TDst & dDst) const
		{
			Deserializer<TDst> dser;
			uint32_t * p = reinterpret_cast<uint32_t *>(dser.prepare_span(4, dDst));
			if (!p)
				return false;
			*p = get();
			return true;
		}

		uint32_t Get() const { return get(); }

		constexpr size_t SizeBytes() { return 4; }
	};

	struct XXHash64
	{
	private:
		/// magic constants :-)
		static constexpr uint64_t Prime1 = 11400714785074694791ULL;
		static constexpr uint64_t Prime2 = 14029467366897019727ULL;
		static constexpr uint64_t Prime3 =  1609587929392839161ULL;
		static constexpr uint64_t Prime4 =  9650029242287828579ULL;
		static constexpr uint64_t Prime5 =  2870177450012600261ULL;

		/// temporarily store up to 31 bytes between multiple add() calls
		static constexpr uint64_t MaxBufferSize = 31 + 1;

		/// rotate bits, should compile to a single CPU instruction (ROL)
		FORCEINLINE static uint64_t rotateLeft(uint64_t x, uint8_t bits) { return (x << bits) | (x >> (64 - bits)); }

		/// process a single 64 bit value
		FORCEINLINE static uint64_t processSingle(uint64_t previous, uint64_t input) { return rotateLeft(previous + input * Prime2, 31) * Prime1; }

		/// process a block of 4x4 bytes, this is the main part of the XXHash32 algorithm
		FORCEINLINE static void process(const void* data, uint64_t& state0, uint64_t& state1, uint64_t& state2, uint64_t& state3)
		{
			const uint64_t* block = (const uint64_t*)data;
			state0 = processSingle(state0, block[0]);
			state1 = processSingle(state1, block[1]);
			state2 = processSingle(state2, block[2]);
			state3 = processSingle(state3, block[3]);
		}

		uint64_t state[4];
		uint8_t  buffer[MaxBufferSize];
		uint32_t bufferSize;
		uint64_t totalLength;

		// get current hash
		uint64_t get() const
		{
			// fold 256 bit state into one single 64 bit value
			uint64_t result;
			if (totalLength >= MaxBufferSize)
			{
				result = rotateLeft(state[0], 1) +
					rotateLeft(state[1], 7) +
					rotateLeft(state[2], 12) +
					rotateLeft(state[3], 18);
				result = (result ^ processSingle(0, state[0])) * Prime1 + Prime4;
				result = (result ^ processSingle(0, state[1])) * Prime1 + Prime4;
				result = (result ^ processSingle(0, state[2])) * Prime1 + Prime4;
				result = (result ^ processSingle(0, state[3])) * Prime1 + Prime4;
			}
			else
			{
				// internal state wasn't set in add(), therefore original seed is still stored in state2
				result = state[2] + Prime5;
			}

			result += totalLength;

			// process remaining bytes in temporary buffer
			uint8_t const * data = buffer;
			// point beyond last byte
			uint8_t const * stop = data + bufferSize;

			// at least 8 bytes left ? => eat 8 bytes per step
			for (; data + 8 <= stop; data += 8)
				result = rotateLeft(result ^ processSingle(0, *(uint64_t*)data), 27) * Prime1 + Prime4;

			// 4 bytes left ? => eat those
			if (data + 4 <= stop)
			{
				result = rotateLeft(result ^ (*(uint32_t*)data) * Prime1, 23) * Prime2 + Prime3;
				data += 4;
			}

			// take care of remaining 0..3 bytes, eat 1 byte per step
			while (data != stop)
				result = rotateLeft(result ^ (*data++) * Prime5, 11) * Prime1;

			// mix bits
			result ^= result >> 33;
			result *= Prime2;
			result ^= result >> 29;
			result *= Prime3;
			result ^= result >> 32;
			return result;
		}

	public:

		void Reset(uint64_t seed = 0)
		{
			state[0] = seed + Prime1 + Prime2;
			state[1] = seed + Prime2;
			state[2] = seed;
			state[3] = seed - Prime1;
			bufferSize = 0;
			totalLength = 0;
		}

		explicit XXHash64(uint64_t seed = 0) { Reset(seed); }

		//@param  input  pointer to a continuous block of data
		//@param  length number of bytes
		//@return false if parameters are invalid / zero
		bool Add(void const * input, uint64_t length)
		{
			// no data ?
			if (!input || length == 0)
				return false;

			totalLength += length;
			// byte-wise access
			uint8_t const * data = (uint8_t const *)input;

			// unprocessed old data plus new data still fit in temporary buffer ?
			if (bufferSize + length < MaxBufferSize)
			{
				// just add new data
				while (length-- > 0)
					buffer[bufferSize++] = *data++;
				return true;
			}

			// point beyond last byte
			uint8_t const * stop = data + length;
			uint8_t const * stopBlock = stop - MaxBufferSize;

			// some data left from previous update ?
			if (bufferSize > 0)
			{
				// make sure temporary buffer is full (16 bytes)
				while (bufferSize < MaxBufferSize)
					buffer[bufferSize++] = *data++;

				// process these 32 bytes (4x8)
				process(buffer, state[0], state[1], state[2], state[3]);
			}

			// copying state to local variables helps optimizer A LOT
			uint64_t s0 = state[0], s1 = state[1], s2 = state[2], s3 = state[3];
			// 32 bytes at once
			while (data <= stopBlock)
			{
				// local variables s0..s3 instead of state[0]..state[3] are much faster
				process(data, s0, s1, s2, s3);
				data += 32;
			}
			// copy back
			state[0] = s0; state[1] = s1; state[2] = s2; state[3] = s3;

			// copy remainder to temporary buffer
			bufferSize = (uint32_t)(stop - data);
			for (uint32_t i = 0; i < bufferSize; i++)
				buffer[i] = data[i];

			// done
			return true;
		}

		template <typename TSrc>
		bool Add(TSrc const & dSrc)
		{
			Serializer<TSrc> src;
			auto srcSpan = src.get_span(dSrc);
			Add(srcSpan.data(), srcSpan.size());

			return true;
		}

		template <typename TDst>
		bool Get(TDst & dDst) const
		{
			Deserializer<TDst> dser;
			uint64_t * p = reinterpret_cast<uint64_t *>(dser.prepare_span(8, dDst));

			if (!p)
				return false;

			*p = get();
		}

		uint64_t Get() const { return get(); }

		constexpr size_t SizeBytes() { return 8; }
	};
}
