#include <Windows.h>
#include "util.hpp"

#include "AppEngine.hpp"

/**
 * エントリーポイントです
 */
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	UNUSED(hInstance);
	UNUSED(hPrevInstance);
	UNUSED(lpCmdLine);
	UNUSED(nCmdShow);

    AppEngine engine;

    if (SUCCEEDED(engine.initD3DWindow())) {
        engine.intoMainLoop();
    }
    engine.termD3DWindow();

	return 0;
}