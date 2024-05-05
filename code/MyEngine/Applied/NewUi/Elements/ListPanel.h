#pragma once

#include <Applied/NewUi/ParentElem.h>
#include <Applied/NewUi/EmptyChildOptions.h>

namespace MyEngine
{
	namespace NewUi
	{
		class ListPanel
			: public ParentElem<EmptyChildOptions>
		{
		public:
			enum class Direction
			{
				RightAndUp = 0,
				RightAndDown = 1,
				LeftAndUp = 2,
				LeftAndDown = 3,
				UpAndRight = 4,
				UpAndLeft = 5,
				DownAndRight = 6,
				DownAndLeft = 7
			};
			explicit ListPanel(Direction direction, float childMargin = 5, float lineMargin = 5);

			void AddChild(Elem* pChild);

		private:
			Float2 m_FlowDir;
			Float2 m_FillDir;
			float m_ChildMargin;
			float m_LineMargin;

			explicit ListPanel(const Float2& flowDir, const Float2& fillDir, float childMargin, float lineMargin);
			
			virtual const std::string GetTypeName() const override { return "ListPanel"; };
			virtual void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			virtual void Clear() override;
			virtual void Create() override;

			//returns true when there should be other lines
			//iChild should be the list's end (thus child index behind it's last child)
			//lineSize in: maxLineSize
			//lineSize out: actual lineSize
			void CreateLineOnOrigin(unsigned& iChild, float& lineFlowSize, float& lineFillSize, bool& isLastLine);
		
			static Float2 ToFlowDirection(Direction direction);
			static Float2 ToFillDirection(Direction direction);

		};
	}
}
