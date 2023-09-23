#pragma once
#include "Gui/NdcUtils.h"
#include "Gui/UiRect.h"
#include "Math/Vectors.h"
#include "Rendering/Canvas.h"
#include "Rendering/Buffers/Buffer.h"
#include "Rendering/Buffers/InvalidateBuffer.h"
#include "Rendering/State/DepthStencilState.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/PrimitiveTopology.h"
#include "Rendering/State/Shader.h"

namespace MyEngine
{
	struct UiRect;

	namespace App
	{
		struct ResizedEvent;
	}
	namespace Rendering
	{
		template<typename Vertex, typename Instance>
		class NdcRectRenderer
		{
		public:
			//---| Constructors/Destructor |---
			NdcRectRenderer(const std::wstring& shader);

			//---| Operations |---
			void OnCanvasResized(const App::ResizedEvent& event);
			void Render();

			Instance& Get(int id);
			const Instance& Get(int id) const;
			void Remove(int id);
			int Add(const Float2& pivot, const Float2& offset, const Float2& size);
			int Add(const Float2& screenPivot, const Float2& elementPivot, const Float2& offset, const Float2& size);
			int Add(const UiRect& uiRect);
			int AddCenterBottom(const Float2& offset, const Float2& size);

			Float2 GetMouseNdc() const;
			int GetElementUnderMouse() const;
			void SetOffset(int id, const Float2& pixels);
			void SetOffsetX(int id, float xPixels);
			void SetOffsetY(int id, float yPixels);

		private:
			DepthStencilState m_DepthStencilState;
			InputLayout m_InputLayout;
			Shader m_Shader;

			Float2 m_InvCanvasSize;
			Array<Float2> m_Pivots{}; //Elements point at pivot is attached to screens point at pivot

			Buffer<Vertex> m_Vertices;
			InvalidateBuffer<Instance> m_Instances;
			D3D11_PRIMITIVE_TOPOLOGY m_Topology;

			static bool IsEmpty(const Instance& instance);
			static void SetEmpty(Instance& instance);
		};

		template <typename Vertex, typename Instance>
		NdcRectRenderer<Vertex, Instance>::NdcRectRenderer(const std::wstring& shader)
			: m_InputLayout{ InputLayout::FromTypes<Vertex, Instance>() }
			, m_Shader{ shader }
			, m_InvCanvasSize{ Float2{1} / Float2{Globals::pCanvas->GetSize()} }
			, m_Instances{ 4 }
			, m_Topology{ PrimitiveTopologyUtils::ToDx(PrimitiveTopology::TriangleStrip) }
		{
			Array<Vertex> vertices{ Vertex::GetCenterRect() };
			m_Vertices = Buffer<Vertex>(vertices.GetData(), vertices.GetSizeU());
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::OnCanvasResized(const App::ResizedEvent& event)
		{
			const Float2 scale{ Gui::NdcUtils::UpdateInvCanvasSize(event.NewSize, m_InvCanvasSize) };

			Instance* pInstance{ m_Instances.GetFirst() };
			for (int i = 0; i < m_Instances.GetCount(); i++, pInstance++)
			{
				if (!pInstance->IsValid()) continue;
				Gui::NdcUtils::Resize(scale, m_Pivots[i], pInstance->GetCenter(), pInstance->GetSize());
			}
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::Render()
		{
			m_DepthStencilState.Activate();
			m_InputLayout.Activate();
			m_Shader.Activate();

			PrimitiveTopologyUtils::Activate(m_Topology);
			m_Vertices.Activate(0);
			m_Instances.ActivateVertexBuffer(1);
			m_Instances.Draw(m_Vertices.GetCapacity());
		}

		template <typename Vertex, typename Instance>
		Instance& NdcRectRenderer<Vertex, Instance>::Get(int id)
		{
			return m_Instances[id];
		}

		template <typename Vertex, typename Instance>
		const Instance& NdcRectRenderer<Vertex, Instance>::Get(int id) const
		{
			return m_Instances[id];
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::Remove(int id)
		{
			m_Instances.Remove(id);
		}

		template <typename Vertex, typename Instance>
		int NdcRectRenderer<Vertex, Instance>::Add(const Float2& pivot, const Float2& offset, const Float2& size)
		{
			Instance instance{};
			Gui::NdcUtils::ScreenRectToNdc(m_InvCanvasSize, offset, size, pivot, instance.GetCenter(), instance.GetSize());

			const int idx{ m_Instances.Add(std::move(instance)) };
			m_Pivots.EnsureSize(idx + 1);
			m_Pivots[idx] = pivot;
			return idx;
		}

		template <typename Vertex, typename Instance>
		int NdcRectRenderer<Vertex, Instance>::Add(const Float2& screenPivot, const Float2& elementPivot,
			const Float2& offset, const Float2& size)
		{
			Instance instance{};
			Gui::NdcUtils::ScreenRectToNdc(m_InvCanvasSize, screenPivot, offset, size, elementPivot, instance.GetCenter(), instance.GetSize());

			const int idx{ m_Instances.Add(std::move(instance)) };
			m_Pivots.EnsureSize(idx + 1);
			m_Pivots[idx] = screenPivot;
			return idx;
		}

		template <typename Vertex, typename Instance>
		int NdcRectRenderer<Vertex, Instance>::Add(const UiRect& uiRect)
		{
			Instance instance{};
			Gui::NdcUtils::ScreenRectToNdc(m_InvCanvasSize, uiRect.Offset, uiRect.Size, uiRect.Pivot, instance.GetCenter(), instance.GetSize());

			const int idx{ m_Instances.Add(std::move(instance)) };
			m_Pivots.EnsureSize(idx + 1);
			m_Pivots[idx] = uiRect.Pivot;
			return idx;
		}

		template <typename Vertex, typename Instance>
		int NdcRectRenderer<Vertex, Instance>::AddCenterBottom(const Float2& offset, const Float2& size)
		{
			return Add({ 0,-1 }, offset, size);
		}

		template <typename Vertex, typename Instance>
		Float2 NdcRectRenderer<Vertex, Instance>::GetMouseNdc() const
		{
			return Gui::NdcUtils::GetMouseNdc(m_InvCanvasSize);
		}

		template <typename Vertex, typename Instance>
		int NdcRectRenderer<Vertex, Instance>::GetElementUnderMouse() const
		{
			const Float2 mouse{ Gui::NdcUtils::GetMouseNdc(m_InvCanvasSize) };
			for (int i = m_Instances.GetCpuData().GetLastIdx(); i >= m_Instances.GetCpuData().GetFirstIdx(); i--)
			{
				const Instance& instance{ m_Instances[i] };
				if (IsEmpty(instance)) continue;
				const Float2 botLeft{ instance.GetCenter() - instance.GetSize() * .5 };
				if (!mouse.IsRightAbove(botLeft)) continue;
				const Float2 topRight{ botLeft + instance.GetSize() };
				if (!mouse.IsLeftBelow(topRight)) continue;
				return i;
			}
			return -1;
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::SetOffset(int id, const Float2& pixels)
		{
			const Float2& pivot{ m_Pivots[id] };
			Instance& instance{ m_Instances[id] };
			Gui::NdcUtils::SetScreenSpaceOffset(m_InvCanvasSize, pivot, pixels, instance.GetSize(), instance.GetCenter());
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::SetOffsetX(int id, float xPixels)
		{
			const Float2& pivot{ m_Pivots[id] };
			Instance& instance{ m_Instances[id] };
			Gui::NdcUtils::SetScreenSpaceOffsetX(m_InvCanvasSize, pivot, xPixels, instance.GetSize(), instance.GetCenter());
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::SetOffsetY(int id, float yPixels)
		{
			const Float2& pivot{ m_Pivots[id] };
			Instance& instance{ m_Instances[id] };
			Gui::NdcUtils::SetScreenSpaceOffsetY(m_InvCanvasSize, pivot, yPixels, instance.GetSize(), instance.GetCenter());
		}

		template <typename Vertex, typename Instance>
		bool NdcRectRenderer<Vertex, Instance>::IsEmpty(const Instance& instance)
		{
			return !instance.IsValid();
		}

		template <typename Vertex, typename Instance>
		void NdcRectRenderer<Vertex, Instance>::SetEmpty(Instance& instance)
		{
			Instance::Invalidate(instance);
		}
	}
}
