#ifndef _STD3D
#define _STD3D

#include "register.fx"
#include "func.fx"

// 3D Directional Light
//static float3 g_Light3DPos = float3(0.f, 0.f, 0.f);
//static float3 g_Light3DDir = float3(1.f, -1.f, 1.f);
//static float3 g_LightColor = float3(1.f, 1.f, 1.f);
//static float3 g_LightAmb = float3(0.15f, 0.15f, 0.15f);

// ===========================
// Std3DShader
// g_int_0 : LightShadingType
// g_tex_0 : Output Texture
// g_tex_1 : Normal Texture
// ===========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
    
    
};



VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
       
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);
    
    //// 월드 노말 구해주기
    //float3 vWorldNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld).xyz);
    //// 월드 위치 포즈 구해주기
    //float3 vWorldPos = normalize(mul(float4(_in.vPos, 1.f), g_matWorld).xyz);
    //
    ////float3 LightDir = normalize(g_Light3DDir);        
    //if (g_int_0)
    //{
    //    // Gouraud Shading -> 버텍스 단계에서 노말구해준 다음 보간한다. 이제 안 쓰임
    //    // 보간하기 때문에 삼각형 메쉬 윗부분은 밝은쪽부터 보간 되기 떄문에 밝고 아래쪽은 어두운쪽부터 보간되기때문에 어두워서 어색하다
    //    float3 LightDir = normalize(g_Light3DDir);
    //    output.fPow = saturate(dot(-LightDir, vWorldNormal));
    //}
    //else
    //{
    //    // Phong Shading -> 픽셀 쉐이더 단계에서 노말을 지정한다.
    //    output.vNormal = vWorldNormal;
    //}
    return output;
}


float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        
    // 물체 색상 텍스쳐
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
        
    
    float3 vNormal = _in.vViewNormal;
    
    // 노말맵이 있으면
    if (g_btex_1)
    {
        // 물체의 표면에 적용될 탄젠트 스페이스 기준 방향벡터를 가져온다.
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        vNormal = (vNormal * 2.f) - 1.f;
                
        // (1, 0, 0) -> _in.vViewTangent;        
        // (0, 1, 0) -> _in.vViewBinormal;
        // (0, 0, 1) -> _in.vViewNormal;
        
        float3x3 matTBN =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal,
        };
        
        vNormal = normalize(mul(vNormal, matTBN));
    }
    // 광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환 - 모든 광원은 월드로 가서 변환한다.
    // 카메라뷰스페이스 특징은 카메라가 0,0,0에 있고 z축을 바라보고 있기 때문이다.
    //float3 ViewLightDir = normalize(mul(float4(g_Light3DDir, 0.f), g_matView));
    // 광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환
    //tLightInfo info = g_Light3DBuffer[0];
    
    
    //float3 ViewLightDir = normalize(mul(float4(info.vWorldDir.xyz, 0.f), g_matView));
    
    
     
    // View 스페이스 상에서 표면의 빛의 세기를 구함
    //float fPow = saturate(dot(-ViewLightDir, _in.vViewNormal));
    //float fSpecPow = 0.f;
    
    // 반사광 세기를 구함
    // 표면의 빛의 반사벡터
    //float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _in.vViewNormal) * _in.vViewNormal);
    
    // 시점에서 표면을 향하는 벡터
    //float3 vEye = normalize(_in.vViewPos);
        
    // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
    //fSpecPow = saturate(dot(-vEye, vViewReflect));
    
    // 블린 퐁 쉐이더 구현중
    //float3 vViewReflect = normalize(ViewLightDir.xyz + _in.vViewPos);
    //fSpecPow = saturate(dot(_in.vViewNormal, vViewReflect));
    
    
    //fSpecPow = pow(fSpecPow, 15);
        
    
    
        // 표면(픽셀) 이 받는 빛의 총량을 저장할 변수
    tLightColor lightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight3DCount; ++i)
    {
        CalcLight3D(_in.vViewPos, vNormal, i, lightColor);
    }
    
    // 물체의 색상에 광원을 적용
    vOutColor.rgb = (vOutColor.rgb * lightColor.vDiff.rgb)
                    + lightColor.vSpec.rgb * g_float_0
                    + (vOutColor.rgb * lightColor.vAmb.rgb);
    
    

    
    //if (g_int_0)
    //{
    //    // Gouraud Shading
    //    vOutColor.xyz = (vOutColor.xyz * g_LightColor * _in.fPow) + (vOutColor.xyz * g_LightAmb);
    //}
    //else
    //{
    //    // Phong Shading
    //    float3 LightDir = normalize(g_Light3DDir);
    //    float fPow = saturate(dot(-LightDir, _in.vNormal));
    //    vOutColor.xyz = (vOutColor.xyz * g_LightColor * fPow) + (vOutColor.xyz * g_LightAmb);
    //}

    // Blur
    
    

    
    
    
    //vOutColor = Bloom.Sample(g_sam_0, _in.tex);
    
    
    
    
    
     return vOutColor;
}





#endif