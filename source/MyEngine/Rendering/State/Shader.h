#pragma once
#include <d3d11.h>

namespace MyEngine
{
namespace Rendering
{

class Shader
{
public:
	class Function
	{
	public:
		using Flag = unsigned;
		static constexpr Flag Vertex{ 1 << 0 };
		static constexpr Flag Pixel{ 1 << 1 };
		static constexpr Flag Both{ Vertex | Pixel };
	};

	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept;

	explicit Shader(const std::wstring& fullPath);
	~Shader();

	void ActivateVs() const;
	void ActivatePs() const;

	template<Function::Flag Functions = Function::Both, bool UnsetOthers = true>
	void Activate() const;

private:
	ID3D11VertexShader* m_pVertexShader{};
	ID3D11PixelShader* m_pPixelShader{};

	void InitShaders(const std::wstring& fullPath);
	void UnsetPs() const;
	void UnsetVs() const;
};

template<Shader::Function::Flag Functions, bool UnsetOthers>
inline void Shader::Activate() const
{
	if constexpr (Functions & Function::Vertex)
		ActivateVs();
	else if constexpr (UnsetOthers)
		UnsetVs();

	if constexpr (Functions & Function::Pixel)
		ActivatePs();
	else if constexpr(UnsetOthers)
		UnsetPs();
}

}
}

