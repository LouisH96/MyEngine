#pragma once
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		struct AnchorChild
		{
			Elem* pChild;
			Float2 ParentAnchor;
			Float2 ChildAnchor;
		};

		class AnchorParent final
			: public ParentElem<AnchorChild>
		{
		public:
			AnchorParent();

			using ParentElem::AddChild;
			void AddChild(Elem* pElem, const Float2& anchor);
			void AddChild(Elem* pElem, const Float2& parentAnchor, const Float2& childAnchor);

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			const std::string GetTypeName() const override { return "AnchorParent"; }

		private:
			void Clear() override;
			void Create() override;
		};
	}
}
