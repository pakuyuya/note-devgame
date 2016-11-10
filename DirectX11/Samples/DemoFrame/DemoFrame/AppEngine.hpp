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
 * D3D11管理下のウィンドウと各マネージャーを持つクラス。
 */
class AppEngine {

private:
    // --- コンポーネント ---
    Timer m_timer;

	// --- メンバー ---

	HWND m_hWnd;       //!< ウィンドウハンドル
    HINSTANCE m_hInst; //!< インスタンスハンドル

    D3D_DRIVER_TYPE m_driverType;     //!< ドライバータイプ
    D3D_FEATURE_LEVEL m_featureLevel; //!< 機能レベル

	AppEngineConfig m_config; //!< 各設定

    ID3D11Device* m_pDevice;                                     //!< デバイス
    ID3D11DeviceContext* m_pDeviceContext;                       //!< デバイスコンテキスト
    IDXGISwapChain* m_pSwapChain;                                //!< スワップチェイン

    ID3D11RenderTargetView*   m_pRenderTargetView = nullptr;              //!< レンダーターゲットビュー
    ID3D11Texture2D*          m_pRenderTargetTexture = nullptr;             //!< レンダーターゲットのバッファ
    ID3D11ShaderResourceView* m_pRenderTargetShaderResourceView = nullptr; //<! レンダーターゲットのシェーダーリソース

    ID3D11DepthStencilView*   m_pDepthStencilView = nullptr;               //!< 深度ステンシルビュー
    ID3D11Texture2D*          m_pDepthStencilTexture = nullptr;             //!< 深度ステンシルのバッファ
    ID3D11ShaderResourceView* m_pDepthStencilShaderResourceView = nullptr; //<! 深度ステンシルのシェーダーリソース

    FLOAT m_aspectRate; //!< アスペクト比

    tstring m_lastError; //!< 最後のエラーメッセージ

    double m_lastFrameExecuteTime; //!< 最後にframeを処理した時間
    bool m_frameTimePassed = true; //!<
	
public:
    /**
     * コンストラクタです。
     */
    AppEngine();

    /**
     * 設定値付きコンストラクタです。
     */
    AppEngine(AppEngineConfig& config);

private:
    /**
     * このコンストラクタを禁止します
     */
    AppEngine(const AppEngine&);
    bool operator= (const AppEngine&);

public:
    ~AppEngine();

public:
    /**
     * ウィンドウを初期化のちDirectX化させます。
     * @return SUCCEEDマクロで判断できる処理結果コード
     */
    HRESULT initD3DWindow();

    /**
     * ウィンドウを破棄します。 
     * @return SUCCEEDマクロで判断できる処理結果コード
     */
    HRESULT termD3DWindow();

    /**
     * ウィンドウのメインループを実行します。ウィンドウが終了するまで処理を戻しません。
     * @return SUCCEEDマクロで判断できる処理結果コード
     */
    HRESULT intoMainLoop();

public:
    /**
     * 現在の設定を取得します。
     */
    inline AppEngineConfig* getConfig() {
        return &m_config;
    }

    /**
     * 最後のエラーメッセージを取得します。
     */
    inline tstring getLastError() {
        return m_lastError;
    }

private:
    LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

private:
    /** WindowsAPIを介してウィンドウを初期化、表示します。 */
    HRESULT initWnd();
    /** DirectXの共通コンポーネントを初期化し、ウィンドウをDirectXに適応します。 */
    HRESULT initD3D();

    HRESULT initRenderTarget();
    HRESULT initDepthStencil();

    /** その他コンポーネントを初期化します。 */
    HRESULT initCompornent();

    /** その他コンポーネントを破棄します。 */
    HRESULT termCompornent();
    /** DirectXの共通コンポーネントを破棄します。 */
    HRESULT termD3D();
    /** ウィンドウを破棄します。 */
    HRESULT termWnd();

    /** 毎メッセージ処理時の更新処理を行います。 */
    void update(double time, double elapsedTime);
    /** 1フレームごとの更新処理を行います。 */
    void updateOnFrame(double time, double elapsedTime);
    /** 1フレームごとの描画を行います。 */
    void render(double time, double elapsedTime);

private:
    bool IsAllowRender() {
        return m_frameTimePassed;
    }
    void resetAllowRender() {
        m_frameTimePassed = false;
    }
};