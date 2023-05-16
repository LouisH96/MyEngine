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
				class Model;

				class NodeAttribute
				{
				public:
					NodeAttribute() = default;
					explicit NodeAttribute(Reading::FbxObject& object);

					int64_t Id;
					std::string Type;
					Reading::FbxObject* pProperties70; //temp

					void SetParentModel(const Model& parentModel);

				private:
					const Model* m_pParentModel{};
				};
			}
		}
	}
}
