#pragma once

#include <Windows.h>
#include <d3d11_2.h>

#include "util.hpp"

#include "Timer.hpp"
#include "AppEngineConfig.hpp"

namespace {
    const int AE_SUCCESS = S_OK;
    const int AE_ERR_INITWND = -1;
    const int AE_ERR_INITD3D = -2;
}

/**
 * D3D11�Ǘ����̃E�B���h�E�Ɗe�}�l�[�W���[�����N���X�B
 */
class AppEngine {

private:
    // --- �R���|�[�l���g ---
    Timer m_timer;

	// --- �����o�[ ---

	HWND m_hWnd;       //!< �E�B���h�E�n���h��
    HINSTANCE m_hInst; //!< �C���X�^���X�n���h��

    D3D_DRIVER_TYPE m_driverType;     //!< �h���C�o�[�^�C�v
    D3D_FEATURE_LEVEL m_featureLevel; //!< �@�\���x��

	AppEngineConfig m_config; //!< �e�ݒ�

    ID3D11Device* m_pDevice;                                     //!< �f�o�C�X
    ID3D11DeviceContext* m_pDeviceContext;                       //!< �f�o�C�X�R���e�L�X�g
    IDXGISwapChain* m_pSwapChain;                                //!< �X���b�v�`�F�C��

    ID3D11RenderTargetView*   m_pRenderTargetView = nullptr;              //!< �����_�[�^�[�Q�b�g�r���[
    ID3D11Texture2D*          m_pRenderTargetTexture = nullptr;             //!< �����_�[�^�[�Q�b�g�̃o�b�t�@
    ID3D11ShaderResourceView* m_pRenderTargetShaderResourceView = nullptr; //<! �����_�[�^�[�Q�b�g�̃V�F�[�_�[���\�[�X

    ID3D11DepthStencilView*   m_pDepthStencilView = nullptr;               //!< �[�x�X�e���V���r���[
    ID3D11Texture2D*          m_pDepthStencilTexture = nullptr;             //!< �[�x�X�e���V���̃o�b�t�@
    ID3D11ShaderResourceView* m_pDepthStencilShaderResourceView = nullptr; //<! �[�x�X�e���V���̃V�F�[�_�[���\�[�X

    FLOAT m_aspectRate; //!< �A�X�y�N�g��

    tstring m_lastError; //!< �Ō�̃G���[���b�Z�[�W

    double m_lastFrameExecuteTime; //!< �Ō��frame��������������
    bool m_frameTimePassed = true; //!<
	
public:
    /**
     * �R���X�g���N�^�ł��B
     */
    AppEngine();

    /**
     * �ݒ�l�t���R���X�g���N�^�ł��B
     */
    AppEngine(AppEngineConfig& config);

private:
    /**
     * ���̃R���X�g���N�^���֎~���܂�
     */
    AppEngine(const AppEngine&);
    bool operator= (const AppEngine&);

public:
    ~AppEngine();

public:
    /**
     * �E�B���h�E���������̂�DirectX�������܂��B
     * @return SUCCEED�}�N���Ŕ��f�ł��鏈�����ʃR�[�h
     */
    HRESULT initD3DWindow();

    /**
     * �E�B���h�E��j�����܂��B 
     * @return SUCCEED�}�N���Ŕ��f�ł��鏈�����ʃR�[�h
     */
    HRESULT termD3DWindow();

    /**
     * �E�B���h�E�̃��C�����[�v�����s���܂��B�E�B���h�E���I������܂ŏ�����߂��܂���B
     * @return SUCCEED�}�N���Ŕ��f�ł��鏈�����ʃR�[�h
     */
    HRESULT intoMainLoop();

public:
    /**
     * ���݂̐ݒ���擾���܂��B
     */
    inline AppEngineConfig* getConfig() {
        return &m_config;
    }

    /**
     * �Ō�̃G���[���b�Z�[�W���擾���܂��B
     */
    inline tstring getLastError() {
        return m_lastError;
    }

private:
    LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

private:
    /** WindowsAPI����ăE�B���h�E���������A�\�����܂��B */
    HRESULT initWnd();
    /** DirectX�̋��ʃR���|�[�l���g�����������A�E�B���h�E��DirectX�ɓK�����܂��B */
    HRESULT initD3D();

    HRESULT initRenderTarget();
    HRESULT initDepthStencil();

    /** ���̑��R���|�[�l���g�����������܂��B */
    HRESULT initCompornent();

    /** ���̑��R���|�[�l���g��j�����܂��B */
    HRESULT termCompornent();
    /** DirectX�̋��ʃR���|�[�l���g��j�����܂��B */
    HRESULT termD3D();
    /** �E�B���h�E��j�����܂��B */
    HRESULT termWnd();

    /** �����b�Z�[�W�������̍X�V�������s���܂��B */
    void update(double time, double elapsedTime);
    /** 1�t���[�����Ƃ̍X�V�������s���܂��B */
    void updateOnFrame(double time, double elapsedTime);
    /** 1�t���[�����Ƃ̕`����s���܂��B */
    void render(double time, double elapsedTime);

private:
    bool IsAllowRender() {
        return m_frameTimePassed;
    }
    void resetAllowRender() {
        m_frameTimePassed = false;
    }
};