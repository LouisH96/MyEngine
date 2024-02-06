#include "pch.h"
#include "NewUiSystem.h"

#include "App/ResizedEvent.h"
#include "Elements/Button.h"

using namespace NewUi;

const Float3 NewUiSystem::COLOR_DARK = Float3{ .1f };
const Float3 NewUiSystem::COLOR_MEDIUM = Float3{ .4f };
const Float3 NewUiSystem::COLOR_LIGHT = Float3{ .6f };

NewUiSystem::NewUiSystem(const Float2& screenSize)
	: m_Root{ screenSize }
	, m_FontRenderer{ screenSize }
	, m_pCurrentElem{ nullptr }
	, m_CurrentElemState() //no need
{
}

void NewUiSystem::OnCanvasResized(const App::ResizedEvent& event)
{
	m_FontRenderer.OnCanvasResized(event);
	m_Root.UpdateRootSize(event.NewSize);
}

void NewUiSystem::Update()
{
	const Float2 mouse{ static_cast<float>(MOUSE.GetPos().x), m_Root.GetHeight() - MOUSE.GetPos().y };
	Elem* pUnderMouse{ m_Root.GetElemAt(mouse) };

	if (!(m_pCurrentElem || pUnderMouse))
		return;

	if (!m_pCurrentElem)
	{
		m_pCurrentElem = pUnderMouse;
		if (MOUSE.IsLeftBtnDown())
		{
			m_pCurrentElem->ToPressedState();
			m_CurrentElemState = Pressed;
		}
		else
		{
			m_pCurrentElem->ToHoverState();
			m_CurrentElemState = Hovered;
		}
	}
	else
	{
		const bool isUnderMouse{ m_pCurrentElem == pUnderMouse };
		if (m_CurrentElemState == Hovered)
		{
			if (isUnderMouse)
			{
				if (MOUSE.IsLeftBtnDown())
				{
					m_pCurrentElem->ToPressedState();
					m_CurrentElemState = Pressed;
				}
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
					m_pCurrentElem->OnClick();
					m_CurrentElemState = Hovered;
				}
				else
				{
					m_pCurrentElem->ToDefaultState();
					m_pCurrentElem = nullptr;
				}
			}
		}
	}
}

void NewUiSystem::Render()
{
	m_FontRenderer.Render();
}

void NewUiSystem::AddChild(Elem* pChild)
{
	m_Root.AddChild(pChild);
}

void NewUiSystem::RemoveAllChildren()
{
	BeforeEdit();
	m_Root.RemoveChildren();
}

void NewUiSystem::BeforeEdit()
{
	m_pCurrentElem = nullptr;
	m_Root.ClearTree();
}

void NewUiSystem::AfterEdit()
{
	m_Root.CreateTree();
}
