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
    UINT        SwapChainBufferCount = 2;                                  // �X���b�v�`�F�C���̐�
    DXGI_FORMAT SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;         // �X���b�v�`�F�C���̃o�b�t�@�[�t�H�[�}�b�g
    DXGI_FORMAT DepthStencilTextureFormat = DXGI_FORMAT_R24G8_TYPELESS;         // �[�x�X�e���V���o�b�t�@�[�̃e�N�X�`���t�H�[�}�b�g
    DXGI_FORMAT DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;      // �[�x�X�e���V���o�b�t�@�[�r���[�̃e�t�H�[�}�b�g
    DXGI_FORMAT DepthStencilResourceFromat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; // �[�x�X�e���V�����\�[�X�r���[�̃t�H�[�}�b�g

    UINT multiSampleCount = 2;      //<! �}���`�T���v�����O��
    UINT multiSampleQuality = 0;    //<! �}���`�T���v�����O�i��

    UINT wndWidth = 640;  // �E�B���h�E���ł�
    UINT wndHeight = 480; // �E�B���h�E�����ł�

    tstring title = _T("");  // �E�B���h�E�^�C�g���ł�

    UINT refreshRate = 60; // ��ʂ̃��t���b�V�����[�g
    UINT FPSlimit = 60; // FPS�̍ő�l

    FLOAT4 clearColor = DirectX::Colors::CornflowerBlue;
};
