cbuffer CameraBuffer : register(b0)
{
    float3x3 camera_view;
};

struct Vertex
{
    float2 pos : POSITION;
    float3 col : COLOR;
    float2 uv : TEXCOORD;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
    float2 uv : TEXCOORD;
};

Texture2D texture_main : register(t0);
SamplerState sampler_main : register(s0);

Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.pos = float4(mul(float3(vertex.pos, 1), camera_view), 1);
    pixel.col = vertex.col;
    pixel.uv = vertex.uv;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    return texture_main.Sample(sampler_main, pixel.uv) * float4(pixel.col, 1);
}
