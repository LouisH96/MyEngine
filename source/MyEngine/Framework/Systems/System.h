#pragma once

#include <App\ResizedEvent.h>

namespace MyEngine
{
class ISystem
{
public:
	virtual ~ISystem() = default;
	virtual void OnCanvasResized(const App::ResizedEvent& event) = 0;

	virtual void EarlyUpdate() = 0;
	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
};
}