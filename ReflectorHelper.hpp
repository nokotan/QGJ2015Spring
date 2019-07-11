#pragma once

/// @file ReflectorHelper.hpp
/// ReflectorHelper.hpp ------------------------------
/// C++ でのリフレクションを試験的に行う部分（開発中）
/// --------------------------------------------------

#include <typeinfo>
#include <memory>
#include <map>
#include <sstream>
#include <vector>

#include "DebugHelper.h"

namespace Reflection {
	class TypeBase;

	/// <summary>任意型の値を保持することができるクラス</summary>
	class ValueObject {
		/// <summary>任意型の値を実際に保持するクラスの基底クラス</summary>
		struct ValueHolderBase {
			const std::type_info &TypeData;
		protected:
			ValueHolderBase(const type_info &data);
		};

		template <class Type>
		/// <summary>任意型の値を実際に保持するクラス</summary>
		struct ValueHolder : public ValueHolderBase {
			virtual Type &GetData() = 0;
		protected:
			ValueHolder();
		};

		template <class Type>
		/// <summary>任意型の値を実際に保持するクラス</summary>
		struct ByValValueHolder : public ValueHolder<Type> {
			ByValValueHolder(Type data);

			Type &GetData() override;
		private:
			Type Data;
		};

		template <class Type>
		/// <summary>任意型の値への参照を実際に保持するクラス</summary>
		struct ByRefValueHolder : public ValueHolder<Type> {
			ByRefValueHolder(Type& data);

			Type &GetData() override;
		private:
			Type& Data;
		};

		template <class Type>
		Type EnumCast(ValueObject& obj, std::true_type) {
			ValueObject&& CastedObject = obj.InternalCast(typeid(long));
			return static_cast<Type>(CastedObject.Get<long>());
		}

		template <class Type>
		Type EnumCast(ValueObject& obj, std::false_type) {
			throw std::bad_cast();
		}

		typedef std::shared_ptr<ValueHolderBase> ValueHolderBasePtr;
		ValueHolderBasePtr Data;

		ValueObject(ValueHolderBasePtr data) :
			Data(data) {
		}
	public:
		/// <summary>空の値を保持する ValueObject オブジェクトを初期化します。</summary>
		ValueObject();

		template <class Type>
		/// <summary>指定された値を保持する ValueObject オブジェクトを初期化します。</summary>
		ValueObject(Type val) :
			Data(ValueHolderBasePtr(new ByValValueHolder<Type>(val))) {
		}

		template <class Type>
		/// <summary>指定された値への参照を保持する ValueObject オブジェクトを初期化します。</summary>
		ValueObject(std::reference_wrapper<Type> val) :
			Data(ValueHolderBasePtr(new ByRefValueHolder<Type>(val))) {
		}

		template <class Type, size_t Size>
		/// <summary>指定された配列を保持する ValueObject オブジェクトを初期化します。</summary>
		ValueObject(Type (&val)[Size]) :
			Data(ValueHolderBasePtr(new ByRefValueHolder<Type[Size]>(val))) {
		}

		template <class Type>
		/// <summary>このオブジェクトが保持している値を取得します。型変換に失敗した場合、例外を送出します。</summary>
		Type Get();

		template <class Type>
		/// <summary>このオブジェクトが保持している値を取得します。型変換に失敗した場合、例外を送出します。</summary>
		explicit operator Type() {
			return Get<Type>();
		}

		/// <summary>内部の値を指定した型に変換します。</summary>
		ValueObject InternalCast(const type_info& info);

		/// <summary>このオブジェクトが保持している値が空かどうか確認します。</summary>
		/// <returns>このオブジェクトが保持している値が空ならば true, そうでなければ false</returns>
		bool IsEmpty() const;

		bool CheckType(const type_info& info);

		TypeBase &GetType();
	};

	/// <summary>関数に渡す引数のリスト</summary>
	typedef std::vector<ValueObject> ArgsList;



	template <class FunctionType>
	/// <summary>画一的な関数呼び出しを実現するクラス</summary>
	class FunctionWrapper;

	/// <summary>画一的な関数呼び出しを実現するクラスの基底クラス</summary>
	class FunctionWrapperBase {
	protected:
		static ValueObject& GetValue(ArgsList& Args, size_t Index);

		template <size_t ...Indexes>
		struct ArgsIndexes {
		};

		template <size_t Count, size_t ...Indexes>
		struct MakeIndexes : public MakeIndexes<Count - 1, Count - 1, Indexes...> {
		};

		template <size_t ...Indexes>
		struct MakeIndexes <0, Indexes...> {
			typedef ArgsIndexes<Indexes...> Type;
		};

		template <class Type>
		struct CopyHelper {
			static void Copy(const Type& From, Type& To) {
				To = From;
			}
		};

		template <class Type>
		struct CopyHelper <const Type&> {
			static void Copy(const Type& From, const Type& To) {

			}
		};
	
		template <class Type, size_t Size>
		struct CopyHelper <Type(&)[Size]> {
			static void Copy(const Type (&From)[Size], Type (&To)[Size]) {
				for (int i = 0; i < Size; i++) {
					To[i] = From[i];
				}
			}
		};
	public:
		/// <summary>ValueObject の配列を引数として関数を呼び出します。</summary>
		/// <param name="args">関数の引数として渡す ValueObject の配列</param>
		virtual ValueObject Invoke(ArgsList& args) = 0;
		/// <summary>メンバ変数の値を書き込みます。</summary>
		/// <param name="value">メンバ変数の新しい値を保持する ValueObject オブジェクト</param>
		virtual void SetValue(ValueObject& Obj, ValueObject& value) { };
		/// <summary>メンバ変数の値を読み出します。</summary>
		/// <returns>メンバ変数の値を保持する ValueObject オブジェクト</returns>
		virtual ValueObject GetValue(ValueObject& Obj) { return ValueObject(); };
		/// <summary>この FunctionWrapperBase オブジェクトが保持するリソースを解放します。</summary>
		virtual ~FunctionWrapperBase() = default;
	};

	template <class RetType, class ...Args>
	/// <summary>画一的な関数呼び出しを実現するクラス</summary>
	class FunctionWrapper<RetType(*)(Args...)> : public FunctionWrapperBase {
		typedef RetType(*MyFunction)(Args...);
		typedef typename MakeIndexes<sizeof...(Args)>::Type IndexType;
		MyFunction Func;

		template <class RetType, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType(*)(Args...)>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject(Func(GetValue(args, Indexes).Get<Args>()...));
		};

		template <class RetType, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType &(*)(Args...)>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject(std::ref(Func(GetValue(args, Indexes).Get<Args>()...)));
		}

		template <class RetType, size_t Size, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType(&(*)(Args...))[Size]>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject(Func(GetValue(args, Indexes).Get<Args>()...));
		}

		template <size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<void(*)(Args...)>& , ArgsList& args, ArgsIndexes<Indexes...>&&) {
			Func(GetValue(args, Indexes).Get<Args>()...);
			return ValueObject();
		}
	public:
		/// <summary>指定された関数を呼び出す FunctionWrapper オブジェクトを初期化します。</summary>
		/// <param name="MyFunc">この FunctionWrapper オブジェクトが呼び出す関数オブジェクト</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (sizeof...(Args) > args.size()) {
				DebugTrace("OutOfRangeException : 引数の数が足りません。\n引数の数: %d, 必要な数: %d", args.size(), sizeof...(Args));
				return ValueObject();
			} else
#endif
			{
				return DoCall(*this, args, IndexType());
			}
		}
	};

	template <class RetType, class OwnerType, class ...Args>
	/// <summary>画一的な関数呼び出しを実現するクラス</summary>
	class FunctionWrapper<RetType(OwnerType::*)(Args...)> : public FunctionWrapperBase {
		typedef RetType(OwnerType::*MyFunction)(Args...);
		typedef typename MakeIndexes<sizeof...(Args)>::Type IndexType;
		MyFunction Func;

		template <class RetType, class OwnerType, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType(OwnerType::*)(Args...)>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject((GetValue(args, 0).Get<OwnerType&>().*Func)(GetValue(args, Indexes + 1).Get<Args>()...));
		}

		template <class RetType, class OwnerType, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType &(OwnerType::*)(Args...)>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject(std::ref((GetValue(args, 0).Get<OwnerType&>().*Func)(GetValue(args, Indexes + 1).Get<Args>()...)));
		}

		template <class RetType, class OwnerType, size_t Size, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<RetType(&(OwnerType::*)(Args...))[Size]>&, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			return ValueObject((GetValue(args, 0).Get<OwnerType&>().*Func)(GetValue(args, Indexes + 1).Get<Args>()...));
		}

		template <class OwnerType, size_t ...Indexes>
		ValueObject DoCall(FunctionWrapper<void(OwnerType::*)(Args...)>& funcobj, ArgsList& args, ArgsIndexes<Indexes...>&&) {
			(GetValue(args, 0).Get<OwnerType&>().*Func)(GetValue(args, Indexes + 1).Get<Args>()...);
			return ValueObject();
		}
	public:
		/// <summary>指定された関数を呼び出す FunctionWrapper オブジェクトを初期化します。</summary>
		/// <param name="MyFunc">この FunctionWrapper オブジェクトが呼び出す関数オブジェクト</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (sizeof...(Args) > args.size()) {
				DebugTrace("OutOfRangeException : 引数の数が足りません。\n引数の数: %d, 必要な数: %d", args.size(), sizeof...(Args));
				return ValueObject();
			} else
#endif
			{
				return DoCall(*this, args, IndexType());
			}
		}
	};

	template <class RetType, class OwnerType>
	/// <summary>画一的な関数呼び出しを実現するクラス</summary>
	class FunctionWrapper<RetType(OwnerType::*)> : public FunctionWrapperBase{
		typedef RetType(OwnerType::*MyFunction);
		MyFunction Func;
	public:
		/// <summary>指定された関数を呼び出す FunctionWrapper オブジェクトを初期化します。</summary>
		/// <param name="MyFunc">この FunctionWrapper オブジェクトが呼び出す関数オブジェクト</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (1 > args.size()) {
				DebugTrace("OutOfRangeException : 引数の数が足りません。\n引数の数: %d, 必要な数: 1", args.size());
				return ValueObject();
			}
			else
#endif
			{
				return GetValue(args[0]);
			}
		}	
		
		ValueObject GetValue(ValueObject& Obj) override {
			OwnerType& OwnerObj = Obj.Get<OwnerType&>();
			return ValueObject(OwnerObj.*Func);
		}

		void SetValue(ValueObject& Obj, ValueObject& value) override {
			OwnerType& OwnerObj = Obj.Get<OwnerType&>();
			RetType& MemberValue = OwnerObj.*Func;
			ValueObject&& NewObject = value.InternalCast(typeid(RetType));
			CopyHelper<RetType&>::Copy(NewObject.Get<RetType&>(), MemberValue);
		}
	};



	/// <summary>リフレクション用の型情報を保持するクラス</summary>
	class TypeBase {
		typedef std::shared_ptr<FunctionWrapperBase> FunctionWrapperBasePtr;

		std::map<std::string, FunctionWrapperBasePtr> FunctionList;
		std::map<std::string, FunctionWrapperBasePtr> MemberList;
	public:
		/// <summary>指定した名前で FunctionWrapperBase オブジェクトを登録します。</summary>
		/// <param name="name">登録する名前</param>
		/// <param name="func">登録する  FunctionWrapperBase オブジェクトへのポインタ。内部で自動的に解放処理が行われます。</param>
		void AddFunction(std::string name, FunctionWrapperBase *funcobj) {
			FunctionList[name] = FunctionWrapperBasePtr(funcobj);
		}

		template <class RetType, class ...ArgsType>
		/// <summary>指定した名前でグローバル関数を登録します。</summary>
		/// <param name="name">登録する名前</param>
		/// <param name="func">登録するグローバル関数</param>
		void AddFunction(std::string name, RetType (*func)(ArgsType...)) {
			FunctionList[name] = FunctionWrapperBasePtr(new FunctionWrapper<RetType(*)(ArgsType...)>(func));
		}

		template <class Type, class RetType, class ...ArgsType>
		/// <summary>指定した名前でメンバ関数を登録します。</summary>
		/// <param name="name">登録する名前</param>
		/// <param name="func">登録するメンバ関数</param>
		void AddMember(std::string name, RetType(Type::*func)(ArgsType...)) {
			FunctionList[name] = FunctionWrapperBasePtr(new FunctionWrapper<RetType(Type::*)(ArgsType...)>(func));
		}

		template <class OwnerType, class Type>
		/// <summary>指定した名前でメンバ変数を登録します。</summary>
		/// <param name="name">登録する名前</param>
		/// <param name="func">登録するメンバ変数</param>
		void AddMember(std::string name, Type(OwnerType::*member)) {
			MemberList[name] = FunctionWrapperBasePtr(new FunctionWrapper<Type(OwnerType::*)>(member));
		}
	
		/// <summary>指定された名前で登録されている関数を呼び出します。</summary>
		/// <param name="name">登録しておいた呼び出す関数の名前</param>
		/// <param name="args">関数に渡す引数。メンバ関数の場合は、第１引数として、this に相当するものが必要です。</param>
		/// <returns>関数の戻り値をラップする ValueObject オブジェクト</returns>
		ValueObject InvokeMember(const char name[], ArgsList& args);

		/// <summary>指定したオブジェクトのメンバの値を標準出力に吐き出します。</summary>
		/// <param name="name">確認したいメンバ変数の登録名</param>
		/// <param name="Obj">確認したいメンバ変数を保持しているオブジェクト</param>
		void DumpMemberValue(const char name[], ValueObject Obj);

		/// <summary>指定したオブジェクトのメンバの値を読み出します。</summary>
		/// <param name="name">値を読み出したいメンバ変数の登録名</param>
		/// <param name="Obj">確認したいメンバ変数を保持しているオブジェクト</param>
		/// <returns>メンバ変数の値をラップする ValueObject オブジェクト</returns>
		ValueObject GetMemberValue(const char name[], ValueObject Obj);

		/// <summary>指定したオブジェクトのメンバの値を更新します。</summary>
		/// <param name="name">値を更新したいメンバ変数の登録名</param>
		/// <param name="Obj">値を更新したいメンバ変数を保持しているオブジェクト</param>
		/// <param name="value">メンバ変数の新しい値</param>
		void SetMemberValue(const char name[], ValueObject Obj, ValueObject value);
	};

	template <class ThisType>
	/// <summary>メンバの追加を簡単にする機能を提供します。</summary>
	class TypeImpl : public TypeBase {
	protected:
		typedef ThisType MyType;
	};

	void RegisterType(const std::type_info& typeinfo, TypeBase& obj);

	/// <summary>指定された型を表す文字列から、型情報を取得します。</summary>
	/// <param name="name">指定された型を表す文字列</param>
	/// <returns>指定された型を表す文字列に関連付けられた型情報</returns>
	TypeBase &GetType(const char name[]);

	/// <summary>指定された型を表す std::typeinfo オブジェクトから、型情報を取得します。</summary>
	/// <param name="typeinfo">指定された型を表す std::typeinfo オブジェクト</param>
	/// <returns>指定された型を表す std::typeinfo オブジェクトに関連付けられた型情報</returns>
	TypeBase &GetType(const std::type_info& typeinfo);

	template <class Type>
	/// <summary>指定された型から、型情報を取得します。</summary>
	/// <param name="Type">型情報を取得する型</param>
	/// <returns>指定された型に関連付けられた型情報</returns>
	TypeBase &GetType() {
		return GetType(typeid(Type));
	};

	template <class Type>
	/// <summary>このオブジェクトが保持している値を取得します。型変換に失敗した場合、例外を送出します。</summary>
	Type ValueObject::Get() {
		if (CheckType(typeid(Type))) {
			return static_cast<ValueHolder<Type>*>(Data.get())->GetData();
		} else {
			ValueObject ReturnedValue = InternalCast(typeid(Type));

			if (!ReturnedValue.IsEmpty()) {
				return ReturnedValue.Get<Type>();
			}

			std::string ErrorMessage;
			ErrorMessage = ErrorMessage + "Failed to Convert from " + Data->TypeData.name() + " to " + typeid(Type).name();
			DebugTrace("%s\n", ErrorMessage.c_str());

			return EnumCast<Type>(*this, std::is_enum<Type>());
		}
	}

	template <class Type>
	class DefaultTypeTraits : public TypeBase {};

	template <class Type>
	class DefaultTypeTraits <Type*> : public TypeBase {
		static Type &Dereference(Type* val) {
			return *val;
		}

		static std::string ToString(Type* val) {
			std::stringstream StringBuilder;
			StringBuilder << "0x" << static_cast<const void*>(val);

			ValueObject Obj { std::ref(*val) };
			ValueObject ObjStr = Obj.InternalCast(typeid(std::string));
			
			if (!ObjStr.IsEmpty()) {
				StringBuilder << " { " << ObjStr.Get<std::string>() << " }";
			}

			return StringBuilder.str();
		}
	public:
		DefaultTypeTraits() {
			AddFunction("operator*", &Dereference);
			AddFunction(typeid(std::string).name(), &ToString);
		}
	};

	template <class Type, size_t Size>
	class DefaultTypeTraits<Type[Size]> : public TypeBase {
		static Type &Item(Type (&val)[Size], size_t index) {
			return val[index];
		}

		static std::string ToString(Type (&val)[Size]) {
			std::stringstream StringBuilder;
			StringBuilder << "0x" << static_cast<void*>(&val) << " { ";

			for (auto& item : val)
				StringBuilder << item << ", ";

			StringBuilder << "}";
			return StringBuilder.str();
		}
	public:
		DefaultTypeTraits() {
			AddFunction("operator[]", &Item);
			AddFunction(typeid(std::string).name(), &ToString);
			AddFunction("size", static_cast<size_t(*)()>([]() { return Size; }));
		}
	};


};

#include "ValueObject.hpp"

/// <summary>指定したメンバを登録します。このマクロは、TypeImpl クラスを継承したクラスの関数内でのみ有効です。</summary>
/// <param name="membername">登録したいメンバの名前</param>
#define AddMemberTable(membername) AddMember(#membername, &MyType::membername)