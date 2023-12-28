#ifndef _MERGE
#define _MERGE

#include "register.fx"
#include "func.fx"

#define MIDDLEGREY g_float_0
#define LUMWHITESQR g_float_1
#define LUMINANCE g_float_2
#define LUM_FACTOR  float3(0.2126, 0.7152, 0.0722)


// ===================
// Merge
// MRT  : MRT_TYPE::SWAPCHAIN
// mesh : Rect Mesh

// Parameter
// g_tex_0 : ColorTargetTex
// g_tex_1 : PositionTargetTex
// g_tex_2 : DiffuseTargetTex
// g_tex_3 : SpecularTargetTex
// g_tex_4 : DataTargetTex
// ===================

//RWStructuredBuffer<float> AverageLumFinal : register(u1);

float3 ToneMapping(float3 vHDRColor)
{

    // 첫번쨰가 시간 두번째가 이전 휘도
    g_vec4_0.x;
    min(g_vec4_0.x / (g_vec4_0.y / LUMINANCE), 1);
    
    float AverageLum = lerp(g_vec4_0.y, LUMINANCE, g_vec4_0.x);
    
    
    
     // 현재 픽셀에 대한 휘도 스케일 계산
    float fLScale = dot(vHDRColor, LUM_FACTOR);
    fLScale *= MIDDLEGREY / AverageLum;
    fLScale = (fLScale + fLScale * fLScale / LUMWHITESQR) / (1.f + fLScale);

    // 휘도 스케일을 픽셀 색상에 적용
    return vHDRColor * fLScale;
    
    //mad()

}


struct VS_IN
{
    float3 vPos : POSITION;
};
    
struct VS_OUT
{
    float4 vPosition : SV_Position;
    
};

//VS_OUT VS_Merge(VS_IN _in)
//{
//    VS_OUT output = (VS_OUT) 0.f;
//    
//    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
//    
//    return output;
//}
//
//float4 PS_Merge(VS_OUT _in) : SV_Target
//{
//    float4 vOutColor = (float4) 0.f;
//    
//    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
//    
//    float4 vViewPos = g_tex_1.Sample(g_sam_0, vUV);
//    
//    // 알파값이 1이면 물체가 있다고 판단
//    if (vViewPos.a == 0.f)
//        discard;
//    
//        
//    vOutColor = g_tex_0.Sample(g_sam_0, vUV);
//    
//    // 연산한 빛 안으로 넣어줌
//    float4 vDiffuse = g_tex_2.Sample(g_sam_0, vUV);
//    float4 vSpecular = g_tex_3.Sample(g_sam_0, vUV);
//    
//    vOutColor.rgb = (vOutColor.rgb * vDiffuse.rgb) + vSpecular.rgb;
//    
//    
//    return vOutColor;
//    
//
//}
VS_OUT VS_Merge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    
    
    
    return output;
}

float4 PS_Merge(VS_OUT _in) : SV_Target
{
    //vOutColor = Bloom.Sample(g_sam_0, vUV);
    float4 vOutColor = (float4) 0.f;
    
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    float4 vViewPos = g_tex_1.Sample(g_sam_0, vUV);
    if (1.f != vViewPos.a)
        discard;
    

    
    
    // Bloom 
    // g_int_1자리에 rgb값을 넣고 그 이상이면 blur처리되게 하기
    if (g_int_1)
    {
        float2 tuv = 1.f / g_vRenderResolution;
        
        
        
        //vOutColor += g_tex_0.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y));
        //vOutColor += g_tex_0.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y + tuv.y));
        //vOutColor += g_tex_0.Sample(g_sam_0, float2(vUV.x , vUV.y + tuv.y));
        vOutColor += g_tex_0.Sample(g_sam_0, vUV);
        
        //vOutColor /= 4;
    }
    else
    {
        float tu = 1.f / g_vRenderResolution.x;

        float2 t = vUV;
        float2 uv = 0;
        float4 Out = 0;
        for (int i = -6; i < 6; ++i)
        {
            uv = t + float2(tu * i, 0);
            Out += Weight[6 + 1] * g_tex_0.Sample(g_sam_0, uv);

        }
        Out /= Total;
        vOutColor += Out;
    }


    
    
    float4 vDiffuse = g_tex_2.Sample(g_sam_0, vUV);
    float4 vSpecular = g_tex_3.Sample(g_sam_0, vUV);
    
    vOutColor.rgb = (vOutColor.rgb * vDiffuse.rgb) + vSpecular.rgb;
    
    if (g_int_0 == 0)
    {
        // tonemapping
        // hdr 로 휘핑된 계수를 조정
        // float3 Cout = (vOutColor.rgb * (2.51f * vOutColor.rgb + 0.03f)) / (vOutColor.rgb * (2.43f * vOutColor.rgb + 0.59) + 0.14f);
        float3 color = vOutColor.rgb;
        color = ToneMapping(color);
        //vOutColor = float4(saturate(Cout), 1.f);
        vOutColor.rgb = color.rgb;
    }

    vOutColor.rgb;
    
    
    // Fog
    // 선형 포그
    // 1. 선형 포그 ( D3DFOG_LINEAR )
    // 공식
    // 포그 값    f = (Zfar - Z) / (Zfar - Znear)
    //  = Zfar / (Zfar - Znear) + Z * (-1 / (Zfar - Znear))
    // x = fog
    float2 Fog;
    float FogStrat = 3000.f;
    float FogEnd = 1000.f;
    Fog.x = FogStrat / (FogStrat - FogEnd);
    Fog.y = -1 / (FogStrat - FogEnd);
    
    
    
    float fog = Fog.x + vViewPos.z * Fog.y;
    vOutColor = fog * vOutColor + (1 - fog) * float4(0.01f, 0.2f, 0.3f, 1.0f);

    
    //if (vUV.x < 0.1 || 0.9 < vUV.x )
    //{       
    //    float stepx = 1.f / g_vRenderResolution.x;
    //    float stepy = 1.f / g_vRenderResolution.y;
    //    // 소벨 검출
    //    float tleft = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(-stepx, stepy)));
    //    float left = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(-stepx, 0)));
    //    float bleft = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(-stepx, -stepy)));
    //    float top = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(0, stepy)));
    //    float bottom = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(0, -stepy)));
    //    float tright = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(stepx, stepy)));
    //    float right = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(stepx, 0)));
    //    float bright = intensity(g_tex_0.Sample(g_sam_0, vUV + float2(stepx, -stepy)));
    //
	//// Sobel masks (see http://en.wikipedia.org/wiki/Sobel_operator)
	////        1 0 -1     -1 -2 -1
	////    X = 2 0 -2  Y = 0  0  0
	////        1 0 -1      1  2  1
	//
	//// You could also use Scharr operator:
	////        3 0 -3        3 10   3
	////    X = 10 0 -10  Y = 0  0   0
	////        3 0 -3        -3 -10 -3
    //
    //    float x = tleft + 2.0 * left + bleft - tright - 2.0 * right - bright;
    //    float y = -tleft - 2.0 * top - tright + bleft + 2.0 * bottom + bright;
    //    float color = sqrt((x * x) + (y * y));
    ////vOutColor = float4(color, color, color, 1.f);
    //
    //
    //        vOutColor = float4(color, color, color, 1.f);
    //    
    //}
    if (g_tex_4.Sample(g_sam_0, vUV).b == 1)
    {
        //vOutColor = float4(1.f, 1.f, 1.f, 1.f);

    }
    
    
        return vOutColor;

}
#endif