#include "pch.h"
#include "Image.h"

#include "..\NewUiSystem.h"

using namespace NewUi;
using namespace Rendering;

NewUi::Image::Image(unsigned textureId, const Float2& uvLeftTop, const Float2& uvRightBot)
	: m_TextureId{ textureId }
	, m_UvLeftTop{ uvLeftTop }
	, m_UvRightBot{ uvRightBot }
{
}

void NewUi::Image::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref);
}

void NewUi::Image::Clear()
{
}

void NewUi::Image::Create()
{
	Globals::pUi->GetImageRenderer().Add(GetBounds(), m_UvLeftTop, m_UvRightBot, m_TextureId);
}
