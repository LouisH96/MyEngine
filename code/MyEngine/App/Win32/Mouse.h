#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Mouse
			{
			public:
				Mouse() = default;

				void PreChange();
				void PostChange();

				void OnMove(long long position);
				void OnScroll(float scroll);
				void OnLeftBtnPressed();
				void OnLeftBtnReleased();
				void OnMiddleBtnPressed();
				void OnMiddleBtnReleased();
				void OnRightBtnPressed();
				void OnRightBtnReleased();

				bool IsLeftBtnDown() const;
				bool IsLeftBtnPressed() const;
				bool IsLeftBtnReleased() const;

				bool IsMiddleBtnDown() const;
				bool IsMiddleBtnPressed() const;
				bool IsMiddleBtnReleased() const;

				bool IsRightBtnDown() const;
				bool IsRightBtnPressed() const;
				bool IsRightBtnReleased() const;

				void SetPos(Int2 position);
				const Int2& GetPos() const { return m_Pos; }
				const Int2& GetMovement() const { return m_Movement; }
				float GetScroll() const { return m_Scroll; } //positive is scroll up

			private:
				static constexpr int LEFT_MASK = 1;
				static constexpr int RIGHT_MASK = 2;
				static constexpr int MIDDLE_MASK = 4;
				static constexpr int NR_BUTTONS = 3;
				Int2 m_Pos{0, 0};
				Int2 m_Movement{0, 0};
				float m_Scroll{ 0 };
				int m_State{ 0 };
			};
		}
	}
}
