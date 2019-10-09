#include "..\Headers\GfxStats.h"
#include "..\Headers\Mesh_Objects.h"


//Default constructor
Mesh::Mesh()
{
    m_pMesh = NULL; 
    m_numMaterials = 0; 
    m_pMeshMaterials = NULL;
    m_ppMeshTextures = NULL; 
}

//Cleans up resources
void Mesh::Release()
{
    // Delete the materials
    SAFE_DELETE_ARRAY( m_pMeshMaterials );

    // Delete the textures
    if ( m_ppMeshTextures )
    {
        for ( DWORD i = 0; i < m_numMaterials; i++ )
        {
            SAFE_RELEASE( m_ppMeshTextures[i] );
        }
    }
    SAFE_DELETE_ARRAY( m_ppMeshTextures );
    SAFE_RELEASE( m_pMesh );
}


//Load an X file mesh with no animation.
BOOL Mesh::Load( LPDIRECT3DDEVICE9 pDevice, char* file, char* textFile )
{
    Release();

    LPD3DXBUFFER pMaterialBuffer;

	//char* XModel[260] = { file } ;

    HRESULT hr = D3DXLoadMeshFromX( file, D3DXMESH_MANAGED, pDevice, NULL, &pMaterialBuffer, NULL, &m_numMaterials, &m_pMesh );
    if ( FAILED( hr ) )
    {
        SHOWERROR( "D3DXLoadMeshFromX - Failed", __FILE__, __LINE__ );
        return FALSE;
    }



    // Store material and texture information
    D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();
    
    m_pMeshMaterials = new D3DMATERIAL9[m_numMaterials];
    m_ppMeshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];

    // Copy the materials and textures from the buffer to the member arrays
    for ( DWORD i = 0; i < m_numMaterials; i++ )
    {
        m_pMeshMaterials[i] = pMaterials[i].MatD3D;
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
        // Create the texture if it exists
        m_ppMeshTextures[i] = NULL;

        if( pMaterials[i].pTextureFilename )
        {
           // CUtility::GetMediaFile( pMaterials[i].pTextureFilename, path );
			//char XModelMtl[260] = {"..\\Assets\\Meshes\\MGP Car\\bar.bmp"};

            if ( FAILED( D3DXCreateTextureFromFile( pDevice, textFile, &m_ppMeshTextures[i] ) ) )
            {
                SHOWERROR( "Failed to load mesh texture", __FILE__, __LINE__ );
                return FALSE;
            }
        }
    }

    // Don't need this no more!
    pMaterialBuffer->Release();

    return TRUE;
}

//Default constructor
MeshInstance::MeshInstance()
{
    m_pMesh = NULL; 
}


void MeshInstance::Release()
{
    // Mesh data is Released in CMesh
    m_pMesh = NULL; 
}


//Set the CMesh reference
void MeshInstance::SetMesh( Mesh* pMesh )
{
    Release();
    m_pMesh = pMesh; 
}

//Renders the mesh
void MeshInstance::Render( LPDIRECT3DDEVICE9 pDevice )
{
    if ( pDevice && m_pMesh )
    {
        pDevice->SetTransform( D3DTS_WORLD, GetTransform() );

        DWORD numMaterials = m_pMesh->GetNumMaterials();

        for ( DWORD i = 0; i < numMaterials; i++ )
        {
            pDevice->SetMaterial( m_pMesh->GetMeshMaterial( i ) );
            pDevice->SetTexture( 0, m_pMesh->GetMeshTexture( i ) );

            m_pMesh->GetMesh()->DrawSubset( i );
        }
    }
}