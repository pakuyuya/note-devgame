#pragma once

#include <Windows.h>

class OnFrameExcutable {
public:
    virtual HRESULT onUpdate(double time, double elapsedTime) = 0;
    virtual HRESULT onFrameUpdate(double time, double elapsedTime) = 0;
    virtual HRESULT onRender(double time, double elapsedTime) = 0;
};