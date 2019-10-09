#include "AICoreInterface.h"
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9tex.h>
#include <Dxerr.h>  

AI_Interface::AI_Interface()
{
}
AI_Interface::AI_Interface(const Opponent& current)
{
	curOpponent = current;
}
void AI_Interface::update(Opponent current)
{
	curOpponent = current;
}
int AI_Interface::GetState()				// Current state getter
{
	return curOpponent.state; //0 = starting Position, 1 = optimal path, 2 = overtake path
}

Opponent AI_Interface::SetState(State* newState) // Current state setter (newState = StartingPosition::Instance, OptimalPath::Instance, OvertakePath::Instance)
{
	curOpponent.ChangeState(newState);
	return curOpponent;
}

int AI_Interface::GetLocation()		// Current location getter
{
	return curOpponent.track;  // 0 = starting position, 1 = optimal path, 2 = overtake path
}

Opponent AI_Interface::SetLocation(track_type newLocation)	// Current location setter (newLocation = optimalTrack, overtakeTrack, startingPosition)
{
	curOpponent.ChangeLocation(newLocation);
	return curOpponent;
}

D3DXVECTOR3 AI_Interface::GetVehPos()			// Current vehicle position getter
{
	return curOpponent.vehiclePos;
}