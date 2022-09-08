//#include "PostProcessing.fx"

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldViewProj;
    uint gScreenWidth;
    uint gScreenHeight;
};

Texture2D gRenderTargetTexture[8];
//Texture2DMS<float4> gRenderTargetTexture[8];



SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

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

float4 MergePS(VertexOut pin) : SV_Target
{
    
    //float4 Color = gRenderTargetTexture[0].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Depth = gRenderTargetTexture[1].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Normal = gRenderTargetTexture[2].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    //float4 Pos = gRenderTargetTexture[3].Load(int2(pin.UV.x * gScreenWidth, pin.UV.y * gScreenHeight), 0);
    
    float4 Color = gRenderTargetTexture[0].Sample(samAnisotropic, pin.UV);
    float4 Normal = gRenderTargetTexture[1].Sample(samAnisotropic, pin.UV);
    float4 Depth = gRenderTargetTexture[2].Sample(samAnisotropic, pin.UV);
    float4 Pos = gRenderTargetTexture[3].Sample(samAnisotropic, pin.UV);
    //return float4(1.f, 1.f, 1.f, 1.f);
    
    return Color;
}


static float3 BlurWeight[12] =
{
    -3, 0, 0.111, -2, 0, 0.250, -1, 0, 1.000,
	+3, 0, 0.111, +2, 0, 0.250, +1, 0, 1.000,
	
	 0, -3, 0.111, 0, -2, 0.250, 0, -1, 1.000,
	 0, +3, 0.111, 0, +2, 0.250, 0, +1, 1.000
};

float4 PS_Blur(VertexOut pin) : SV_Target
{
    float4 AverageColor = 0;
    float4 Pixel = 0;

    for (int i = 0; i < 12; ++i)
    {
        float2 TexelSize = float2(1.0f / (float) gScreenWidth, 1.0f / (float) gScreenHeight);
        
		// �ֺ� ������ �����´�. 12���� ���� ��ճ��� �ʹ�.
        Pixel = gRenderTargetTexture[0].Sample(samAnisotropic, pin.UV + (BlurWeight[i].xy) * TexelSize) / 12.0f;;
		
		// 0���� 1�� ������ �ٲ� �ش�.
		// Clamps the specified value within the range of 0 to 1.
        Pixel = saturate(Pixel);
		
		// 12���� ��������� ���� �ʹ�.
        AverageColor += Pixel;
    }

    return AverageColor;
}

//�븻�� �ؽ��� �Ѵ� ������
technique11 DEFAULT
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, MergeVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, MergePS()));
    }
}

