#ifndef _TEST
#define _TEST

#include "register.fx"
// ========================================================
// DeadCellShader
// mesh                 : Rect
// Rasterizer           : CULL_NONE 
// BlendState           : DEFAULT
// DepthStencilState    : LESS
// Domain               : MASK

// Material Scalar Parameter
#define ColorTex    g_tex_0
#define NormalTex   g_tex_1
// ========================================================
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_DeadCell(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DeadCell(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    float4 vNormal = (float4) 0.f;
    
    vOutColor = ColorTex.Sample(g_sam_1, _in.vUV);
    
    vNormal = NormalTex.Sample(g_sam_1, _in.vUV);    
    
    // NormalMap ���� ������ ���Ⱚ�� -1 ~ 1 �� ����
    vNormal = vNormal * 2.f - 1.f;
    
    // y ��� z �� ����
    vNormal.xyz = float3(vNormal.x, vNormal.z, vNormal.y);
    
    // �븻 ���⺤�͸� ���� �������� �ű�
    float3 vWorldNormal = normalize(mul(float4(vNormal.xyz, 0.f), g_matWorld)).xyz;
            
    if (0.f == vOutColor.a)
        discard;  
    
    tLightColor color = (tLightColor) 0.f;
    
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        if (0 != g_Light2DBuffer[i].iLightType)
            continue;

        // ǥ���� ������ ���� �޴� ����(����)
        float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, vWorldNormal));
        
        // ���� �� = ���� ���� * ���� ����
        color.vDiff += g_Light2DBuffer[i].color.vDiff * fDiffusePow;
        color.vAmb += g_Light2DBuffer[i].color.vAmb;
    }       
    
    // ���� �� = ���� ��ü�� �� * ������ ��
    return vOutColor * (color.vDiff + color.vAmb);

}






#endif