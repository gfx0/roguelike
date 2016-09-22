#ifndef EngineComponentManager_h__
#define EngineComponentManager_h__

#include "EngineComponentBase.h"

//TODO: Refactor unordered_map to simple vectors with enum indices?
//		Research whether we're being overly complex here and if a vector based enum retrieval would be faster.

#include <unordered_map>
#include <cassert>

class EngineComponentManager
{
public:
	EngineComponentManager();
	~EngineComponentManager();

	void ListLoadedComponents();
	void Update(unsigned int deltaTime);
	bool InitializeAllComponents();
	void DeInitializeAllComponents();

	template<class T>
	void LoadComponent(const char * gameComponentPath)
	{
		
		//NOTE: If you asserted here, you are probably adding a component key value twice into the hash map.
		assert(mpGameComponentMap->find(gameComponentPath) == mpGameComponentMap->end());

		T * GameComponent = new T();
		mpGameComponentMap->insert(std::make_pair(gameComponentPath, GameComponent));

	}

	template<class T>
	T * GetComponent(const char * gameComponentPath)
	{
		GameComponentMap::iterator itr = mpGameComponentMap->find(gameComponentPath);
		assert(itr != mpGameComponentMap->end());
		return static_cast<T*>(itr->second);
	}

private:
	typedef std::tr1::unordered_map<const char*, EngineComponentBase*> GameComponentMap;
	GameComponentMap * mpGameComponentMap;
};

#endif // EngineComponentManager_h__
