#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Root final
			: public ParentElem<EmptyChildOptions>
		{
		public:
			explicit Root(const Float2& initSize);

			void AddChild(Elem* pChild);
			const std::string GetTypeName() const override;

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;

			void Clear() override;
			void Create() override;
		};
	}
}
