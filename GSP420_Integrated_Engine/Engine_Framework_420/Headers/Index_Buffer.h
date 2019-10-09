#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "..\Headers\GfxStats.h"

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer() 
	{ 
		Release();
	}

    BOOL CreateBuffer( LPDIRECT3DDEVICE9 pDevice, UINT numIndices, D3DFORMAT format, BOOL dynamic = FALSE );

	BOOL SetData( UINT numIndices, void *pIndices, DWORD flags = D3DLOCK_DISCARD );

    void Release();
    
    LPDIRECT3DINDEXBUFFER9 GetBuffer()	 { return m_pIndexBuff; }

    UINT GetNumIndices()				 { return m_numIndices; }
    
private:
    LPDIRECT3DINDEXBUFFER9  m_pIndexBuff;
    UINT                    m_numIndices;
};

#endif