#pragma once

#include <string>
#include <tchar.h>

// ���g�p�ϐ��̌x���������}�N���ł��B
#define UNUSED(x) {(void)x;}

// ���S�Ƀ��\�[�X��������܂��B
#define safe_release(p) { if(p) { p->Release(); p = NULL; } }

// TCHAR��std::string�ł��B
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

inline void ErrorLog(tstring message) {
    OutputDebugString(message.c_str());
}
