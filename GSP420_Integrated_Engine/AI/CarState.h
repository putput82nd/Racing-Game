#ifndef CARSTATE_H
#define CARSTATE_H

// Creating an abstract base class to define an interface for a state

class Opponent;

class State
{
public:

	virtual ~State(){}

	// This will execute when the state is entered
	virtual void Enter(Opponent*) = 0;

	// This is the State's normal update function
	virtual void Execute(Opponent*) = 0;

	// This will execute when the state is exited
	virtual void Exit(Opponent*) = 0;

};

#endif