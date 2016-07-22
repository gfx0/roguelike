#ifndef EngineComponentBase_h__
#define EngineComponentBase_h__

class EngineComponentBase
{
public:
	EngineComponentBase();
	~EngineComponentBase();

	virtual bool Start() = 0;
	virtual void Stop() = 0;
	virtual void Update( unsigned int deltaTime ) = 0;

protected:
private:
};

#endif // EngineComponentBase_h__
