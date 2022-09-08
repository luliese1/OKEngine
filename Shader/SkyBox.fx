cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

TextureCube gCubeMap;

SamplerState samTriLinearSam
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//Default
struct VertexIn
{
    float3 Pos : POSITION; //·ÎÄÃÁÂÇ¥
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD0;
    float2 Tex1 : TEXCOORD1;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj).xyww;
	vout.PosL = vin.Pos;
    
	return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
    return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}

RasterizerState NOCULL
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};


technique11 DEFAULT
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader(ps_5_0, PS()) );

        SetRasterizerState(NOCULL);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}
