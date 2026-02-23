//--| CBUFFER |--
cbuffer CameraBuffer : register(b0)
{
    float4x4 camera_matrix;
    float3 camera_pos;
    float camera_padding;
}

cbuffer ModelBuffer : register(b1)
{
    float4x4 model_matrix;
}

//--| STRUCTS |---
struct Vertex
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float3 col : COLOR;
};

struct Pixel
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float3 worldPos : POSITION2;
    float3 col : COLOR;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.worldPos = (float3) mul(float4(vertex.pos, 1), model_matrix);
    pixel.pos = mul(float4(pixel.worldPos, 1), camera_matrix);
    pixel.col = vertex.col;
    pixel.norm = mul(vertex.norm, (float3x3) model_matrix);
    return pixel;
}

//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = dot(normalize(camera_pos - pixel.worldPos), normalize(pixel.norm));
    const float minLambert = .2f;
    const float lambertRange = 1.f - minLambert;
    const float finalLambert = lambert * lambertRange + minLambert;
    
    return float4(pixel.col * finalLambert, 1);
}
