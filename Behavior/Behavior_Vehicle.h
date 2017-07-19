#ifndef _H_BEHAVIOR_VEHICLE_H
#define _H_BEHAVIOR_VEHICLE_H

#include "Behavior.h"
#include <list>

class BaseGameEntity;

/// <summary>Class that represents the behavior expressed by the vehicle agent</summary>
class Behavior_Vehicle : public Behavior
{
public:
	/// <summary>Constructor method</summary>
	/// \param[in] pOwner the vehicle game agent
	Behavior_Vehicle(BaseGameEntity* pOwner);
	/// <summary>Destructor method</summary>
	virtual ~Behavior_Vehicle();

public:
	/// <summary>Method that update the game agent when the timer ticks in the game engine</summary>
	/// \param[in] deltaTime the time interval between two consecutive calls to the update method
	virtual void Update(float deltaTime);
	/// <summary>Method that renders the images and drawings of the vehicle agent</summary>
	/// \param[in] buffer the image buffer onto which the images and drawings of the vehicle agent are rendered and displayed using the double-buffer techniques
	virtual void Render(BITMAP* buffer);

public:
	/// <summary>Method that start the engine of the vehicle agent</summary>
	void Start();

public:
	/// <summary>Method that add the customer agent visited by the vehicle agent to its route</summary>
	/// \param[in] pStation the customer agent added to the vehicle's route
	void RegisterStation(BaseGameEntity* pStation);
	/// <summary>Method that set the speed of the vehicle agent</summary>
	/// \param[in] f the new value of vehicle speed
	void SetSpeed(float f) { m_speed=f; }

private:
	/// <summary>Customers that have already visited by the vehicle agent</summary>
	std::list<BaseGameEntity* > m_unvisited;
	/// <summary>Customers that are yet to be visited by the vehicle agent</summary>
	std::list<BaseGameEntity* > m_visited;
	/// <summary>The vehicle agent that express the behavior</summary>
	BaseGameEntity* m_pOwner;
	/// <summary>The speed of the vehicle</summary>
	float m_speed;
};
#endif