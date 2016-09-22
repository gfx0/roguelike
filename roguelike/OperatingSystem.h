#ifndef OperatingSystem_h__
#define OperatingSystem_h__
#include "EngineComponentBase.h"

int g_initOS(int argCount, char * argValues[]);

class OperatingSystem : public EngineComponentBase
{
public:
	OperatingSystem();
	~OperatingSystem();

private:

public:

	bool Start();
	void Stop();
	void Update(unsigned int deltaTime);
	char * GetBasePath();
};
#endif // OperatingSystem_h__
