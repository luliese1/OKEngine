//�ؽ��ĺ���
struct VertexIn
{
    float3 Pos : POSITION; //������ǥ
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
