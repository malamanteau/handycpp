
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

#include "HandyBase.hpp"
#include "HandyCompat.hpp"

namespace HANDY_NS {

	namespace
	{
		/// Helper to recognize spans.
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_span_impl
		{
			template <class ElementType> struct is_span : std::false_type {};
			template <class ElementType, std::ptrdiff_t Extent> struct is_span<std::span<ElementType, Extent>> : std::true_type { };
		}

		template <typename T> struct is_span
		{
			static constexpr bool const value = is_span_impl::is_span<std::decay_t<T>>::value;
		};

		/// Helper to recognize vectors and arrays.
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_vector_or_array_impl
		{
			template <typename T>       struct is_vector_or_array : std::false_type {};
			template <typename T, std::size_t N> struct is_vector_or_array<std::array    <T, N>> :std::true_type { };
			template <typename... Args> struct is_vector_or_array<std::vector            <Args...>> :std::true_type { };
		}

		template <typename T> struct is_vector_or_array
		{
			static constexpr bool const value = is_vector_or_array_impl::is_vector_or_array<std::decay_t<T>>::value;
		};

		/// Helper to recognize arrays.
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_array_impl
		{
			template <typename T>       struct is_array : std::false_type {};
			template <typename T, std::size_t N> struct is_array<std::array    <T, N>> :std::true_type { };
		}

		template <typename T> struct is_array
		{
			static constexpr bool const value = is_array_impl::is_array<std::decay_t<T>>::value;
		};

		/// Helper to recognize vectors
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_vector_impl
		{
			template <typename T>       struct is_vector : std::false_type { };
			template <typename... Args> struct is_vector<std::vector<Args...>> :std::true_type { };
		}

		template <typename T> struct is_vector
		{
			static constexpr bool const value = is_vector_impl::is_vector<std::decay_t<T>>::value;
		};

		/// Helper to recognize strings
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_basic_string_impl
		{
			template <typename T>       struct is_basic_string : std::false_type { };
			template <typename... Args> struct is_basic_string<std::basic_string<Args...>> :std::true_type { };
		}

		template <typename T> struct is_basic_string
		{
			static constexpr bool const value = is_basic_string_impl::is_basic_string<std::decay_t<T>>::value;
		};

		/// Helper to recognize string_views
		// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
		namespace is_basic_string_view_impl
		{
			template <typename T>       struct is_basic_string_view : std::false_type { };
			template <typename... Args> struct is_basic_string_view<std::basic_string_view<Args...>> :std::true_type { };
		}

		template <typename T> struct is_basic_string_view
		{
			static constexpr bool const value = is_basic_string_view_impl::is_basic_string_view<std::decay_t<T>>::value;
		};

		/// Helper to get the size of a std::array STATICALLY
		template<class Array, size_t N = 0, bool B = std::is_base_of<std::array<typename Array::value_type, N>, Array>::value>
		struct size_of : size_of<Array, N + 1, std::is_base_of<std::array<typename Array::value_type, N + 1>, Array>::value> {};

		template<class Array, size_t N>
		struct size_of<Array, N, true> : std::integral_constant<size_t, N> {};

		// this is a replacement for `static Array::size()`    
		template<class Array, size_t N = size_of<Array>::value>
		constexpr size_t static_size() { return N; }

		// this version can be called with an object like `static Array::size()` could
		template<class Array, size_t N = size_of<Array>::value>
		constexpr size_t static_size(Array const&) { return N; }
	}

	/// -------------------------------------------------------------------------------------------------
	/// Types that are already built-in and work:
	/// 
	/// Any arithmetic type
	/// 
	/// std::span 
	///     T can be any type.
	///     When you GET a VALUE:
	///         If the span is .empty():
	///             The data will be allocated with new[], and will need to be deallocated with delete[] myspan.data()!
	///         If the span is !.empty():
	///             If the value data is the wrong size, a nullptr will be returned.
	/// 
	/// std::array
	///     T can be any type.
	///     When you GET a VALUE, if the data is the wrong size to fit in the array, a nullptr will be returned.
	/// 
	/// std::vector
	///     T can be any type EXCEPT bool, this is because the bool type specifically is specialized and the storage is not assured.
	/// 
	/// Any of the std::basic_string and std::basic_string_view (Serialization only) types, 
	///
	/// Handy::Guid
	///     When GETing a value of type Guid, if pre-sized value data is the wrong size, a nullptr will be returned.
	///
	///
	/// -------------------------------------------------------------------------------------------------
	/// Any other types, you will need to either:
	///     1. Specialize the to_span, and prepare_span methods, or 
	///     2. Specialize the Handy::Serializer and Handy::Deserializer classes.
	///
	/// Do (1) if your class already is readable and writeable as an array (ie has .data() and .size(), or can be read/written in a block).
	/// Do (2) if you need to prepare serialized data going in or out.
	///     For an example of this, look at the example below:
	///
	/// Example of (2): 
	///		We need to use/add a string to the class's fields, so we specialize the WHOLE class template:
	///		template <> struct Serializer<json> // Implement the serializer for an arbitrary json class:
	///		{
	/// 		std::string Dump;
	///	
	///			std::span<std::byte const> to_span(json const & inItem)
	/// 		{
	/// 			Dump = inItem.dump();
	/// 			return std::span<std::byte const>((std::byte *)(void *)Dump.data(), Dump.size());
	/// 		}
	///		};
	///
	///		We need to use/add a string to the class's fields, so we specialize the WHOLE class template:
	///		template <> struct Deserializer<json> // Implement the deserializer for an arbitrary json class:
	///		{
	/// 		json * dest;
	/// 		std::string temp;
	///		
	/// 		std::byte * prepare_span(size_t sizeBytes, json & outItem)
	/// 		{
	///             dest = &outItem;
	/// 			temp.resize(sizeBytes);
	/// 			return (std::byte *)(void *)temp.data();
	/// 		}
	/// 
	///			~Deserializer()
	/// 		{
	/// 			*dest = json::parse(temp);
	/// 		}
	///		};
	/// -------------------------------------------------------------------------------------------------

	/// The methods will always be called in the order: ctor, to_span, dtor.
	template <typename T>
	struct Serializer
	{
		/// If you have a complicated type, specialize the entire class rather than just the to_span, particularly if 
		/// you have complicated serialization code to manage (which can be handled in the ctor and dtor!).

		/// "Gimme a span I can read the data for inItem. When the dtor is called, I'm done reading it."
		std::span<std::byte const> get_span(T const & inItem)
		{
			constexpr bool isArithmetic    = std::is_arithmetic<T>       ::value || 
			                                 std::is_same<std::byte, T>  ::value;
			constexpr bool isSpan          =      is_span<T>             ::value;
			constexpr bool isVector        =      is_vector<T>           ::value;
			constexpr bool isArray         =      is_array<T>            ::value;
			constexpr bool isVectorOrArray =      is_vector_or_array<T>  ::value;
			constexpr bool isString        =      is_basic_string<T>     ::value;
			constexpr bool isStringView    =      is_basic_string_view<T>::value;

			// Common:
			static_assert(isStringView || isString || isArithmetic || isSpan || isVectorOrArray, "Handy SerDe must be implemented for your type");
			if constexpr(isVector) /// Prevent the static_assert from evaluating T::value_type on non-vectors
				static_assert(!isVector || (!std::is_same<bool, typename T::value_type>::value),     "Handy SerDe : not available for std::vector<bool>>");

			/// arithmetic type case:
			if constexpr (isArithmetic)
			{
				return std::span<std::byte const>((std::byte *)(void *)&inItem, sizeof(T));
			}
			/// std::span case:
			else if constexpr (isSpan)
			{
				return std::as_bytes(inItem);
			}
			/// std::vector, array, string_view, or string case:
			else if constexpr (isVectorOrArray || isString || isStringView)
			{
				return std::span<std::byte const>((std::byte *)(void *)inItem.data(), inItem.size() * sizeof(typename T::value_type));
			}

			throw std::runtime_error("Uncaught bad Serializer<> type error. This should have been a compile time error.");
		}
	};

	/// --

	template <typename T>
	struct Deserializer
	{
		/// "Gimme a pointer to write my data to. Here's how big it needs to be. When the dtor is called, you can be sure it's done."
		std::byte * prepare_span(size_t sizeBytes, T & outItem)
		{
			static_assert(!std::is_const<T>::value, "Cannot deserialize to a const type");

			constexpr bool isArithmetic    = std::is_arithmetic<T>       ::value || 
			                                 std::is_same<std::byte, T>  ::value;
			constexpr bool isSpan          =      is_span<T>             ::value;
			constexpr bool isVector        =      is_vector<T>           ::value;
			constexpr bool isArray         =      is_array<T>            ::value;
			constexpr bool isVectorOrArray =      is_vector_or_array<T>  ::value;
			constexpr bool isString        =      is_basic_string<T>     ::value;
			constexpr bool isStringView    =      is_basic_string_view<T>::value;

			// Unique to Deserializer:
			static_assert(!                     std::is_const<T>                       ::value,  "Handy::Deserializer cannot be used on any type that is const");
			if constexpr(isSpan) /// Prevent the static_assert from evaluating T::element_type on non-spans
				static_assert(!isSpan          || (!std::is_const<typename T::element_type>::value), "Handy::Deserializer cannot be used on a span whose element_type is const");
			if constexpr(isVectorOrArray) /// Prevent the static_assert from evaluating T::value_type on non-vec/arrs
				static_assert(!isVectorOrArray || (!std::is_const<typename T::value_type>  ::value), "Handy::Deserializer cannot be used on a type whose value_type is const");

			// Common:      std::string_view is immutable so not a deserialization target
			static_assert(/*isStringView || */isString || isArithmetic || isSpan || isVectorOrArray, "Handy SerDe must be implemented for your type");
			if constexpr(isVector) /// Prevent the static_assert from evaluating T::value_type on non-vectors
				static_assert(!isVector || (!std::is_same<bool,      typename T::value_type>::value),    "Handy SerDe : not available for std::vector<bool>>");

			/// arithmetic type case:
			if constexpr (isArithmetic)
			{
				if (sizeBytes != sizeof(T))
				{
					#if defined HANDY_SERDE_NOISY_NULLRET
					std::cerr << "Handy::Deserializer - arithmetic type T is the wrong size to be assigned this data.";
					#endif
					return nullptr;
				}

				return (std::byte *)(void *)&outItem;
			}
			/// std::span case:
			else if constexpr (isSpan)
			{
				if (outItem.empty())
				{
					if (sizeBytes % sizeof(typename T::element_type) != 0)
					{
						#if defined HANDY_SERDE_NOISY_NULLRET
						std::cerr << "Handy::Deserializer - std::span element_type is the wrong size to be assigned this data.";
						#endif
						return nullptr;
					}

					outItem = T(new typename T::element_type[sizeBytes / sizeof(typename T::element_type)], sizeBytes);
					/// YOU MUST DELETE the span.data() after you are done with it! Like this: delete [] span.data();
				}
				else
				{
					if (sizeBytes != (size_t)outItem.size_bytes())
					{
						#if defined HANDY_SERDE_NOISY_NULLRET
						std::cerr << "Handy::Deserializer - std::array value_type is the wrong size to be assigned this data.";
						#endif
						return nullptr;
					}

					return (std::byte *)(void *)outItem.data();
				}

				return (std::byte *)(void *)outItem.data();
			}
			/// std::vector, string case:
			else if constexpr (isVector || isString)
			{
				if (sizeBytes % sizeof(typename T::value_type) != 0)
				{
					#if defined HANDY_SERDE_NOISY_NULLRET
					std::cerr << "Handy::Deserializer - std::vector/basic_string value_type is the wrong size to be assigned this data.";
					#endif
					return nullptr;
				}

				outItem.resize(sizeBytes / sizeof(typename T::value_type));
				return (std::byte *)(void *)outItem.data();
			}
			/// std::array case:
			else if constexpr (isArray)
			{
				if (sizeBytes != outItem.size() * sizeof(typename T::value_type))
				{
					#if defined HANDY_SERDE_NOISY_NULLRET
					std::cerr << "Handy::Deserializer - std::array value_type is the wrong size to be assigned this data.";
					#endif
					return nullptr;
				}
				return (std::byte *)(void *)outItem.data();
			}

			/// Everything else:
			throw std::runtime_error("Uncaught bad Deserializer<> type error. This should have been a compile time error.");
		}

		/// The methods will always be called in the above order: ctor, prepare(), dtor.
	};
}

