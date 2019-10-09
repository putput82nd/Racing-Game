#ifndef BASEGAMEOPPONENT_H
#define BASEGAMEOPPONENT_H

// A base class for a game opponent

class BaseGameOpponent
{
private:
	
	
	// Ensure the ID is set correctly by verifying the value pass to the method is greater than or equal to the next valid ID before setting the ID and incrementing the next valid ID
	void SetID(int val);

public:
	BaseGameOpponent(int id)
	{
		SetID(id);
	}
	
	// Next valid ID
	static int m_iNextValidID;
	
	// Opponent identifying number
	int m_ID;

	BaseGameOpponent();
	virtual ~BaseGameOpponent(){}

	// Update function for all entities
	virtual void Update() = 0;

	int ID()const{return m_ID;}
	
	
};

#endif