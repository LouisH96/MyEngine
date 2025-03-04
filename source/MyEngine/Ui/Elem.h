#pragma once
#include <Geometry/Shapes/Rects.h>

namespace MyEngine
{
namespace Ui
{
class UiShapeRenderer;

enum class FillMode
{
	Min, Max, NoPref
};

struct ResizePref
{
	Float2 minSize;
	Float2 maxSize;
	FillMode horMode{};
	FillMode verMode{};

	void SetMin();
	void SetMax();
	Float2 GetPreferredSize() const;
};

class Elem
{
public:
	Elem(bool takeMouse = false);
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
	virtual void OnMouseDown(Float2 relativePos);

	virtual const std::string GetTypeName() const = 0;

	void SetTakeMouse(bool takeMouse);
	virtual void SetChildrenTakeMouse(bool takeMouse);
	void MakeTakeMouseParent();
	void SetTakeMouseAll(bool takeMouse); //self & children

protected:
	void SetSize(const Float2& size);
	void SetSize(const ResizePref& pref);
	void SetWidth(float width);
	void SetHeight(float height);

	void RequestUpdate();

	//---| Resizing |---
	/* Functions that will be called for the resizing process:
	 * 1) TreeUpdate(..) -> In that function the element should:
	 *		- Update it's own position
	 *		- Call TreeUpdate on all it's children
	 *		- Call SetPosition on all it's children
	 * 2) MoveAllPositions(..) -> This will move each element and it's children to the global position
	 * 3) ClearTree() -> Destroys visuals
	 * 4) CreateTree() -> Creates visuals
	 */
	virtual void TreeUpdate(const ResizePref& pref) = 0; //update this size, its children size and their relative positions
	virtual void MoveAllPositions(const Float2& position);
	void SetPosition(const Float2& position); //relative to parent

	//---| Visuals |---
	void RecreateTree();
	virtual void ClearTree();
	virtual void CreateTree();
	virtual void Clear() = 0;
	virtual void Create() = 0;

	static void ChildTreeUpdate(Elem& child, const ResizePref& pref);
	static void ChildSetPosition(Elem& child, const Float2& position);
	static void ChildMoveAllPositions(Elem& child, const Float2& position);
	static void ChildClearTree(Elem& child);
	static void ChildCreateTree(Elem& child);

	static void ChildTreeUpdate(Elem* pChild, const ResizePref& pref);
	static void ChildSetPosition(Elem* pChild, const Float2& position);
	static void ChildMoveAllPositions(Elem* pChild, const Float2& position);
	static void ChildClearTree(Elem* pChild);
	static void ChildCreateTree(Elem* pChild);

	static void AssertWithinMaxSize(const Float2& desired, const ResizePref& pref);

	static UiShapeRenderer& Draw();
private:
	RectFloat m_Bounds;
	bool m_TakeMouse;

	template<typename T>
	friend class ParentElem;
	friend class TreeManager;
};
}
}
