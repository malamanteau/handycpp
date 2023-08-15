
/// =============================================================================
///	Base64, Base64URL, Base85_Z85, Base91_Hencke, and Base91_Rlyeh 
///
/// LICENSE: ZLIB
/// Copyright (c) 2011..2016 r-lyeh (https://github.com/r-lyeh)
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
/// COBS (https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing)
/// fast-hex (https://github.com/zbjornson/fast-hex)
/// 
/// LICENSE: MIT
/// 
/// Copyright (c) 2010-2016 Christopher Baker <http://christopherbaker.net>
/// Copyright (c) 2011, Jacques Fortier. All rights reserved. 
/// Copyright (c) 2017 Zach Bjornson
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// =============================================================================

/// =============================================================================
///	Shrinker (https://code.google.com/archive/p/data-shrinker/), 
/// Base91_Hencke (http://base91.sourceforge.net/)
///
/// LICENSE: BSD 3-Clause License
/// 
/// Copyright (c) 2012, Siyuan Fu (fusiyuan2010@gmail.com)
/// Copyright (c) 2000-2006 Joachim Henke http://base91.sourceforge.net/
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
#include <cstring>
#include <iostream>

#include "HandyBase.hpp"
#include "HandyCompat.hpp"
#include "HandySerDe.hpp"

namespace HANDY_NS {

	// Recommendations: 
	//     BaseNM encodings:
	//         Use either Base64URL or Base91_Rlyeh
	//         If you need to be interoperable, use Base64 (with or without padding)
	// 
	//     For Compression, use Shrinker.
	//
	enum class EncodingScheme
	{
		// Simple copy, no encoding actually done.
		Raw,

		// Standard Hex representation, 0-9, A-F.
		// https://github.com/zbjornson/fast-hex
		//   URL-safe: Y
		//   Quotable: Y
		//   JSON, XML, and TSV: Y
		//   Characters: 0123456789ABCDEF
		//   Note: This is a scalar only implementation. If doing HUGE volumes for encoding/decoding, use 
		//         the vectorized version in the fast-hex github repo.
		Hex,

		// Standard Hex representation, 0-9, A-F, with dashes at normal intervals for Guid/UUIDs
		// Same as above, just formatted with dashes.
		// Note: Decode does NOT depend on the dashes being there. It actually just filters out 
		// all non-hex characters, at which point it better be the correct number of digits.
		HexGuid,

		// Base64: Canonical implementation, defined by rfc4648
		// https://tools.ietf.org/html/rfc4648#page-5
		//   URL-safe: N
		//   Quotable: Y
		//   JSON, XML, and TSV: Y
		//   Characters: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ and = (for padding)
		Base64,       

		// Base64: URL-safe implementation, defined by rfc4648
		// https://tools.ietf.org/html/rfc4648#page-10
		//   URL-safe: Y
		//   Quotable: Y
		//   JSON, XML, and TSV: Y
		//   Characters: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_ and = (for padding)
		Base64URL,    

		// Base85: ZeroMQ version
		// https://en.wikipedia.org/wiki/Ascii85#ZeroMQ_Version_(Z85)
		//   17% to 08% smaller than Base64.
		//   URL-safe: N
		//   Quotable: Y
		//   JSON, XML, and TSV: N
		//   Characters: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-:+=^!/*?&<>()[]{}@%$#
		Base85_Z85,   

		// Base91_Hencke: Original spec for Base91, not safe for schemaless formats!
		// http://base91.sourceforge.net/
		//   19% to 10% smaller than Base64
		//   URL-safe: N
		//   Quotable: N
		//   JSON, XML, and TSV: N
		//   Characters: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;<=>?@[]^_`{|}"~
		Base91_Hencke,

		// Base91_Rlyeh: Rlyeh's alternate charset version of Hencke's, safer!
		// https://github.com/r-lyeh-archived/base
		//   19% to 10% smaller than Base64
		//   URL-safe: N
		//   Quotable: Y
		//   JSON, XML, and TSV: Y (some characters still need to be escaped, I believe)
		//   Characters: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;-=\?@[]^_`{|}\~ 
		Base91_Rlyeh,
		
		// COBS: Consistent Overhead Byte Stuffing
		// https://github.com/anome/millumin-dev-kit/blob/master/Arduino/_Library/PacketSerial/src/Encoding/COBS.h
		// http://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
		// This is an encoding that removes all 0
		// bytes from arbitrary binary data. The encoded data consists only of bytes
		// with values from 0x01 to 0xFF. This is useful for preparing data for
		// transmission over a serial link (RS-232 or RS-485 for example), as the 0
		// byte can be used to unambiguously indicate packet boundaries. COBS also has
		// the advantage of adding very little overhead (at least 1 byte, plus up to an
		// additional byte per 254 bytes of data). For messages smaller than 254 bytes,
		// the overhead is constant.
		COBS,




		/// --- COMPRESSION ---

		// Shrinker Compression
		// https://code.google.com/archive/p/data-shrinker/
		//   An LZ77-based data compression program that can be used in high performance demand environment, it
		//   can remove much of the redundancy in data at the speed of hundreds of megabytes per second.
		Shrinker
	};

	/// My test file (some json data, 711MB):
	//                       Compression Time   Decompression Time   Compression Ratio
	//             Shrinker         1.55s             0.64s                0.15
	// 7z Fast (to compare)         5.27s            ~1.75s                0.08
	// 7z Norm (to compare)        39.00s            ~1.75s                0.07

	template<EncodingScheme SCHEME>
	struct Encoding { };

	namespace detail {

		#ifdef _MSC_VER
			#define _CRT_SECECURE_NO_WARNINGS
			#define _CRT_DISABLE_PERFCRIT_LOCKS
		#endif

		static constexpr uint32_t BLOCK_SIZE = (uint32_t)1_MiB;

		inline size_t GetTotalUncompressedSize(uint8_t const * src, uint8_t const * srcEnd)
		{
			size_t size = 0;

			while (src < srcEnd)
			{
				uint64_t const * compressedSize   = (uint64_t *)src;
				uint64_t const * uncompressedSize = (compressedSize   + 1);

				size += *uncompressedSize;
				src += *compressedSize + 16;
			}

			return size;
		}

	}


	namespace detail {
	

		#define HASH_BITS 15
		#define HASH(a) ((a*21788233) >> (32 - HASH_BITS))
		#define MINMATCH 4

		#define MEMCPY_NOOVERLAP(a, b, c) do { do { memcpy(a, b, 8); a+=8; b+=8; } while (b < c); a-=(b-c); b=c; } while(0)
		#define MEMCPY_NOOVERLAP_NOSURPASS(a, b, c) do { c-=8; while(b<c) { memcpy(a, b, 8); a+=8; b+=8; } c+=8; while (b<c) *a++=*b++; } while(0)
		#define MEMCPY(a, b, c) do { if (LIKELY(a>b+8)) MEMCPY_NOOVERLAP(a, b, c); else while (b<c) *a++=*b++; } while(0)

		FORCEINLINE uint16_t unaligned_load_u16(void* ptr)
		{
			uint16_t tempU16;
			memcpy(&tempU16, ptr, 2);
			return tempU16;
		}

		FORCEINLINE uint32_t unaligned_load_u32(void* ptr)
		{
			uint32_t tempU32;
			memcpy(&tempU32, ptr, 4);
			return tempU32;
		}

		// in:     inbuf --- source data
		// out:    outbuf --- compressed data to place in
		// size:   inbuf size
		// ******* IMPORTAT *******:
		// "the outbuf's size MUST be equal or greater than that of inbuf"
		//     NOTE: THIS IS INCORRECT shrinker_compress can write PAST (out + size).
		//          You WILL foul the heap if you don't have an extra buffer.
		//              The Shrinker test app adds a buffer of (BLOCK_SIZE >> 6), so the creator 
		//              probably thought this safe, and seems to jive with my testing (at BLOCK_SIZE of 1MB).
		//              
		//
		// if size < 32 or size >= 128 MB the function will refuse to run and returns -1
		// return value:
		//     positive integer means compress success and it's the size of compressed data,
		//     or -1 means compress failed which mostly means the data is uncompressable
		inline int shrinker_compress(void *in, void *out, int size)
		{
			uint32_t * ht = (uint32_t*)malloc((1 << HASH_BITS)*4);
			uint8_t * src = (uint8_t*)in;
			uint8_t * dst = (uint8_t*)out;
			uint8_t * src_end = src + size - MINMATCH - 8;
			uint8_t * dst_end = dst + size - MINMATCH - 8;
			uint8_t * p_last_lit = src;
			uint32_t  cur_hash, len, match_dist;
			uint8_t   flag, cache;
			uint8_t * pflag;

			if (size < 32 || size > (1 << 27) - 1)
			{
				free(ht);
				return -1;
			}
			memset(ht, 0, (1 << HASH_BITS)*4);

			while(LIKELY(src < src_end) && LIKELY(dst < dst_end))
			{
				uint32_t u32val, distance = (uint32_t)(src - (uint8_t*)in);
				uint8_t *pfind, *pcur;
				pcur = src;
				u32val = unaligned_load_u32(pcur);
				cur_hash = HASH(u32val);
				cache = ht[cur_hash] >> 27;
				pfind = (uint8_t*)in + (ht[cur_hash] & 0x07ffffff);
				ht[cur_hash] = distance|(*src<<27);

				if (UNLIKELY(cache == (*pcur & 0x1f))
					&& pfind + 0xffff >= (uint8_t*)pcur
					&& pfind < pcur
					&& unaligned_load_u32(pfind) == unaligned_load_u32(pcur))
				{  
					pfind += 4; 
					pcur  += 4;
					while (LIKELY(pcur < src_end) && unaligned_load_u32(pfind) == unaligned_load_u32(pcur))
					{ 
						pfind += 4; 
						pcur += 4;
					}
					if (LIKELY(pcur < src_end))
						if (unaligned_load_u16(pfind) == unaligned_load_u16(pcur)) 
						{
							pfind += 2; 
							pcur  += 2;
						}

					if (*pfind == *pcur) 
					{
						pfind++; 
						pcur++;
					}

					pflag = dst++;
					len = (uint32_t)(src - p_last_lit);
					if (LIKELY(len < 7))
						flag = len << 5;
					else
					{
						len -= 7;flag = (7<<5);
						while (len >= 255)
						{
							*dst++ = 255;
							len-= 255;
						}
						*dst++ = len;
					}

					len = (uint32_t)(pcur - src  - MINMATCH);
					if (LIKELY(len < 15))
						flag |= len;
					else
					{
						len  -= 15; 
						flag |= 15;
						while (len >= 255)
						{
							*dst++ = 255;
							len   -= 255;
						}
						*dst++ = len;
					}
					match_dist = (uint32_t)(pcur - pfind - 1);
					*pflag = flag;
					*dst++ = match_dist & 0xff;
					if (match_dist > 0xff) 
					{
						*pflag |= 0x10;
						*dst++ = match_dist >> 8;
					}
					MEMCPY_NOOVERLAP(dst, p_last_lit, src);

					u32val = unaligned_load_u32(src+1); ht[HASH(u32val)] = (uint32_t)((src - (uint8_t*)in + 1)|(*(src+1)<<27));
					u32val = unaligned_load_u32(src+3); ht[HASH(u32val)] = (uint32_t)((src - (uint8_t*)in + 3)|(*(src+3)<<27));
					p_last_lit = src = pcur;
					continue;
				}
				src++;
			}

			if (dst - (uint8_t*)out + 3 >= src - (uint8_t*)in)
			{
				free(ht);
				return -1;
			}
			src = (uint8_t*)in + size;
			pflag = dst++;
			len = (uint32_t)(src - p_last_lit);
			if (LIKELY(len < 7)) 
				flag = len << 5;
			else 
			{
				len -= 7; flag = (7<<5);
				while (len >= 255)
				{
					*dst++ = 255; 
					len   -= 255;
				}
				*dst++ = len;
			}

			flag |= 0x10; // any number
			*pflag = flag;
			*dst++ = 0xff; *dst++ = 0xff;
			MEMCPY_NOOVERLAP_NOSURPASS(dst, p_last_lit, src);

			free(ht);
			if (dst > dst_end) 
			{
				//std::cout << "Block will not be compressed and exceeded size of source by: " << (size_t)(dst - (dst_end + MINMATCH + 8)) << std::endl;
				return -1;
			}
			else return (int)(dst - (uint8_t*)out);
		}

		//in:     inbuf --- compressed data
		//out:    outbuf --- decompressed data to place in
		//size:   decompressed(original) data size should be
		//return value:
		//     positive integer means decompress success and it's the sizeof decompressed data,
		//     which should be equal to size.
		//     or -1 means decompress failed
		inline int shrinker_decompress(void *in, void *out, int size)
		{
			uint8_t *src = (uint8_t*)in, *dst = (uint8_t*)out;
			uint8_t *end = dst + size;
			uint8_t *pcpy, *pend;
			uint8_t flag, long_dist;
			uint32_t literal_len;
			uint32_t match_len, match_dist;

			for(;;)
			{
				flag = *src++;
				literal_len = flag >> 5;
				match_len = flag & 0xf;
				long_dist = flag & 0x10;

				if (UNLIKELY(literal_len == 7))
				{
					while((flag = *src++) == 255)
						literal_len += 255;
					literal_len += flag;
				}

				if (UNLIKELY(match_len == 15))
				{
					while((flag = *src++) == 255)
						match_len += 255;
					match_len += flag;
				}

				match_dist = *src++;
				if (long_dist) 
				{
					match_dist |= ((*src++) << 8);
					if (UNLIKELY(match_dist == 0xffff))
					{
						pend = src + literal_len;
						if (UNLIKELY(dst + literal_len > end)) 
							return -1;
						MEMCPY_NOOVERLAP_NOSURPASS(dst, src, pend);
						break;
					}
				}

				pend = src + literal_len;
				if (UNLIKELY(dst + literal_len > end)) 
					return -1;
				MEMCPY_NOOVERLAP(dst, src, pend);
				pcpy = dst - match_dist - 1;
				pend = pcpy + match_len + MINMATCH;
				if (UNLIKELY(pcpy < (uint8_t*)out || dst + match_len + MINMATCH > end))
					return -1;
				MEMCPY(dst, pcpy, pend);
			}
			return (int)(dst - (uint8_t*)out);
		}

		#undef HASH_BITS
		#undef HASH
		#undef MINMATCH
		#undef MEMCPY_NOOVERLAP
		#undef MEMCPY_NOOVERLAP_NOSURPASS
		#undef MEMCPY

		/// dst must accomidate at least max size of BLOCK_SIZE bytes + 16 bytes
		inline size_t CompressBlockShrinker(void * dstIn, void const * src, size_t srcSize)
		{
			uint64_t *   compressedSize = (uint64_t *)dstIn;
			uint64_t * uncompressedSize = (compressedSize + 1);
			void *     dst = (void *)(uncompressedSize + 1);

			  *compressedSize = shrinker_compress(const_cast<void *>(src), dst, (int)srcSize);
			*uncompressedSize = srcSize;

			if (*compressedSize < 0 || *compressedSize >= srcSize)
			{
				*compressedSize = srcSize;
				memcpy(dst, src, srcSize);
			}

			return *compressedSize + 16;
		}

		/// dst must have at least BLOCK_SIZE bytes
		inline size_t DecompressBlockShrinker(void * dst, void const * srcIn)
		{
			uint64_t const * compressedSize   = (uint64_t *)srcIn;
			uint64_t const * uncompressedSize = (compressedSize   + 1);
			void const *     src =      (void *)(uncompressedSize + 1);

			if (*compressedSize == *uncompressedSize)
			{
				memcpy(dst, src, *uncompressedSize);
			}
			else
			{
				uint64_t decompressedSize = shrinker_decompress(const_cast<void *>(src), dst, (int)*uncompressedSize);

				if (decompressedSize != *uncompressedSize)
					throw std::runtime_error("Decompression Error");
			}

			return *uncompressedSize;
		}

	}

	template<>
	struct Encoding<EncodingScheme::Shrinker>
	{
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer<TSrc>           srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const *            src     = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t const *            srcEnd  = src + srcSpan.size();

			std::vector<uint8_t> outData;
			outData.resize(srcSpan.size() + 16 * (1 + (srcSpan.size() / detail::BLOCK_SIZE)) + (16_KiB));

			{
				uint8_t * dst = &outData[0];

				while (src < srcEnd)
				{
					uint64_t bSize = FastMin((uint32_t)(srcEnd - src), detail::BLOCK_SIZE);
					dst += detail::CompressBlockShrinker(dst, src, bSize);
					src += bSize;
				}

				outData.resize(dst - (&outData[0]));
			}

			Deserializer<TDst> dstDeser;
			uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(outData.size(), dDst));

			if (!dst)
				return false;

			memcpy(dst, &outData[0], outData.size());

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer<TSrc>           srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const *            src     = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t const *            srcEnd  = src + srcSpan.size();

			Deserializer<TDst> dstDeser;
			uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(detail::GetTotalUncompressedSize(src, srcEnd), dDst));

			{
				while (src < srcEnd)
				{
					uint64_t const * compressedSize   = (uint64_t *)src;
					uint64_t const * uncompressedSize = (compressedSize + 1);

					dst += detail::DecompressBlockShrinker(dst, src);
					src += *compressedSize + 16;
				}
			}

			return true;
		}
	};

	template<>
	struct Encoding<EncodingScheme::Raw>
	{
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer<TSrc>           srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const *            src = reinterpret_cast<uint8_t const *>(srcSpan.data());

			Deserializer<TDst> dstDeser;
			uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(srcSpan.size(), dDst));

			if (!dst)
				return false;

			memcpy(dst, src, srcSpan.size());

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer<TSrc>           srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const *            src = reinterpret_cast<uint8_t const *>(srcSpan.data());

			Deserializer<TDst> dstDeser;
			uint8_t *          dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(srcSpan.size(), dDst));

			if (!dst)
				return false;

			memcpy(dst, src, srcSpan.size());

			return true;
		}
	};

	namespace detail {

		/// Encode a byte buffer with the COBS encoder.
		/// source:      The buffer to encode.
		/// size:        The size of the buffer to encode.
		/// destination: The target buffer for the encoded bytes.
		/// returns:     The number of bytes in the encoded buffer.
		/// warning:     Destination must have a minimum capacity of (size + size / 254 + 1).
		inline size_t cobs_encode(uint8_t const * source, size_t size, uint8_t* destination)
		{
			size_t read_index  = 0;
			size_t write_index = 1;
			size_t code_index  = 0;
			uint8_t code       = 1;

			while (read_index < size)
			{
				if (source[read_index] == 0)
				{
					destination[code_index] = code;
					code = 1;
					code_index = write_index++;
					read_index++;
				}
				else
				{
					destination[write_index++] = source[read_index++];
					code++;

					if (code == 0xFF)
					{
						destination[code_index] = code;
						code = 1;
						code_index = write_index++;
					}
				}
			}

			destination[code_index] = code;

			return write_index;
		}

		/// Decode a COBS-encoded buffer.
		/// source:      The COBS-encoded buffer to decode.
		/// size:        The size of the COBS-encoded buffer.
		/// destination: The target buffer for the decoded bytes.
		/// returns:     The number of bytes in the decoded buffer.
		/// warning:     Destination must have a minimum capacity of size
		inline size_t cobs_decode(uint8_t const * source, size_t size, uint8_t* destination)
		{
			size_t read_index  = 0;
			size_t write_index = 0;

			while (read_index < size)
			{
				uint8_t code = source[read_index];

				if (read_index + code > size && code != 1)
					return 0;

				read_index++;

				for (uint8_t u = 1; u < code; u++)
					destination[write_index++] = source[read_index++];

				if (code != 0xFF && read_index != size)
					destination[write_index++] = '\0';
			}

			return write_index;
		}

		/// Get the maximum encoded buffer size needed for a given source size.
		/// sourceSize: The size of the buffer to be encoded.
		/// returns:    The maximum size required for encoded buffer.
		inline size_t cobs_getEncodedBufferMaxSize(size_t sourceSize)
		{
			return sourceSize + sourceSize / 254 + 1;
		}
	}


	template<>
	struct Encoding<EncodingScheme::COBS>
	{
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer<TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());

			std::vector<uint8_t> temp;
			temp.resize(detail::cobs_getEncodedBufferMaxSize(srcSpan.size()), 0_u8);

			size_t destSize = detail::cobs_encode(src, srcSpan.size(), &temp[0]);

			Deserializer<TDst> dstDeser;
			uint8_t * dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(destSize, dDst));

			if (!dst)
				return false;

			memcpy(dst, &temp[0], destSize);

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{

			Serializer<TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());

			std::vector<uint8_t> temp;
			temp.resize(srcSpan.size(), 0_u8);

			size_t destSize = detail::cobs_decode(src, srcSpan.size(), &temp[0]);

			Deserializer<TDst> dstDeser;
			uint8_t * dst = reinterpret_cast<uint8_t *>(dstDeser.prepare_span(destSize, dDst));

			if (!dst)
				return false;

			memcpy(dst, &temp[0], destSize);

			return true;
		}
	};


	template<>
	struct Encoding<EncodingScheme::Hex>
	{
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			Deserializer<TDst> dstDeser;

			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t       * dst = reinterpret_cast<uint8_t       *>(dstDeser.prepare_span(srcSpan.size() * 2_szt, dDst));

			if (!dst)
				return false;

			static const char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

			for (size_t i = 0; i < (size_t)srcSpan.size(); i++)
			{
				uint8_t a = src[i];
				uint8_t lo = a & 0b1111;
				uint8_t hi = a >> 4;

				*dst++ = hex_table[hi];
				*dst++ = hex_table[lo];
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			Deserializer<TDst> dstDeser;

			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			if (srcSpan.size() % 2 == 1)
				return false;

			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t       * dst = reinterpret_cast<uint8_t       *>(dstDeser.prepare_span(srcSpan.size() / 2_szt, dDst));
				
			if (!dst)
				return false;

			// ASCII -> hex value << 4 (upper nibble)
			static const uint8_t unhex_table4[256] = {
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,      0,  16,  32,  48,  64,  80,  96, 112, 128, 144, 255, 255, 255, 255, 255, 255,
				255, 160, 176, 192, 208, 224, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 160, 176, 192, 208, 224, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
			};

			// ASCII -> hex value
			static const uint8_t unhex_table[256] = {
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 255, 255, 255, 255, 255, 255,
				255, 10, 11, 12, 13, 14, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 10, 11, 12, 13, 14, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
			};

			for (size_t i = 0; i < (size_t)srcSpan.size() / 2_szt; i++)
			{
				uint8_t a = *src++;
				uint8_t b = *src++;
				a = unhex_table4[a];
				b = unhex_table[b];
				dst[i] = a | b;
			}

			return true;
		}
	};

	template<>
	struct Encoding<EncodingScheme::HexGuid>
	{
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			Deserializer<TDst> dstDeser;

			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			
			if (srcSpan.size() != 16)
				return false;
			
			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t       * dst = reinterpret_cast<uint8_t       *>(dstDeser.prepare_span(srcSpan.size() * 2_szt + 4_szt, dDst));

			if (!dst)
				return false;

			static const char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

			for (size_t i = 0; i < (size_t)srcSpan.size(); i++)
			{
				// If we've just written the 4th... etc byte, add a hyphen.
				switch (i)
				{
					case  4:
					case  6:
					case  8:
					case 10:
						*dst++ = '-';
				}

				uint8_t a = src[i];
				uint8_t lo = a & 0b1111;
				uint8_t hi = a >> 4;

				*dst++ = hex_table[hi];
				*dst++ = hex_table[lo];
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			Deserializer<TDst> dstDeser;

			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			// ASCII -> hex value
			static const uint8_t validTable[256] = {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
				0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};

			std::string filteredSrc;
			for (size_t i = 0; i < (size_t)srcSpan.size(); i++)
				if (validTable[(size_t)srcSpan.data()[i]])
					filteredSrc.push_back((char)srcSpan.data()[i]);

			srcSpan = std::span<std::byte const>((std::byte const *)&filteredSrc[0], filteredSrc.size());
			
			if (srcSpan.size() % 2 == 1)
				return false;

			uint8_t const * src = reinterpret_cast<uint8_t const *>(srcSpan.data());
			uint8_t       * dst = reinterpret_cast<uint8_t       *>(dstDeser.prepare_span(srcSpan.size() / 2_szt, dDst));

			if (!dst)
				return false;

			// ASCII -> hex value << 4 (upper nibble)
			static const uint8_t unhex_table4[256] = {
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,      0,  16,  32,  48,  64,  80,  96, 112, 128, 144, 255, 255, 255, 255, 255, 255,
				255, 160, 176, 192, 208, 224, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 160, 176, 192, 208, 224, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
			};

			// ASCII -> hex value
			static const uint8_t unhex_table[256] = {
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,     0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255,
				255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
				255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
			};

			for (size_t i = 0; i < (size_t)srcSpan.size() / 2_szt; i++)
			{
				uint8_t a = *src++;
				uint8_t b = *src++;
				a = unhex_table4[a];
				b = unhex_table[b];
				dst[i] = a | b;
			}

			return true;
		}
	};


	template<>
	struct Encoding<EncodingScheme::Base91_Rlyeh>
	{
	public:
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			static const unsigned char enctab[91] = {
				// // rlyeh's modification
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', //00..12
				'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', //13..25
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', //26..38
				'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', //39..51
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '#', '$', //52..64
				'%', '&', '(', ')', '*', '+', ',', '.', '/', ':', ';', '-', '=', //65..77
				'\\','?', '@', '[', ']', '^', '_', '`', '{', '|', '}', '~', '\'' //78..90
			};

			std::string out;

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * ib = reinterpret_cast<uint8_t const *>(srcSpan.data());

			unsigned long queue = 0;
			unsigned int nbits = 0;

			for (size_t len = srcSpan.size(); len--; )
			{
				queue |= *ib++ << nbits;
				nbits += 8;
				if (nbits > 13)
				{// enough bits in queue
					unsigned int val = queue & 8191;

					if (val > 88)
					{
						queue >>= 13;
						nbits -= 13;
					}
					else
					{// we can take 14 bits
						val = queue & 16383;
						queue >>= 14;
						nbits -= 14;
					}
					out.push_back(enctab[val % 91]);
					out.push_back(enctab[val / 91]);
				}
			}

			// process remaining bits from bit queue; write up to 2 bytes
			if (nbits)
			{
				out.push_back(enctab[queue % 91]);
				if (nbits > 7 || queue > 90)
					out.push_back(enctab[queue / 91]);
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			const unsigned char dectab[256] = {
				// // rlyeh's modification
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //000..015
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //016..031
				91, 62, 91, 63, 64, 65, 66, 90, 67, 68, 69, 70, 71, 76, 72, 73, //032..047 // @34: ", @39: ', @45: -
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 74, 75, 91, 77, 91, 79, //048..063 // @60: <, @62: >
				80,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //064..079
				15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 81, 78, 82, 83, 84, //080..095 // @92: slash
				85, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //096..111
				41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 86, 87, 88, 89, 91, //112..127
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //128..143
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //144..159
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //160..175
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //176..191
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //192..207
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //208..223
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //224..239
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91  //240..255
			};

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * ib = reinterpret_cast<uint8_t const *>(srcSpan.data());

			std::string out;

			unsigned long queue = 0;
			unsigned int nbits = 0;
			int val = -1;

			for (size_t len = srcSpan.size(); len--; )
			{
				unsigned int d = dectab[*ib++];
				if (d == 91)
					continue;   // ignore non-alphabet chars
				if (val == -1)
					val = d;    // start next value
				else
				{
					val += d * 91;
					queue |= val << nbits;
					nbits += (val & 8191) > 88 ? 13 : 14;
					do
					{
						out.push_back(char(queue));
						queue >>= 8;
						nbits -= 8;
					} while (nbits > 7);
					val = -1;   // mark value complete
				}
			}

			// process remaining bits; write at most 1 byte
			if (val != -1)
				out.push_back( char( queue | val << nbits ) );

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}
	};

	template<>
	struct Encoding<EncodingScheme::Base91_Hencke>
	{
	public:
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			static const unsigned char enctab[91] = {
				 // Henke's original
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', //00..12
				'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', //13..25
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', //26..38
				'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', //39..51
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '#', '$', //52..64
				'%', '&', '(', ')', '*', '+', ',', '.', '/', ':', ';', '<', '=', //65..77
				'>', '?', '@', '[', ']', '^', '_', '`', '{', '|', '}', '~', '"'  //78..90
			};

			std::string out;

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * ib = reinterpret_cast<uint8_t const *>(srcSpan.data());

			unsigned long queue = 0;
			unsigned int nbits = 0;

			for (size_t len = srcSpan.size(); len--; )
			{
				queue |= *ib++ << nbits;
				nbits += 8;
				if (nbits > 13)
				{// enough bits in queue
					unsigned int val = queue & 8191;

					if (val > 88)
					{
						queue >>= 13;
						nbits -= 13;
					}
					else
					{// we can take 14 bits
						val = queue & 16383;
						queue >>= 14;
						nbits -= 14;
					}
					out.push_back(enctab[val % 91]);
					out.push_back(enctab[val / 91]);
				}
			}

			// process remaining bits from bit queue; write up to 2 bytes
			if (nbits)
			{
				out.push_back(enctab[queue % 91]);
				if (nbits > 7 || queue > 90)
					out.push_back(enctab[queue / 91]);
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			const unsigned char dectab[256] = {
				// Henke's original
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //000..015
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //016..031
				91, 62, 90, 63, 64, 65, 66, 91, 67, 68, 69, 70, 71, 91, 72, 73, //032..047
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 74, 75, 76, 77, 78, 79, //048..063
				80,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //064..079
				15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 81, 91, 82, 83, 84, //080..095
				85, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //096..111
				41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 86, 87, 88, 89, 91, //112..127
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //128..143
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //144..159
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //160..175
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //176..191
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //192..207
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //208..223
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, //224..239
				91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91  //240..255
			};

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);
			uint8_t const * ib = reinterpret_cast<uint8_t const *>(srcSpan.data());

			std::string out;

			unsigned long queue = 0;
			unsigned int nbits = 0;
			int val = -1;

			for (size_t len = srcSpan.size(); len--; )
			{
				unsigned int d = dectab[*ib++];
				if (d == 91)
					continue;   // ignore non-alphabet chars
				if (val == -1)
					val = d;    // start next value
				else
				{
					val += d * 91;
					queue |= val << nbits;
					nbits += (val & 8191) > 88 ? 13 : 14;
					do
					{
						out.push_back(char(queue));
						queue >>= 8;
						nbits -= 8;
					} while (nbits > 7);
					val = -1;   // mark value complete
				}
			}

			// process remaining bits; write at most 1 byte
			if (val != -1)
				out.push_back( char( queue | val << nbits ) );

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}
	};


	namespace detail {

		// base85 (z85): standard rfc size (multiples of 4/5)
		inline bool encodeZ85(std::string & out, unsigned char const * raw, size_t rawlen)
		{
			if (rawlen % 4)
				return false; // error: raw string size must be multiple of 4

			// encode
			const char encoder[86] =
				"0123456789" "abcdefghij" "klmnopqrst" "uvwxyzABCD"             // 00..39
				"EFGHIJKLMN" "OPQRSTUVWX" "YZ.-:+=^!/" "*?&<>()[]{" "}@%$#";    // 40..84 // free chars: , ; _ ` | ~ \'
			out.resize(rawlen * 5 / 4);
			for (size_t o = 0; o < rawlen * 5 / 4; raw += 4) 
			{
				unsigned value = 
					(raw[0] << 24) | 
					(raw[1] << 16) | 
					(raw[2] << 8)  |
					raw[3];
				out[o++] = encoder[(value / 0x31C84B1) % 0x55];
				out[o++] = encoder[(value /   0x95EED) % 0x55];
				out[o++] = encoder[(value /    0x1C39) % 0x55];
				out[o++] = encoder[(value /      0x55) % 0x55];
				out[o++] = encoder[ value %      0x55 ];
			}
			return true;
		}

		inline bool decodeZ85( std::string &out, unsigned char const * z85, size_t z85len)
		{
			if (z85len % 5)
				return false; // error: z85 string size must be multiple of 5

			// decode
			const unsigned char decoder[128] = {
				0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 0x00..0x0F
				0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 0x10..0x1F
				0,  68,  0, 84, 83, 82, 72,  0, 75, 76, 70, 65,  0, 63, 62, 69, // 0x20..0x2F
				0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 64,  0, 73, 66, 74, 71, // 0x30..0x3F
				81, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, // 0x40..0x4F
				51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77,  0, 78, 67,  0, // 0x50..0x5F
				0,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x60..0x6F
				25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 79,  0, 80,  0,  0, // 0x70..0x7F
			};
			out.resize( z85len * 4 / 5 );
			for( size_t o = 0; o < z85len * 4 / 5; z85 += 5 ) {
				unsigned value = decoder[z85[0]] * 0x31C84B1 + decoder[z85[1]] * 0x95EED +
					decoder[z85[2]] *    0x1C39 + decoder[z85[3]] *    0x55 + decoder[z85[4]];
				out[o++] = (value >> 24) & 0xFF;
				out[o++] = (value >> 16) & 0xFF;
				out[o++] = (value >>  8) & 0xFF;
				out[o++] = (value >>  0) & 0xFF;
			}
			return true;
		}
	}

	template<>
	struct Encoding<EncodingScheme::Base85_Z85>
	{
	public:
		// base85 (z85): arbitrary size (this may lead up to four additional bytes)

		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			// create padding, if needed
			std::string pad4 = std::string((const char *)srcSpan.data(), srcSpan.size()) + '\1' + std::string("\0\0\0\0", 4 - (srcSpan.size() + 1) % 4);
			
			bool result = detail::encodeZ85(out, (const unsigned char *)pad4.c_str(), pad4.size());

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());

			}

			return result;

		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{
			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			bool result = detail::decodeZ85(out, (const unsigned char *)srcSpan.data(), srcSpan.size());

			if (!result) 
				return false;

			// remove padding, if needed
			while (out.size() && *out.rbegin() == '\0') out.resize(out.size() - 1);
			if    (out.size() && *out.rbegin() == '\1') out.resize(out.size() - 1);

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}
	};

	template<>
	struct Encoding<EncodingScheme::Base64>
	{
	public:
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc, bool usePadding = false)
		{

			static const std::string chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

			Serializer<TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			unsigned char const* bytes_to_encode = (unsigned char const *)srcSpan.data();
			unsigned int in_len = (unsigned int)srcSpan.size();
			unsigned int i = 0;
			unsigned int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];

			while (in_len--)
			{
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3)
				{
					char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] =   char_array_3[2] & 0x3f;

					for (i = 0; (i < 4); i++)
						out += chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i) 
			{
				for (j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] =   char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					out += chars[char_array_4[j]];

				if (usePadding)
					while ((i++ < 3))
						out += '=';
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{

			static const std::string chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			unsigned char const* encoded = (unsigned char const *)srcSpan.data();
			unsigned int in_len = (unsigned int)srcSpan.size();
			unsigned int i = 0;
			unsigned int j = 0;
			unsigned int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];

			while (in_len-- && (encoded[in_] != '=') && (isalnum(encoded[in_]) || encoded[in_] == '+' || encoded[in_] == '/'))
			{
				char_array_4[i++] = encoded[in_]; in_++;
				if (i == 4)
				{
					for (i = 0; i < 4; i++)
						char_array_4[i] = chars.find(char_array_4[i]);

					char_array_3[0] =  (char_array_4[0]        << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

					for (i = 0; (i < 3); i++)
						out += char_array_3[i];
					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 4; j++)
					char_array_4[j] = 0;

				for (j = 0; j < 4; j++)
					char_array_4[j] = chars.find(char_array_4[j]);

				char_array_3[0] =  (char_array_4[0]        << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

				for (j = 0; (j < i - 1); j++) 
					out += char_array_3[j];
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}
	};

	template<>
	struct Encoding<EncodingScheme::Base64URL>
	{
	public:
		template <typename TDst, typename TSrc>
		static bool Encode(TDst & dDst, TSrc const & dSrc, bool usePadding = false)
		{

			static const std::string chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789-_";

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			unsigned char const* bytes_to_encode = (unsigned char const *)srcSpan.data();
			unsigned int in_len = (unsigned int)srcSpan.size();
			unsigned int i = 0;
			unsigned int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];

			while (in_len--)
			{
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3)
				{
					char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] =   char_array_3[2] & 0x3f;

					for (i = 0; (i < 4); i++)
						out += chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i) 
			{
				for (j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] =   char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					out += chars[char_array_4[j]];

				if (usePadding)
					while ((i++ < 3))
						out += '=';
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}

		template <typename TDst, typename TSrc>
		static bool Decode(TDst & dDst, TSrc const & dSrc)
		{

			static const std::string chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789-_";

			Serializer  <TSrc> srcSer;
			std::span<std::byte const> srcSpan = srcSer.get_span(dSrc);

			std::string out;

			unsigned char const* encoded = (unsigned char const *)srcSpan.data();
			unsigned int in_len = (unsigned int)srcSpan.size();
			unsigned int i = 0;
			unsigned int j = 0;
			unsigned int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];

			while (in_len-- && (encoded[in_] != '=') && (isalnum(encoded[in_]) || encoded[in_] == '-' || encoded[in_] == '_'))
			{
				char_array_4[i++] = encoded[in_]; in_++;
				if (i == 4)
				{
					for (i = 0; i < 4; i++)
						char_array_4[i] = chars.find(char_array_4[i]);

					char_array_3[0] =  (char_array_4[0]        << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

					for (i = 0; (i < 3); i++)
						out += char_array_3[i];
					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 4; j++)
					char_array_4[j] = 0;

				for (j = 0; j < 4; j++)
					char_array_4[j] = chars.find(char_array_4[j]);

				char_array_3[0] =  (char_array_4[0]        << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

				for (j = 0; (j < i - 1); j++) 
					out += char_array_3[j];
			}

			if constexpr (std::is_same<TDst, std::string>::value)
			{
				std::swap(out, dDst);
			}
			else
			{
				Deserializer<TDst> dstDeser;
				std::byte * dst = dstDeser.prepare_span(out.size(), dDst);
				if (!dst)
					return false;
				memcpy(dst, out.data(), out.size());
			}

			return true;
		}
	};

	template <typename TDst, typename TSrc>
	FORCEINLINE bool Copy(TDst & dDst, TSrc const & dSrc)
	{
		return Handy::Encoding<Handy::EncodingScheme::Raw>::Encode(dDst, dSrc);
	}

}

//void encoding_test()
//{
//	std::string encoded_hex; Encoding<EncodingScheme::Hex          >::Encode(encoded_hex, "Hello world in HEX         ! \x1"s);
//	std::string decoded_hex; Encoding<EncodingScheme::Hex          >::Decode(decoded_hex, encoded_hex);
//	std::cout<< decoded_hex << " <-> " << encoded_hex << std::endl;
//
//	std::string encoded_64; Encoding<EncodingScheme::Base64        >::Encode(encoded_64,  "Hello world in BASE64      ! \x1"s);
//	std::string decoded_64; Encoding<EncodingScheme::Base64        >::Decode(decoded_64,  encoded_64);
//	std::cout<< decoded_64 << " <-> " << encoded_64 << std::endl;
//
//	std::string encoded_64u; Encoding<EncodingScheme::Base64URL    >::Encode(encoded_64u, "Hello world in BASE64URL   ! \x1"s);
//	std::string decoded_64u; Encoding<EncodingScheme::Base64URL    >::Decode(decoded_64u, encoded_64u);
//	std::cout<< decoded_64u << " <-> " << encoded_64u << std::endl;
//
//	std::string encoded_85; Encoding<EncodingScheme::Base85_Z85    >::Encode(encoded_85,  "Hello world in BASE85      ! \x1"s);
//	std::string decoded_85; Encoding<EncodingScheme::Base85_Z85    >::Decode(decoded_85,  encoded_85);
//	std::cout<< decoded_85 << " <-> " << encoded_85 << std::endl;
//
//	std::string encoded_91h; Encoding<EncodingScheme::Base91_Hencke>::Encode(encoded_91h, "Hello world in BASE91Hencke! \x1"s);
//	std::string decoded_91h; Encoding<EncodingScheme::Base91_Hencke>::Decode(decoded_91h, encoded_91h);
//	std::cout<< decoded_91h << " <-> " << encoded_91h << std::endl;
//
//	std::string encoded_91r; Encoding<EncodingScheme::Base91_Rlyeh >::Encode(encoded_91r, "Hello world in BASE91Rlyeh ! \x1"s);
//	std::string decoded_91r; Encoding<EncodingScheme::Base91_Rlyeh >::Decode(decoded_91r, encoded_91r);
//	std::cout<< decoded_91r << " <-> " << encoded_91r << std::endl;
//
//	std::cout << "=================================================" << std::endl;
//
//	Handy::Guid TestGuid = Handy::Guid::New();
//	Handy::Guid TempGuid;
//
//	Encoding<EncodingScheme::HexGuid>::Encode(encoded_hex, TestGuid);
//	Encoding<EncodingScheme::HexGuid>::Decode(TempGuid, encoded_hex);
//	Encoding<EncodingScheme::HexGuid>::Encode(decoded_hex, TempGuid);
//	std::cout << "HXG: " << decoded_hex << " <-> " << encoded_hex << std::endl;
//
//	Encoding<EncodingScheme::Hex>::Encode(encoded_hex, TestGuid);
//	Encoding<EncodingScheme::Hex>::Decode(TempGuid, encoded_hex);
//	Encoding<EncodingScheme::Hex>::Encode(decoded_hex, TempGuid);
//	std::cout << "HEX: " << decoded_hex << "     <-> " << encoded_hex << std::endl;
//
//	Encoding<EncodingScheme::Base64>::Encode(encoded_64, TestGuid);
//	Encoding<EncodingScheme::Base64>::Decode(TempGuid, encoded_64);
//	Encoding<EncodingScheme::Base64>::Encode(decoded_64, TempGuid);
//	std::cout << "64:  " << decoded_64 << "               <-> " << encoded_64 << std::endl;
//
//	Encoding<EncodingScheme::Base64URL>::Encode(encoded_64u, TestGuid);
//	Encoding<EncodingScheme::Base64URL>::Decode(TempGuid, encoded_64u);
//	Encoding<EncodingScheme::Base64URL>::Encode(decoded_64u, TempGuid);
//	std::cout << "64U: " << decoded_64u << "               <-> " << encoded_64u << std::endl;
//
//	Encoding<EncodingScheme::Base91_Hencke>::Encode(encoded_91h, TestGuid);
//	Encoding<EncodingScheme::Base91_Hencke>::Decode(TempGuid, encoded_91h);
//	Encoding<EncodingScheme::Base91_Hencke>::Encode(decoded_91h, TempGuid);
//	std::cout << "91H: " << decoded_91h << "                 <-> " << encoded_91h << std::endl;
//
//	Encoding<EncodingScheme::Base91_Rlyeh>::Encode(encoded_91r, TestGuid);
//	Encoding<EncodingScheme::Base91_Rlyeh>::Decode(TempGuid, encoded_91r);
//	Encoding<EncodingScheme::Base91_Rlyeh>::Encode(decoded_91r, TempGuid);
//	std::cout << "91R: " << decoded_91r << "                 <-> " << encoded_91r << std::endl << std::endl;
//
//	Encoding<EncodingScheme::Base85_Z85>::Encode(encoded_85, TestGuid);
//	Encoding<EncodingScheme::Base85_Z85>::Decode(TempGuid, encoded_85);
//	Encoding<EncodingScheme::Base85_Z85>::Encode(decoded_85, TempGuid);
//	std::cout << "Z85: " << decoded_85 << " <-> " << encoded_85 << std::endl << std::endl;
//}
