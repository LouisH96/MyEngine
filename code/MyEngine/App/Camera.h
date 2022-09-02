#pragma once
#include "ICamera.h"

namespace MyEngine
{
	namespace App
	{
		class Camera final
			: public ICamera
		{
		public:
			void Update() override;
		};
	}
}
