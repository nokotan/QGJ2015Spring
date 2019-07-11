#include "ReflectorHelper.hpp"

namespace Reflection {

	ValueObject::ValueObject() :
		Data(ValueHolderBasePtr(new ByValValueHolder<nullptr_t>(nullptr))) {
	}

	bool ValueObject::IsEmpty() const {
		return Data->TypeData == typeid(nullptr_t);
	}

	ValueObject ValueObject::InternalCast(const type_info& info) {
		if (Data->TypeData == info) {
			return *this;
		}

		auto ThisType = Reflection::GetType(Data->TypeData);

		ArgsList Args{ *this };
		return ThisType.InvokeMember(info.name(), Args);	
	}

	ValueObject& FunctionWrapperBase::GetValue(ArgsList& Args, size_t Index) {
		return Args[Index];
	}

	ValueObject::ValueHolderBase::ValueHolderBase(const type_info &data) :
		TypeData(data) {
	}

	bool ValueObject::CheckType(const type_info& info) {
		return Data->TypeData == info;
	}

	TypeBase &ValueObject::GetType() {
		return Reflection::GetType(Data->TypeData);
	}
}

template class std::basic_stringstream<char>;
