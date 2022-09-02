#pragma once

namespace MyEngine
{
	namespace App
	{
		class ICamera;
	}
}

namespace MyEngine
{
	namespace Gpu
	{
		class IMesh;
		class IShader;
		class ICanvas;

		class IPainter
		{
		public:
			IPainter(const IPainter& other) = delete;
			IPainter(IPainter&& other) noexcept = delete;
			IPainter& operator=(const IPainter& other) = delete;
			IPainter& operator=(IPainter&& other) noexcept = delete;

			IPainter() = default;
			virtual ~IPainter() = default;

			virtual void SetCanvas(ICanvas& canvas) = 0;
			virtual void SetShader(IShader& shader) = 0;
			virtual void SetMesh(IMesh& mesh) = 0;
			virtual void SetCamera(App::ICamera& camera) = 0;

			virtual void BeginPaint() = 0;
			virtual void EndPaint() = 0;
			virtual void Paint() const = 0;

		private:
			virtual void OnCamUpdated() = 0;
		};
	}
}
