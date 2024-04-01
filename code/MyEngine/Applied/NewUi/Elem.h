#pragma once
#include "Geometry/Shapes/Rects.h"

namespace MyEngine
{
	namespace NewUi
	{
		class NewUiShapeRenderer;

		enum FillMode
		{
			Min, Max, NoPref
		};

		struct ResizePref
		{
			Float2 minSize;
			Float2 maxSize;
			FillMode horMode;
			FillMode verMode;

			Float2 GetPreferredSize() const;
		};

		class Elem
		{
		public:
			Elem() = default;
			Elem(const Elem& other) = delete;
			Elem(Elem&& other) noexcept = delete;
			Elem& operator=(const Elem& other) = delete;
			Elem& operator=(Elem&& other) noexcept = delete;
			virtual ~Elem() = default;

			const Float2& GetPosition() const { return m_Bounds.GetLeftBot(); }
			const Float2& GetSize() const { return m_Bounds.GetSize(); }
			const Float2 GetCenter() const;
			float GetWidth() const { return m_Bounds.GetWidth(); }
			float GetHeight() const { return m_Bounds.GetHeight(); }
			const RectFloat& GetBounds() const { return m_Bounds; }

			virtual Elem* GetElemAt(const Float2& position);

			virtual void ToDefaultState() {}
			virtual void ToHoverState() {}
			virtual void ToPressedState() {}
			virtual void OnClick() {}

			virtual const std::string GetTypeName() const = 0;

		protected:
			void SetSize(const Float2& size);
			void SetSize(const ResizePref& pref);
			void SetWidth(float width);
			void SetHeight(float height);

			static void AssertWithinMaxSize(const Float2& desired, const ResizePref& pref);

			static NewUiShapeRenderer& Draw();

		private:
			//---| Resizing |---
			/* Functions that will be called for the resizing process:
			 * 1) UpdateSizeAndTreePositions(..) -> In that function the element should:
			 *		- Update it's own position
			 *		- Call UpdateSizeAndTreePositions on all it's children
			 *		- Call SetRelativePosition on all it's children
			 * 2) UpdateTreePositions(..) -> This will move each element and it's children to the global position
			 * 3) ClearTree() -> Destroys visuals
			 * 4) CreateTree() -> Creates visuals
			 */
			void SetRelativePosition(const Float2& relativePosition); //relative to parent
			virtual void UpdateSizeAndTreePositions(const ResizePref& pref) = 0; //update this size, its children size and their relative positions
			virtual void UpdateTreePositions(const Float2& position);

			//---| Visuals |---
			void RecreateTree();
			virtual void ClearTree();
			virtual void CreateTree();
			virtual void Clear() = 0;
			virtual void Create() = 0;

			RectFloat m_Bounds;

			template<typename T>
			friend class ParentElem;
			friend class Root;
		};
	}
}
