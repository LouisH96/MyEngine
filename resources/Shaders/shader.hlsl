
//--| STRUCTS |---
struct Vertex
{
	float3 pos : POS;
};

struct Pixel
{
	float4 pos : SV_POSITION;
};

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
	Pixel pixel = (Pixel)0;
	pixel.pos = float4(vertex.pos, 1);
	return pixel;
}


//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
	return float4(1,0,0,1);
}
