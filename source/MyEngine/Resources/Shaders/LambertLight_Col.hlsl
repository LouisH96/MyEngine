//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 camera_matrix;
    float3 camera_pos;
    float padding;
}

static const float3 invLightDir = normalize(float3(1, 1.2f, -2));

//--| STRUCTS |---
struct Vertex
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float3 color : COLOR;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float3 worldPos : POSITION2;
    float3 color : COLOR;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
    Pixel pixel = (Pixel) 0;
    pixel.worldPos = vertex.pos;
    pixel.pos = mul(float4(vertex.pos, 1), camera_matrix);
    pixel.color = vertex.color;
    pixel.norm = vertex.norm;
    return pixel;
}

//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = max(dot(invLightDir, normalize(pixel.norm)), .2);
    return float4(pixel.color * lambert, 1);
}
