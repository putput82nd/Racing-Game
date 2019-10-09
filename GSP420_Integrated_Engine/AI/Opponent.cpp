#include "stdafx.h"

// Create an opponent

Opponent::Opponent(int id):BaseGameOpponent(id), m_Location(startPosition),	m_pCurrentState(StartingPosition::Instance())
{
	m_pCurrentState->Enter(this);
}
void Opponent::intialize()
{	
	laps = 0;
	mass = 700.0;
	maxSpeed = 100.0;
	subScript = 0;
	optimal = true;	
	vehiclePos = assignWaypoints(); //Setting initial vehicle point
	subScript++;
	setNextTarget(assignWaypoints()); //Setting first waypoint
	timerStart();
	heading= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	angle = 3.14;
}
void Opponent::timerStart()
{
	QueryPerformanceFrequency((LARGE_INTEGER*) &timerFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*) &timeStart);

	timeScale = 1.0 / timerFrequency;
}
void Opponent::frameRateCalc()
{
	QueryPerformanceCounter((LARGE_INTEGER*) &timeEnd);

	frameRate = (timeEnd - timeStart) * timeScale;

	timeStart = timeEnd;
}
// Change opponent state
void Opponent::ChangeState(State* pNewState)
{
	// Ensure both states are valid before calling their methods
	assert (m_pCurrentState && pNewState);

	// Call exit method of existing state
	m_pCurrentState->Exit(this);

	// Change to the new state
	m_pCurrentState = pNewState;

	// Call the entry method of the new state
	m_pCurrentState->Enter(this);
}
Opponent::Opponent()
{
	m_Location = startPosition;
	m_pCurrentState = StartingPosition::Instance();
	m_pCurrentState->Enter(this);
}
Opponent::~Opponent() // Destructor
{
	m_iNextValidID = m_ID;
}
void Opponent::seek() //Seek function
{
	desiredVel = targetPoint - vehiclePos; //Setting the desired velocity - target position - current position
	D3DXVec3Normalize(&desiredVel, &desiredVel); //Normalize the desired velocity to get only a direction
	desiredVel *= maxSpeed; //multiply desired velocity by the max speed 
	steeringForce = (desiredVel - velocity); //finding the steering force by subtracting velocity from desired velocity 
}

void Opponent::setNextTarget(D3DXVECTOR3 target) //Setting next target x,y,z and taking velocity to 0 when setting new target
{
	targetPoint.x = target.x; 
	targetPoint.y = target.y;
	targetPoint.z = target.z;
	velocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}
void Opponent::followPath(bool optimal) 
{
	if (distance(targetPoint) < 1.6) //If approaching the target
	{
		if (optimal) //If on the optimal path
		{

			subScript++; //adding one to the subscript

			setNextTarget(assignWaypoints()); //set the next waypoint for optimal path
			if (subScript == 29) //Resetting the subscript if reached the end of array
			{
				subScript = -1;
			}
		}
		else
		{
			subScript++; //adding one to the subscript

			setNextTarget(assignWaypoints());//set the next waypoint for the optimal path

			if (subScript == 29)//Resetting the subscript if reached the end of array
			{
				subScript = -1;
			}
		}
	}
}
void Opponent::Update()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Execute(this);
	}
}
float Opponent::distance(D3DXVECTOR3 point)
{
	return (sqrt((point.x - vehiclePos.x) + (point.y - vehiclePos.y) + (point.z - vehiclePos.z))); /*finding the distance between the vehicle
																								   position and a target*/
}
void Opponent::updatePos() //Updating the steering of the vehicle
{
	D3DXVECTOR3 toTarget =  target - vehiclePos ;
	
	D3DXVec3Normalize(&toTarget, &toTarget);
	
	float velocitySQlen; //variable to store the squared length of the velocity

	followPath(optimal); //calling follow path function

	seek(); //call seek function

	velocitySQlen = ((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z)); //finding the squared length of velocity

	acceleration = steeringForce / mass; //finding the acceleration by dividing the steering force by the mass

	velocity += acceleration * .0001; //adding the acceleration to the velocity

	D3DXVec3Normalize(&velocity, &velocity); //normailizing the velocity to find a direction

	velocity *= maxSpeed; //multiplying velocity and maxspeed 

	vehiclePos += velocity * .0001; //updating vehicle position with velocity times .01	

	D3DXVec3Normalize(&heading, &velocity); //FIX THIS

	heading = D3DXVECTOR3(heading.x + vehiclePos.x, 0.0f, heading.z + vehiclePos.z);

	//velocity = D3DXVECTOR3(velocity.x + vehiclePos.x, 0.0f, velocity.z + vehiclePos.z);

	dot = D3DXVec3Dot(&toTarget, &heading);

	angle = acos(dot);	

	/*if ( velocitySQlen > 0.00000001 ) //updating the heading with velocity unless the squared lenght of the velocity is a very small number
	{
	D3DXVec3Normalize(&heading, &velocity);
	}*/
}
D3DXVECTOR3 Opponent::assignWaypoints()
{

	waypoint Optimal[] = //The optimal path array of structure
	{
		{ -41, 0, 169 },
		{ -10, 0, 170 }, 
		{ 30, 0, 162 },
		{ 61, 0, 147},
		{ 80, 0, 120},
		{ 96, 0, 106},
		{ 111, 0, 85},
		{ 125, 0, 55},
		{ 132, 0, 28},
		{ 134, 0, 7},
		{ 135, 0, -9},
		{ 136, 0, -38},
		{ 131, 0, -74},
		{ 114, 0, -119},
		{ 91, 0, -147},
		{ 44, 0, -179},
		{ 1, 0, -187},
		{ -30, 0, -186},
		{ -60, 0, -182},
		{ -96, 0, -177},
		{ -135, 0, -171},
		{ -167, 0, -160},
		{ -186, 0, -133},
		{ -206, 0, -88},
		{ -211, 0, -30},
		{ -209, 0, 13},
		{ -203, 0, 56},
		{ -187, 0, 101},
		{ -152, 0, 125},
		{ -101, 0, 145}
	};

	waypoint Overtake[] = //The overtake array of structure
	{
		{-41, 0, 195}, 
		{-11, 0, 194}, 
		{40, 0, 192}, 
		{73, 0, 163}, 
		{82, 0, 152}, 
		{114, 0, 118}, 
		{130, 0, 90},  
		{142, 0, 58}, 
		{146, 0, 28}, 
		{148, 0, 7}, 
		{151, 0, -9}, 
		{148, 0, -42}, 
		{145, 0, -80}, 
		{130, 0, -136}, 
		{100, 0, -163}, 
		{49, 0, -209}, 
		{-1, 0, -216}, 
		{-33, 0, -210}, 
		{-63, 0, -206}, 
		{-99, 0, -199}, 
		{-53, 0, -198}, 
		{-179, 0, -170}, 
		{-202, 0, -138}, 
		{-220, 0, -89}, 
		{-226, 0, -28}, 
		{-219, 0, 14}, 
		{-213, 0, 60}, 
		{-198, 0, 118}, 
		{-157, 0, 140}, 
		{-109, 0, 157} 
	};
	if (subScript == 1)
		laps++;

	if (optimal)
	{
		target.x = Optimal[subScript].x;
		target.y = Optimal[subScript].y;
		target.z = Optimal[subScript].z;
	}
	else
	{
		target.x = Overtake[subScript].x;
		target.y = Overtake[subScript].y;
		target.z = Overtake[subScript].z;
	}

	return target;
}