#pragma once
#include "pch.h"

// 버퍼가 반환되는 정보를 보관할 만큼 충분히 크지 않은 경우 반환된 정보는 버퍼에 맞게
// 잘리는 경우가 많으며 메서드에서 S_FALSE 반환됩니다.
HRESULT hr;

// 
ComPtr<ID3D11Device> m_bDevice;