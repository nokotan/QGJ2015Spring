#pragma once

// MemFuncCast.hpp ----------------------
// メンバ関数を静的関数に変換するクラス。
// --------------------------------------

template <class Type>
// メンバ関数を静的関数に変換するクラス。
struct ClassFunction;

template <class Type, class RetType, class... Args> 
// メンバ関数を静的関数に変換するクラス。
struct ClassFunction<RetType(Type::*)(Args...)> {
	typedef RetType (Type::*MyMemberFunctionType)(Args...);
	typedef RetType (*MyFunctionType)(Type*, Args...);

	template <MyMemberFunctionType FuncPtr>
	class Member {
	public:
		// メンバ関数を呼び出します。
		static void DoCall(Type *OwnPtr, Args... args) {
			(OwnPtr->*FuncPtr)(static_cast<Args>(args)...);
		}
	};
};

// メンバ関数（定数値）を静的関数に変換します。変換後の関数は、第１引数にそのクラスのインスタンスのポインタを渡します。
#define member_function_cast(x) ClassFunction<decltype(x)>::Member<x>::DoCall





// is_same_function_ptr -----------------------
// 同じ関数のポインタであるかどうか判定します。
// --------------------------------------------

template<class x1, class x2>
// 同じ関数のポインタであるかどうか判定します。
struct is_same_function_ptr_t {
	static bool value(x1 arg1, x2 arg2) {
		return false;
	}
};

template<class ret1, class... Signiture>
// <特殊化１> 普通の関数の比較
struct is_same_function_ptr_t<ret1(*)(Signiture...), ret1(*)(Signiture...)> {
	static bool value(ret1(*ptr1)(Signiture...), ret1(*ptr2)(Signiture...)) {
		return (ptr1 == ptr2);
	}
};

template<class ret1, class x1, class... Signiture>
// <特殊化２> メンバ関数の比較
struct is_same_function_ptr_t<ret1(x1::*)(Signiture...), ret1(x1::*)(Signiture...)> {
	static bool value(ret1(x1::*ptr1)(Signiture...), ret1(x1::*ptr2)(Signiture...)) {
		return (ptr1 == ptr2);
	}
};

template<class x1, class x2>
// 同じ関数のポインタであるかどうか判定します。
bool is_same_function_ptr(x1 ptr1, x2 ptr2) {
	return is_same_function_ptr_t<x1, x2>::value(ptr1, ptr2);
}
