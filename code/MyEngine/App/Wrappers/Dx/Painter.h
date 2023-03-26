#pragma once

#include <Rendering/State/Shader.h>
#include <Rendering/State/Mesh.h>

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}
	
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				template<typename Vertex>
				class Painter
				{
				public:
					void SetShader(Rendering::Shader& shader);
					void SetMesh(Rendering::Mesh& mesh);
					void SetCamera(Game::Camera::Camera& camera);
					
					void Paint() const;

				private:
					Rendering::Shader* m_pShader{};
					Rendering::Mesh* m_pMesh{};
					Game::Camera::Camera* m_pCamera{};

					bool m_IsPainting{ false };
				};

				template <typename Vertex>
				void Painter<Vertex>::SetShader(Rendering::Shader& shader)
				{
					m_pShader = &shader;
					m_pShader->Activate();
				}

				template <typename Vertex>
				void Painter<Vertex>::SetMesh(Rendering::Mesh& mesh)
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
				void Painter<Vertex>::Paint() const
				{
					m_pMesh->Draw();
				}
			}
		}
	}
}
