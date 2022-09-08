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
    Transfrom g_Transform : packoffset(c0);
    Material g_Material : packoffset(c12);
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

struct PS_OUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Depth : SV_Target2;
    float4 PosW : SV_Target3;
    float4 MaterialID : SV_Target4;
    float4 Material : SV_Target5;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUT PSMain(PS_INPUT Input) : SV_TARGET
{
    PS_OUT psOut;
    
#ifdef USING_ALBEDO
    psOut.Color = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
#else
    psOut.Color = float4(0.f, 0.f, 0.f, 1.f);
#endif

#ifdef USING_NORMALMAP
    float3 normalMapSample = g_NormalMapTexture.Sample(g_samLinear, Input.Texcoord0).rgb;
        
        //노멀맵의 데이터는 0~1사이의 값 그러므로 -1~1사이의 값으로 바꿔준다.
    float3 normalT = normalMapSample * 2.0f - 1.0f;
        
    float3 N = Input.Normal;
    float3 T = normalize(Input.Tangent - dot(Input.Tangent, N) * N);
    float3 B = cross(N, T);
        
    float3x3 tanspaceMat = float3x3(T, B, N);
          
    psOut.Normal = float4(mul(normalT, tanspaceMat), 1.0f);
#else
    psOut.Normal = float4(Input.Normal, 1.0f);
#endif
    
    psOut.Depth = float4(Input.PosH.zzz, 1.0f);
    psOut.PosW = float4(Input.PosW.xyz, 1.0f);
    //float fLighting = saturate(dot(g_vLightDir, Input.Normal));
    //fLighting = max(fLighting, g_fAmbient);
    return psOut;
}