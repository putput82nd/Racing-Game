#ifndef _UTIL_H
#define _UTIL_H

#include "btBulletDynamicsCommon.h"

struct WorldInfo
{
	WorldInfo(): Gravity(0,-10,0),WorldAabbMin(-10000,-10000,-10000),
		WorldAabbMax(10000,10000,10000),MaxProxies(1024){}
	btVector3 Gravity;
	btVector3 WorldAabbMin;
	btVector3 WorldAabbMax;
	int MaxProxies;
};

struct SphereInfo
{
	SphereInfo(): orgin(0,0,0),localInertia(0,0,0),friction(1),restitution(0.0),linearDamping(0),
		angularDamping(0),mass(1),radius(1){}
	btVector3 orgin;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	float mass;
	float radius;
	btVector3 localInertia;
};

struct BoxInfo
{
	BoxInfo(): orgin(0,0,0),localInertia(0,0,0),friction(1),restitution(0.0),linearDamping(0),
		angularDamping(0),mass(1),halfscale(1,1,1){}
	btVector3 orgin;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	float mass;
	btVector3 halfscale;
	btVector3 localInertia;
};

struct CarInfo
{
	CarInfo(): orgin(0,0,0),localInertia(0,0,0),friction(1),restitution(0.0),linearDamping(0),
		angularDamping(0),mass(1),halfscale(1,1,1), maxVelocity(0),acceleration(0),steeringAngle(0), brakingPower(0){}
	btVector3 orgin;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	float mass;
	float maxVelocity;
	float acceleration;
	float steeringAngle;
	float brakingPower;
	btVector3 halfscale;
	btVector3 localInertia;
};

#endif