#pragma once
#include <cstdint>

#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxElement;
			}

			namespace Wrapping
			{
				class Geometry;

				class Deformer
				{
				public:
					Deformer() = default;
					Deformer(Reading::FbxElement& object);

					int64_t Id;

					void SetParentGeometry(const Geometry& geometry);
					void SetParentDeformer(const Deformer& deformer);
					void AddChildDeformer(const Deformer& deformer);

				private:
					const Geometry* m_pParentGeometry{};
					const Deformer* m_pParentDeformer{};
					Array<const Deformer*> m_ChildDeformers{};
				};
			}
		}
	}
}
