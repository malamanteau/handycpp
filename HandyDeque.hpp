
/// ================================================================================
/// Microsoft's implementation of std::deque, but with different tuning values
/// Downloaded on: 9/18/2019 
///     https://github.com/microsoft/STL/blob/master/stl/inc/deque
///
/// LICENSE - Apache-2.0 WITH LLVM-exception
/// 
/// Copyright (c) Microsoft Corporation.
/// 
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
/// 
/// http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
/// 
/// As an exception, if, as a result of your compiling your source code, 
/// portions of this Software are embedded into an Object form of such 
/// source code, you may redistribute such embedded portions in such Object 
/// form without complying with the conditions of Sections 4(a), 4(b) and 
/// 4(d) of the License.
/// 
/// In addition, if you combine or link compiled forms of this Software with 
/// software that is licensed under the GPLv2 ("Combined Software") and if a 
/// court of competent jurisdiction determines that the patent provision 
/// (Section 3), the indemnity provision (Section 9) or other Section of the 
/// License conflicts with the conditions of the GPLv2, you may retroactively 
/// and prospectively choose to deem waived or otherwise exclude such 
/// Section(s) of the License, but only in their entirety and only with respect 
/// to the Combined Software.
/// ================================================================================

#pragma once

#include "HandyBase.hpp"

#if defined IS_WINDOWS

#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <algorithm>


/// DEQUE PARAMETERS --- THESE MUST BE POWERS OF TWO:
/// ORIGINAL MSSTL VERSION: 
//#define HTD_DEQUEMAPSIZE 8 // minimum map size, at least 1
/*#define HTD_DEQUESIZE                           \
	( sizeof(value_type) <= 1 ? 16               \
	: sizeof(value_type) <= 2 ? 8       \
	: sizeof(value_type) <= 4 ? 4       \
	: sizeof(value_type) <= 8 ? 2 : 1) // elements per block (a power of 2)
*/
/// LIBC++ VALUES:
#define HTD_DEQUEMAPSIZE 8 // minimum map size, at least 1
#define HTD_DEQUESIZE (sizeof(value_type) < 256 ? 4096 / sizeof(value_type) : 16) // elements per block (a power of 2)

#define HTD_ITERATOR_DEBUG_LEVEL 0

#define HTD_HAS_CXX17	1

// Enforcement of matching allocator value_types
#ifndef HTD_ENFORCE_MATCHING_ALLOCATORS
	#define HTD_ENFORCE_MATCHING_ALLOCATORS	HTD_HAS_CXX17
#endif /* HTD_ENFORCE_MATCHING_ALLOCATORS */

#define HTD_MISMATCHED_ALLOCATOR_MESSAGE(_CONTAINER, _VALUE_TYPE) \
	_CONTAINER " requires that Allocator's value_type match " _VALUE_TYPE \
	" (See N4659 26.2.1 [container.requirements.general]/16 allocator_type)" \
	" Either fix the allocator value_type or define _ENFORCE_MATCHING_ALLOCATORS=0" \
	" to suppress this diagnostic."


// warning: constexpr if is a C++17 extension [-Wc++17-extensions]
// warning: user-defined literal suffixes not starting with '_' are reserved [-Wuser-defined-literals]
// warning: unknown pragma ignored [-Wunknown-pragmas]
#ifndef HTD_STL_DISABLE_CLANG_WARNINGS
 #ifdef __clang__
  #define HTD_STL_DISABLE_CLANG_WARNINGS	_Pragma("clang diagnostic push")	\
	_Pragma("clang diagnostic ignored \"-Wc++17-extensions\"")				\
	_Pragma("clang diagnostic ignored \"-Wuser-defined-literals\"")			\
	_Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
 #else /* __clang__ */
  #define HTD_STL_DISABLE_CLANG_WARNINGS
 #endif /* __clang__ */
#endif /* HTD_STL_DISABLE_CLANG_WARNINGS */

#ifndef HTD_STL_RESTORE_CLANG_WARNINGS
 #ifdef __clang__
  #define HTD_STL_RESTORE_CLANG_WARNINGS	_Pragma("clang diagnostic pop")
 #else /* __clang__ */
  #define HTD_STL_RESTORE_CLANG_WARNINGS
 #endif /* __clang__ */
#endif /* HTD_STL_RESTORE_CLANG_WARNINGS */


namespace htd {
	template<class _Ptrty> inline auto  Unfancy(_Ptrty _Ptr) { return (std::addressof(*_Ptr)); } // converts from a fancy pointer to a plain pointer
	template<class _Ty>    inline _Ty * Unfancy(_Ty *  _Ptr) { return (_Ptr); }                  // do nothing for plain pointers

	template <class _Ty, class... _Types>
	void Construct_in_place(_Ty& _Obj, _Types&&... _Args) noexcept(std::is_nothrow_constructible_v<_Ty, _Types...>)
	{
		::new (const_cast<void*>(static_cast<const volatile void*>(std::addressof(_Obj)))) _Ty(std::forward<_Types>(_Args)...);
	}

	template<class _Ty>
	constexpr const _Ty& Min_value(const _Ty& _Left, const _Ty& _Right) noexcept(noexcept(_Right < _Left))
	{	// return smaller of _Left and _Right
		return (_Right < _Left ? _Right : _Left);
	}

	template<class _Ty>
	constexpr const _Ty& Max_value(const _Ty& _Left, const _Ty& _Right) noexcept(noexcept(_Left < _Right))
	{	// return larger of _Left and _Right
		return (_Left < _Right ? _Right : _Left);
	}

	template <class _Ty, class = void> struct Is_allocator : std::false_type {}; // selected when _Ty can't possibly be an allocator
	template <class _Ty>               struct Is_allocator
		<_Ty, std::void_t<typename _Ty::value_type, decltype(std::declval<_Ty&>().deallocate(std::declval<_Ty&>().allocate(size_t{1}), size_t{1}))>>
		: std::true_type { }; // selected when _Ty resembles an allocator, N4687 26.2.1 [container.requirements.general]/17
	
	template <class _Alloc> // tests if allocator has simple addressing
	inline constexpr bool Is_simple_alloc_v =
		std::is_same_v<typename std::allocator_traits<_Alloc>::size_type, size_t>&&
		std::is_same_v<typename std::allocator_traits<_Alloc>::difference_type, ptrdiff_t>&&
		std::is_same_v<typename std::allocator_traits<_Alloc>::pointer, typename _Alloc::value_type*>&&
		std::is_same_v<typename std::allocator_traits<_Alloc>::const_pointer, const typename _Alloc::value_type*>;

	template <class _Alloc>                    using Alloc_ptr_t    = typename std::allocator_traits<_Alloc>::pointer;
	template <class _Alloc, class _Value_type> using Rebind_alloc_t = typename std::allocator_traits<_Alloc>::template rebind_alloc<_Value_type>;
	template <class _Iter>                     using Iter_ref_t     = typename std::iterator_traits<_Iter>::reference;
	template <class _Iter>                     using Iter_value_t   = typename std::iterator_traits<_Iter>::value_type;
	template <class _Iter>                     using Iter_cat_t     = typename std::iterator_traits<_Iter>::iterator_category;

	template <class _Ty, class = void> inline constexpr bool Is_iterator_v = false;
	template <class _Ty>               inline constexpr bool Is_iterator_v<_Ty, std::void_t<Iter_cat_t<_Ty>>> = true;
	template <class _Ty> struct Is_iterator : std::bool_constant<Is_iterator_v<_Ty>> {};

	struct Equal_allocators {};                      // usually allows contents to be stolen (e.g. with swap)
	using Propagate_allocators    = std::true_type;  // usually allows the allocator to be propagated, and then contents stolen
	using No_propagate_allocators = std::false_type; // usually turns moves into copies

	// Choose_pocca returns whether an attempt to propagate allocators is necessary in copy assignment operations.
	// Note that even when false_type, callers should call _Pocca as we want to assign allocators even when equal.
	template <class _Alloc>
	using Choose_pocca = std::bool_constant<std::allocator_traits<_Alloc>::propagate_on_container_copy_assignment::value
		&& !std::allocator_traits<_Alloc>::is_always_equal::value>;

	template <class _Alloc>
	using Choose_pocma = std::conditional_t<std::allocator_traits<_Alloc>::is_always_equal::value, Equal_allocators,
		typename std::allocator_traits<_Alloc>::propagate_on_container_move_assignment::type>;

	template <class NoThrowFwdIt>
	inline constexpr bool Use_memset_value_construct_v = std::conjunction_v<std::is_pointer<NoThrowFwdIt>,
		std::is_scalar<Iter_value_t<NoThrowFwdIt>>, std::negation<std::is_volatile<std::remove_reference_t<Iter_ref_t<NoThrowFwdIt>>>>,
		std::negation<std::is_member_pointer<Iter_value_t<NoThrowFwdIt>>>>;

	template <class _Ptr>
	_Ptr Zero_range(const _Ptr _First, const _Ptr _Last) // fill [_First, _Last) with zeroes
	{ 
		char* const _First_ch = reinterpret_cast<char*>(_First);
		char* const _Last_ch  = reinterpret_cast<char*>(_Last);
		memset(_First_ch, 0, static_cast<size_t>(_Last_ch - _First_ch));
		return _Last;
	}


	template <class NoThrowFwdIt>
	struct Uninitialized_backout // struct to undo partially constructed ranges in _Uninitialized_xxx algorithms
	{
		NoThrowFwdIt _First;
		NoThrowFwdIt _Last;

		explicit Uninitialized_backout(NoThrowFwdIt _Dest) : _First(_Dest), _Last(_Dest) {}
		Uninitialized_backout(NoThrowFwdIt _First_, NoThrowFwdIt _Last_) : _First(_First_), _Last(_Last_) {}

		Uninitialized_backout(const Uninitialized_backout&) = delete;
		Uninitialized_backout& operator=(const Uninitialized_backout&) = delete;

		~Uninitialized_backout() { Destroy_range(_First, _Last); }

		template <class... _Types>
		void _Emplace_back(_Types&&... _Vals)
		{ // construct a new element at *_Last and increment
			Construct_in_place(*_Last, std::forward<_Types>(_Vals)...);
			++_Last;
		}

		NoThrowFwdIt _Release()
		{ // suppress any exception handling backout and return _Last
			_First = _Last;
			return _Last;
		}
	};

	template <class NoThrowFwdIt, class _Diff>
	NoThrowFwdIt Uninitialized_value_construct_n_unchecked1(NoThrowFwdIt _UFirst, _Diff _Count)
	{
		// value-initialize all elements in [_UFirst, _UFirst + _Count_raw)
		if constexpr (Use_memset_value_construct_v<NoThrowFwdIt>) 
		{
			return Zero_range(_UFirst, _UFirst + _Count);
		}
		else
		{
			Uninitialized_backout<NoThrowFwdIt> _Backout{ _UFirst };
			for (; 0 < _Count; --_Count) 
			{
				_Backout._Emplace_back();
			}

			return _Backout._Release();
		}
	}

	template <class _Ty>
	void Destroy_in_place(_Ty& _Obj) noexcept
	{
		_Obj.~_Ty();
	}

	template <class _NoThrowFwdIt>
	void Destroy_range(_NoThrowFwdIt _First, const _NoThrowFwdIt _Last) noexcept
	{
		// note that this is an optimization for debug mode codegen; in release mode the BE removes all of this
		if constexpr(std::is_trivially_destructible_v<Iter_value_t<_NoThrowFwdIt>>) 
		{
			(void) _First;
			(void) _Last;
		}
		else
		{
			for (; _First != _Last; ++_First)
			{
				Destroy_in_place(*_First);
			}
		}
	}

	template <class _Iter, class _Sentinel>
	constexpr void Adl_verify_range(const _Iter& _First, const _Sentinel& _Last) {
		// check that [_First, _Last) forms an iterator range
		//if constexpr (_Range_verifiable_v<_Iter, _Sentinel>) 
		//{
		//	_Verify_range(_First, _Last);
		//} 
		//else
		{
			(void) _First; // TRANSITION, VSO#486357
			(void) _Last; // TRANSITION, VSO#486357
		}
	}


	template <class _Alloc>
	void Deallocate_plain(_Alloc& _Al, typename _Alloc::value_type* const _Ptr) noexcept 
	{
		// deallocate a plain pointer using an allocator
		using _Alloc_traits = std::allocator_traits<_Alloc>;
		if constexpr (std::is_same_v<Alloc_ptr_t<_Alloc>, typename _Alloc::value_type*>)
		{
			_Alloc_traits::deallocate(_Al, _Ptr, 1);
		} 
		else
		{
			using _Ptr_traits = std::pointer_traits<Alloc_ptr_t<_Alloc>>;
			_Alloc_traits::deallocate(_Al, _Ptr_traits::pointer_to(*_Ptr), 1);
		}
	}

	template <class _Alloc>
	void Delete_plain_internal(_Alloc& _Al, typename _Alloc::value_type* const _Ptr) noexcept {
		// destroy *_Ptr in place, then deallocate _Ptr using _Al; used for internal container types the user didn't name
		using _Ty = typename _Alloc::value_type;
		_Ptr->~_Ty();
		Deallocate_plain(_Al, _Ptr);
	}

	template <class _Ty> struct Wrap { _Ty _Value; }; // workaround for "T^ is not allowed in a union"

	template <class _Alloc>
	struct Alloc_temporary
	{
		using value_type = typename _Alloc::value_type;
		using _Traits    = std::allocator_traits<_Alloc>;

		_Alloc& _Al;
		union 
		{
			Wrap<value_type> _Storage;
		};

		template <class... _Args>
		explicit Alloc_temporary(_Alloc& _Al_, _Args&&... _Vals) noexcept(
			noexcept(_Traits::construct(_Al_, std::addressof(_Storage._Value), std::forward<_Args>(_Vals)...)))
			: _Al(_Al_) {
			_Traits::construct(_Al, std::addressof(_Storage._Value), std::forward<_Args>(_Vals)...);
		}

		Alloc_temporary(const Alloc_temporary&) = delete;
		Alloc_temporary& operator=(const Alloc_temporary&) = delete;

		~Alloc_temporary() 
		{
			_Traits::destroy(_Al, std::addressof(_Storage._Value));
		}
	};

	template <class _Iter, class = void>
	struct Allow_inheriting_unwrap : std::true_type {};

	template <class _Iter>
	struct Allow_inheriting_unwrap<_Iter, std::enable_if_t<!std::is_same_v<_Iter, typename _Iter::_Prevent_inheriting_unwrap>>>
		: std::false_type {};
	   
	template <class _Iter, class = void> struct Unwrappable : std::false_type {};
	template <class _Iter>               struct Unwrappable<_Iter, std::void_t<decltype(std::declval<_Iter&>()._Seek_to(std::declval<const _Iter&>()._Unwrapped()))>>
		: Allow_inheriting_unwrap<_Iter>::type {};

	template <class _Iter> inline constexpr bool Unwrappable_v = Unwrappable<_Iter>::value;
	template <class _Iter, std::enable_if_t<Unwrappable_v<_Iter>, int> = 0>
	[[nodiscard]] constexpr auto Get_unwrapped(const _Iter& _It) { return _It._Unwrapped(); } // unwrap an iterator previously subjected to Adl_verify_range or otherwise validated
	template <class _Iter, std::enable_if_t<!Unwrappable_v<_Iter>, int> = 0>
	[[nodiscard]] constexpr const _Iter& Get_unwrapped(const _Iter& _It) { return _It; } // (don't) unwrap an iterator previously subjected to Adl_verify_range or otherwise validated
	template <class _Iter, std::enable_if_t<!Unwrappable_v<_Iter>, int> = 0>
	[[nodiscard]] constexpr const _Iter&& Get_unwrapped(const _Iter&& _It) { return static_cast<const _Iter&&>(_It); } // (don't) unwrap an iterator previously subjected to Adl_verify_range or otherwise validated
	template <class _Ty>
	[[nodiscard]] constexpr _Ty* Get_unwrapped(_Ty* const _Ptr) { return _Ptr; } // special case already-unwrapped pointers 

	template <class _Ty>
	void Swap_adl(_Ty& _Left, _Ty& _Right) noexcept(std::is_nothrow_swappable<_Ty>::value) { std::swap(_Left, _Right); }

	struct Container_base12;
	struct Iterator_base12;
	
	struct Container_proxy // store head of iterator chain and back pointer
	{
		Container_proxy() noexcept : _Mycont(nullptr), _Myfirstiter(nullptr) {}
		Container_proxy(Container_base12* _Mycont_) noexcept : _Mycont(_Mycont_), _Myfirstiter(nullptr) {}

		const Container_base12* _Mycont;
		Iterator_base12* _Myfirstiter;
	};

	struct Container_base12 
	{
	public:
		Container_base12() noexcept : _Myproxy(nullptr) {}

		Container_base12(const Container_base12&) = delete;
		Container_base12& operator=(const Container_base12&) = delete;

		void _Orphan_all() noexcept;
		void _Swap_proxy_and_iterators(Container_base12&) noexcept;

		template <class _Alloc>
		void _Alloc_proxy(_Alloc&& _Al)
		{
			Container_proxy* const _New_proxy = Unfancy(_Al.allocate(1));
			Construct_in_place(*_New_proxy, this);
			_Myproxy = _New_proxy;
			_New_proxy->_Mycont = this;
		}

		template <class _Alloc>
		void _Reload_proxy(_Alloc&& _Old_alloc, _Alloc&& _New_alloc)
		{
			// pre: no iterators refer to the existing proxy
			Container_proxy* const _New_proxy = Unfancy(_New_alloc.allocate(1));
			Construct_in_place(*_New_proxy, this);
			_New_proxy->_Mycont = this;
			Delete_plain_internal(_Old_alloc, std::exchange(_Myproxy, _New_proxy));
		}

		Container_proxy* _Myproxy;
	};

	struct BasicContainer_proxy_ptr12 // smart pointer components for a Container_proxy * that don't depend on the allocator
	{
		Container_proxy* _Ptr;

		void _Release() noexcept { // disengage this BasicContainer_proxy_ptr12
			_Ptr = nullptr;
		}

	protected:
		BasicContainer_proxy_ptr12()                                    = default;
		BasicContainer_proxy_ptr12(const BasicContainer_proxy_ptr12&) = delete;
		BasicContainer_proxy_ptr12(BasicContainer_proxy_ptr12&&)      = delete;
	};

	struct Leave_proxy_unbound {}; // tag to indicate that a proxy is being allocated before it is safe to bind to a
									// _Container_base12


	template <class _Alloc>
	struct Container_proxy_ptr12 : BasicContainer_proxy_ptr12 
	{
		// smart pointer components for a Container_proxy * for an allocator family
		_Alloc& _Al;

		Container_proxy_ptr12(_Alloc& _Al_, Leave_proxy_unbound) : _Al(_Al_) // create a new unbound Container_proxy
		{
			_Ptr = Unfancy(_Al_.allocate(1));
			Construct_in_place(*_Ptr);
		}

		Container_proxy_ptr12(_Alloc& _Al_, Container_base12& _Mycont) // create a new Container_proxy pointing at _Mycont
			: _Al(_Al_) 
		{ 
			_Ptr = Unfancy(_Al_.allocate(1));
			Construct_in_place(*_Ptr, std::addressof(_Mycont));
			_Mycont._Myproxy = _Ptr;
		}

		void _Bind(_Alloc& _Old_alloc, Container_base12* _Mycont) noexcept 
		{
			// Attach the proxy stored in *this to _Mycont, and destroy _Mycont's existing proxy
			// with _Old_alloc. Requires that no iterators are alive referring to _Mycont.
			_Ptr->_Mycont = _Mycont;
			Delete_plain_internal(_Old_alloc, std::exchange(_Mycont->_Myproxy, std::exchange(_Ptr, nullptr)));
		}

		~Container_proxy_ptr12()
		{
			if (_Ptr)
				Delete_plain_internal(_Al, _Ptr);
		}
	};


	struct Iterator_base12 // store links to container proxy, next iterator
	{ 
		Iterator_base12() noexcept : _Myproxy(nullptr), _Mynextiter(nullptr) {} // construct orphaned iterator

		Iterator_base12(const Iterator_base12& _Right) noexcept : _Myproxy(nullptr), _Mynextiter(nullptr) {
			*this = _Right;
		}

		Iterator_base12& operator=(const Iterator_base12& _Right) noexcept
		{
			if (_Myproxy != _Right._Myproxy)
			{
				if (_Right._Myproxy)
				{
					_Adopt(_Right._Myproxy->_Mycont);
				}
				else { // becoming invalid, disown current parent
					#if HTD_ITERATOR_DEBUG_LEVEL == 2
					_Lockit _Lock(_LOCK_DEBUG);
					_Orphan_me();
					#else // HTD_ITERATOR_DEBUG_LEVEL == 2
					_Myproxy = nullptr;
					#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
				}
			}

			return *this;
		}

		~Iterator_base12() noexcept
		{
			#if HTD_ITERATOR_DEBUG_LEVEL == 2
			_Lockit _Lock(_LOCK_DEBUG);
			_Orphan_me();
			#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
		}

		void _Adopt(const Container_base12* _Parent) noexcept
		{
			if (_Parent)
			{// have a parent, do adoption
				Container_proxy* _Parent_proxy = _Parent->_Myproxy;

				#if HTD_ITERATOR_DEBUG_LEVEL == 2
				if (_Myproxy != _Parent_proxy)
				{ // change parentage
					_Lockit _Lock(_LOCK_DEBUG);
					_Orphan_me();
					_Mynextiter = _Parent_proxy->_Myfirstiter;
					_Parent_proxy->_Myfirstiter = this;
					_Myproxy = _Parent_proxy;
				}

				#else // HTD_ITERATOR_DEBUG_LEVEL == 2
				_Myproxy = _Parent_proxy;
				#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
			}
			else
			{// no future parent, just disown current parent
				#if HTD_ITERATOR_DEBUG_LEVEL == 2
				_Lockit _Lock(_LOCK_DEBUG);
				_Orphan_me();
				#else // HTD_ITERATOR_DEBUG_LEVEL == 2
				_Myproxy = nullptr;
				#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
			}
		}

		const Container_base12* _Getcont() const noexcept { return _Myproxy ? _Myproxy->_Mycont : nullptr; }

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		void _Orphan_me() noexcept 
		{
			if (_Myproxy) { // adopted, remove self from list
				Iterator_base12** _Pnext = &_Myproxy->_Myfirstiter;
				while (*_Pnext && *_Pnext != this) {
					_Pnext = &(*_Pnext)->_Mynextiter;
				}

				_STL_VERIFY(*_Pnext, "ITERATOR LIST CORRUPTED!");
				*_Pnext = _Mynextiter;
				_Myproxy = nullptr;
			}
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		static constexpr bool _Unwrap_when_unverified = HTD_ITERATOR_DEBUG_LEVEL == 0;

		Container_proxy* _Myproxy;
		Iterator_base12* _Mynextiter;
	};

	// MEMBER FUNCTIONS FOR Container_base12
	inline void Container_base12::_Orphan_all() noexcept 
	{
		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		if (_Myproxy) { // proxy allocated, drain it
			_Lockit _Lock(_LOCK_DEBUG);

			for (auto _Pnext = &_Myproxy->_Myfirstiter; *_Pnext; *_Pnext = (*_Pnext)->_Mynextiter) {
				(*_Pnext)->_Myproxy = nullptr;
			}

			_Myproxy->_Myfirstiter = nullptr;
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
	}

	inline void Container_base12::_Swap_proxy_and_iterators(Container_base12& _Right) noexcept
	{
		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_Lockit _Lock(_LOCK_DEBUG);
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		Container_proxy* _Temp = _Myproxy;
		_Myproxy = _Right._Myproxy;
		_Right._Myproxy = _Temp;

		if (_Myproxy)
			_Myproxy->_Mycont = this;
		

		if (_Right._Myproxy) 
			_Right._Myproxy->_Mycont = &_Right;
		
	}



	struct Zero_then_variadic_args_t {}; // tag type for value-initializing first, constructing second from remaining args
	struct  One_then_variadic_args_t {}; // tag type for constructing first from one arg, constructing second from remaining args

	template <class _Ty1, class _Ty2, bool = std::is_empty_v<_Ty1> && !std::is_final_v<_Ty1>>
	class Compressed_pair final : private _Ty1 { // store a pair of values, deriving from empty first
	public:
		_Ty2 _Myval2;

		using _Mybase = _Ty1; // for visualization

		template <class... _Other2>
		constexpr explicit Compressed_pair(Zero_then_variadic_args_t, _Other2&&... _Val2) noexcept(
			std::conjunction_v<std::is_nothrow_default_constructible<_Ty1>, std::is_nothrow_constructible<_Ty2, _Other2...>>)
			: _Ty1(), _Myval2(std::forward<_Other2>(_Val2)...) { }

		template <class _Other1, class... _Other2>
		Compressed_pair(One_then_variadic_args_t, _Other1&& _Val1, _Other2&&... _Val2) noexcept(
			std::conjunction_v<std::is_nothrow_constructible<_Ty1, _Other1>, std::is_nothrow_constructible<_Ty2, _Other2...>>)
			: _Ty1(std::forward<_Other1>(_Val1)), _Myval2(std::forward<_Other2>(_Val2)...) { }

		      _Ty1& _Get_first()       noexcept { return *this; }
		const _Ty1& _Get_first() const noexcept { return *this; }
	};

	template <class _Ty1, class _Ty2>
	class Compressed_pair<_Ty1, _Ty2, false> final { // store a pair of values, not deriving from first
	public:
		_Ty1 _Myval1;
		_Ty2 _Myval2;

		template <class... _Other2>
		constexpr explicit Compressed_pair(Zero_then_variadic_args_t, _Other2&&... _Val2) noexcept(
			std::conjunction_v<std::is_nothrow_default_constructible<_Ty1>, std::is_nothrow_constructible<_Ty2, _Other2...>>)
			: _Myval1(), _Myval2(std::forward<_Other2>(_Val2)...) {}

		template <class _Other1, class... _Other2>
		Compressed_pair(One_then_variadic_args_t, _Other1&& _Val1, _Other2&&... _Val2) noexcept(
			std::conjunction_v<std::is_nothrow_constructible<_Ty1, _Other1>, std::is_nothrow_constructible<_Ty2, _Other2...>>)
			: _Myval1(std::forward<_Other1>(_Val1)), _Myval2(std::forward<_Other2>(_Val2)...) {}

		_Ty1& _Get_first() noexcept {
			return _Myval1;
		}

		const _Ty1& _Get_first() const noexcept {
			return _Myval1;
		}
	};


	template <class _Ty>
	struct Tidy_guard
	{ // class with destructor that calls _Tidy
		_Ty* _Target;
		~Tidy_guard()
		{
			if (_Target) 
				_Target->_Tidy();
		}
	};


}


#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
HTD_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

namespace htd {

// CLASS TEMPLATE DequeUnchecked_const_iterator
template <class _Mydeque>
class DequeUnchecked_const_iterator
{
private:
	using _Size_type = typename _Mydeque::size_type;

public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::const_pointer;
	using reference       = const value_type&;

	DequeUnchecked_const_iterator() noexcept : _Mycont(), _Myoff(0) {}

	DequeUnchecked_const_iterator(_Size_type _Off, const Container_base12* _Pdeque) noexcept
		: _Mycont(static_cast<const _Mydeque*>(_Pdeque)), _Myoff(_Off) {}

	[[nodiscard]] reference operator*() const
	{
		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % HTD_DEQUESIZE;
		return _Mycont->_Map[_Block][_Off];
	}

	[[nodiscard]] pointer operator->() const { return std::pointer_traits<pointer>::pointer_to(**this); }

	DequeUnchecked_const_iterator& operator++()    { ++_Myoff; return *this; }
	DequeUnchecked_const_iterator& operator--()    { --_Myoff; return *this; }
	DequeUnchecked_const_iterator  operator++(int) { DequeUnchecked_const_iterator _Tmp = *this; ++_Myoff; return _Tmp; }
	DequeUnchecked_const_iterator  operator--(int) { DequeUnchecked_const_iterator _Tmp = *this; --_Myoff; return _Tmp; }
	DequeUnchecked_const_iterator& operator+=(const difference_type _Off) { _Myoff += _Off; return *this; }
	DequeUnchecked_const_iterator& operator-=(const difference_type _Off) { _Myoff -= _Off; return *this; }

	[[nodiscard]] DequeUnchecked_const_iterator operator+(const difference_type _Off) const { DequeUnchecked_const_iterator _Tmp = *this; return _Tmp += _Off; }
	[[nodiscard]] DequeUnchecked_const_iterator operator-(const difference_type _Off) const { DequeUnchecked_const_iterator _Tmp = *this; return _Tmp -= _Off; }
	[[nodiscard]] difference_type operator-(const DequeUnchecked_const_iterator& _Right) const { return static_cast<difference_type>(_Myoff - _Right._Myoff); }
	[[nodiscard]] reference operator[](const difference_type _Off) const { return *(*this + _Off); }
	[[nodiscard]] bool operator==(const DequeUnchecked_const_iterator& _Right) const { return _Myoff == _Right._Myoff; }
	[[nodiscard]] bool operator!=(const DequeUnchecked_const_iterator& _Right) const { return !(*this == _Right); }
	[[nodiscard]] bool operator< (const DequeUnchecked_const_iterator& _Right) const { return _Myoff < _Right._Myoff; }
	[[nodiscard]] bool operator> (const DequeUnchecked_const_iterator& _Right) const { return _Right < *this; }
	[[nodiscard]] bool operator<=(const DequeUnchecked_const_iterator& _Right) const { return !(_Right < *this); }
	[[nodiscard]] bool operator>=(const DequeUnchecked_const_iterator& _Right) const { return !(*this < _Right); }

	const Container_base12* _Getcont() const {  return _Mycont; } // get container pointer

	const _Mydeque * _Mycont;
	_Size_type _Myoff; // offset of element in deque
};

template <class _Mydeque>
[[nodiscard]] DequeUnchecked_const_iterator<_Mydeque> operator+(
	typename DequeUnchecked_const_iterator<_Mydeque>::difference_type _Off,
	DequeUnchecked_const_iterator<_Mydeque> _Next) {
	return _Next += _Off;
}

// CLASS TEMPLATE DequeUnchecked_iterator
template <class _Mydeque>
class DequeUnchecked_iterator : public DequeUnchecked_const_iterator<_Mydeque> {
private:
	using _Size_type = typename _Mydeque::size_type;
	using _Mybase    = DequeUnchecked_const_iterator<_Mydeque>;

public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::pointer;
	using reference       = value_type&;

	using _Mybase::_Mybase;

	[[nodiscard]] reference operator*() const { return const_cast<reference>(_Mybase::operator*()); }
	[[nodiscard]] pointer operator->() const { return std::pointer_traits<pointer>::pointer_to(**this); }

	DequeUnchecked_iterator& operator++() { _Mybase::operator++(); return *this; }
	DequeUnchecked_iterator& operator--() { _Mybase::operator--(); return *this; }
	DequeUnchecked_iterator  operator++(int) { DequeUnchecked_iterator _Tmp = *this; _Mybase::operator++(); return _Tmp; }
	DequeUnchecked_iterator  operator--(int) { DequeUnchecked_iterator _Tmp = *this; _Mybase::operator--(); return _Tmp; }

	DequeUnchecked_iterator& operator+=(const difference_type _Off) { _Mybase::operator+=(_Off); return *this; }
	DequeUnchecked_iterator& operator-=(const difference_type _Off) { _Mybase::operator-=(_Off); return *this; }

	[[nodiscard]] DequeUnchecked_iterator operator+(const difference_type _Off) const { DequeUnchecked_iterator _Tmp = *this; return _Tmp += _Off; }
	[[nodiscard]] DequeUnchecked_iterator operator-(const difference_type _Off) const { DequeUnchecked_iterator _Tmp = *this; return _Tmp -= _Off; }

	[[nodiscard]] difference_type operator-(const _Mybase& _Right) const { return _Mybase::operator-(_Right); }

	[[nodiscard]] reference operator[](const difference_type _Off) const { return const_cast<reference>(_Mybase::operator[](_Off)); }
};

template <class _Mydeque>
[[nodiscard]] DequeUnchecked_iterator<_Mydeque> operator+(typename DequeUnchecked_iterator<_Mydeque>::difference_type _Off, DequeUnchecked_iterator<_Mydeque> _Next)
{
	return _Next += _Off;
}

// CLASS TEMPLATE Deque_const_iterator
template <class _Mydeque>
class Deque_const_iterator : public Iterator_base12 
{
private:
	using _Size_type = typename _Mydeque::size_type;

public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::const_pointer;
	using reference       = const value_type&;

	using _Mydeque_t = _Mydeque; // helper for expression evaluator
	enum { _EEN_DS = HTD_DEQUESIZE }; // helper for expression evaluator
	Deque_const_iterator() noexcept : _Myoff(0) {
		_Setcont(nullptr);
	}

	Deque_const_iterator(_Size_type _Off, const Container_base12* _Pdeque) noexcept : _Myoff(_Off) 
	{
		_Setcont(static_cast<const _Mydeque*>(_Pdeque));
	}

	[[nodiscard]] reference operator*() const
	{
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
		#if HTD_ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(_Mycont, "cannot dereference value-initialized deque iterator");
		_STL_VERIFY(_Mycont->_Myoff <= this->_Myoff && this->_Myoff < _Mycont->_Myoff + _Mycont->_Mysize,
					"cannot deference out of range deque iterator");
		#endif // HTD_ITERATOR_DEBUG_LEVEL != 0

		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % HTD_DEQUESIZE;
		return _Mycont->_Map[_Block][_Off];
	}

	[[nodiscard]] pointer operator->() const { return std::pointer_traits<pointer>::pointer_to(**this); }

	Deque_const_iterator& operator++() 
	{
		#if HTD_ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
		_STL_VERIFY(_Mycont, "cannot increment value-initialized deque iterator");
		_STL_VERIFY(this->_Myoff < _Mycont->_Myoff + _Mycont->_Mysize, "cannot increment deque iterator past end");
		#endif // HTD_ITERATOR_DEBUG_LEVEL != 0

		++_Myoff;
		return *this;
	}

	Deque_const_iterator operator++(int)
	{
		Deque_const_iterator _Tmp = *this;
		++*this;
		return _Tmp;
	}

	Deque_const_iterator& operator--() 
	{
		#if HTD_ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
		_STL_VERIFY(_Mycont, "cannot decrement value-initialized deque iterator");
		_STL_VERIFY(_Mycont->_Myoff < this->_Myoff, "cannot decrement deque iterator before begin");
		#endif // HTD_ITERATOR_DEBUG_LEVEL != 0

		--_Myoff;
		return *this;
	}

	Deque_const_iterator operator--(int)
	{
		Deque_const_iterator _Tmp = *this;
		--*this;
		return _Tmp;
	}

	Deque_const_iterator& operator+=(const difference_type _Off) 
	{
		#if HTD_ITERATOR_DEBUG_LEVEL != 0
		if (_Off != 0) {
			const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
			_STL_VERIFY(_Mycont, "cannot seek value-initialized deque iterator");
			_STL_VERIFY(
				_Mycont->_Myoff <= this->_Myoff + _Off && this->_Myoff + _Off <= _Mycont->_Myoff + _Mycont->_Mysize,
				"cannot seek deque iterator out of range");
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL != 0

		_Myoff += _Off;
		return *this;
	}

	void _Compat(const Deque_const_iterator& _Right) const 
	{ // test for compatible iterator pair
		#if HTD_ITERATOR_DEBUG_LEVEL == 0
		(void) _Right;
		#else // HTD_ITERATOR_DEBUG_LEVEL == 0
		_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "deque iterators incompatible");
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 0
	}

	[[nodiscard]] Deque_const_iterator operator+(const difference_type _Off) const { Deque_const_iterator _Tmp = *this; return _Tmp += _Off; }

	Deque_const_iterator& operator-=(const difference_type _Off) { return *this += -_Off; }

	[[nodiscard]] Deque_const_iterator operator-(const difference_type _Off) const { Deque_const_iterator _Tmp = *this; return _Tmp -= _Off; }

	[[nodiscard]] difference_type operator-(const Deque_const_iterator& _Right) const { _Compat(_Right); return static_cast<difference_type>(this->_Myoff - _Right._Myoff); }

	[[nodiscard]] reference operator[](const difference_type _Off) const { return *(*this + _Off); }

	[[nodiscard]] bool operator==(const Deque_const_iterator& _Right) const { _Compat(_Right); return this->_Myoff == _Right._Myoff; }
	[[nodiscard]] bool operator!=(const Deque_const_iterator& _Right) const { return !(*this == _Right); }
	[[nodiscard]] bool operator< (const Deque_const_iterator& _Right) const { _Compat(_Right); return this->_Myoff < _Right._Myoff; }
	[[nodiscard]] bool operator> (const Deque_const_iterator& _Right) const { return _Right < *this; }
	[[nodiscard]] bool operator<=(const Deque_const_iterator& _Right) const { return !(_Right < *this); }
	[[nodiscard]] bool operator>=(const Deque_const_iterator& _Right) const { return !(*this < _Right); }

	// set container pointer
	void _Setcont(const _Mydeque* _Pdeque) { this->_Adopt(_Pdeque); }

	using _Prevent_inheriting_unwrap = Deque_const_iterator;

	[[nodiscard]] DequeUnchecked_const_iterator<_Mydeque> _Unwrapped() const { return {this->_Myoff, this->_Getcont()}; }

	void _Verify_offset(const difference_type _Off) const noexcept {
		#if HTD_ITERATOR_DEBUG_LEVEL == 0
		(void) _Off;
		#else // ^^^ HTD_ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv HTD_ITERATOR_DEBUG_LEVEL != 0 vvv
		if (_Off != 0) {
			const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
			_STL_VERIFY(_Mycont, "cannot use value-initialized deque iterator");
			_STL_VERIFY(
				_Mycont->_Myoff <= this->_Myoff + _Off && this->_Myoff + _Off <= _Mycont->_Myoff + _Mycont->_Mysize,
				"cannot seek deque iterator out of range");
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 0
	}

	#if HTD_ITERATOR_DEBUG_LEVEL != 0
	friend void _Verify_range(const Deque_const_iterator& _First, const Deque_const_iterator& _Last) {
		// note _Compat check inside operator<=
		_STL_VERIFY(_First <= _Last, "deque iterators transposed");
	}
	#endif // HTD_ITERATOR_DEBUG_LEVEL != 0

	void _Seek_to(const DequeUnchecked_const_iterator<_Mydeque>& _UIt) { _Myoff = _UIt._Myoff; }

	_Size_type _Myoff; // offset of element in deque
};

template <class _Mydeque>
[[nodiscard]] Deque_const_iterator<_Mydeque> operator+(
	typename Deque_const_iterator<_Mydeque>::difference_type _Off, Deque_const_iterator<_Mydeque> _Next) {
	return _Next += _Off;
}

// CLASS TEMPLATE Deque_iterator
template <class _Mydeque>
class Deque_iterator : public Deque_const_iterator<_Mydeque> {
private:
	using _Size_type = typename _Mydeque::size_type;
	using _Mybase    = Deque_const_iterator<_Mydeque>;

public:
	using _Deque_unchecked_type = DequeUnchecked_iterator<_Mydeque>;
	using iterator_category     = std::random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::pointer;
	using reference       = value_type&;

	using _Mybase::_Mybase;

	[[nodiscard]] reference operator*()  const { return const_cast<reference>(_Mybase::operator*()); }
	[[nodiscard]] pointer   operator->() const { return std::pointer_traits<pointer>::pointer_to(**this); }

	Deque_iterator& operator++()    { _Mybase::operator++(); return *this; }
	Deque_iterator& operator--()    { _Mybase::operator--(); return *this; }
	Deque_iterator  operator++(int) { Deque_iterator _Tmp = *this; _Mybase::operator++(); return _Tmp; }
	Deque_iterator  operator--(int) { Deque_iterator _Tmp = *this; _Mybase::operator--(); return _Tmp; }

	Deque_iterator& operator+=(const difference_type _Off) { _Mybase::operator+=(_Off); return *this; }
	Deque_iterator& operator-=(const difference_type _Off) { _Mybase::operator-=(_Off); return *this; }
	
	using _Mybase::operator-;

	[[nodiscard]] Deque_iterator operator+(const difference_type _Off) const { Deque_iterator _Tmp = *this; return _Tmp += _Off; }
	[[nodiscard]] Deque_iterator operator-(const difference_type _Off) const { Deque_iterator _Tmp = *this; return _Tmp -= _Off; }

	[[nodiscard]] reference operator[](const difference_type _Off) const { return const_cast<reference>(_Mybase::operator[](_Off)); }

	using _Prevent_inheriting_unwrap = Deque_iterator;

	[[nodiscard]] DequeUnchecked_iterator<_Mydeque> _Unwrapped() const {
		return {this->_Myoff, this->_Getcont()};
	}
};

template <class _Mydeque>
[[nodiscard]] Deque_iterator<_Mydeque> operator+(typename Deque_iterator<_Mydeque>::difference_type _Off, Deque_iterator<_Mydeque> _Next)
{
	return _Next += _Off;
}

// deque TYPE WRAPPERS
template <class _Value_type, class _Size_type, class _Difference_type, class _Pointer, class _Const_pointer,
	class _Reference, class _Const_reference, class _Mapptr_type>
struct Deque_iter_types
{
	using value_type      = _Value_type;
	using size_type       = _Size_type;
	using difference_type = _Difference_type;
	using pointer         = _Pointer;
	using const_pointer   = _Const_pointer;
	using _Mapptr         = _Mapptr_type;
};

template <class _Ty>
struct Deque_simple_types
{
	using value_type      = _Ty;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using pointer         = value_type*;
	using const_pointer   = const value_type*;

	using _Mapptr = _Ty**;
};

// CLASS TEMPLATE Deque_val
template <class _Val_types>
class Deque_val : public Container_base12
{
public:
	using value_type      = typename _Val_types::value_type;
	using size_type       = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer         = typename _Val_types::pointer;
	using const_pointer   = typename _Val_types::const_pointer;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using _Mapptr         = typename _Val_types::_Mapptr;

	Deque_val() noexcept : _Map(), _Mapsize(0), _Myoff(0), _Mysize(0) {}

	// NB: _Mapsize and HTD_DEQUESIZE are guaranteed to be powers of 2
	size_type _Getblock(size_type _Off) const noexcept { return (_Off / HTD_DEQUESIZE) & (_Mapsize - 1); }

	_Mapptr _Map; // pointer to array of pointers to blocks
	size_type _Mapsize; // size of map array, zero or 2^N
	size_type _Myoff; // offset of initial element
	size_type _Mysize; // current length of sequence
};

// CLASS TEMPLATE deque
template <class _Ty, class _Alloc = std::allocator<_Ty>>
class deque {
private:
	friend Tidy_guard<deque>;
	static_assert(!HTD_ENFORCE_MATCHING_ALLOCATORS || std::is_same_v<_Ty, typename _Alloc::value_type>,
				  HTD_MISMATCHED_ALLOCATOR_MESSAGE("deque<T, Allocator>", "T"));

	using _Alty           = Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits    = std::allocator_traits<_Alty>;
	using _Alpty          = Rebind_alloc_t<_Alloc, typename _Alty_traits::pointer>;
	using _Alpty_traits   = std::allocator_traits<_Alpty>;
	using _Mapptr         = typename _Alpty_traits::pointer;
	using _Alproxy_ty     = Rebind_alloc_t<_Alty, Container_proxy>;
	using _Alproxy_traits = std::allocator_traits<_Alproxy_ty>;

	using _Scary_val = Deque_val<std::conditional_t<Is_simple_alloc_v<_Alty>, Deque_simple_types<_Ty>,
		Deque_iter_types<_Ty, typename _Alty_traits::size_type, typename _Alty_traits::difference_type,
		typename _Alty_traits::pointer, typename _Alty_traits::const_pointer, _Ty&, const _Ty&, _Mapptr>>>;

public:
	using allocator_type  = _Alloc;
	using value_type      = _Ty;
	using size_type       = typename _Alty_traits::size_type;
	using difference_type = typename _Alty_traits::difference_type;
	using pointer         = typename _Alty_traits::pointer;
	using const_pointer   = typename _Alty_traits::const_pointer;
	using reference       = _Ty&;
	using const_reference = const _Ty&;

	using iterator                  = Deque_iterator<_Scary_val>;
	using const_iterator            = Deque_const_iterator<_Scary_val>;
	using Unchecked_iterator       = DequeUnchecked_iterator<_Scary_val>;
	using Unchecked_const_iterator = DequeUnchecked_const_iterator<_Scary_val>;

	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	enum { _EEN_DS = HTD_DEQUESIZE }; // helper for expression evaluator

	deque() : _Mypair(Zero_then_variadic_args_t()) { _Get_data()._Alloc_proxy(static_cast<_Alproxy_ty>(_Getal())); }

	explicit deque(const _Alloc& _Al) : _Mypair(One_then_variadic_args_t(), _Al) { _Get_data()._Alloc_proxy(static_cast<_Alproxy_ty>(_Getal())); }

	explicit deque(size_type _Count, const _Alloc& _Al = _Alloc())
		: _Mypair(One_then_variadic_args_t(), _Al) 
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		resize(_Count);
		_Proxy._Release();
	}

	deque(size_type _Count, const _Ty& _Val) : _Mypair(Zero_then_variadic_args_t()) 
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct_n(_Count, _Val);
		_Proxy._Release();
	}

	deque(size_type _Count, const _Ty& _Val, const _Alloc& _Al)
		: _Mypair(One_then_variadic_args_t(), _Al) {
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct_n(_Count, _Val);
		_Proxy._Release();
	}

	deque(const deque& _Right)
		: _Mypair(One_then_variadic_args_t(), _Alty_traits::select_on_container_copy_construction(_Right._Getal())) 
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_Right._Unchecked_begin(), _Right._Unchecked_end());
		_Proxy._Release();
	}

	deque(const deque& _Right, const _Alloc& _Al) : _Mypair(One_then_variadic_args_t(), _Al)
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_Right._Unchecked_begin(), _Right._Unchecked_end());
		_Proxy._Release();
	}

	template <class _Iter, class = std::enable_if_t<Is_iterator_v<_Iter>>>
	deque(_Iter _First, _Iter _Last) : _Mypair(Zero_then_variadic_args_t())
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_First, _Last);
		_Proxy._Release();
	}

	template <class _Iter, class = std::enable_if_t<Is_iterator_v<_Iter>>>
	deque(_Iter _First, _Iter _Last, const _Alloc& _Al) : _Mypair(One_then_variadic_args_t(), _Al)
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_First, _Last);
		_Proxy._Release();
	}

private:
	template <class _Iter>
	void _Construct(_Iter _First, _Iter _Last)
	{ // initialize from [_First, _Last), input iterators
		Tidy_guard<deque> _Guard{this};
		for (; _First != _Last; ++_First) {
			emplace_back(*_First);
		}

		_Guard._Target = nullptr;
	}

	void _Construct_n(size_type _Count, const _Ty& _Val) 
	{ // construct from _Count * _Val
		Tidy_guard<deque> _Guard{this};
		for (; 0 < _Count; --_Count)
			_Emplace_back_internal(_Val);

		_Guard._Target = nullptr;
	}

public:
	deque(deque&& _Right) : _Mypair(One_then_variadic_args_t(), std::move(_Right._Getal()))
	{
		_Get_data()._Alloc_proxy(static_cast<_Alproxy_ty>(_Getal()));
		_Take_contents(_Right);
	}

	deque(deque&& _Right, const _Alloc& _Al) : _Mypair(One_then_variadic_args_t(), _Al)
	{
		_Alproxy_ty _Alproxy(_Getal());
		if
			constexpr(!_Alty_traits::is_always_equal::value) {
			if (_Getal() != _Right._Getal()) {
				Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
				_Construct(std::make_move_iterator(_Right._Unchecked_begin()),
						   std::make_move_iterator(_Right._Unchecked_end()));
				_Proxy._Release();
				return;
			}
		}

		_Get_data()._Alloc_proxy(_Alproxy);
		_Take_contents(_Right);
	}

private:
	void _Move_assign(deque& _Right, Equal_allocators) noexcept
	{
		_Tidy();
		_Pocma(_Getal(), _Right._Getal());
		_Take_contents(_Right);
	}

	void _Move_assign(deque& _Right, Propagate_allocators) {
		auto& _Al       = _Getal();
		auto& _Right_al = _Right._Getal();
		if (_Al == _Right_al) {
			_Move_assign(_Right, Equal_allocators{});
		} else {
			_Alproxy_ty _Alproxy(_Al);
			_Alproxy_ty _Right_alproxy(_Right_al);
			Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Right_alproxy, Leave_proxy_unbound{});
			_Tidy();
			_Pocma(_Al, _Right_al);
			_Proxy._Bind(_Alproxy, std::addressof(_Get_data()));
			_Take_contents(_Right);
		}
	}

	void _Move_assign(deque& _Right, No_propagate_allocators)
	{
		if (_Getal() == _Right._Getal()) {
			_Move_assign(_Right, Equal_allocators{});
		} else {
			assign(
				std::make_move_iterator(_Right._Unchecked_begin()), std::make_move_iterator(_Right._Unchecked_end()));
		}
	}

public:
	deque& operator=(deque&& _Right) noexcept(_Alty_traits::is_always_equal::value)
	{
		if (this != std::addressof(_Right))
			_Move_assign(_Right, Choose_pocma<_Alty>{});

		return *this;
	}

private:
	void _Take_contents(deque& _Right) noexcept
	{
		// move from _Right, stealing its contents
		// pre: _Getal() == _Right._Getal()
		auto& _My_data    = _Get_data();
		auto& _Right_data = _Right._Get_data();
		_My_data._Swap_proxy_and_iterators(_Right_data);
		_My_data._Map     = _Right_data._Map;
		_My_data._Mapsize = _Right_data._Mapsize;
		_My_data._Myoff   = _Right_data._Myoff;
		_My_data._Mysize  = _Right_data._Mysize;

		_Right_data._Map     = nullptr;
		_Right_data._Mapsize = 0;
		_Right_data._Myoff   = 0;
		_Right_data._Mysize  = 0;
	}

public:
	void push_front(_Ty&& _Val) 
	{
		_Orphan_all();
		
		//PUSH_FRONT_BEGIN:
			if (_Myoff() % HTD_DEQUESIZE == 0 && _Mapsize() <= (_Mysize() + HTD_DEQUESIZE) / HTD_DEQUESIZE)
				_Growmap(1);
			_Myoff() &= _Mapsize() * HTD_DEQUESIZE - 1;
			size_type _Newoff = _Myoff() != 0 ? _Myoff() : _Mapsize() * HTD_DEQUESIZE;
			size_type _Block = _Getblock(--_Newoff);
			if (_Map()[_Block] == nullptr) 
				_Map()[_Block] = _Getal().allocate(HTD_DEQUESIZE);

		_Alty_traits::construct(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE), std::move(_Val));
		
		//PUSH_FRONT_END:
			_Myoff() = _Newoff;
			++_Mysize();
	}

	void push_back(_Ty&& _Val)
	{
		_Orphan_all();
		_Emplace_back_internal(std::move(_Val));
	}

	iterator insert(const_iterator _Where, _Ty&& _Val)
	{
		return emplace(_Where, std::move(_Val));
	}

	template <class... _Valty>
	decltype(auto) emplace_front(_Valty&&... _Val)
	{
		_Orphan_all();

		//PUSH_FRONT_BEGIN:
			if (_Myoff() % HTD_DEQUESIZE == 0 && _Mapsize() <= (_Mysize() + HTD_DEQUESIZE) / HTD_DEQUESIZE)
				_Growmap(1);
			_Myoff() &= _Mapsize() * HTD_DEQUESIZE - 1;
			size_type _Newoff = _Myoff() != 0 ? _Myoff() : _Mapsize() * HTD_DEQUESIZE;
			size_type _Block = _Getblock(--_Newoff);
			if (_Map()[_Block] == nullptr) 
				_Map()[_Block] = _Getal().allocate(HTD_DEQUESIZE);

		_Alty_traits::construct(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE), std::forward<_Valty>(_Val)...);
		
		//PUSH_FRONT_END:
			_Myoff() = _Newoff;
			++_Mysize();

		#if HTD_HAS_CXX17
		return front();
		#endif // HTD_HAS_CXX17
	}

	template <class... _Valty>
	decltype(auto) emplace_back(_Valty&&... _Val)
	{
		_Orphan_all();

		//PUSH_BACK_BEGIN:
			if ((_Myoff() + _Mysize()) % HTD_DEQUESIZE == 0 && _Mapsize() <= (_Mysize() + HTD_DEQUESIZE) / HTD_DEQUESIZE)
				_Growmap(1);
			_Myoff() &= _Mapsize() * HTD_DEQUESIZE - 1;
			size_type _Newoff = _Myoff() + _Mysize();
			size_type _Block = _Getblock(_Newoff);
			if (_Map()[_Block] == nullptr)
				_Map()[_Block] = _Getal().allocate(HTD_DEQUESIZE);


		_Alty_traits::construct(
			_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE), std::forward<_Valty>(_Val)...);

		//PUSH_BACK_END:
			++_Mysize();

		#if HTD_HAS_CXX17
		return back();
		#endif // HTD_HAS_CXX17
	}

	template <class... _Valty>
	iterator emplace(const_iterator _Where, _Valty&&... _Val) 
	{
		const auto _Off = static_cast<size_type>(_Where - begin());

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Off <= _Mysize(), "deque emplace iterator outside range");
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		if (_Off <= _Mysize() / 2)
		{ // closer to front, push to front then rotate
			emplace_front(std::forward<_Valty>(_Val)...);
			std::rotate(begin(), begin() + 1, begin() + static_cast<difference_type>(1 + _Off));
		} 
		else 
		{ // closer to back, push to back then rotate
			emplace_back(std::forward<_Valty>(_Val)...);
			std::rotate(begin() + static_cast<difference_type>(_Off), end() - 1, end());
		}
		return begin() + static_cast<difference_type>(_Off);
	}

	deque(std::initializer_list<_Ty> _Ilist, const _Alloc& _Al = allocator_type())
		: _Mypair(One_then_variadic_args_t(), _Al) 
	{
		_Alproxy_ty _Alproxy(_Getal());
		Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_Ilist.begin(), _Ilist.end());
		_Proxy._Release();
	}

	deque& operator=(std::initializer_list<_Ty> _Ilist)
	{
		assign(_Ilist.begin(), _Ilist.end());
		return *this;
	}

	void assign(std::initializer_list<_Ty> _Ilist) 
	{
		assign(_Ilist.begin(), _Ilist.end());
	}

	iterator insert(const_iterator _Where, std::initializer_list<_Ty> _Ilist) 
	{
		return insert(_Where, _Ilist.begin(), _Ilist.end());
	}

	~deque() noexcept
	{
		_Tidy();
		_Alproxy_ty _Proxy_allocator(_Getal());
		Delete_plain_internal(_Proxy_allocator, std::exchange(_Get_data()._Myproxy, nullptr));
	}

	void _Copy_assign(const deque& _Right, std::false_type)
	{
		_Pocca(_Getal(), _Right._Getal());
		assign(_Right._Unchecked_begin(), _Right._Unchecked_end());
	}

	void _Copy_assign(const deque& _Right, std::true_type)
	{
		if (_Getal() != _Right._Getal()) {
			_Tidy();
			_Get_data()._Reload_proxy(static_cast<_Alproxy_ty>(_Getal()), static_cast<_Alproxy_ty>(_Right._Getal()));
		}

		_Copy_assign(_Right, std::false_type{});
	}

	deque& operator=(const deque& _Right)
	{
		if (this != std::addressof(_Right)) {
			_Copy_assign(_Right, Choose_pocca<_Alty>{});
		}

		return *this;
	}

	[[nodiscard]]       iterator begin()       noexcept { return       iterator(_Myoff(),             std::addressof(_Get_data())); }
	[[nodiscard]]       iterator end()         noexcept { return       iterator(_Myoff() + _Mysize(), std::addressof(_Get_data())); }
	[[nodiscard]] const_iterator begin() const noexcept { return const_iterator(_Myoff(),             std::addressof(_Get_data())); }
	[[nodiscard]] const_iterator end()   const noexcept { return const_iterator(_Myoff() + _Mysize(), std::addressof(_Get_data())); }

	Unchecked_iterator       _Unchecked_begin()       noexcept { return Unchecked_iterator      (_Myoff(),             std::addressof(_Get_data())); }
	Unchecked_iterator       _Unchecked_end()         noexcept { return Unchecked_iterator      (_Myoff() + _Mysize(), std::addressof(_Get_data())); }
	Unchecked_const_iterator _Unchecked_begin() const noexcept { return Unchecked_const_iterator(_Myoff(),             std::addressof(_Get_data())); }
	Unchecked_const_iterator _Unchecked_end()   const noexcept { return Unchecked_const_iterator(_Myoff() + _Mysize(), std::addressof(_Get_data())); }

	iterator _Make_iter(const_iterator _Where) const noexcept { return                  iterator(_Where._Myoff,               std::addressof(_Get_data())); }
	
	[[nodiscard]]       reverse_iterator rbegin()        noexcept { return       reverse_iterator(end()); }
	[[nodiscard]] const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(end()); }
	[[nodiscard]]       reverse_iterator rend()          noexcept { return       reverse_iterator(begin()); }
	[[nodiscard]] const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(begin()); }
	[[nodiscard]]         const_iterator cbegin()  const noexcept { return begin(); }
	[[nodiscard]]         const_iterator cend()    const noexcept { return end(); }
	[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return rbegin(); }
	[[nodiscard]] const_reverse_iterator crend()   const noexcept { return rend(); }

	void shrink_to_fit() 
	{
		size_type _Oldcapacity = HTD_DEQUESIZE * _Mapsize();
		size_type _Newcapacity = _Oldcapacity / 2;

		if (_Newcapacity < HTD_DEQUESIZE * HTD_DEQUEMAPSIZE)
			_Newcapacity = HTD_DEQUESIZE * HTD_DEQUEMAPSIZE;

		if ((empty() && 0 < _Mapsize())
			|| (!empty() && size() <= _Newcapacity && _Newcapacity < _Oldcapacity)) { // worth shrinking, do it
			deque _Tmp(std::make_move_iterator(begin()), std::make_move_iterator(end()));
			swap(_Tmp);
		}
	}

	void resize(size_type _Newsize)
	{
		while (_Mysize() < _Newsize)
			emplace_back();

		while (_Newsize < _Mysize())
			pop_back();
	}

	void resize(size_type _Newsize, const _Ty& _Val)
	{
		_Orphan_all();
		while (_Mysize() < _Newsize)
			_Emplace_back_internal(_Val);

		while (_Newsize < _Mysize())
			pop_back();
	}

	[[nodiscard]] size_type size() const noexcept { return _Mysize(); }

	[[nodiscard]] size_type max_size() const noexcept
	{
		return Min_value(static_cast<size_type>((std::numeric_limits<difference_type>::max)()), _Alty_traits::max_size(_Getal()));
	}

	[[nodiscard]] bool empty() const noexcept { return _Mysize() == 0; }

	[[nodiscard]] allocator_type get_allocator() const noexcept { return static_cast<allocator_type>(_Getal()); }

	[[nodiscard]] const_reference at(size_type _Pos) const 
	{
		if (_Mysize() <= _Pos)
			throw std::out_of_range("invalid deque<T> subscript");
		return *(begin() + static_cast<difference_type>(_Pos));
	}

	[[nodiscard]] reference at(size_type _Pos) 
	{
		if (_Mysize() <= _Pos)
			throw std::out_of_range("invalid deque<T> subscript");
		return *(begin() + static_cast<difference_type>(_Pos));
	}

	[[nodiscard]] const_reference operator[](size_type _Pos) const noexcept /* strengthened */
	{
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(_Pos < _Mysize(), "deque subscript out of range");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *(_Unchecked_begin() + static_cast<difference_type>(_Pos));
	}

	[[nodiscard]] reference operator[](size_type _Pos) noexcept /* strengthened */ 
	{
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(_Pos < _Mysize(), "deque subscript out of range");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *(_Unchecked_begin() + static_cast<difference_type>(_Pos));
	}

	[[nodiscard]] reference front() noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(!empty(), "front() called on empty deque");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *_Unchecked_begin();
	}

	[[nodiscard]] const_reference front() const noexcept /* strengthened */ 
	{
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(!empty(), "front() called on empty deque");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *_Unchecked_begin();
	}

	[[nodiscard]] reference back() noexcept /* strengthened */
	{
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(!empty(), "back() called on empty deque");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *(_Unchecked_end() - 1);
	}

	[[nodiscard]] const_reference back() const noexcept /* strengthened */
	{
		#if _CONTAINER_DEBUG_LEVEL > 0
		_STL_VERIFY(!empty(), "back() called on empty deque");
		#endif // _CONTAINER_DEBUG_LEVEL > 0

		return *(_Unchecked_end() - 1);
	}

	void push_front(const _Ty& _Val)
	{
		_Orphan_all();

		//PUSH_FRONT_BEGIN:
			if (_Myoff() % HTD_DEQUESIZE == 0 && _Mapsize() <= (_Mysize() + HTD_DEQUESIZE) / HTD_DEQUESIZE)
				_Growmap(1);
			_Myoff() &= _Mapsize() * HTD_DEQUESIZE - 1;
			size_type _Newoff = _Myoff() != 0 ? _Myoff() : _Mapsize() * HTD_DEQUESIZE;
			size_type _Block = _Getblock(--_Newoff);
			if (_Map()[_Block] == nullptr) 
				_Map()[_Block] = _Getal().allocate(HTD_DEQUESIZE);

		_Alty_traits::construct(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE), _Val);
		
		//PUSH_FRONT_END:
			_Myoff() = _Newoff;
			++_Mysize();
	}

	void pop_front() noexcept /* strengthened */
	{
		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		if (empty()) {
			_STL_REPORT_ERROR("deque empty before pop");
		} else { // something to erase, do it
			_Orphan_off(_Myoff());
			size_type _Block = _Getblock(_Myoff());
			_Alty_traits::destroy(_Getal(), Unfancy(_Map()[_Block] + _Myoff() % HTD_DEQUESIZE));
			if (--_Mysize() == 0) {
				_Myoff() = 0;
			} else {
				++_Myoff();
			}
		}

		#else // HTD_ITERATOR_DEBUG_LEVEL == 2
		size_type _Block = _Getblock(_Myoff());
		_Alty_traits::destroy(_Getal(), Unfancy(_Map()[_Block] + _Myoff() % HTD_DEQUESIZE));
		if (--_Mysize() == 0) {
			_Myoff() = 0;
		} else {
			++_Myoff();
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
	}

private:
	template <class... _Tys>
	void _Emplace_back_internal(_Tys&&... _Vals)
	{
		//PUSH_BACK_BEGIN:
			if ((_Myoff() + _Mysize()) % HTD_DEQUESIZE == 0 && _Mapsize() <= (_Mysize() + HTD_DEQUESIZE) / HTD_DEQUESIZE)
				_Growmap(1);
			_Myoff() &= _Mapsize() * HTD_DEQUESIZE - 1;
			size_type _Newoff = _Myoff() + _Mysize();
			size_type _Block = _Getblock(_Newoff);
			if (_Map()[_Block] == nullptr)
				_Map()[_Block] = _Getal().allocate(HTD_DEQUESIZE);

		_Alty_traits::construct(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE), std::forward<_Tys>(_Vals)...);
		//PUSH_BACK_END:
			++_Mysize();
	}

public:
	void push_back(const _Ty& _Val)
	{
		_Orphan_all();
		_Emplace_back_internal(_Val);
	}

	void pop_back() noexcept /* strengthened */ 
	{
		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		if (empty()) {
			_STL_REPORT_ERROR("deque empty before pop");
		} else { // something to erase, do it
			size_type _Newoff = _Myoff() + _Mysize() - 1;
			_Orphan_off(_Newoff);
			size_type _Block = _Getblock(_Newoff);
			_Alty_traits::destroy(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE));
			if (--_Mysize() == 0) {
				_Myoff() = 0;
			}
		}

		#else // HTD_ITERATOR_DEBUG_LEVEL == 2
		size_type _Newoff = _Myoff() + _Mysize() - 1;
		size_type _Block  = _Getblock(_Newoff);
		_Alty_traits::destroy(_Getal(), Unfancy(_Map()[_Block] + _Newoff % HTD_DEQUESIZE));
		if (--_Mysize() == 0) {
			_Myoff() = 0;
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2
	}

	template <class _Iter, class = std::enable_if_t<Is_iterator_v<_Iter>>>
	void assign(_Iter _First, _Iter _Last)
	{
		_Orphan_all();
		Adl_verify_range(_First, _Last);
		auto _UFirst       = Get_unwrapped(_First);
		const auto _ULast  = Get_unwrapped(_Last);
		auto _Myfirst      = _Unchecked_begin();
		const auto _Mylast = _Unchecked_end();
		for (; _UFirst != _ULast; ++_UFirst) { // try to assign over an element in the container
			if (_Myfirst == _Mylast) { // container wasn't big enough, insert what's left at end
				do {
					emplace_back(*_UFirst);
					++_UFirst;
				} while (_UFirst != _ULast);
				return;
			}

			*_Myfirst = *_UFirst;
			++_Myfirst;
		}

		_Erase_last_n(static_cast<size_type>(_Mylast - _Myfirst));
	}

	void assign(size_type _Count, const _Ty& _Val) 
	{ // assign _Count * _Val
		_Orphan_all();
		auto _Myfirst       = _Unchecked_begin();
		const auto _Oldsize = _Mysize();
		auto _Assign_count  = Min_value(_Count, _Oldsize);
		for (; 0 < _Assign_count; --_Assign_count) {
			*_Myfirst = _Val;
			++_Myfirst;
		}

		const auto _Shrink_by = _Oldsize - _Assign_count;
		auto _Extend_by       = _Count - _Assign_count;
		_Erase_last_n(_Shrink_by);
		for (; 0 < _Extend_by; --_Extend_by) {
			_Emplace_back_internal(_Val);
		}
	}

	iterator insert(const_iterator _Where, const _Ty& _Val)
	{
		size_type _Off = static_cast<size_type>(_Where - begin());

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Off <= _Mysize(), "deque insert iterator outside range");
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		if (_Off <= _Mysize() / 2) { // closer to front, push to front then copy
			push_front(_Val);
			std::rotate(begin(), begin() + 1, begin() + static_cast<difference_type>(1 + _Off));
		} else { // closer to back, push to back then copy
			push_back(_Val);
			std::rotate(begin() + static_cast<difference_type>(_Off), end() - 1, end());
		}

		return begin() + static_cast<difference_type>(_Off);
	}

	iterator insert(const_iterator _Where, size_type _Count, const _Ty& _Val) 
	{
		// insert _Count * _Val at _Where
		size_type _Off = static_cast<size_type>(_Where - begin());
		_Insert_n(_Where, _Count, _Val);
		return begin() + static_cast<difference_type>(_Off);
	}

	template <class _Iter, class = std::enable_if_t<Is_iterator_v<_Iter>>>
	iterator insert(const_iterator _Where, _Iter _First, _Iter _Last)
	{
		// insert [_First, _Last) at _Where, input iterators
		size_type _Off = static_cast<size_type>(_Where - begin());

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Mysize() >= _Off, "deque insert iterator outside range");
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		Adl_verify_range(_First, _Last);
		auto _UFirst      = Get_unwrapped(_First);
		const auto _ULast = Get_unwrapped(_Last);

		size_type _Oldsize = _Mysize();

		if (_UFirst != _ULast) {
			if (_Off <= _Mysize() / 2) { // closer to front, push to front then rotate
				try {
					for (; _UFirst != _ULast; ++_UFirst) {
						emplace_front(*_UFirst); // prepend flipped
					}

				}
				catch (...) {
					while (_Oldsize < _Mysize()) {
						pop_front(); // restore old size, at least
					}

					throw;
				}

				size_type _Num = _Mysize() - _Oldsize;
				std::reverse(begin(), begin() + static_cast<difference_type>(_Num)); // flip new stuff in place
				std::rotate(begin(), begin() + static_cast<difference_type>(_Num),
							begin() + static_cast<difference_type>(_Num + _Off));
			}
			else { // closer to back
				try {
					_Orphan_all();
					for (; _UFirst != _ULast; ++_UFirst) {
						_Emplace_back_internal(*_UFirst);
					}

				}
				catch (...) {
					while (_Oldsize < _Mysize()) {
						pop_back(); // restore old size, at least
					}

					throw;
				}

				std::rotate(begin() + static_cast<difference_type>(_Off),
							begin() + static_cast<difference_type>(_Oldsize), end());
			}
		}

		return begin() + static_cast<difference_type>(_Off);
	}

	iterator erase(const_iterator _Where) noexcept(std::is_nothrow_move_assignable_v<value_type>) /* strengthened */ 
	{
		return erase(_Where, _Where + 1);
	}

	iterator erase(const_iterator _First_arg, const_iterator _Last_arg) noexcept(std::is_nothrow_move_assignable_v<value_type>) /* strengthened */ 
	{
		iterator _First = _Make_iter(_First_arg);
		iterator _Last  = _Make_iter(_Last_arg);

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_First <= _Last && begin() <= _First && _Last <= end(), "deque erase iterator outside range");
		Adl_verify_range(_First, _Last);

		auto _Off   = static_cast<size_type>(_First - begin());
		auto _Count = static_cast<size_type>(_Last - _First);
		bool _Moved = 0 < _Off && _Off + _Count < _Mysize();

		#else // HTD_ITERATOR_DEBUG_LEVEL == 2
		auto _Off   = static_cast<size_type>(_First - begin());
		auto _Count = static_cast<size_type>(_Last - _First);
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		if (_Off < static_cast<size_type>(end() - _Last)) { // closer to front
			std::move_backward(begin(), _First, _Last); // copy over hole
			for (; 0 < _Count; --_Count) {
				pop_front(); // pop copied elements
			}
		} else { // closer to back
			std::move(_Last, end(), _First); // copy over hole
			for (; 0 < _Count; --_Count) {
				pop_back(); // pop copied elements
			}
		}

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		if (_Moved) {
			_Orphan_all();
		}
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		return begin() + static_cast<difference_type>(_Off);
	}

private:
	void _Erase_last_n(size_type _Count) noexcept {
		for (; 0 < _Count; --_Count) {
			pop_back();
		}
	}

public:
	void clear() noexcept { _Tidy(); }// erase all

	void swap(deque& _Right) noexcept /* strengthened */ 
	{
		if (this != std::addressof(_Right)) 
		{
			_Pocs(_Getal(), _Right._Getal());
			auto& _My_data    = _Get_data();
			auto& _Right_data = _Right._Get_data();
			_My_data._Swap_proxy_and_iterators(_Right_data);
			Swap_adl(_My_data._Map, _Right_data._Map);
			std::swap(_My_data._Mapsize, _Right_data._Mapsize);
			std::swap(_My_data._Myoff, _Right_data._Myoff);
			std::swap(_My_data._Mysize, _Right_data._Mysize);
		}
	}

private:
	void _Insert_n(const_iterator _Where, size_type _Count, const _Ty& _Val) // insert _Count * _Val at _Where
	{ 
		iterator _Mid;
		size_type _Num;
		size_type _Off     = static_cast<size_type>(_Where - begin());
		size_type _Oldsize = _Mysize();
		size_type _Rem     = _Oldsize - _Off;

		#if HTD_ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Off <= _Oldsize, "deque insert iterator outside range");
		#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

		if (_Off < _Rem) { // closer to front
			try {
				if (_Off < _Count) { // insert longer than prefix
					for (_Num = _Count - _Off; 0 < _Num; --_Num) {
						push_front(_Val); // push excess values
					}
					for (_Num = _Off; 0 < _Num; --_Num) {
						push_front(begin()[static_cast<difference_type>(_Count - 1)]); // push prefix
					}

					_Mid = begin() + static_cast<difference_type>(_Count);
					std::fill(_Mid, _Mid + static_cast<difference_type>(_Off), _Val); // fill in rest of values
				}
				else { // insert not longer than prefix
					for (_Num = _Count; 0 < _Num; --_Num) {
						push_front(begin()[static_cast<difference_type>(_Count - 1)]); // push part of prefix
					}

					_Mid = begin() + static_cast<difference_type>(_Count);
					Alloc_temporary<_Alty> _Tmp(_Getal(), _Val); // in case _Val is in sequence
					std::move(_Mid + static_cast<difference_type>(_Count), _Mid + static_cast<difference_type>(_Off),
							  _Mid); // copy rest of prefix
					std::fill(begin() + static_cast<difference_type>(_Off), _Mid + static_cast<difference_type>(_Off),
							  _Tmp._Storage._Value); // fill in values
				}
			}
			catch (...) {
				while (_Oldsize < _Mysize()) {
					pop_front(); // restore old size, at least
				}

				throw;
			}
		}
		else { // closer to back
			try {
				if (_Rem < _Count) { // insert longer than suffix
					_Orphan_all();
					for (_Num = _Count - _Rem; 0 < _Num; --_Num) {
						_Emplace_back_internal(_Val); // push excess values
					}
					for (_Num = 0; _Num < _Rem; ++_Num) {
						_Emplace_back_internal(begin()[static_cast<difference_type>(_Off + _Num)]); // push suffix
					}

					_Mid = begin() + static_cast<difference_type>(_Off);
					std::fill(_Mid, _Mid + static_cast<difference_type>(_Rem), _Val); // fill in rest of values
				}
				else { // insert not longer than prefix
					for (_Num = 0; _Num < _Count; ++_Num) {
						_Emplace_back_internal(
							begin()[static_cast<difference_type>(_Off + _Rem - _Count + _Num)]); // push part of prefix
					}

					_Mid = begin() + static_cast<difference_type>(_Off);
					Alloc_temporary<_Alty> _Tmp(_Getal(), _Val); // in case _Val is in sequence
					std::move_backward(_Mid, _Mid + static_cast<difference_type>(_Rem - _Count),
									   _Mid + static_cast<difference_type>(_Rem)); // copy rest of prefix
					std::fill(_Mid, _Mid + static_cast<difference_type>(_Count),
							  _Tmp._Storage._Value); // fill in values
				}
			}
			catch (...) {
				_Erase_last_n(_Mysize() - _Oldsize);
				throw;
			}
		}
	}

	void _Growmap(size_type _Count) // grow map by at least _Count pointers, _Mapsize() a power of 2
	{
		static_assert(1 < HTD_DEQUEMAPSIZE, "The _Xlen() test should always be performed.");

		_Alpty _Almap(_Getal());
		size_type _Newsize = 0 < _Mapsize() ? _Mapsize() : 1;
		while (_Newsize - _Mapsize() < _Count || _Newsize < HTD_DEQUEMAPSIZE)
		{
			// scale _Newsize to 2^N >= _Mapsize() + _Count
			if (max_size() / HTD_DEQUESIZE - _Newsize < _Newsize)
			{
				throw std::length_error("deque<T> too long"); // result too long
			}

			_Newsize *= 2;
		}

		_Count = _Newsize - _Mapsize();

		size_type _Myboff = _Myoff() / HTD_DEQUESIZE;
		_Mapptr _Newmap   = _Almap.allocate(_Mapsize() + _Count);
		_Mapptr _Myptr    = _Newmap + _Myboff;

		_Myptr = std::uninitialized_copy(_Map() + _Myboff, _Map() + _Mapsize(), _Myptr); // copy initial to end
		if (_Myboff <= _Count)
		{ // increment greater than offset of initial block
			_Myptr = std::uninitialized_copy(_Map(), _Map() + _Myboff, _Myptr); // copy rest of old
			Uninitialized_value_construct_n_unchecked1(_Myptr, _Count - _Myboff); // clear suffix of new
			Uninitialized_value_construct_n_unchecked1(_Newmap, _Myboff); // clear prefix of new
		} 
		else
		{ // increment not greater than offset of initial block
			std::uninitialized_copy(_Map(), _Map() + _Count, _Myptr); // copy more old
			_Myptr = std::uninitialized_copy(_Map() + _Count, _Map() + _Myboff, _Newmap); // copy rest of old
			Uninitialized_value_construct_n_unchecked1(_Myptr, _Count); // clear rest to initial block
		}

		Destroy_range(_Map() + _Myboff, _Map() + _Mapsize());
		if (_Map() != _Mapptr()) {
			_Almap.deallocate(_Map(), _Mapsize()); // free storage for old
		}

		_Map() = _Newmap; // point at new
		_Mapsize() += _Count;
	}

	void _Tidy() noexcept // free all storage
	{
		_Orphan_all();

		_Alpty _Almap(_Getal());
		while (!empty()) {
			pop_back();
		}

		for (size_type _Block = _Mapsize(); 0 < _Block;) { // free storage for a block and destroy pointer
			if (_Map()[--_Block]) { // free block and destroy its pointer
				_Getal().deallocate(_Map()[_Block], HTD_DEQUESIZE);
				Destroy_in_place(_Map()[_Block]);
			}
		}

		if (_Map() != _Mapptr()) {
			_Almap.deallocate(_Map(), _Mapsize()); // free storage for map
		}

		_Mapsize() = 0;
		_Map()     = _Mapptr();
	}

	#if HTD_ITERATOR_DEBUG_LEVEL == 2
	void _Orphan_off(size_type _Offlo) const noexcept { // orphan iterators with specified offset(s)
		size_type _Offhigh = _Myoff() + _Mysize() <= _Offlo + 1 ? static_cast<size_type>(-1) : _Offlo;
		if (_Offlo == _Myoff()) {
			_Offlo = 0;
		}

		_Lockit _Lock(_LOCK_DEBUG);
		Iterator_base12** _Pnext = &_Get_data()._Myproxy->_Myfirstiter;
		while (*_Pnext) {
			const auto _Pnextoff = static_cast<const_iterator&>(**_Pnext)._Myoff;
			if (_Pnextoff < _Offlo || _Offhigh < _Pnextoff) {
				_Pnext = &(*_Pnext)->_Mynextiter;
			} else { // orphan the iterator
				(*_Pnext)->_Myproxy = nullptr;
				*_Pnext             = (*_Pnext)->_Mynextiter;
			}
		}
	}
	#endif // HTD_ITERATOR_DEBUG_LEVEL == 2

	size_type _Getblock(size_type _Off) const noexcept { return _Get_data()._Getblock(_Off); }

	void _Orphan_all() noexcept { _Get_data()._Orphan_all(); }

	      _Alty&      _Getal()          noexcept { return _Mypair._Get_first(); }
	const _Alty&      _Getal()    const noexcept { return _Mypair._Get_first(); }
	      _Scary_val& _Get_data()       noexcept { return _Mypair._Myval2; }
	const _Scary_val& _Get_data() const noexcept { return _Mypair._Myval2; }

	      _Mapptr&    _Map()            noexcept { return _Get_data()._Map; }
	const _Mapptr&    _Map()      const noexcept { return _Get_data()._Map; }
	      size_type&  _Mapsize()        noexcept { return _Get_data()._Mapsize; }
	const size_type&  _Mapsize()  const noexcept { return _Get_data()._Mapsize; }
	      size_type&  _Myoff()          noexcept { return _Get_data()._Myoff; }
	const size_type&  _Myoff()    const noexcept { return _Get_data()._Myoff; }
	      size_type&  _Mysize()         noexcept { return _Get_data()._Mysize; }
	const size_type&  _Mysize()   const noexcept { return _Get_data()._Mysize; }

	Compressed_pair<_Alty, _Scary_val> _Mypair;
};

#if HTD_HAS_CXX17
template <class _Iter, class _Alloc = std::allocator<Iter_value_t<_Iter>>,
	std::enable_if_t<std::conjunction_v<Is_iterator<_Iter>, Is_allocator<_Alloc>>, int> = 0>
	deque(_Iter, _Iter, _Alloc = _Alloc())->deque<Iter_value_t<_Iter>, _Alloc>;
#endif // HTD_HAS_CXX17

template <class _Ty, class _Alloc>
void swap(deque<_Ty, _Alloc>& _Left, deque<_Ty, _Alloc>& _Right) noexcept /* strengthened */ 
{
	_Left.swap(_Right); 
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator==(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right) 
{
	return _Left.size() == _Right.size() && std::equal(_Left._Unchecked_begin(), _Left._Unchecked_end(), _Right._Unchecked_begin());
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator!=(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right)
{
	return !(_Left == _Right);
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator<(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right) 
{
	return std::lexicographical_compare(_Left._Unchecked_begin(), _Left._Unchecked_end(), _Right._Unchecked_begin(), _Right._Unchecked_end());
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator<=(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right)
{
	return !(_Right < _Left);
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator>(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right) 
{
	return _Right < _Left;
}

template <class _Ty, class _Alloc>
[[nodiscard]] bool operator>=(const deque<_Ty, _Alloc>& _Left, const deque<_Ty, _Alloc>& _Right)
{
	return !(_Left < _Right);
}

}

#pragma pop_macro("new")
HTD_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#else
	#include <deque>
	namespace htd
	{
		template <class _Ty, class _Alloc = std::allocator<_Ty>> 
		using deque = std::deque<_Ty, _Alloc>;
	}

#endif


