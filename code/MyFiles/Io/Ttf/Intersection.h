#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			struct Intersection
			{
				float distance;
				bool rightIsInside;

				bool operator<(const Intersection& other) const;
				bool operator>(const Intersection& other) const;
			};
		}
	}
}
