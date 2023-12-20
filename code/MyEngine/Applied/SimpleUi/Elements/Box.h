#pragma once
#include "../Elem.h"

namespace MyEngine
{
	namespace SimpleUi
	{
		class Box final : public Elem
		{
		public:
			explicit Box(const RectFloat& bounds, const Float3& color);

		protected:
			void Clear() override;
			void Make() override;
			bool IsAlive() override { return m_BoxId >= 0; }
			void OnChildAdded(Elem& newChild) override;
			void OnBoundsChanged(const RectFloat& oldBounds) override;

		private:
			int m_BoxId{ -1 };
			Float3 m_Color;
		};
	}
}
