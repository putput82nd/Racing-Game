#include "stdafx.h"

#include <iostream>
#include <windows.h>
using std::cout;

//define this to output to a file
#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

// Method for StartingPosition state

StartingPosition* StartingPosition::Instance()
{
	static StartingPosition instance;

	return &instance;
}

void StartingPosition::Enter(Opponent* pOpponent)
{
	pOpponent->state = 0;
	//  Ensure that the opponent begins at the starting position
	if (pOpponent->Location() != startPosition)
	{
		pOpponent->ChangeLocation(startPosition);
		pOpponent->track = 0;
	}
	pOpponent->intialize();
}

void StartingPosition::Execute(Opponent* pOpponent)
{
	pOpponent->frameRateCalc();
	Sleep(15);
	pOpponent->ChangeState(OptimalPath::Instance());
}

void StartingPosition::Exit(Opponent* pOpponent)
{
	pOpponent->ChangeLocation(optimalTrack);	
}

// Method for OptimalPath state

OptimalPath* OptimalPath::Instance()
{
	static OptimalPath instance;

	return &instance;
}

void OptimalPath::Enter(Opponent* pOpponent)
{
	pOpponent->state = 1;
	pOpponent->optimal = true;
	pOpponent->track = 1;
	//pOpponent->subScript++;
		//  Ensure that the opponent begins at the starting position
	if (pOpponent->Location() != optimalTrack)
	{
		pOpponent->ChangeLocation(optimalTrack);
	}
}

void OptimalPath::Execute(Opponent* pOpponent)
{
	pOpponent->frameRateCalc();
	pOpponent->updatePos();
}

void OptimalPath::Exit(Opponent* pOpponent)
{
	pOpponent->ChangeLocation(overtakeTrack);	
}

// Method for OvertakePath state

OvertakePath* OvertakePath::Instance()
{
	static OvertakePath instance;

	return &instance;
}

void OvertakePath::Enter(Opponent* pOpponent)
{
	pOpponent->state = 2;
	pOpponent->optimal = false;
	pOpponent->track = 2;
		//  Ensure that the opponent begins at the starting position
	if (pOpponent->Location() != overtakeTrack)
	{
		pOpponent->ChangeLocation(overtakeTrack);		
	}
}

void OvertakePath::Execute(Opponent* pOpponent)
{
	pOpponent->frameRateCalc();
	pOpponent->updatePos();
}

void OvertakePath::Exit(Opponent* pOpponent)
{
	pOpponent->ChangeLocation(optimalTrack);
}