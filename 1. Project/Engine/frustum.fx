#ifndef _FRUSTUM
#define _FRUSTUM

#include "register.fx"



float4 VS_Frustum(float3 _vPos : POSITION) : SV_Position
{
    // float4 vPosition = mul(float4(_vPos, 1.f), g_mat_0);
    float4 vPosition = float4(_vPos, 1);
    // vPosition.xy *= 10;
    
    vPosition = mul(vPosition, g_matWVP);
    
    return vPosition;
}





float4 PS_Frustum(float4 _vPosition : SV_Position) : SV_Target
{
    float4 vOutColor = float4(0.2f,0.8f,0.2f,1.f);
    
    return vOutColor;
}








#endif