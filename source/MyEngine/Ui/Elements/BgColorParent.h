#pragma once

#include "SingleParent.h"
#include <Ui\GraphicsId.h>

/*
	This is a class that can contain 1 'content' child &
	will have a colored background behind it.

	The background will have the same size as the child.
*/

namespace MyEngine
{
namespace Ui
{
class BgColorParent
	: public SingleParent
{
public:
	BgColorParent(const Float3& color, bool takeMouse = false);

	const std::string GetTypeName() const override { return "SingleParent"; }
	void TreeUpdate(const ResizePref& pref) override;

protected:
	void Clear() override;
	void Create() override;

private:
	Float3 m_Color;
	RectId m_BgId;
};
}
}