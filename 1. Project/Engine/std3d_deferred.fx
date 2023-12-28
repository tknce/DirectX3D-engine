#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED

#include "register.fx"
#include "struct.fx"
#include "func.fx"


struct VS_IN
{
    float3 vPos : POSITION;
    float3 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VTX_IN_INST
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;

    // Per Instance Data
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint iRowIndex : ROWINDEX;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
    
    //float fog : FOG;
};

VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    float3 vPos = {0,0,0};
    // 소켓 적용중
    if (g_int_2 == 1)
    {   
        
        matrix matbone;
        // vPos = _in.vPos;
        for (int i = 0; i < 4; ++i)
        {
            if (g_vec4_2[i] == 0)
                continue;
            
            matbone = GetBoneOriginMat((int) g_vec4_3[i], 0);
            
            vPos += (mul(float4(_in.vPos, 1.f), matbone) * g_vec4_2[i]).xyz;

        }
        output.vPosition = mul(float4(vPos, 1.f), g_matWVP);               
        // output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        output.vUV = _in.vUV;
    
        output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
        output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
        output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
        output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);

    }
    else
    {
        output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        output.vUV = _in.vUV;
    
        output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
        output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
        output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
        output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);
    }
    
    
    

    
    //float2 Fog;
    //float FogStrat = 3000.f;
    //float FogEnd = 1000.f;
    //Fog.x = FogStrat / (FogStrat - FogEnd);
    //Fog.y = -1 / (FogStrat - FogEnd);
    //
    //output.fog = Fog.x + output.vViewPos.z * Fog.y;

    
    return output;
}
VS_OUT VS_Std3D_Deferred_Inst(VTX_IN_INST _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, _in.iRowIndex);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV = _in.vUV;

    output.vViewPos = mul(float4(_in.vPos, 1.f), _in.matWV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), _in.matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), _in.matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), _in.matWV).xyz);

    return output;
}


struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vData : SV_Target3;
};

PS_OUT PS_Std3D_Deferred(VS_OUT _in) : SV_Target
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float4 vObjColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_btex_0)
    {
        vObjColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }


    float3 vNormal = _in.vViewNormal;
    
    // 노말맵이 있으면
    if (g_btex_1)
    {
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        vNormal = (vNormal * 2.f) - 1.f;
       
        float3x3 matTBN =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal,
        };
        
        vNormal = normalize(mul(vNormal, matTBN));
    }
    
    
    // 소벨 검출 구현중
    float tu = 1.f / g_vRenderResolution.x;

    
    float2 t = _in.vUV;
    float2 uv = 0;
    float4 Out = 0;
    float width = 0;
    float heiht = 0;
    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            uv = t + float2(tu * i, tu * j);
            Out = g_tex_0.Sample(g_sam_0, uv);
            vector widthvalue = sobel_x[i+1][j+1];
            vector heihtvalue = sobel_y[i+1][j+1];
            width += Out * widthvalue;
            heiht += Out * heihtvalue;

        }      
    }

    
    output.vData.r = width;
    output.vData.g = heiht;
    
    
    output.vColor = vObjColor * g_vDiff;
    output.vNormal = float4(vNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    
    float4 vSpecCoeff = float4(g_float_0, g_float_0, g_float_0, 1.f);
       
    // Spec 맵이 있으면
    if (g_btex_2)
    {
        vSpecCoeff *= g_tex_2.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vSpecCoeff *= g_vSpec;
    }
    
    output.vData.x = encode(vSpecCoeff);
    //output.vData.w = _in.fog;
    //output.vData.w = 1.f;
    float2 vUV = _in.vUV;
    
    // 왼쪽 오른쪽만 일단 검출중
    if (vUV.x < 0.001 || 0.999 < vUV.x ||
        vUV.y < 0.001 || 0.999 < vUV.y)
    {  
        output.vData.y = 1.f;
    }
    
    
    
    return output;
}






#endif