#include "..\Headers\GfxStats.h"
#include "..\Headers\Index_Buffer.h"

//Default Constructor
IndexBuffer::IndexBuffer()
{
    m_pIndexBuff        = NULL; 
    m_numIndices = 0; 
}

//Free up resources
void IndexBuffer::Release()
{
    SAFE_RELEASE( m_pIndexBuff );
    m_numIndices = 0;
}

//Creates an index buffer.
BOOL IndexBuffer::CreateBuffer( LPDIRECT3DDEVICE9 pDevice, UINT numIndices, D3DFORMAT format, BOOL dynamic )
{
    Release();
    m_numIndices = numIndices;

    // Dynamic buffers can't be in D3DPOOL_MANAGED
    D3DPOOL pool = dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
    UINT size = (format == D3DFMT_INDEX32) ? sizeof( UINT ) : sizeof( USHORT );
    DWORD usage = dynamic ? D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC : D3DUSAGE_WRITEONLY;

    if( FAILED( pDevice->CreateIndexBuffer( m_numIndices * size, usage, format, pool, &m_pIndexBuff, NULL ) ) )
    {
        SHOWERROR( "CreateIndexBuffer failed.", __FILE__, __LINE__ );
        return FALSE;
    }
    return TRUE;
}

//Fill up the index buffer
BOOL IndexBuffer::SetData( UINT numIndices, void *pIndices, DWORD flags )
{
    if ( m_pIndexBuff == NULL )
    {
        return FALSE;
    }

    char *pData;

    D3DINDEXBUFFER_DESC desc;
    m_pIndexBuff->GetDesc( &desc );
    UINT size = (desc.Format == D3DFMT_INDEX32) ? sizeof( UINT ) : sizeof( USHORT );

    // Lock the index buffer
    if ( FAILED( m_pIndexBuff->Lock( 0, 0, (void**)&pData, flags ) ) )
    {
        return FALSE;
    }

    memcpy( pData, pIndices, numIndices * size );

    // Unlock index buffer
    if ( FAILED( m_pIndexBuff->Unlock() ) )
    {
        return FALSE;
    }

    return TRUE;
}