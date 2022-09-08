#include "ShaderHelper.hlsl"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerFrame
cbuffer cbPerFrame : register(b0)
{
    Camera g_Camera : packoffset(c0);
    
};
//// PerObject
cbuffer cbPerObject : register(b1)
{
    uint DirectionalLightCnt : packoffset(c0.x);
    uint PointLightCnt : packoffset(c0.y);
    uint SpotLightCnt : packoffset(c0.z);
    uint pad : packoffset(c0.w);
    
    DirectionalLight g_DirLight[3] : packoffset(c1);
    PointLight g_PointLight[10] : packoffset(c16);
    SpotLight g_SpotLight[10] : packoffset(c66);
};
//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_Color : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_PosW : register(t2);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct PS_OUT
{
    float4 result : SV_Target0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_OUT PSMain(PS_INPUT Input)
{
    PS_OUT Output;
    
    float4 albedo = g_Color.Sample(g_samLinear, Input.Texcoord0);
    float3 Normal = g_Normal.Sample(g_samLinear, Input.Texcoord0).xyz;
    float3 PosW = g_PosW.Sample(g_samLinear, Input.Texcoord0).xyz;
    
    float3 eyeVec = g_Camera.g_ViewMatrix[3].xyz - PosW;
    
    Material Mat;
    Mat.Ambient = float4(0.5f, 0.5f, 0.5f, 1.0f);
    Mat.Diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
    Mat.Specular = float4(0.5f, 0.5f, 0.5f, 1.0f);
    Mat.Reflect = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    float4 A, D, S;
    
    for (uint diridx = 0; diridx < DirectionalLightCnt; diridx++)
    {
        ComputeDirectionalLight(Mat, g_DirLight[diridx], Normal, eyeVec, A, D ,S);
        Ambient += A;
        Diffuse += D;
        Specular += S;
    }
    
    for (uint Pointidx = 0; Pointidx < PointLightCnt; Pointidx++)
    {
        ComputePointLight(Mat, g_PointLight[Pointidx], PosW, Normal, eyeVec, A, D, S);
        Ambient += A;
        Diffuse += D;
        Specular += S;
    }
    
    for (uint Spotidx = 0; Spotidx < Spotidx; Spotidx++)
    {
        ComputeSpotLight(Mat, g_SpotLight[Spotidx], PosW, Normal, eyeVec, A, D, S);
        Ambient += A;
        Diffuse += D;
        Specular += S;
    }
    
    float4 litColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    litColor = (Ambient + Diffuse) * albedo + Specular;
    
    Output.result = float4(litColor.xyz, 1.f);

    return Output;
}