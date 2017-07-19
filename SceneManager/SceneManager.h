#ifndef _H_SCENE_MANAGER_H
#define _H_SCENE_MANAGER_H

#include <string>
#include "../Util/Rect2D.h"
#include "../GameEntity/BaseGameEntity.h"
#include <vector>
#include <list>
#include "../allegro_include/allegro.h"

/// <summary>Class that represents the game world of the game engine</summary>
class SceneManager
{
public:
	/// <summary>Method that returns the singleton instance of the class</summary>
	/// <returns>The singleton instance of the class</returns>
	static SceneManager* Instance();
	/// <summary>Destructor method</summary>
	virtual ~SceneManager();

private:
	/// <summary>Private constructor</summary>
	SceneManager();
	/// <summary>Private copy constructor</summary>
	/// \param[in] rhs the object to be copied
	SceneManager(const SceneManager& rhs);
	/// <summary>Private assignment operator</summary>
	/// \param[in] rhs the object to be copied
	SceneManager& operator=(const SceneManager& rhs);

public:
	/// <summary>Method that updates all the game agents in the world when the timer ticks in the game engine</summary>
	/// \param[in] deltaTime the time duration between two consecutive calls to the update method
	void Update(float deltaTime);
	/// <summary>Method taht renders all the game agents in the world</summary>
	/// \param[in] buffer the image buffer onto which the game agents are rendered and displayed using double-buffer techniques
	void Render(BITMAP* buffer);

protected:
	/// <summary>Method that initialize the game world</summary>
	/// \param[in] filename the configuration file of the application
	/// <returns>The boolean flag indicating whether the loading and initialization is successful</returns>
	bool Init(std::string filename);

public:
	/// <summary>Method that returns the activity region of the game engine</summary>
	/// <returns>The activity region of the game engine</returns>
	Rect2D GetActivityZone() { return m_ActivityZone; }
private:
	/// <summary>the activity region of the game engine</summary>
	Rect2D m_ActivityZone;

private:
	/// <summary>The boolean flag indicating whether the station id should be visible</summary>
	 bool m_StationIdVisible;
	 /// <summary>The boolean flag indicating whether the links between any two customers are visible</summary>
	 bool m_StationLinkVisible;
	 /// <summary>The boolean flag indicating whether the dot of customer is visible</summary>
	 bool m_StationDotVisible;
protected:
	/// <summary>Method that renders the customer id on each customer agent</summary>
	/// \param[in] buffer the image buffer onto which the station ids are rendered and displayed using double-buffer technique
	void RenderStationId(BITMAP* buffer);
	/// <summary>Method that renders the link between customer agent</summary>
	/// \param[in] buffer the image buffer onto which the links are rendered and displayed using double-buffer technique
	void RenderStationLink(BITMAP* buffer);
	/// <summary>Method that renders the customer dot on each customer agent</summary>
	/// \param[in] buffer the image buffer onto which the station dots are rendered and displayed using double-buffer technique
	void RenderStationDots(BITMAP* buffer);

private:
	/// <summary>list of customers to be visited by the vehicle agents</summary>
	std::vector<BaseGameEntity*> m_tour;
	/// <summary>The vehicle agent</summary>
	BaseGameEntity* m_pVehicle;

};

#define SimulatorSceneManager SceneManager::Instance()

#endif