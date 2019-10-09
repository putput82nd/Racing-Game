#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "..\Headers\GfxStats.h"
#include "..\Headers\Index_Buffer.h"

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer()
	{
		Release();
	}

    BOOL CreateBuffer( LPDIRECT3DDEVICE9 pDevice, UINT numVertices, DWORD FVF, UINT vertexSize, BOOL dynamic = FALSE );
    BOOL SetData( UINT numVertices, void *pVertices, DWORD flags = D3DLOCK_DISCARD );
    
	void Release();
	void SetIndexBuffer( IndexBuffer* pIndexBuff );
    void Render( LPDIRECT3DDEVICE9 pDevice, UINT numPrimitives, D3DPRIMITIVETYPE primitiveType );

private:
    LPDIRECT3DVERTEXBUFFER9 m_pVertBuff;
    IndexBuffer*            m_pIndexBuff;
    UINT                    m_numVertices;
    UINT                    m_vertexSize;
    DWORD                   m_FVF;
};

#endif