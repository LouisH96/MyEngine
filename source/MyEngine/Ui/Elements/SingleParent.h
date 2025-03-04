#pragma once

#include "..\Elem.h"

/*
	This class is an Element that has 0 or 1 child.
*/
namespace MyEngine
{
namespace Ui
{
class SingleParent
	: public Elem
{
public:
	SingleParent(bool takeMouse = false);
	~SingleParent();

	Elem* GetElemAt(const Float2& position) override;
	const std::string GetTypeName() const override { return "SingleParent"; }
	void SetChildrenTakeMouse(bool takeMouse) override;

	Elem* SetChild(Elem* pChild);
	Elem* GetChild() { return m_pChild; }

protected:
	void UpdateTreePositions(const Float2& position);

private:
	Elem* m_pChild{};

	void ClearTree() override;
	void CreateTree() override;
};
}
}