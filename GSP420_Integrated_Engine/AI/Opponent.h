#ifndef OPPONENT_H
#define OPPONENT_H

// A class defining the opponent vehicle

#include <string>
#include <cassert>

#include "BaseGameOpponent.h"
#include "OpponentOwnedStates.h"
#include "Vector2D.h"

class State;

class Opponent : public BaseGameOpponent
{
private:

	State* m_pCurrentState;
	track_type m_Location;
	D3DXVECTOR3 acceleration;
	D3DXVECTOR3 steeringForce;
	D3DXVECTOR3 desiredVel;
	LONGLONG timeStart, timeEnd, timerFrequency;
	Vector2D ToEvader;
	double dot;
	float timeScale;
	float frameRate;
	
	void seek();
	void followPath(bool);
	
public:
	
	Opponent(int id);
	Opponent();
	~Opponent();

	struct waypoint
	{
		int x,y,z;
	};
	
	waypoint Optimal[30], Overtake[30];	
	D3DXVECTOR3 target;
	D3DXVECTOR3 assignWaypoints();
	D3DXVECTOR3 oldHeading;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 heading;
	D3DXVECTOR3 vehiclePos;
	D3DXVECTOR3 targetPoint;
	int subScript;
	int state;
	int track;
	int laps;
	float angle;
	bool optimal;
	bool PassedCar()const;
	float mass;
	float maxSpeed;
	float maxForce;
	float maxTurn;	
	float distance(D3DXVECTOR3);

	// Method to change the state of the opponent object to a new state.
	void ChangeState(State* new_state);
	void setNextTarget(D3DXVECTOR3);
	void Update();
	void ChangeLocation(const track_type loc){m_Location = loc;}
	void updatePos();
	void Update(D3DXVECTOR3);
	void frameRateCalc(); //Calculate Framerate
	void timerStart();
	void intialize();
	track_type Location()const{return m_Location;}	
};

#endif