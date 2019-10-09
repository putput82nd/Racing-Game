#ifndef OPPONENT_OWNED_STATES_H
#define OPPONENT_OWNED_STATES_H

#ifndef TRACKLOCATION_H
#define TRACKLOCATION_H

enum track_type
{
	optimalTrack,
	overtakeTrack,
	startPosition
};

#endif

// All the states that can be assigned to the Opponent class

#include "CarState.h"
#include <d3d9.h>
#include <d3dx9tex.h>
#include <Dxerr.h> 
#include "BaseGameOpponent.h"
#include "Opponent.h"

class Opponent;

// This is the initial state of all opponent objects

class StartingPosition : public State

{
private:

	StartingPosition(){}

	StartingPosition(const StartingPosition&);

	StartingPosition& operator = (const StartingPosition&);

public:

	static StartingPosition* Instance();

	virtual void Enter(Opponent* opponent);

	virtual void Execute(Opponent* opponent);

	virtual void Exit(Opponent* opponent);
};

// In this state, the opponent will drive on the optimal driving path

class OptimalPath : public State
{
private:

	OptimalPath(){}

	OptimalPath(const OptimalPath&);

	OptimalPath& operator = (const OptimalPath&);

public:

	static OptimalPath* Instance();

	virtual void Enter(Opponent* opponent);

	virtual void Execute(Opponent* opponent);

	virtual void Exit(Opponent* opponent);
};

// In this state, the opponent will attempt to overtake the opponent by switching to the overtake path

class OvertakePath : public State
{
private:

	OvertakePath(){}

	OvertakePath(const OvertakePath&);

	OvertakePath& operator = (const OvertakePath&);

public:

	static OvertakePath* Instance();

	virtual void Enter(Opponent* opponent);

	virtual void Execute(Opponent* opponent);

	virtual void Exit(Opponent* opponent);
};

#endif