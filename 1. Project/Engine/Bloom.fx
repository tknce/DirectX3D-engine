#ifndef _COMPUTE_BLOOM
#define _COMPUTE_BLOOM

#include "register.fx"

#define LUM_ADAPT_FACTOR 0.19
#define AvgLum g_float_0
#define BloomThreshold g_float_1

// HDRTEX   61
// AvgLum   t1


RWTexture2D<float4> Bloom_1 : register(u2);

[numthreads(1024, 1, 1)]
void BloomReveal(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadID.x % g_vRenderResolution.x,
    dispatchThreadID.x / g_vRenderResolution.x);

    if (vCurPixel.y < g_vRenderResolution.y)
    {
        float4 vColor = HDR.Load(int3(vCurPixel, 0));
        float fLum = dot(vColor, LUM_ADAPT_FACTOR);
        float fAvgLum = 0.35f;
        

        float fColorScale = saturate(fLum - fAvgLum * 0.1f);

        Bloom_1[vCurPixel.xy] = vColor * fColorScale;
    }
}




#endif 