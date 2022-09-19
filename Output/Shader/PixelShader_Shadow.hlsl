#include "ShaderHelper.hlsl"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerFrame
cbuffer cbPerFrame : register(b0)
{
    Camera g_Camera : packoffset(c0); 
    
    uint DirectionalLightCnt : packoffset(c0.x);
    uint PointLightCnt : packoffset(c0.y);
    uint SpotLightCnt : packoffset(c0.z);
    uint pad : packoffset(c0.w);
    
    DirectionalLight g_DirLight[3] : packoffset(c1);
    PointLight g_PointLight[10] : packoffset(c16);
    SpotLight g_SpotLight[10] : packoffset(c66);
};
//// PerObject
cbuffer cbPerObject : register(b1)
{

};


//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_DiffuseTexture : register(t0);
Texture2D g_NormalMapTexture : register(t1);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float2 Texcoord0 : TEXCOORD0;
    float2 Texcoord1 : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    //float4 diffuse = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
    //
    ////알파값에 따라 그림자를 클리핑할 것인지.. 
    //clip(diffuse.a - 0.15f);
    
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
