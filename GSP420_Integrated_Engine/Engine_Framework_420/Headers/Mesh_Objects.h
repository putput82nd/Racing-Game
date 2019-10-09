#ifndef MESH_OBJECTS_H
#define MESH_OBJECTS_H

#include "..\Headers\GfxStats.h"
#include "..\Headers\World_Transformations.h"

class Mesh
{
public:
    Mesh();
    ~Mesh() 
	{
		Release();
	}

    BOOL Load( LPDIRECT3DDEVICE9 pDevice, char* file, char* textFile );
    void Release();

    LPD3DXMESH GetMesh()	{ return m_pMesh; }
    DWORD GetNumMaterials() { return m_numMaterials; }

    D3DMATERIAL9* GetMeshMaterial( int i )		{ return &m_pMeshMaterials[i]; }
    LPDIRECT3DTEXTURE9 GetMeshTexture( int i )	{ return m_ppMeshTextures[i]; }

private:
    LPD3DXMESH m_pMesh;
    DWORD m_numMaterials;
    D3DMATERIAL9 *m_pMeshMaterials;
    LPDIRECT3DTEXTURE9 *m_ppMeshTextures;
};


class MeshInstance : public WorldTransform
{
public:
    MeshInstance();
    ~MeshInstance()
	{
		Release(); 
	}

    void Release();
    void SetMesh( Mesh* pMesh );
    void Render(  LPDIRECT3DDEVICE9 p_device );

private:
    Mesh* m_pMesh;
};

#endif