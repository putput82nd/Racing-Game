#ifndef _ENTITYMESSAGE_H
#define _ENTITYMESSAGE_H

#include "Message.h"
#include <d3dx9.h>
#include "EntityPropertyList.h"

enum EEntityMessage
{
	MSG_ADDENTITY,
	MSG_REMOVEENTITY,
	MSG_SETVELOCITY,
	MSG_SETFORCE,
	MSG_INCSCRIPTFLAG1,
	MSG_INCSCRIPTFLAG2,
	MSG_SETPOSITION,
	MSG_SETROTATION,
	MSG_ROTATEONX,
	MSG_ROTATEONY,
	MSG_ROTATEONZ,
	MSG_SETSTEERING

};

struct SMessageAddEntity : SMessage
{
	SMessageAddEntity(int *id, int type, D3DXVECTOR3 pos) : SMessage(CORE_ENTITY, MSG_ADDENTITY),
		ID(id), Type(type), Position(pos)
	{
	}
	int *ID;
	int Type;
	D3DXVECTOR3 Position;
};

struct SMessageRemoveEntity : SMessage
{
	SMessageRemoveEntity(int *id) : SMessage(CORE_ENTITY, MSG_REMOVEENTITY),
		ID(id)
	{
	}
	int *ID;
};

struct SMessageSetVelocity : public SMessage
{
	SMessageSetVelocity(int *id, D3DXVECTOR3 velocity):SMessage(CORE_ENTITY, MSG_SETVELOCITY)
	{
		ID = id;
		Velocity = velocity;
	}
	int *ID;
	D3DXVECTOR3 Velocity;
};

struct SMessageSetForce : public SMessage
{
	SMessageSetForce(int *id, D3DXVECTOR3 force) : SMessage(CORE_ENTITY, MSG_SETFORCE)
	{
		ID = id;
		Force = force;
	}
	int *ID;
	D3DXVECTOR3 Force;
};

struct SMessageIncScriptFlag1 : public SMessage
{
	SMessageIncScriptFlag1(int id) : SMessage(CORE_ENTITY, MSG_INCSCRIPTFLAG1), ID(id)
	{}
	int ID;
};

struct SMessageSetPosition : public SMessage
{
	SMessageSetPosition(int *id, D3DXVECTOR3 position) : SMessage(CORE_ENTITY, MSG_SETFORCE)
	{
		ID = id;
		Position = position;
	}
	int *ID;
	D3DXVECTOR3 Position;
};

struct SMessageSetRotation : public SMessage
{
	SMessageSetRotation(int *id, D3DXVECTOR3 axis, float degree) : SMessage(CORE_ENTITY, MSG_SETFORCE)
	{
		ID = id;
		Axis = axis;
		Degree = degree;
	}
	int *ID;
	D3DXVECTOR3 Axis;
	float Degree;
};

struct SMessageAddRotationOnX : public SMessage
{
	SMessageAddRotationOnX(int *id, float degree):SMessage(CORE_ENTITY, MSG_ROTATEONX)
	{
		ID = id;
		Degree = degree;
	}
	int *ID;
	float Degree;
};

struct SMessageAddRotationOnY : public SMessage
{
	SMessageAddRotationOnY(int *id, float degree):SMessage(CORE_ENTITY, MSG_ROTATEONY)
	{
		ID = id;
		Degree = degree;
	}
	int *ID;
	float Degree;
};

struct SMessageAddRotationOnZ : public SMessage
{
	SMessageAddRotationOnZ(int *id, float degree):SMessage(CORE_ENTITY, MSG_ROTATEONZ)
	{
		ID = id;
		Degree = degree;
	}
	int *ID;
	float Degree;
};

struct SMessageSetSteering : public SMessage
{
	SMessageSetSteering(int *id, int steeringtype) : SMessage(CORE_ENTITY, MSG_SETSTEERING)
	{
		ID = id;
		SteeringType = steeringtype;
	}
	int *ID;
	int SteeringType;
};

#endif