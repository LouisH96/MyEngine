#pragma once

#include "Elements\AnchorParent.h"

namespace MyEngine
{
namespace Ui
{
class TreeManager
{
public:
	TreeManager();

	void RecreateTree();

	AnchorParent& GetRoot() { return m_Root; }
	Elem* GetElemUnderMouse();

	void RequestUpdate() { m_NeedUpdate = true; }
	bool NeedUpdate() const { return m_NeedUpdate; }

private:
	AnchorParent m_Root;
	bool m_NeedUpdate{ false };

	void ClearGraphics();
	void CreateGraphics();

	Elem& RootElem();
	const Elem& RootElem() const;
	ResizePref GetRootResizePref() const;
};
}
}