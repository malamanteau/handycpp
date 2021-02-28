#pragma once

/// ========================================================================
/// UNLICENSE
/// 
/// small_vector (https://github.com/awishnick/small_vector)
/// awishnick
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

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <memory>  // std::allocator
#include <utility> // std::move

template <class T, size_t SmallSize>
class small_vector_storage 
{
protected:
	small_vector_storage() {}

	      T * small_begin()       { return reinterpret_cast<      T *>(&m_storage[0]); }
	const T * small_begin() const { return reinterpret_cast<const T *>(&m_storage[0]); }
	      T * small_end()         { return small_begin() + SmallSize; }
	const T * small_end()   const { return small_begin() + SmallSize; }

	char m_storage[sizeof(T) * SmallSize];
private:
	small_vector_storage(small_vector_storage<T, SmallSize> const &);
	small_vector_storage<T, SmallSize> & operator=(small_vector_storage<T, SmallSize> const &);
};

template <class T>
class small_vector_storage<T, 0>
{
protected:
	small_vector_storage() {}

	T * small_begin()       { return NULL; }
	T * small_begin() const { return NULL; }
	T * small_end()   const { return NULL; }
private:
	small_vector_storage(const small_vector_storage<T, 9>&);
	small_vector_storage<T, 0> & operator=(const small_vector_storage<T, 0>&);
};

template <class T, size_t SmallSize>
class small_vector : private small_vector_storage<T, SmallSize>, private std::allocator<T>
{
	typedef small_vector_storage<T, SmallSize> storage_base;

public:
	typedef T                       value_type;
	typedef std::allocator<T>   allocator_type;
	typedef       value_type &        reference;
	typedef const value_type &  const_reference;
	typedef       T *                 iterator;
	typedef const T *           const_iterator;
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	// Check the standard on these two. Supposed to use std::allocator_traits
	typedef       T *                  pointer;
	typedef const T *            const_pointer;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// 23.3.6.2, construct/copy/destroy:
	explicit small_vector()
		: m_begin       (storage_base::small_begin())
		, m_end         (storage_base::small_begin())
		, m_capacity_end(storage_base::small_end()) 
	{ }

	explicit small_vector(size_type n)
		: m_begin       (storage_base::small_begin())
		, m_end         (storage_base::small_begin())
		, m_capacity_end(storage_base::small_end()) 
	{
		// If n is greater than the small size, allocate memory first. Otherwise we can use our small storage.
		if (n > SmallSize)
		{
			m_begin = std::allocator<T>::allocate(n);
			m_capacity_end = m_begin + n;
		}
		m_end = m_begin + n;

		uninitialized_fill(m_begin, m_end, T()); // Fill our range with a default-constructed value
	}

	small_vector(size_type n, T const & value)
		: m_begin(storage_base::small_begin())
		, m_end(storage_base::small_begin())
		, m_capacity_end(storage_base::small_end()) 
	{
		// If n is greater than the small size, allocate memory first. Otherwise we can use our small storage.
		if (n > SmallSize)
		{
			m_begin = std::allocator<T>::allocate(n);
			m_capacity_end = m_begin + n;
		}
		m_end = m_begin + n;
			
		uninitialized_fill(m_begin, m_end, value); // Fill our range with a default-constructed value
	}

	template <class InputIterator>
	small_vector(InputIterator first, InputIterator last)
		: m_begin(storage_base::small_begin())
		, m_end(storage_base::small_begin())
		, m_capacity_end(storage_base::small_end()) 
	{
		typedef typename std::iterator_traits<InputIterator>::iterator_category iterator_category;
		range_construct(first, last, iterator_category());
	}

	template <size_type OtherSize>
	small_vector(small_vector<T, OtherSize> const & x)
		: m_begin(storage_base::small_begin())
		, m_end(storage_base::small_begin())
		, m_capacity_end(storage_base::small_end())
	{
		range_construct(x.begin(), x.end(), std::random_access_iterator_tag());
	}

	// Need a separate non-templated copy constructor, otherwise the default copy constructor gets synthesized and used
	small_vector(const small_vector<T, SmallSize> & x) :
		m_begin(storage_base::small_begin()),
		m_end(storage_base::small_begin()),
		m_capacity_end(storage_base::small_end())
	{

		range_construct(x.begin(), x.end(), std::random_access_iterator_tag());
	}

	~small_vector()
	{
		destroy_range(m_begin, m_end); // Destroy our objects

		if (!is_small()) // Free our memory if not using the small storage
			std::allocator<T>::deallocate(m_begin, capacity());
	}

	// iterators:
	iterator               begin()         { return m_begin; }
	const_iterator         begin()   const { return m_begin; }
	iterator               end()           { return m_end; }
	const_iterator         end()     const { return m_end; }
	reverse_iterator       rbegin()        { return std::reverse_iterator<iterator>(end()); }
	const_reverse_iterator rbegin()  const { return std::reverse_iterator<const_iterator>(end()); }
	reverse_iterator       rend()          { return std::reverse_iterator<iterator>(begin()); }
	const_reverse_iterator rend()    const { return std::reverse_iterator<const_iterator>(begin()); }
	const_iterator         cbegin()  const { return begin(); }
	const_iterator         cend()    const { return end(); }
	const_reverse_iterator crbegin() const { return rbegin(); }
	const_reverse_iterator crend()   const { return rend(); }

	// 23.3.6.3, capacity:
	size_type size()     const { return m_end - m_begin; }
	size_type max_size() const { return std::numeric_limits<size_type>::max(); }
	size_type capacity() const { return m_capacity_end - m_begin; }
	bool empty()         const { return m_begin == m_end; }

	// element access:
	reference operator[](size_type n) { return m_begin[n]; }
	const_reference operator[](size_type n) const { return m_begin[n]; }

	//static T && mymove(T && t) { return static_cast<T&&>(t); }
	static T & mymove(T & t) { return t; }

	// 23.3.6.5, modifiers:
	void push_back(T const & x)
	{
		// If we do not have enough capacity, reallocate
		size_type const new_size = size() + 1;
		if (new_size > capacity())
		{
			// Reallocate a bigger array, but make it still have size()
			// elements, not size() + 1

			// This could throw bad_alloc
			size_type const new_capacity = std::max<size_type>(1u, 2 * capacity());
			T * new_begin = std::allocator<T>::allocate(new_capacity);

			// Copy- or move-construct elements. If the constructor throws,
			// we'll delete our new array and rethrow.
			// After constructing the new element, we destroy the old one.
			try 
			{
				T * old_elem = m_begin;
				for (T * new_elem = new_begin; old_elem != m_end; ++new_elem, ++old_elem)
				{
					std::allocator<T>::construct(new_elem, mymove(*old_elem));
					std::allocator<T>::destroy(old_elem);
				}
			}
			catch (...)
			{
				std::allocator<T>::deallocate(new_begin, new_capacity);
				throw;
			}

			// Now use the new array and free the old one
			T * old_begin = m_begin;

			size_type const old_size     = size();
			size_type const old_capacity = capacity();

			bool const was_small = is_small();

			m_begin        = new_begin;
			m_end          = new_begin + old_size;
			m_capacity_end = new_begin + new_capacity;

			// Only free memory if it's not from our small backing storage
			if (!was_small)
				std::allocator<T>::deallocate(old_begin, old_capacity);
		}

		// Now just construct the new element
		std::allocator<T>::construct(m_end, x);
		++m_end;
	}

	// Returns whether we're using our small storage
	bool is_small() const { return m_begin == storage_base::small_begin(); }

private:
	T * m_begin;
	T * m_end;
	T * m_capacity_end;

	// Initializes the range [first, last) to value. Doesn't destruct the
	// range because it assumes that no objects have been constructed there.
	void uninitialized_fill(T * first, T * last, T const & value)
	{
		for (; first != last; ++first)
			std::allocator<T>::construct(first, value);
	}

	// Destroys the objects in the range [first, last)
	void destroy_range(T * first, T * last)
	{
		for (; first != last; ++first)
			std::allocator<T>::destroy(first);
	}

	// Range construct for multi-pass iterators
	template <class Iterator>
	void range_construct_multipass(Iterator first, Iterator last)
	{
		// Allocate space
		size_type const n = std::distance(first, last);
		if (n > SmallSize)
		{
			m_begin = std::allocator<T>::allocate(n);
			m_capacity_end = m_begin + n;
		}
		m_end = m_begin + n;

		// Copy construct the range
		for (T * elem = m_begin; first != last; ++first, ++elem)
		{
			std::allocator<T>::construct(elem, *first);
		}
	}

	template <class ForwardIterator>
	void range_construct(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag) 
	{
		range_construct_multipass(first, last);
	}

	template <class BidirectionalIterator>
	void range_construct(BidirectionalIterator first, BidirectionalIterator last, std::bidirectional_iterator_tag) 
	{
		range_construct_multipass(first, last);
	}

	template <class RandomAccessIterator>
	void range_construct(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag)
	{
		range_construct_multipass(first, last);
	}

	// Range construct for input iterators
	template <class InputIterator>
	void range_construct(InputIterator first, InputIterator last, std::input_iterator_tag) 
	{
		for(; first != last; ++first)
			push_back(*first);
	}
};