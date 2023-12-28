#ifndef _BLOOD
#define _BLOOD

#include "register.fx"
#include "func.fx"

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};
//g_int_3 = skill bool
//g_vec4_0 = skill color

StructuredBuffer<tTile> TileBuffer : register(t59);

// HLSL �� VertexShader �ۼ��ϱ�
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float2 vUV_Tile : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    float2 vUV_Tile : TEXCOORD;
};


VTX_OUT VS_Blood(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    //output.vUV_Tile = 
    
    return output;
}
float4 PS_Blood(VTX_OUT _in) : SV_Target
{
    // ���� ���� �ؽ����� Ư�� uv���� ������
    float2 vUV1 = g_vec2_3.x + g_vec2_3.y;
    
    // �װ��� ���ø���
    float4 Color = g_tex_1.Sample(g_sam_1, vUV1);
    
    
    // �ִϸ��̼�
    float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
    float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);
        
    if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
    {
        discard;
    }
        
    float4 vOutColor = g_Atals.Sample(g_sam_1, vUV);
    
    
    
    // ������ ���İ��κ��� ���Խ�Ŵ
    vOutColor.a = Color.a;
    
    if (vOutColor.a <= 0.f)
        discard;
    
    if (g_int_0 == 1)
    {
        float4 Skill = float4(0.3, 0.3, 0.3, 1);
        vOutColor *= Skill;

    }
    
    
    
    return vOutColor;
}

#endif