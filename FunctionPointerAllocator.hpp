#pragma once

#include <functional>

// FunctionPointerAllocator.hpp ---------------------
// メンバ関数などに静的関数ポインタを割り当てるクラス
// --------------------------------------------------





// ICallable ----------------------------------------------------------------
// 関数オブジェクトを実装します。
// (テンプレートと仮想関数が同時に使用できないのでコメントアウトしています。)
// --------------------------------------------------------------------------

// template<class Ret, class ...Signiture>
// // 関数オブジェクトを実装します。
// class ICallable {
//	std::function<Ret(Signiture...)> Func;
// public:
//	bool IsEmpty() const {
//		return (Func == nullptr);
//	}
//
//	void Set(std::function<Ret(Signiture...)>& func) {
//		Func = func;
//	}
//
//	virtual Ret operator()(Signiture... args) {
//		Func(args...);
//	}
// };





// Caller -------------------------
// 静的関数ポインタを提供するクラス
// --------------------------------

template<class Ret, class ...Signiture>
// 静的関数ポインタを提供するクラス
struct Caller {
	template <std::function<Ret(Signiture...)> &funcobj>
	struct Instance {
		static Ret DoCall(Signiture... args) {
			return funcobj(args...);
		}
	};
};





// StaticFunctionPointerAllocator -------------------
// メンバ関数などに静的関数ポインタを割り当てるクラス
// --------------------------------------------------

template<class Ty>
// メンバ関数などに静的関数ポインタを割り当てるクラス
struct StaticFunctionPointerAllocator;

template<class Ret, class ...Signiture>
// メンバ関数などに静的関数ポインタを割り当てるクラス
struct StaticFunctionPointerAllocator <Ret(Signiture...)> {
	typedef void (*PtrType)(Signiture...);
	typedef std::function<Ret(Signiture...)> FuncType;

	// 同時に割り当てられる静的関数ポインタの数
	static const int Capacity = 5;
	
	// 指定した関数に静的関数ポインタを割り当てます。
	static PtrType Allocate(FuncType func) {
		for (int i = 0; i < Capacity; i++) {
			if ((*List[i]) == nullptr) {
				(*List[i]) = func;
				return FuncPtr[i];
			}
		}
	
		return nullptr;
	}

	// 取得した静的関数ポインタを解放します。
	static int Free(PtrType funcptr) {
		for (int i = 0; i < Capacity; i++) {
			if (FuncPtr[i] == funcptr) {
				(*List[i]) = nullptr;
				return 0;
			}
		}

		return -1;
	}
private:
	StaticFunctionPointerAllocator() = delete;
	StaticFunctionPointerAllocator(StaticFunctionPointerAllocator&) = delete;
	StaticFunctionPointerAllocator &operator=(StaticFunctionPointerAllocator&) = delete;

	static PtrType FuncPtr[];
	static FuncType _1, _2, _3, _4, _5;
	static FuncType *List[];
};

// 静的メンバを初期化 -----

#define DeclearMember(x) \
	template<class Ret, class ...Signiture> \
	std::function<Ret(Signiture...)> StaticFunctionPointerAllocator<Ret(Signiture...)>::##x = std::function<Ret(Signiture...)>()

DeclearMember(_1);
DeclearMember(_2);
DeclearMember(_3);
DeclearMember(_4);
DeclearMember(_5);

#undef DeclearMember

template<class Ret, class ...Signiture>
std::function<Ret(Signiture...)> *StaticFunctionPointerAllocator<Ret(Signiture...)>::List[] = {
		&_1, &_2, &_3, &_4, &_5
};

template<class Ret, class ...Signiture>
typename StaticFunctionPointerAllocator<Ret(Signiture...)>::PtrType StaticFunctionPointerAllocator<Ret(Signiture...)>::FuncPtr[] = {
	Caller<Ret, Signiture...>::Instance<_1>::DoCall,
	Caller<Ret, Signiture...>::Instance<_2>::DoCall,
	Caller<Ret, Signiture...>::Instance<_3>::DoCall,
	Caller<Ret, Signiture...>::Instance<_4>::DoCall,
	Caller<Ret, Signiture...>::Instance<_5>::DoCall
};

// ------------------------