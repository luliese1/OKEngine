Texture2D gRenderTargetTexture[8];

//텍스쳐병함
struct VertexIn
{
    float3 Pos : POSITION; //로컬좌표
    float2 UV : TEXCOORD0;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD0;
};

SamplerState g_samLinear : register(s0);

VertexOut MergeVS(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosH = float4(vin.Pos, 1.0f);
    vout.UV = vin.UV;
    
    return vout;
}

float4 MergePS(VertexOut pin) : SV_Target
{

    float4 Color = gRenderTargetTexture[0].Sample(g_samLinear, pin.UV);
    float4 Normal = gRenderTargetTexture[1].Sample(g_samLinear, pin.UV);
    float4 Depth = gRenderTargetTexture[2].Sample(g_samLinear, pin.UV);
    float4 Pos = gRenderTargetTexture[3].Sample(g_samLinear, pin.UV);
    
    return Color;
}
