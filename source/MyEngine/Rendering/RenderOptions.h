#pragma once

namespace MyEngine
{
struct RenderOptions
{
	static const unsigned Samples{ 2 };

	static bool UsingMultiSampling() { return Samples > 1; }
};
}