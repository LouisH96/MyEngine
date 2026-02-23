
struct Vertex
{
    float2 pos : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 color : COLOR;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

Pixel vs_main(Vertex vertex)
{
    Pixel pixel = (Pixel) 0;
    pixel.pos = float4(vertex.pos, .5f, 1);
    pixel.uv = vertex.uv;
    pixel.color = vertex.color;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float4 color = g_texture.Sample(g_sampler, pixel.uv) * float4(pixel.color, 1);
    return color;
}
