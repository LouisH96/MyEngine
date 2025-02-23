#pragma once
#include <Applied\NewUi\EmptyChildOptions.h>
#include <Applied\NewUi\ParentElem.h>
#include <Applied\NewUi\GraphicsId.h>

namespace MyEngine
{
namespace NewUi
{
class SideMenuTab;

class SideMenuPath final : public ParentElem<EmptyChildOptions>
{
public:
	SideMenuPath();
	const std::string GetTypeName() const override { return "SideMenuPath"; }

	SideMenuTab* GetClickedTab() const;

	void SetTab(SideMenuTab& tab);

private:
	struct ButtonInfo
	{
		std::string Text;
		RectId BackgroundId{};
		FontId TextId{};
		FontId ArrowId{};
		SideMenuTab* pTab{};

		Float2 Size;
		Float2 Pos;
		bool DisplayPoints{ false };

		void ClearGraphics();
	};

	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;
	void AddButton(SideMenuTab& tab);

	using ParentElem::AddChild;

	static constexpr float FONT_SIZE{ 10 };
	static const Float2 BUTTON_MARGIN;
	static constexpr float ARROW_MARGIN{ 5 };
	static constexpr float ROW_MARGIN{ 4 };

	List<ButtonInfo> m_ButtonInfo{};
	Float2 m_ArrowSize;
	Float2 m_PointsSize;

	void AddSelfAfterParent(SideMenuTab& tab);

	friend class SideMenuPathHelper;
};
}
}
