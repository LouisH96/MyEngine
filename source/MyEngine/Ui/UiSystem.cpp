#include "pch.h"
#include "UiSystem.h"

#include "App/ResizedEvent.h"
#include "Elements/Button.h"
#include <Rendering\Canvas.h>

using namespace Ui;

const Float3 UiSystem::COLOR_DARK = Float3{ .1f };
const Float3 UiSystem::COLOR_MEDIUM = Float3{ .4f };
const Float3 UiSystem::COLOR_LIGHT = Float3{ .6f };

UiSystem::UiSystem(const Float2& screenSize)
	: m_FontRenderer{ screenSize }
	, m_pCurrentElem{ nullptr }
	, m_CurrentElemState() //no need
{
}

void UiSystem::OnCanvasResized(const App::ResizedEvent& event)
{
	m_ShapeRenderer.OnCanvasResized(event);
	m_FontRenderer.OnCanvasResized(event);
	m_ImageRenderer.OnCanvasResized(event);
	m_Tree.RequestUpdate();
}

void UiSystem::Update()
{
	if (m_Tree.NeedUpdate())
		RecreateTree();
	UpdateCurrentElemState();
}

void UiSystem::Render()
{
	m_RectRenderer.Render();

	m_ShapeRenderer.Render();
	m_ImageRenderer.Render();
	m_FontRenderer.Render();
}

void UiSystem::AddChild(Elem* pChild,
	const Float2& parentPivot, const Float2& childPivot)
{
	m_Tree.GetRoot().AddChild({ pChild, parentPivot, childPivot });
}

void UiSystem::RemoveChild(Elem* pChild)
{
	m_Tree.GetRoot().RemoveChild(pChild);
}

void UiSystem::DeleteChild(Elem* pChild)
{
	m_Tree.GetRoot().DeleteChild(pChild);
}

void UiSystem::ClearDebugBorder()
{
	for (unsigned i = 0; i < NR_DEBUG_BORDER_SHAPES; ++i)
		m_ShapeRenderer.Remove(m_DebugBorder[i]);
}

void UiSystem::CreateDebugBorder()
{
	CreateDebugBorder(*m_pCurrentElem);
}

void UiSystem::CreateDebugBorder(const Elem& elem)
{
	//outside border
	constexpr float thickness{ 2 };
	static const Float3 color{ .75f,0,0 };

	//left
	Float2 pos{ elem.GetBounds().GetLeftBot() };
	pos -= Float2{ thickness, thickness };
	Float2 size{ thickness, elem.GetHeight() + thickness * 2 };

	m_DebugBorder[0] = m_ShapeRenderer.Rect(pos, size, color);

	//right
	pos.x += thickness + elem.GetWidth();

	m_DebugBorder[1] = m_ShapeRenderer.Rect(pos, size, color);

	//top
	pos = elem.GetBounds().GetLeftTop();
	pos -= Float2{ thickness, 0 };
	size = Float2{ elem.GetWidth() + thickness * 2, thickness };

	m_DebugBorder[2] = m_ShapeRenderer.Rect(pos, size, color);

	//bot
	pos.y -= elem.GetHeight() + thickness;

	m_DebugBorder[3] = m_ShapeRenderer.Rect(pos, size, color);
}

void UiSystem::UpdateCurrentElemState()
{
	Elem* pUnderMouse{ m_Tree.GetElemUnderMouse() };

	if (!(m_pCurrentElem || pUnderMouse))
		return;

	if (!m_pCurrentElem)
	{
		m_pCurrentElem = pUnderMouse;
		if (m_ShowDebugBorder)
			CreateDebugBorder();

		if (MOUSE.IsLeftBtnDown())
		{
			m_pCurrentElem->ToPressedState();
			m_CurrentElemState = Pressed;
			OnCurrentElemMouseDown();
		}
		else
		{
			m_pCurrentElem->ToHoverState();
			m_CurrentElemState = Hovered;
		}
	}
	else
	{
		if (m_ShowDebugBorder)
			ClearDebugBorder();

		const bool isUnderMouse{ m_pCurrentElem == pUnderMouse };
		if (m_CurrentElemState == Hovered)
		{
			if (isUnderMouse)
			{
				if (MOUSE.IsLeftBtnDown())
				{
					m_pCurrentElem->ToPressedState();
					m_CurrentElemState = Pressed;
					OnCurrentElemMouseDown();
				}

				if (m_ShowDebugBorder)
					CreateDebugBorder();
			}
			else
			{
				m_pCurrentElem->ToDefaultState();
				m_pCurrentElem = nullptr;
			}
		}
		else
		{
			if (!MOUSE.IsLeftBtnDown())
			{
				if (isUnderMouse)
				{
					m_pCurrentElem->ToHoverState();
					m_CurrentElemState = Hovered;

					if (m_ShowDebugBorder)
						CreateDebugBorder();

					m_pCurrentElem->OnClick();
				}
				else
				{
					m_pCurrentElem->ToDefaultState();
					m_pCurrentElem = nullptr;
				}
			}
			else
			{
				OnCurrentElemMouseDown();
				if (m_ShowDebugBorder)
					CreateDebugBorder();
			}
		}
	}
}

void UiSystem::OnCurrentElemMouseDown()
{
	const Float2 mousePos{ MOUSE.GetPosLeftBot() };
	const Float2& elemPos{ m_pCurrentElem->GetPosition() };
	m_pCurrentElem->OnMouseDown(mousePos - elemPos);
}

void UiSystem::RecreateTree()
{
	if (m_ShowDebugBorder && m_pCurrentElem)
		ClearDebugBorder();

	m_pCurrentElem = nullptr;

	m_Tree.RecreateTree();
}
