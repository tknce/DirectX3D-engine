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
    
    //// ���� �븻 �����ֱ�
    //float3 vWorldNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld).xyz);
    //// ���� ��ġ ���� �����ֱ�
    //float3 vWorldPos = normalize(mul(float4(_in.vPos, 1.f), g_matWorld).xyz);
    //
    ////float3 LightDir = normalize(g_Light3DDir);        
    //if (g_int_0)
    //{
    //    // Gouraud Shading -> ���ؽ� �ܰ迡�� �븻������ ���� �����Ѵ�. ���� �� ����
    //    // �����ϱ� ������ �ﰢ�� �޽� ���κ��� �����ʺ��� ���� �Ǳ� ������ ��� �Ʒ����� ��ο��ʺ��� �����Ǳ⶧���� ��ο��� ����ϴ�
    //    float3 LightDir = normalize(g_Light3DDir);
    //    output.fPow = saturate(dot(-LightDir, vWorldNormal));
    //}
    //else
    //{
    //    // Phong Shading -> �ȼ� ���̴� �ܰ迡�� �븻�� �����Ѵ�.
    //    output.vNormal = vWorldNormal;
    //}
    return output;
}


float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        
    // ��ü ���� �ؽ���
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
        
    
    float3 vNormal = _in.vViewNormal;
    
    // �븻���� ������
    if (g_btex_1)
    {
        // ��ü�� ǥ�鿡 ����� ź��Ʈ �����̽� ���� ���⺤�͸� �����´�.
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
    // ������ ������ ���� ��ǥ�迡�� �� ��ǥ��� ��ȯ - ��� ������ ����� ���� ��ȯ�Ѵ�.
    // ī�޶�佺���̽� Ư¡�� ī�޶� 0,0,0�� �ְ� z���� �ٶ󺸰� �ֱ� �����̴�.
    //float3 ViewLightDir = normalize(mul(float4(g_Light3DDir, 0.f), g_matView));
    // ������ ������ ���� ��ǥ�迡�� �� ��ǥ��� ��ȯ
    //tLightInfo info = g_Light3DBuffer[0];
    
    
    //float3 ViewLightDir = normalize(mul(float4(info.vWorldDir.xyz, 0.f), g_matView));
    
    
     
    // View �����̽� �󿡼� ǥ���� ���� ���⸦ ����
    //float fPow = saturate(dot(-ViewLightDir, _in.vViewNormal));
    //float fSpecPow = 0.f;
    
    // �ݻ籤 ���⸦ ����
    // ǥ���� ���� �ݻ纤��
    //float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _in.vViewNormal) * _in.vViewNormal);
    
    // �������� ǥ���� ���ϴ� ����
    //float3 vEye = normalize(_in.vViewPos);
        
    // �ü� ���Ϳ� �ݻ纤�͸� �����ؼ� �ݻ籤�� ���⸦ ����
    //fSpecPow = saturate(dot(-vEye, vViewReflect));
    
    // �� �� ���̴� ������
    //float3 vViewReflect = normalize(ViewLightDir.xyz + _in.vViewPos);
    //fSpecPow = saturate(dot(_in.vViewNormal, vViewReflect));
    
    
    //fSpecPow = pow(fSpecPow, 15);
        
    
    
        // ǥ��(�ȼ�) �� �޴� ���� �ѷ��� ������ ����
    tLightColor lightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight3DCount; ++i)
    {
        CalcLight3D(_in.vViewPos, vNormal, i, lightColor);
    }
    
    // ��ü�� ���� ������ ����
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