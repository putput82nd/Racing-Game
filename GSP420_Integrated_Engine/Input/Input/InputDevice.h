#pragma once
 
#include <windows.h>
#include <d3dx9tex.h>
#include <stdio.h>

#include "DIDevice.h"

class InputDevice
{
protected:
	HWND m_hWndMain;
	HINSTANCE  m_hInst;
	bool m_bRunning;
	FILE *m_pLog;


public:
	InputDevice(void){};
	~InputDevice(void){};

	virtual HRESULT Init(HWND, const RECT*, bool) = 0;	
	virtual void Release(void) = 0;
	virtual bool Running(void) = 0;
	virtual bool HasJoystick(char *) = 0;
	virtual HRESULT Update(void) = 0;

	//mouse and joystic
	virtual HRESULT GetPosition(INPUT_TYPE, POINT*) = 0;

	//all devices
	virtual bool Pressed(INPUT_TYPE, UINT) = 0;

	//all devices
	virtual bool Released(INPUT_TYPE, UINT) = 0;
};

