#pragma once

namespace MyEngine
{
	namespace App
	{
		struct ResizedEvent
		{
			Int2 PreviousSize{};
			Int2 NewSize{};
		};
	}
}
