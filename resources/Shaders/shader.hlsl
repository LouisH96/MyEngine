//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 cameraMatrix;
}

//--| STRUCTS |---
struct Vertex
{
	float3 pos : POSITION;
	float3 col : COLOR;
};

struct Pixel
{
	float4 pos : SV_POSITION;
	float3 col : COLOR;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
	Pixel pixel = (Pixel)0;
    pixel.pos = mul(cameraMatrix, float4(vertex.pos, 1));
	pixel.col = vertex.col;
	return pixel;
}


//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
	return float4(pixel.col,1);
}
