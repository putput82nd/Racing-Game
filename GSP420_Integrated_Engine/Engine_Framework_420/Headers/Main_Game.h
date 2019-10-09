#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "..\Headers\GfxStats.h"
#include "..\Headers\Framework.h"
#include "..\Headers\Vertex_Buffer.h"
#include "..\Headers\Index_Buffer.h"
#include "..\Headers\World_Transformations.h"
#include "..\Headers\Geometry.h"
#include "..\Headers\Text.h"
#include "..\Headers\Mesh_Objects.h"
#include "..\Headers\Terrain.h"
#include "..\Headers\Camera.h"
#include "..\Headers\Sound.h"
#include "..\Headers\Interface.h"
#include "PlayerClass.h"

//MAIN PLACE WHERE GAME DATA TAKES PLACE
//ALMOST ALL DIRECTX CHANGES TAKES PLACE HERE

class GameApp : public BaseApp
{
public:
    GameApp();
    ~GameApp()
	{
		Release(); 
	}

    void SetFramework( Framework* pFramework );
    
	BOOL Initialize();
    
	virtual void Release();
    virtual void OnCreateDevice( LPDIRECT3DDEVICE9 pDevice );
    virtual void OnResetDevice( LPDIRECT3DDEVICE9 pDevice );
    virtual void OnLostDevice();
    virtual void OnDestroyDevice();
	virtual void OnUpdateFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime);
    virtual void OnRenderFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime );

    virtual void ProcessInput( long xDelta, long yDelta, long zDelta, BOOL* pMouseButtons, BOOL* pPressedKeys, float elapsedTime );

	void updateCamera();

	AI_Interface AI1;
	AI_Interface AI2;

	Opponent car1;
	Opponent car2;
	
	D3DXVECTOR3 reference;
	D3DXVECTOR3 movedReference;
	D3DXVECTOR3 camPos; 
	D3DXVECTOR3 camLook;

	D3DXMATRIX rotation;

	Player car3;
	

private:
    Framework*         m_pFramework;
    Text               m_font;
    Terrain            m_terrain;
    Camera             m_camera;
	//Interface		   m_interface;
	D3DLIGHT9          m_light;
	Mesh			   m_meshObject1;
	Mesh			   m_meshObject2;
	Mesh			   m_meshObject3;
	Mesh			   m_meshObject4;
	
	MeshInstance*	   m_meshCar1;
	MeshInstance*	   m_meshCar2;
	MeshInstance*	   m_meshTrack;
	MeshInstance*	   m_meshCar3;  // Player
	
	D3DXVECTOR3 up;

    LPD3DXSPRITE        m_pTextSprite;

    char    m_fps[100];
    BOOL    m_showInstructions;
	float radian;
};
#endif