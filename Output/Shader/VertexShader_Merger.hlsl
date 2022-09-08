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

VertexOut MergeVS(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosH = float4(vin.Pos, 1.0f);
    vout.UV = vin.UV;
    
    return vout;
}
