
static const uint MAX_BONES = 100;
static const uint BONES_PER_VERTEX = 4;
cbuffer BonesBuffer : register(b3)
{
    float4x4 bone_transforms[MAX_BONES];
}

float4x4 MakeBonesMatrix(uint4 boneIds, float4 boneWeights)
{
    float4x4 bone = float4x4(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);
    
    for (uint i = 0; i < BONES_PER_VERTEX; i++)
    {
        if (boneIds[i] < 0)
            break;
        bone += bone_transforms[boneIds[i]] * boneWeights[i];
    }
    
    return bone;
}