#include "pch.h"
#include "Extender.h"

using namespace Ui;

Extender::Extender(const SizeDef& sizeDef)
	: m_SizeDef{ sizeDef }
{
}

void Extender::TreeUpdate(const ResizePref& pref)
{
	const SizeDef::Mode& horMode{ m_SizeDef.HorizontalMode };
	const SizeDef::Mode& verMode{ m_SizeDef.VerticalMode };

	ResizePref childPref{};
	childPref.minSize = {};

	if (horMode == SizeDef::Mode::Pixels)
	{
		childPref.maxSize.x = m_SizeDef.Value.x;
		childPref.horMode = FillMode::Max;
	}
	else if (horMode == SizeDef::Mode::Percentage)
	{
		childPref.maxSize.x = pref.maxSize.x * m_SizeDef.Value.x;
		childPref.horMode = FillMode::Max;
	}
	else
	{
		childPref.maxSize.x = pref.maxSize.x;
		childPref.horMode = pref.horMode;
	}

	if (verMode == SizeDef::Mode::Pixels)
	{
		childPref.maxSize.y = m_SizeDef.Value.y;
		childPref.verMode = FillMode::Max;
	}
	else if (verMode == SizeDef::Mode::Percentage)
	{
		childPref.maxSize.y = pref.maxSize.y * m_SizeDef.Value.y;
		childPref.verMode = FillMode::Max;
	}
	else
	{
		childPref.maxSize.y = pref.maxSize.y;
		childPref.verMode = pref.verMode;
	}

	Float2 childBounds{};
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		ChildTreeUpdate(i, childPref);

		const Float2& childSize{ GetChild(i).GetSize() };
		childBounds.x = Float::Max(childBounds.x, childSize.x);
		childBounds.y = Float::Max(childBounds.y, childSize.y);

		ChildSetPosition(i, {});
	}

	//set my sizes
	Float2 mySize;
	if (horMode == SizeDef::Mode::Pixels
		|| horMode == SizeDef::Mode::Percentage)
		mySize.x = childPref.maxSize.x;
	else
		mySize.x = childBounds.x;
	if (verMode == SizeDef::Mode::Pixels
		|| verMode == SizeDef::Mode::Percentage)
		mySize.y = childPref.maxSize.y;

	if (horMode == SizeDef::Mode::PercentageOfOther)
		mySize.x = mySize.y * m_SizeDef.Value.y;
	if (verMode == SizeDef::Mode::PercentageOfOther)
		mySize.y = mySize.x * m_SizeDef.Value.x;
	SetSize(mySize);

	//reset child sizes
	childPref.maxSize = GetSize();
	childPref.SetMax();
	for (unsigned i{ 0 }; i < GetNrChildren(); ++i)
		ChildTreeUpdate(i, childPref);
}

void Extender::SetSizeDef(const Float2& value)
{
	m_SizeDef.Value = value;
	RequestUpdate();
}

void Extender::SetSizeDefX(float valueX)
{
	m_SizeDef.Value.x = valueX;
	RequestUpdate();
}

void Extender::SetSizeDefY(float valueY)
{
	m_SizeDef.Value.y = valueY;
	RequestUpdate();
}

void Extender::Clear()
{
}

void Extender::Create()
{
}
