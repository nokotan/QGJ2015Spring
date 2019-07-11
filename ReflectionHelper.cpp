#include "ReflectorHelper.hpp"

extern template class std::basic_stringstream<char>;

namespace Reflection {

#	define ExpandMacro(macro, delim)	\
	macro(char) delim				\
	macro(unsigned char) delim		\
	macro(short) delim				\
	macro(unsigned short) delim		\
	macro(int) delim				\
	macro(unsigned int) delim		\
	macro(long) delim				\
	macro(unsigned long) delim		\
	macro(float) delim				\
	macro(double) delim				\
	macro(long double)

	struct StaticConverter {
		template <class From, class To>
		static To Convert(From val) {
			return static_cast<To>(val);
		}
	};

	struct LexicalConverter {
		template <class From>
		static std::string Convert(From val) {
			std::stringstream StringBuilder;
			StringBuilder << val;
			return StringBuilder.str();
		}
	};

	struct ReverseLexicalConverter {
		template <class To>
		static To ConvertFromAsciiChar(const char* val) {
			std::stringstream StringBuilder;
			StringBuilder << val;

			To RetVal;
			StringBuilder >> RetVal;
			return RetVal;
		}
	};

#	define RegisterConvertFunction(to) AddFunction(typeid(to).name(), new FunctionWrapper<to(*)(MyType)>(&StaticConverter::Convert<MyType, to>))
#	define RegisterLexicalConvertFunction() AddFunction(typeid(std::string).name(), new FunctionWrapper<std::string(*)(MyType)>(&LexicalConverter::Convert<MyType>))
#	define RegisterReverseLexicalConvertFunction(to) AddFunction(typeid(to).name(), new FunctionWrapper<to(*)(const char*)>(&ReverseLexicalConverter::ConvertFromAsciiChar<to>))

	class Type : public TypeBase {
		static std::string ToString(nullptr_t) {
			return std::string("nothing");
		}
	public:
		template <class MyType>
		static TypeBase CreateType() {
			TypeBase MyTypeObj;
			ExpandMacro(MyTypeObj.RegisterConvertFunction, ;);
			MyTypeObj.RegisterLexicalConvertFunction();
			return MyTypeObj;
		}

		static TypeBase CreateConstCharArrayType() {
			TypeBase MyTypeObj;
			ExpandMacro(MyTypeObj.RegisterReverseLexicalConvertFunction, ;);
			return MyTypeObj;
		}

		static TypeBase CreateNullptrType() {
			TypeBase MyTypeObj;
			MyTypeObj.AddFunction(typeid(std::string).name(), new FunctionWrapper<std::string(*)(nullptr_t)>(&ToString));
			return MyTypeObj;
		}
	};

#	define RegisterTypes(type) Types[typeid(type).name()] = Type::CreateType<type>();

	class BasicTypes {
		BasicTypes() {
			ExpandMacro(RegisterTypes, ;);
			Types[typeid(const char*).name()] = Type::CreateConstCharArrayType();
			Types[typeid(nullptr_t).name()] = Type::CreateNullptrType();
		}
	public:
		static BasicTypes &Instance() {
			static BasicTypes Inst;
			return Inst;
		}

		std::map<std::string, TypeBase> Types;
	};

	void RegisterType(const std::type_info& info, TypeBase& obj) {
		if (BasicTypes::Instance().Types.find(info.name()) == BasicTypes::Instance().Types.end()) {
			BasicTypes::Instance().Types[info.name()] = obj;
		}
	}

	TypeBase &GetType(const char name[]) {
		return BasicTypes::Instance().Types[name];
	}

	TypeBase &GetType(const std::type_info& info) {
		return GetType(info.name());
	}
}