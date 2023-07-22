#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class DiskGenerator
		{
		public:
			static Array<Float3> GenerateXz(const Float3& center, float radius, unsigned nrPoints = 32);

		private:
		};
	}
}
