struct Vertex
{
	float2 pos : POSITION;
	float3 col : COLOR;
};

struct Pixel
{
	float4 pos : SV_POSITION;
	float3 col : COLOR;
};

Pixel vs_main(Vertex vertex)
{
	Pixel pixel;
	pixel.pos = float4(vertex.pos, 0, 1);
	pixel.col = vertex.col;
	return pixel;
}

float4 ps_main(Pixel pixel) : SV_TARGET
{
	return float4(pixel.col,1);
}