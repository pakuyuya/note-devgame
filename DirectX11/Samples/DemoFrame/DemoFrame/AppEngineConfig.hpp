#pragma once

#include <tchar.h>
#include <Windows.h>
#include <DirectXColors.h>
#include <d3d11_2.h>
#include "dxmath.hpp"

#include "util.hpp"

/**
* AppEngineConfig.
*/
struct AppEngineConfig {
    UINT        SwapChainBufferCount = 2;                                  // スワップチェインの数
    DXGI_FORMAT SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;         // スワップチェインのバッファーフォーマット
    DXGI_FORMAT DepthStencilTextureFormat = DXGI_FORMAT_R24G8_TYPELESS;         // 深度ステンシルバッファーのテクスチャフォーマット
    DXGI_FORMAT DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;      // 深度ステンシルバッファービューのテフォーマット
    DXGI_FORMAT DepthStencilResourceFromat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; // 深度ステンシルリソースビューのフォーマット

    UINT multiSampleCount = 2;      //<! マルチサンプリング数
    UINT multiSampleQuality = 0;    //<! マルチサンプリング品質

    UINT wndWidth = 640;  // ウィンドウ幅です
    UINT wndHeight = 480; // ウィンドウ高さです

    tstring title = _T("");  // ウィンドウタイトルです

    UINT refreshRate = 60; // 画面のリフレッシュレート
    UINT FPSlimit = 60; // FPSの最大値

    FLOAT4 clearColor = DirectX::Colors::CornflowerBlue;
};
