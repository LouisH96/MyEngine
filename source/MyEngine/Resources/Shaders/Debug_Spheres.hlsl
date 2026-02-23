//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 camera_matrix;
    float3 camera_pos;
    float padding;
}

//--| STRUCTS |---
struct Vertex
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    
    float3 instPos : INST_POSITION;
    float3 col : INST_COLOR;
    float size : INST_SIZE;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
    float3 norm : NORMAL;
    float3 worldPos : POSITION2;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.worldPos = (vertex.pos * vertex.size) + vertex.instPos;
    pixel.pos = mul(float4(pixel.worldPos, 1), camera_matrix);
    pixel.col = vertex.col;
    pixel.norm = vertex.norm;
    return pixel;
}

//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = dot(normalize(camera_pos - pixel.worldPos), normalize(pixel.norm));
    return float4(pixel.col * lambert, 1);
}
