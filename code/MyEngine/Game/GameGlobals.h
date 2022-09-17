#pragma once

#define DELTA_TIME GameGlobals::GetDeltaTime();

namespace MyEngine
{
	namespace Game
	{
		class GameGlobals
		{
		public:
			static float GetDeltaTime() { return m_DeltaTime; }
			static void SetDeltaTime(float dt) { m_DeltaTime = dt; }
		private:
			static float m_DeltaTime;
		};
	}
}
