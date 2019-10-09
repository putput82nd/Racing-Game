
#pragma once	//


#include <d3d9.h>
#include <dinput.h>
#include <DxErr.h>
#include <stdio.h>

#define SUCCESS S_OK
#define FAIL 0x82000001 // unique value for failures...I hope
#define INVALID 0x83000001 // unique values for Invalid parameters...Yikes

//Use this to Identify which type of device is used
typedef enum INPUT_TYPE
{
	DEV_KEYBOARD,
	DEV_JOYSTICK,
	DEV_MOUSE
};

class DIDevice
{

protected:
	LPDIRECTINPUTDEVICE8	m_pDevice;  // Input device pointer
	LPDIRECTINPUT8			m_pDI;		// Direct Input obj pointer
	HWND					m_hWnd;		// Window handle
	long					m_lX;		// x and y position
	long					m_lY;
	FILE					*m_pLog;	//log file

	virtual HRESULT GetData(INPUT_TYPE type, void *pData, DWORD *dwNum);

public:
	DIDevice(void){}
	~DIDevice(void){}

	virtual void Create(LPDIRECTINPUT8, HWND, FILE*);
	virtual void Release(void);
	virtual HRESULT StartUp(REFGUID guid, LPCDIDATAFORMAT dataFormat);

	virtual void GetPosition(POINT* pPoint);

	virtual HRESULT Init(void) = 0;
	virtual HRESULT Update(void) = 0;
};

// create device
void DIDevice::Create(LPDIRECTINPUT8 pDI, HWND hWnd, FILE* pLog)
{
	m_pDevice = NULL;
	m_pDI = pDI;
	m_hWnd = hWnd;
	m_pLog = pLog;
}
// release device
void DIDevice::Release(void)
{
	if (m_pDevice)
	{
		m_pDevice -> Unacquire();
		m_pDevice -> Release();
		m_pDevice = NULL;
	}
}
//start up a device
HRESULT DIDevice::StartUp(REFGUID guid, LPCDIDATAFORMAT dataFormat)
{	
	// window focus flags
	DWORD dwFlags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE; 

	// restart device if already acquired
	if (m_pDevice)
	{
		this -> Release();
	}

	// create device
	if (FAILED(m_pDI ->CreateDevice(guid, &m_pDevice, NULL)))
	{
		return FAIL;
	}

	// set data format
	if (FAILED(m_pDevice ->SetDataFormat(dataFormat)))
	{
		return FAIL;
	}

	// set coop levels
	if (FAILED(m_pDevice->SetCooperativeLevel(m_hWnd, dwFlags)))
	{
		return FAIL;
	}


	return SUCCESS;	 
}
//get position of cursor of Joystick
void DIDevice::GetPosition(POINT* pPoint)
{
	(*pPoint).x = m_lX; 
	(*pPoint).y = m_lY;
}
// KEyboard data
HRESULT DIDevice::GetData(INPUT_TYPE type, void *pData, DWORD *pdwNum)
{
	HRESULT hr = FAIL;
	size_t size = 0;

	//Check for mouse, keyboard, joystick
	if (type == DEV_MOUSE)
	{
		size = sizeof(DIDEVICEOBJECTDATA);

		hr = m_pDevice->GetDeviceData(size, (DIDEVICEOBJECTDATA*)pData, pdwNum, 0);
	}

	else
	{
		if (type == DEV_KEYBOARD)
		{	
			size = sizeof(char) * 256;
		}
		else
		{
			size = sizeof(DIJOYSTATE);
		}

		hr = m_pDevice -> GetDeviceState(size, pData);
	}

	// check for device acquisition and input 
	if (FAILED(hr))
	{
		if ( (hr == DIERR_NOTACQUIRED) || (hr == DIERR_INPUTLOST))
		{
			hr = m_pDevice -> Acquire();

			//when input is lost keep acquiring device
			while(hr == DIERR_INPUTLOST)
				hr = m_pDevice ->Acquire();

			if (hr == DIERR_OTHERAPPHASPRIO) return S_OK;

			if(SUCCEEDED (hr))
			{
				if (type == DEV_MOUSE)
				{
					hr = m_pDevice ->GetDeviceData(size,(DIDEVICEOBJECTDATA*)pData, pdwNum, 0);
				}
				else
				{
					hr = m_pDevice ->GetDeviceState(size, pData);
				}

			}

			if (FAILED(hr))
			{	
				return FAIL;
			}
		}
		else return FAIL;
	}
	return S_OK;
}

