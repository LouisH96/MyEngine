#pragma once
#include <d3d11.h>
#include <string>

#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace Rendering
	{
		class InputLayout
		{
		public:
			enum class ElementType
			{
				Float4X4, Float3X3,
				Float4, Float3, Float2, Float,
				Int4, Int3, Int2, Int,
				Uint4, Uint3, Uint2, Uint
			};
			enum class SlotClass
			{
				PerVertex, PerInstance
			};
			struct Element
			{
				std::string Semantic{};
				ElementType Type{};
				int InputSlot{ 0 };
				SlotClass SlotClass{ SlotClass::PerVertex };
			};

			template<typename T> static InputLayout FromType();
			template<typename Vertex, typename Instance> static InputLayout FromTypes();
			template<typename Vertex, typename Instance> static InputLayout FromTypes2();

			InputLayout(const Element* pElements, unsigned nrElements);
			~InputLayout();
			InputLayout(const InputLayout& other) = delete;
			InputLayout(InputLayout&& other) noexcept;
			InputLayout& operator=(const InputLayout& other) = delete;
			InputLayout& operator=(InputLayout&& other) noexcept;

			void Activate() const;

		private:
			ID3D11InputLayout* m_pInputLayout{};

			static DXGI_FORMAT ToDxFormat(ElementType type);
			static D3D11_INPUT_CLASSIFICATION ToDx(SlotClass slotClass);
			static std::string CreateDummyShaderString(const Element* pElements, int nrElements);
			static std::string ToTypeString(ElementType type);
			static unsigned ToNrDxElements(ElementType type);
			static void AddDxElements(D3D11_INPUT_ELEMENT_DESC*& dxElements, const Element& element);
			static Array<D3D11_INPUT_ELEMENT_DESC> ToDxDescriptions(const Element* pElements, int nrElements);
		};

		template <typename T>
		InputLayout InputLayout::FromType()
		{
			return { T::ELEMENTS, T::NR_ELEMENTS };
		}

		template <typename Vertex, typename Instance>
		InputLayout InputLayout::FromTypes()
		{
			Array<Element> elements{ Vertex::NR_ELEMENTS + Instance::NR_ELEMENTS };
			for (int i = 0; i < Vertex::NR_ELEMENTS; i++) elements[i] = Vertex::ELEMENTS[i];
			for (int i = 0; i < Instance::NR_ELEMENTS; i++) elements[i + Vertex::NR_ELEMENTS] = Instance::ELEMENTS[i];
			return { elements.GetData(), elements.GetSize() };
		}

		template <typename Vertex, typename Instance>
		InputLayout InputLayout::FromTypes2()
		{
			Array<Element> elements{ Vertex::NR_ELEMENTS + Instance::NR_ELEMENTS };
			for (int i = 0; i < Vertex::NR_ELEMENTS; i++) elements[i] = Vertex::ELEMENTS[i];
			for (int i = 0; i < Instance::NR_ELEMENTS; i++)
			{
				Element& element{ elements[i + Vertex::NR_ELEMENTS] };
				element = Instance::ELEMENTS[i];
				element.Semantic = "INST_" + element.Semantic;
				element.InputSlot = 1;
				element.SlotClass = SlotClass::PerInstance;
			}
			return { elements.GetData(), elements.GetSize() };
		}
	}
}

