#include "LightHelper.fx"

cbuffer cbPerFrame
{
	//���� ����...
	//�������� �������� ���� ����Ϸ���... ���۵巻����?
    DirectionalLight gDirLights;
    PointLight gPointLights;
    SpotLight gSpotLightLights;
	
	//���� ������ǥ?
    float3 gEyePosW;

	//�� �ʿ��� �̰�?
    uint gRenderFlag;
    //float gFogRange;
    //float4 gFogColor;
};
cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    Material gMaterial;
    
    //���� �����͸� Frame�� �־��ִ°� Object�� �־��ִ°�?
    //���� �����͸� Frame�� ���� �� �ִٸ� �����Ͱ����ϴ�.
    //�ϴ��� ������Ʈ ������ �ٸ� �����͸� �޾ƿͼ�.
    //����� ������.
    float4x4 gBone[32];
};

Texture2D gDiffuseMap;
Texture2D gNormalMap;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

//SKINNEDMESH
struct VertexIn
{
    float3 Pos : POSITION; //������ǥ
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD0;
    float2 Tex1 : TEXCOORD1;
    float4 Weight : BLENDWEIGHT;
    uint4 BoneIndex : BLENDINDICES;
};
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; //������ǥ
    float3 NormalW : NORMAL; //W��ǥ
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float2 Tex1 : TEXCOORD1;
};
struct PS_OUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Depth : SV_Target2;
    float4 Pos : SV_Target3;
};

VertexOut SkinnedVS(VertexIn vin)
{
    VertexOut vout;

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    
    
    for (int i = 0; i < 4; i++)
    {
        posL += vin.Weight[i] * mul(float4(vin.Pos, 1.0f), gBone[vin.BoneIndex[i]]).xyz;
        normalL += vin.Weight[i] * mul(vin.Normal, (float3x3) gBone[vin.BoneIndex[i]]).xyz;
    }
    
    vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
    vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;

	//�븻 ���Ͷ��� ���� ��ǥ�� �Űܼ� ����� �ؾ��Ѵ�.	
    vout.NormalW = normalize(mul(normalL, (float3x3) gWorldInvTranspose));
    vout.TangentW = mul(vin.Tangent, (float3x3) gWorld);
    
    vout.Tex = vin.Tex;
    vout.Tex1 = vin.Tex1;

    return vout;
}

PS_OUT PS(VertexOut pin,
    uniform bool gUseTexure,
    uniform bool gUseNormalMap) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);
    float3 eyeVec = normalize(gEyePosW - pin.PosW);
    
    float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    float4 A, D, S;
	
    ComputeDirectionalLight(gMaterial, gDirLights, pin.NormalW, eyeVec, A, D, S);
    Ambient += A;
    Diffuse += D;
    Specular += S;
    
    ComputePointLight(gMaterial, gPointLights, pin.PosW, pin.NormalW, eyeVec, A, D, S);
    Ambient += A;
    Diffuse += D;
    Specular += S;
    
    ComputeSpotLight(gMaterial, gSpotLightLights, pin.PosW, pin.NormalW, eyeVec, A, D, S);
    Ambient += A;
    Diffuse += D;
    Specular += S;
	
    float4 litColor = Ambient + Diffuse;
    
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (gUseTexure)
    {
        texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    }
    
    texColor = pow(texColor, 2.0f);
    texColor = litColor * texColor + Specular;
    texColor.a = gMaterial.Diffuse.a * texColor.a;
   
    float4 ret = saturate(texColor);

    ret = pow(ret, 1.0f / 2.0f);
    
    PS_OUT pout;
    
    pout.Color = ret;
    pout.Depth = float4(pin.PosH.zzz, 1.0f);
    pout.Normal = float4(pin.NormalW.xyz, 1.0f);
    pout.Pos = (pin.PosW.xyz, 1.0f);
    
    return pout;
}

//�븻�� �ؽ��� �Ѵ� ������
technique11 DEFAULT
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, true)));
    }
}

//�ؽ��ĸ� ������
technique11 TextureOnly
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
    }
}

//�븻�ʸ� ������
technique11 NormalOnly
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(false, true)));
    }
}

//�Ѵ� ������
technique11 None
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(false, false)));
    }
}