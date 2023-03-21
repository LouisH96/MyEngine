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
			namespace Dx
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
					
					void Paint() const;

				private:
					Shader* m_pShader{};
					Mesh* m_pMesh{};
					Game::Camera::Camera* m_pCamera{};

					bool m_IsPainting{ false };
				};

				template <typename Vertex>
				void Painter<Vertex>::SetShader(Shader& shader)
				{
					m_pShader = &shader;
					m_pShader->Activate();
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
				void Painter<Vertex>::Paint() const
				{
					m_pMesh->Draw();
				}
			}
		}
	}
}
