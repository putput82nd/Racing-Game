#include "..\Headers\GfxStats.h"
#include "..\Headers\Vertex_Buffer.h"


VertexBuffer::VertexBuffer()
{
    m_pVertBuff          = NULL; 
    m_pIndexBuff         = NULL; 
    m_numVertices = 0; 
    m_FVF         = 0; 
    m_vertexSize  = 0;
}

//Creates the vertex buffer.
BOOL VertexBuffer::CreateBuffer( LPDIRECT3DDEVICE9 pDevice, UINT numVertices, DWORD FVF, UINT vertexSize, BOOL dynamic )
{
    Release();
    m_numVertices = numVertices;
    m_FVF = FVF;
    m_vertexSize = vertexSize;

    // Dynamic buffers can't be in D3DPOOL_MANAGED
    D3DPOOL pool = dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
    DWORD usage = dynamic ? D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC : D3DUSAGE_WRITEONLY;
    if ( FAILED( pDevice->CreateVertexBuffer( m_numVertices * m_vertexSize, usage, m_FVF, pool, &m_pVertBuff, NULL ) ) )
    {
        SHOWERROR( "CreateVertexBuffer failed.", __FILE__, __LINE__ );
        return FALSE;
    }

    return TRUE;
}


void VertexBuffer::Release()
{
    SAFE_RELEASE( m_pVertBuff );
    m_numVertices = 0;
    m_FVF = 0;
    m_vertexSize = 0;
    // IndexBuffer is released in CIndexBuffer
    m_pIndexBuff = NULL;

}

//Fill up the vertex buffer
BOOL VertexBuffer::SetData( UINT numVertices, void *pVertices, DWORD flags )
{
    if ( m_pVertBuff == NULL )
    {
        return FALSE;
    }

    char *pData;
    // Lock the vertex buffer
    if ( FAILED( m_pVertBuff->Lock( 0, 0, (void**)&pData, flags ) ) )
    {
        SHOWERROR( "IDirect3DVertexBuffer9::Lock failed.", __FILE__, __LINE__ );
        return FALSE;
    }

    // Copy vertices to vertex buffer
    memcpy( pData, pVertices, numVertices * m_vertexSize );
    
    // Unlock vertex buffer
    if ( FAILED( m_pVertBuff->Unlock() ) )
    {
        SHOWERROR( "IDirect3DVertexBuffer9::Unlock failed.", __FILE__, __LINE__ );
        return FALSE;
    }

    return TRUE;
}

//Assigns an index buffer to the vertex buffer.
void VertexBuffer::SetIndexBuffer( IndexBuffer* pIB )
{
    m_pIndexBuff = pIB;
}

//Render the buffer
void VertexBuffer::Render( LPDIRECT3DDEVICE9 pDevice, UINT numPrimitives, D3DPRIMITIVETYPE primitiveType )
{
    if ( pDevice == NULL )
    {
        return;
    }

    pDevice->SetStreamSource( 0, m_pVertBuff, 0, m_vertexSize );
    pDevice->SetFVF( m_FVF );

    if ( m_pIndexBuff )
    {
        pDevice->SetIndices( m_pIndexBuff->GetBuffer() );
        pDevice->DrawIndexedPrimitive( primitiveType, 0, 0, m_numVertices, 0, numPrimitives );
    }
    else
    {
        pDevice->SetIndices( NULL );
        pDevice->DrawPrimitive( primitiveType, 0, numPrimitives );
    }
}