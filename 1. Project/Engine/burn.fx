#ifndef _STD2D
#define _STD2D

#include "register.fx"
#include "func.fx"



struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

#define AtlasTex    g_tex_1
#define LeftTop     g_vec2_0
#define Slice       g_vec2_1
#define TileCount   g_vec2_2
StructuredBuffer<tTile> TileBuffer : register(t17);


//g_int_3 = skill bool
//g_vec4_0 = skill color

// HLSL ∑Œ VertexShader ¿€º∫«œ±‚
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


VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);
        
        if (vUV.x < g_vLeftTop.x || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }
        
        vOutColor = g_Atals.Sample(g_sam_1, vUV);
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    // ≥Î¿Ã¡Ó ≈ÿΩ∫√ƒ Burn »ø∞˙
    if (g_int_3 == 1)
    {
        if (0.f < g_fAccTime)
        {
            vOutColor.a -= g_Noise.Sample(g_sam_0, _in.vUV).x * 0.5f + (g_fAccTime) * 0.5f;
        }
    }
        
    
    
    
    if (vOutColor.a <= 0.f)
        discard;
       
    
    // ±§ø¯ √≥∏Æ
    tLightColor color = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }
    
    if (g_int_0 == 1)
    {
        float4 Color = float4(0.2, 0.2, 0.2, 1);
        vOutColor *= Color;
    }
    
    if (g_int_1 == 1)
    {
        float4 Color = float4(5, 5, 0.2, 1);
        vOutColor *= Color;
    }
    
    vOutColor *= color.vDiff;
    if (g_int_3 == 1)
    {
        //vOutColor = 0;
        //g_EffectTex1
        float4 vColor = (float4) 0.f;




        float4 vFX_tex = g_tex_1.Sample(g_sam_0, _in.vUV);


        if (vColor.a == 0.f)
            clip(-1);

        if (vFX_tex.r >= vFX_tex.a)
            vColor.a = 1;
        else
            vColor.a = 0;

        if (vFX_tex.r >= vFX_tex.a - 0.05 && vFX_tex.r <= vFX_tex.a + 0.05)
            vColor = float4(1, 0, 0, 1); // ª°
        else
		;

        if (vFX_tex.r >= vFX_tex.a - 0.03 && vFX_tex.r <= vFX_tex.a + 0.03)
            vColor = float4(1, 1, 0, 1); // ≥Î
        else
		;

        if (vFX_tex.r >= vFX_tex.a - 0.025 && vFX_tex.r <= vFX_tex.a + 0.025)
            vColor = float4(1, 1, 1, 1); // »Ú
        else
		;

        vOutColor = vColor;
        //return vOutColor;
        
        if (g_int_3 == 1)
        {
        //vOutColor = 0;
        //g_EffectTex1
            float4 vColor = (float4) 0.f;


            float4 vFX_tex = 0;
            if (0.f < g_fAccTime)
            {
                vFX_tex = g_tex_1.Sample(g_sam_0, _in.vUV).x * 0.5f + (g_fAccTime) * 0.5f;
            }
        //vOutColor.a -= g_Noise.Sample(g_sam_0, _in.vUV).x * 0.5f + (g_fAccTime) * 0.5f;

            if (vColor.a == 0.f)
                clip(-1);

            if (vFX_tex.r >= vFX_tex.a)
                vColor.a = 1;
            else
                vColor.a = 0;

            if (vFX_tex.r >= vFX_tex.a - 0.05 && vFX_tex.r <= vFX_tex.a + 0.05)
                vColor = float4(1, 0, 0, 1); // ª°
            else
		;

            if (vFX_tex.r >= vFX_tex.a - 0.03 && vFX_tex.r <= vFX_tex.a + 0.03)
                vColor = float4(1, 1, 0, 1); // ≥Î
            else
		;

            if (vFX_tex.r >= vFX_tex.a - 0.025 && vFX_tex.r <= vFX_tex.a + 0.025)
                vColor = float4(1, 1, 1, 1); // »Ú
            else
		;

            vOutColor = vColor;
        //return vOutColor;
        }
    }
    //float4 Color = float4(0.4, 0.7, 0.85, 1);
    //vOutColor += Color;
        return vOutColor;
    
    
    
    

}



#endif