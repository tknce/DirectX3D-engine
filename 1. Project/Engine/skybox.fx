#ifndef _SKYBOX
#define _SKYBOX

#include "register.fx"

// =========================
// SkyBoxShader
// mesh : Sphere, Cube
// 
// g_int_0      : SkyBox Type( 0 : Sphere, 1 : Cube)
// g_tex_0      : Output Texture(Sphere)
// g_texcube_0  : Output Texture(Cube)
// =========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vCubeUV : TEXCOORD1;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // �����
    float3 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    // ������ǥ�迡�� ���̷�Ʈ�� ��������� ���ؼ� ������
    float4 vProjPos = mul(float4(vViewPos, 1.f), g_matProj);
    // z�� 0~1 ���̸� 4��° ���ڷ� �Ȱ��� ��� ���ڸ� ������ ������ z�� w�� ������ ������ 1�� ���´�.
    vProjPos.z = vProjPos.w;
    
    output.vPosition = vProjPos;
    output.vUV = _in.vUV;
    output.vCubeUV = normalize(_in.vPos);
 
    return output;
}


float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
        
    // SphereType SkyBox �� ���
    if (0 == g_int_0)
    {
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    
    // CubeType SkyBox �� ���
    else if (1 == g_int_0)
    {
        // cube�� ���� ����� ���� �־ texture�� ���� �ִ�.
        if (g_btexcube_0)
        {
            vOutColor = g_texcube_0.Sample(g_sam_0, _in.vCubeUV);
        }
    }
    if (1 == g_int_1)
    {
    
        float3 Cout = (vOutColor.rgb * (2.51f * vOutColor.rgb + 0.03f)) / (vOutColor.rgb * (2.43f * vOutColor.rgb + 0.59) + 0.14f);
        
        vOutColor = float4(saturate(Cout), 1.f);

    }
       
    
    return vOutColor;
}

#endif