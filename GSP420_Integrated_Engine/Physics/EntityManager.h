#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include "BaseManager.h"


enum EEDType
{
	EDTYPE_POSITION,
	EDTYPE_VELOCITY,
	EDTYPE_FORCE,
	EDTYPE_ROTATION,
	EDTYPE_PHYSICSID,
	EDTYPE_SCRIPTFLAG1,
	EDTYPE_SCRIPTFLAG2,
	EDTYPE_STEERINGTYPE,
	EDTYPE_ACCELERATION,
	EDTYPE_DECELERATION
};

struct EntityData
{
	int Type;

	//Physics attributes
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Velocity;
	D3DXVECTOR3 Force;
	D3DXQUATERNION Rotation;
	int PhysicsID;
	float Acceleration;
	float MaxVelocity;
	float SteeringAngle;
	float BrakingPower;

	//Graphics attribures

	//Scripting attributes
	int ScriptFlag1;
	int ScriptFlag2;

	//AI attributes
		//Steering
	int SteeringType;

	EntityData()
	{
		Position = D3DXVECTOR3(0,0,0);
		Velocity = D3DXVECTOR3(0,0,0);
		Force = D3DXVECTOR3(0,0,0);
		Rotation = D3DXQUATERNION(0,0,0,1);
		PhysicsID = -1;
		SteeringType = 0;
		Acceleration = 0;
		MaxVelocity = 0;
		SteeringAngle = 0;
		BrakingPower = 0;

		ScriptFlag1 = 0;
		ScriptFlag2 = 0;
	}

	void SetData(int type, void* value)
	{
		switch(type)
		{
		case EDTYPE_POSITION:
			{
				Position = *((D3DXVECTOR3*)value);
				break;
			}
		case EDTYPE_VELOCITY:
			{
				Velocity = *((D3DXVECTOR3*)value);
				break;
			}
		case EDTYPE_FORCE:
			{
				Force = *((D3DXVECTOR3*)value);
				break;
			}
		case EDTYPE_ROTATION:
			{
				Rotation = *((D3DXQUATERNION*) value);
				break;
			}
		case EDTYPE_PHYSICSID:
			{
				PhysicsID = *((int*)value);
				break;
			}
		case EDTYPE_SCRIPTFLAG1:
			{
				ScriptFlag1 = *((int*)value);
				break;
			}
		case EDTYPE_SCRIPTFLAG2:
			{
				ScriptFlag2 = *((int*)value);
				break;
			}
		case EDTYPE_STEERINGTYPE:
			{
				SteeringType = *((int*)value);
				break;
			}
		case EDTYPE_ACCELERATION:
			{
				MaxVelocity = *((float*)value);
				Acceleration = *((float*)value);
				SteeringAngle = *((float*)value);
				Velocity = *((D3DXVECTOR3*)value);
				break;
			}
		case EDTYPE_DECELERATION:
			{
				MaxVelocity = *((float*)value);
				BrakingPower = *((float*)value);
				SteeringAngle = *((float*)value);
				Velocity = *((D3DXVECTOR3*)value);
				break;
			}
		}
	}

};
class EntityManager : public BaseManager<EntityData>
{
public:
	EntityManager();
	~EntityManager();

	void Clear();
	int AddItem(int type,int *id_OUT);
	bool RemoveItem(int id);
	void SetValue(int *id, int type, void* value);
private:
};

#endif
