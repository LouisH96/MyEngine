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

	SamplerState(BorderMode borderX = BorderMode::Repeat, BorderMode borderY = BorderMode::Repeat);
	SamplerState(BorderMode borderX, BorderMode borderY, Float4 value);
	~SamplerState();
	SamplerState(const SamplerState& other) = delete;
	SamplerState(SamplerState&& other) noexcept = delete;
	SamplerState& operator=(const SamplerState& other) = delete;
	SamplerState& operator=(SamplerState&& other) noexcept = delete;

	void ActivateVs() const;
	void ActivatePs() const;
	void Activate() const;

private:
	ID3D11SamplerState* m_pSamplerState{};

	static constexpr D3D11_TEXTURE_ADDRESS_MODE ToDx(BorderMode mode);
	static constexpr BorderMode ToBorderMode(D3D11_TEXTURE_ADDRESS_MODE mode);

	static constexpr D3D11_SAMPLER_DESC MakeDesc(BorderMode borderX, BorderMode borderY);
	void MakeDx(const D3D11_SAMPLER_DESC& desc);
};
}
}
