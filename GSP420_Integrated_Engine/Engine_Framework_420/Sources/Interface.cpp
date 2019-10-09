#include "..\Headers\Interface.h"


void Interface::Initialize()
{
	SetCameraPos(0.0f, 60.0f, 10.0f);
}


void Interface::SetCameraPos( float X, float Y, float Z )
{
	CameraPos.x = X;
	CameraPos.y = Y;
	CameraPos.z = Z;

	//return CameraPos;
}
