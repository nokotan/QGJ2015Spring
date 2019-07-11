#include "ReflectorHelper.hpp"

namespace Reflection {

	ValueObject TypeBase::InvokeMember(const char name[], ArgsList& args) {
		std::string namestr{ name };
		auto&& Item = FunctionList.find(namestr);

		if (Item != FunctionList.end()) {
			return Item->second->Invoke(args);
		}

		DebugTrace("KeyNotFoundException: �w�肳�ꂽ�֐��̓��t���N�^�ɓo�^����Ă��܂���B\n\t�Ăяo�����Ƃ����֐���:%s\n", name);
		return ValueObject(nullptr);
	};

	void TypeBase::DumpMemberValue(const char name[], ValueObject Obj) {
		ArgsList Args { Obj };

		if (name[0] == '*') {
			for (auto& item : MemberList) {
				printf("%s = %s\n", item.first.c_str(), (static_cast<std::string>(item.second->Invoke(Args))).c_str());
			}
		} else {
			auto&& iter = MemberList.find(std::string(name));

			if (iter != MemberList.end()) {
				printf("%s = %s\n", name, (static_cast<std::string>(iter->second->Invoke(Args))).c_str());
			} else {
				DebugTrace("KeyNotFoundException: �w�肳�ꂽ�ϐ��̓��t���N�^�ɓo�^����Ă��܂���B\n\t�ǂݎ�낤�Ƃ����ϐ���:%s\n", name);
			}
		}
	}

	ValueObject TypeBase::GetMemberValue(const char name[], ValueObject Obj) {
		std::string namestr{ name };
		auto&& Item = MemberList.find(namestr);

		if (Item != MemberList.end()) {
			return Item->second->GetValue(Obj);
		}

		DebugTrace("KeyNotFoundException: �w�肳�ꂽ�ϐ��̓��t���N�^�ɓo�^����Ă��܂���B\n\t�ǂݎ�낤�Ƃ����ϐ���:%s\n", name);
		return ValueObject(nullptr);
	};

	void TypeBase::SetMemberValue(const char name[], ValueObject Obj, ValueObject value) {
		std::string namestr{ name };
		auto&& Item = MemberList.find(namestr);

		if (Item != MemberList.end()) {
			Item->second->SetValue(Obj, value);
			return;
		}

		DebugTrace("KeyNotFoundException: �w�肳�ꂽ�ϐ��̓��t���N�^�ɓo�^����Ă��܂���B\n\t�����������Ƃ����ϐ���:%s\n", name);
	};
}