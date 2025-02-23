#pragma once
#include <Framework\Globals.h>
#include "NewUiSystem.h"

namespace MyEngine
{
namespace NewUi
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

	static constexpr unsigned EMPTY{ Uint::MAX };

private:
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
inline NewUiRectRenderer& GraphicsId<NewUiRectRenderer>::GetRenderer()
{
	return UI_RECT;
}
template<>
inline NewUiFontRenderer& GraphicsId<NewUiFontRenderer>::GetRenderer()
{
	return UI_FONT;
}
template<>
inline NewUiShapeRenderer& GraphicsId<NewUiShapeRenderer>::GetRenderer()
{
	return UI_SHAPE;
}
template<>
inline NewUiImageRenderer& GraphicsId<NewUiImageRenderer>::GetRenderer()
{
	return UI_IMAGE;
}

using RectId = GraphicsId<NewUiRectRenderer>;
using FontId = GraphicsId<NewUiFontRenderer>;
using ImageId = GraphicsId<NewUiImageRenderer>;
using ShapeId = GraphicsId<NewUiShapeRenderer>;

}
}