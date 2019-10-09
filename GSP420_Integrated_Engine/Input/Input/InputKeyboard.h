#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <DxErr.h>
#include <stdio.h>

#include "DIDevice.h"

class InputKeyboard : public DIDevice
{
private:
	char m_key[256];
	char m_keyLast[256];

public:
	InputKeyboard(LPDIRECTINPUT8, HWND, FILE*);
	~InputKeyboard(void);

	HRESULT Init(void);
	HRESULT Update(void);

	bool Pressed(UINT key);
	bool Released(UINT key);
};										   

InputKeyboard::InputKeyboard(LPDIRECTINPUT8 pDI, HWND hWnd, FILE *p_log)
{
	Create(pDI, hWnd,p_log);
}

InputKeyboard::~InputKeyboard(void)
{
	Release();
}

//Initialize the keyboard device
HRESULT InputKeyboard::Init(void)
{
	if (FAILED(StartUp(GUID_SysKeyboard, &c_dfDIKeyboard)))
	{
		return FAIL;
	}
	
	//clear memory
	memset(m_key, 0, sizeof(m_key));
	memset(m_keyLast, 0, sizeof(m_keyLast));

	m_pDevice ->Acquire();
	
	return SUCCESS;
}

HRESULT InputKeyboard::Update(void)
{	
	// copy current key to last key
	memcpy(m_keyLast, m_key,sizeof(m_key));

	//if no data
	if (FAILED(GetData(DEV_KEYBOARD, &m_key, NULL)));
	{
		return FAIL;
	}

	return S_OK;
}

// Check for key press
bool InputKeyboard::Pressed(UINT key)
{
	if(m_key[key] & 0x08)
		return true;

	return false;
}

//Check for key release
bool InputKeyboard::Released(UINT key)
{
	if((m_keyLast[key] & 0x08) && !(m_key[key] & 0x08))
	{
		return true;
	}
	
	return false;
}



