//--| CBUFFER |--
cbuffer CBuffer
{
    float4x4 camera_matrix;
    float3 cameraPos;
    float padding;
}

//--| STRUCTS |---
struct Vertex
{
	float3 pos : POSITION;
	float3 col : COLOR;
    float3 norm : NORMAL;
};

struct Pixel
{
	float4 pos : SV_POSITION;
	float3 col : COLOR;
    float3 norm : NORMAL;
    float3 worldPos : POSITION2;
};

static const float3 invLightDir = normalize(float3(1, 1, -1));

//--| VERTEX SHADER |--
Pixel vs_main(Vertex vertex)
{
	Pixel pixel = (Pixel)0;
    pixel.worldPos = vertex.pos;
    pixel.pos = mul(float4(vertex.pos, 1), camera_matrix);
	pixel.col = vertex.col;
    pixel.norm = vertex.norm;
	return pixel;
}


//--| PIXEL SHADER |--
float4 ps_main(Pixel pixel) : SV_TARGET
{
    const float lambert = dot(invLightDir, normalize(pixel.norm));
	return float4(pixel.col * lambert,1);
}
