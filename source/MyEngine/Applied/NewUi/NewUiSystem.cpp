#include "pch.h"
#include "NewUiSystem.h"

#include "App/ResizedEvent.h"
#include "Elements/Button.h"
#include <Rendering\Canvas.h>

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
	m_ShapeRenderer.OnCanvasResized(event);
	m_FontRenderer.OnCanvasResized(event);
	m_ImageRenderer.OnCanvasResized(event);

	Elem* pRoot{ &m_Root };
	pRoot->UpdateSizeAndTreePositions(GetRootResizePref());
	pRoot->UpdateTreePositions({});
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
		if (m_ShowDebugBorder)
			CreateDebugBorder();

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
			else if (m_ShowDebugBorder)
				CreateDebugBorder();
		}
	}
}

void NewUiSystem::Render()
{
	m_RectRenderer.Render();

	m_ShapeRenderer.Render();
	m_ImageRenderer.Render();
	m_FontRenderer.Render();
}

void NewUiSystem::AddChild(Elem* pChild)
{
	m_Root.AddChild(pChild);
}

void NewUiSystem::RemoveChild(Elem* pChild)
{
	m_Root.RemoveChild(pChild);
}

void NewUiSystem::DeleteChild(Elem* pChild)
{
	m_Root.DeleteChild(pChild);
}

void NewUiSystem::BeforeEdit()
{
	if (m_ShowDebugBorder && m_pCurrentElem)
		ClearDebugBorder();

	m_pCurrentElem = nullptr;
	m_ImageRenderer.Clear();

	Elem* pRoot{ &m_Root };
	pRoot->ClearTree();
}

void NewUiSystem::AfterEdit()
{
	Elem* pRoot{ &m_Root };

	pRoot->UpdateSizeAndTreePositions(GetRootResizePref());
	pRoot->UpdateTreePositions({});
	pRoot->CreateTree();

	m_ImageRenderer.CreateBuffer();
}

void NewUiSystem::ClearDebugBorder()
{
	for (unsigned i = 0; i < NR_DEBUG_BORDER_SHAPES; ++i)
		m_ShapeRenderer.Remove(m_DebugBorder[i]);
}

void NewUiSystem::CreateDebugBorder()
{
	CreateDebugBorder(*m_pCurrentElem);
}

void NewUiSystem::CreateDebugBorder(const Elem& elem)
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

ResizePref NewUiSystem::GetRootResizePref() const
{
	ResizePref pref{};
	pref.minSize = { 0,0 };
	pref.maxSize = CANVAS.GetSize();
	pref.SetMax();
	return pref;
}
