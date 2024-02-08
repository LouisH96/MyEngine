#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class ListElem final
			: public ParentElem<EmptyChildOptions>
		{
		public:
			struct Settings
			{
				float ChildMargin{ 5.f };
				bool UniformChildWidth{ true };
			};

			explicit ListElem(const Settings& settings);

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			const std::string GetTypeName() const override;

		private:
			float m_ChildMargin;
			bool m_UniformChildWidth;

			void Clear() override;
			void Create() override;
		};
	}
}
