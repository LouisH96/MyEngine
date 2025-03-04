#include "pch.h"
#include "Image.h"

#include "..\UiSystem.h"

using namespace Ui;
using namespace Rendering;

Ui::Image::Image(unsigned textureId, const Float2& uvLeftTop, const Float2& uvRightBot)
	: m_TextureId{ textureId }
	, m_UvLeftTop{ uvLeftTop }
	, m_UvRightBot{ uvRightBot }
{
}

void Ui::Image::TreeUpdate(const ResizePref& pref)
{
	SetSize(pref);
}

void Ui::Image::Clear()
{
}

void Ui::Image::Create()
{
	Globals::pUi->GetImageRenderer().Add(GetBounds(), m_UvLeftTop, m_UvRightBot, m_TextureId);
}
