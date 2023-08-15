
/// =============================================================================
/// crossguid (https://github.com/graeme-hill/crossguid)
/// 
/// LICENSE: MIT
/// 
/// Copyright (c) 2014 Graeme Hill (http://graemehill.ca)
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

#include "Common.hpp"

namespace HANDY_NS
{

	// overload << so that it's easy to convert to a string
	std::ostream & operator<<(std::ostream &s, Guid const & guid)
	{
		std::ios_base::fmtflags f(s.flags()); // politely don't leave the ostream in hex mode
		s << std::hex << std::setfill('0')
			<< std::setw(2) << (int)guid.Bytes[0]
			<< std::setw(2) << (int)guid.Bytes[1]
			<< std::setw(2) << (int)guid.Bytes[2]
			<< std::setw(2) << (int)guid.Bytes[3]
			<< "-"
			<< std::setw(2) << (int)guid.Bytes[4]
			<< std::setw(2) << (int)guid.Bytes[5]
			<< "-"
			<< std::setw(2) << (int)guid.Bytes[6]
			<< std::setw(2) << (int)guid.Bytes[7]
			<< "-"
			<< std::setw(2) << (int)guid.Bytes[8]
			<< std::setw(2) << (int)guid.Bytes[9]
			<< "-"
			<< std::setw(2) << (int)guid.Bytes[10]
			<< std::setw(2) << (int)guid.Bytes[11]
			<< std::setw(2) << (int)guid.Bytes[12]
			<< std::setw(2) << (int)guid.Bytes[13]
			<< std::setw(2) << (int)guid.Bytes[14]
			<< std::setw(2) << (int)guid.Bytes[15];
		s.flags(f);
		return s;
	}

	Guid Guid::New()
	{
		#if defined IS_WINDOWS

			GUID newId;
			CoCreateGuid(&newId);

			std::array<unsigned char, 16> bytes =
			{
				(uint8_t)((newId.Data1 >> 24) & 0xFF),
				(uint8_t)((newId.Data1 >> 16) & 0xFF),
				(uint8_t)((newId.Data1 >> 8) & 0xFF),
				(uint8_t)((newId.Data1) & 0xff),

				(uint8_t)((newId.Data2 >> 8) & 0xFF),
				(uint8_t)((newId.Data2) & 0xff),

				(uint8_t)((newId.Data3 >> 8) & 0xFF),
				(uint8_t)((newId.Data3) & 0xFF),

				(uint8_t)newId.Data4[0],
				(uint8_t)newId.Data4[1],
				(uint8_t)newId.Data4[2],
				(uint8_t)newId.Data4[3],
				(uint8_t)newId.Data4[4],
				(uint8_t)newId.Data4[5],
				(uint8_t)newId.Data4[6],
				(uint8_t)newId.Data4[7]
			};

			return Guid{ std::move(bytes) };

		#elif defined IS_APPLE

			auto newId = CFUUIDCreate(NULL);
			auto bytes = CFUUIDGetUUIDBytes(newId);
			CFRelease(newId);

			std::array<uint8_t, 16> byteArray =
			{ {
					bytes.byte0,
					bytes.byte1,
					bytes.byte2,
					bytes.byte3,
					bytes.byte4,
					bytes.byte5,
					bytes.byte6,
					bytes.byte7,
					bytes.byte8,
					bytes.byte9,
					bytes.byte10,
					bytes.byte11,
					bytes.byte12,
					bytes.byte13,
					bytes.byte14,
					bytes.byte15
				} };
			return Guid{ std::move(byteArray) };

		#elif defined IS_NIX
		/// This is the linux friendly implementation, but it could work on other systems that have libuuid available
			std::array<uint8_t, 16> data;
			static_assert(std::is_same<uint8_t[16], uuid_t>::value, "Wrong type!");
			uuid_generate(data.data());
			return Guid{ std::move(data) };
		#endif
	}

}

