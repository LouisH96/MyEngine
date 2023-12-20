#pragma once
#include "Applied/SimpleUi/Elem.h"

namespace MyEngine
{
	namespace SimpleUi
	{
		class Root final : public Elem
		{
		public:
			explicit Root(const RectFloat& bounds);

		protected:
			void Clear() override;
			void Make() override;
			bool IsAlive() override { return true; }
			void OnChildAdded(Elem& newChild) override { Remake(newChild); }
			void OnBoundsChanged(const RectFloat& oldBounds) override;
		};
	}
}
