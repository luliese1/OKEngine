#include "ShaderHelper.hlsl"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerFrame
cbuffer cbPerFrame : register(b0)
{
    Camera g_Camera : packoffset(c0);
    Light g_Light : packoffset(c16);
};
