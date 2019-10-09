#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <DxErr.h>
#include <stdio.h>

#include "DIDevice.h"

#define BUFFERSIZE 16 

class InputMouse : public DIDevice
{
private:
	HANDLE m_hEvent;
	bool m_bPressed[3];
	bool m_bReleased[3];

	RECT m_rcWindow; 

public:
	InputMouse(LPDIRECTINPUT8, HWND, FILE*);
	~InputMouse(void);

	HRESULT Init(void);
	HRESULT Update(void);

	void SetWindowRect(RECT rcWindow) { m_rcWindow = rcWindow; } 

	bool Pressed(UINT btn);
	bool Released(UINT btn);

};


InputMouse::InputMouse(LPDIRECTINPUT8 pDI, HWND hWnd, FILE* pLog)
{
	Create(pDI, hWnd, pLog);	
}

InputMouse::~InputMouse(void)
{
	Release();
}

// initialize the mouse device
HRESULT InputMouse::Init(void)
{
	memset(m_bPressed, 0, sizeof(m_bPressed));
	memset(m_bReleased, 0, sizeof(m_bReleased));
	m_lX = 0;
	m_lY = 0;
	
	if(FAILED(StartUp(GUID_SysMouse, &c_dfDIMouse)))
	{
		return FAIL;
	}
	
	if(!(m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
	{
		return FAIL;
	}

	if (FAILED(m_pDevice->SetEventNotification(m_hEvent)))
	{
		return FAIL;
	}

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = BUFFERSIZE;

	if(FAILED(m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		return FAIL;
	}

	m_pDevice->Acquire();

	return SUCCESS;
}

// check the button and position status.
HRESULT InputMouse::Update(void)
{

	DIDEVICEOBJECTDATA objDat[BUFFERSIZE];
	DWORD dwNumElem = BUFFERSIZE;

	if(FAILED(GetData(DEV_MOUSE, &objDat[0], &dwNumElem)))
	{
		return FAIL;
	}

	m_bReleased[0] = false;
	m_bReleased[1] = false;
	m_bReleased[2] = false;

	//cycle through the buttons and the positions 
	for (DWORD  i = 0; i < dwNumElem; i++)
	{
		//using the object data offsets
		switch (objDat[i].dwOfs)
		{
		case DIMOFS_X:
			{
				m_lX += objDat[i].dwData;

				//constrain coordinate data
				if (m_lX < m_rcWindow.left) {m_lX = m_rcWindow.left;}
				if (m_lX > m_rcWindow.right){m_lX = m_rcWindow.right;}
				break;
			}

		case DIMOFS_Y:
			{
				m_lY += objDat[i].dwData;

				//constrain coordinate data
				if (m_lY < m_rcWindow.top){m_lY = m_rcWindow.top;}
				if (m_lY > m_rcWindow.bottom){m_lY = m_rcWindow.bottom;}
				break;
			}

		case DIMOFS_BUTTON0:
			{
				if(objDat[i].dwData & 0x80)
				{
					m_bPressed[0] = true;
				}

				else if(m_bPressed[0])
				{
					m_bReleased[0] = true; 
					m_bPressed[0] = false;
				}
				break;
			}
		case DIMOFS_BUTTON1:
			{
				if(objDat[i].dwData & 0x80)
				{
					m_bPressed[1] = true;
				}

				else if(m_bPressed[1])
				{
					m_bReleased[1] = true; 
					m_bPressed[1] = false;
				}
				break;
			}

		case DIMOFS_BUTTON2:
			{
				if(objDat[i].dwData & 0x80)
				{
					m_bPressed[2] = true;
				}

				else if(m_bPressed[2])
				{
					m_bReleased[2] = true; 
					m_bPressed[2] = false;
				}
				break;
			}

		return S_OK; 

		}
	}
}
//check for button press
bool InputMouse::Pressed(UINT btn)
{
	if (btn < 3)
	{
		return m_bPressed[btn];
	}

	return false;
}
//check for button release
bool InputMouse::Released(UINT btn)
{
	if (btn < 3)
	{
		return m_bReleased[btn];
	}

	return false;
}



