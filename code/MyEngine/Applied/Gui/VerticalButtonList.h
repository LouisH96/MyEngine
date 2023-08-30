#pragma once
#include "DataStructures/InvalidateList.h"

namespace MyEngine
{
	namespace Applied
	{
		class VerticalButtonList
		{
		public:
			struct Settings
			{
				Float3 borderColor = { 0,0,0 };
				Float3 innerColor = { 1,1,1 };
				Float2 pivot = { 0,0 };
				float textHeight = 10;
				float outerMargin = 5;
				float borderSize = 2;
				Float2 buttonMargin = { 20,10 };
			};

			explicit VerticalButtonList(const Settings& settings = {});

			void Update();

			int AddButton(const std::string& text);
			void RemoveButton(int buttonId);

			bool IsVisible() const { return m_BorderId != -1; }
			bool IsHidden() const { return m_BorderId == -1; }
			void Show();
			void Hide();
			void SetVisible(bool isVisible);

			bool IsHovered(int buttonId) const;
			bool IsClicked(int buttonId) const;

		private:
			struct Button
			{
				std::string text{};
				int borderId{ -1 };
				int innerId;
				int textId;

				bool IsValid() const { return text != ""; }
				void Invalidate() { text = ""; }
			};
			Settings m_Settings;
			InvalidateList<Button> m_Buttons;
			int m_BorderId{ -1 };
			int m_InnerId;

			int m_HoveredButton{ -1 }; //button-id

			void SetHoveredColors(const Button& button) const;
			void SetDefaultColors(const Button& button) const;

			bool HasHoveredButton() const;
			void SetHoveredButton(int buttonId);
			void UnsetHoveredButton();
			Button& GetHoveredButton();
			const Button& GetHoveredButton() const;
		};
	}
}
