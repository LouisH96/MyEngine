#pragma once
#include "Shader.h"
#include "Mesh.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}
}
namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				class Shader;
				class Mesh;

				template<typename Vertex>
				class Painter
				{
				public:
					void SetShader(Shader& shader);
					void SetMesh(Mesh& mesh);
					void SetCamera(Game::Camera::Camera& camera);

					void BeginPaint();
					void EndPaint();
					void Paint() const;

				private:
					Shader* m_pShader{};
					Mesh* m_pMesh{};
					Game::Camera::Camera* m_pCamera{};

					bool m_IsPainting{ false };

					void OnCamUpdated();
				};

				template <typename Vertex>
				void Painter<Vertex>::SetShader(Shader& shader)
				{
					m_pShader = &shader;
					m_pShader->Activate();

					if (m_IsPainting && m_pCamera)
						m_pShader->OnCamUpdated(*m_pCamera);
				}

				template <typename Vertex>
				void Painter<Vertex>::SetMesh(Mesh& mesh)
				{
					m_pMesh = &mesh;
					m_pMesh->Activate();
				}

				template <typename Vertex>
				void Painter<Vertex>::SetCamera(Game::Camera::Camera& camera)
				{
					m_pCamera = &camera;
				}

				template <typename Vertex>
				void Painter<Vertex>::BeginPaint()
				{
					OnCamUpdated();
				}

				template <typename Vertex>
				void Painter<Vertex>::EndPaint()
				{
				}

				template <typename Vertex>
				void Painter<Vertex>::Paint() const
				{
					m_pMesh->Draw();
				}

				template <typename Vertex>
				void Painter<Vertex>::OnCamUpdated()
				{
					m_pShader->OnCamUpdated(*m_pCamera);
				}
			}
		}
	}
}
