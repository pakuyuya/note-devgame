#pragma once

struct FLOAT4 {
    float x;
    float y;
    float z;
    float w;

    FLOAT4(const float* pf);
    FLOAT4(float nx, float ny, float nz, float nw);
    operator float* () ;
    operator const float* () const;
};

#include "dxmath.inl"