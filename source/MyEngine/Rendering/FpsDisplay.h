#pragma once

#include <Ui\Elements\DynamicNumberLabel.h>

namespace MyEngine
{
namespace Rendering
{
class FpsDisplay
{
public:
	FpsDisplay();

	void SetFps(unsigned fps);

private:
	static constexpr unsigned NR_CHAR{ 3 };
	static constexpr float FONT_SCALE{ 8 };

	DynamicNumberLabel* m_pLabel{};
};
}
}
