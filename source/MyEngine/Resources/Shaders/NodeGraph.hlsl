cbuffer CameraBuffer : register(b0)
{
    float3x3 camera_view;
};

struct Vertex
{
    float2 pos : POSITION;
    float3 color : COLOR;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.pos = float4(mul(float3(vertex.pos, 1), camera_view), 1);
    pixel.color = vertex.color;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    return float4(pixel.color, 1);
}
