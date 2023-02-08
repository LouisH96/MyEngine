#pragma once

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
			}
		}
	}
}
