#ifndef OperatingSystem_h__
#define OperatingSystem_h__
#include "EngineComponentBase.h"

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
