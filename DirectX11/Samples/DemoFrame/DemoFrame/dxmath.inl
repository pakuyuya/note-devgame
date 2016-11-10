#include "dxmath.hpp"

__forceinline
FLOAT4::FLOAT4(const float *pf) {
    x = pf[0];
    y = pf[1];
    z = pf[2];
    w = pf[3];
}

__forceinline
FLOAT4::FLOAT4(float nx, float ny, float nz, float nw) {
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

__forceinline
FLOAT4::operator float *() {
    return (float*)(&x);
}

__forceinline
FLOAT4::operator const float *() const {
    return (const float*)(&x);
}
