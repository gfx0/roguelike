#include "EngineComponentManager.h"

#include <windows.h>

/******************************************************************************

	The purpose of this manager is to manage and control the:

			"Parts of the engine" of the "Car"

******************************************************************************/

EngineComponentManager::EngineComponentManager()
{
	mpGameComponentMap = new GameComponentMap();
}

EngineComponentManager::~EngineComponentManager()
{
	delete mpGameComponentMap;
}

void EngineComponentManager::ListLoadedComponents()
{
	for (GameComponentMap::iterator it = mpGameComponentMap->begin(); it != mpGameComponentMap->end(); ++it)
	{
		OutputDebugStringA("Component loaded:");
		OutputDebugStringA(it->first);
		OutputDebugStringA("\n");
	}
}

void EngineComponentManager::Update( unsigned int deltaTime )
{
	for (GameComponentMap::iterator it = mpGameComponentMap->begin(); it != mpGameComponentMap->end(); ++it)
		it->second->Update( deltaTime );
}

bool EngineComponentManager::InitializeAllComponents()
{
	//If even one component fails to initialize, it is a reason to shut down.
	for (GameComponentMap::iterator it = mpGameComponentMap->begin(); it != mpGameComponentMap->end(); ++it)
		if (!it->second->Start())
			return false;

	return true;
}

void EngineComponentManager::DeInitializeAllComponents()
{
	for (GameComponentMap::iterator it = mpGameComponentMap->begin(); it != mpGameComponentMap->end(); ++it)
	{
		OutputDebugStringA("deinitializing component:");
		OutputDebugStringA(it->first);
		OutputDebugStringA("\n");
		it->second->Stop();
	}
}
