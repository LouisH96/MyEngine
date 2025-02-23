#pragma once
#include "Elements\AnchorParent.h"
#include "NewUiFontRenderer.h"
#include "Renderers\NewUiImageRenderer.h"
#include "Renderers\NewUiRectRenderer.h"
#include "Renderers\NewUiShapeRenderer.h"
#include "TreeManager.h"

namespace MyEngine
{
namespace NewUi
{
class Elem;

class NewUiSystem
{
public:
	explicit NewUiSystem(const Float2& screenSize);

	void OnCanvasResized(const App::ResizedEvent& event);
	void Update();
	void Render();

	void AddChild(Elem* pChild,
		const Float2& parentPivot = {},
		const Float2& childPivot = {});
	void RemoveChild(Elem* pChild);
	void DeleteChild(Elem* pChild);

	NewUiRectRenderer& GetRectRenderer() { return m_RectRenderer; }
	NewUiFontRenderer& GetFontRenderer() { return m_FontRenderer; }
	NewUiShapeRenderer& GetShapeRenderer() { return m_ShapeRenderer; }
	NewUiImageRenderer& GetImageRenderer() { return m_ImageRenderer; }
	TreeManager& GetTreeManager() { return m_Tree; }

	static const Float3 COLOR_DARK;
	static const Float3 COLOR_MEDIUM;
	static const Float3 COLOR_LIGHT;
	static constexpr float BORDER_THICKNESS{ 4.f };

private:
	enum CurrentElemState
	{
		Hovered, Pressed
	};

	TreeManager m_Tree;
	NewUiRectRenderer m_RectRenderer;
	NewUiFontRenderer m_FontRenderer;
	NewUiShapeRenderer m_ShapeRenderer;
	NewUiImageRenderer m_ImageRenderer;

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
