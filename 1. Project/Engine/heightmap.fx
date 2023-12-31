#ifndef _HEIGHTMAP
#define _HEIGHTMAP

#include "register.fx"

RWTexture2D<float> HEIGHT_MAP : register(u0); // unordered acess
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // 브러쉬 위치(좌상단 기준)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

//#define BRUSH_TEX   g_texarr_0
#define BRUSH_TEX   g_tex_0
#define SCALE       g_vec2_0   // 브러쉬 크기
#define BRUSH_IDX   g_int_2    // 브러쉬 인덱스

[numthreads(32, 32, 1)]
void CS_HeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
    //HEIGHT_MAP[_iThreadID.xy] += 10.f;
    //return;

    // 브레쉬의 크기가 넘어가고 레이캐스트가 실패하면 리턴
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }

    // 중심 위치를 구함
    // 너비 높이와 중심 uv 곱함
    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vScale = float2(WIDTH, HEIGHT) * SCALE;

    // 위에서 구한 센터와 크기가 왼쪽 위 오른쪽 아래를 넘어가면 리턴
    if (_iThreadID.x < vCenterPos.x - (vScale.x / 2) || vCenterPos.x + (vScale.x / 2) < _iThreadID.x
        || _iThreadID.y < vCenterPos.y - (vScale.y / 2) || vCenterPos.y + (vScale.y / 2) < _iThreadID.y)
    {
        return;
    }

    // brush texture 에서 샘플링 할 UV 계산
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);

    // 브러쉬로 부터 알파값 샘플링
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, vUV, 0);
    HEIGHT_MAP[_iThreadID.xy] += g_fDT * vBrushColor.a * 0.2f; // 브러쉬 알파값으로 높이 설정

    // cos 그래프로 높이 설정
    //float vDist = (distance(vCenterPos, _iThreadID.xy) / vScale) * 3.1415926535f;        
    //HEIGHT_MAP[_iThreadID.xy] += saturate(fDT * cos(vDist) * 0.2f);    
}


#endif