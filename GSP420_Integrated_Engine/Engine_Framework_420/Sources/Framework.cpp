#include "..\Headers\GfxStats.h"
#include "..\Headers\Framework.h"


//Default constructor
Framework::Framework( BaseApp* pMainGame )
{
    m_pGameApp = pMainGame;
    m_pDirInput = NULL;
    m_active = TRUE;
    m_renderingPaused = FALSE;
    m_timerPaused = FALSE;
    m_closing = FALSE;

    m_pGraphics = new D3DX_Graphics();
    m_pTimer = new Timer();
    m_fillMode = D3DFILL_SOLID;
}

//Destructor
void Framework::Release()
{
    m_closing = TRUE;
    SAFE_RELEASE( m_pGraphics );
    OnLostDevice();
    OnDestroyDevice();
}

//Creates the window and initializes DirectX Graphics.
BOOL Framework::Initialize( char* title, HINSTANCE hInstance, int width, int height, BOOL windowed )
{
    m_hInstance = hInstance;
    m_windowWidth = width;
    m_windowHeight = height;
    
    // Define the window
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof( WNDCLASSEX ); 
    wcex.style          = CS_DBLCLKS;
    wcex.lpfnWndProc    = (WNDPROC)Framework::StaticWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;//LoadIcon( hInstance, "Resources\\DeVryShield.ico" );
    wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = title;
    wcex.hIconSm        = NULL;//LoadIcon( hInstance, "Resources\\DeVryShield.ico" );

    // Register the window
    RegisterClassEx( &wcex );

    // Create the window
    m_hWnd = CreateWindow( title, title,  windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST, CW_USEDEFAULT, 
        0, width, height, NULL, NULL, hInstance, this );
    
    // Adjust to desired size
    RECT rect = { 0, 0, width, height };

    AdjustWindowRect( &rect, GetWindowLong( m_hWnd, GWL_STYLE ), FALSE );

    SetWindowPos( m_hWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE  );

    ShowWindow( m_hWnd, SW_SHOW );
    UpdateWindow( m_hWnd );

    // Save current location/size
    ZeroMemory( &m_wp, sizeof( WINDOWPLACEMENT ) );

    m_wp.length = sizeof( WINDOWPLACEMENT );

    GetWindowPlacement( m_hWnd, &m_wp );

    // Initialize Direct3D
    if ( !m_pGraphics->Initialize( m_hWnd, windowed ) )
    {
        return FALSE;
    }

    // Initialize DirectInput
    if( FAILED( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirInput, NULL ) ) )
    {
        SHOWERROR( "DirectInput8() - Failed", __FILE__, __LINE__ );
        return FALSE;
    }
    if ( !m_mouse.Initialize( m_pDirInput, m_hWnd, DIT_MOUSE ) )
    {
        return FALSE;
    }
    if ( !m_keyboard.Initialize( m_pDirInput, m_hWnd, DIT_KEYBOARD ) )
    {
        return FALSE;
    }

    m_fullscreenWidth = m_pGraphics->GetDisplayMode()->Width;
    m_fullscreenHeight = m_pGraphics->GetDisplayMode()->Height;
	   
    OnCreateDevice();
    OnResetDevice();

    // Start the timer
    Pause( FALSE, FALSE );

    return TRUE;
}

//Runs the application
void Framework::Run()
{
    MSG msg;

    while ( 1 ) 
    {
        // Did we recieve a message, or are we idling ?
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
        {
            if ( msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage( &msg );
            DispatchMessage ( &msg );
        } 
        else 
        {
            // Advance Game Frame.
            if ( m_pGraphics->GetDevice() != NULL && m_active )
            {
				OnUpdateFrame();
                OnRenderFrame();
            }
        } 
    }
}

//Called after the device is created. Create D3DPOOL_MANAGED resources here.
void Framework::OnCreateDevice()
{
    if ( m_pGameApp != NULL && m_pGraphics != NULL )
    {
        m_pGameApp->OnCreateDevice( m_pGraphics->GetDevice() );
    }
}

//Called after the device is reset. Create D3DPOOL_DEFAULT resources here.
void Framework::OnResetDevice()
{
    // Start the timer
    Pause( FALSE, FALSE );
    if ( m_pGameApp != NULL && m_pGraphics != NULL )
    {
        m_pGameApp->OnResetDevice( m_pGraphics->GetDevice() );
    }
}

//Called when the device is lost. Release D3DPOOL_DEFAULT resources here.
void Framework::OnLostDevice()
{
    if ( !m_closing )
    {
        // Stop the timer only if we're not closing down
        // or else stack corruption on return from Pause
        Pause( TRUE, TRUE );
    }
    if ( m_pGameApp != NULL )
    {
        m_pGameApp->OnLostDevice();
    }
}

//Called after the device is destroyed. Release D3DPOOL_MANAGED resources here.
void Framework::OnDestroyDevice()
{
    m_mouse.Release();
    m_keyboard.Release();
    SAFE_RELEASE( m_pDirInput );
    if ( m_pGameApp != NULL )
    {
        m_pGameApp->OnDestroyDevice();
    }
}

//Updates the current frame.
void Framework::OnUpdateFrame()
{
    if ( m_pTimer != NULL )
    {
        m_pTimer->Update();
    }
    if ( m_pGameApp != NULL && m_pGraphics != NULL && m_pTimer != NULL )
    {
        float elapsedTime = m_pTimer->GetElapsedTime();
        // Send out input data
        m_mouse.Read();
        m_keyboard.Read();
        long xDelta = m_mouse.GetXDelta();
        long yDelta = m_mouse.GetYDelta();
        long zDelta = m_mouse.GetZDelta();
        BOOL* pMouseButtons = m_mouse.GetButtons();
        BOOL* pPressedKeys = m_keyboard.GetKeys();
        m_pGameApp->ProcessInput( xDelta, yDelta, zDelta, pMouseButtons, pPressedKeys, elapsedTime );

        // Send out OnUpdateFrame
        m_pGameApp->OnUpdateFrame( m_pGraphics->GetDevice(), elapsedTime);
    }
}

//Renders the current frame.
void Framework::OnRenderFrame()
{
    if ( !m_active || (m_pGraphics->GetDevice() == NULL) )
    {
        return;
    }

    // Check for lost device
    HRESULT result = m_pGraphics->GetDevice()->TestCooperativeLevel();
    if ( FAILED( result ) )
    {
        if ( result == D3DERR_DEVICELOST )
        {
            Sleep( 50 );
            return;
        }
        else
        {
            OnLostDevice();
            if ( m_pGraphics->Reset() == D3DERR_DEVICELOST )
            {
                // Device is lost still
                Sleep( 50 );
                return;
            }
            else
            {
                OnResetDevice();
            }
        }
    }

    if ( m_pGameApp != NULL && !m_renderingPaused && m_pTimer != NULL )
    {
        m_pGameApp->OnRenderFrame( m_pGraphics->GetDevice(), m_pTimer->GetElapsedTime() );
    }
}

//Toggles between fullscreen and windowed mode.
void Framework::ToggleFullscreen()
{
    if ( m_pGraphics == NULL || !m_active )
    {
        return;
    }
    Pause( TRUE, TRUE );

    m_pGraphics->Windowed = !m_pGraphics->Windowed;

    // Set new window style
    if ( m_pGraphics->Windowed )
    {
        // Going to windowed mode
        SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW );
    }
    else 
    {
        // Save current location/size
        ZeroMemory( &m_wp, sizeof( WINDOWPLACEMENT ) );
        m_wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( m_hWnd, &m_wp );

        // Going to fullscreen mode
        SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_EX_TOPMOST );
    }

    // Reset the Device
    OnLostDevice();
    m_pGraphics->Reset();
    OnResetDevice();

    if ( m_pGraphics->Windowed )
    {
        // Going to windowed mode
        // Restore the window location/size
        SetWindowPlacement( m_hWnd, &m_wp );
    }

    Pause( FALSE, FALSE );
}

//Toggles between wireframe and solid fill modes
void Framework::ToggleWireframe()
{
    m_fillMode = (m_fillMode == D3DFILL_SOLID) ? D3DFILL_WIREFRAME : D3DFILL_SOLID;
    m_pGraphics->GetDevice()->SetRenderState( D3DRS_FILLMODE, m_fillMode );      
}

//Pauses or unpauses rendering and the timer.
void Framework::Pause( BOOL rendering, BOOL timer )
{
    m_renderingPauseCount += rendering ? 1 : -1;
    m_renderingPauseCount = (m_renderingPauseCount < 0) ? 0 : m_renderingPauseCount;

    m_timerPauseCount += timer ? 1 : -1;
    m_timerPauseCount = (m_timerPauseCount < 0) ? 0 : m_timerPauseCount;

    m_renderingPaused = (m_renderingPauseCount > 0);
    m_timerPaused = (m_timerPauseCount > 0);

    if ( m_timerPaused && m_pTimer != NULL )
    {
        m_pTimer->Stop();
    }
    else if ( !m_timerPaused && m_pTimer != NULL )
    {
        m_pTimer->Start();
    }
}

//Locks a key so it is only read once per key down.
void Framework::LockKey( DWORD key )
{
    m_keyboard.LockKey( key );
}

//Event handler. Routes messages to appropriate instance.
LRESULT CALLBACK Framework::StaticWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if ( msg == WM_CREATE )
    {
        SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams );
    }

    Framework *targetApp = (Framework*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

    if ( targetApp )
    {
        return targetApp->WndProc( hWnd, msg, wParam, lParam );
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//Application event handler.
LRESULT CALLBACK Framework::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message ) 
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    case WM_PAINT:
        if ( m_pGraphics->GetDevice() )
        {
            OnUpdateFrame();
            OnRenderFrame();
        } 
        ValidateRect( hWnd, 0 );
        return 0;
    case WM_SIZE:
        if ( wParam == SIZE_MINIMIZED )
        {
            // Disable application on minimized
            m_active = FALSE;
        } 
        else
        {
            m_active = TRUE;
            m_windowWidth = LOWORD( lParam );
            m_windowHeight = HIWORD( lParam );
            if ( m_pGraphics->GetDevice() )
            {
                OnLostDevice();
                m_pGraphics->Reset(); 
                OnResetDevice();
                OnUpdateFrame();
                OnRenderFrame();
            }
        }
        return 0;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}

//Gets x coordinate of the mouse in client coordinates.
long Framework::GetXCursor()
{
    return m_mouse.GetX();
}

//Gets y coordinate of the mouse in client coordinates.
long Framework::GetYCursor()
{
    return m_mouse.GetY();
}

//Gets the current fill mode.
DWORD Framework::GetFillMode()
{
    return m_fillMode;
}

//Gets the current client width.
int Framework::GetWidth()
{
    if ( m_pGraphics->Windowed )
    {
        return m_windowWidth;
    }

    return m_fullscreenWidth;
}

//Gets the current client height.
int Framework::GetHeight()
{
    if ( m_pGraphics->Windowed )
    {
        return m_windowHeight;
    }

    return m_fullscreenHeight;
}

//Gets the framerate
float Framework::GetFPS()
{
    if ( m_pTimer != NULL )
    {
        return m_pTimer->GetFPS();
    }
    return 0.0f;
}