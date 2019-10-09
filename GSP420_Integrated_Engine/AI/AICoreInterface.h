#ifndef AICORE_H
#define AICORE_H

#include "stdafx.h"

//class State;

class AI_Interface : public Opponent
{
public: 
	AI_Interface(const Opponent& current);
	AI_Interface();

	int GetState();				// Current state getter
	Opponent SetState(State*);			// Current state setter

	int GetLocation();		// Current location getter
	Opponent SetLocation(track_type);	// Current location setter

	D3DXVECTOR3 GetVehPos();		// Current vehicle position getter

	void SetWaypointOpt(int, int, int);		// Optimal waypoints setter

	void SetWaypointOvt(int, int, int);		// Overtake waypoints setter
	Opponent curOpponent;
	void update(Opponent);
};
#endif