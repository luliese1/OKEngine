

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
//		// �ֺ� ������ �����´�. 12���� ���� ��ճ��� �ʹ�.
//        Pixel = gDiffuseMap.Sample(samAnisotropic, pin.Tex + (g_Filter[i].xy) * g_TexelSize) * 1.0f / 12.0f;
		
//		// 0���� 1�� ������ �ٲ� �ش�.
//		// Clamps the specified value within the range of 0 to 1.
//        Pixel = saturate(Pixel);
		
//		// 12���� ��������� ���� �ʹ�.
//        AverageColor += Pixel;
//    }

//    return AverageColor;
//}