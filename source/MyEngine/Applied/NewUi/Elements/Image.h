#pragma once
#include "..\Elem.h"

namespace MyEngine
{
namespace NewUi
{
class Image final
	: public Elem
{
public:
	//TextureId coming from NewUiImageRenderer
	Image(unsigned textureId, const Float2& uvLeftTop = { 0,0 }, const Float2& uvRightBot = { 1,1 });

	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	const std::string GetTypeName() const override { return "Image"; }

	unsigned GetTextureId() const { return m_TextureId; }
	void SetTextureId(unsigned newId) { m_TextureId = newId; }

private:
	void Clear() override;
	void Create() override;

	unsigned m_TextureId;
	Float2 m_UvLeftTop;
	Float2 m_UvRightBot;
};
}
}

