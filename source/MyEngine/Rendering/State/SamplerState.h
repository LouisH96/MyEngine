#pragma once

struct ID3D11SamplerState;

namespace MyEngine
{
namespace Rendering
{
class Gpu;

class SamplerState
{
public:
	enum class BorderMode {
		Repeat, //1.2 -> 0.2
		Mirror,  //1.2 -> 0.8
		Clamp, //1.2 -> 1.0
		Value,
		MirrorOnce //Mirror then Clamp
	};

	enum class Filter {
		Point,
		Linear,
		Anisotropic
	};

	enum class Comparison {
		Equal,
		NotEqual,
		Less, LessEqual,
		Greater, GreaterEqual
	};

	SamplerState(BorderMode borderX = BorderMode::Repeat, BorderMode borderY = BorderMode::Repeat, Filter filter = Filter::Point);
	SamplerState(BorderMode borderX, BorderMode borderY, const Float4& outsideValue, Filter filter);

	//With compare
	SamplerState(BorderMode borderX, BorderMode borderY, Filter filter, Comparison compare);
	SamplerState(BorderMode borderX, BorderMode borderY, const Float4& outsideValue, Filter filter, Comparison compare);

	~SamplerState();
	SamplerState(const SamplerState& other) = delete;
	SamplerState(SamplerState&& other) noexcept = delete;
	SamplerState& operator=(const SamplerState& other) = delete;
	SamplerState& operator=(SamplerState&& other) noexcept = delete;

	void ActivateVs() const;
	void ActivatePs() const;
	void Activate() const;

private:
	using Desc = D3D11_SAMPLER_DESC;
	ID3D11SamplerState* m_pSamplerState{};

	static constexpr D3D11_TEXTURE_ADDRESS_MODE ToDx(BorderMode mode);
	static constexpr BorderMode ToBorderMode(D3D11_TEXTURE_ADDRESS_MODE mode);

	static constexpr D3D11_FILTER ToDx(Filter filter);
	static constexpr D3D11_FILTER ToDxWithCompare(Filter filter);
	static constexpr D3D11_COMPARISON_FUNC ToDx(Comparison comparison);

	static constexpr Desc MakeDesc();
	static void SetBorderMode(Desc& desc, BorderMode borderX, BorderMode borderY);
	static void SetOutsideValue(Desc& desc, const Float4& outsideValue);
	static void SetFilter(Desc& desc, Filter filter);
	static void SetFilter(Desc& desc, Filter filter, Comparison compare);
	void MakeDx(const D3D11_SAMPLER_DESC& desc);
};
}
}
