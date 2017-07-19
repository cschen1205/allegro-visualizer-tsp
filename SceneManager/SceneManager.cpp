#include "SceneManager.h"
#include "../Application/Application.h"
#include "../tinyxml/tinyxml.h"
#include "../Behavior/Behavior_Vehicle.h"
#include <cfloat>
#include <string>
#include <sstream>

/// <summary>Destructor method</summary>
SceneManager::SceneManager() :  m_StationIdVisible(true), m_StationLinkVisible(false), m_StationDotVisible(true)
{
	m_pVehicle = NULL;
	if(!Init("tsp_gui.xml"))
	{
		Simulator->Quit();
	}
}

/// <summary>Private copy constructor</summary>
/// \param[in] rhs the object to be copied
SceneManager::SceneManager(const SceneManager& rhs)
{

}

/// <summary>Private assignment operator</summary>
/// \param[in] rhs the object to be copied
SceneManager& SceneManager::operator=(const SceneManager& rhs)
{
	return *this;
}

/// <summary>Method that returns the singleton instance of the class</summary>
/// <returns>The singleton instance of the class</returns>
SceneManager* SceneManager::Instance()
{
	static SceneManager theInstance;
	return &theInstance;
}

/// <summary>Destructor method</summary>
SceneManager::~SceneManager()
{
	if(m_pVehicle!=NULL)
	{
		delete m_pVehicle;
		m_pVehicle=NULL;
	}

	std::vector<BaseGameEntity*>::iterator pos;
	for(pos=m_tour.begin(); pos!=m_tour.end(); ++pos)
	{
		delete (*pos);
	}
	m_tour.clear();
}

/// <summary>Method that initialize the game world</summary>
/// \param[in] filename the configuration file of the application
/// <returns>The boolean flag indicating whether the loading and initialization is successful</returns>
bool SceneManager::Init(std::string filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str()))
	{
		return false;
	}

	TiXmlElement* pXmlRoot=doc.RootElement();

	m_ActivityZone.left=FLT_MAX;
	m_ActivityZone.right=-FLT_MAX;
	m_ActivityZone.top=FLT_MAX;
	m_ActivityZone.bottom=-FLT_MAX;

	if(m_pVehicle!=NULL)
	{
		delete m_pVehicle;
		m_pVehicle=NULL;
	}
	m_pVehicle=new BaseGameEntity();
	m_pVehicle->SetID("vehicle");
	Behavior_Vehicle* pBehavior=new Behavior_Vehicle(m_pVehicle);
	m_pVehicle->SetBehavior(pBehavior);

	float speed;
	pXmlRoot->QueryFloatAttribute("vehicle_velocity", &speed);
	pBehavior->SetSpeed(speed);



	for(TiXmlElement* pXmlElement1=pXmlRoot->FirstChildElement(); pXmlElement1!=NULL; pXmlElement1=pXmlElement1->NextSiblingElement())
	{
		if(strcmp(pXmlElement1->Value(), "vertex")==0)
		{
			float x=0;
			float y=0;
			std::string id=pXmlElement1->Attribute("id");
			pXmlElement1->QueryFloatAttribute("x", &x);
			pXmlElement1->QueryFloatAttribute("y", &y);

			if(m_ActivityZone.left > x)
			{
				m_ActivityZone.left=x;
			}
			if(m_ActivityZone.right < x)
			{
				m_ActivityZone.right=x;
			}
			if(m_ActivityZone.top > y)
			{
				m_ActivityZone.top=y;
			}
			if(m_ActivityZone.bottom < y)
			{
				m_ActivityZone.bottom=y;
			}

			BaseGameEntity* pV=new BaseGameEntity();
			
			pV->SetPosition(Point2D(x, y));
			pV->SetID(id);

			m_tour.push_back(pV);

			pBehavior->RegisterStation(pV);
		}
	}

	pBehavior->Start();

	


	return true;
}

/// <summary>Method taht renders all the game agents in the world</summary>
/// \param[in] buffer the image buffer onto which the game agents are rendered and displayed using double-buffer techniques
void SceneManager::Render(BITMAP* buffer)
{
	if(m_StationLinkVisible)
	{
		RenderStationLink(buffer);
	}

	if(m_pVehicle!=NULL)
	{		
		m_pVehicle->Render(buffer);
	}
	
	if(m_StationDotVisible)
	{
		RenderStationDots(buffer);
	}

	if(m_pVehicle!=NULL)
	{
		circlefill(buffer, m_pVehicle->GetPositionOnSimulator().GetX(), m_pVehicle->GetPositionOnSimulator().GetY(), 5, makecol(0, 255, 0));
		circle(buffer, m_pVehicle->GetPositionOnSimulator().GetX(), m_pVehicle->GetPositionOnSimulator().GetY(), 5, makecol(0, 0, 255));
	}

	if( m_StationIdVisible)
	{
		RenderStationId(buffer);
	}
	
}

/// <summary>Method that renders the customer dot on each customer agent</summary>
/// \param[in] buffer the image buffer onto which the station dots are rendered and displayed using double-buffer technique
void SceneManager::RenderStationDots(BITMAP* buffer)
{
	for(int i=0; i<m_tour.size(); i++)
	{
		Point2D pos=m_tour[i]->GetPositionOnSimulator();
		circlefill(buffer, pos.GetX(), pos.GetY(), 4, 0xFFFFCC); //makecol(120, 120, 120));
		circle(buffer, pos.GetX(), pos.GetY(), 4, makecol(255, 0, 0));
	}
}

/// <summary>Method that updates all the game agents in the world when the timer ticks in the game engine</summary>
/// \param[in] deltaTime the time duration between two consecutive calls to the update method
void SceneManager::Update(float deltaTime)
{
	if(key[KEY_A])
	{
		m_StationIdVisible=true;
	}
	if(key[KEY_S])
	{
		m_StationIdVisible=false;
	}

	if(key[KEY_B])
	{
		m_StationLinkVisible=true;
	}
	if(key[KEY_V])
	{
		m_StationLinkVisible=false;
	}

	if(key[KEY_F])
	{
		m_StationDotVisible=true;
	}
	if(key[KEY_G])
	{
		m_StationDotVisible=false;
	}
	
	if(m_pVehicle!=NULL)
	{
		m_pVehicle->Update(deltaTime);
	}
}

/// <summary>Method that renders the customer id on each customer agent</summary>
/// \param[in] buffer the image buffer onto which the station ids are rendered and displayed using double-buffer technique
void SceneManager::RenderStationId(BITMAP* buffer)
{
	for(int i=0; i < m_tour.size(); i++)
	{
		Point2D pos=m_tour[i]->GetPositionOnSimulator();
		pos.SetY(pos.GetY()+5);

		textout(buffer, font, m_tour[i]->GetID().c_str(), pos.GetX(), pos.GetY(), makecol(255, 255, 255));
	}
}

/// <summary>Method that renders the link between customer agent</summary>
/// \param[in] buffer the image buffer onto which the links are rendered and displayed using double-buffer technique
void SceneManager::RenderStationLink(BITMAP* buffer)
{
	for(int i=0; i <= m_tour.size(); i++)
	{
		int index1 = i % m_tour.size();
		int index2 = (i + 1) % m_tour.size();

		Point2D pos1=m_tour[index1]->GetPositionOnSimulator();
		Point2D pos2=m_tour[index2]->GetPositionOnSimulator();

		line(buffer, pos1.GetX(), pos1.GetY(), pos2.GetX(), pos2.GetY(), 0xCCCCFF);
	}
}
