#ifndef D3DX_H
#define D3DX_H

#include "..\Headers\GfxStats.h"

class D3DX_Graphics
{
public:
    D3DX_Graphics();
    ~D3DX_Graphics()
	{
		Release();
	}

    BOOL Initialize( HWND hWnd, BOOL bWindowed );
    BOOL Reset();
	BOOL Windowed;
	
	void Release();
    
    LPDIRECT3D9       GetD3D()          { return m_pD3D9; }
    LPDIRECT3DDEVICE9 GetDevice()       { return m_pDevice; }
    D3DDISPLAYMODE*   GetDisplayMode()  { return &m_displayMode; }
   
private:
    BOOL BuildPresentParameters();

    HWND                    m_hWnd;
    HINSTANCE               m_hInstance;
    LPDIRECT3D9             m_pD3D9;
    LPDIRECT3DDEVICE9       m_pDevice;
    D3DCAPS9                m_caps;
    D3DPRESENT_PARAMETERS   m_pp;
    D3DDISPLAYMODE          m_displayMode;
};

#endif