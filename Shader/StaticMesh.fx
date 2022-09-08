#include "LightHelper.fx"

cbuffer cbPerFrame
{
	//빛의 개수...
	//동적으로 여러개의 빛을 사용하려면... 디퍼드렌더링?
    DirectionalLight gDirLights;
    PointLight gPointLights;
    SpotLight gSpotLightLights;
	
	//눈의 월드좌표?
    float3 gEyePosW;

	//왜 필요함 이거?
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
};

Texture2D gDiffuseMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 Pos : POSITION; //로컬좌표
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD0;
    float2 Tex1 : TEXCOORD1;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; //
    float3 NormalW : NORMAL; //W좌표
    float3 TangentW : TANGENT; //W좌표
    float2 Tex : TEXCOORD;
    float2 Tex1 : TEXCOORD1;
};
VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
    
    vout.PosW = mul(float4(vin.Pos, 1.0f), gWorld).xyz;
   
	//노말 벡터또한 월드 좌표로 옮겨서 계산을 해야한다.	
    vout.NormalW = mul(vin.Normal, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.Tangent, (float3x3) gWorld);
    
    vout.Tex = vin.Tex;
    vout.Tex1 = vin.Tex1;

    return vout;
}
struct PS_OUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Depth : SV_Target2;
    float4 Pos : SV_Target3;
};

PS_OUT PS(VertexOut pin,
uniform bool gUseTexure,
uniform bool gUseNormalMap) : SV_Target
{
   
    pin.NormalW = normalize(pin.NormalW);
    
    if (gUseNormalMap)
    {
        float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
        
        //노멀맵의 데이터는 0~1사이의 값 그러므로 -1~1사이의 값으로 바꿔준다.
        float3 normalT = normalMapSample * 2.0f - 1.0f;
        
        float3 N = pin.NormalW;
        float3 T = normalize(pin.TangentW - dot(pin.TangentW, N) * N);
        float3 B = cross(N, T);
        
        float3x3 tanspaceMat = float3x3(T, B, N);
          
        pin.NormalW = mul(normalT, tanspaceMat);
    }
    
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
   
    if (true)
    {
        float3 incident = -eyeVec;
        float3 reflectionVector = reflect(incident, pin.NormalW);
        float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
        
        texColor += gMaterial.Reflect * reflectionColor;
    }
    
    float4 ret = saturate(texColor);
    
    ret = pow(ret, 1.0f / 2.0f);
        
    PS_OUT pout;
    
    pout.Color = ret;
    pout.Depth = float4(pin.PosH.zzz, 1.0f);
    pout.Normal = float4(pin.NormalW.xyz, 1.0f);
    pout.Pos = (pin.PosW.xyz, 1.0f);
    
    return pout;
}

technique11 DEFAULT
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, true)));
    }
}

technique11 TextureOnly
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
    }
}

technique11 NormalOnly
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(false, true)));
    }
}

technique11 None
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(false, false)));
    }
}