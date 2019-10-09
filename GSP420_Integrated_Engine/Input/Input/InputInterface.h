#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <DxErr.h>
#include <stdio.h>

#include "DIDevice.h"
#include "InputDevice.h"
#include "InputKeyboard.h"
#include "InputJoystick.h"
#include "InputMouse.h"

class InputInterface : public InputDevice
{

private:

	LPDIRECTINPUT8 m_pDI;
	InputKeyboard  *m_pKeyboard;
	InputJoystick  *m_pJoystick;
	InputMouse     *m_pMouse;

public:

	InputInterface(HINSTANCE hInst);
	~InputInterface(void);

	HRESULT Init(HWND, const RECT*, bool);
	void Release(void);
	HRESULT Update(void);
	bool Running(void);	
	bool HasJoystick(char *pJoyName);
	bool Pressed(INPUT_TYPE type, UINT nBtn);
	bool Released(INPUT_TYPE type, UINT nBtn);
	HRESULT GetPosition(INPUT_TYPE type, POINT* pPt);
};	

InputInterface::InputInterface(HINSTANCE hInst)
{
	m_hInst = hInst;
	m_bRunning = false;
	m_pDI = NULL;
	m_pLog = NULL;
	m_pJoystick = NULL;
	m_pMouse = NULL;
	m_pKeyboard = NULL;
}

InputInterface::~InputInterface()
{
	Release();
}

HRESULT InputInterface::Init(HWND hWnd, const RECT *rcWindow, bool bSaveToLog)
{
	HRESULT hr;
	m_hWndMain = hWnd;

	if (FAILED(hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI,NULL)))
	{
		return FAIL;
	}

	m_pKeyboard = new InputKeyboard(m_pDI,hWnd, m_pLog);
	m_pJoystick = new InputJoystick(m_pDI, hWnd, m_pLog);
	m_pMouse = new InputMouse(m_pDI, hWnd, m_pLog);

	if(FAILED(m_pKeyboard->Init()))
	{
		if(m_pKeyboard)
		{
			delete m_pKeyboard;
		}

		m_pKeyboard = NULL;
		return FAIL;
	}

	if(FAILED(m_pMouse->Init()))
	{
		if(m_pMouse)
		{
			delete m_pMouse;
		}

		m_pKeyboard = NULL;
		return FAIL;
	}

	if(rcWindow)
	{
		m_pMouse->SetWindowRect(*rcWindow);
	}

	if(FAILED(m_pJoystick->Init))
	{
		if (m_pJoystick)
		{
			delete m_pJoystick;
		}

		m_pJoystick = NULL;
	}

	m_bRunning = true;
	return SUCCESS;
}

void InputInterface::Release()
{
	if(m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	if(m_pMouse)
	{
		delete m_pMouse;
		m_pMouse = NULL; 
	}

	if(m_pJoystick)
	{
		delete m_pJoystick;
		m_pJoystick = NULL;
	}

	if(m_pDI)
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}

HRESULT InputInterface::Update(void)
{
	HRESULT hr;

	if(!Running())
	{
		return FAIL;
	}

	if(m_pKeyboard)
	{
		if(FAILED(hr = m_pKeyboard->Update()))
		{return hr;}		
	}

	if(m_pMouse)
	{
		if(FAILED(hr = m_pMouse->Update()))
		{return hr;}
	}

	if(m_pJoystick)
	{
		if(FAILED(hr = m_pJoystick->Update()))
		{return hr;}
	}

	return SUCCESS;
}

bool InputInterface::HasJoystick(char *pJoyName)
{
	if (m_pJoystick)
	{
		if(pJoyName)
		{
			m_pJoystick->GetName(pJoyName);
			return true;
		}
	}
	return false;
}	

HRESULT InputInterface::GetPosition(INPUT_TYPE type, POINT* pPt)
{
	if(type == DEV_MOUSE)
	{
		m_pMouse->GetPosition(pPt);
		return SUCCESS;
	}
	else if(type == DEV_JOYSTICK)
	{
		if(m_pJoystick)
		{
			m_pJoystick->GetPosition(pPt);
		}
		else
		{
			(*pPt).x = 0;
			(*pPt).y = 0;
		}

		return SUCCESS;
	}
	else
	{
		return INVALID;
	}
}

bool InputInterface::Pressed(INPUT_TYPE type, UINT nBtn)
{
	
	if(type == DEV_KEYBOARD)
	{
		return m_pKeyboard->Pressed(nBtn);
	}
	
	if(type == DEV_MOUSE)
	{
		return m_pMouse->Pressed(nBtn);
	}
	else if(type == DEV_JOYSTICK)
	{
		return m_pJoystick->Pressed(nBtn);
	}
	else
	{
		return false;
	}

}

bool InputInterface::Released(INPUT_TYPE type, UINT nBtn)
{

	if(type == DEV_KEYBOARD)
	{
		return m_pKeyboard->Released(nBtn);
	}

	if(type == DEV_MOUSE)
	{
		return m_pMouse->Released(nBtn);
	}
	else if(type == DEV_JOYSTICK)
	{
		return m_pJoystick->Released(nBtn);
	}
	else
	{
		return false;
	}

}


