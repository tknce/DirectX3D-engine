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
    
//// ���� luma ���̰� �Ӱ谪���� �۴ٸ�(�Ǵ� �츮�� ���� ��ο� �������� �ִٸ�), 
//// �츮�� �����ڸ��� ���� �ʴ� ���̹Ƿ�, AA�� �������� �ʴ´�.
//    if (range < max(EDGE_THRESHOLD_MIN,
//    lumaMax * EDGE_THRESHOLD_MAX))
//    {
//        fragColor = vOutColor;
//        return fragColor;
//    }

    
//    // ���� 4���� �ڳ� lumas �� ����
//    float DownLeft =    RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x - tuv.x, vUV.y - tuv.y), 0).rgb);
//    float UpRight = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x + tuv.x, vUV.y + tuv.y), 0).rgb);
//    float UpLeft = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x - tuv.x, vUV.y + tuv.y), 0).rgb);
//    float DownRight = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, float2(vUV.x + tuv.x, vUV.y - tuv.y), 0).rgb);

//    // 4 ���� �����ڸ��� lumas�� ���� (���� ������ ���� ����� ���� �߰� ������ ���)
//    float lumaDownUp = down + up;
//    float lumaLeftRight = left + right;

//    // �ڳʵ� ����
//    float lumaLeftCorners = DownLeft + UpLeft;
//    float lumaDownCorners = DownLeft + DownRight;
//    float lumaRightCorners = DownRight + UpRight;
//    float lumaUpCorners = UpRight + UpLeft;

//    // ����� ���� ���� ������ ��ȭ���� ����ġ ���
//    float edgeHorizontal = abs(-2.0 * left + lumaLeftCorners) + abs(-2.0 * Center + lumaDownUp) * 2.0 + abs(-2.0 * right + lumaRightCorners);
//    float edgeVertical = abs(-2.0 * up + lumaUpCorners) + abs(-2.0 * Center + lumaLeftRight) * 2.0 + abs(-2.0 * down + lumaDownCorners);

//    // ���� �����ڸ��� �����ΰ�? �����ΰ�?
//    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
//    // ���� �����ڸ��� �ݴ�������� 2���� �̿� �ؼ��� ������
//    float luma1 = isHorizontal ? down : left;
//    float luma2 = isHorizontal ? up : right;

//    // �� �������� ��ȭ���� ���
//    float gradient1 = luma1 - Center;
//    float gradient2 = luma2 - Center;

//    // ��� ������ ��ȭ�� �� ���ĸ���?
//    bool is1Steepest = abs(gradient1) >= abs(gradient2);

//    // �ش� ������ ��ȭ��, ����ȭ
//    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
//    // �����ڸ��� ���⿡ ���� ���� ũ��(�� �ȼ�)�� ����
//    float stepLength = isHorizontal ? g_vRenderResolution.y : g_vRenderResolution.x;

//    // �ùٸ� ������ ��� luma
//    float lumaLocalAverage = 0.0;

//    if (is1Steepest)
//    {
//    // ������ �ٲ�
//        stepLength = -stepLength;
//        lumaLocalAverage = 0.5 * (luma1 + Center);
//    }
//    else
//    {
//        lumaLocalAverage = 0.5 * (luma2 + Center);
//    }

//    // UV �� �ùٸ� �������� ���ȼ� �̵���Ŵ
//    float2 currentUv = vUV;
//    if (isHorizontal)
//    {
//        currentUv.y += stepLength * 0.5;
//    }
//    else
//    {
//        currentUv.x += stepLength * 0.5;
//    }
    
//    // ������ �������� offset (�� �ݺ� ���ǿ� ����) �� ���.
//    float2 offset = isHorizontal ? float2(g_vRenderResolution.x, 0.0) : float2(0.0, g_vRenderResolution.y);

//// �����ڸ��� �� ������ �����Ͽ� Ž���ϱ� ���ؼ� UV�� ���. QUALITY �� ������ �� ������ ��.
//    float2 uv1 = currentUv - offset;
//    float2 uv2 = currentUv + offset;

//// Ž���ϴ� ���׸�Ʈ�� ���� ������ lumas�� �а�, delta �� ����ϰ� ���� ��� luma�� ���
//    float lumaEnd1 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv1,0).rgb);
//    float lumaEnd2 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv2,0).rgb);
//    lumaEnd1 -= lumaLocalAverage;
//    lumaEnd2 -= lumaLocalAverage;

//// ���� �������� luma delta�� ���� ��ȭ�� ���� ũ��, �츮�� �����ڸ��� ���鿡 ������ ����
//    bool reached1 = abs(lumaEnd1) >= gradientScaled;
//    bool reached2 = abs(lumaEnd2) >= gradientScaled;
//    bool reachedBoth = reached1 && reached2;

//// ���鿡 �������� ���ߴٸ�, �츮�� ����ؼ� �� �������� Ž����.
//    if (!reached1)
//    {
//        uv1 -= offset;
//    }
//    if (!reached2)
//    {
//        uv2 += offset;
//    }
    
//    // ���� �� ���� ��� ���鿡 �������� �ʾҴٸ�, ����ؼ� Ž��
//    if (!reachedBoth)
//    {
//        for (int i = 2; i < 4; i++)
//        {
//    // �ʿ��ϴٸ�, ù���� ������ luma�� ����, delta ���
//            if (!reached1)
//            {
//                lumaEnd1 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv1,0).rgb);
//                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
//            }
//    // �ʿ��ϴٸ�, �ݴ� ������ luma�� ����, delta ���
//            if (!reached2)
//            {
//                lumaEnd2 = RGBtoluma(g_RTCopyTex.SampleLevel(g_sam_0, uv2, 0).rgb);
//                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
//            }
//    // ���� ���� ������ luma delta�� ���� ��ȭ�� ���� ũ�ٸ�, �츮�� �����ڸ� ���鿡 ������ ����
//            reached1 = abs(lumaEnd1) >= gradientScaled;
//            reached2 = abs(lumaEnd2) >= gradientScaled;
//            reachedBoth = reached1 && reached2;

//    // ���� ���鿡 �������� �ʾҴٸ�, �츮�� �� �������� ��� Ž��, 
//    // ���� ǰ���� ���� (���� : ������ �ݺ��� ���� �����ϰڴٴ� �ǹ�)
//            if (!reached1)
//            {
//                uv1 -= offset * QUALITY(i);
//            }
//            if (!reached2)
//            {
//                uv2 += offset * QUALITY(i);
//            }

//    // �� ���鿡 �����ߴٸ�, Ž���� �ߴ�
//            if (reachedBoth)
//            {
//                break;
//            }
//        }
//    }
//    // �� �� �����ڸ������� �Ÿ��� ���
//    float distance1 = isHorizontal ? (vUV.x - uv1.x) : (vUV.y - uv1.y);
//    float distance2 = isHorizontal ? (uv2.x - vUV.x) : (uv2.y - vUV.y);

//    // � ������ �����ڸ��� ���� �� ������?
//    bool isDirection1 = distance1 < distance2;
//    float distanceFinal = min(distance1, distance2);

//    // �����ڸ��� ����
//    float edgeThickness = (distance1 + distance2);

//    // UV offset: �����ڸ��� ������� ���� ����� �������� ����
//    float pixelOffset = -distanceFinal / edgeThickness + 0.5;
    
//    // �߾� luma�� �κ� ��պ��� �� �۳�?
//    bool isLumaCenterSmaller = Center < lumaLocalAverage;

//    // ���� �߾� luma�� �װ��� �̿����� �� �۴ٸ�, �� ���� delta luma�� ������� �մϴ�. (���� ����)
//    // (�����ڸ��� ����� �� ����� ��������)
//    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

//    // ���� luma ���̰� �߸��� ���, offset �� �������� ����
//    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
//    // Sub-pixel shifting
//    // 3x3 �̿��� ���� luma�� ��ü ���� ���
//    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

//    // 3x3 �̿��� �縶 ������ ����, ���� ��հ� �߾� luma ���� delta�� ����
//    float subPixelOffset1 = clamp(abs(lumaAverage - Center) / range, 0.0, 1.0);
//    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

//    // �� delta�� ����� sub-pixel�� offset ���
//    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

//    // �� offset �� �� ū���� ��
//    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
//    // ���� UV ��ǥ ���
//    float2 finalUv = vUV;
//    if (isHorizontal)
//    {
//        finalUv.y += finalOffset * stepLength;
//    }
//    else
//    {
//        finalUv.x += finalOffset * stepLength;
//    }

//// ���ο� UV ��ǥ���� �÷��� �а� �����
//    float4 finalColor = g_RTCopyTex.SampleLevel(g_sam_0, finalUv,0);
//    fragColor = finalColor;
    
//    return fragColor;

    //RGBtoluma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x, vUV.y + tuv.y)).rgb)
    
    // FXAA �̴� Bloom�� �ƴ�
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
    
// delta ���
    float lumaRange = lumaMax - lumaMin;

// ���� luma ���̰� �Ӱ谪���� �۴ٸ�(�Ǵ� �츮�� ���� ��ο� �������� �ִٸ�), 
// �츮�� �����ڸ��� ���� �ʴ� ���̹Ƿ�, AA�� �������� �ʴ´�.
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
    {
        fragColor = colorCenter;
        return fragColor;
    }
    
    // ���� 4���� �ڳ� lumas �� ����
    float lumaDownLeft =    rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x - tuv.x, vUV.y - tuv.y)).rgb);
    float lumaUpRight =     rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y + tuv.y)).rgb);
    float lumaUpLeft =      rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x - tuv.x, vUV.y + tuv.y)).rgb);
    float lumaDownRight =   rgb2luma(g_RTCopyTex.Sample(g_sam_0, float2(vUV.x + tuv.x, vUV.y - tuv.y)).rgb);

    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    // �ڳʵ� ����
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    // ����� ���� ���� ������ ��ȭ���� ����ġ ���
    float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);

    // ���� �����ڸ��� �����ΰ�? �����ΰ�?
    // �ΰ��� ���� ���� �� ū ���� �������� ���ϴ� ����
    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
    
    // ���� �����ڸ��� �ݴ�������� 2���� �̿� �ؼ��� ������
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;

    // �� �������� ��ȭ���� ���
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    // ��� ������ ��ȭ�� �� ���ĸ���?
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    // �ش� ������ ��ȭ��, ����ȭ
    // �� ���밪 ���̿��� �ִ� ���� �������� ���밪�� �׻� ����̱� ������ �ִ밪�� ���� �� 4�� ������ ����ȭ�ϴ� ��Ȱ�� �Ѵ�.
    // �븻������.
    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));
    
    // �����ڸ��� ���⿡ ���� ���� ũ��(�� �ȼ�)�� ����
    float stepLength = isHorizontal ? tuv.y : tuv.x;

    // �ùٸ� ������ ��� luma
    float lumaLocalAverage = 0.0;

    if (is1Steepest)
    {
    // ������ �ٲ�
        stepLength = -stepLength;
        lumaLocalAverage = 0.5 * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5 * (luma2 + lumaCenter);
    }

// UV �� �ùٸ� �������� ���ȼ� �̵���Ŵ
    float2 currentUv = vUV;
    if (isHorizontal)
    {
        currentUv.y += stepLength * 0.5;
    }
    else
    {
        currentUv.x += stepLength * 0.5;
    }
    
    // ������ �������� offset (�� �ݺ� ���ǿ� ����) �� ���.
    float2 offset = isHorizontal ? float2(tuv.x, 0.0) : float2(0.0, tuv.y);

// �����ڸ��� �� ������ �����Ͽ� Ž���ϱ� ���ؼ� UV�� ���. QUALITY �� ������ �� ������ ��.
    float2 uv1 = currentUv - offset;
    float2 uv2 = currentUv + offset;

// Ž���ϴ� ���׸�Ʈ�� ���� ������ lumas�� �а�, delta �� ����ϰ� ���� ��� luma�� ���
    float lumaEnd1 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv1).rgb);
    float lumaEnd2 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

// ���� �������� luma delta�� ���� ��ȭ�� ���� ũ��, �츮�� �����ڸ��� ���鿡 ������ ����
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

// ���鿡 �������� ���ߴٸ�, �츮�� ����ؼ� �� �������� Ž����.
    if (!reached1)
    {
        uv1 -= offset;
    }
    if (!reached2)
    {
        uv2 += offset;
    }
    
    // ���� �� ���� ��� ���鿡 �������� �ʾҴٸ�, ����ؼ� Ž��
    if (!reachedBoth)
    {
        for (int i = 2; i < 8; i++)
        {
    // �ʿ��ϴٸ�, ù���� ������ luma�� ����, delta ���
            if (!reached1)
            {
                lumaEnd1 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
    // �ʿ��ϴٸ�, �ݴ� ������ luma�� ����, delta ���
            if (!reached2)
            {
                lumaEnd2 = rgb2luma(g_RTCopyTex.Sample(g_sam_0, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
    // ���� ���� ������ luma delta�� ���� ��ȭ�� ���� ũ�ٸ�, �츮�� �����ڸ� ���鿡 ������ ����
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

    // ���� ���鿡 �������� �ʾҴٸ�, �츮�� �� �������� ��� Ž��, 
    // ���� ǰ���� ���� (���� : ������ �ݺ��� ���� �����ϰڴٴ� �ǹ�)
            if (!reached1)
            {
                uv1 -= offset * QUALITY(i);
            }
            if (!reached2)
            {
                uv2 += offset * QUALITY(i);
            }

    // �� ���鿡 �����ߴٸ�, Ž���� �ߴ�
            if (reachedBoth)
            {
                break;
            }
        }
    }
    
    // �� �� �����ڸ������� �Ÿ��� ���
    float distance1 = isHorizontal ? (vUV.x - uv1.x) : (vUV.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - vUV.x) : (uv2.y - vUV.y);

    // � ������ �����ڸ��� ���� �� ������?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    // �����ڸ��� ����
    float edgeThickness = (distance1 + distance2);

    // UV offset: �����ڸ��� ������� ���� ����� �������� ����
    float pixelOffset = -distanceFinal / edgeThickness + 0.5;
    
    // �߾� luma�� �κ� ��պ��� �� �۳�?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

// ���� �߾� luma�� �װ��� �̿����� �� �۴ٸ�, �� ���� delta luma�� ������� �մϴ�. (���� ����)
// (�����ڸ��� ����� �� ����� ��������)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

// ���� luma ���̰� �߸��� ���, offset �� �������� ����
    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
    // Sub-pixel shifting
// 3x3 �̿��� ���� luma�� ��ü ���� ���
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

// 3x3 �̿��� �縶 ������ ����, ���� ��հ� �߾� luma ���� delta�� ����
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0, 1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

// �� delta�� ����� sub-pixel�� offset ���
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

// �� offset �� �� ū���� ��
    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    // ���� UV ��ǥ ���
    float2 finalUv = vUV;
    if (isHorizontal)
    {
        finalUv.y += finalOffset * stepLength;
    }
    else
    {
        finalUv.x += finalOffset * stepLength;
    }

// ���ο� UV ��ǥ���� �÷��� �а� �����
    float4 finalColor = g_RTCopyTex.Sample(g_sam_0, finalUv);
    fragColor = finalColor;
    
    return fragColor;
}




#endif