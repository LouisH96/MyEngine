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

			void UpdateRootSize(const Float2& size);
			void AddChild(Elem* pChild);
			void CreateUi();
			const std::string GetTypeName() const override;

		private:
			void Clear() override;
			void Create() override;
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			
		};
	}
}
