#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <DxErr.h>
#include <stdio.h>

#include "DIDevice.h"

class InputJoystick : public DIDevice
{
private:

	GUID m_guid;
	char m_Name[256];
	bool m_bConnected;
	bool m_bPressed[12];
	bool m_bReleased[12];
	DWORD m_dwNumBtns;

public:
	InputJoystick(LPDIRECTINPUT8, HWND, FILE*);
	~InputJoystick(void); 

	HRESULT Init(void);
	HRESULT Update(void);

	bool JoystickFound(void) {return m_bConnected;}
	BOOL EnumJoyCallback(const DIDEVICEINSTANCE *pI);
	void GetName(char *pJoystickName) {memcpy(pJoystickName, m_Name, sizeof(char) * 256);}

	bool Pressed(UINT btn);
	bool Released(UINT btn);
	
};

// pointer to InputJoystic
InputJoystick *pThis = NULL;

// callback for the joystick using this class
BOOL CALLBACK gEnumJoyCallback(const DIDEVICEINSTANCE *pInst, void* pUserDat)
{
	return pThis->EnumJoyCallback(pInst);
}


InputJoystick::InputJoystick(LPDIRECTINPUT8 pDI, HWND hWnd, FILE* pLog)
{
	Create(pDI, hWnd, pLog);
}

InputJoystick::~InputJoystick(void)
{
	Release();
}

// joystick connected
bool InputJoystick::JoystickFound(void)
{
	return m_bConnected;
}

// Init Joystick
HRESULT InputJoystick::Init(void)
{
	DIPROPRANGE diprng;
	DIDEVCAPS diCaps;

	//clear the memory
	memset(m_bPressed, 0, sizeof(m_bPressed));
	memset(m_bReleased, 0, sizeof(m_bReleased));

	m_bConnected = false;
	m_lX = 0;
	m_lY = 0;
	
	// enumerate the Joystick devices
	m_pDI ->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK) gEnumJoyCallback, &m_guid, DIEDFL_ATTACHEDONLY);

	if(!m_bConnected)
	{
		return FAIL;
	}


	diprng.diph.dwSize = sizeof(DIPROPRANGE);
	diprng.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprng.diph.dwHow = DIPH_BYOFFSET;
	diprng.lMax = +1000;
	diprng.lMin = -1000;


	diprng.diph.dwObj = DIJOFS_X;

	m_pDevice->SetProperty(DIPROP_RANGE, &diprng.diph);

	diprng.diph.dwObj = DIJOFS_Y;

	m_pDevice->SetProperty(DIPROP_RANGE, &diprng.diph);

	if(SUCCEEDED(m_pDevice->GetCapabilities(&diCaps)))
	{
		m_dwNumBtns = diCaps.dwButtons;
	}
	else
	{
		m_dwNumBtns = 4;
	}

	return S_OK;
}

// callback function
BOOL InputJoystick::EnumJoyCallback(const DIDEVICEINSTANCE *pInst)
{
	//Startup the device
	if(SUCCEEDED(StartUp(pInst->guidInstance, &c_dfDIJoystick)))
	{
		m_bConnected = true;
		strcpy(m_Name, (char*)pInst->tszProductName);
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

HRESULT InputJoystick::Update(void)
{
	DIJOYSTATE js;

	m_pDevice ->Poll();

	//get joystick data
	if (FAILED(GetData(DEV_JOYSTICK, &js, NULL)))
	{
		return FAIL;
	}
	
	//Joystick button pressed query
	for(DWORD i =0; i < m_dwNumBtns; i++)
	{
		m_bReleased(i) = false;

		if(js.rgbButtons[i] &0x80)
		{
			m_bPressed[i] = true;
		}
		else
		{
			m_bPressed[i] = false;
		}
	}

	//Joystick Position
	m_lX = js.lX;
	m_lY = js.lY;

	return S_OK;

}

bool InputJoystick::Pressed(UINT btn)
{
	if (btn < m_dwNumBtns)
	{
		return m_bPressed[btn];
	}
	else
	{
		return false;
	}
}

bool InputJoystick::Released(UINT btn)
{
	if (btn < m_dwNumBtns)
	{
		return m_bReleased[btn];
	}
	else
	{
		return false;
	}
}
