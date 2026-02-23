cbuffer CameraBuffer
{
    float3 camera_position;
    float padding;
}

struct Vertex
{
    float3 localPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;

    float4x4 model : INST_MODEL;
    float4x4 modelViewProjection : INST_MODEL_VIEW_PROJECTION;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION2;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D texture_main : register(t0);
SamplerState sampler_main : register(s0);

Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.pos = mul(float4(vertex.localPos, 1), vertex.modelViewProjection);
    pixel.worldPos = (float3) mul(float4(vertex.localPos, 1), vertex.model);
    pixel.normal = mul(vertex.normal, (float3x3) vertex.model);
    pixel.uv = vertex.uv;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = dot(normalize(camera_position - pixel.worldPos), normalize(pixel.normal));
    const float3 color = (float3) texture_main.Sample(sampler_main, pixel.uv);
    return float4(color * lambert, 1);
}
