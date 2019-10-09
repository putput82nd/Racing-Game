#ifndef INTERFACE_H
#define INTERFACE_H

//#include "Main_Game.h"
#include "..\Headers\GfxStats.h"



class Interface
{
public:

	Interface();
    ~Interface()
	{
		//Release(); 
	}

	D3DXVECTOR3 CameraPos;


	void Initialize();
	void SetInterface();

	//MESH FUNCTIONS
	void NewMesh();
	void SetMesh();
	void PlaceMesh();

	//FMOD MUSIC FUNCTIONS
	void SetBackGroundSong();
	void Set3DSong();
	void SetSoundEffect();
	void SetReverb();

	//TERRAIN FUNCTIONS
	void SetTerrain();

	//CAMERA FUNCTIONS
	void SetCameraPos( float X, float Y, float Z );
	D3DXVECTOR3 GetCameraPos() { return CameraPos; }



private:




};




#endif