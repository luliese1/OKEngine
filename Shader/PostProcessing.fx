

//static float3 BlurWeight[12] =
//{
//    -3, 0, 0.111, -2, 0, 0.250, -1, 0, 1.000,
//	+3, 0, 0.111, +2, 0, 0.250, +1, 0, 1.000,
	
//	 0, -3, 0.111, 0, -2, 0.250, 0, -1, 1.000,
//	 0, +3, 0.111, 0, +2, 0.250, 0, +1, 1.000
//};

//float4 PS_Blur()
//{
//    float4 AverageColor = 0;
//    float4 Pixel = 0;

//    for (int i = 0; i < 12; ++i)
//    {
//		// 주변 샘플을 가져온다. 12개의 값을 평균내고 싶다.
//        Pixel = gDiffuseMap.Sample(samAnisotropic, pin.Tex + (g_Filter[i].xy) * g_TexelSize) * 1.0f / 12.0f;
		
//		// 0에서 1의 범위로 바꿔 준다.
//		// Clamps the specified value within the range of 0 to 1.
//        Pixel = saturate(Pixel);
		
//		// 12개를 누적평균을 내고 싶다.
//        AverageColor += Pixel;
//    }

//    return AverageColor;
//}