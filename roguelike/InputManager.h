#ifndef InputManager_h__
#define InputManager_h__
#include "EngineComponentBase.h"

class InputManager : public EngineComponentBase
{
public:
	InputManager();
	~InputManager();

private:

public:
	bool Start();
	void Stop();
	void Update();
};
#endif // InputManager_h__
