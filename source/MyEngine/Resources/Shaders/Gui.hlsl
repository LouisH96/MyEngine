struct Vertex
{
    float2 localPos : POSITION; //(0,0) = center, bounds = (-.5,5)

    float3 offset : INST_POSITION;
    float3 col : INST_COLOR;
    float2 size : INST_SIZE;
};

struct Pixel
{
    float4 pos : SV_Position;
    float3 col : COLOR;
};

Pixel vs_main(Vertex vertex)
{
    Pixel pixel;
    pixel.pos = float4(
		vertex.localPos * vertex.size + vertex.offset.xy,
		vertex.offset.z, 1);
    pixel.col = vertex.col;
    return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
    return float4(pixel.col, 1);
}
