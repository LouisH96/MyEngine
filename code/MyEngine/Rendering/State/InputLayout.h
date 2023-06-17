#pragma once
#include <d3d11.h>
#include <string>

namespace MyEngine
{
	namespace Rendering
	{
		class InputLayout
		{
		public:
			enum class ElementType
			{
				Float4, Float3, Float2, Float
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
				const int SemanticIndex{ 0 };
			};

			template<typename T> static InputLayout FromType();

			InputLayout(const Element* pElements, int nrElements);
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
		};

		template <typename T>
		InputLayout InputLayout::FromType()
		{
			return { T::ELEMENTS, T::NR_ELEMENTS };
		}
	}
}

