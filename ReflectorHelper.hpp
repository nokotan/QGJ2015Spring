#pragma once

/// @file ReflectorHelper.hpp
/// ReflectorHelper.hpp ------------------------------
/// C++ �ł̃��t���N�V�����������I�ɍs�������i�J�����j
/// --------------------------------------------------

#include <typeinfo>
#include <memory>
#include <map>
#include <sstream>
#include <vector>

#include "DebugHelper.h"

namespace Reflection {
	class TypeBase;

	/// <summary>�C�ӌ^�̒l��ێ����邱�Ƃ��ł���N���X</summary>
	class ValueObject {
		/// <summary>�C�ӌ^�̒l�����ۂɕێ�����N���X�̊��N���X</summary>
		struct ValueHolderBase {
			const std::type_info &TypeData;
		protected:
			ValueHolderBase(const type_info &data);
		};

		template <class Type>
		/// <summary>�C�ӌ^�̒l�����ۂɕێ�����N���X</summary>
		struct ValueHolder : public ValueHolderBase {
			virtual Type &GetData() = 0;
		protected:
			ValueHolder();
		};

		template <class Type>
		/// <summary>�C�ӌ^�̒l�����ۂɕێ�����N���X</summary>
		struct ByValValueHolder : public ValueHolder<Type> {
			ByValValueHolder(Type data);

			Type &GetData() override;
		private:
			Type Data;
		};

		template <class Type>
		/// <summary>�C�ӌ^�̒l�ւ̎Q�Ƃ����ۂɕێ�����N���X</summary>
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
		/// <summary>��̒l��ێ����� ValueObject �I�u�W�F�N�g�����������܂��B</summary>
		ValueObject();

		template <class Type>
		/// <summary>�w�肳�ꂽ�l��ێ����� ValueObject �I�u�W�F�N�g�����������܂��B</summary>
		ValueObject(Type val) :
			Data(ValueHolderBasePtr(new ByValValueHolder<Type>(val))) {
		}

		template <class Type>
		/// <summary>�w�肳�ꂽ�l�ւ̎Q�Ƃ�ێ����� ValueObject �I�u�W�F�N�g�����������܂��B</summary>
		ValueObject(std::reference_wrapper<Type> val) :
			Data(ValueHolderBasePtr(new ByRefValueHolder<Type>(val))) {
		}

		template <class Type, size_t Size>
		/// <summary>�w�肳�ꂽ�z���ێ����� ValueObject �I�u�W�F�N�g�����������܂��B</summary>
		ValueObject(Type (&val)[Size]) :
			Data(ValueHolderBasePtr(new ByRefValueHolder<Type[Size]>(val))) {
		}

		template <class Type>
		/// <summary>���̃I�u�W�F�N�g���ێ����Ă���l���擾���܂��B�^�ϊ��Ɏ��s�����ꍇ�A��O�𑗏o���܂��B</summary>
		Type Get();

		template <class Type>
		/// <summary>���̃I�u�W�F�N�g���ێ����Ă���l���擾���܂��B�^�ϊ��Ɏ��s�����ꍇ�A��O�𑗏o���܂��B</summary>
		explicit operator Type() {
			return Get<Type>();
		}

		/// <summary>�����̒l���w�肵���^�ɕϊ����܂��B</summary>
		ValueObject InternalCast(const type_info& info);

		/// <summary>���̃I�u�W�F�N�g���ێ����Ă���l���󂩂ǂ����m�F���܂��B</summary>
		/// <returns>���̃I�u�W�F�N�g���ێ����Ă���l����Ȃ�� true, �����łȂ���� false</returns>
		bool IsEmpty() const;

		bool CheckType(const type_info& info);

		TypeBase &GetType();
	};

	/// <summary>�֐��ɓn�������̃��X�g</summary>
	typedef std::vector<ValueObject> ArgsList;



	template <class FunctionType>
	/// <summary>���I�Ȋ֐��Ăяo������������N���X</summary>
	class FunctionWrapper;

	/// <summary>���I�Ȋ֐��Ăяo������������N���X�̊��N���X</summary>
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
		/// <summary>ValueObject �̔z��������Ƃ��Ċ֐����Ăяo���܂��B</summary>
		/// <param name="args">�֐��̈����Ƃ��ēn�� ValueObject �̔z��</param>
		virtual ValueObject Invoke(ArgsList& args) = 0;
		/// <summary>�����o�ϐ��̒l���������݂܂��B</summary>
		/// <param name="value">�����o�ϐ��̐V�����l��ێ����� ValueObject �I�u�W�F�N�g</param>
		virtual void SetValue(ValueObject& Obj, ValueObject& value) { };
		/// <summary>�����o�ϐ��̒l��ǂݏo���܂��B</summary>
		/// <returns>�����o�ϐ��̒l��ێ����� ValueObject �I�u�W�F�N�g</returns>
		virtual ValueObject GetValue(ValueObject& Obj) { return ValueObject(); };
		/// <summary>���� FunctionWrapperBase �I�u�W�F�N�g���ێ����郊�\�[�X��������܂��B</summary>
		virtual ~FunctionWrapperBase() = default;
	};

	template <class RetType, class ...Args>
	/// <summary>���I�Ȋ֐��Ăяo������������N���X</summary>
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
		/// <summary>�w�肳�ꂽ�֐����Ăяo�� FunctionWrapper �I�u�W�F�N�g�����������܂��B</summary>
		/// <param name="MyFunc">���� FunctionWrapper �I�u�W�F�N�g���Ăяo���֐��I�u�W�F�N�g</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (sizeof...(Args) > args.size()) {
				DebugTrace("OutOfRangeException : �����̐�������܂���B\n�����̐�: %d, �K�v�Ȑ�: %d", args.size(), sizeof...(Args));
				return ValueObject();
			} else
#endif
			{
				return DoCall(*this, args, IndexType());
			}
		}
	};

	template <class RetType, class OwnerType, class ...Args>
	/// <summary>���I�Ȋ֐��Ăяo������������N���X</summary>
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
		/// <summary>�w�肳�ꂽ�֐����Ăяo�� FunctionWrapper �I�u�W�F�N�g�����������܂��B</summary>
		/// <param name="MyFunc">���� FunctionWrapper �I�u�W�F�N�g���Ăяo���֐��I�u�W�F�N�g</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (sizeof...(Args) > args.size()) {
				DebugTrace("OutOfRangeException : �����̐�������܂���B\n�����̐�: %d, �K�v�Ȑ�: %d", args.size(), sizeof...(Args));
				return ValueObject();
			} else
#endif
			{
				return DoCall(*this, args, IndexType());
			}
		}
	};

	template <class RetType, class OwnerType>
	/// <summary>���I�Ȋ֐��Ăяo������������N���X</summary>
	class FunctionWrapper<RetType(OwnerType::*)> : public FunctionWrapperBase{
		typedef RetType(OwnerType::*MyFunction);
		MyFunction Func;
	public:
		/// <summary>�w�肳�ꂽ�֐����Ăяo�� FunctionWrapper �I�u�W�F�N�g�����������܂��B</summary>
		/// <param name="MyFunc">���� FunctionWrapper �I�u�W�F�N�g���Ăяo���֐��I�u�W�F�N�g</param>
		FunctionWrapper(MyFunction MyFunc) :
			Func(MyFunc) {
		}

		ValueObject Invoke(ArgsList& args) override {
#if defined(_DEBUG)
			if (1 > args.size()) {
				DebugTrace("OutOfRangeException : �����̐�������܂���B\n�����̐�: %d, �K�v�Ȑ�: 1", args.size());
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



	/// <summary>���t���N�V�����p�̌^����ێ�����N���X</summary>
	class TypeBase {
		typedef std::shared_ptr<FunctionWrapperBase> FunctionWrapperBasePtr;

		std::map<std::string, FunctionWrapperBasePtr> FunctionList;
		std::map<std::string, FunctionWrapperBasePtr> MemberList;
	public:
		/// <summary>�w�肵�����O�� FunctionWrapperBase �I�u�W�F�N�g��o�^���܂��B</summary>
		/// <param name="name">�o�^���閼�O</param>
		/// <param name="func">�o�^����  FunctionWrapperBase �I�u�W�F�N�g�ւ̃|�C���^�B�����Ŏ����I�ɉ���������s���܂��B</param>
		void AddFunction(std::string name, FunctionWrapperBase *funcobj) {
			FunctionList[name] = FunctionWrapperBasePtr(funcobj);
		}

		template <class RetType, class ...ArgsType>
		/// <summary>�w�肵�����O�ŃO���[�o���֐���o�^���܂��B</summary>
		/// <param name="name">�o�^���閼�O</param>
		/// <param name="func">�o�^����O���[�o���֐�</param>
		void AddFunction(std::string name, RetType (*func)(ArgsType...)) {
			FunctionList[name] = FunctionWrapperBasePtr(new FunctionWrapper<RetType(*)(ArgsType...)>(func));
		}

		template <class Type, class RetType, class ...ArgsType>
		/// <summary>�w�肵�����O�Ń����o�֐���o�^���܂��B</summary>
		/// <param name="name">�o�^���閼�O</param>
		/// <param name="func">�o�^���郁���o�֐�</param>
		void AddMember(std::string name, RetType(Type::*func)(ArgsType...)) {
			FunctionList[name] = FunctionWrapperBasePtr(new FunctionWrapper<RetType(Type::*)(ArgsType...)>(func));
		}

		template <class OwnerType, class Type>
		/// <summary>�w�肵�����O�Ń����o�ϐ���o�^���܂��B</summary>
		/// <param name="name">�o�^���閼�O</param>
		/// <param name="func">�o�^���郁���o�ϐ�</param>
		void AddMember(std::string name, Type(OwnerType::*member)) {
			MemberList[name] = FunctionWrapperBasePtr(new FunctionWrapper<Type(OwnerType::*)>(member));
		}
	
		/// <summary>�w�肳�ꂽ���O�œo�^����Ă���֐����Ăяo���܂��B</summary>
		/// <param name="name">�o�^���Ă������Ăяo���֐��̖��O</param>
		/// <param name="args">�֐��ɓn�������B�����o�֐��̏ꍇ�́A��P�����Ƃ��āAthis �ɑ���������̂��K�v�ł��B</param>
		/// <returns>�֐��̖߂�l�����b�v���� ValueObject �I�u�W�F�N�g</returns>
		ValueObject InvokeMember(const char name[], ArgsList& args);

		/// <summary>�w�肵���I�u�W�F�N�g�̃����o�̒l��W���o�͂ɓf���o���܂��B</summary>
		/// <param name="name">�m�F�����������o�ϐ��̓o�^��</param>
		/// <param name="Obj">�m�F�����������o�ϐ���ێ����Ă���I�u�W�F�N�g</param>
		void DumpMemberValue(const char name[], ValueObject Obj);

		/// <summary>�w�肵���I�u�W�F�N�g�̃����o�̒l��ǂݏo���܂��B</summary>
		/// <param name="name">�l��ǂݏo�����������o�ϐ��̓o�^��</param>
		/// <param name="Obj">�m�F�����������o�ϐ���ێ����Ă���I�u�W�F�N�g</param>
		/// <returns>�����o�ϐ��̒l�����b�v���� ValueObject �I�u�W�F�N�g</returns>
		ValueObject GetMemberValue(const char name[], ValueObject Obj);

		/// <summary>�w�肵���I�u�W�F�N�g�̃����o�̒l���X�V���܂��B</summary>
		/// <param name="name">�l���X�V�����������o�ϐ��̓o�^��</param>
		/// <param name="Obj">�l���X�V�����������o�ϐ���ێ����Ă���I�u�W�F�N�g</param>
		/// <param name="value">�����o�ϐ��̐V�����l</param>
		void SetMemberValue(const char name[], ValueObject Obj, ValueObject value);
	};

	template <class ThisType>
	/// <summary>�����o�̒ǉ����ȒP�ɂ���@�\��񋟂��܂��B</summary>
	class TypeImpl : public TypeBase {
	protected:
		typedef ThisType MyType;
	};

	void RegisterType(const std::type_info& typeinfo, TypeBase& obj);

	/// <summary>�w�肳�ꂽ�^��\�������񂩂�A�^�����擾���܂��B</summary>
	/// <param name="name">�w�肳�ꂽ�^��\��������</param>
	/// <returns>�w�肳�ꂽ�^��\��������Ɋ֘A�t����ꂽ�^���</returns>
	TypeBase &GetType(const char name[]);

	/// <summary>�w�肳�ꂽ�^��\�� std::typeinfo �I�u�W�F�N�g����A�^�����擾���܂��B</summary>
	/// <param name="typeinfo">�w�肳�ꂽ�^��\�� std::typeinfo �I�u�W�F�N�g</param>
	/// <returns>�w�肳�ꂽ�^��\�� std::typeinfo �I�u�W�F�N�g�Ɋ֘A�t����ꂽ�^���</returns>
	TypeBase &GetType(const std::type_info& typeinfo);

	template <class Type>
	/// <summary>�w�肳�ꂽ�^����A�^�����擾���܂��B</summary>
	/// <param name="Type">�^�����擾����^</param>
	/// <returns>�w�肳�ꂽ�^�Ɋ֘A�t����ꂽ�^���</returns>
	TypeBase &GetType() {
		return GetType(typeid(Type));
	};

	template <class Type>
	/// <summary>���̃I�u�W�F�N�g���ێ����Ă���l���擾���܂��B�^�ϊ��Ɏ��s�����ꍇ�A��O�𑗏o���܂��B</summary>
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

/// <summary>�w�肵�������o��o�^���܂��B���̃}�N���́ATypeImpl �N���X���p�������N���X�̊֐����ł̂ݗL���ł��B</summary>
/// <param name="membername">�o�^�����������o�̖��O</param>
#define AddMemberTable(membername) AddMember(#membername, &MyType::membername)