#pragma once

struct ID3D11RasterizerState;

namespace MyEngine::Rendering
{
class RasterizerState
{
public:
	enum class Culling {
		Front, Back, None
	};

public:
	explicit RasterizerState(Culling culling = Culling::Back, bool isWireframe = false);
	~RasterizerState();
	RasterizerState(const RasterizerState& other) = delete;
	RasterizerState(RasterizerState&& other) noexcept = delete;
	RasterizerState& operator=(const RasterizerState& other) = delete;
	RasterizerState& operator=(RasterizerState&& other) noexcept = delete;

	void Activate() const;

	static RasterizerState MakeWireframe();
	static RasterizerState MakeDefault(bool isWireframe);

private:
	ID3D11RasterizerState* m_pState{};

	static D3D11_CULL_MODE ToDx(Culling culling);
};
}
