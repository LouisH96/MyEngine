#pragma once
#include "System.h"
#include <DataStructures\List.h>

namespace MyEngine
{
class Systems
{
public:
	Systems() = default;
	~Systems();

	void Add(ISystem* pSystem);
	
	/*
	todo
	void RemoveSystem(ISystem* pSystem);
	void DeleteSystem(ISystem* pSystem);*/

	void OnCanvasResized(const App::ResizedEvent& event);
	void EarlyUpdate();
	void Update();
	void PreRender();
	void Render();

private:
	List<ISystem*> m_Systems{};
};
}