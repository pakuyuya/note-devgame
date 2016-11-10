#pragma once

#include <string>
#include <tchar.h>

// 未使用変数の警告を消すマクロです。
#define UNUSED(x) {(void)x;}

// 安全にリソースを解放します。
#define safe_release(p) { if(p) { p->Release(); p = NULL; } }

// TCHAR版std::stringです。
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

inline void ErrorLog(tstring message) {
    OutputDebugString(message.c_str());
}
