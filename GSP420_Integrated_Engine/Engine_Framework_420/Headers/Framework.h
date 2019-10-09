#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "..\Headers\GfxStats.h"
#include "..\Headers\D3DX.h"
#include "..\Headers\Simple_Input.h"
#include "..\Headers\Timer.h"
#include "AICoreInterface.h"



//Interface that the main game application must implement
class BaseApp
{
public:
    virtual ~BaseApp() {}

    virtual void Release() = 0;
    virtual void OnCreateDevice( LPDIRECT3DDEVICE9 pDevice ) = 0;
    virtual void OnResetDevice( LPDIRECT3DDEVICE9 pDevice ) = 0;
    virtual void OnLostDevice() = 0;
    virtual void OnDestroyDevice() = 0;

    virtual void OnUpdateFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime) = 0;
    virtual void OnRenderFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime ) = 0;

    virtual void ProcessInput( long xDelta, long yDelta, long zDelta, BOOL* pMouseButtons, BOOL* pPressedKeys, float elapsedTime ) = 0;
};



//Main framework class
class Framework
{
public:
    Framework( BaseApp* pMainGame );
    ~Framework()
	{
		Release();
	}

    BOOL Initialize( char* title, HINSTANCE hInstance, int width, int height, BOOL windowed = TRUE );
    
	void Run();
    void Release();
    void ToggleFullscreen();
    void ToggleWireframe();
    void LockKey( DWORD key );
	
	static LRESULT CALLBACK StaticWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
    
    long GetXCursor();
    long GetYCursor();
    
	DWORD GetFillMode();
    
	int GetWidth();
    int GetHeight();
    
	float GetFPS();

private:
    LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

    void OnCreateDevice();
    void OnResetDevice();
    void OnLostDevice();
    void OnDestroyDevice();
    void OnUpdateFrame();
    void OnRenderFrame();
    void Pause( BOOL rendering, BOOL timer );

    HWND            m_hWnd;
    HINSTANCE       m_hInstance;
    
    int             m_windowWidth;
    int             m_windowHeight;
    int             m_fullscreenWidth;
    int             m_fullscreenHeight;
	int             m_renderingPauseCount;
    int             m_timerPauseCount;
    
    WINDOWPLACEMENT m_wp;
    DWORD           m_fillMode;
    
	BOOL            m_active;
	BOOL            m_renderingPaused;
    BOOL            m_timerPaused;
    BOOL            m_closing;

    D3DX_Graphics*      m_pGraphics;
    BaseApp*       m_pGameApp;
    Timer*         m_pTimer;
    InputDevice    m_mouse;
    InputDevice    m_keyboard;

    LPDIRECTINPUT8  m_pDirInput;
};




#endif