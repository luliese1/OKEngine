cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
    float3 gcolor;
    bool IsUsingcolor;
};

Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

//Default
struct VertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};
struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Normal = vin.Normal;
	vout.Color = vin.Color;

	return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
    if (IsUsingcolor)
        return float4(gcolor, 1.0f);
	
	return pin.Color;
}


technique11 DEFAULT
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader(ps_5_0, PS()) );
	}
}
