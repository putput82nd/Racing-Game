#include "..\Headers\GfxStats.h"
#include "..\Headers\Text.h"

//Default constructor.
Text::Text()
{
    m_pFont = NULL; 
}

//Initialize the font
BOOL Text::Initialize( LPDIRECT3DDEVICE9 pDevice, char* faceName, int size, BOOL bold, BOOL italic )
{
    SAFE_RELEASE( m_pFont );

	HRESULT hr = 0;
    
	hr = D3DXCreateFont( pDevice, -size, 0, bold ? 800 : 0, 1, italic, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, faceName, &m_pFont );
    
	if ( FAILED( hr ) )
    {
        SHOWERROR( "D3DXCreateFont() failed.", __FILE__, __LINE__ );
        return FALSE;
    }
    
	return TRUE;
}

//Print some 2D text.
void Text::Print( char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite, 
    int textBoxWidth, int textBoxHeight, TEXT_ALIGNMENT alignment )
{
    if ( !m_pFont ) 
    {
        return;
    }
    DWORD format = DT_EXPANDTABS;
    if ( textBoxWidth == 0 )
    {
        format |= DT_NOCLIP;
    }
    else
    {
        format |= DT_WORDBREAK;
        switch ( alignment )
        {
        case FA_LEFT:
            format |= DT_LEFT;
            break;
        case FA_CENTER:
            format |= DT_CENTER;
            break;
        case FA_RIGHT:
            format |= DT_RIGHT;
            break;
        case FA_TOPRIGHT:
            format |= DT_RIGHT | DT_TOP;
            break;
        case FA_BOTTOMRIGHT:
            format |= DT_RIGHT | DT_BOTTOM;
            break;
        case FA_TOPLEFT:
            format |= DT_LEFT | DT_TOP;
            break;
        case FA_BOTTOMLEFT:
            format |= DT_LEFT | DT_BOTTOM;
            break;
        }
        if ( textBoxHeight == 0 )
        {
            // A width is specified, but not a height.
            // Make it seem like height is infinite
            textBoxHeight = 2000;
        }
    }

    RECT rect = { xPosition, yPosition, xPosition + textBoxWidth, yPosition + textBoxHeight };

    m_pFont->DrawText( sprite, text, -1, &rect, format, color );
}

//Releases video resources. Call whenever the device is lost or before reseting the device.
void Text::OnLostDevice()
{
    if ( m_pFont )
    {
        m_pFont->OnLostDevice();
    }
}

//Re-acquire video resources. Call after device is reset
void Text::OnResetDevice()
{
    if ( m_pFont )
    {
        m_pFont->OnResetDevice();
    }
}

//Release interfaces
void Text::Release()
{
    SAFE_RELEASE( m_pFont ); 
}