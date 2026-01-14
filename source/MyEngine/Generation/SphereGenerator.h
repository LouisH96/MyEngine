#pragma once
#include <Generation\StripLoopGenerator.h>
#include <Rendering\Structs\VertexTypes.h>
#include <Generation\MeshSection.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
	namespace Generation
	{
		struct SphereGeneratorOptions
		{
			float Radius{ 1.f };
			unsigned NrLayers{ 24 };
			unsigned NrCorners{ 24 };
		};

		class SphereGenerator
		{
		public:
			using Options = SphereGeneratorOptions;

			template<typename Combinator, typename M>
			static MeshSection<typename M::VertexType, M::TOPOLOGY> Generate(Combinator combinator, M& meshData, const Options& options);
		};

		template<typename Combinator, typename M>
		inline MeshSection<typename M::VertexType, M::TOPOLOGY> SphereGenerator::Generate(Combinator combinator, M& meshData, const Options& options)
		{
			//Create MeshSection
			MeshSection<typename M::VertexType, M::TOPOLOGY> section{ meshData };

			//create StripLoopGenerator
			using Generator = StripLoopGenerator<typename M::VertexType, M::TOPOLOGY>;

			typename Generator::Options stripGeneratorOptions{};
			stripGeneratorOptions.NrCorners = options.NrCorners;

			Generator generator{ meshData, meshData.Vertices.GetSize(), stripGeneratorOptions };

			//start cap
			generator.SetStartCap(combinator(
				{ 0, -options.Radius, 0 },
				{ 0,-1,0 }
			));

			//middle
			const float layerAngleStep{ Constants::PI / options.NrLayers };
			const float cornerAngleStep{ Constants::PI2 / options.NrCorners };

			for (unsigned iLayer = 0; iLayer < options.NrLayers; iLayer++)
			{
				const float layerAngle{ layerAngleStep * iLayer };
				const float layerCos{ cos(layerAngle) };
				const float layerSin{ sin(layerAngle) };

				const float layerRadius{ layerSin * options.Radius };
				const float layerHeight{ -layerCos * options.Radius };

				Float3 position{ 0, layerHeight, 0 };
				Float3 normal{ 0, -layerCos, 0 };

				for (unsigned iCorner = 0; iCorner < options.NrCorners; iCorner++)
				{
					const float cornerAngle{ cornerAngleStep * iCorner };
					const float cornerCos{ cos(cornerAngle) };
					const float cornerSin{ sin(cornerAngle) };

					position.x = cornerCos * layerRadius;
					position.z = cornerSin * layerRadius;

					normal.x = cornerCos * layerSin;
					normal.z = cornerSin * layerSin;

					generator.AddVertex(combinator(position, normal));
				}
			}

			//end cap
			generator.SetEndCap(combinator(
				{ 0, options.Radius, 0 },
				{ 0, 1, 0 }
			));

			//finish meshSection
			section.Finish();
			return section;
		}
	}
}