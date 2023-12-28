#ifndef _DECAL
#define _DECAL

#include "register.fx"

// =================
// DecalShader
// MRT  : MRT_TYPE::DECAL
// mesh : CubeMesh
// Blend        : AlphaBlend
// DepthStencil : No_Test_NoWrite

// g_tex_0 : Position Target Tex
// g_tex_1 : Decal Texture
// =================
struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_Decal(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 월드
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}

struct PS_OUT
{
    float4 vOutColor : SV_Target;
};

PS_OUT PS_Decal(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // 데칼용 텍스쳐가 세팅되어있지 않으면
    if (g_btex_1 == 0)
    {
        discard;
    }
    
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    // 포지션 정보
    float4 vViewPos = g_tex_0.Sample(g_sam_0, vUV);
    if (vViewPos.a == 0.f)
        discard;
    
    // 로컬 스페이스에서 큐브내에 있는지 판정한다.
    // 역행렬로 곱해서 로컬로 끌고 내러간다.
    float4 vLocalPos = mul(mul(vViewPos, g_matViewInv), g_matWorldInv);
    if (abs(vLocalPos.x) > 0.5f || abs(vLocalPos.y) > 0.5f || abs(vLocalPos.z) > 0.5f)
        discard;
    
    // 데칼 uv를 0~1범위로 끌고 올라간다.
    float2 vDecalUV = float2(vLocalPos.x + 0.5f, 1.f - (vLocalPos.y + 0.5f));
    // 그렇게 구한 uv를 데칼 텍스쳐로 샘플링
    float4 vDecaColor = g_tex_1.Sample(g_sam_0, vDecalUV);
    output.vOutColor = vDecaColor;
    
    return output;
}
    


#endif