#pragma once

// EventHandler.hpp -----------------
// 登録した関数を一括実行できるクラス
// ----------------------------------





// FunctionDataBase, FunctionData, ClassFunctionData --
// 関数ポインタを持つクラスの基底クラス。比較ができる。
// ----------------------------------------------------

#include <new>

template <class RetType, class ...Signiture>
// 関数ポインタを持つクラスの基底クラス。比較ができる。
class FunctionDataBase {
	void *Owner;
	char *FuncPtrVal;
	size_t FuncPtrValLength;

	FunctionDataBase(const FunctionDataBase&);
	FunctionDataBase &operator=(const FunctionDataBase&);
protected:
	FunctionDataBase(RetType (*funcptr)(Signiture...)) :
		Owner(nullptr),
		FuncPtrVal(new char[sizeof funcptr]),
		FuncPtrValLength(sizeof(funcptr)) {

		// 関数ポインタの値をコピー
		new(FuncPtrVal) (RetType(*)(Signiture...))(funcptr);
	}

	template<class Ty>
	FunctionDataBase(RetType (Ty::*funcptr)(Signiture...), Ty* owner) :
		Owner(owner),
		FuncPtrVal(new char[sizeof funcptr]),
		FuncPtrValLength(sizeof(funcptr)) {

		// 関数ポインタの値をコピー
		new(FuncPtrVal) (RetType(Ty::*)(Signiture...))(funcptr);
	}
public:
	// std::string FuncSig;

	virtual ~FunctionDataBase() {
		delete[] FuncPtrVal;
	}

	virtual RetType operator()(Signiture...) = 0;

	bool operator==(FunctionDataBase& _Right) {
		return (Owner == _Right.Owner && FuncPtrValLength == _Right.FuncPtrValLength && memcmp(FuncPtrVal, _Right.FuncPtrVal, FuncPtrValLength) == 0);
	}

	bool operator!=(FunctionDataBase& _Right) {
		return (Owner != _Right.Owner || FuncPtrValLength != _Right.FuncPtrValLength || memcmp(FuncPtrVal, _Right.FuncPtrVal, FuncPtrValLength) != 0);
	}
	
	virtual FunctionDataBase *CloneNew() const = 0;
};



template<class RetType, class ...Signiture>
// 普通の関数ポインタのラッパークラス
class FunctionData : public FunctionDataBase<RetType, Signiture...> {
	typedef RetType (*MyFunction)(Signiture...);
	
	MyFunction FuncPtr;

	FunctionData(const FunctionData&);
	FunctionData &operator=(const FunctionData);
public:
	FunctionData(MyFunction funcptr) :
		FunctionDataBase(funcptr),
		FuncPtr(funcptr) {
	}

	// 関数を実行します。
	RetType operator()(Signiture... args) override {
		return (*FuncPtr)(std::forward<Signiture>(args)...);
	}
	
	//bool operator==(FunctionDataBase* Other) override {
	//	if (FuncSig == Other->FuncSig) {
	//		FunctionData<Ty> *OtherPtr = static_cast<FunctionData<Ty>*>(Other);
	//		return (is_same_function_ptr(FuncPtr, OtherPtr->FuncPtr) && Owner == OtherPtr->Owner);
	//	} else {
	//		return false;
	//	}
	//}

	//bool operator!=(FunctionDataBase* Other) override {
	//	if (FuncSig == Other->FuncSig) {
	//		FunctionData<Ty> *OtherPtr = static_cast<FunctionData<Ty>*>(Other);
	//		return (!is_same_function_ptr(FuncPtr, OtherPtr->FuncPtr) || Owner != OtherPtr->Owner);
	//	} else {
	//		return true;
	//	}
	//}

	// 新しいインスタンスを作成します。
	FunctionDataBase *CloneNew() const override {
		FunctionData *clone = new FunctionData(FuncPtr);
		return clone;
	}
};



template<class Ty, class RetType, class ...Signiture>
// メンバ関数ポインタのラッパークラス
class ClassFunctionData : public FunctionDataBase<RetType, Signiture...> {
	typedef RetType(Ty::*MyFunction)(Signiture...);

	MyFunction FuncPtr;
	Ty *Owner;

	ClassFunctionData(const ClassFunctionData&);
	ClassFunctionData &operator=(const ClassFunctionData);
public:
	ClassFunctionData(MyFunction funcptr, Ty *owner) :
		FunctionDataBase(funcptr, owner),
		FuncPtr(funcptr),
		Owner(owner) {
	}

	// 関数を実行します。
	RetType operator()(Signiture... args) override {
		return (Owner->*FuncPtr)(std::forward<Signiture>(args)...);
	}

	// 新しいインスタンスを作成します。
	FunctionDataBase *CloneNew() const override {
		ClassFunctionData *clone = new ClassFunctionData(FuncPtr, Owner);
		return clone;
	}
};





// Function --------------------
// FunctionData のラッパークラス
// -----------------------------

// #include <functional>

template<class RetType, class... Signiture>
// FunctionData のラッパークラス
class Function {
	// typedef std::function<void(Signiture...)> MyFunctionType;

	FunctionDataBase<RetType, Signiture...> *base;
	// MyFunctionType MyFunction;
public:
	Function() :
		base(nullptr) {
	}

	Function(RetType(*funcptr)(Signiture...)) :
		base(new FunctionData<RetType, Signiture...>(funcptr)) {
	}

	template<class Ty>
	Function(RetType(Ty::*funcptr)(Signiture...), Ty* Owner) :
		base(new ClassFunctionData<Ty, RetType, Signiture...>(funcptr, Owner)) {
		
		/* if (sizeof...(Signiture) > 0)
			MyFunction = std::function<void(Signiture...)>(std::bind(funcptr, Owner, std::placeholders::_1));
		else
			MyFunction = std::function<void()>(std::bind(funcptr, Owner)); */
	}

	Function(const Function& Other) {
		base = Other.base->CloneNew();
		// MyFunction = Other.MyFunction;
	}

	Function &operator=(const Function& Other) {
		if (&Other == *this)
			return (*this);

		if (base != nullptr) {
			delete base;
		}

		base = Other.base->CloneNew();
		// MyFunction = Other.MyFunction;

		return (*this);
	}

	Function(Function&& Right) {
		base = Right.base;
		Right.base = nullptr;

		// MyFunction = std::move(Right.MyFunction);
	}

	Function &operator=(Function&& Right) {
		if (&Right == this)
			return (*this);

		if (base != nullptr)
			delete base;

		base = Right.base;
		Right.base = nullptr;

		// MyFunction = std::move(Right.MyFunction);

		return (*this);
	}

	~Function() {
		delete base;
		base = nullptr;
	}

	// MyFunctionType &GetFunction() {
	//	return MyFunction;
	// }

	//bool operator==(const Function& Other) {
	//	return base->operator==(*(Other.base));
	//}

	//bool operator!=(const Function& Other) {
	//	return base->operator!=(*(Other.base));
	//}

	RetType operator()(Signiture... Args...) {
		// MyFunction(Args...);
		return (*base)(Args...);
	}
};





// EventHandler -------------------------
// 引数が同じ関数の一括実行を行うクラス。
// --------------------------------------

#include <list>

template<class Type>
class EventHandler;

// <特殊化> 関数形式のとき
template <class... Signiture>
// 引数が同じ関数の一括実行を行うクラス。
class EventHandler<void(Signiture...)> {
	// typedef std::function<void(Signiture...)> MyFunctionType;
	typedef Function<void, Signiture...> MyFunctionData;

	std::list<MyFunctionData> MyFunctionList;

	EventHandler(const EventHandler&);
	EventHandler &operator=(const EventHandler&);
public:
	EventHandler() {
		MyFunctionList.clear();
	}

	// 実行関数リストに登録します。
	MyFunctionData operator+=(void(*funcptr)(Signiture...)) {
		Function<void, Signiture...> myfunc(funcptr);
		MyFunctionList.push_back(myfunc);
		return std::move(myfunc);
	}

	// 実行関数リストから削除します。
	MyFunctionData operator-=(void(*funcptr)(Signiture...)) {
		Function<void, Signiture...> myfunc(funcptr);
		MyFunctionList.remove(myfunc);
		return std::move(myfunc);
	}

	// 登録された関数を実行します。
	void operator()(Signiture... Args...) {
		for (MyFunctionData &func : MyFunctionList) {
			func(Args...);
		}
	}

	// 実行関数リストに登録します。
	MyFunctionData AddHandler(void(*funcptr)(Signiture...)) {
		return operator+=(funcptr);
	}

	template<class Ty>
	// 実行関数リストに登録します。
	MyFunctionData AddHandler(void(Ty::*funcptr)(Signiture...), Ty *Owner) {
		Function<void, Signiture...> myfunc(funcptr, Owner);
		MyFunctionList.push_back(myfunc);
		return std::move(myfunc);
	}

	// 実行関数リストに登録します。
	MyFunctionData &AddHandler(MyFunctionData& func) {
		MyFunctionList.push_back(func);
		return func;
	}

	// 実行関数リストから削除します。
	MyFunctionData RemoveHandler(void(*funcptr)(Signiture...)) {
		return operator-=(funcptr);
	}

	template<class Ty>
	// 実行関数リストから削除します。
	MyFunctionData RemoveHandler(void(Ty::*funcptr)(Signiture...), Ty *Owner) {
		Function<void, Signiture...> myfunc(funcptr, Owner);
		MyFunctionList.remove(myfunc);
		return std::move(myfunc);
	}

	// 実行関数リストから削除します。
	MyFunctionData &RemoveHandler(MyFunctionData& func) {
		MyFunctionList.remove(func);
		return func;
	}

	// 登録された関数を実行します。
	void RaiseEvent(Signiture... Args...) {
		for (auto &func : MyFunctionList) {
			func(Args...);
		}
	}
};