#include "cbPerFrame.hlsl"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
#ifdef USE_SKINNING
#else
cbuffer cbPerObject : register(b1)
{
    Transfrom g_Transform : packoffset(c0);
    Material g_Material : packoffset(c12);
};
#endif
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
#ifdef USE_SKINNING
    struct VS_INPUT
    {
        float3 pos : POSITION;
        float2 uv : TEXCOORD;
        float3 normal : NORMAL;
        float3 tangent : TANGENT;
        float4 color : COLOR;

        float4 weight : WEIGHT;
        uint4 boneIndex : BONEINDEX;
    };
#else
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Texcoord0 : TEXCOORD0;
    float2 Texcoord1 : TEXCOORD1;
};
#endif
struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float2 Texcoord0 : TEXCOORD0;
    float2 Texcoord1 : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    
    Output.PosH = mul(float4(Input.Position, 1.0f), mul(g_Transform.g_World, g_Camera.g_ViewProjectionMatrix));
    Output.PosW = mul(float4(Input.Position, 1.0f), g_Transform.g_World);
    Output.Tangent = mul(Input.Tangent, (float3x3) g_Transform.g_WorldInvTranspose);
    Output.Normal = mul(Input.Normal, (float3x3) g_Transform.g_WorldInvTranspose);
    Output.Texcoord0 = Input.Texcoord0;
    Output.Texcoord1 = Input.Texcoord0;
    
    return Output;
}