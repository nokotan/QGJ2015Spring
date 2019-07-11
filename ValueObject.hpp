#pragma once

namespace Reflection {

	template <class Type>
	ValueObject::ValueHolder<Type>::ValueHolder() :
		ValueHolderBase(typeid(Type)) {
	}



	template <class Type>
	ValueObject::ByValValueHolder<Type>::ByValValueHolder(Type data) :
		Data(data) {
		RegisterType(typeid(Type), DefaultTypeTraits<Type>());
	}

	template <class Type>
	Type &ValueObject::ByValValueHolder<Type>::GetData() {
		return Data;
	}



	template <class Type>
	ValueObject::ByRefValueHolder<Type>::ByRefValueHolder(Type& data) :
		Data(data) {
		RegisterType(typeid(Type), DefaultTypeTraits<Type>());
	}

	template <class Type>
	Type &ValueObject::ByRefValueHolder<Type>::GetData() {
		return Data;
	}	
}