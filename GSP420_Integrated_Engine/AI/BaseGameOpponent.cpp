#include "stdafx.h"
#include <cassert>

BaseGameOpponent::BaseGameOpponent()
{
}
int BaseGameOpponent::m_iNextValidID = 0;

// The following must be called within each constructor to make sure the ID is set correctly

void BaseGameOpponent::SetID(int val)
{
	// Ensure the val is >= the next available ID
	assert ((val >= m_iNextValidID) && "<BaseGameOpponent::SetID>: invalid ID"); // FAILING (m_ID isn't incrementing when we creating new opponents in the aiinterfacecore

	m_ID = val;

	m_iNextValidID = m_ID + 1;
}
