#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class Deformer
				{
				public:
					Deformer() = default;
					Deformer(Reading::FbxObject& object);

					int64_t Id;

					void SetParentDeformer(const Deformer& deformer);
					void AddChildDeformer(const Deformer& deformer);

				private:
					const Deformer* m_pParentDeformer{};
					Array<const Deformer*> m_ChildDeformers{};
				};
			}
		}
	}
}
