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

#include <cassert>
#include <csignal>
#include <cstdint>
#include <exception>
#include <vector>

#include "../Handy.hpp"

// Run any of these static methods to trigger the calamitous error condition of your choosing!
// Used for testing of error handling.

//     https://spin.atomicobject.com/2013/01/13/exceptions-stack-traces-c/
//     https://www.codeproject.com/Articles/154686/SetUnhandledExceptionFilter-and-the-C-C-Runtime-Li

namespace HANDY_NS {

	class Calamity
	{
	public:
		static void DivideByZero();
		static void InvalidAccess();
		static void StackOverflow();
		static void AssertFalse();
		static void AbortCall();
		static void OutOfBoundsStdVector();
		static void UninitedVirtCallInCtor();
		static void InfiniteLoop(); // break out with ctrl+c to test SIGINT handling
		static void IllegalInstruction();
		static void ThrowStdException();
		static void ThrowNumber42();

		class CustomException { };
		static void ThrowCustomException();
	};

	static uint64_t calamity_helper_so(volatile uint64_t & i) { if (i < 1) return i; return calamity_helper_so(++i); }

	/*static*/ inline void Calamity::DivideByZero()  { volatile uint64_t a = 1, b = 1; b--; volatile uint64_t c = a / b; }
	/*static*/ inline void Calamity::InvalidAccess() { char * p = 0; *p = 5; }
	/*static*/ inline void Calamity::StackOverflow() { volatile uint64_t i = 1; calamity_helper_so(i); }
	/*static*/ inline void Calamity::AssertFalse()   { assert(false); }
	/*static*/ inline void Calamity::AbortCall()     { abort(); }
	/*static*/ inline void Calamity::OutOfBoundsStdVector() { std::vector<int> v; v[0] = 5; }
	/*static*/ inline void Calamity::InfiniteLoop()         { while(true) {}; }
	/*static*/ inline void Calamity::IllegalInstruction()   { raise(SIGILL); } // I couldn't find an easy way to cause this one, so I'm cheating
	/*static*/ inline void Calamity::ThrowStdException()    { throw std::exception(); }
	/*static*/ inline void Calamity::ThrowNumber42()        { throw 42; }
	/*static*/ inline void Calamity::ThrowCustomException() { throw CustomException(); }	
	/*static*/ inline void Calamity::UninitedVirtCallInCtor()
	{
		struct B
		{
			B() { Bar(); }
			virtual void Foo() = 0;
			void Bar() { Foo(); }
		};

		struct D: public B
		{
			void Foo() { }
		};

		B * b = new D;
		// Just to silence the warning C4101:
		//    'VirtualFunctionCallCrash::B::Foo' : unreferenced local variable
		b->Foo(); 
	}
}
