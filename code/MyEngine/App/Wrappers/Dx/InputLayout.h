#pragma once
#include <d3d11.h>
#include <string>

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				class Gpu;
			}
		}
	}
}

namespace MyEngine
{
	namespace Dx
	{
		class InputLayout
		{
		public:
			enum class ElementType
			{
				Float3, Float2
			};
			struct Element
			{
				const std::string& Semantic{};
				const ElementType Type{};
			};

			InputLayout(const App::Wrappers::Dx::Gpu& gpu, const Element* pElements, int nrElements);
			~InputLayout();
			InputLayout(const InputLayout& other) = delete;
			InputLayout(InputLayout&& other) noexcept = delete;
			InputLayout& operator=(const InputLayout& other) = delete;
			InputLayout& operator=(InputLayout&& other) noexcept = delete;

			void Activate(const App::Wrappers::Dx::Gpu& gpu) const;

		private:
			ID3D11InputLayout* m_pInputLayout{};

			static DXGI_FORMAT ToDxFormat(ElementType type);
			static std::string CreateDummyShaderString(const Element* pElements, int nrElements);
			static std::string ToTypeString(ElementType type);
		};
	}
}

