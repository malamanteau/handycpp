
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

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>

#include "HandyBase.hpp"
#include "HandyCompat.hpp"

namespace HANDY_NS {

	///-----------------------------------------------
	/// Helpful Try-Get functions for std::map and 
	/// std::unordered_map, these could be duplicated 
	/// for other collections.
	template<typename TKey, typename TVal, typename C, typename A>
	std::optional<TVal> TryGet(std::map<TKey, TVal, C, A> const & m, TKey const & k)
	{
		auto iter = m.find(k);
		if (iter == m.end())
			return std::optional<TVal>();
		return iter->second;
	}

	template<typename TKey, typename TVal, typename C, typename A>
	TVal TryGetDef(std::map<TKey, TVal, C, A> const & m, TKey const & k, TVal const & def)
	{
		auto iter = m.find(k);
		if (iter == m.end())
			return def;
		return iter->second;
	}

	template<typename TKey, typename Tp, typename C, typename A>
	Tp TryGetP(std::map<TKey, Tp, C, A> const & m, TKey const & k)
	{
		static_assert(std::is_pointer<Tp>::value, "Compile Error: TryGetP usage with non-pointer value type.");

		auto iter = m.find(k);
		if (iter == m.end())
			return nullptr;
		return iter->second;
	}

	template<typename TKey, typename Tp, typename C, typename A>
	Tp TryGetRemoveP(std::map<TKey, Tp, C, A> & m, TKey const & k) noexcept
	{
		static_assert(std::is_pointer<Tp>::value, "Compile Error: TryGetRemoveP usage with non-pointer value type.");

		auto find = m.find(k);
		if (find == m.end())
			return nullptr;
		Tp ret = find->second;

		m.erase(find);

		return ret;
	}

	template<typename TKey, typename TVal, typename H, typename E, typename A>
	std::optional<TVal> TryGet(std::unordered_map<TKey, TVal, H, E, A> const & m, TKey const & k)
	{
		auto iter = m.find(k);
		if (iter == m.end())
			return std::optional<TVal>();
		return iter->second;
	}

	template<typename TKey, typename TVal, typename H, typename E, typename A>
	TVal TryGetDef(std::unordered_map<TKey, TVal, H, E, A> const & m, TKey const & k, TVal const & def)
	{
		auto iter = m.find(k);
		if (iter == m.end())
			return def;
		return iter->second;
	}

	template<typename TKey, typename Tp, typename H, typename E, typename A>
	Tp TryGetP(std::unordered_map<TKey, Tp, H, E, A> const & m, TKey const & k)
	{
		static_assert(std::is_pointer<Tp>::value, "Compile Error: TryGetP usage with non-pointer value type.");

		auto iter = m.find(k);
		if (iter == m.end())
			return nullptr;
		return iter->second;
	}

	template<typename TKey, typename Tp, typename H, typename E, typename A>
	Tp TryGetRemoveP(std::unordered_map<TKey, Tp, H, E, A> & m, TKey const & k)
	{
		static_assert(std::is_pointer<Tp>::value, "Compile Error: TryGetRemoveP usage with non-pointer value type.");

		auto find = m.find(k);
		if (find == m.end())
			return nullptr;
		Tp ret = find->second;

		m.erase(find);

		return ret;
	}

	template<typename TKey, typename TVal, typename C, typename A>
	bool Contains(std::map<TKey, TVal, C, A> const & m, TKey const & k)
	{
		auto iter = m.find(k);
		return iter != m.end();
	}

	template<typename TKey, typename TVal, typename H, typename E, typename A>
	bool Contains(std::unordered_map<TKey, TVal, H, E, A> const & m, TKey const & k)
	{
		auto iter = m.find(k);
		return iter != m.end();
	}

	template<typename T, typename C, typename A>
	bool Contains(std::set<T, C, A> const & m, T const & k)
	{
		auto iter = m.find(k);
		return iter != m.end();
	}

	template<typename T, typename H, typename E, typename A>
	bool Contains(std::unordered_set<T, H, E, A> const & m, T const & k)
	{

		auto iter = m.find(k);
		return iter != m.end();
	}

	template<typename T, typename A>
	bool Contains(std::vector<T, A> const & m, T const & k)
	{
		auto iter = std::find(m.cbegin(), m.cend(), k);
		return iter != m.cend();
	}

	template<typename T, typename A>
	bool Contains(std::deque<T, A> const & m, T const & k)
	{
		auto iter = std::find(m.cbegin(), m.cend(), k);
		return iter != m.cend();
	}

	template<typename T, typename A>
	size_t IndexOf(std::vector<T, A> const & v, T const & k)
	{
		auto iter = std::find(v.cbegin(), v.cend(), k);

		if (iter == v.cend())
			return -1;

		return iter - v.cbegin();
	}

	template<typename T, typename A>
	size_t IndexOf(std::deque<T, A> const & v, T const & k)
	{
		auto iter = std::find(v.cbegin(), v.cend(), k);

		if (iter == v.cend())
			return -1;

		return iter - v.cbegin();
	}

	template<typename T, typename A>
	bool RemoveFirstOf(std::vector<T, A> & v, T const & k)
	{
		size_t idx = IndexOf(v, k);
		if (idx != size_t(-1))
		{
			v.erase(v.begin() + idx);
			return true;
		}

		return false;
	}

	template<typename T, typename A>
	bool RemoveFirstOf(std::deque<T, A> & v, T const & k)
	{
		size_t idx = IndexOf(v, k);
		if (idx != -1)
		{
			v.erase(v.begin() + idx);
			return true;
		}

		return false;
	}

	template<typename K, typename C, typename A>
	std::set<K, C, A> Intersection(std::set<K, C, A> const & s1, std::set<K, C, A> const & s2) noexcept
	{
		std::set<K, C, A> ret;
		std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(ret, ret.begin()));
		return ret;
	}

	template<typename K, typename H, typename E, typename A>
	static std::unordered_set<K, H, E, A> Intersection(std::unordered_set<K, H, E, A> const & s1, std::unordered_set<K, H, E, A> const & s2) noexcept
	{
		std::unordered_set<K, H, E, A> ret;
		auto s1s = s1.size();
		auto s2s = s2.size();
		auto &in1 = s1s > s2s ? s2 : s1;
		auto &in2 = s1s > s2s ? s1 : s2;

		auto e = in2.end();
		for (auto & v : in1)
		{
			if (in2.find(v) != e)
				ret.insert(v);
		}

		return ret;
	}

	template<typename K, typename H, typename E, typename A, typename A2>
	static std::unordered_set<K, H, E, A> Intersection(std::vector<K, A2> const & s1, std::unordered_set<K, H, E, A> const & s2) noexcept
	{
		std::unordered_set<K, H, E, A> ret;
		auto s1s = s1.size();
		auto s2s = s2.size();
		
		if (s1s > s2s)
		{
			auto e = s1.end();
			for (auto & v : s2)
				if (Contains(s1, v))
					ret.insert(v);
		}
		else
		{
			auto e = s2.end();
			for (auto & v : s1)
				if (s2.find(v) != e)
					ret.insert(v);
		}
		return ret;
	}

	template<typename T>
	void ConcatInPlace(std::vector<T> & dstEnd, std::vector<T> const & src)
	{
		dstEnd.insert(dstEnd.end(), src.cbegin(), src.cend());
	}


	///-----------------------------------------------

} // HANDY_NS
