#ifndef PLAYER_H
#define PLAYER_H

// A class defining the player vehicle

#include <string>
#include <cassert>

#include "BaseGameOpponent.h"
#include "OpponentOwnedStates.h"
#include "Vector2D.h"

class State;

class Player : public BaseGameOpponent
{
private:
	track_type m_Location;
	
	D3DXVECTOR3 acceleration;
	D3DXVECTOR3 steeringForce;
	D3DXVECTOR3 desiredVel;
	
	LONGLONG timeStart, timeEnd, timerFrequency;
	double timeScale;
	float frameRate;
	Vector2D ToEvader;
	
public:
		
	Player();
	~Player();

	D3DXVECTOR3 velocity;
	D3DXVECTOR3 heading;
	D3DXVECTOR3 vehiclePos;
	D3DXVECTOR3 targetPoint;
	D3DXVECTOR3 target;
	D3DXVECTOR3 oldHeading;

	int laps;
	float mass;
	float maxSpeed;
	float maxForce;
	float maxTurn;	
	float distance(D3DXVECTOR3);

	void updatePos();
	void frameRateCalc();
	void timerStart();
	void Update();
};

#endif