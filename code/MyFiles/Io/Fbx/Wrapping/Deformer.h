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
				class Model;
				class Geometry;

				struct IDeformerData
				{
					virtual ~IDeformerData() = default;
					virtual IDeformerData* Clone() const = 0;
				};

				struct DeformerSkinData final : IDeformerData
				{
					explicit DeformerSkinData(const Reading::FbxElement& object);
					IDeformerData* Clone() const override;

					int Version{};
					double Accuracy{};
					std::string SkinningType{};
				};

				struct DeformerClusterData final : IDeformerData
				{
					explicit DeformerClusterData(Reading::FbxElement& object);
					IDeformerData* Clone() const override;

					int Version{};
					Array<int> Indexes{};
					Array<double> Weights{};
					Array<double> Transform{};
					Array<double> TransformLink{};
				};

				class Deformer
				{
				public:
					Deformer() = default;
					explicit Deformer(Reading::FbxElement& object);
					~Deformer();

					Deformer(const Deformer& other);
					Deformer(Deformer&& other) noexcept;
					Deformer& operator=(const Deformer& other);
					Deformer& operator=(Deformer&& other) noexcept;

					int64_t Id{};

					void SetParentGeometry(const Geometry& geometry);
					void SetParentDeformer(const Deformer& deformer);
					void AddChildDeformer(const Deformer& deformer);
					void SetModel(const Model& model);

					const Array<const Deformer*>& GetChildDeformers() const { return m_ChildDeformers; }
					const Model* GetModel() const { return m_pModel; }

					bool HasSkinData() const { return dynamic_cast<const DeformerSkinData*>(m_pData); }
					bool HasClusterData() const { return dynamic_cast<const DeformerClusterData*>(m_pData); }

					const DeformerSkinData& GetSkinData() const { return *reinterpret_cast<const DeformerSkinData*>(m_pData); }
					const DeformerClusterData& GetClusterData() const { return *reinterpret_cast<const DeformerClusterData*>(m_pData); }


				private:
					IDeformerData* m_pData{};
					const Geometry* m_pParentGeometry{};
					const Deformer* m_pParentDeformer{};
					const Model* m_pModel{};
					Array<const Deformer*> m_ChildDeformers{};
				};
			}
		}
	}
}
