#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_CSBlob;
    ComPtr<ID3D11ComputeShader>     m_CS;

protected:
    tMtrlConst                      m_Param;    // CS �� ������ ��� ��

    UINT                            m_iGroupX;
    UINT                            m_iGroupY;
    UINT                            m_iGroupZ;

    const UINT                      m_iGroupPerThreadX;
    const UINT                      m_iGroupPerThreadY;
    const UINT                      m_iGroupPerThreadZ;

public:
    void CreateComputeShader(const wstring& _strRelativePath, const string& _strFuncName);
    void SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData);
    void GetScalarParam(SCALAR_PARAM _eScalarType, void* _pOut);

    void Execute();

    virtual void UpdateData() = 0;
    virtual void Clear() = 0;

public:
    CComputeShader(UINT _iGroupPerX, UINT _iGroupPerY, UINT _iGroupPerZ);
    ~CComputeShader();
};

