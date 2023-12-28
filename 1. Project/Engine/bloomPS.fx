#ifndef _BLOOMPS
#define _BLOOMPS

#include "register.fx"
#include "func.fx"

#define EDGE_THRESHOLD_MIN  0.0312
#define EDGE_THRESHOLD_MAX  0.125
#define SUBPIXEL_QUALITY 0.75
struct VS_IN
{
    float3 vPos : POSITION;
};
    
struct VS_OUT
{
    float4 vPosition : SV_Position;
};

VS_OUT VS_BLOOM(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    
    return output;
}

float4 PS_BLOOM(VS_OUT _in) : SV_Target
{
    
//    float4 vOutColor = (float4) 0.f;
//    float4 fragColor = (float4) 0.f;
//    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
//    float2 tuv = 1 / g_vRenderResolution;
    
//    vOutColor = g_RTCopyTex.SampleLevel(g_sam_0, vUV, 0);
    
//    float Center = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, vUV, 0).rgb);
    
//    float up =      RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x , vUV.y + tuv.y), 0).rgb);
//    float down =    RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x, vUV.y - tuv.y), 0).rgb);
//    float right =   RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x + tuv.x, vUV.y),0).rgb);
//    float left =    RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x - tuv.x, vUV.y ), 0).rgb);
    
//    float lumaMin = min(Center, min(min(down, up), min(left, right)));
//    float lumaMax = max(Center, max(max(down, up), max(left, right)));
    
//    float range = lumaMax - lumaMin;
    
//// 만약 luma 차이가 임계값보다 작다면(또는 우리가 정말 어두운 영역내에 있다면), 
//// 우리는 가장자리에 있지 않는 것이므로, AA를 수행하지 않는다.
//    if (range < max(EDGE_THRESHOLD_MIN,
//    lumaMax * EDGE_THRESHOLD_MAX))
//    {
//        fragColor = vOutColor;
//        return fragColor;
//    }

    
//    // 남은 4개의 코너 lumas 를 얻음
//    float DownLeft =    RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x - tuv.x, vUV.y - tuv.y), 0).rgb);
//    float UpRight = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x + tuv.x, vUV.y + tuv.y), 0).rgb);
//    float UpLeft = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x - tuv.x, vUV.y + tuv.y), 0).rgb);
//    float DownRight = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x + tuv.x, vUV.y - tuv.y), 0).rgb);

//    // 4 개의 가장자리의 lumas를 조합 (같은 값으로 추후 계산을 위해 중간 변수로 사용)
//    float lumaDownUp = down + up;
//    float lumaLeftRight = left + right;

//    // 코너도 동일
//    float lumaLeftCorners = DownLeft + UpLeft;
//    float lumaDownCorners = DownLeft + DownRight;
//    float lumaRightCorners = DownRight + UpRight;
//    float lumaUpCorners = UpRight + UpLeft;

//    // 수평과 수직 축을 따르는 변화도의 추정치 계산
//    float edgeHorizontal = abs(-2.0 * left + lumaLeftCorners) + abs(-2.0 * Center + lumaDownUp) * 2.0 + abs(-2.0 * right + lumaRightCorners);
//    float edgeVertical = abs(-2.0 * up + lumaUpCorners) + abs(-2.0 * Center + lumaLeftRight) * 2.0 + abs(-2.0 * down + lumaDownCorners);

//    // 로컬 가장자리가 수평인가? 수직인가?
//    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
//    // 로컬 가장자리의 반대방향으로 2개의 이웃 텍셀을 선택함
//    float luma1 = isHorizontal ? down : left;
//    float luma2 = isHorizontal ? up : right;

//    // 이 방향으로 변화도를 계산
//    float gradient1 = luma1 - Center;
//    float gradient2 = luma2 - Center;

//    // 어느 방향이 변화가 더 가파른가?
//    bool is1Steepest = abs(gradient1) >= abs(gradient2);

//    // 해당 방향의 변화도, 정규화
//    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
//    // 가장자리의 방향에 따라서 스텝 크기(한 픽셀)를 선택
//    float stepLength = isHorizontal ? g_vRenderResolution.y : g_vRenderResolution.x;

//    // 올바른 방향의 평균 luma
//    float lumaLocalAverage = 0.0;

//    if (is1Steepest)
//    {
//    // 방향을 바꿈
//        stepLength = -stepLength;
//        lumaLocalAverage = 0.5 * (luma1 + Center);
//    }
//    else
//    {
//        lumaLocalAverage = 0.5 * (luma2 + Center);
//    }

//    // UV 를 올바른 방향으로 반픽셀 이동시킴
//    float2 currentUv = vUV;
//    if (isHorizontal)
//    {
//        currentUv.y += stepLength * 0.5;
//    }
//    else
//    {
//        currentUv.x += stepLength * 0.5;
//    }
    
//    // 오른쪽 방향으로 offset (각 반복 스탭에 대해) 을 계산.
//    float2 offset = isHorizontal ? float2(g_vRenderResolution.x, 0.0) : float2(0.0, g_vRenderResolution.y);

//// 가장자리의 각 측면을 직교하여 탐색하기 위해서 UV를 계산. QUALITY 는 스탭을 더 빠르게 함.
//    float2 uv1 = currentUv - offset;
//    float2 uv2 = currentUv + offset;

//// 탐색하는 세그먼트의 양쪽 끝에서 lumas를 읽고, delta 를 계산하고 로컬 평균 luma에 기록
//    float lumaEnd1 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv1,0).rgb);
//    float lumaEnd2 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv2,0).rgb);
//    lumaEnd1 -= lumaLocalAverage;
//    lumaEnd2 -= lumaLocalAverage;

//// 현재 끝점에서 luma delta가 로컬 변화도 보다 크면, 우리는 가장자리의 측면에 도달한 것임
//    bool reached1 = abs(lumaEnd1) >= gradientScaled;
//    bool reached2 = abs(lumaEnd2) >= gradientScaled;
//    bool reachedBoth = reached1 && reached2;

//// 측면에 도달하지 못했다면, 우리는 계속해서 이 방향으로 탐색함.
//    if (!reached1)
//    {
//        uv1 -= offset;
//    }
//    if (!reached2)
//    {
//        uv2 += offset;
//    }
    
//    // 만약 양 방향 모두 측면에 도달하지 않았다면, 계속해서 탐색
//    if (!reachedBoth)
//    {
//        for (int i = 2; i < 4; i++)
//        {
//    // 필요하다면, 첫번재 방향의 luma를 읽음, delta 계산
//            if (!reached1)
//            {
//                lumaEnd1 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv1,0).rgb);
//                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
//            }
//    // 필요하다면, 반대 방향의 luma를 읽음, delta 계산
//            if (!reached2)
//            {
//                lumaEnd2 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv2, 0).rgb);
//                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
//            }
//    // 만약 현재 끝점의 luma delta가 로컬 변화도 보다 크다면, 우리는 가장자리 측면에 도달한 것임
//            reached1 = abs(lumaEnd1) >= gradientScaled;
//            reached2 = abs(lumaEnd2) >= gradientScaled;
//            reachedBoth = reached1 && reached2;

//    // 만약 측면에 도달하지 않았다면, 우리는 이 방향으로 계속 탐색, 
//    // 가변 품질로 진행 (역주 : 스탭을 반복에 따라 조정하겠다는 의미)
//            if (!reached1)
//            {
//                uv1 -= offset * QUALITY(i);
//            }
//            if (!reached2)
//            {
//                uv2 += offset * QUALITY(i);
//            }

//    // 두 측면에 도착했다면, 탐색을 중단
//            if (reachedBoth)
//            {
//                break;
//            }
//        }
//    }
//    // 양 끝 가장자리까지의 거리를 계산
//    float distance1 = isHorizontal ? (vUV.x - uv1.x) : (vUV.y - uv1.y);
//    float distance2 = isHorizontal ? (uv2.x - vUV.x) : (uv2.y - vUV.y);

//    // 어떤 방향의 가장자리의 끝이 더 가깝나?
//    bool isDirection1 = distance1 < distance2;
//    float distanceFinal = min(distance1, distance2);

//    // 가장자리의 길이
//    float edgeThickness = (distance1 + distance2);

//    // UV offset: 가장자리의 측면까지 가장 가까운 방향으로 읽음
//    float pixelOffset = -distanceFinal / edgeThickness + 0.5;
    
//    // 중앙 luma가 로별 평균보다 더 작나?
//    bool isLumaCenterSmaller = Center < lumaLocalAverage;

//    // 만약 중앙 luma가 그것의 이웃보다 더 작다면, 양 끝의 delta luma가 양수여야 합니다. (같은 변형)
//    // (가장자리의 측면과 더 가까운 방향으로)
//    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

//    // 만약 luma 차이가 잘못된 경우, offset 을 적용하지 않음
//    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
//    // Sub-pixel shifting
//    // 3x3 이웃에 대한 luma의 전체 가중 평균
//    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

//    // 3x3 이웃의 루마 범위에 대한, 전역 평균과 중앙 luma 간의 delta의 비율
//    float subPixelOffset1 = clamp(abs(lumaAverage - Center) / range, 0.0, 1.0);
//    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

//    // 이 delta에 기반한 sub-pixel의 offset 계산
//    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

//    // 두 offset 중 더 큰것을 고름
//    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
//    // 최종 UV 좌표 계산
//    float2 finalUv = vUV;
//    if (isHorizontal)
//    {
//        finalUv.y += finalOffset * stepLength;
//    }
//    else
//    {
//        finalUv.x += finalOffset * stepLength;
//    }

//// 새로운 UV 좌표에서 컬러를 읽고 사용함
//    float4 finalColor = g_RTCopyTex.SampleLevel(g_sam_0, finalUv,0);
//    fragColor = finalColor;
    
//    return fragColor;

    //RGBtoluma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x, vUV.y + tuv.y)).rgb)
    
    // FXAA 이다 Bloom이 아님
    float4 vOutColor = (float4) 0.f;
    float4 fragColor = (float4) 0.f;
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    float2 tuv = 1 / g_vRenderResolution;
    
    float4 colorCenter = g_RTCopyTex.Sample(g_sam_0, vUV);

// Luma at the current fragment
    float lumaCenter = rgb2luma(colorCenter.rgb);

// Luma at the four direct neighbours of the current fragment.
    float lumaDown = rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x, vUV.y - tuv.y)).rgb);
    float lumaUp = rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x, vUV.y + tuv.y)).rgb);
    float lumaLeft = rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x - tuv.x, vUV.y)).rgb);
    float lumaRight = rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y)).rgb);

// Find the maximum and minimum luma around the current fragment.
    float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));
    
// delta 계산
    float lumaRange = lumaMax - lumaMin;

// 만약 luma 차이가 임계값보다 작다면(또는 우리가 정말 어두운 영역내에 있다면), 
// 우리는 가장자리에 있지 않는 것이므로, AA를 수행하지 않는다.
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
    {
        fragColor = colorCenter;
        return fragColor;
    }
    
    // 남은 4개의 코너 lumas 를 얻음
    float lumaDownLeft =    rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x - tuv.x, vUV.y - tuv.y)).rgb);
    float lumaUpRight =     rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y + tuv.y)).rgb);
    float lumaUpLeft =      rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x - tuv.x, vUV.y + tuv.y)).rgb);
    float lumaDownRight =   rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y - tuv.y)).rgb);

    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    // 코너도 동일
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    // 수평과 수직 축을 따르는 변화도의 추정치 계산
    float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);

    // 로컬 가장자리가 수평인가? 수직인가?
    // 두개의 수평 수직 중 큰 것이 그쪽으로 향하는 방향
    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
    
    // 로컬 가장자리의 반대방향으로 2개의 이웃 텍셀을 선택함
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;

    // 이 방향으로 변화도를 계산
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    // 어느 방향이 변화가 더 가파른가?
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    // 해당 방향의 변화도, 정규화
    // 두 절대값 사이에서 최대 값을 가져오고 절대값은 항상 양수이기 때문에 최대값을 취한 후 4로 나누면 정규화하는 역활을 한다.
    // 노말라이즈.
    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
    // 가장자리의 방향에 따라서 스텝 크기(한 픽셀)를 선택
    float stepLength = isHorizontal ? tuv.y : tuv.x;

    // 올바른 방향의 평균 luma
    float lumaLocalAverage = 0.0;

    if (is1Steepest)
    {
    // 방향을 바꿈
        stepLength = -stepLength;
        lumaLocalAverage = 0.5 * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5 * (luma2 + lumaCenter);
    }

// UV 를 올바른 방향으로 반픽셀 이동시킴
    float2 currentUv = vUV;
    if (isHorizontal)
    {
        currentUv.y += stepLength * 0.5;
    }
    else
    {
        currentUv.x += stepLength * 0.5;
    }
    
    // 오른쪽 방향으로 offset (각 반복 스탭에 대해) 을 계산.
    float2 offset = isHorizontal ? float2(tuv.x, 0.0) : float2(0.0, tuv.y);

// 가장자리의 각 측면을 직교하여 탐색하기 위해서 UV를 계산. QUALITY 는 스탭을 더 빠르게 함.
    float2 uv1 = currentUv - offset;
    float2 uv2 = currentUv + offset;

// 탐색하는 세그먼트의 양쪽 끝에서 lumas를 읽고, delta 를 계산하고 로컬 평균 luma에 기록
    float lumaEnd1 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv1).rgb);
    float lumaEnd2 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

// 현재 끝점에서 luma delta가 로컬 변화도 보다 크면, 우리는 가장자리의 측면에 도달한 것임
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

// 측면에 도달하지 못했다면, 우리는 계속해서 이 방향으로 탐색함.
    if (!reached1)
    {
        uv1 -= offset;
    }
    if (!reached2)
    {
        uv2 += offset;
    }
    
    // 만약 양 방향 모두 측면에 도달하지 않았다면, 계속해서 탐색
    if (!reachedBoth)
    {
        for (int i = 2; i < 8; i++)
        {
    // 필요하다면, 첫번재 방향의 luma를 읽음, delta 계산
            if (!reached1)
            {
                lumaEnd1 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
    // 필요하다면, 반대 방향의 luma를 읽음, delta 계산
            if (!reached2)
            {
                lumaEnd2 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
    // 만약 현재 끝점의 luma delta가 로컬 변화도 보다 크다면, 우리는 가장자리 측면에 도달한 것임
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

    // 만약 측면에 도달하지 않았다면, 우리는 이 방향으로 계속 탐색, 
    // 가변 품질로 진행 (역주 : 스탭을 반복에 따라 조정하겠다는 의미)
            if (!reached1)
            {
                uv1 -= offset * QUALITY(i);
            }
            if (!reached2)
            {
                uv2 += offset * QUALITY(i);
            }

    // 두 측면에 도착했다면, 탐색을 중단
            if (reachedBoth)
            {
                break;
            }
        }
    }
    
    // 양 끝 가장자리까지의 거리를 계산
    float distance1 = isHorizontal ? (vUV.x - uv1.x) : (vUV.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - vUV.x) : (uv2.y - vUV.y);

    // 어떤 방향의 가장자리의 끝이 더 가깝나?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    // 가장자리의 길이
    float edgeThickness = (distance1 + distance2);

    // UV offset: 가장자리의 측면까지 가장 가까운 방향으로 읽음
    float pixelOffset = -distanceFinal / edgeThickness + 0.5;
    
    // 중앙 luma가 로별 평균보다 더 작나?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

// 만약 중앙 luma가 그것의 이웃보다 더 작다면, 양 끝의 delta luma가 양수여야 합니다. (같은 변형)
// (가장자리의 측면과 더 가까운 방향으로)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

// 만약 luma 차이가 잘못된 경우, offset 을 적용하지 않음
    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
    // Sub-pixel shifting
// 3x3 이웃에 대한 luma의 전체 가중 평균
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

// 3x3 이웃의 루마 범위에 대한, 전역 평균과 중앙 luma 간의 delta의 비율
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0, 1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

// 이 delta에 기반한 sub-pixel의 offset 계산
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

// 두 offset 중 더 큰것을 고름
    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    // 최종 UV 좌표 계산
    float2 finalUv = vUV;
    if (isHorizontal)
    {
        finalUv.y += finalOffset * stepLength;
    }
    else
    {
        finalUv.x += finalOffset * stepLength;
    }

// 새로운 UV 좌표에서 컬러를 읽고 사용함
    float4 finalColor = g_RTCopyTex.Sample(g_sam_0, finalUv);
    fragColor = finalColor;
    
    return fragColor;
}




#endif