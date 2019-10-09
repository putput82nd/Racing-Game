#include "stdafx.h"
#include "PlayerClass.h"

// Creating a player object

Player::Player()
{	
	laps = 0;
	mass = 700;
	maxSpeed = 120;
	m_Location = startPosition;
	vehiclePos = D3DXVECTOR3(-41, 0, 169);
	velocity = D3DXVECTOR3(0.1f, 0.0f, 0.1f);
	D3DXVec3Normalize(&heading, &velocity);
}

Player::~Player()
{

}

void Player::timerStart()
{
	QueryPerformanceFrequency((LARGE_INTEGER*) &timerFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*) &timeStart);

	timeScale = 1.0 / timerFrequency;
}

void Player::frameRateCalc()
{
	QueryPerformanceCounter((LARGE_INTEGER*) &timeEnd);
	
	frameRate = (timeEnd - timeStart) * timeScale;

	timeStart = timeEnd;
}

void Player::Update()
{
	updatePos();
}

float Player::distance(D3DXVECTOR3 point)
{
	return (sqrt((point.x - vehiclePos.x) + (point.y - vehiclePos.y) + (point.z - vehiclePos.z))); /*finding the distance between the vehicle
																								   position and a target*/
}

void Player::updatePos() //Updating the steering of the vehicle
{
	float velocitySQlen; //variable to store the squared length of the velocity

	velocitySQlen = ((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z)); //finding the squared length of velocity
	
	acceleration = velocity / mass; //finding the acceleration by dividing the steering force by the mass
	
	//acceleration = D3DXVECTOR3( heading.x * 1.1, 0.0, heading.z * 1.1 );
	
	acceleration *= frameRate;

	velocity += acceleration; //adding the acceleration to the velocity
	
	D3DXVec3Normalize(&velocity, &velocity); //normailizing the velocity to find a direction
	
	velocity *= maxSpeed; //multiplying velocity and maxspeed 
	
	vehiclePos += velocity * .0001; //updating vehicle position with velocity times .01

	D3DXVec3Normalize(&heading, &velocity);	

	heading = D3DXVECTOR3(heading.x + vehiclePos.x, 0.0f, heading.z + vehiclePos.z);

	velocity = D3DXVECTOR3(velocity.x + vehiclePos.x, 0.0f, velocity.z + vehiclePos.z);

	/*if ( velocitySQlen > 0.00000001 ) //updating the heading with velocity unless the squared lenght of the velocity is a very small number
	{
		D3DXVec3Normalize(&heading, &velocity);
	}*/
}