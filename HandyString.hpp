
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

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <locale>
#include <cstdio>
#include <charconv>

#include "HandyBase.hpp"
#include "HandyCompat.hpp"

namespace HANDY_NS {

	///-----------------------------------------------
	/// isWhitespace
	///
	inline
	FCONST bool IsWhitespace(char ch) noexcept
	{
		switch (ch)
		{
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				return true;
		}
		return false;
	}
	///-----------------------------------------------

	inline
	bool StringContains(std::string const & str, std::string const & sub)
	{
		bool result = str.find(sub) != std::string::npos;
		return result;
	}

	///-----------------------------------------------
	/// Trim, Replace
	///
	inline
	std::string TrimLeft(std::string const & str)
	{
		const char * p = str.data();
		const char * end = p + str.length();

		while (p < end && IsWhitespace(*p))
			++p;

		return std::string(p, size_t(end - p));
	}

	inline
	std::string TrimRight(std::string const & str)
	{
		const char * p = str.data();
		const char * end = p + str.length();

		while (end > p && IsWhitespace(end[-1]))
			--end;

		return std::string(p, size_t(end - p));
	}

	inline
	std::string Trim(std::string const & str)
	{
		return TrimLeft(TrimRight(str));
	}

	inline
	std::string_view TrimLeft(std::string_view const & str)
	{
		const char * p = str.data();
		const char * end = p + str.length();

		while (p < end && IsWhitespace(*p))
			++p;

		return std::string_view(p, size_t(end - p));
	}

	inline
	std::string_view TrimRight(std::string_view const & str)
	{
		const char * p = str.data();
		const char * end = p + str.length();

		while (end > p && IsWhitespace(end[-1]))
			--end;

		return std::string_view(p, size_t(end - p));
	}

	inline
	std::string_view Trim(std::string_view const & str)
	{
		return TrimLeft(TrimRight(str));
	}

	// If need speed or something complicated, use std::regex_replace.
	inline
	void Replace(std::string & str, std::string const & from, std::string const & to)
	{
		if (from.empty())
			return;

		size_t start_pos = 0;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	inline
	bool ReplaceFirst(std::string & str, std::string const & from, std::string const & to)
	{
		size_t start_pos = str.find(from);

		if (start_pos == std::string::npos)
			return false;

		str.replace(start_pos, from.length(), to);

		return true;
	}

	inline
	std::vector<std::string> Explode(std::string const & str, char separator)
	{
		std::vector<std::string> result;
		size_t npos, pos = 0;

		do
		{
			npos = str.find(separator, pos);
			result.push_back(str.substr(pos, npos != std::string::npos ? npos - pos : std::string::npos));
			pos = npos + 1;
		} while (npos != std::string::npos);

		return result;
	}

	inline
	void ExplodeView(std::string_view const & str, char separator, std::vector<std::string_view> & dest)
	{
		size_t npos, pos = 0;
		do
		{
			npos = str.find(separator, pos);
			dest.push_back(str.substr(pos, npos != std::string_view::npos ? npos - pos : std::string_view::npos));
			pos = npos + 1;
		} while (npos != std::string::npos);
	}

	// Tokenize Including Separators, ie TokenizeWSep(SomeString, "!\"#~$%&/(){}[]|,;.:<>+-/*@'\"\t\n\\ ");
	inline
	std::vector<std::string> TokenizeWSep(const std::string &string, const std::string &delimiters)
	{
		std::string str;
		std::vector< std::string > tokens;
		for (auto &ch : string) 
		{
			if (delimiters.find_first_of(ch) != std::string::npos)
			{
				if (str.size()) 
				{
					tokens.push_back(str);
					str = "";
				}
				tokens.push_back(std::string() + ch);
			}
			else str += ch;
		}
		return str.empty() ? tokens : (tokens.push_back(str), tokens);
	}

	///-----------------------------------------------

	#if defined IS_WINDOWS
	#pragma warning(push)
	#pragma warning(disable : 4996)
		//inline
		//std::wstring ToWString(std::string const & str)
		//{
		//	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		//	return converterX.from_bytes(str);
		//}

		//inline
		//std::string ToString(std::wstring const & wstr)
		//{
		//	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		//	return converterX.to_bytes(wstr);
		//}
	#pragma warning(pop) 

	#endif

	inline std::string ToLower(std::string str) // By value
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return str;
	}

	/// I'd recommend Trim()ing the str first
	inline bool IsNumber(std::string str)
	{
		bool isNumber = true;

		for (auto c : str)
			if (std::isdigit(c) == 0)
			{
				isNumber = false;
				break;
			}

		return isNumber;
	}

	inline int ConvertHex4CharsToInt(const char * temp) 
	{
		int result = 0;
		for (int i=0; i < 4; i++)
		{
			int n = temp[i];
			if(n >= '0' && n <= '9')
				n -= '0';
			else if(n >= 'a' && n <= 'f')
				n = n - 'a' + 10;
			else if(n >= 'A' && n <= 'F')
				n = n - 'A' + 10;
			result = (result << 4) + n;
		}
		return result;
	}

	inline int ConvertHex8CharsToInt(const char * temp)
	{
		int result = 0;
		for (int i=0; i < 8; i++)
		{
			int n = temp[i];
			if(n >= '0' && n <= '9')
				n -= '0';
			else if(n >= 'a' && n <= 'f')
				n = n - 'a' + 10;
			else if(n >= 'A' && n <= 'F')
				n = n - 'A' + 10;
			else
				return -1;
			result = (result << 4) + n;
		}
		return result;
	}

	inline std::string ToString_Hex(int32_t hexvalue)
	{
		std::string result;
		result.reserve(8);
		for(int i = 28; i >= 0; i -= 4)
			result += "0123456789ABCDEF"[(hexvalue >> i) & 0xF];
		return result;
	}

	// Take a vector of strings and return a camma-separated list of the items (as a string)
	inline std::string VectorOfStringsToCSL(std::vector<std::string> VecOfStrings) 
	{
		std::string CSL;
		if (! VecOfStrings.empty()) 
		{
			for (size_t n = 0U; n + 1U < VecOfStrings.size(); n++)
				CSL += VecOfStrings[n] + ", ";
			CSL += VecOfStrings.back();
		}
		return CSL;
	}
	
	///-----------------------------------------------
	/// String casting
	///

	namespace Internals
	{
		template<typename T> struct DefaultValue              { static inline const T Value = 0; };
		template<>           struct DefaultValue<std::string> { static inline const std::string Value = ""; };
		template<>           struct DefaultValue<bool>        { static constexpr bool Value = false; };

		//using a struct allows partial specialization, we'll wrap this with nicer functions later
		template <typename Tval, typename Tstring> struct Impl
		{
			static FORCEINLINE bool Convert(Tstring str, Tval & result)
			{
				if (std::empty(str))
					return false; 
				const char * start = std::data(str); 
				const char * end   = start + std::size(str); 
				auto[p, ec] = std::from_chars(start, end, result); 
				return ec == std::errc() && p == end; 
			} 
		};

		template<typename Tstring> struct Impl<bool, Tstring>
		{
			using Tval = bool;
			static FORCEINLINE bool Convert(Tstring str, Tval & result)
			{
				if (std::empty(str))
					return false;
				const char * data = std::data(str);
				const size_t size = std::size(str);
				switch (size)
				{
					case 1: //"y" "n" "t" "f" "1" "0"
					{
						switch (data[0])
						{
							case 't': case 'T': case 'y': case 'Y': case '1': result = true;  return true;
							case 'f': case 'F': case 'n': case 'N': case '0': result = false; return true;
							default: return false;
						}
					}
					case 2: /*"no" */   switch (data[0]) { case 'n': case 'N': switch (data[1]) { case 'o': case 'O': result = false; return true; default: return false; } default: return false; }
					case 3: /*"yes"*/   switch (data[0]) { case 'y': case 'Y': switch (data[1]) { case 'e': case 'E': switch (data[2]) { case 's': case 'S': result = true;  return true; default: return false; } default: return false; } default: return false; }
					case 4: /*"true"*/  switch (data[0]) { case 't': case 'T': switch (data[1]) { case 'r': case 'R': switch (data[2]) { case 'u': case 'U': switch (data[3]) { case 'e': case 'E': result = true; return true; default: return false; } default: return false; } default: return false; } default: return false; }
					case 5: /*"false"*/ switch (data[0]) { case 'f': case 'F': switch (data[1]) { case 'a': case 'A': switch (data[2]) { case 'l': case 'L': switch (data[3]) { case 's': case 'S': switch (data[4]) { case 'e': case 'E': result = false; return true; default: return false; } default: return false; } default: return false; } default: return false; } default: return false; }
					default: return false;
				}
			}
		};
	}
	template<typename Tval> FORCEINLINE Tval SafeConvert(const char *     str, size_t len, Tval const & def = Internals::DefaultValue<Tval>::Value) { Tval v; return                   Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str, len), v) ? v : def; }
	template<typename Tval> FORCEINLINE Tval SafeConvert(const char *     str,             Tval const & def = Internals::DefaultValue<Tval>::Value) { Tval v; return str != nullptr && Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str),      v) ? v : def; }
	template<typename Tval> FORCEINLINE Tval SafeConvert(std::string      str,             Tval const & def = Internals::DefaultValue<Tval>::Value) { Tval v; return                   Internals::Impl<Tval, std::string>     ::Convert(str, v)                        ? v : def; }
	template<typename Tval> FORCEINLINE Tval SafeConvert(std::string_view str,             Tval const & def = Internals::DefaultValue<Tval>::Value) { Tval v; return                   Internals::Impl<Tval, std::string_view>::Convert(str, v)                        ? v : def; }

	template<typename Tval> FORCEINLINE std::optional<Tval> Convert(const char *     str, size_t len) { Tval v; return                   Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str, len), v) ? std::optional<Tval>(v) : std::optional<Tval>(); }
	template<typename Tval> FORCEINLINE std::optional<Tval> Convert(const char *     str)             { Tval v; return str != nullptr && Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str),      v) ? std::optional<Tval>(v) : std::optional<Tval>(); }
	template<typename Tval> FORCEINLINE std::optional<Tval> Convert(std::string      str)             { Tval v; return                   Internals::Impl<Tval, std::string>     ::Convert(str, v)                        ? std::optional<Tval>(v) : std::optional<Tval>(); }
	template<typename Tval> FORCEINLINE std::optional<Tval> Convert(std::string_view str)             { Tval v; return                   Internals::Impl<Tval, std::string_view>::Convert(str, v)                        ? std::optional<Tval>(v) : std::optional<Tval>(); }

	template<typename Tval> FORCEINLINE bool TryConvert(const char *     str, size_t len, Tval & result) { Tval v; if                   (Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str, len), v)) { result = v; return true; } else { return false; } }
	template<typename Tval> FORCEINLINE bool TryConvert(const char *     str,             Tval & result) { Tval v; if (str != nullptr && Internals::Impl<Tval, std::string_view>::Convert(std::string_view(str), v))      { result = v; return true; } else { return false; } }
	template<typename Tval> FORCEINLINE bool TryConvert(std::string      str,             Tval & result) { Tval v; if                   (Internals::Impl<Tval, std::string>     ::Convert(str, v))                        { result = v; return true; } else { return false; } }
	template<typename Tval> FORCEINLINE bool TryConvert(std::string_view str,             Tval & result) { Tval v; if                   (Internals::Impl<Tval, std::string_view>::Convert(str, v))                        { result = v; return true; } else { return false; } }




	

	/// At some point I'd like to absorb the "fmt" library from GitHub, but this function will do for now.
	template<typename ... Args>
	std::string Format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	///--------------------------------------------------------------------------------------------------
	/// Build Timestamp, format args are Year, Month, Day, Hour, Minute, and Second.
	inline std::string BuildTimestamp(std::string const & format = "%04d/%02d/%02d %02d:%02d:%02d")
	{
		return Format(format, BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
	}


} // HANDY_NS