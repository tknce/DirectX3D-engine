#ifndef _TILEMAP
#define _TILEMAP

#include "register.fx"

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

// ==================
// TileMap Shader
#define AtlasTex    g_tex_0
#define LeftTop     g_vec2_0
#define Slice       g_vec2_1
#define TileCount   g_vec2_2
StructuredBuffer<tTile> TileBuffer : register(t17);
// ==================


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

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * TileCount; // UV값을 곱해서 확장
    

    
    return output;
};

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    int bufferIdx = g_vec2_2 * floor(_in.vUV.y) + _in.vUV.x;
    
    // 1이 넘은 UV의 정수를 내림
    float2 vUV = TileBuffer[bufferIdx].vLeftTop + (TileBuffer[bufferIdx].vSlice * frac(_in.vUV));
    
    
    float4 vOutColor = AtlasTex.Sample(g_sam_1, vUV);
    if (vOutColor.a <= 0.f)
        discard;
    
    if (g_int_0 == 1)
    {
        float4 Skill = float4(0.3, 0.3, 0.3, 1);
        vOutColor *= Skill;

    }
    
    float a = TileCount.x / 5;
    TileCount.y / 3;
    a = 5.f;
    vOutColor *= float4(a, a, a, 1);
    
    return vOutColor;
}


#endif