#pragma once
#include <Framework\Globals.h>
#include "UiSystem.h"

namespace MyEngine
{
namespace Ui
{
template<typename TRenderer>
class GraphicsId
{
public:
	GraphicsId() = default;
	GraphicsId(unsigned id);

	//no auto clearing by design
	~GraphicsId();
	GraphicsId(const GraphicsId<TRenderer>& other) = default;
	GraphicsId& operator=(const GraphicsId<TRenderer>& other) = default;
	GraphicsId(GraphicsId<TRenderer>&& other);
	GraphicsId& operator=(GraphicsId<TRenderer>&& other) noexcept;

	void Clear();
	unsigned GetId() const { return m_Id; }

	bool IsEmpty() const { return m_Id == EMPTY; }
	bool IsActive() const { return m_Id != EMPTY; }

	static constexpr unsigned EMPTY{ Uint::MAX };

protected:
	unsigned m_Id{ EMPTY };

	TRenderer& GetRenderer();
};

template<typename TRenderer>
inline GraphicsId<TRenderer>::GraphicsId(unsigned id)
	: m_Id{ id }
{
}

template<typename TRenderer>
inline GraphicsId<TRenderer>::~GraphicsId()
{
	m_Id = EMPTY;
}

template<typename TRenderer>
inline GraphicsId<TRenderer>::GraphicsId(GraphicsId<TRenderer>&& other)
	: m_Id{ other.m_Id }
{
	other.m_Id = EMPTY;
}
template<typename TRenderer>
inline GraphicsId<TRenderer>& GraphicsId<TRenderer>::operator=(GraphicsId<TRenderer>&& other) noexcept
{
	//no self check
	m_Id = other.m_Id;
	other.m_Id = EMPTY;
	return *this;
}
template<typename TRenderer>
inline void GraphicsId<TRenderer>::Clear()
{
	if (m_Id == EMPTY)
		return;

	GetRenderer().Remove(m_Id);
	m_Id = EMPTY;
}
template<>
inline UiRectRenderer& GraphicsId<UiRectRenderer>::GetRenderer()
{
	return UI_RECT;
}
template<>
inline UiFontRenderer& GraphicsId<UiFontRenderer>::GetRenderer()
{
	return UI_FONT;
}
template<>
inline UiShapeRenderer& GraphicsId<UiShapeRenderer>::GetRenderer()
{
	return UI_SHAPE;
}
template<>
inline UiImageRenderer& GraphicsId<UiImageRenderer>::GetRenderer()
{
	return UI_IMAGE;
}

using FontId = GraphicsId<UiFontRenderer>;
using ImageId = GraphicsId<UiImageRenderer>;
using ShapeId = GraphicsId<UiShapeRenderer>;

class RectId
	: public GraphicsId<UiRectRenderer>
{
public:
	RectId() = default;
	RectId(unsigned id);

	void SetColor(const Float3& newColor);
	void MoveTo(const Float2& leftBot);
};

}
}