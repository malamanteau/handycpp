
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
#include "HandyCompat.hpp" // For std::optional

namespace HANDY_NS {

	struct Result final
	{
		bool Success = false;
		std::string Reason;

		Result() = default;

		FORCEINLINE Result(bool success)
			: Success(success)
			, Reason("")
		{ }


		FORCEINLINE Result(bool success, std::string const & reason)
			: Success(success)
			, Reason(reason)
		{ }

		FORCEINLINE explicit operator bool() const
		{
			return Success;
		}

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser.saveonly("_type", "Result");
			ser("Success", Success);
			ser("Reason", Reason);
		}
	};

	template <typename EnumOutcomeType>
	struct ResultE final
	{
		static_assert(std::is_enum_v<EnumOutcomeType>, "Compile Error: ResultE usage with non-enum value type.");

		EnumOutcomeType State;
		std::string     Reason;

		/*explicit*/ ResultE(EnumOutcomeType state)
			: State(state)
			, Reason("")
		{ }

		ResultE(EnumOutcomeType state, std::string const & reason)
			: State(state)
			, Reason(reason)
		{ }

		explicit operator EnumOutcomeType() const
		{
			return State;
		}
	};

	
	template <typename TPtr>
	struct ResultP final
	{

		static_assert(std::is_pointer_v<TPtr>, "Compile Error: ResultP usage with non-pointer value type.");

		/// TODO This should be TPtr* w/o the static_assert.
		TPtr        Value = nullptr;
		std::string Reason;

		bool Success() const { return Value != nullptr; }

		/*explicit */ResultP(TPtr value)
			: Value(value)
		{ }

		ResultP(TPtr value, std::string_view reason)
			: Value(value)
			, Reason(reason)
		{ }

		ResultP(std::string_view reason)
			: Reason(reason)
		{ }

		explicit operator TPtr() const
		{
			return Value;
		}
	};

	template <typename TPtr, typename EnumOutcomeType>
	struct ResultPE final
	{
		static_assert(std::is_enum_v<EnumOutcomeType>, "Compile Error: ResultPE usage with non-enum value type.");

		TPtr *          Value = nullptr;
		EnumOutcomeType State;
		std::string     Reason;

		ResultPE(TPtr * value, EnumOutcomeType state, std::string_view reason)
			: Value(value)
			, State(state)
			, Reason(reason)
		{ }

		/*explicit */ResultPE(EnumOutcomeType state)
			: State(state)
		{ }

		ResultPE(EnumOutcomeType state, std::string const & reason)
			: State(state)
			, Reason(reason)
		{ }

		ResultPE(TPtr * value, EnumOutcomeType state)
			: Value(value)
			, State(state)
		{ }
	};


	template <typename T>
	struct ResultV final
	{
		bool Success = false;
		std::string Reason;
		std::optional<T> OpValue;

		//ResultV(bool success, T value) 
		//: 
		//	Success(success),
		//	Reason(""),
		//	OpValue(value)
		//{ }

		ResultV(bool success, T const & value)
			: Success(success)
			, Reason("")
			, OpValue(value)
		{ }

		ResultV(bool success, T && value)
			: Success(success)
			, Reason("")
			, OpValue(std::move(value))
		{ }

		ResultV(bool success, std::string const & reason)
			: Success(success)
			, Reason(reason)
		{
			//			if (!success)
			//				std::cout << "Result failed: " << reason << std::endl;
		}

		explicit operator bool() const
		{
			return Success;
		}
	};





	struct ResultS final
	{
		bool Success = false;
		std::string Reason;
		std::optional<std::string> OpValue;

		ResultS(bool success, std::string var)
			: Success(success)
			, Reason("")
		{
			if (Success)
				OpValue = var;
			else
				Reason = var;
		}

		ResultS(bool success)
			: Success(success)
			, Reason("")
		{ }

		std::string Value()
		{
			return OpValue.has_value() ? OpValue.value() : "";
		}

		explicit operator bool() const
		{
			return Success;
		}
	};

} // HANDY_NS
