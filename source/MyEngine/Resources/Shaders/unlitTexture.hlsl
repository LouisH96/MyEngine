//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 camera_matrix;
}

//--| STRUCTS |---
struct Vertex
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//--| Textures |--
Texture2D texture_main : register(t0);
SamplerState sampler_main : register(s0);

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
    Pixel pixel = (Pixel) 0;
    pixel.pos = mul(float4(vertex.pos, 1), camera_matrix);
    pixel.uv = vertex.uv;
    return pixel;
}


//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
	const float4 color = texture_main.Sample(sampler_main, pixel.uv);
    return color;
}
