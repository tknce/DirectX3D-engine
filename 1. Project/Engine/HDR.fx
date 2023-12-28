#ifndef _COMPUTE
#define _COMPUTE

#include "register.fx"

#define LUM_FACTOR  float4(0.2126, 0.7152, 0.0722, 1)




StructuredBuffer<float> AverageValues1D : register(t0);
RWStructuredBuffer<float> AverageLumFinal : register(u1);

groupshared float SharedPositions[1024];

//cbuffer Adapt_Bloom_CB : register(b3)
//{
//    float g_fAdaptation;
//    float3 vEmpty;
//}
//
//cbuffer BloomThresholdCB : register(b4)
//{
//    float g_fBloomThreshold;
//    float3 g_vEmpty;
//}

// 각 스레드에 대해 4x4 다운 스케일을 수행한다
float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
    float avgLum = 0.f;

    if (CurPixel.y < g_vRenderResolution.y)
    {
        int3 iFullResPos = int3(CurPixel * 4, 0);
        float4 vDownScaled = float4(0.f, 0.f, 0.f, 0.f);

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            [unroll]
            for (int j = 0; j < 4; ++j)
            {
                vDownScaled += HDR.Load(iFullResPos, int2(j, i));
            }
        }
        vDownScaled /= 16;
        
        // 픽셀별 휘도 값 계산
        avgLum = dot(vDownScaled, LUM_FACTOR);

        // 공유 메모리에 결과 기록
        SharedPositions[groupThreadId] = avgLum;
    }

    // 동기화 후 다음 단계로
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}

// 위에서 구한 값을 4개의 값으로 다운스케일한다
float DownScale1024to4(uint dispachThreadId, uint groupThreadId,
    float avgLum)
{
    // 다운스케일 코드를 확장
    for (uint iGroupSize = 4, iStep1 = 1, iStep2 = 2, iStep3 = 3;
        iGroupSize < 1024;
        iGroupSize *= 4, iStep1 *= 4, iStep2 *= 4, iStep3 *= 4)
    {
        if (groupThreadId % iGroupSize == 0)
        {
            float fStepAvgLum = avgLum;

            fStepAvgLum += dispachThreadId + iStep1 < (1600 * 900) ?
                SharedPositions[groupThreadId + iStep1] : avgLum;

            fStepAvgLum += dispachThreadId + iStep2 < (1600 * 900) ?
                SharedPositions[groupThreadId + iStep2] : avgLum;

            fStepAvgLum += dispachThreadId + iStep3 < (1600 * 900) ?
              SharedPositions[groupThreadId + iStep3] : avgLum;

            // 결과 값 저장
            avgLum = fStepAvgLum;
            SharedPositions[groupThreadId] = fStepAvgLum;
        }
        // 동기화 후 다음으로
        GroupMemoryBarrierWithGroupSync();
    }
    return avgLum;
}

// 4개의 값을 하나의 평균값으로 다운스케일한 후 저장한다
void DownScale4to1(uint dispatchThreadId, uint groupThreadId,
    uint groupId, float avgLum)
{
    if (groupThreadId == 0)
    {
        //  스레드 그룹에 대한 평균 휘도 값 계산
        float fFinalAvgLum = avgLum;

        fFinalAvgLum += dispatchThreadId + 256 < (1600 * 900) ?
            SharedPositions[groupThreadId + 256] : avgLum;

        fFinalAvgLum += dispatchThreadId + 512 < (1600 * 900) ?
            SharedPositions[groupThreadId + 512] : avgLum;

        fFinalAvgLum += dispatchThreadId + 768 < (1600 * 900) ?
            SharedPositions[groupThreadId + 768] : avgLum;

        fFinalAvgLum /= 1024.f;

        // 최종 값을 ID UAV에 기록 후 다음 과정으로
        AverageLumFinal[groupId] = fFinalAvgLum;
        //AverageLumFinal[0] = fFinalAvgLum;

    }
}

// 이렇게 구한 값은 셰이더 엔트리 포인트에 대입된다
[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 groupId : SV_GroupID,
    uint3 dispatchThreadId : SV_DispatchThreadID,
    uint3 groupThreadId : SV_GroupThreadID)
{
    uint2 vCurPixel = uint2(dispatchThreadId.x % g_vRenderResolution.x,
    dispatchThreadId.x / g_vRenderResolution.x);

    // 16 픽셀 그룹을 하나의 픽셀로 줄여 공유 메모리에 저장
    float favgLum = DownScale4x4(vCurPixel, groupThreadId.x);

    // 1024에서 4로 다운스케일
    favgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x,
         favgLum);

    // 4에서 1로 다운스케일
    DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x,
        favgLum);
    

}

// 첫 번째 컴퓨트 셰이더의 실행이 완료되면 동일한 상수 버퍼를 사용한 두번째 컴퓨트 셰이더를 실행한다
// 중간 값 휘도 SRV와 평균 휘도 UAV 값을 지정해 사용한다
#define MAX_GROUPS 64

// 공유 메모리 그룹에 중간 값 저장
groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID,
        uint3 groupThreadId : SV_GroupThreadID,
        uint3 dispatchThreadId : SV_DispatchThreadID)
{

    // 공유 메모리에 ID값 저장
    float favgLum = 0.f;
    float domain = 350.f;
    if (dispatchThreadId.x < domain)
    {
        favgLum = AverageLumFinal[dispatchThreadId.x];
    }

    SharedAvgFinal[dispatchThreadId.x] = favgLum;

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 64에서 16으로 다운 스케일
    if (dispatchThreadId.x % 4 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 1 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 1] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 2 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 2] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 3 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 3] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;

        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 16에서 4로 다운스케일
    if (dispatchThreadId.x % 16 == 0)
    {
        // 휘도 값 합산
        float fstepAvgLum = favgLum;

        fstepAvgLum += dispatchThreadId.x + 4 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 4] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 8 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 8] : favgLum;

        fstepAvgLum += dispatchThreadId.x + 12 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 12] : favgLum;

        // 결과 값 저장
        favgLum = fstepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = fstepAvgLum;
    }
    
    GroupMemoryBarrierWithGroupSync(); // 동기화 후 다음 과정으로

    // 4에서 1로 다운스케일
    if (dispatchThreadId.x == 0)
    {
        // 휘도 값 합산
        float fFinalLumValue = favgLum;

        fFinalLumValue += dispatchThreadId.x + 16 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 16] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 32 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 32] : favgLum;

        fFinalLumValue += dispatchThreadId.x + 48 < domain ?
            SharedAvgFinal[dispatchThreadId.x + 48] : favgLum;

        fFinalLumValue /= 64.f;
        
        AverageLumFinal[0] = max(fFinalLumValue, 0.0001);
        //AverageLumFinal[0] = 10.f;

    }
    //AverageLumFinal[dispatchThreadId.x] = SharedAvgFinal[dispatchThreadId.x];
    //AverageLumFinal[dispatchThreadId.x] = favgLum;
}

#endif