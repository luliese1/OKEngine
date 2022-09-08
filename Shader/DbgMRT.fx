#include "LightHelper.fx"

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldViewProj;
};

Texture2D gRenderTargetTexture;
//Texture2DMS<float4> gRenderTargetTexture[8];

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

//텍스쳐병함
struct VertexIn
{
    float3 Pos : POSITION; //로컬좌표
    float2 UV : TEXCOORD0;
};
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 Pos : POSITION; //로컬좌표
    float2 UV : TEXCOORD0;
};

VertexOut MergeVS(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosH = float4(vin.Pos, 1.0f);
    vout.UV = vin.UV;
    
    return vout;
}

float4 MergePS(VertexOut pin) : SV_Target
{
    
    //float4 Color = gRenderTargetTexture[0].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Depth = gRenderTargetTexture[1].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Normal = gRenderTargetTexture[2].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Pos = gRenderTargetTexture[3].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    
    float4 Color =  gRenderTargetTexture.Sample(samAnisotropic, pin.UV);

    //return float4(1.f, 1.f, 1.f, 1.f);
    
    return Color;
}

//노말맵 텍스쳐 둘다 있을떄
technique11 DEFAULT
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, MergeVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, MergePS()));
    }
}

