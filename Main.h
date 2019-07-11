#pragma once

// Main.h	-----------
// ���ʂ̏����Ɗg��
// --------------------

// ���O�錾�i����ŁA<functional>�������ŃC���N���[�h���Ȃ��Ă悭�Ȃ�B�j
namespace std {
	template<class Ty>
	class function;
}

// �񓯊��ōs������������ǉ����܂��B
extern void AddWork(std::function<void()> func);
// �񓯊��ōs������������ǉ����܂��B
extern void AddWorkRef(std::function<void()>& func);
// �񓯊��ōs������������ǉ����܂��B
extern void AddWorkRef(std::function<void()>&& func);

extern int DrawTriangleGraph(int x, int y, int srcx1, int srcy1, int srcx2, int srcy2, int srcx3, int srcy3, int grhandle, int transflag = TRUE);

extern int DrawGlowString(int x, int y, const char text[], unsigned int color, unsigned int glowcolor);