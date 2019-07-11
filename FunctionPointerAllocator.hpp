#pragma once

#include <functional>

// FunctionPointerAllocator.hpp ---------------------
// �����o�֐��ȂǂɐÓI�֐��|�C���^�����蓖�Ă�N���X
// --------------------------------------------------





// ICallable ----------------------------------------------------------------
// �֐��I�u�W�F�N�g���������܂��B
// (�e���v���[�g�Ɖ��z�֐��������Ɏg�p�ł��Ȃ��̂ŃR�����g�A�E�g���Ă��܂��B)
// --------------------------------------------------------------------------

// template<class Ret, class ...Signiture>
// // �֐��I�u�W�F�N�g���������܂��B
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
// �ÓI�֐��|�C���^��񋟂���N���X
// --------------------------------

template<class Ret, class ...Signiture>
// �ÓI�֐��|�C���^��񋟂���N���X
struct Caller {
	template <std::function<Ret(Signiture...)> &funcobj>
	struct Instance {
		static Ret DoCall(Signiture... args) {
			return funcobj(args...);
		}
	};
};





// StaticFunctionPointerAllocator -------------------
// �����o�֐��ȂǂɐÓI�֐��|�C���^�����蓖�Ă�N���X
// --------------------------------------------------

template<class Ty>
// �����o�֐��ȂǂɐÓI�֐��|�C���^�����蓖�Ă�N���X
struct StaticFunctionPointerAllocator;

template<class Ret, class ...Signiture>
// �����o�֐��ȂǂɐÓI�֐��|�C���^�����蓖�Ă�N���X
struct StaticFunctionPointerAllocator <Ret(Signiture...)> {
	typedef void (*PtrType)(Signiture...);
	typedef std::function<Ret(Signiture...)> FuncType;

	// �����Ɋ��蓖�Ă���ÓI�֐��|�C���^�̐�
	static const int Capacity = 5;
	
	// �w�肵���֐��ɐÓI�֐��|�C���^�����蓖�Ă܂��B
	static PtrType Allocate(FuncType func) {
		for (int i = 0; i < Capacity; i++) {
			if ((*List[i]) == nullptr) {
				(*List[i]) = func;
				return FuncPtr[i];
			}
		}
	
		return nullptr;
	}

	// �擾�����ÓI�֐��|�C���^��������܂��B
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

// �ÓI�����o�������� -----

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