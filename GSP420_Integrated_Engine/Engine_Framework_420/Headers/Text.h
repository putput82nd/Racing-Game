#ifndef TEXT_H
#define TEXT_H

#include "..\Headers\GfxStats.h"

// Font alignment
enum TEXT_ALIGNMENT { FA_LEFT, FA_CENTER, FA_RIGHT, FA_TOPRIGHT, FA_TOPLEFT, FA_BOTTOMRIGHT, FA_BOTTOMLEFT };

class Text
{
public:
    Text();
    ~Text() { Release(); }

    BOOL Initialize( LPDIRECT3DDEVICE9 pDevice, char* faceName, int size, BOOL bold = FALSE, BOOL italic = FALSE );

    void Print( char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite = NULL, 
        int textBoxWidth = 0, int textBoxHeight = 0, TEXT_ALIGNMENT alignment = FA_LEFT );

    void OnLostDevice();
    void OnResetDevice();
    void Release();

private:
    LPD3DXFONT m_pFont;

};

#endif