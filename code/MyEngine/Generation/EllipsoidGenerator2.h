#pragma once
#include <Geometry\ModelTopology.h>
#include <Generation\ClosedStripGenerator.h>
#include <Rendering\Mesh\MeshListData.h>
#include <Math\Vectors.h>

namespace MyEngine
{
	namespace Generation
	{
		struct EllipsoidGenerator2Options
		{
			Float3 Radia;
			unsigned NrLayers; //on y-axis
			unsigned NrCorners; //on xz-plane
		};

		class EllipsoidGenerator2
		{
		public:
			using Options = EllipsoidGenerator2Options;

			template<typename Combinator, typename MeshData>
			static void Generate(Combinator combinator, MeshData& meshData, unsigned firstVertex, const Options& options);
		};

		template<typename Combinator, typename MeshData>
		inline void EllipsoidGenerator2::Generate(Combinator combinator, MeshData& meshData, unsigned firstVertex, const Options& options)
		{
			using StripGenerator = ClosedStripGenerator<MeshData::VertexType, MeshData::TOPOLOGY>;
			typename StripGenerator::Options stripGeneratorOptions{};
			stripGeneratorOptions.NrCorners = options.NrCorners;

			StripGenerator generator{ meshData, firstVertex, stripGeneratorOptions };

			//start cap
			const Float3 startCapPos{ 0, -options.Radia[1], 0 };
			const Float3 startCapNormal{ 0, -1, 0 };

			generator.SetStartCap(combinator(startCapPos, startCapNormal));

			//middle
			const float invRadia[3]{
				1.f / options.Radia[0],
				1.f / options.Radia[1],
				1.f / options.Radia[2]
			};

			const float sliceAngleStep{ Constants::PI / (options.NrLayers + 1) };
			const float cornerAngleStep{ Constants::PI * 2 / options.NrCorners };

			for (unsigned iSlice = 0; iSlice < options.NrLayers; iSlice++)
			{
				const float sliceAngle{ sliceAngleStep * (iSlice + 1) };
				const float sliceAngleCos{ cos(sliceAngle) };
				const float sliceAngleSin{ sin(sliceAngle) };

				const float sliceHeight{ -sliceAngleCos * options.Radia[1] };
				const float sliceRadius{ sliceAngleSin * options.Radia[0] };

				Float3 point{ 0,sliceHeight, 0 };

				for (unsigned iCorner = 0; iCorner < options.NrCorners; iCorner++)
				{
					const float cornerAngle{ iCorner * cornerAngleStep };
					const float cornerAngleCos{ cos(cornerAngle) };
					const float cornerAngleSin{ sin(cornerAngle) };

					point.x = cornerAngleCos * sliceAngleSin * options.Radia[0];
					point.z = cornerAngleSin * sliceAngleSin * options.Radia[2];

					const Float3 normal{ Float3{
						cornerAngleCos * sliceAngleSin * invRadia[0],
						-sliceAngleCos * invRadia[1] ,
						cornerAngleSin * sliceAngleSin * invRadia[2]}.Normalized() };

					generator.AddVertex(combinator(point, normal));
				}
			}

			//end cap
			const Float3 endCapPos{ 0, options.Radia[1], 0 };
			const Float3 endCapNormal{ 0,1,0 };
			generator.SetEndCap(combinator(endCapPos, endCapNormal));
		}
	}
}