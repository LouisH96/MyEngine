
struct Vertex
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
    float2 offset : INST_POS_OFFSET;
    float2 size : INST_SIZE;
    float2 uvScale : INST_UV_SCALE;
    float2 uvOffset : INST_UV_OFFSET;
    float3 color : INST_COLOR;
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
    const float2 pos = vertex.pos * vertex.size + vertex.offset;
    const float2 uv = vertex.uv * vertex.uvScale + vertex.uvOffset;

    Pixel pixel = (Pixel) 0;
    pixel.pos = float4(pos, .5f, 1);
    pixel.uv = uv;
    pixel.color = vertex.color;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float4 color = g_texture.Sample(g_sampler, pixel.uv) * float4(pixel.color, 1);
    return color;
}
