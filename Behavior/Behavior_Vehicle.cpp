#include "Behavior_Vehicle.h"
#include "../GameEntity/BaseGameEntity.h"

/// <summary>Constructor method</summary>
/// \param[in] pOwner the vehicle game agent
Behavior_Vehicle::Behavior_Vehicle(BaseGameEntity* pOwner) : m_pOwner(pOwner)
{

}

/// <summary>Destructor method</summary>
Behavior_Vehicle::~Behavior_Vehicle()
{
	m_visited.clear();
	m_unvisited.clear();
}

/// <summary>Method that update the game agent when the timer ticks in the game engine</summary>
/// \param[in] deltaTime the time interval between two consecutive calls to the update method
void Behavior_Vehicle::Update(float deltaTime)
{
	if(m_unvisited.empty())
	{
		Start();
		return;
	}

	BaseGameEntity* pStation=m_unvisited.front();

	Point2D disp=pStation->GetPosition() - m_pOwner->GetPosition();
	float dist=disp.Length();

	float d=m_speed * deltaTime;
	if(d <= dist)
	{
		Point2D dir=disp.Normalize();
		m_pOwner->SetPosition(m_pOwner->GetPosition() + dir * d);

	}
	else
	{
		m_pOwner->SetPosition(pStation->GetPosition());
		m_visited.push_back(m_unvisited.front());
		m_unvisited.pop_front();
	}

}

/// <summary>Method that renders the images and drawings of the vehicle agent</summary>
/// \param[in] buffer the image buffer onto which the images and drawings of the vehicle agent are rendered and displayed using the double-buffer techniques
void Behavior_Vehicle::Render(BITMAP* buffer)
{
	std::list<BaseGameEntity*>::iterator pos;
	BaseGameEntity* pStation1=NULL;
	BaseGameEntity* pStation2=NULL;
	for(pos=m_visited.begin(); pos!=m_visited.end(); ++pos)
	{
		pStation1=pStation2;
		pStation2=*pos;
		if(pStation1!=NULL)
		{
			line(buffer, pStation1->GetPositionOnSimulator().GetX(), pStation1->GetPositionOnSimulator().GetY(),
				pStation2->GetPositionOnSimulator().GetX(), pStation2->GetPositionOnSimulator().GetY(), 0xFF0000);
		}
	}

	if(!m_unvisited.empty() && !m_visited.empty())
	{
		pStation1=m_unvisited.front();
		pStation2=m_visited.back();

		line(buffer, pStation1->GetPositionOnSimulator().GetX(), pStation1->GetPositionOnSimulator().GetY(),
				pStation2->GetPositionOnSimulator().GetX(), pStation2->GetPositionOnSimulator().GetY(), 0x00FFFF);

		circlefill(buffer, pStation1->GetPositionOnSimulator().GetX(), pStation1->GetPositionOnSimulator().GetY(), 7, makecol(255, 255, 255));
		circle(buffer, pStation1->GetPositionOnSimulator().GetX(), pStation1->GetPositionOnSimulator().GetY(), 7, makecol(0, 255, 0));

		circlefill(buffer, pStation2->GetPositionOnSimulator().GetX(), pStation2->GetPositionOnSimulator().GetY(), 7, makecol(255, 255, 255));
		circle(buffer, pStation2->GetPositionOnSimulator().GetX(), pStation2->GetPositionOnSimulator().GetY(), 7, makecol(0, 255, 0));
	}
}

/// <summary>Method that add the customer agent visited by the vehicle agent to its route</summary>
/// \param[in] pStation the customer agent added to the vehicle's route
void Behavior_Vehicle::RegisterStation(BaseGameEntity* pStation)
{
	m_unvisited.push_back(pStation);
}

/// <summary>Method that start the engine of the vehicle agent</summary>
void Behavior_Vehicle::Start()
{
	while(!m_visited.empty())
	{
		m_unvisited.push_back(m_visited.front());
		m_visited.pop_front();
	}

	//m_visited.swap(m_unvisited);

	BaseGameEntity* pStation=m_unvisited.front();

	m_pOwner->SetPosition(pStation->GetPosition());

	m_visited.push_back(pStation);
	m_unvisited.pop_front();


}
