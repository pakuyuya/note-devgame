#include "AppEngine.hpp"

#include <tchar.h>

namespace {
    const TCHAR const * WNDCLASSNAME = _T("appEngineWnd");
}

/**
 * @inherit
 */
AppEngine::AppEngine() {
    
}

AppEngine::~AppEngine() {
    termD3DWindow();
}

/**
 * @inherit
 */
HRESULT AppEngine::initD3DWindow() {
    int ret;

    ret = initWnd();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    ret = initD3D();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    ret = initCompornent();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    return AE_SUCCESS;
}

/**
* @inherit
*/
HRESULT AppEngine::termD3DWindow() {
    int ret;

    ret = termCompornent();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    ret = termD3D();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    ret = termWnd();
    if (FAILED(ret)) {
        ErrorLog(getLastError());
        return ret;
    }

    return AE_SUCCESS;
}

HRESULT AppEngine::intoMainLoop() {
    MSG msg = { 0 };

    while (msg.message != WM_QUIT) {
        // �E�B���h�E���b�Z�[�W�̎擾�����݂�
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // �������b�Z�[�W������΃E�B���h�E����̏���
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // ���b�Z�[�W���Ȃ����̂݃Q�[�����L�̏���
            auto time = m_timer.snap();
            auto elapse = time.time - m_lastFrameExecuteTime;
            update(time.time, elapse);

            if (IsAllowRender()) {
                updateOnFrame(time.time, elapse);
                render(time.time, elapse);

                resetAllowRender();
            }
        }
    }

    return S_OK;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp) {
    // TODO: �Q�l�ɂ����\�[�X�́A�����ŃL�[�{�[�h�E�}�E�X�̃C�x���g���t�b�N���Ă���

    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg) {
    case WM_PAINT: // Windows�W���̕`��C�x���g�B���������Ȃ�
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY: // �E�B���h�E�Y�̔j�����N�G�X�g
        PostQuitMessage(0);
        break;

    case WM_SIZE: // �E�B���h�E�T�C�Y�̕ύX
        // TODO: ���T�C�Y�ʒm�̎���
        break;
    }

    // Windows�K��̃��b�Z�[�W���������{
    return DefWindowProc(hWnd, uMsg, wp, lp);
}


HRESULT AppEngine::initWnd() {
    // �v���Z�X�Ɍ��т����C���X�^���X�n���h���擾
    auto hInst = GetModuleHandle(nullptr);
    if (!hInst) {
        ErrorLog(_T("fialed GetModuleHandle"));
        return AE_ERR_INITWND;
    }

    // �g���E�B���h�E�N���X
    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = wndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WNDCLASSNAME;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        ErrorLog(_T("failed RegisterClassEx"));
        return AE_ERR_INITWND;
    }

    m_hInst = hInst;

    // �E�B���h�E�\���ʒu���v�Z
    RECT rc = { 0, 0, m_config.wndWidth, m_config.wndHeight};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindow(
        WNDCLASSNAME,
        m_config.title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (rc.right - rc.left),
        (rc.bottom - rc.top),
        NULL,
        NULL,
        hInst,
        NULL);

    if (!m_hWnd) {
        ErrorLog(_T("failed CreateWindow"));
        return AE_ERR_INITWND;
    }

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);

    SetFocus(m_hWnd);

    // �E�B���h�E�T�C�Y�擾�A�v���p�e�B�X�V
    RECT adjrc;
    GetWindowRect(m_hWnd, &adjrc);

    auto w = m_config.wndWidth = adjrc.right - adjrc.left;
    auto h = m_config.wndHeight = adjrc.bottom - adjrc.top;

    m_aspectRate = static_cast<FLOAT>(w) / static_cast<FLOAT>(h);

    return AE_SUCCESS;
}

HRESULT AppEngine::initD3D() {
    HRESULT hr;

    // Device�����SwapChain�쐬�̂��߂̏���

    const D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_SOFTWARE,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    const D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC sd;

    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = m_config.SwapChainBufferCount;
    sd.BufferDesc.Width = m_config.wndWidth;
    sd.BufferDesc.Height = m_config.wndHeight;
    sd.BufferDesc.Format = m_config.SwapChainBufferFormat;
    sd.BufferDesc.RefreshRate.Numerator = m_config.refreshRate;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    sd.OutputWindow = m_hWnd;
    sd.SampleDesc.Count = m_config.multiSampleCount;
    sd.SampleDesc.Quality = m_config.multiSampleQuality;
    sd.Windowed = TRUE;

    // Device�����SwapChain�쐬
    for (auto driverType : driverTypes) {
        UINT createDeviseFlg = 0;
#if defined(DEBUG) || defined(_DEBUG)
        createDeviseFlg |= D3D11_CREATE_DEVICE_DEBUG;;
#endif
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            driverType,
            nullptr,
            createDeviseFlg,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            &m_featureLevel,
            &m_pDeviceContext
        );

        if (SUCCEEDED(hr)) {
            // �ŏ��ɂ������������DriverType��I��
            m_driverType = driverType;
            break;
        }
    }

    if (FAILED(hr)) {
        DWORD dw = GetLastError();

        ErrorLog(_T("failed D3D11CreateDeviceAndSwapChain"));
        return AE_ERR_INITD3D;
    }

    // �����_�[�^�[�Q�b�g�쐬
    if (FAILED(initRenderTarget())) {
        ErrorLog(_T("failed InitRenaderRarget"));
        return AE_ERR_INITD3D;
    }

    return AE_SUCCESS;
}
HRESULT AppEngine::initRenderTarget() {
    // �o�b�N�o�b�t�@���퓗
    HRESULT hr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pRenderTargetTexture);
    if (FAILED(hr)) {
        ErrorLog(_T("failed IDXGISwapChain->GetBuffer()"));
        return hr;
    }

    // �����_�[�^�[�Q�b�g�r���[���쐬
    hr = m_pDevice->CreateRenderTargetView(m_pRenderTargetTexture, nullptr, &m_pRenderTargetView);
    if (FAILED(hr)) {
        ErrorLog(_T("failed IDXGISwapChain->GetBuffer()"));
        safe_release(m_pRenderTargetTexture);
        return hr;
    }
    
    // �V�F�[�_���\�[�X�r���[���쐬
    hr = m_pDevice->CreateShaderResourceView(m_pRenderTargetTexture, nullptr, &m_pRenderTargetShaderResourceView);
    if (FAILED(hr)) {
        ErrorLog(_T("failed IDXGISwapChain->GetBuffer()"));
        safe_release(m_pRenderTargetView);
        safe_release(m_pRenderTargetTexture);
        return hr;
    }

    return AE_SUCCESS;
}

HRESULT AppEngine::initDepthStencil() {
    HRESULT hr;

    // �e�N�X�`���t�H�[�}�b�g������
    DXGI_FORMAT inputFormat = m_config.DepthStencilTextureFormat;

    DXGI_FORMAT textureFormat = inputFormat;
    DXGI_FORMAT resourceFormat = inputFormat;

    switch (textureFormat) {
    case DXGI_FORMAT_D16_UNORM:
        textureFormat = DXGI_FORMAT_R16_TYPELESS;
        resourceFormat = DXGI_FORMAT_R16_UNORM;
        break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
        resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        break;
    case DXGI_FORMAT_D32_FLOAT:
        textureFormat = DXGI_FORMAT_R32_TYPELESS;
        resourceFormat = DXGI_FORMAT_R32_FLOAT;
        break;
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
        resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        break;
    }

    // �[�x�X�e���V���o�b�t�@�̍쐬
    D3D11_TEXTURE2D_DESC td = { 0 };
    td.Width = m_config.wndWidth;
    td.Height = m_config.wndHeight;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = textureFormat;
    td.SampleDesc.Count = m_config.multiSampleCount;
    td.SampleDesc.Quality = m_config.multiSampleQuality;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;

    hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthStencilTexture);
    if (FAILED(hr)) {
        ErrorLog(_T("failed CreateTexture2D"));
        return hr;
    }

    // �[�x�X�e���V���r���[�̍쐬
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = inputFormat;
    // �}���`�T���v�����O����
    if (m_config.multiSampleCount == 0)
    {
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Texture2D.MipSlice = 0;
    }
    // �}���`�T���v�����O�Ȃ�
    else
    {
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    }

    // texture��ArraySize > 1�̏ꍇ�͈ȉ�
    //// �}���`�T���v�����O�����̏ꍇ.
    //if (m_config.multiSampleCount == 0)
    //{
    //    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    //    dsvd.Texture2DArray.ArraySize = ArraySize;
    //    dsvd.Texture2DArray.FirstArraySlice = 0;
    //    dsvd.Texture2DArray.MipSlice = 0;
    //}
    //// �}���`�T���v�����O�L��̏ꍇ.
    //else
    //{
    //    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
    //    dsvd.Texture2DMSArray.ArraySize = ArraySize;
    //    dsvd.Texture2DMSArray.FirstArraySlice = 0;
    //}

    hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, &dsvd, &m_pDepthStencilView);
    if (FAILED(hr)) {
        ErrorLog(_T("failed CreateDepthStencilView"));
        safe_release(m_pDepthStencilTexture);

        return hr;
    }

    // �V�F�[�_���\�[�X�r���[�̍쐬
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = inputFormat;

    hr = m_pDevice->CreateShaderResourceView(m_pDepthStencilTexture, &srvd, &m_pDepthStencilShaderResourceView);
    if (FAILED(hr)) {
        ErrorLog(_T("failed CreateDepthStencilView"));
        safe_release(m_pDepthStencilView);
        safe_release(m_pDepthStencilTexture);

        return hr;
    }

    return AE_SUCCESS;
}

HRESULT AppEngine::initCompornent() {
    m_timer.reset();
    auto time = m_timer.snap();
    m_lastFrameExecuteTime = time.time;

    return AE_SUCCESS;
}

HRESULT AppEngine::termCompornent() {
    // Do nothing.
    return AE_SUCCESS;
}
HRESULT AppEngine::termD3D() {
    if (m_pDeviceContext) {
        m_pDeviceContext->ClearState();
        m_pDeviceContext->Flush();
    }

    safe_release(m_pDepthStencilShaderResourceView);
    safe_release(m_pDepthStencilView);
    safe_release(m_pDepthStencilTexture);

    safe_release(m_pRenderTargetShaderResourceView);
    safe_release(m_pDepthStencilView);
    safe_release(m_pDepthStencilTexture);

    safe_release(m_pSwapChain);
    safe_release(m_pDeviceContext);
    safe_release(m_pDevice);

    return AE_SUCCESS;
}
HRESULT AppEngine::termWnd() {
    // Do nothing.
    return AE_SUCCESS;
}


void AppEngine::update(double time, double elapsedTime) {
    UNUSED(elapsedTime);

    if (time - m_lastFrameExecuteTime >= 1.0 / static_cast<double>(m_config.FPSlimit)) {
        m_frameTimePassed = true;
    }

    m_units.onUpdate(time, elapsedTime);
}

void AppEngine::updateOnFrame(double time, double elapsedTime) {
    UNUSED(time);
    UNUSED(elapsedTime);
    
    m_units.onFrameUpdate(time, elapsedTime);
}

void AppEngine::render(double time, double elapsedTime) {
    UNUSED(time);
    UNUSED(elapsedTime);

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_config.clearColor);

    m_units.onRender(time, elapsedTime);
        
    m_pSwapChain->Present(0, 0);
}