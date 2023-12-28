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
    
    // ����
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
    
    // ��Į�� �ؽ��İ� ���õǾ����� ������
    if (g_btex_1 == 0)
    {
        discard;
    }
    
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    
    // ������ ����
    float4 vViewPos = g_tex_0.Sample(g_sam_0, vUV);
    if (vViewPos.a == 0.f)
        discard;
    
    // ���� �����̽����� ť�곻�� �ִ��� �����Ѵ�.
    // ����ķ� ���ؼ� ���÷� ���� ��������.
    float4 vLocalPos = mul(mul(vViewPos, g_matViewInv), g_matWorldInv);
    if (abs(vLocalPos.x) > 0.5f || abs(vLocalPos.y) > 0.5f || abs(vLocalPos.z) > 0.5f)
        discard;
    
    // ��Į uv�� 0~1������ ���� �ö󰣴�.
    float2 vDecalUV = float2(vLocalPos.x + 0.5f, 1.f - (vLocalPos.y + 0.5f));
    // �׷��� ���� uv�� ��Į �ؽ��ķ� ���ø�
    float4 vDecaColor = g_tex_1.Sample(g_sam_0, vDecalUV);
    output.vOutColor = vDecaColor;
    
    return output;
}
    


#endif