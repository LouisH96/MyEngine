//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 camera_matrix;
    float3 camera_pos;
    float padding;
}

//--| Textures |--
Texture2D texture_main : register(t0);
SamplerState sampler_main : register(s0);

static const float3 invLightDir = normalize(float3(1, 1, -1));

//--| STRUCTS |---
struct Vertex
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float2 uv : TEXCOORD;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float3 worldPos : POSITION2;
    float2 uv : TEXCOORD;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
    Pixel pixel = (Pixel) 0;
    pixel.worldPos = vertex.pos;
    pixel.pos = mul(float4(vertex.pos, 1), camera_matrix);
    pixel.uv = vertex.uv;
    pixel.norm = vertex.norm;
    return pixel;
}

//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = dot(invLightDir, normalize(pixel.norm));
	const float3 color = texture_main.Sample(sampler_main, pixel.uv);
    return float4(color * lambert, 1);
}
