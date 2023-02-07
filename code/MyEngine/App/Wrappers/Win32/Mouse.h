#pragma once

#include <Math/Int2.h>

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class Mouse
				{
				public:
					Mouse();

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

					const Math::Int2& GetPos() const { return m_Pos; }
					const Math::Int2& GetMovement() const { return m_Movement; }
					float GetScroll() const { return m_Scroll; }

				private:
					static constexpr int LEFT_MASK = 1;
					static constexpr int RIGHT_MASK = 2;
					static constexpr int MIDDLE_MASK = 4;
					static constexpr int NR_BUTTONS = 3;
					Math::Int2 m_Pos;
					Math::Int2 m_Movement;
					float m_Scroll;
					int m_State;
				};
			}
		}
	}
}
