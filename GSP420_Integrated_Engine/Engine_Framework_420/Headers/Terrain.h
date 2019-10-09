#ifndef TERRAIN_H
#define TERRAIN_H

#include "..\Headers\GfxStats.h"
#include "..\Headers\World_Transformations.h"
#include "..\Headers\Geometry.h"
#include "..\Headers\Vertex_Buffer.h"
#include "..\Headers\Index_Buffer.h"


class Terrain : public WorldTransform
{
public:

    Terrain();
    ~Terrain()
	{
		Release();
	}

    BOOL Initialize( LPDIRECT3DDEVICE9 pDevice, char* rawFile, char* terrainTexture );
    void Render(     LPDIRECT3DDEVICE9 pDevice);
    void Release();

private:
    VertexBuffer m_vertbuff;
    IndexBuffer m_indexbuff;
    
	LPDIRECT3DTEXTURE9 m_pTexture;
    UCHAR* m_pHeight;
    
	UINT m_numVertices;
    UINT m_numIndices;
};

#endif