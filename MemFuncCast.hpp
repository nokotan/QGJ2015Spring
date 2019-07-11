#pragma once

// MemFuncCast.hpp ----------------------
// �����o�֐���ÓI�֐��ɕϊ�����N���X�B
// --------------------------------------

template <class Type>
// �����o�֐���ÓI�֐��ɕϊ�����N���X�B
struct ClassFunction;

template <class Type, class RetType, class... Args> 
// �����o�֐���ÓI�֐��ɕϊ�����N���X�B
struct ClassFunction<RetType(Type::*)(Args...)> {
	typedef RetType (Type::*MyMemberFunctionType)(Args...);
	typedef RetType (*MyFunctionType)(Type*, Args...);

	template <MyMemberFunctionType FuncPtr>
	class Member {
	public:
		// �����o�֐����Ăяo���܂��B
		static void DoCall(Type *OwnPtr, Args... args) {
			(OwnPtr->*FuncPtr)(static_cast<Args>(args)...);
		}
	};
};

// �����o�֐��i�萔�l�j��ÓI�֐��ɕϊ����܂��B�ϊ���̊֐��́A��P�����ɂ��̃N���X�̃C���X�^���X�̃|�C���^��n���܂��B
#define member_function_cast(x) ClassFunction<decltype(x)>::Member<x>::DoCall





// is_same_function_ptr -----------------------
// �����֐��̃|�C���^�ł��邩�ǂ������肵�܂��B
// --------------------------------------------

template<class x1, class x2>
// �����֐��̃|�C���^�ł��邩�ǂ������肵�܂��B
struct is_same_function_ptr_t {
	static bool value(x1 arg1, x2 arg2) {
		return false;
	}
};

template<class ret1, class... Signiture>
// <���ꉻ�P> ���ʂ̊֐��̔�r
struct is_same_function_ptr_t<ret1(*)(Signiture...), ret1(*)(Signiture...)> {
	static bool value(ret1(*ptr1)(Signiture...), ret1(*ptr2)(Signiture...)) {
		return (ptr1 == ptr2);
	}
};

template<class ret1, class x1, class... Signiture>
// <���ꉻ�Q> �����o�֐��̔�r
struct is_same_function_ptr_t<ret1(x1::*)(Signiture...), ret1(x1::*)(Signiture...)> {
	static bool value(ret1(x1::*ptr1)(Signiture...), ret1(x1::*ptr2)(Signiture...)) {
		return (ptr1 == ptr2);
	}
};

template<class x1, class x2>
// �����֐��̃|�C���^�ł��邩�ǂ������肵�܂��B
bool is_same_function_ptr(x1 ptr1, x2 ptr2) {
	return is_same_function_ptr_t<x1, x2>::value(ptr1, ptr2);
}
