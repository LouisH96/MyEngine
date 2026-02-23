
cbuffer CameraBuffer : register(b0)
{
    float2 camera_multiplier;
    float2 padding;
}

struct Vertex
{
    float2 pos : POSITION;
    float3 color : COLOR;
};

struct Pixel
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.pos.x = vertex.pos.x * camera_multiplier.x - 1;
    pixel.pos.y = vertex.pos.y * camera_multiplier.y - 1;
    pixel.pos.zw = float2(1, 1);
    pixel.color = vertex.color;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    return float4(pixel.color, 1);
}