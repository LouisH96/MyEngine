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
			const std::string GetTypeName() const override;
			void RemoveChildren(); //try to remove this function

			void Clear() override;
			void Create() override;

			using ParentElem::ClearTree;
			using ParentElem::CreateTree;

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			
		};
	}
}
