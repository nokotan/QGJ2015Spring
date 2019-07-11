#pragma once

// Main.h	-----------
// 共通の処理と拡張
// --------------------

// 事前宣言（これで、<functional>をここでインクルードしなくてよくなる。）
namespace std {
	template<class Ty>
	class function;
}

// 非同期で行いたい処理を追加します。
extern void AddWork(std::function<void()> func);
// 非同期で行いたい処理を追加します。
extern void AddWorkRef(std::function<void()>& func);
// 非同期で行いたい処理を追加します。
extern void AddWorkRef(std::function<void()>&& func);

extern int DrawTriangleGraph(int x, int y, int srcx1, int srcy1, int srcx2, int srcy2, int srcx3, int srcy3, int grhandle, int transflag = TRUE);

extern int DrawGlowString(int x, int y, const char text[], unsigned int color, unsigned int glowcolor);