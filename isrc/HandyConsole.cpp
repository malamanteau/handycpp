

/// =============================================================================
///	DrEcho (https://github.com/r-lyeh-archived/DrEcho)
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

/// rang library, by Abhinav Gauniyal (https://github.com/agauniyal/rang/)

#include "Common.hpp"

std::streambuf * GetOrigSB(std::ostream & os)
{
	return os.rdbuf();
}

std::ostream GetNewStream(std::streambuf * sb) { return std::ostream(sb); }

std::ostream ncout = GetNewStream(GetOrigSB(std::cout));
std::ostream ncerr = GetNewStream(GetOrigSB(std::cerr));
std::ostream nclog = GetNewStream(GetOrigSB(std::clog));

std::streambuf * nobuf = GetOrigSB(std::cout);
std::streambuf * nebuf = GetOrigSB(std::cerr);
std::streambuf * nlbuf = GetOrigSB(std::clog);

std::streambuf * uobuf = nullptr;
std::streambuf * uebuf = nullptr;
std::streambuf * ulbuf = nullptr;

namespace HC {

	namespace detail 
	{
		bool supportsColor() noexcept
		{
			#if defined(IS_LINUX) || defined(IS_OSX)
				static const bool result = [] {
					const char *Terms[]
						= { "ansi",    "color",  "console", "cygwin", "gnome",
						"konsole", "kterm",  "linux",   "msys",   "putty",
						"rxvt",    "screen", "vt100",   "xterm" };

					const char *env_p = std::getenv("TERM");
					if (env_p == nullptr) {
						return false;
					}
					return std::any_of(std::begin(Terms), std::end(Terms),
									   [&](const char *term) {
						return std::strstr(env_p, term) != nullptr;
					});
				}();

			#elif defined(IS_WINDOWS)
				// All windows versions support colors through native console methods
				static constexpr bool result = true;
			#endif
			return result;
		}

		#ifdef IS_WINDOWS

		bool isMsysPty(int fd) noexcept
		{
			// Dynamic load for binary compability with old Windows
			const auto ptrGetFileInformationByHandleEx = reinterpret_cast<decltype(&GetFileInformationByHandleEx)>(
					GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetFileInformationByHandleEx"));
			
			if (!ptrGetFileInformationByHandleEx)
				return false;

			HANDLE h = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
			if (h == INVALID_HANDLE_VALUE)
				return false;

			if (GetFileType(h) != FILE_TYPE_PIPE) // Check that it's a pipe:
				return false;

			// POD type is binary compatible with FILE_NAME_INFO from WinBase.h
			// It have the same alignment and used to avoid UB in caller code
			struct MY_FILE_NAME_INFO 
			{
				DWORD FileNameLength;
				WCHAR FileName[MAX_PATH];
			};

			auto pNameInfo = std::unique_ptr<MY_FILE_NAME_INFO>(new (std::nothrow) MY_FILE_NAME_INFO());
			if (!pNameInfo) {
				return false;
			}

			// Check pipe name is template of
			// {"cygwin-","msys-"}XXXXXXXXXXXXXXX-ptyX-XX
			if (!ptrGetFileInformationByHandleEx(h, FileNameInfo, pNameInfo.get(), sizeof(MY_FILE_NAME_INFO))) 
				return false;

			std::wstring name(pNameInfo->FileName, pNameInfo->FileNameLength / sizeof(WCHAR));

			if ((name.find(L"msys-")   == std::wstring::npos
			&&   name.find(L"cygwin-") == std::wstring::npos)
			||   name.find(L"-pty")    == std::wstring::npos) {
				return false;
			}

			return true;
		}

		#endif

		bool isTerminal(const std::streambuf *osbuf) noexcept
		{
			if (osbuf == nobuf || osbuf == uobuf)
			{
				#if defined(IS_LINUX) || defined(IS_OSX)
				static const bool cout_term = isatty(fileno(stdout)) != 0;
				#elif defined IS_WINDOWS
				static const bool cout_term = (_isatty(_fileno(stdout)) || isMsysPty(_fileno(stdout)));
				#endif
				return cout_term;
			} 
			else if (osbuf == nebuf || osbuf == nlbuf || osbuf == uebuf || osbuf == ulbuf)
			{
				#if defined(IS_LINUX) || defined(IS_OSX)
				static const bool cerr_term = isatty(fileno(stderr)) != 0;
				#elif defined IS_WINDOWS
				static const bool cerr_term = (_isatty(_fileno(stderr)) || isMsysPty(_fileno(stderr)));
				#endif
				return cerr_term;
			}
			return false;
		}

		struct stack_point
		{
			int pStyle = (int)HC::style::reset;
			int pFG    = (int)HC::fg::reset;
			int pBG    = (int)HC::bg::reset;
		};

		static std::mutex                                                   pSync;
		static std::unordered_map<std::ostream *, std::vector<stack_point>> pStack;
		static std::unordered_map<std::ostream *, stack_point>              pCurrent;

		#ifdef IS_WINDOWS

		struct SGR // Select Graphic Rendition parameters for Windows console
		{  
			BYTE fgColor;  // foreground color (0-15) lower 3 rgb bits + intense bit
			BYTE bgColor;  // background color (0-15) lower 3 rgb bits + intense bit
			BYTE bold;  // emulated as FOREGROUND_INTENSITY bit
			BYTE underline;  // emulated as BACKGROUND_INTENSITY bit
			BOOLEAN inverse;  // swap foreground/bold & background/underline
			BOOLEAN conceal;  // set foreground/bold to background/underline
		};

		enum class AttrColor : BYTE // Color attributes for console screen buffer
		{  
			black   = 0,
			red     = 4,
			green   = 2,
			yellow  = 6,
			blue    = 1,
			magenta = 5,
			cyan    = 3,
			gray    = 7
		};

		HANDLE getConsoleHandle(const std::streambuf *osbuf) noexcept
		{
			if (osbuf == nobuf || osbuf == uobuf) 
			{
				static const HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
				return hStdout;
			}
			else if (osbuf == nebuf || osbuf == nlbuf || osbuf == uebuf || osbuf == ulbuf)
			{
				static const HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
				return hStderr;
			}
			return INVALID_HANDLE_VALUE;
		}

		bool setWinTermAnsiColors(const std::streambuf *osbuf) noexcept
		{
			HANDLE h = getConsoleHandle(osbuf);
			if (h == INVALID_HANDLE_VALUE) {
				return false;
			}
			DWORD dwMode = 0;
			if (!GetConsoleMode(h, &dwMode)) {
				return false;
			}
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (!SetConsoleMode(h, dwMode)) {
				return false;
			}
			return true;
		}

		bool supportsAnsi(const std::streambuf *osbuf) noexcept
		{
			using std::cerr;
			using std::clog;
			using std::cout;
			if (osbuf == nobuf || osbuf == uobuf)
			{
				static const bool cout_ansi
					= (isMsysPty(_fileno(stdout)) || setWinTermAnsiColors(osbuf));
				return cout_ansi;
			}
			else if (osbuf == nebuf || osbuf == nlbuf || osbuf == uebuf || osbuf == ulbuf)
			{
				static const bool cerr_ansi
					= (isMsysPty(_fileno(stderr)) || setWinTermAnsiColors(osbuf));
				return cerr_ansi;
			}
			return false;
		}

		const SGR &defaultState() noexcept
		{
			static const SGR defaultSgr = []() -> SGR {
				CONSOLE_SCREEN_BUFFER_INFO info;
				WORD attrib = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
											   &info)
					|| GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
												  &info)) {
					attrib = info.wAttributes;
				}
				SGR sgr     = { 0, 0, 0, 0, FALSE, FALSE };
				sgr.fgColor = attrib & 0x0F;
				sgr.bgColor = (attrib & 0xF0) >> 4;
				return sgr;
			}();
			return defaultSgr;
		}

		BYTE ansi2attr(BYTE rgb) noexcept
		{
			static const AttrColor rev[8]
				= { AttrColor::black,  AttrColor::red,  AttrColor::green,
				AttrColor::yellow, AttrColor::blue, AttrColor::magenta,
				AttrColor::cyan,   AttrColor::gray };
			return static_cast<BYTE>(rev[rgb]);
		}

		void setWinSGR(HC::bg col, SGR &state) noexcept
		{
			if (col != HC::bg::reset) {
				state.bgColor = ansi2attr(static_cast<BYTE>(col) - 40);
			} else {
				state.bgColor = defaultState().bgColor;
			}
		}

		void setWinSGR(HC::fg col, SGR &state) noexcept
		{
			if (col != HC::fg::reset) {
				state.fgColor = ansi2attr(static_cast<BYTE>(col) - 30);
			} else {
				state.fgColor = defaultState().fgColor;
			}
		}

		void setWinSGR(HC::bgB col, SGR &state) noexcept
		{
			state.bgColor = (BACKGROUND_INTENSITY >> 4)
				| ansi2attr(static_cast<BYTE>(col) - 100);
		}

		void setWinSGR(HC::fgB col, SGR &state) noexcept
		{
			state.fgColor
				= FOREGROUND_INTENSITY | ansi2attr(static_cast<BYTE>(col) - 90);
		}

		void setWinSGR(HC::style style, SGR &state) noexcept
		{
			switch (style)
			{
				case HC::style::reset:    state           = defaultState();       break;
				case HC::style::bold:     state.bold      = FOREGROUND_INTENSITY; break;
				case HC::style::underline:
				case HC::style::blink:    state.underline = BACKGROUND_INTENSITY; break;
				case HC::style::reversed: state.inverse   = TRUE;                 break;
				case HC::style::conceal:  state.conceal   = TRUE;                 break;
				default: break;
			}
		}

		SGR &current_state() noexcept
		{
			static SGR state = defaultState();
			return state;
		}

		WORD SGR2Attr(const SGR &state) noexcept
		{
			WORD attrib = 0;
			if (state.conceal)
			{
				if (state.inverse)
				{
					attrib = (state.fgColor << 4) | state.fgColor;
					if (state.bold)
						attrib |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
				} 
				else
				{
					attrib = (state.bgColor << 4) | state.bgColor;
					if (state.underline)
						attrib |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
				}
			} 
			else if (state.inverse) 
			{
				attrib = (state.fgColor << 4) | state.bgColor;

				if (state.bold)      attrib |= BACKGROUND_INTENSITY;
				if (state.underline) attrib |= FOREGROUND_INTENSITY;
			}
			else 
			{
				attrib = state.fgColor | (state.bgColor << 4) | state.bold | state.underline;
			}
			return attrib;
		}

		template <typename T>
		void setWinColorAnsi(std::ostream &os, T const value)
		{
			os << "\033[" << static_cast<int>(value) << "m";
		}

		template <typename T>
		void setWinColorNative(std::ostream &os, T const value)
		{
			const HANDLE h = getConsoleHandle(os.rdbuf());
			if (h != INVALID_HANDLE_VALUE) 
			{
				setWinSGR(value, current_state());
				// Out all buffered text to console with previous settings:
				os.flush();
				SetConsoleTextAttribute(h, SGR2Attr(current_state()));
			}
		}
		
		template <typename T>
		enableStd<T> setColorT(std::ostream &os, T const value)
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os];
				auto & ps = pStack[&os];
				if (ps.size() == 0)
					ps.push_back(pCurrent[&os]);
			}

			if (winTermMode() == winTerm::Auto)
			{
				if (supportsAnsi(os.rdbuf()))
					setWinColorAnsi(os, value);
				else
					setWinColorNative(os, value);
			} 
			else if (winTermMode() == winTerm::Ansi)
			{
				setWinColorAnsi(os, value);
			}
			else
			{
				setWinColorNative(os, value);
			}
			return os;
		}
		#else
		template <typename T>
		enableStd<T> setColorT(std::ostream &os, T const value)
		{
			return os << "\033[" << static_cast<int>(value) << "m";
		}
		#endif

		void push(std::ostream & os)
		{
			std::scoped_lock slock(pSync);
			pStack[&os].push_back(pCurrent[&os]);
		}

		void pop(std::ostream & os)
		{
			stack_point spc;
			{
				std::scoped_lock slock(pSync);
				stack_point & sp = pCurrent[&os];
				auto & ps = pStack[&os];
				sp = ps.back();
				spc = sp;
				if (ps.size() > 1)
					pStack[&os].pop_back();
			}

			setColorT(os, (HC::style)spc.pStyle);

			if (spc.pBG >= 90) // Bright, then
				setColorT(os, (HC::bgB)spc.pBG);
			else
				setColorT(os, (HC::bg )spc.pBG);

			if (spc.pFG >= 90) // Bright, then
				setColorT(os, (HC::fgB)spc.pFG);
			else
				setColorT(os, (HC::fg )spc.pFG);
		}

		std::ostream & setColor(std::ostream & os, HC::style const value) 
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os].pStyle = (int)value;
			}

			return setColorT(os, value); 
		}

		std::ostream & setColor(std::ostream & os, HC::fg    const value)
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os].pFG = (int)value;
			}

			return setColorT(os, value); 
		}

		std::ostream & setColor(std::ostream & os, HC::bg    const value) 
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os].pBG = (int)value;
			}

			return setColorT(os, value);
		}

		std::ostream & setColor(std::ostream & os, HC::fgB   const value)
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os].pFG = (int)value;
			}

			return setColorT(os, value); 
		}

		std::ostream & setColor(std::ostream & os, HC::bgB   const value)
		{
			{
				std::scoped_lock slock(pSync);
				pCurrent[&os].pBG = (int)value;
			}

			return setColorT(os, value); 
		}

		std::ostream & setColor(std::ostream & os, HC::st    const value)
		{
			if (value == st::push)
				push(os); 
			else if (value == st::pop)
				pop(os); 
			return os; }

	}  // namespace detail
}  // namespace HC

namespace HANDY_NS::Console
{
	/// Much of this was borrowed from DrEcho (https://github.com/r-lyeh-archived/DrEcho/blob/master/LICENSE)
	/// Still some clean up to do, but it works just fine.

	static std::unordered_map<std::string, int> HighlightWords;
	static StopWatch const                      AppStartEpoch;
	static std::function<void(std::string const &)>     Function;
	static std::mutex Mutex;

	void SetOutputCallback(std::function<void(std::string const &)> func)
	{
		Function = func;
	}

	class AutoFormattingStreamBuf : public std::streambuf
	{
	public:
		bool isNewLine = true;
		bool firstNewLine = true;

		bool ignoreUntilControlEnd = false;

		std::vector<std::string> split(std::string const & str, char sep)
		{
			std::vector<std::string> tokens;
			tokens.push_back(std::string());

			for (std::string::const_iterator it = str.begin(), end = str.end(); it != end; ++it)
			{
				if (*it == sep)
				{
					tokens.push_back(std::string() + sep);
					tokens.push_back(std::string());
				}
				else
				{
					tokens.back() += *it;
				}
			}

			return tokens;
		}

		void log(std::string const & text)
		{
			std::lock_guard<std::mutex> lg(Mutex);

			if (!text.size())
				return;

			std::vector<std::string> lines = split(text, '\n');

			for (std::vector<std::string>::iterator it = lines.begin(), end = lines.end(); it != end; ++it)
			{
				if (*it == "\n")
				{
					isNewLine = true;
					continue;
				}

				std::vector<std::string> tags = TokenizeWSep(*it, "!\"#~$%&/(){}[]|,;.:<>+-/*@'\"\t\n\\ ");

				if (isNewLine && tags.size() > 0)
				{
					ncout
						<< (firstNewLine ? "" : "\n")
						<< HC::st::push << HC::fgB::black
						<< HANDY_NS::Format("%08.3fs ", fmod(AppStartEpoch.SecondsD(), 10000.)) 
						<< HC::fgB::gray << "| " << HC::st::pop;

					if (Function)
					{
						std::string lineBegin = (firstNewLine ? "" : "\n");

						lineBegin += HANDY_NS::Format("%08.3fs ", fmod(AppStartEpoch.SecondsD(), 10000.)) + "| "s;

						Function(lineBegin);
					}

					isNewLine = false;
					firstNewLine = false;
				}

				for (auto & tag : tags) 
				{
					if (Function)
					{
						/// We're going to assume ANYTHING with an x1b character is garbage until the first letter (not checking for the [ character)

						std::string noAnsiChars;

						for (char c : tag)
						{
							if (ignoreUntilControlEnd)
							{
								if (Handy::IsLetterASCII(c))
									ignoreUntilControlEnd = false;

								continue;
							}

							if (c == '\x1b')
							{
								ignoreUntilControlEnd = true;
								continue;
							}

							noAnsiChars.push_back(c);
						}

						if (!noAnsiChars.empty())
							Function(noAnsiChars);
					}

					auto find = HighlightWords.find(tag);

					if (find == HighlightWords.end())
						ncout << tag;
					else
					{
						HC::fgB color = (HC::fgB)find->second;
						ncout << HC::st::push << color << tag << HC::st::pop;
					}


				}
			}

			ncout << std::flush;
		}

		int_type        overflow(int_type c = traits_type::eof())      override { return log(std::string() + (char)(c)), 1; }
		std::streamsize xsputn  (const char *c_str, std::streamsize n) override { return log(std::string(c_str, (unsigned)n)), n; }
	};
	
	struct captured_ostream
	{
		std::streambuf *        Original = 0;
		AutoFormattingStreamBuf sb;
	};

	static std::unordered_set<std::ostream *>            CapturedStreams;


	std::unordered_map<std::ostream *, captured_ostream> Loggers;

	void AddHighlights(HC::fgB color, std::vector<std::string> const & user_highlights)
	{
		for (auto & highlight : user_highlights)
			HighlightWords[highlight] = (int)color;
	}

	//std::vector<std::string> Highlights(Color color) 
	//{
	//	std::vector<std::string> out;
	//	for (auto &hl : HighlightWords) 
	//	{
	//		if (hl.second == color)
	//			out.push_back(hl.first);
	//	}
	//	return out;
	//}

	void Attach(std::ostream &_os)
	{
		std::ostream *os = &_os;

		Loggers[os];

		if (!Loggers[os].Original)
		{
			// capture ostream
			Loggers[os].Original = os->rdbuf(&Loggers[os].sb);
		}

		if (os == &std::cout)
			uobuf = &Loggers[os].sb;

		if (os == &std::cerr)
			uebuf = &Loggers[os].sb;

		if (os == &std::clog)
			ulbuf = &Loggers[os].sb;
	}

	void Detach(std::ostream &_os)
	{
		std::ostream *os = &_os;

		Attach(_os);

		os->rdbuf(Loggers[os].Original); // release original stream
	}

	bool Capture(std::ostream &os_)
	{
		std::lock_guard<std::mutex> lg(Mutex);
		
		std::ostream *os = &os_;
		if (CapturedStreams.find(os) == CapturedStreams.end())
		{
			CapturedStreams.insert(os);
			Attach(*os);
			return true;
		}
		return false;
	}

	bool Release(std::ostream &os_) 
	{
		std::lock_guard<std::mutex> lg(Mutex);
		
		std::ostream *os = &os_;
		if (CapturedStreams.find(os) != CapturedStreams.end())
		{
			CapturedStreams.erase(os);
			Detach(*os);
			return true;
		}
		return false;
	}
}


namespace HANDY_NS {

	void ClearErrors()
	{
		errno = 0;
		$win(SetLastError(ERROR_SUCCESS));
	}

	std::string GetAnyError()
	{
		std::string err, os;

		if (errno)
		{
			err.resize(2048);

			$msvc(strerror_s(&err[0], err.size(), errno));
			$melse(strcpy(&err[0], strerror(errno)));

			err = std::string("(errno ") + std::to_string(errno) + ": " + err.c_str() + std::string(")");
		}

		$win(
			DWORD os_code = GetLastError();

		if (os_code)
		{
			LPVOID lpMsgBuf;
			DWORD buflen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				os_code,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL
			);

			if (buflen > 2)
			{
				std::string text((const char *)lpMsgBuf, buflen - 2); // remove \r\n
				os = std::string("(w32errno ") + std::to_string(os_code) + ": " + text + ")";
			}

			if (buflen)
			{
				LocalFree(lpMsgBuf);
			}
		}
		)

			return err + os;
	}
}
