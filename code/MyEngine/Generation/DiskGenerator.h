#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class DiskGenerator
		{
		public:
			struct Options
			{
				float Radius = 1;
				unsigned NrPoints = 32;
			};

			static unsigned GetNrVertices(const Options& options);
			static unsigned GetNrIndices(const Options& options);

			template<typename Combinator, typename Adder>
			static void Generate(Combinator combinator, Adder&& adder, const Options& options);

			template<typename Adder>
			static void GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options);

			static Array<Float3> GenerateXz(const Float3& center, float radius, unsigned nrPoints = 32);
		};

		template <typename Combinator, typename Adder>
		void DiskGenerator::Generate(Combinator combinator, Adder&& adder, const Options& options)
		{
			const float radius{ options.Radius };
			const float angleStep{ Constants::PI2 / static_cast<float>(options.NrPoints) };

			for (unsigned i = 0; i < options.NrPoints; i++)
			{
				const float angle{ angleStep * static_cast<float>(i) };
				const Float2 point{ cosf(angle) * radius, sinf(angle) * radius };
				adder.Add(combinator(point));
			}
		}

		template <typename Adder>
		void DiskGenerator::GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options)
		{
			int i0 = 0;
			int i1 = 1;
			int i2 = 2;
			while (i2 != i0)
			{
				//first triangle
				adder.Add(i0 + firstVertexIdx);
				adder.Add(i2 + firstVertexIdx);
				adder.Add(i1 + firstVertexIdx);

				//second triangle
				i1 = i2;
				i2 = i0 - 1;
				if (i2 < 0) i2 += GetNrVertices(options);
				if (i1 == i2) return;

				adder.Add(i0 + firstVertexIdx);
				adder.Add(i2 + firstVertexIdx);
				adder.Add(i1 + firstVertexIdx);

				//find next
				i0 = i2;
				i2 = i1 + 1;
			}
		}
	}
}
