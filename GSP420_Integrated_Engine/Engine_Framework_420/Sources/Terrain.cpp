#include "..\Headers\GfxStats.h"
#include "..\Headers\Terrain.h"

//Default constructor.
Terrain::Terrain()
{
    m_pHeight = NULL;
    m_pTexture = NULL;
    m_numVertices = m_numIndices = 0;
}

//Create a new Terrain object.
BOOL Terrain::Initialize( LPDIRECT3DDEVICE9 pDevice, char *rawFile, char *terrainTexture )
{
    Release();
    
    // Load height map
	char HeightMap[260] = {"Resources//TerragenExp1.raw"};
	//path[MAX_PATH] ="..\media\heightMap.raw";
   // CUtility::GetMediaFile( rawFile, path );

    std::ifstream heightStream;

    heightStream.open( HeightMap, std::ios::binary );

    if ( heightStream.fail() )
    {
        SHOWERROR( "Could not open height file.", __FILE__, __LINE__ );
        return FALSE;
    }

    // Get number of vertices
    heightStream.seekg( 0, std::ios::end );

	//Gave warning, if there are issues with HeightMap, it could be due to the heightstream being bigger than an UINT
    //m_numVertices = heightStream.tellg();
	m_numVertices = (UINT)heightStream.tellg();


    heightStream.seekg( 0, std::ios::beg );

    // Allocate memory and read the data
    m_pHeight = new UCHAR[m_numVertices];

    heightStream.read( (char *)m_pHeight, m_numVertices );
    heightStream.close();

    // Generate vertices
    UINT width = (int)sqrt( (float)m_numVertices );
    PositionTextured* pVertices = NULL; 

    Geometry::GeneratePositionTexturedWithHeight( &pVertices, width, width, m_pHeight );

    m_vertbuff.CreateBuffer( pDevice, m_numVertices, D3DFVF_XYZ | D3DFVF_TEX1, sizeof( PositionTextured ) );
    m_vertbuff.SetData( m_numVertices, pVertices, 0 );

    // Generate indices
    int* pIndices = NULL;
    m_numIndices = Geometry::GenerateIndices( &pIndices, width, width );
    m_indexbuff.CreateBuffer( pDevice, m_numIndices, D3DFMT_INDEX32 );
    m_indexbuff.SetData( m_numIndices, pIndices, 0 );
    m_vertbuff.SetIndexBuffer( &m_indexbuff );
    
    //CUtility::GetMediaFile( terrainTexture, path );
	//char TerrainText[260] = {"terrain.jpg"};
	char TerrainText[260] = {"Resources//grass.dds"};
    if ( FAILED( D3DXCreateTextureFromFile( pDevice, TerrainText, &m_pTexture ) ) )
    {
        SHOWERROR( "Unable to load terrain textures.", __FILE__, __LINE__ );
        return FALSE;

    }
    return TRUE;
}

//Renders the terrain.
void Terrain::Render( LPDIRECT3DDEVICE9 pDevice )
{
    pDevice->SetTransform( D3DTS_WORLD, GetTransform() );
    pDevice->SetTexture( 0, m_pTexture );
    m_vertbuff.Render( pDevice, m_numIndices - 2, D3DPT_TRIANGLESTRIP );
}

void Terrain::Release()
{
    SAFE_DELETE_ARRAY( m_pHeight );
    SAFE_RELEASE( m_pTexture );
    m_vertbuff.Release();
    m_indexbuff.Release();
}