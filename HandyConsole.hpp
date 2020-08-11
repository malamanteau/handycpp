
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

#include <iostream>
#include <atomic>
#include <functional>

/// rang library, by Abhinav Gauniyal (https://github.com/agauniyal/rang/)

namespace HC /// I've abbreviated the Handy::Console namespace for the stream tokens, so 
			 /// you can just say "std::cout << HC::fg::blue", instead of "std::cout << Handy::Console::fg::blue".
{
	// For better compability with most of terminals do not use any style settings
	// except of reset, bold and reversed.
	// Note that on Windows terminals bold style is same as fgB color.
	enum class style : int
	{
		reset     = 0,
		bold      = 1,
		dim       = 2,
		italic    = 3,
		underline = 4,
		blink     = 5,
		rblink    = 6,
		reversed  = 7,
		conceal   = 8,
		crossed   = 9
	};

	enum class fg : int
	{
		black   = 30,
		red     = 31,
		green   = 32,
		yellow  = 33,
		blue    = 34,
		magenta = 35,
		cyan    = 36,
		gray    = 37,
		reset   = 39
	};

	enum class bg : int
	{
		black   = 40,
		red     = 41,
		green   = 42,
		yellow  = 43,
		blue    = 44,
		magenta = 45,
		cyan    = 46,
		gray    = 47,
		reset   = 49
	};

	enum class fgB : int
	{
		black   = 90,
		red     = 91,
		green   = 92,
		yellow  = 93,
		blue    = 94,
		magenta = 95,
		cyan    = 96,
		gray    = 97
	};

	enum class bgB : int
	{
		black   = 100,
		red     = 101,
		green   = 102,
		yellow  = 103,
		blue    = 104,
		magenta = 105,
		cyan    = 106,
		gray    = 107
	};

	enum class st : int
	{
		push,
		pop,
	};

	enum class control : int // Behaviour of HC function calls
	{ 
		Off   = 0,  // toggle off HC style/color calls
		Auto  = 1,  // (Default) autodect terminal and colorize if needed
		Force = 2   // force ansi color output to non terminal streams
	};
	// Use HC::setControlMode to set HC control mode

	enum class winTerm : int  // Windows Terminal Mode
	{ 
		Auto   = 0, // (Default) automatically detects wheter Ansi or Native API
		Ansi   = 1, // Force use Ansi API
		Native = 2  // Force use Native API
	};
	// Use HC::setWinTermMode to explicitly set terminal API for Windows
	// Calling HC::setWinTermMode have no effect on other OS

	namespace detail
	{
		inline std::atomic<control> &controlMode() noexcept
		{
			static std::atomic<control> value(control::Auto);
			return value;
		}

		inline std::atomic<winTerm> &winTermMode() noexcept
		{
			static std::atomic<winTerm> termMode(winTerm::Auto);
			return termMode;
		}

		template <typename T>
		using enableStd = typename std::enable_if<
			std::is_same<T, HC::style>::value
			|| std::is_same<T, HC::fg   >::value
			|| std::is_same<T, HC::bg   >::value
			|| std::is_same<T, HC::fgB  >::value
			|| std::is_same<T, HC::bgB  >::value
			|| std::is_same<T, HC::st   >::value,
			std::ostream &>::type;

		bool supportsColor() noexcept;
		bool isTerminal(const std::streambuf *osbuf) noexcept;

		std::ostream & setColor(std::ostream &os, HC::style const value);
		std::ostream & setColor(std::ostream &os, HC::fg    const value);
		std::ostream & setColor(std::ostream &os, HC::bg    const value);
		std::ostream & setColor(std::ostream &os, HC::fgB   const value);
		std::ostream & setColor(std::ostream &os, HC::bgB   const value);
		std::ostream & setColor(std::ostream &os, HC::st    const value);
	}

	template <typename T>
	inline detail::enableStd<T> operator<<(std::ostream &os, const T value)
	{
		const control option = detail::controlMode();
		switch (option) {
			case control::Auto:
				return detail::supportsColor()
					&& detail::isTerminal(os.rdbuf())
					?  detail::setColor(os, value)
					: os;
			case control::Force: return detail::setColor(os, value);
			default: return os;
		}
	}

	inline void setWinTermMode(const HC::winTerm value) noexcept
	{
		detail::winTermMode() = value;
	}

	inline void setControlMode(const control value) noexcept
	{
		detail::controlMode() = value;
	}
}

namespace HANDY_NS::Console
{
	bool Capture(std::ostream & os = std::cout);
	bool Release(std::ostream & os = std::cout);
	void AddHighlights(HC::fgB color, std::vector<std::string> const & highlights);

	void SetOutputCallback(std::function<void(std::string const &)> func);
}
