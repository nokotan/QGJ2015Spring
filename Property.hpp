#pragma once

// Property.hpp ------------------------------------
// C# �� VB �Ɏ�������Ă���v���p�e�B���������܂��B
// -------------------------------------------------

#include <functional>

template<typename Ty, typename GetterSigniture = Ty(void), typename SetterSigniture = Ty(const Ty&)>
// �v���p�e�B���������܂��B
class Property;

template<typename Ty, typename GetterRetType, typename... GetterArgs, typename SetterRetType, typename... SetterArgs>
// �v���p�e�B���������܂��B
class Property<Ty, GetterRetType(GetterArgs...), SetterRetType(SetterArgs...)> {
public:
	typedef std::function<GetterRetType(GetterArgs...)> GetterType;
	typedef std::function<SetterRetType(SetterArgs...)> SetterType;
private:
	GetterType Getter;
	SetterType Setter;
public:
	Property(const GetterType& getter, const SetterType& setter) :
		Getter(getter), Setter(setter) {
	}

	GetterRetType operator()(GetterArgs... Args) const {
		return Getter(std::forward<GetterArgs>(Args)...);
	}

	SetterRetType operator()(SetterArgs... Args) {
		return Setter(Args...);
	}
	
	operator Ty() const {
		return Getter();
	}

	SetterRetType operator=(SetterArgs... Args) {
		return Setter(Args...);
	}
};
