#ifndef _POST_PROCESS
#define _POST_PROCESS

#include "register.fx"

// ====================
// PostProcess Shader
// mesh : RectMesh
// BS_TYPE : Default
// DS_TYPE : NoTest NoWrite 
// ====================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;    
    float2 vUV : TEXCOORD;
};


//VS_OUT VS_PostProcess(VS_IN _in)
//{
//    VS_OUT output = (VS_OUT) 0.f;
    
//    output.vPosition = float4(_in.vPos.xy * 2.f, 0.5f, 1.f);
//    output.vUV = _in.vUV;
    
//    return output;
//}


//float4 PS_PostProcess(VS_OUT _in) : SV_Target
//{    
//    float4 vColor = (float4) 0.f;
        
//    vColor = g_RTCopyTex.Sample(g_sam_0, _in.vUV);    
    
//    vColor.rgb = (vColor.r + vColor.g + vColor.b) / 3.f;
    
//    return vColor;
//}

VS_OUT VS_PostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    //output.vUV.y = _in.vUV.y * g_float_3;
    
    return output;
}


float4 PS_PostProcess(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
          
    // VS_OUT ���� ������ SV_Position ���� PixelShader �� �Էµ� �� �ȼ���ǥ�� ��ȯ�ؼ� �Է�
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;    
    
    //vUV.y += cos((_in.vUV.x - g_fAccTime * 0.5f) * 10.f * 3.141592f) * 0.01f;
    //vUV.y += sin((_in.vUV.x - g_fAccTime * 0.5f) * 10.f * 3.141592f) * 0.01f;
    float2 fAdd = float2(g_Noise.Sample(g_sam_0, _in.vUV + g_fAccTime * 0.2f).x
                         , g_Noise.Sample(g_sam_0, _in.vUV + float2(0.1f, 0.f) + g_fAccTime * 0.2f).x);
    fAdd -= fAdd / g_float_2;
    //fAdd -= fAdd / 2.f;
    vUV += fAdd * 0.05f;
    
    vColor = g_RTCopyTex.Sample(g_sam_0, vUV);
    
    //vColor.r *= 1.5f;
    
    return vColor;
}







#endif

