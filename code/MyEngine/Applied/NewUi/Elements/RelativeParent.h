#pragma once
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Elem;

		struct RelativeChild
		{
			Elem* pChild{};

			float MaxWidthRatio{ .5f };
			float MaxHeightRatio{ .5f };
		};

		class RelativeParent final
				: public ParentElem<RelativeChild>
		{
		public:
			RelativeParent();

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			const std::string GetTypeName() const override;

		private:
			void Clear() override;
			void Create() override;

		};
	}
}
