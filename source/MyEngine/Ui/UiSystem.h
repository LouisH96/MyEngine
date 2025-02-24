#pragma once
#include "Elements\AnchorParent.h"
#include "Renderers\UiFontRenderer.h"
#include "Renderers\UiImageRenderer.h"
#include "Renderers\UiRectRenderer.h"
#include "Renderers\UiShapeRenderer.h"
#include "TreeManager.h"

namespace MyEngine
{
namespace Ui
{
class Elem;

class UiSystem
{
public:
	enum CurrentElemState
	{
		Hovered, Pressed
	};

	explicit UiSystem(const Float2& screenSize);

	void OnCanvasResized(const App::ResizedEvent& event);
	void Update();
	void Render();

	void AddChild(Elem* pChild,
		const Float2& parentPivot = {},
		const Float2& childPivot = {});
	void RemoveChild(Elem* pChild);
	void DeleteChild(Elem* pChild);

	UiRectRenderer& GetRectRenderer() { return m_RectRenderer; }
	UiFontRenderer& GetFontRenderer() { return m_FontRenderer; }
	UiShapeRenderer& GetShapeRenderer() { return m_ShapeRenderer; }
	UiImageRenderer& GetImageRenderer() { return m_ImageRenderer; }
	TreeManager& GetTreeManager() { return m_Tree; }

	static const Float3 COLOR_DARK;
	static const Float3 COLOR_MEDIUM;
	static const Float3 COLOR_LIGHT;
	static constexpr float BORDER_THICKNESS{ 4.f };

	Elem* GetCurrentElem() { return m_pCurrentElem; }
	CurrentElemState GetCurrentElemState() const { return m_CurrentElemState; }

private:
	TreeManager m_Tree;
	UiRectRenderer m_RectRenderer;
	UiFontRenderer m_FontRenderer;
	UiShapeRenderer m_ShapeRenderer;
	UiImageRenderer m_ImageRenderer;

	Elem* m_pCurrentElem;
	CurrentElemState m_CurrentElemState;

	static constexpr unsigned NR_DEBUG_BORDER_SHAPES{ 4 };
	unsigned m_DebugBorder[NR_DEBUG_BORDER_SHAPES]{};
	bool m_ShowDebugBorder{ true };

	void ClearDebugBorder();
	void CreateDebugBorder();
	void CreateDebugBorder(const Elem& elem);

	void UpdateCurrentElemState();

	void RecreateTree();
};
}
}
