
/// ========================================================================
/// TCLAP (http://tclap.sourceforge.net/)
///
/// LICENSE - MIT (https://opensource.org/licenses/MIT)
/// 
/// Copyright (c) 2003 Michael E. Smoot 
/// Copyright (c) 2004 Daniel Aarno
/// Copyright (c) 2017 Google Inc.
///
/// Permission is hereby granted, free of charge, to any person 
/// obtaining a copy of this software and associated documentation 
/// files (the "Software"), to deal in the Software without restriction, 
/// including without limitation the rights to use, copy, modify, merge, 
/// publish, distribute, sublicense, and/or sell copies of the Software, 
/// and to permit persons to whom the Software is furnished to do so, 
/// subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be 
/// included in all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
/// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
/// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
/// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
/// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
/// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
/// THE SOFTWARE.
/// ========================================================================

#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h> // Needed for exit(), which isn't defined in some envs.
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

#include "HandyBase.hpp"
#include "HandyString.hpp"

// If Microsoft has already typedef'd wchar_t as an unsigned short, then compiles will break 
// because it's as if we're creating ArgTraits twice for unsigned short. Thus...
#ifdef _MSC_VER
	#ifndef _NATIVE_WCHAR_T_DEFINED
		#define TCLAP_DONT_DECLARE_WCHAR_T_ARGTRAITS
	#endif
#endif

namespace Handy::Args {

	// A simple class that defines and argument exception. Should be caught whenever a CmdLine is created and parsed.
	class ArgException : public std::exception
	{
		std::string _errorText; // The text of the exception message.
		std::string _argId; // The argument related to this exception.
		std::string _typeDescription; // Describes the type of the exception.  Used to distinguish between different child exceptions.
	public:

		//   \param text - The text of the exception.
		//   \param id - The text identifying the argument source.
		//   \param td - Text describing the type of ArgException it is of the exception.
		ArgException(std::string const & text = "undefined exception", std::string const & id   = "undefined", std::string const & td   = "Generic ArgException")
			: std::exception()
			, _errorText(text)
			, _argId(id)
			, _typeDescription(td)
		{ }

		virtual ~ArgException() throw() { }

		// Returns the error text.
		std::string error() const { return (_errorText); }

		// Returns the argument id.
		std::string argId() const { return _argId == "undefined" ? " " : ("Argument: " + _argId); }

		// Returns the arg id and error text.
		const char* what() const throw()
		{
			static std::string ex;
			ex = _argId + " -- " + _errorText;
			return ex.c_str();
		}

		// Returns the type of the exception.  Used to explain and distinguish between different child exceptions.
		std::string typeDescription() const { return _typeDescription; }
	};

	// Thrown from within the child Arg classes when it fails to properly parse the argument it has been passed.
	class ArgParseException : public ArgException
	{
	public:
		//   \param text - The text of the exception.
		//   \param id - The text identifying the argument source of the exception.
		ArgParseException(std::string const & text = "undefined exception", std::string const & id = "undefined")
			: ArgException(text, id, std::string("Exception found while parsing ") + std::string("the value the Arg has been passed."))
		{ }
	};

	// Thrown from CmdLine when the arguments on the command line are not properly specified, e.g. too many arguments, required argument missing, etc.
	class CmdLineParseException : public ArgException
	{
	public:
		//   \param text - The text of the exception.
		//   \param id - The text identifying the argument source of the exception.
		CmdLineParseException(std::string const & text = "undefined exception", std::string const & id = "undefined")
			: ArgException(text, id, std::string("Exception found when the values on the command line do not meet the requirements of the defined Args."))
		{ }
	};

	// Thrown from Arg and CmdLine when an Arg is improperly specified, e.g. same flag as another Arg, same name, etc.
	class SpecificationException : public ArgException
	{
	public:
		//   \param text - The text of the exception.
		//   \param id - The text identifying the argument source of the exception.
		SpecificationException(std::string const & text = "undefined exception",
							   std::string const & id = "undefined")
			: ArgException(text, id, std::string("Exception found when an Arg object is improperly defined by the developer."))
		{ }

	};

	class ExitException
	{
		int _estat;
	public:
		ExitException(int estat) : _estat(estat) {}
		int getExitStatus() const { return _estat; }
	};


	// A base class that defines the interface for visitors.
	class Visitor
	{
	public:

		Visitor() { }
		virtual ~Visitor() { }

		// This method (to implemented by children) will becalled when the visitor is visited.
		virtual void visit() = 0;
	};
	
	class Arg;
	class CmdLineOutput;
	class XorHandler;

	// The base class that manages the command line definition and passes along the parsing to the appropriate Arg classes.
	class CmdLineInterface
	{
	public:

		virtual ~CmdLineInterface() {}

		// Adds an argument to the list of arguments to be parsed.
		//   \param a - Argument to be added.
		virtual void add(Arg& a) = 0;

		// An alternative add.  Functionally identical.
		//   \param a - Argument to be added.
		virtual void add(Arg* a) = 0;

		// Add two Args that will be xor'd. If this method is used, add does not need to be called.
		//   \param a - Argument to be added and xor'd.
		//   \param b - Argument to be added and xor'd.
		virtual void xorAdd(Arg& a, Arg& b) = 0;

		// Add a list of Args that will be xor'd.  If this method is used, add does not need to be called.
		//   \param xors - List of Args to be added and xor'd.
		virtual void xorAdd(std::vector<Arg*>& xors) = 0;

		// Parses the command line.
		//   \param argc - Number of arguments.
		//   \param argv - Array of arguments.
		virtual void parse(int argc, const char * const * argv) = 0;

		// Parses the command line.
		//   \param args - A vector of strings representing the args. args[0] is still the program name.
		virtual void parse(std::vector<std::string>& args) = 0;

		// Returns the CmdLineOutput object.
		virtual CmdLineOutput* getOutput() = 0;

		//   \param co - CmdLineOutput object that we want to use instead.
		virtual void setOutput(CmdLineOutput* co) = 0;

		// Returns the version string.
		virtual std::string& getVersion() = 0;

		// Returns the program name string.
		virtual std::string& getProgramName() = 0;

		// Returns the argList.
		virtual std::list<Arg*>& getArgList() = 0;

		// Returns the XorHandler.
		virtual XorHandler& getXorHandler() = 0;

		// Returns the delimiter string.
		virtual char getDelimiter() = 0;

		// Returns the message string.
		virtual std::string& getMessage() = 0;

		// Indicates whether or not the help and version switches were created automatically.
		virtual bool hasHelpAndVersion() = 0;

		// Resets the instance as if it had just been constructed so that the instance can be reused.
		virtual void reset() = 0;
	};

	// We use two empty structs to get compile type specialization function to work

	// A value like argument value type is a value that can be set using operator>>. This is the default value type.
	struct ValueLike
	{
		typedef ValueLike ValueCategory;
		virtual ~ValueLike() {}
	};

	// A string like argument value type is a value that can be set using operator=(string). 
	// Useful if the value type contains spaces which will be broken up into individual tokens by operator>>.
	struct StringLike
	{
		virtual ~StringLike() {}
	};

	// A class can inherit from this object to make it have string like traits. This is a compile time thing and 
	// does not add any overhead to the inherenting class.
	struct StringLikeTrait
	{
		typedef StringLike ValueCategory;
		virtual ~StringLikeTrait() {}
	};

	// A class can inherit from this object to make it have value like traits. This is a compile time thing and 
	// does not add any overhead to the inherenting class.
	struct ValueLikeTrait 
	{
		typedef ValueLike ValueCategory;
		virtual ~ValueLikeTrait() {}
	};

	// Arg traits are used to get compile type specialization when parsing argument values. Using an ArgTraits you can specify the way that
	// values gets assigned to any particular type during parsing. The two supported types are StringLike and ValueLike. ValueLike is the
	// default and means that operator>> will be used to assign values to the type.
	template<typename T>
	class ArgTraits
	{
		// This is a bit silly, but what we want to do is:
		//   1) If there exists a specialization of ArgTraits for type X, use it.
		//   2) If no specialization exists but X has the typename X::ValueCategory, use the specialization for X::ValueCategory.
		//   3) If neither (1) nor (2) defines the trait, use the default which is ValueLike.
		// This is the "how":
		//
		// test<T>(0) (where 0 is the NULL ptr) will match
		// test(typename C::ValueCategory*) iff type T has the
		// corresponding typedef. If it does not test(...) will be
		// matched. This allows us to determine if T::ValueCategory
		// exists by checking the sizeof for the test function (return
		// value must have different sizeof).
		template<typename C> static short test(typename C::ValueCategory*) {}
		template<typename C> static long  test(...) {}
		static const bool hasTrait = sizeof(test<T>(0)) == sizeof(short);

		template <typename C, bool>
		struct DefaultArgTrait { typedef ValueLike ValueCategory; };

		template <typename C>
		struct DefaultArgTrait<C, true> { typedef typename C::ValueCategory ValueCategory; };

	public:
		typedef typename DefaultArgTrait<T, hasTrait>::ValueCategory ValueCategory;
	};

	// Integer types (signed, unsigned and bool) and floating point types all have value-like semantics.
	// Strings have string like argument traits.
	template<>
	struct ArgTraits<std::string> { typedef StringLike ValueCategory; };

	template<typename T>
	void SetString(T &dst, std::string const & src) { dst = src; }

	// A virtual base class that defines the essential data for all arguments. This class, or one of its existing children, must be subclassed to do anything.
	class Arg
	{
	private:
		COPY_ASSIGN_MOVE_CTOR(Arg, delete, delete, delete)

		// Indicates whether the rest of the arguments should be ignored.
		static bool& ignoreRestRef() { static bool ign = false; return ign; }

		// The delimiter that separates an argument flag/name from the value.
		static char& delimiterRef() { static char delim = ' '; return delim; }

	protected:

		// The single char flag used to identify the argument. This value (preceded by a dash {-}), can be used to identify
		// an argument on the command line.  The _flag can be blank, in fact this is how unlabeled args work.  Unlabeled args must
		// override appropriate functions to get correct handling. Note that the _flag does NOT include the dash as part of the flag.
		std::string _flag;

		// A single work namd indentifying the argument. This value (preceded by two dashed {--}) can also be used to identify an 
		// argument on the command line.  Note that the _name does NOT include the two dashes as part of the _name. The _name cannot be blank.
		std::string _name;

		// Description of the argument.
		std::string _description;

		// Indicating whether the argument is required.
		bool _required;

		// Label to be used in usage description.  Normally set to "required", but can be changed when necessary.
		std::string _requireLabel = "required";

		// Indicates whether a value is required for the argument. Note that the value may be required but the 
		// argument/value combination may not be, as specified by _required.
		bool _valueRequired;

		// Indicates whether the argument has been set.
		// Indicates that a value on the command line has matched the name/flag of this argument and the values have been set accordingly.
		bool _alreadySet = false;

		// A pointer to a vistitor object.
		// The visitor allows special handling to occur as soon as the argument is matched.  This defaults to nullptr and should not
		// be used unless absolutely necessary.
		Visitor * _visitor = nullptr;

		// Whether this argument can be ignored, if desired.
		bool _ignoreable = true;

		// Indicates that the arg was set as part of an XOR and not on the command line.
		bool _xorSet = false;

		bool _acceptsMultipleValues = false;

		// Performs the special handling described by the Visitor.
		void _checkWithVisitor() const { if (_visitor != nullptr) _visitor->visit(); }

		// Primary constructor. YOU (yes you) should NEVER construct an Arg directly, this is a base class that is extended by various children
		// that are meant to be used.  Use SwitchArg, ValueArg, MultiArg, UnlabeledValueArg, or UnlabeledMultiArg instead.
		//   \param flag - The flag identifying the argument.
		//   \param name - The name identifying the argument.
		//   \param desc - The description of the argument, used in the usage.
		//   \param req - Whether the argument is required.
		//   \param valreq - Whether the a value is required for the argument.
		//   \param v - The visitor checked by the argument. Defaults to nullptr.
		Arg(std::string const & flag,
			std::string const & name,
			std::string const & desc,
			bool req, bool valreq, Visitor * v = nullptr)
			: _flag(flag)
			, _name(name)
			, _description(desc)
			, _required(req)
			, _valueRequired(valreq)
			, _visitor(v)
		{
			if (_flag.length() > 1)
				throw(SpecificationException(
					"Argument flag can only be one character long", toString()));

			if (_name != ignoreNameString() &&
				(_flag == Arg::flagStartString() ||
				 _flag == Arg::nameStartString() ||
				 _flag == " "))
				throw(SpecificationException("Argument flag cannot be either '" +
											 Arg::flagStartString() + "' or '" +
											 Arg::nameStartString() + "' or a space.",
											 toString()));

			if ((_name.substr(0, Arg::flagStartString().length()) == Arg::flagStartString()) ||
				(_name.substr(0, Arg::nameStartString().length()) == Arg::nameStartString()) ||
				(_name.find(" ", 0) != std::string::npos))
				throw(SpecificationException("Argument name begin with either '" +
											 Arg::flagStartString() + "' or '" +
											 Arg::nameStartString() + "' or space.",
											 toString()));

		}

	public:
		virtual ~Arg() { }

		// Adds this to the specified list of Args.
		//   \param argList - The list to add this to.
		// Impl note: Overridden by Args that need to added to the end of the list.
		virtual void addToList(std::list<Arg*>& argList) const { argList.push_front(const_cast<Arg*>(this)); }

		// Begin ignoring arguments since the "--" argument was specified.
		static void beginIgnoring() { ignoreRestRef() = true; }

		// Whether to ignore the rest.
		static bool ignoreRest() { return ignoreRestRef(); }

		// The delimiter that separates an argument flag/name from the value.
		static char delimiter() { return delimiterRef(); }

		// The char used as a place holder when SwitchArgs are combined. Currently set to the bell char (ASCII 7).
		static char blankChar() { return (char)7; }

		// The char that indicates the beginning of a flag.  Defaults to '-', but clients can define HANDYARGS_FLAGSTARTCHAR to override.
		#ifndef HANDYARGS_FLAGSTARTCHAR
			#define HANDYARGS_FLAGSTARTCHAR '-'
		#endif
		static char flagStartChar() { return HANDYARGS_FLAGSTARTCHAR; }

		// The string that indicates the beginning of a flag.  Defaults to "-", but
		// clients can define HANDYARGS_FLAGSTARTSTRING to override. Should be the same as HANDYARGS_FLAGSTARTCHAR.
		#ifndef HANDYARGS_FLAGSTARTSTRING
			#define HANDYARGS_FLAGSTARTSTRING "-"
		#endif
		static std::string const flagStartString() { return HANDYARGS_FLAGSTARTSTRING; }

		// The string that indicates the beginning of a name.  Defaults to "--", but clients can define HANDYARGS_NAMESTARTSTRING to override.
		#ifndef HANDYARGS_NAMESTARTSTRING
			#define HANDYARGS_NAMESTARTSTRING "--"
		#endif
		static std::string const nameStartString() { return HANDYARGS_NAMESTARTSTRING; }

		// The name used to identify the ignore rest argument.
		static std::string const ignoreNameString() { return "ignore_rest"; }

		// Sets the delimiter for all arguments.
		//   \param c - The character that delimits flags/names from values.
		static void setDelimiter(char c) { delimiterRef() = c; }

		// Pure virtual method meant to handle the parsing and value assignment of the string on the command line.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. What is passed in from main.
		virtual bool processArg(int *i, std::vector<std::string>& args) = 0;

		// Equality operator. Must be virtual to handle unlabeled args.
		//   \param a - The Arg to be compared to this.
		virtual bool operator==(const Arg& a) const
		{
			if ((_flag != "" && _flag == a._flag) || _name == a._name)
				return true;
			else
				return false;
		}

		// Returns the argument flag.
		std::string const & getFlag() const { return _flag; }

		// Returns the argument name.
		std::string const & getName() const { return _name; }

		// Returns the argument description.
		std::string getDescription() const
		{
			std::string desc = "";
			if (_required)
				desc = "(" + _requireLabel + ")  ";

			//	if ( _valueRequired )
			//		desc += "(value required)  ";

			desc += _description;
			return desc;
		}


		// Indicates whether the argument is required.
		virtual bool isRequired() const { return _required; }

		// Sets _required to true. This is used by the XorHandler.
		// You really have no reason to ever use it.
		void forceRequired() { _required = true; }

		// Sets the _alreadySet value to true.  This is used by the XorHandler. You really have no reason to ever use it.
		void xorSet()
		{
			_alreadySet = true;
			_xorSet = true;
		}

		// Indicates whether a value must be specified for argument.
		bool isValueRequired() const { return _valueRequired; }

		// Indicates whether the argument has already been set.  Only true if the arg has been matched on the command line.
		bool isSet() const { return (_alreadySet && !_xorSet); }

		// Indicates whether the argument can be ignored, if desired.
		bool isIgnoreable() const { return _ignoreable; }

		// A method that tests whether a string matches this argument. This is generally called by the processArg() method.
		// This method could be re-implemented by a child to change how arguments are specified on the command line.
		//   \param s - The string to be compared to the flag/name to determine whether the arg matches.
		virtual bool argMatches(std::string const & argFlag) const
		{
			return ((argFlag == Arg::flagStartString() + _flag && _flag != "") ||
			         argFlag == Arg::nameStartString() + _name);
		}


		// Returns a simple string representation of the argument.
		// Primarily for debugging.
		virtual std::string toString() const	
		{
			std::string s = "";

			if (_flag != "")
				s += Arg::flagStartString() + _flag + " ";

			s += "(" + Arg::nameStartString() + _name + ")";

			return s;
		}

		// Returns a short ID for the usage.
		//   \param valueId - The value used in the id.
		virtual std::string shortID(std::string const & valueId = "val") const
		{
			std::string id = "";

			if (_flag != "")
				id = Arg::flagStartString() + _flag;
			else
				id = Arg::nameStartString() + _name;

			if (_valueRequired)
				id += std::string(1, Arg::delimiter()) + "<" + valueId + ">";

			if (!_required)
				id = "[" + id + "]";

			return id;
		}


		// Returns a long ID for the usage.
		//   \param valueId - The value used in the id.
		virtual std::string longID(std::string const & valueId = "val") const
		{
			std::string id = "";

			if (_flag != "")
			{
				id += Arg::flagStartString() + _flag;

				if (_valueRequired)
					id += std::string(1, Arg::delimiter()) + "<" + valueId + ">";

				id += ",  ";
			}

			id += Arg::nameStartString() + _name;

			if (_valueRequired)
				id += std::string(1, Arg::delimiter()) + "<" + valueId + ">";

			return id;

		}

		// Trims a value off of the flag.
		//   \param flag - The string from which the flag and value will be trimmed. Contains the flag once the value has been trimmed.
		//   \param value - Where the value trimmed from the string will be stored.
		virtual void trimFlag(std::string& flag, std::string& value) const
		{
			int stop = 0;
			for (int i = 0; static_cast<unsigned int>(i) < flag.length(); i++)
				if (flag[i] == Arg::delimiter())
				{
					stop = i;
					break;
				}

			if (stop > 1)
			{
				value = flag.substr(stop + 1);
				flag = flag.substr(0, stop);
			}

		}


		// Checks whether a given string has blank chars, indicating that it is a combined SwitchArg.  If so, return true, otherwise return false.
		//   \param s - string to be checked.
		bool _hasBlanks(std::string const & s) const
		{
			for (int i = 1; static_cast<unsigned int>(i) < s.length(); i++)
				if (s[i] == Arg::blankChar())
					return true;

			return false;
		}

		// Sets the requireLabel. Used by XorHandler.  You shouldn't ever use this.
		//   \param s - Set the requireLabel to this value.
		void setRequireLabel(std::string const & s) { _requireLabel = s; }

		// Used for MultiArgs and XorHandler to determine whether args can still be set.
		virtual bool allowMore() { return false; }

		// Use by output classes to determine whether an Arg accepts multiple values.
		virtual bool acceptsMultipleValues() { return _acceptsMultipleValues; }

		// Clears the Arg object and allows it to be reused by new command lines.
		virtual void reset()
		{
			_xorSet = false;
			_alreadySet = false;
		}
	};
	
	typedef std::  list<Arg*>  ::iterator     ArgListIterator; // Typedef of an Arg list iterator.
	typedef std::vector<Arg*>  ::iterator   ArgVectorIterator; // Typedef of an Arg vector iterator.
	typedef std::list<Visitor*>::iterator VisitorListIterator; // Typedef of a Visitor list iterator.

	// Extract a value of type T from it's string representation contained in strVal. The ValueLike parameter used to select the correct
	// specialization of ExtractValue depending on the value traits of T. ValueLike traits use operator>> to assign the value from strVal.
	template<typename T>
	void ExtractValue(T &destVal, std::string const & strVal, ValueLike vl)
	{
		static_cast<void>(vl); // Avoid warning about unused vl
		std::istringstream is(strVal);

		int valuesRead = 0;
		while (is.good())
		{
			if (is.peek() != EOF)
			#ifdef HANDYARGS_SETBASE_ZERO
				is >> std::setbase(0) >> destVal;
			#else
				is >> destVal;
			#endif
			else
				break;

			valuesRead++;
		}

		if (is.fail())
			throw(ArgParseException("Couldn't read argument value from string '" + strVal + "'"));


		if (valuesRead > 1)
			throw(ArgParseException("More than one valid value parsed from string '" + strVal + "'"));

	}

	// Extract a value of type T from it's string representation contained in strVal. The ValueLike parameter used to select the correct
	// specialization of ExtractValue depending on the value traits of T. StringLike uses assignment (operator=) to assign from strVal.
	template<typename T> 
	void ExtractValue(T &destVal, std::string const & strVal, StringLike sl)
	{
		static_cast<void>(sl); // Avoid warning about unused sl
		SetString(destVal, strVal);
	}



	// A simple switch argument.  If the switch is set on the command line, then the getValue method will return the opposite of the default value for the switch.
	class SwitchArg : public Arg
	{
		// Checks to see if we've found the last match in a combined string.
		bool lastCombined(std::string& combinedSwitches)
		{
			for (unsigned int i = 1; i < combinedSwitches.length(); i++)
				if (combinedSwitches[i] != Arg::blankChar())
					return false;

			return true;
		}

		// Does the common processing of processArg.
		void commonProcessing()
		{
			if (_xorSet)
				throw(CmdLineParseException(
					"Mutually exclusive argument already set!", toString()));

			if (_alreadySet)
				throw(CmdLineParseException("Argument already set!", toString()));

			_alreadySet = true;

			if (_value == true)
				_value = false;
			else
				_value = true;

			_checkWithVisitor();
		}

	protected:

		// The value of the switch.
		bool _value;

		// Used to support the reset() method so that ValueArg can be reset to their constructed value.
		bool _default;

	public:

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param def - The default value for this Switch.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		SwitchArg(std::string const & flag,
				  std::string const & name,
				  std::string const & desc,
				  bool def = false,
				  Visitor* v = nullptr)
			: Arg(flag, name, desc, false, false, v)
			, _value(def)
			, _default(def)
		{ }


		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param def - The default value for this Switch.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		SwitchArg(std::string const & flag,
				  std::string const & name,
				  std::string const & desc,
				  CmdLineInterface& parser,
				  bool def = false,
				  Visitor* v = nullptr)
			: Arg(flag, name, desc, false, false, v)
			, _value(def)
			, _default(def)
		{
			parser.add(this);
		}



		// Handles the processing of the argument.
		// This re-implements the Arg version of this method to set the _value of the argument appropriately.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. Passed in from main().
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_ignoreable && Arg::ignoreRest())
				return false;

			if (argMatches(args[*i])) // if the whole string matches the flag or name string
			{
				commonProcessing();
				return true;
			}
			else if (combinedSwitchesMatch(args[*i])) // if a substring matches the flag as part of a combination
			{
				if (combinedSwitchesMatch(args[*i])) // check again to ensure we don't misinterpret this as a MultiSwitchArg 
					throw(CmdLineParseException("Argument already set!",
												toString()));

				commonProcessing();

				// We only want to return true if we've found the last combined match in the string, otherwise 
				// we return true so that other switches in the combination will have a chance to match.
				return lastCombined(args[*i]);
			}
			else
				return false;
		}

		// Checks a string to see if any of the chars in the string match the flag for this Switch.
		bool combinedSwitchesMatch(std::string& combinedSwitches)
		{
			
			if (combinedSwitches.length() > 0 && combinedSwitches[0] != Arg::flagStartString()[0]) // make sure this is actually a combined switch
				return false;
			if (combinedSwitches.substr(0, Arg::nameStartString().length()) == Arg::nameStartString()) // make sure it isn't a long name 
				return false;
			if (combinedSwitches.find_first_of(Arg::delimiter()) != std::string::npos) // make sure the delimiter isn't in the string 
				return false;

			// ok, we're not specifying a ValueArg, so we know that we have a combined switch list.  
			for (unsigned int i = 1; i < combinedSwitches.length(); i++)
				if (_flag.length() > 0 && combinedSwitches[i] == _flag[0] && _flag[0] != Arg::flagStartString()[0])
				{
					// update the combined switches so this one is no longer present
					// this is necessary so that no unlabeled args are matched later in the processing.
					//combinedSwitches.erase(i,1);
					combinedSwitches[i] = Arg::blankChar();
					return true;
				}

			return false; // none of the switches passed in the list match. 
		}

		// Returns bool, whether or not the switch has been set.
		bool getValue() const { return _value; }

		// A SwitchArg can be used as a boolean, indicating whether or not the switch has been set. This is the same as calling getValue()
		operator bool() const { return _value; }

		virtual void reset()
		{
			Arg::reset();
			_value = _default;
		}

	};

	// A multiple switch argument.  If the switch is set on the command line, then
	// the getValue method will return the number of times the switch appears.
	class MultiSwitchArg : public SwitchArg
	{
	protected:

		// The value of the switch.
		int _value;

		// Used to support the reset() method so that ValueArg can be reset to their constructed value.
		int _default;

	public:

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param init - Optional. The initial/default value of this Arg. Defaults to 0.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		MultiSwitchArg(std::string const & flag,
					   std::string const & name,
					   std::string const & desc,
					   int init = 0,
					   Visitor* v = nullptr)
			: SwitchArg(flag, name, desc, false, v)
			, _value(init)
			, _default(init)
		{ }

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param init - Optional. The initial/default value of this Arg. Defaults to 0.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		MultiSwitchArg(std::string const & flag,
					   std::string const & name,
					   std::string const & desc,
					   CmdLineInterface& parser,
					   int init = 0,
					   Visitor* v = nullptr)
			: SwitchArg(flag, name, desc, false, v)
			, _value(init)
			, _default(init)
		{
			parser.add(this);
		}


		// Handles the processing of the argument.
		// This re-implements the SwitchArg version of this method to set the _value of the argument appropriately.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. Passed in from main().
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_ignoreable && Arg::ignoreRest())
				return false;

			if (argMatches(args[*i]))
			{
				_alreadySet = true; // so the isSet() method will work
				++_value; // Matched argument: increment value.
				_checkWithVisitor();
				return true;
			}
			else if (combinedSwitchesMatch(args[*i]))
			{
				_alreadySet = true; // so the isSet() method will work
				++_value; // Matched argument: increment value.
				while (combinedSwitchesMatch(args[*i])) // Check for more in argument and increment value.
					++_value;
				_checkWithVisitor();
				return false;
			}
			else
				return false;
		}

		// Returns int, the number of times the switch has been set.
		int getValue() const { return _value; }

		// Returns the shortID for this Arg.
		std::string shortID(std::string const & val) const { return Arg::shortID(val) + " ... "; }

		// Returns the longID for this Arg.
		std::string longID(std::string const & val) const { return Arg::longID(val) + "  (accepted multiple times)"; }

		void reset() { MultiSwitchArg::_value = MultiSwitchArg::_default; }

	};

	// The interface that defines the interaction between the Arg and Constraint.
	template<class T>
	class Constraint
	{

	public:
		// Returns a description of the Constraint.
		virtual std::string description() const = 0;

		// Returns the short ID for the Constraint.
		virtual std::string shortID() const = 0;

		// The method used to verify that the value parsed from the command line meets the constraint.
		//   \param value - The value that will be checked.
		virtual bool check(const T& value) const = 0;

		// Silences warnings about Constraint being a base class with virtual functions but without a virtual destructor.
		virtual ~Constraint() { ; }

		static std::string shortID(Constraint<T> * constraint)
		{
			if (!constraint)
				throw std::logic_error("Cannot create a ValueArg with a nullptr constraint");
			return constraint->shortID();
		}
	};


	// The basic labeled argument that parses a value. This is a template class, which means the type T defines the type
	// that a given object will attempt to parse when the flag/name is matched on the command line.  While there is nothing stopping you from creating
	// an unflagged ValueArg, it is unwise and would cause significant problems. Instead use an UnlabeledValueArg.
	template<class T>
	class ValueArg : public Arg
	{
		COPY_ASSIGN_MOVE_CTOR(ValueArg, delete, delete, delete)

	protected:

		// The value parsed from the command line.
		// Can be of any type, as long as the >> operator for the type is defined.
		T _value;

		// Used to support the reset() method so that ValueArg can be reset to their constructed value.
		T _default;

		// A human readable description of the type to be parsed. This is a hack, plain and simple.  Ideally we would use RTTI to
		// return the name of type T, but until there is some sort of consistent support for human readable names, we are left to our own devices.
		std::string _typeDesc;

		// A Constraint this Arg must conform to.
		Constraint<T> * _constraint = nullptr;

		// Extracts the value from the string. Attempts to parse string as type T, if this fails an exception is thrown.
		//   \param val - value to be parsed.
		void _extractValue(std::string const & val)
		{
			try 
			{
				ExtractValue(_value, val, typename ArgTraits<T>::ValueCategory());
			}
			catch (ArgParseException &e)
			{
				throw ArgParseException(e.error(), toString());
			}

			if (_constraint &&!_constraint->check(_value))
				throw(CmdLineParseException("Value '" + val +"' does not meet constraint: " + _constraint->description(), toString()));
	}

	public:

		// Labeled ValueArg constructor.
		// You could conceivably call this constructor with a blank flag, but that would make you a bad person. It would also cause
		// an exception to be thrown. If you want an unlabeled argument, use the other constructor.
		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects. This is used in the generation
		//                     of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		ValueArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 T value,
				 std::string const & typeDesc,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _value(value)
			, _default(value)
			, _typeDesc(typeDesc)
		{ }

		// Labeled ValueArg constructor.
		// You could conceivably call this constructor with a blank flag, but that would make you a bad person.  It would also cause
		// an exception to be thrown. If you want an unlabeled argument, use the other constructor.
		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument. Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects. This is used in the generation
		//                     of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		ValueArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 T value,
				 std::string const & typeDesc,
				 CmdLineInterface & parser,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _value(value)
			, _default(value)
			, _typeDesc(typeDesc)
		{
			parser.add(this);
		}

		// Labeled ValueArg constructor.
		// You could conceivably call this constructor with a blank flag, but that would make you a bad person.  It would also cause
		// an exception to be thrown. If you want an unlabeled argument, use the other constructor.
		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument. Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param parser - A CmdLine parser object to add this Arg to.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		ValueArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 T value,
				 Constraint<T> * constraint,
				 CmdLineInterface & parser,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _value(value)
			, _default(value)
			, _typeDesc(Constraint<T>::shortID(constraint))  // TODO(macbishop): Will crash if constraint is nullptr
			, _constraint(constraint)
		{
			parser.add(this);
		}

		// Labeled ValueArg constructor.
		// You could conceivably call this constructor with a blank flag, but that would make you a bad person.  It would also cause
		// an exception to be thrown. If you want an unlabeled argument, use the other constructor.
		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument. Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		ValueArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 T value,
				 Constraint<T>* constraint,
				 Visitor* v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _value(value)
			, _default(value)
			, _typeDesc(Constraint<T>::shortID(constraint))
			, _constraint(constraint)
		{ }

		// Handles the processing of the argument.
		// This re-implements the Arg version of this method to set the _value of the argument appropriately.  
		// It knows the difference between labeled and unlabeled.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. Passed in from main().
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_ignoreable && Arg::ignoreRest())
				return false;

			if (_hasBlanks(args[*i]))
				return false;

			std::string flag = args[*i];

			std::string value = "";
			trimFlag(flag, value);

			if (argMatches(flag))
			{
				if (_alreadySet)
				{
					if (_xorSet)
						throw(CmdLineParseException("Mutually exclusive argument already set!", toString()));
					else
						throw(CmdLineParseException("Argument already set!", toString()));
				}

				if (Arg::delimiter() != ' ' && value == "")
					throw(ArgParseException("Couldn't find delimiter for this argument!", toString()));

				if (value == "")
				{
					(*i)++;
					if (static_cast<unsigned int>(*i) < args.size())
						_extractValue(args[*i]);
					else
						throw(ArgParseException("Missing a value for this argument!", toString()));
				}
				else
					_extractValue(value);

				_alreadySet = true;
				_checkWithVisitor();
				return true;
			}
			else
				return false;
		}

		// Returns the value of the argument.
		const T& getValue() const { return _value; }

		// TODO(macbishop): Non-const variant is deprecated, don't use. Remove in next major.
		//T & getValue() { return _value; }

		// A ValueArg can be used as as its value type (T) This is the same as calling getValue()
		operator const T&() const { return getValue(); }

		// Specialization of shortID.
		//   \param val - value to be used.
		virtual std::string shortID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return Arg::shortID(_typeDesc);
		}

		// Specialization of longID.
		//   \param val - value to be used.
		virtual std::string longID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return Arg::longID(_typeDesc);
		}

		virtual void reset()
		{
			Arg::reset();
			_value = _default;
		}
	};

	class OptionalUnlabeledTracker
	{

	public:
		static void check(bool req, std::string const & argName)
		{
			if (OptionalUnlabeledTracker::alreadyOptional())
				throw(SpecificationException(
					"You can't specify ANY Unlabeled Arg following an optional Unlabeled Arg",
					argName));

			if (!req)
				OptionalUnlabeledTracker::gotOptional();
		}

		static void gotOptional() { alreadyOptionalRef() = true; }

		static bool& alreadyOptional() { return alreadyOptionalRef(); }

	private:

		static bool& alreadyOptionalRef() { static bool ct = false; return ct; }
	};


	// The basic unlabeled argument that parses a value. This is a template class, which means the type T defines the type
	// that a given object will attempt to parse when an UnlabeledValueArg is reached in the list of args that the CmdLine 
	// iterates over.
	template<class T>
	class UnlabeledValueArg : public ValueArg<T>
	{

		// If compiler has two stage name lookup (as gcc >= 3.4 does) this is requried to prevent undef. symbols
		using ValueArg<T>::_ignoreable;
		using ValueArg<T>::_hasBlanks;
		using ValueArg<T>::_extractValue;
		using ValueArg<T>::_typeDesc;
		using ValueArg<T>::_name;
		using ValueArg<T>::_description;
		using ValueArg<T>::_alreadySet;
		using ValueArg<T>::toString;

	public:

		//   \param name - A one word name for the argument.  Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects. This is used in the generation
		//                     of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param ignoreable - Allows you to specify that this argument can be ignored if the '--' flag is set.
		//                       This defaults to false (cannot be ignored) and should generally stay that way unless you have some 
		//                       special need for certain arguments to be ignored.
		//   \param v - Optional Vistor.  You should leave this blank unless you have a very good reason.
		UnlabeledValueArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  T value,
						  std::string const & typeDesc,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: ValueArg<T>("", name, desc, req, value, typeDesc, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(req, toString());
		}


		//   \param name - A one word name for the argument.  Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects. This is used in the generation
		//                     of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param ignoreable - Allows you to specify that this argument can be ignored if the '--' flag is set.
		//                     This defaults to false (cannot be ignored) and should generally stay that way unless you have some 
		//                     special need for certain arguments to be ignored.
		//   \param v - Optional Vistor.  You should leave this blank unless you have a very good reason.
		UnlabeledValueArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  T value,
						  std::string const & typeDesc,
						  CmdLineInterface& parser,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: ValueArg<T>("", name, desc, req, value, typeDesc, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(req, toString());
			parser.add(this);
		}


		//   \param name - A one word name for the argument.  Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param ignoreable - Allows you to specify that this argument can be ignored if the '--' flag is set.
		//                       This defaults to false (cannot be ignored) and should generally stay that way unless you have some 
		//                       special need for certain arguments to be ignored.
		//   \param v - Optional Vistor.  You should leave this blank unless you have a very good reason.
		UnlabeledValueArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  T value,
						  Constraint<T>* constraint,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: ValueArg<T>("", name, desc, req, value, constraint, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(req, toString());
		}

		//   \param name - A one word name for the argument.  Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param value - The default value assigned to this argument if it is not present on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param ignoreable - Allows you to specify that this argument can be ignored if the '--' flag is set.
		//                       This defaults to false (cannot be ignored) and should generally stay that way unless you have some 
		//                       special need for certain arguments to be ignored.
		//   \param v - Optional Vistor.  You should leave this blank unless you have a very good reason.
		UnlabeledValueArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  T value,
						  Constraint<T>* constraint,
						  CmdLineInterface& parser,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: ValueArg<T>("", name, desc, req, value, constraint, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(req, toString());
			parser.add(this);
		}


		// Handles the processing of the argument.
		// This re-implements the Arg version of this method to set the _value of the argument appropriately.  Handling specific 
		// to unlabled arguments.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings.
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_alreadySet)
				return false;

			if (_hasBlanks(args[*i]))
				return false;

			// never ignore an unlabeled arg

			_extractValue(args[*i]);
			_alreadySet = true;
			return true;
		}

		// Overrides shortID for specific behavior.
		virtual std::string shortID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return std::string("<") + _typeDesc + ">";
		}

		// Overrides longID for specific behavior.
		virtual std::string longID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn

									// Ideally we would like to be able to use RTTI to return the name
									// of the type required for this argument.  However, g++ at least, 
									// doesn't appear to return terribly useful "names" of the types.  
			return std::string("<") + _typeDesc + ">";
		}

		// Overrides operator== for specific behavior.
		virtual bool operator==(const Arg& a) const
		{
			return (_name == a.getName() || _description == a.getDescription());
		}

		// Instead of pushing to the front of list, push to the back.
		//   \param argList - The list to add this to.
		virtual void addToList(std::list<Arg*>& argList) const
		{
			argList.push_back(const_cast<Arg*>(static_cast<const Arg* const>(this)));
		}
	};


	// An argument that allows multiple values of type T to be specified.  Very similar to a ValueArg, except a vector of 
	// values will be returned instead of just one.
	template<class T>
	class MultiArg : public Arg
	{
		COPY_ASSIGN_MOVE_CTOR(MultiArg, delete, delete, delete)
	public:
		typedef std::vector<T> container_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::const_iterator const_iterator;

	protected:

		// The list of values parsed from the CmdLine.
		std::vector<T> _values;

		// The description of type T to be used in the usage.
		std::string _typeDesc;

		// A list of constraint on this Arg.
		Constraint<T> * _constraint = nullptr;

		// Used by XorHandler to decide whether to keep parsing for this arg.
		bool _allowMore = false;

		// Extracts the value from the string. Attempts to parse string as type T, if this fails an exception is thrown.
		//   \param val - The string to be read.
		void _extractValue(std::string const & val)
		{
			try 
			{
				T tmp;
				ExtractValue(tmp, val, typename ArgTraits<T>::ValueCategory());
				_values.push_back(tmp);
			}
			catch (ArgParseException &e) 
			{
				throw ArgParseException(e.error(), toString());
			}

			if (_constraint && !_constraint->check(_values.back()))
				throw (CmdLineParseException("Value '" + val + "' does not meet constraint: " + _constraint->description(), toString()));
		}

	public:

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects.  This is used in the 
		//                     generation of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.

		MultiArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 std::string const & typeDesc,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _typeDesc(typeDesc)
		{
			_acceptsMultipleValues = true;
		}

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects.  This is used in the 
		//                     generation of the USAGE statement.  The goal is to be helpful to the end user of the program.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		MultiArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 std::string const & typeDesc,
				 CmdLineInterface& parser,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _typeDesc(typeDesc)
		{
			parser.add(this);
			_acceptsMultipleValues = true;
		}

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		MultiArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 Constraint<T>* constraint,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _typeDesc(Constraint<T>::shortID(constraint))
			, _constraint(constraint)
		{
			_acceptsMultipleValues = true;
		}

		//   \param flag - The one character flag that identifies this argument on the command line.
		//   \param name - A one word name for the argument.  Can be used as a long flag on the command line.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		MultiArg(std::string const & flag,
				 std::string const & name,
				 std::string const & desc,
				 bool req,
				 Constraint<T>* constraint,
				 CmdLineInterface& parser,
				 Visitor * v = nullptr)
			: Arg(flag, name, desc, req, true, v)
			, _typeDesc(Constraint<T>::shortID(constraint))
			, _constraint(constraint)
		{
			parser.add(this);
			_acceptsMultipleValues = true;
		}


		// Handles the processing of the argument. This re-implements the Arg version of this method to set the _value 
		// of the argument appropriately.  It knows the difference between labeled and unlabeled.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. Passed from main().
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_ignoreable && Arg::ignoreRest())
				return false;

			if (_hasBlanks(args[*i]))
				return false;

			std::string flag = args[*i];
			std::string value = "";

			trimFlag(flag, value);

			if (argMatches(flag))
			{
				if (Arg::delimiter() != ' ' && value == "")
					throw(ArgParseException("Couldn't find delimiter for this argument!", toString()));

				// always take the first one, regardless of start string
				if (value == "")
				{
					(*i)++;
					if (static_cast<unsigned int>(*i) < args.size())
						_extractValue(args[*i]);
					else
						throw(ArgParseException("Missing a value for this argument!", toString()));
				}
				else
					_extractValue(value);

				// continuing taking the args until we hit one with a start string
				// while ( (unsigned int)(*i)+1 < args.size() &&
				// args[(*i)+1].find_first_of( Arg::flagStartString() ) != 0 &&
				// args[(*i)+1].find_first_of( Arg::nameStartString() ) != 0 )
				// _extractValue( args[++(*i)] );

				_alreadySet = true;
				_checkWithVisitor();

				return true;
			}
			else
				return false;
		}

		// Returns a vector of type T containing the values parsed from the command line.
		const std::vector<T>& getValue() const { return _values; }

		// Returns an iterator over the values parsed from the command line.
		const_iterator begin() const { return _values.begin(); }

		// Returns the end of the values parsed from the command line.
		const_iterator end() const { return _values.end(); }

		// Returns the a short id string.  Used in the usage.
		//   \param val - value to be used.
		virtual std::string shortID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return Arg::shortID(_typeDesc) + " ... ";
		}

		// Returns the a long id string.  Used in the usage.
		//   \param val - value to be used.
		virtual std::string longID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return Arg::longID(_typeDesc) + "  (accepted multiple times)";
		}

		// Once we've matched the first value, then the arg is no longer required.
		virtual bool isRequired() const
		{
			if (_required)
			{
				if (_values.size() > 1)
					return false;
				else
					return true;
			}
			else
				return false;

		}

		virtual bool allowMore()
		{
			bool am = _allowMore;
			_allowMore = true;
			return am;
		}

		virtual void reset()
		{
			Arg::reset();
			_values.clear();
		}
	};

	// Just like a MultiArg, except that the arguments are unlabeled.  Basically, this Arg will 
	// slurp up everything that hasn't been matched to another Arg.
	template<class T>
	class UnlabeledMultiArg : public MultiArg<T>
	{

		// If compiler has two stage name lookup (as gcc >= 3.4 does) this is requried to prevent undef. symbols
		using MultiArg<T>::_ignoreable;
		using MultiArg<T>::_hasBlanks;
		using MultiArg<T>::_extractValue;
		using MultiArg<T>::_typeDesc;
		using MultiArg<T>::_name;
		using MultiArg<T>::_description;
		using MultiArg<T>::_alreadySet;
		using MultiArg<T>::toString;

	public:

		//   \param name - The name of the Arg. Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects.  This 
		//                   is used in the generation of the USAGE statement.  The goal is to be helpful to 
		//                   the end user of the program.
		//   \param ignoreable - Whether or not this argument can be ignored using the "--" flag.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		UnlabeledMultiArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  std::string const & typeDesc,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: MultiArg<T>("", name, desc, req, typeDesc, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(true, toString());
		}
			
		//   \param name - The name of the Arg. Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param typeDesc - A short, human readable description of the type that this object expects.  This 
		//                   is used in the generation of the USAGE statement.  The goal is to be helpful to 
		//                   the end user of the program.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param ignoreable - Whether or not this argument can be ignored using the "--" flag.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		UnlabeledMultiArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  std::string const & typeDesc,
						  CmdLineInterface& parser,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: MultiArg<T>("", name, desc, req, typeDesc, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(true, toString());
			parser.add(this);
		}

		//   \param name - The name of the Arg. Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param ignoreable - Whether or not this argument can be ignored using the "--" flag.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		UnlabeledMultiArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  Constraint<T>* constraint,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: MultiArg<T>("", name, desc, req, constraint, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(true, toString());
		}

		//   \param name - The name of the Arg. Note that this is used for identification, not as a long flag.
		//   \param desc - A description of what the argument is for or does.
		//   \param req - Whether the argument is required on the command line.
		//   \param constraint - A pointer to a Constraint object used to constrain this Arg.
		//   \param parser - A CmdLine parser object to add this Arg to
		//   \param ignoreable - Whether or not this argument can be ignored using the "--" flag.
		//   \param v - An optional visitor.  You probably should not use this unless you have a very good reason.
		UnlabeledMultiArg(std::string const & name,
						  std::string const & desc,
						  bool req,
						  Constraint<T>* constraint,
						  CmdLineInterface& parser,
						  bool ignoreable = false,
						  Visitor * v = nullptr)
			: MultiArg<T>("", name, desc, req, constraint, v)
		{
			_ignoreable = ignoreable;
			OptionalUnlabeledTracker::check(true, toString());
			parser.add(this);
		}

		// Handles the processing of the argument. This re-implements the Arg version of this method to set the
		// _value of the argument appropriately.  It knows the difference between labeled and unlabeled.
		//   \param i - Pointer the the current argument in the list.
		//   \param args - Mutable list of strings. Passed from main().
		virtual bool processArg(int* i, std::vector<std::string>& args)
		{
			if (_hasBlanks(args[*i]))
				return false;

			// never ignore an unlabeled multi arg

			// always take the first value, regardless of the start string 
			_extractValue(args[(*i)]);

			// continue taking args until we hit the end or a start string
			// while ( (unsigned int)(*i)+1 < args.size() &&
			// args[(*i)+1].find_first_of( Arg::flagStartString() ) != 0 &&
			// args[(*i)+1].find_first_of( Arg::nameStartString() ) != 0 )
			// _extractValue( args[++(*i)] );

			_alreadySet = true;
			return true;
		}

		// Returns the a short id string.  Used in the usage.
		//   \param val - value to be used.
		virtual std::string shortID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return std::string("<") + _typeDesc + "> ...";
		}

		// Returns the a long id string.  Used in the usage.
		//   \param val - value to be used.
		virtual std::string longID(std::string const & val = "val") const
		{
			static_cast<void>(val); // Ignore input, don't warn
			return std::string("<") + _typeDesc + ">  (accepted multiple times)";
		}

		// Opertor ==.
		//   \param a - The Arg to be compared to this.
		virtual bool operator==(const Arg& a) const { return _name == a.getName() || _description == a.getDescription(); }

		// Pushes this to back of list rather than front.
		//   \param argList - The list this should be added to.
		virtual void addToList(std::list<Arg *> & argList) const { argList.push_back(const_cast<Arg*>(static_cast<const Arg* const>(this))); }
	};

	// This class handles lists of Arg's that are to be XOR'd on the command line.  
	// This is used by CmdLine and you shouldn't ever use it.
	class XorHandler
	{
	protected:
		// The list of of lists of Arg's to be or'd together.
		std::vector< std::vector<Arg*> > _orList;

	public:
		XorHandler() = default;

		// Add a list of Arg*'s that will be orred together.
		//   \param ors - list of Arg* that will be xor'd.
		void add(std::vector<Arg *> & ors) { _orList.push_back(ors); }

		// Checks whether the specified Arg is in one of the xor lists and if it does match one, returns the size of the xor list 
		// that the Arg matched.  If the Arg matches, then it also sets the rest of the Arg's in the list. You shouldn't use this.
		//   \param a - The Arg to be checked.
		int check(const Arg* a)
		{
			for (int i = 0; static_cast<unsigned int>(i) < _orList.size(); i++) // iterate over each XOR list
			{
				ArgVectorIterator ait = std::find(_orList[i].begin(), _orList[i].end(), a);
				if (ait != _orList[i].end()) // if the XOR list contains the arg..
				{
					// first check to see if a mutually exclusive switch has not already been set
					for (ArgVectorIterator it = _orList[i].begin(); it != _orList[i].end(); it++)
						if (a != (*it) && (*it)->isSet())
							throw(CmdLineParseException(
								"Mutually exclusive argument already set!",
								(*it)->toString()));

					// go through and set each arg that is not a
					for (ArgVectorIterator it = _orList[i].begin(); it != _orList[i].end(); it++)
						if (a != (*it))
							(*it)->xorSet();

					// return the number of required args that have now been set
					if ((*ait)->allowMore())
						return 0;
					else
						return static_cast<int>(_orList[i].size());
				}
			}

			if (a->isRequired())
				return 1;
			else
				return 0;
		}

		/// Looks like this was never implemented
		//// Returns the XOR specific short usage.
		//std::string shortUsage();

		/// Looks like this was never implemented
		//// Prints the XOR specific long usage.
		////   \param os - Stream to print to.
		//void printLongUsage(std::ostream& os);

		// Simply checks whether the Arg is contained in one of the arg lists.
		//   \param a - The Arg to be checked.
		bool contains(const Arg* a)
		{
			for (int i = 0; static_cast<unsigned int>(i) < _orList.size(); i++)
				for (ArgVectorIterator it = _orList[i].begin(); it != _orList[i].end(); it++)
					if (a == (*it))
						return true;
			return false;
		}

		std::vector< std::vector<Arg*> >& getXorList();
	};

	inline std::vector< std::vector<Arg*> >& XorHandler::getXorList() { return _orList; }

	class CmdLineInterface;
	class ArgException;

	// The interface that any output object must implement.
	class CmdLineOutput
	{
	public:
		virtual ~CmdLineOutput() {}

		// Generates some sort of output for the USAGE.
		//   \param c - The CmdLine object the output is generated for.
		virtual void usage(CmdLineInterface& c) = 0;

		// Generates some sort of output for the version.
		//   \param c - The CmdLine object the output is generated for.
		virtual void version(CmdLineInterface& c) = 0;

		// Generates some sort of output for a failure.
		//   \param c - The CmdLine object the output is generated for.
		//   \param e - The ArgException that caused the failure.
		virtual void failure(CmdLineInterface & c, ArgException & e) = 0;
	};


	// A Visitor object that calls the usage method of the given CmdLineOutput object for the specified CmdLine object.
	class HelpVisitor : public Visitor
	{
		COPY_ASSIGN_MOVE_CTOR(HelpVisitor, delete, delete, delete)

	protected:

		// The CmdLine the output will be generated for.
		CmdLineInterface* _cmd;
		
		// The output object.
		CmdLineOutput** _out;

	public:
		//   \param cmd - The CmdLine the output will be generated for.
		//   \param out - The type of output.
		HelpVisitor(CmdLineInterface* cmd, CmdLineOutput** out)
			: Visitor(), _cmd(cmd), _out(out) { }

		// Calls the usage method of the CmdLineOutput for the specified CmdLine.
		void visit() { (*_out)->usage(*_cmd); throw ExitException(0); }
	};


	// A Vistor that will call the version method of the given CmdLineOutput for the specified CmdLine object and then exit.
	class VersionVisitor : public Visitor
	{
		COPY_ASSIGN_MOVE_CTOR(VersionVisitor, delete, delete, delete)

	protected:
		// The CmdLine of interest.
		CmdLineInterface* _cmd;

		// The output object.
		CmdLineOutput** _out;

	public:
		//   \param cmd - The CmdLine the output is generated for.
		//   \param out - The type of output.
		VersionVisitor(CmdLineInterface* cmd, CmdLineOutput** out)
			: Visitor(), _cmd(cmd), _out(out) { }

		// Calls the version method of the output object using the specified CmdLine.
		void visit()
		{
			(*_out)->version(*_cmd);
			throw ExitException(0);
		}

	};


	// A Vistor that tells the CmdLine to begin ignoring arguments after this one is parsed.
	class IgnoreRestVisitor : public Visitor
	{
	public:
		IgnoreRestVisitor() : Visitor() {}

		// Sets Arg::_ignoreRest.
		void visit() { Arg::beginIgnoring(); }
	};

	// A class that isolates any output from the CmdLine object so that it may be easily modified.
	class StdOutput : public CmdLineOutput
	{
	public:
		// Prints the usage to stdout.  Can be overridden to produce alternative behavior.
		//   \param c - The CmdLine object the output is generated for.
		virtual void usage(CmdLineInterface & c)
		{
			std::string message = c.getMessage();
			std::string xversion = c.getVersion();

			std::cout << std::endl;
			spacePrint(std::cout, message, 75, 0, 0);
			std::cout << "Version: " << xversion << std::endl;
			std::cout << std::endl << "USAGE: " << std::endl << std::endl;
			_shortUsage(c, std::cout);
			std::cout << std::endl << /*std::endl <<*/ "Where: " << std::endl << std::endl;
			_longUsage(c, std::cout);
			std::cout << std::endl;
		}

		// Prints the version to stdout. Can be overridden to produce alternative behavior.
		//   \param c - The CmdLine object the output is generated for.
		virtual void version(CmdLineInterface & c)
		{
			std::string message = c.getMessage();
			//std::string progName = c.getProgramName();
			std::string xversion = c.getVersion();

			std::cout << std::endl;
			spacePrint(std::cout, message, 75, 0, 0);
			std::cout << "Version: " << xversion << std::endl << std::endl;
			//std::cout << "Executable: " << progName << std::endl << std::endl;
		}

		// Prints (to stderr) an error message, short usage. Can be overridden to produce alternative behavior.
		//   \param c - The CmdLine object the output is generated for.
		//   \param e - The ArgException that caused the failure.
		virtual void failure(CmdLineInterface & c, ArgException & e)
		{
			std::string progName = c.getProgramName();

			std::cerr
				<< "PARSE ERROR: " << e.argId() << std::endl
				<< "             " << e.error() << std::endl << std::endl;

			if (c.hasHelpAndVersion())
			{
				std::cerr << "Brief USAGE: " << std::endl;

				_shortUsage(c, std::cerr);

				std::cerr
					<< std::endl << "For complete USAGE and HELP type: "
					<< std::endl << "   " << progName << " "
					<< Arg::nameStartString() << "help"
					<< std::endl << std::endl;
			}
			else
				usage(c);

			throw ExitException(1);
		}

	protected:
		// Writes a brief usage message with short args.
		//   \param c - The CmdLine object the output is generated for.
		//   \param os - The stream to write the message to.
		void _shortUsage(CmdLineInterface& c, std::ostream& os) const
		{
			std::list<Arg *> argList = c.getArgList();
			std::string progName = c.getProgramName();
			XorHandler xorHandler = c.getXorHandler();
			std::vector< std::vector<Arg*> > xorList = xorHandler.getXorList();

			std::string s = progName + " ";

			// first the xor
			for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++)
			{
				s += " {";
				for (ArgVectorIterator it = xorList[i].begin();
					 it != xorList[i].end(); it++)
					s += (*it)->shortID() + "|";

				s[s.length() - 1] = '}';
			}

			// then the rest
			for (ArgListIterator it = argList.begin(); it != argList.end(); it++)
				if (!xorHandler.contains((*it)))
					s += " " + (*it)->shortID();

			// if the program name is too long, then adjust the second line offset 
			int secondLineOffset = static_cast<int>(progName.length()) + 2;
			if (secondLineOffset > 75 / 2)
				secondLineOffset = static_cast<int>(75 / 2);

			spacePrint(os, s, 75, 4, secondLineOffset);
		}


		// Writes a longer usage message with long and short args, provides descriptions and prints message.
		//   \param c - The CmdLine object the output is generated for.
		//   \param os - The stream to write the message to.
		void _longUsage(CmdLineInterface& c, std::ostream& os) const
		{
			std::list<Arg *> argList = c.getArgList();
			//std::string      message = c.getMessage();
			XorHandler    xorHandler = c.getXorHandler();
			std::vector<std::vector<Arg*>> xorList = xorHandler.getXorList();

			// first the xor 
			for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++)
			{
				for (ArgVectorIterator it = xorList[i].begin();
					 it != xorList[i].end();
					 it++)
				{
					spacePrint(os, (*it)->longID(), 75, 4, 0);
					spacePrint(os, (*it)->getDescription(), 75, 8, 0);

					if (it + 1 != xorList[i].end())
						spacePrint(os, "-- OR --", 75, 9, 0);
				}
				os << std::endl << std::endl;
			}

			// then the rest
			for (ArgListIterator it = argList.begin(); it != argList.end(); it++)
				if (!xorHandler.contains((*it)))
				{
					spacePrint(os, (*it)->longID(), 75, 4, 0);
					spacePrint(os, (*it)->getDescription(), 75, 8, 0);
					os << std::endl;
				}

			os << std::endl;
		}

		// This function inserts line breaks and indents long strings according the  params input. 
		// It will only break lines at spaces, commas and pipes.
		//   \param os - The stream to be printed to.
		//   \param s - The string to be printed.
		//   \param maxWidth - The maxWidth allowed for the output line.
		//   \param indentSpaces - The number of spaces to indent the first line.
		//   \param secondLineOffset - The number of spaces to indent the second and all subsequent lines in addition to indentSpaces.
		void spacePrint(std::ostream& os,
						std::string const & s,
						int maxWidth,
						int indentSpaces,
						int secondLineOffset) const
		{
			int len = static_cast<int>(s.length());

			if ((len + indentSpaces > maxWidth) && maxWidth > 0)
			{
				int allowedLen = maxWidth - indentSpaces;
				int start = 0;
				while (start < len)
				{
					// find the substring length
					// int stringLen = std::min<int>( len - start, allowedLen );
					// doing it this way to support a VisualC++ 2005 bug 
					int stringLen = std::min(len - start, allowedLen);

					// trim the length so it doesn't end in middle of a word
					if (stringLen == allowedLen)
						while (stringLen >= 0 &&
							   s[stringLen + start] != ' ' &&
							   s[stringLen + start] != ',' &&
							   s[stringLen + start] != '|')
							stringLen--;

					// ok, the word is longer than the line, so just split wherever the line ends
					if (stringLen <= 0)
						stringLen = allowedLen;

					// check for newlines
					for (int i = 0; i < stringLen; i++)
						if (s[start + i] == '\n')
							stringLen = i;

					// print the indent	
					for (int i = 0; i < indentSpaces; i++)
						os << " ";

					if (start == 0)
					{
						// handle second line offsets
						indentSpaces += secondLineOffset;

						// adjust allowed len
						allowedLen -= secondLineOffset;
					}

					os << s.substr(start, stringLen) << std::endl;

					// so we don't start a line with a space
					while (IsWhitespace(s[stringLen + start]) && (s[stringLen + start] != '\t') && start < len)
						start++;

					start += stringLen;
				}
			}
			else
			{
				for (int i = 0; i < indentSpaces; i++)
					os << " ";
				os << s << std::endl;
			}
		}

	};

	// A Constraint that constrains the Arg to only those values specified in the constraint.
	template<class T>
	class ValuesConstraint : public Constraint<T>
	{
	protected:
		// The list of valid values.
		std::vector<T> _allowed;

		// The string used to describe the allowed values of this constraint.
		std::string _typeDesc = "";
	public:
		//   \param allowed - vector of allowed values.
		ValuesConstraint(std::vector<T> & allowed)
			: _allowed(allowed)
		{
			for (unsigned int i = 0; i < _allowed.size(); i++)
			{

				std::ostringstream os;
				os << _allowed[i];
				std::string temp(os.str());
				if (i > 0)
					_typeDesc += "|";
				_typeDesc += temp;
			}
		}

		virtual ~ValuesConstraint() {}

		// Returns a description of the Constraint.
		virtual std::string description() const { return _typeDesc; }

		// Returns the short ID for the Constraint.
		virtual std::string shortID() const { return _typeDesc; }

		// The method used to verify that the value parsed from the command line meets the constraint.
		//   \param value - The value that will be checked.
		virtual bool check(const T& value) const { return !(std::find(_allowed.begin(), _allowed.end(), value) == _allowed.end()); }
	};

	template<typename T> void DelPtr(T ptr) { delete ptr; }
	template<typename C> void ClearContainer(C &c)
	{
		typedef typename C::value_type value_type;
		std::for_each(c.begin(), c.end(), DelPtr<value_type>);
		c.clear();
	}


	// The base class that manages the command line definition and passes
	// along the parsing to the appropriate Arg classes.
	class CmdLine : public CmdLineInterface
	{
		// Prevent accidental copying.
		COPY_ASSIGN_MOVE_CTOR(CmdLine, delete, delete, delete)

		// Encapsulates the code common to the constructors (which is all of it).
		void _constructor();

		// Is set to true when a user sets the output object. We use this so that we don't delete objects that are created outside of this lib.
		bool _userSetOutput = false;

		// Whether or not to automatically create help and version switches.
		bool _helpAndVersion;

		// Whether or not to ignore unmatched args.
		bool _ignoreUnmatched = false;

	protected:
		std::list<Arg*> _argList;  // The list of arguments that will be tested against the command line.
		std::string     _progName = "not_set_yet"; // The name of the program.  Set to argv[0].
		std::string     _message;  // A message used to describe the program.  Used in the usage output.
		std::string     _version;  // The version to be displayed with the --version switch.

		// The number of arguments that are required to be present on the command line. This is set 
		// dynamically, based on the Args added to the CmdLine object.
		int _numRequired = 0;

		char _delimiter; // The character that is used to separate the argument flag/name from the value.  Defaults to ' ' (space).

		XorHandler _xorHandler; // The handler that manages xoring lists of args.

		std::list<Arg*> _argDeleteOnExitList; // A list of Args to be explicitly deleted when the destructor is called.  ATM, this only includes the three default Args. 

		// A list of Visitors to be explicitly deleted when the destructor is called.  
		// At the moment, these are the Vistors created for the default Args.
		std::list<Visitor*> _visitorDeleteOnExitList;

		CmdLineOutput* _output = 0; // Object that handles all output for the CmdLine.
		bool _handleExceptions = true; // Should CmdLine handle parsing exceptions internally?

		// Throws an exception listing the missing args.
		void missingArgsException();

		// Checks whether a name/flag string matches entirely matches the Arg::blankChar.
		// Used when multiple switches are combined into a single argument.
		//   \param s - The message to be used in the usage.
		bool _emptyCombined(std::string const & s);

		// Perform a delete ptr; operation on ptr when this object is deleted.
		void deleteOnExit(Arg* ptr) { _argDeleteOnExitList.push_back(ptr); }

		// Perform a delete ptr; operation on ptr when this object is deleted.
		void deleteOnExit(Visitor* ptr) { _visitorDeleteOnExitList.push_back(ptr); }

	public:

		// Command line constructor. Defines how the arguments will be parsed.
		//   \param message - The message to be used in the usage output.
		//   \param delimiter - The character that is used to separate the argument flag/name from the value.  Defaults to ' ' (space).
		//   \param version - The version number to be used in the --version switch.
		//   \param helpAndVersion - Whether or not to create the Help and Version switches. Defaults to true.
		CmdLine(std::string const & message,
				const char delimiter = ' ',
				std::string const & version = "none",
				bool helpAndVersion = true)
			: _message(message)
			, _version(version)
			, _delimiter(delimiter)
			, _helpAndVersion(helpAndVersion)
		{
			_constructor();
		}

		virtual ~CmdLine(); //Deletes any resources allocated by a CmdLine object.

		// Adds an argument to the list of arguments to be parsed.
		//   \param a - Argument to be added.
		void add(Arg& a) { add(&a); }

		// An alternative add.  Functionally identical.
		//   \param a - Argument to be added.
		void add(Arg* a);

		// Add two Args that will be xor'd.  If this method is used, add does
		// not need to be called.
		//   \param a - Argument to be added and xor'd.
		//   \param b - Argument to be added and xor'd.
		void xorAdd(Arg& a, Arg& b);

		// Add a list of Args that will be xor'd.  If this method is used, add does not need to be called.
		//   \param xors - List of Args to be added and xor'd.
		void xorAdd(std::vector<Arg*>& xors);

		// Parses the command line.
		//   \param argc - Number of arguments.
		//   \param argv - Array of arguments.
		void parse(int argc, const char * const * argv);

		// Parses the command line.
		//   \param args - A vector of strings representing the args. args[0] is still the program name.
		void parse(std::vector<std::string>& args);

		CmdLineOutput * getOutput() { return _output; }
		void setOutput(CmdLineOutput * co);
		std::string & getVersion() { return _version; }
		std::string & getProgramName() { return _progName; }
		std::list<Arg*> & getArgList() { return _argList; }
		XorHandler & getXorHandler() { return _xorHandler; }
		char getDelimiter() { return _delimiter; }
		std::string & getMessage() { return _message; }
		bool hasHelpAndVersion() { return _helpAndVersion; }

		// Disables or enables CmdLine's internal parsing exception handling.
		// @param state Should CmdLine handle parsing exceptions internally?
		void setExceptionHandling(const bool state) { _handleExceptions = state; }

		// Returns the current state of the internal exception handling.
		// @retval true Parsing exceptions are handled internally.
		// @retval false Parsing exceptions are propagated to the caller.
		bool getExceptionHandling() const { return _handleExceptions; }

		// Allows the CmdLine object to be reused.
		void reset();

		// Allows unmatched args to be ignored. By default false.
		// @param ignore If true the cmdline will ignore any unmatched args and if false it will behave as normal.
		void ignoreUnmatched(const bool ignore) { _ignoreUnmatched = ignore; }
	};

	inline CmdLine::~CmdLine()
	{
		ClearContainer(_argDeleteOnExitList);
		ClearContainer(_visitorDeleteOnExitList);

		if (!_userSetOutput) 
		{
			delete _output;
			_output = 0;
		}
	}

	inline void CmdLine::_constructor()
	{
		_output = new StdOutput;

		Arg::setDelimiter(_delimiter);

		Visitor* v;

		if (_helpAndVersion)
		{
			v = new HelpVisitor(this, &_output);
			SwitchArg* help = new SwitchArg("h", "help", "Displays usage information and exits.", false, v);
			add(help);
			deleteOnExit(help);
			deleteOnExit(v);

			v = new VersionVisitor(this, &_output);
			SwitchArg* vers = new SwitchArg("v", "version", "Displays version information and exits.", false, v);
			add(vers);
			deleteOnExit(vers);
			deleteOnExit(v);
		}

		v = new IgnoreRestVisitor();
		SwitchArg* ignore = new SwitchArg(Arg::flagStartString(), Arg::ignoreNameString(), 
										  "Ignores the rest of the labeled arguments following this flag.", false, v);
		add(ignore);
		deleteOnExit(ignore);
		deleteOnExit(v);
	}

	inline void CmdLine::xorAdd(std::vector<Arg*>& ors)
	{
		_xorHandler.add(ors);

		for (ArgVectorIterator it = ors.begin(); it != ors.end(); it++)
		{
			(*it)->forceRequired();
			(*it)->setRequireLabel("OR required");
			add(*it);
		}
	}

	inline void CmdLine::xorAdd(Arg& a, Arg& b)
	{
		std::vector<Arg*> ors;
		ors.push_back(&a);
		ors.push_back(&b);
		xorAdd(ors);
	}

	inline void CmdLine::add(Arg * a)
	{
		for (ArgListIterator it = _argList.begin(); it != _argList.end(); it++)
			if (*a == *(*it))
				throw(SpecificationException("Argument with same flag/name already exists!", a->longID()));

		a->addToList(_argList);

		if (a->isRequired())
			_numRequired++;
	}


	inline void CmdLine::parse(int argc, const char * const * argv)
	{
		// this step is necessary so that we have easy access to mutable strings.
		std::vector<std::string> args;
		for (int i = 0; i < argc; i++)
			args.push_back(argv[i]);

		parse(args);
	}

	inline void CmdLine::parse(std::vector<std::string>& args)
	{
		bool shouldExit = false;
		int estat = 0;

		try 
		{
			_progName = args.front();
			args.erase(args.begin());

			int requiredCount = 0;

			for (int i = 0; static_cast<unsigned int>(i) < args.size(); i++)
			{
				bool matched = false;
				for (ArgListIterator it = _argList.begin();
					 it != _argList.end(); it++) {
					if ((*it)->processArg(&i, args))
					{
						requiredCount += _xorHandler.check(*it);
						matched = true;
						break;
					}
				}

				// checks to see if the argument is an empty combined
				// switch and if so, then we've actually matched it
				if (!matched && _emptyCombined(args[i]))
					matched = true;

				if (!matched && !Arg::ignoreRest() && !_ignoreUnmatched)
					throw(CmdLineParseException("Couldn't find match "
												"for argument",
												args[i]));
			}

			if (requiredCount < _numRequired)
				missingArgsException();

			if (requiredCount > _numRequired)
				throw(CmdLineParseException("Too many arguments!"));

		}
		catch (ArgException& e) 
		{
			// If we're not handling the exceptions, rethrow.
			if (!_handleExceptions)
				throw;

			try 
			{
				_output->failure(*this, e);
			}
			catch (ExitException &ee) 
			{
				estat = ee.getExitStatus();
				shouldExit = true;
			}
		}
		catch (ExitException &ee) 
		{
			// If we're not handling the exceptions, rethrow.
			if (!_handleExceptions)
				throw;

			estat = ee.getExitStatus();
			shouldExit = true;
		}

		if (shouldExit)
			exit(estat);
	}

	inline bool CmdLine::_emptyCombined(std::string const & s)
	{
		if (s.length() > 0 && s[0] != Arg::flagStartChar())
			return false;

		for (int i = 1; static_cast<unsigned int>(i) < s.length(); i++)
			if (s[i] != Arg::blankChar())
				return false;

		return true;
	}

	inline void CmdLine::missingArgsException()
	{
		int count = 0;

		std::string missingArgList;
		for (ArgListIterator it = _argList.begin(); it != _argList.end(); it++)
		{
			if ((*it)->isRequired() && !(*it)->isSet())
			{
				missingArgList += (*it)->getName();
				missingArgList += ", ";
				count++;
			}
		}
		missingArgList = missingArgList.substr(0, missingArgList.length() - 2);

		std::string msg = (count > 1 ? "Required arguments missing: " : "Required argument missing: ") + missingArgList;

		throw CmdLineParseException(msg);
	}


	inline void CmdLine::setOutput(CmdLineOutput* co)
	{
		if (!_userSetOutput)
			delete _output;
		_userSetOutput = true;
		_output = co;
	}

	inline void CmdLine::reset()
	{
		for (ArgListIterator it = _argList.begin(); it != _argList.end(); it++)
			(*it)->reset();

		_progName.clear();
	}

} //namespace Handy::Args
