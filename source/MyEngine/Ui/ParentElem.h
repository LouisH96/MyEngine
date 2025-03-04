#pragma once
#include "Elem.h"

namespace MyEngine
{
namespace Ui
{
template<typename ChildData>
class ParentElem : public Elem
{
public:
	ParentElem(bool takeMouse = false);
	~ParentElem() override;
	ParentElem(const ParentElem& other) = delete;
	ParentElem(ParentElem&& other) noexcept = delete;
	ParentElem& operator=(const ParentElem& other) = delete;
	ParentElem& operator=(ParentElem&& other) noexcept = delete;

	void AddChild(const ChildData& child);

	void RemoveChild(Elem* pChild);
	void DeleteChild(Elem* pChild);
	void DeleteChild(unsigned idx);

	void DeleteAllChildren();

	template<typename T>
	T& GetChild(unsigned idx);

	Elem* GetElemAt(const Float2& position) override;

	void SetChildrenTakeMouse(bool takeMouse) override;

protected:
	unsigned m_NrVisibleChilds{ 0 };

	void ChildSetPosition(unsigned iChild, const Float2& position);
	void ChildUpdateSizeAndTreePositions(unsigned iChild, const ResizePref& pref);

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

template<typename ChildData>
inline ParentElem<ChildData>::ParentElem(bool takeMouse)
	: Elem{ takeMouse }
{
}

template <typename ChildData>
ParentElem<ChildData>::~ParentElem()
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
	{
		delete m_Children[i].pChild;
		m_Children[i].pChild = nullptr;
	}
	RequestUpdate();
}

template <typename ChildData>
void ParentElem<ChildData>::AddChild(const ChildData& child)
{
	if (m_NrVisibleChilds == m_Children.GetSize())
		m_NrVisibleChilds++;

	m_Children.Add(child);

	RequestUpdate();
}

template <typename ChildData>
void ParentElem<ChildData>::RemoveChild(Elem* pChild)
{
	if (m_NrVisibleChilds == m_Children.GetSize())
		m_NrVisibleChilds--;

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].pChild == pChild)
		{
			m_Children.Remove(i);
			RequestUpdate();
			return;
		}
	Logger::Warning("[ParentElem::RemoveChild] couldn't find child");
	if (m_NrVisibleChilds + 1 == m_Children.GetSize())
		m_NrVisibleChilds++;
}

template <typename ChildData>
void ParentElem<ChildData>::DeleteChild(Elem* pChild)
{
	if (m_NrVisibleChilds == m_Children.GetSize())
		m_NrVisibleChilds--;

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].pChild == pChild)
		{
			m_Children[i].pChild->ClearTree();
			delete m_Children[i].pChild;
			m_Children.Remove(i);
			RequestUpdate();
			return;
		}
	Logger::Warning("[ParentElem::DeleteChild] couldn't find child");
	if (m_NrVisibleChilds + 1 == m_Children.GetSize())
		m_NrVisibleChilds++;
}

template <typename ChildData>
void ParentElem<ChildData>::DeleteChild(unsigned idx)
{
	if (m_NrVisibleChilds == m_Children.GetSize())
		m_NrVisibleChilds--;

	m_Children[idx].pChild->ClearTree();
	delete m_Children[idx].pChild;
	m_Children.Remove(idx);

	if (m_NrVisibleChilds + 1 == m_Children.GetSize())
		m_NrVisibleChilds++;

	RequestUpdate();
}

template <typename ChildData>
void ParentElem<ChildData>::DeleteAllChildren()
{
	for (unsigned i = 0; i < m_Children.GetSize(); ++i)
	{
		m_Children[i].pChild->ClearTree();
		delete m_Children[i].pChild;
	}
	m_Children.Clear();

	m_NrVisibleChilds = 0;

	RequestUpdate();
}

template <typename ChildData>
template <typename T>
T& ParentElem<ChildData>::GetChild(unsigned idx)
{
	return reinterpret_cast<T&>(m_Children[idx].pChild);
}

template <typename ChildData>
Elem* ParentElem<ChildData>::GetElemAt(const Float2& position)
{
	for (unsigned i = m_NrVisibleChilds - 1; i + 1 != 0; --i)
	{
		Elem* pUnder{ m_Children[i].pChild->GetElemAt(position) };
		if (pUnder)
			return pUnder;
	}
	return Elem::GetElemAt(position);
}

template<typename ChildData>
inline void ParentElem<ChildData>::SetChildrenTakeMouse(bool takeMouse)
{
	for (unsigned iChild{ 0 }; iChild < m_Children.GetSize(); ++iChild)
	{
		Elem& child{ *m_Children[iChild].pChild };
		child.SetTakeMouse(takeMouse);
		child.SetChildrenTakeMouse(takeMouse);
	}
}

template<typename ChildData>
inline void ParentElem<ChildData>::ChildSetPosition(unsigned iChild, const Float2& position)
{
	m_Children[iChild].pChild->SetPosition(position);
}

template<typename ChildData>
inline void ParentElem<ChildData>::ChildUpdateSizeAndTreePositions(unsigned iChild, const ResizePref& pref)
{
	m_Children[iChild].pChild->UpdateSizeAndTreePositions(pref);
}

template <typename ChildData>
void ParentElem<ChildData>::UpdateTreePositions(const Float2& movement)
{
	m_Bounds.Move(movement);

	for (unsigned i = 0; i < m_NrVisibleChilds; i++)
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

template <typename ChildData>
void ParentElem<ChildData>::ClearTree()
{
	Clear();
	for (unsigned i = 0; i < m_NrVisibleChilds; i++)
		m_Children[i].pChild->ClearTree();
}

template <typename ChildData>
void ParentElem<ChildData>::CreateTree()
{
	Create();
	for (unsigned i = 0; i < m_NrVisibleChilds; i++)
		m_Children[i].pChild->CreateTree();
}
}
}
