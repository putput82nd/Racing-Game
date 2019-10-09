#include "..\Headers\GfxStats.h"
#include "..\Headers\Simple_Input.h"

//Default constructor
InputDevice::InputDevice()
{
    m_pDevice = NULL;
    m_x = m_y = 0;
    ZeroMemory( m_keyLock, sizeof( BOOL ) * 256 );
    ZeroMemory( &m_mouseState, sizeof( DIMOUSESTATE ) );
    ZeroMemory( m_keyboardState, 256 );
    ZeroMemory( m_pressedKeys, 256 );
    ZeroMemory( m_pressedButtons, 4 );
}

//Initializes a new input device
BOOL InputDevice::Initialize( LPDIRECTINPUT8 pDI, HWND hWnd, DIRECTINPUTTYPE type )
{
    // Check for a valid parent DIRECTINPUT8 interface
    if ( pDI == NULL || type == DIT_FORCE_DWORD )
    {
        return FALSE;
    }
    Release();

    DIDATAFORMAT* pDataFormat;
    m_hWnd = hWnd;
    m_type = type;
    
    // Create the device
    switch( type )
    {
    case DIT_KEYBOARD:
        if ( FAILED( pDI->CreateDevice( GUID_SysKeyboard, &m_pDevice, NULL ) ) )
        {
            SHOWERROR( "Unable to create keyboard device.", __FILE__, __LINE__ );
            return FALSE;
        }
        pDataFormat = (DIDATAFORMAT*)&c_dfDIKeyboard;
        break;
    case DIT_MOUSE:
        if ( FAILED( pDI->CreateDevice( GUID_SysMouse, &m_pDevice, NULL ) ) )
        {
            SHOWERROR( "Unable to create mouse device.", __FILE__, __LINE__ );
            return FALSE;
        }
        pDataFormat = (DIDATAFORMAT*)&c_dfDIMouse;
        break;
    default: 
        return FALSE;
    }

    // Set the data format
    if( FAILED( m_pDevice->SetDataFormat( pDataFormat ) ) )
    {
        SHOWERROR( "Unable to set input data format.", __FILE__, __LINE__ );
        return FALSE;
    }

    // Set the cooperative level
    if( FAILED( m_pDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
    {
        SHOWERROR( "Unable to set input cooperative level.", __FILE__, __LINE__ );
        return FALSE;
    }

    // Acquire the device
    if( FAILED( m_pDevice->Acquire() ) )
    {
        SHOWERROR( "Unable to acquire the input device.", __FILE__, __LINE__ );
        return FALSE;
    }

    return TRUE;
}

//Get the current device state
void InputDevice::Read()
{
    if ( !m_pDevice )
    {
        return;
    }
    
    // Grab the data 
    if ( m_type == DIT_MOUSE )
    {
        HRESULT hr = m_pDevice->GetDeviceState( sizeof( DIMOUSESTATE ), (LPVOID)&m_mouseState );
        if ( FAILED( hr )  )
        {
            if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
            {
                // Device is lost, try to reaquire it
                m_pDevice->Acquire();
            }
            return;
        }
        // Store cursor position
        POINT pt;
        GetCursorPos( &pt );
        ScreenToClient( m_hWnd, &pt );
        m_x = pt.x;
        m_y = pt.y;
        // Get pressed keys
        for ( int i = 0; i < 4; i++ )
        {
            if ( m_mouseState.rgbButtons[i] & 0x80 )
            {
                m_pressedButtons[i] = TRUE;
            }
            else
            {
                m_pressedButtons[i] = FALSE;
            }
        }
    }
    else if ( m_type == DIT_KEYBOARD )
    {
        HRESULT hr = m_pDevice->GetDeviceState( 256, (LPVOID)&m_keyboardState );
        if ( FAILED( hr )  )
        {
            if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
            {
                // Device is lost, try to reaquire it
                m_pDevice->Acquire();
            }
            return;
        }
        // Get pressed keys and release locks on key up
        for ( int i = 0; i < 256; i++ )
        {
            if ( !(m_keyboardState[i] & 0x80) )
            {
                // Key is up so release lock
                m_keyLock[i] = FALSE;
                m_pressedKeys[i] = FALSE;
            }
            else
            {
                // Key is pressed if it isn't locked
                m_pressedKeys[i] = !(m_keyLock[i]);
            }
        }
    }
}

//Locks a key so it is only read once per key down.
void InputDevice::LockKey( DWORD key )
{
    m_keyLock[key] = TRUE;
}

//Free resources
void InputDevice::Release()
{
    if( m_pDevice )
    {
        m_pDevice->Unacquire();
        SAFE_RELEASE( m_pDevice );
    }
}