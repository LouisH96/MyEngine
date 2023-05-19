#pragma once
#include <d3d11.h>
#include <string>

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		class InputLayout
		{
		public:
			enum class ElementType
			{
				Float3, Float2
			};
			enum class SlotClass
			{
				PerVertex, PerInstance
			};
			struct Element
			{
				const std::string& Semantic{};
				const ElementType Type{};
				const int InputSlot{ 0 };
				const SlotClass SlotClass{ SlotClass::PerVertex };
			};

			InputLayout(const Gpu& gpu, const Element* pElements, int nrElements);
			~InputLayout();
			InputLayout(const InputLayout& other) = delete;
			InputLayout(InputLayout&& other) noexcept = delete;
			InputLayout& operator=(const InputLayout& other) = delete;
			InputLayout& operator=(InputLayout&& other) noexcept = delete;

			void Activate(const Gpu& gpu) const;

		private:
			ID3D11InputLayout* m_pInputLayout{};

			static DXGI_FORMAT ToDxFormat(ElementType type);
			static D3D11_INPUT_CLASSIFICATION ToDx(SlotClass slotClass);
			static std::string CreateDummyShaderString(const Element* pElements, int nrElements);
			static std::string ToTypeString(ElementType type);
		};
	}
}

