#pragma once
#include "Elem.h"

namespace MyEngine
{
	namespace NewUi
	{
		template<typename ChildData>
		class ParentElem : public Elem
		{
		public:
			ParentElem() = default;
			~ParentElem() override;
			ParentElem(const ParentElem& other) = delete;
			ParentElem(ParentElem&& other) noexcept = delete;
			ParentElem& operator=(const ParentElem& other) = delete;
			ParentElem& operator=(ParentElem&& other) noexcept = delete;

			void AddChild(const ChildData& child);
			void RemoveChild(const Elem* pChild);
			void RemoveAndDeleteChild(unsigned idx);

			Elem* GetElemAt(const Float2& position) override;

		protected:
			void SetChildPosition(unsigned childIdx, const Float2& relativePosition);
			void UpdateChildSize(unsigned childIdx, const ResizePref& pref);

			ChildData& GetChildData(unsigned i) { return m_Children[i]; }
			Elem& GetChild(unsigned i) { return *m_Children[i].pChild; }
			unsigned GetNrChildren() const { return m_Children.GetSize(); }
			float ChildWidth(unsigned i) const;
			float ChildHeight(unsigned i) const;

		private:
			//---| Visuals |---
			void ClearTree() final;
			void CreateTree() final;

			//---| Resizing |---
			void UpdateTreePositions(const Float2& movement) final;

			List<ChildData> m_Children;
			friend class Root;
		};

		template <typename ChildOptions>
		ParentElem<ChildOptions>::~ParentElem()
		{
			for (unsigned i = 0; i < m_Children.GetSize(); i++)
			{
				delete m_Children[i].pChild;
				m_Children[i].pChild = nullptr;
			}
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::AddChild(const ChildOptions& child)
		{
			m_Children.Add(child);
		}

		template <typename ChildData>
		void ParentElem<ChildData>::RemoveChild(const Elem* pChild)
		{
			for (unsigned i = 0; i < m_Children.GetSize(); i++)
				if (m_Children[i].pChild == pChild)
				{
					m_Children.Remove(i);
					return;
				}
			Logger::PrintWarning("[ParentElem::RemoveChild] couldn't find child");
		}

		template <typename ChildData>
		void ParentElem<ChildData>::RemoveAndDeleteChild(unsigned idx)
		{
			delete m_Children[idx].pChild;
			m_Children.Remove(idx);
		}

		template <typename ChildData>
		Elem* ParentElem<ChildData>::GetElemAt(const Float2& position)
		{
			for (unsigned i = m_Children.GetSize() - 1; i + 1 != 0; --i)
			{
				Elem* pUnder{ m_Children[i].pChild->GetElemAt(position) };
				if (pUnder)
					return pUnder;
			}
			return Elem::GetElemAt(position);
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::SetChildPosition(unsigned childIdx, const Float2& relativePosition)
		{
			m_Children[childIdx].pChild->SetRelativePosition(relativePosition);
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::UpdateChildSize(unsigned childIdx, const ResizePref& pref)
		{
			m_Children[childIdx].pChild->UpdateSizeAndTreePositions(pref);
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::UpdateTreePositions(const Float2& movement)
		{
			m_Bounds.Move(movement);

			for (unsigned i = 0; i < m_Children.GetSize(); i++)
				m_Children[i].pChild->UpdateTreePositions(m_Bounds.GetLeftBot());
		}

		template <typename ChildData>
		float ParentElem<ChildData>::ChildWidth(unsigned i) const
		{
			return m_Children[i].pChild->GetWidth();
		}

		template <typename ChildData>
		float ParentElem<ChildData>::ChildHeight(unsigned i) const
		{
			return m_Children[i].pChild->GetHeight();
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::ClearTree()
		{
			Clear();
			for (unsigned i = 0; i < m_Children.GetSize(); i++)
				m_Children[i].pChild->ClearTree();
		}

		template <typename ChildOptions>
		void ParentElem<ChildOptions>::CreateTree()
		{
			Create();
			for (unsigned i = 0; i < m_Children.GetSize(); i++)
				m_Children[i].pChild->CreateTree();
		}
	}
}
