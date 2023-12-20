#pragma once
#include "Geometry/Shapes/Rects.h"

namespace MyEngine
{
	namespace SimpleUi
	{
		class Elem
		{
		public:
			explicit Elem(const RectFloat& bounds);
			virtual ~Elem();

			Elem(const Elem& other) = delete;
			Elem(Elem&& other) noexcept = delete;
			Elem& operator=(const Elem& other) = delete;
			Elem& operator=(Elem&& other) noexcept = delete;

			const RectFloat& GetBounds() const { return m_Bounds; }
			const Float2& GetLeftBot() const { return m_Bounds.GetLeftBot(); }
			const Float2& GetSize() const { return m_Bounds.GetSize(); }

			void SetBounds(const RectFloat& newBounds);
			void SetLeftBot(const Float2& newPos);
			void SetSize(const Float2& newSize);
			void Move(const Float2& displacement);

			const Elem* GetParent() const { return m_pParent; }
			const Elem* GetNextSibling() const { return m_pSibling; }
			const Elem* GetFirstChild() const { return m_pChild; }

			Elem* GetFirstChild() { return m_pChild; }
			Elem* GetNextSibling() { return m_pSibling; }

			void AddChild(Elem* pChild);

		protected:
			RectFloat m_Bounds;

			void Remake();
			virtual void OnBoundsChanged(const RectFloat& oldBounds) = 0;
			virtual void Clear() = 0;
			virtual void Make() = 0;
			virtual bool IsAlive() = 0;
			virtual void OnChildAdded(Elem& newChild) = 0;

			static void Remake(Elem& other) { other.Remake(); }

		private:
			Elem* m_pParent{};
			Elem* m_pSibling{};
			Elem* m_pChild{};

			void LeaveFamily();
			Elem* GetChildBefore(const Elem& child);
			Elem* GetLastChild();
		};
	}
}
