#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CRes :
    public CEntity
{
private:
    wstring         m_strKey;
    wstring         m_strRelativePath;
    const RES_TYPE  m_eResType;
    int             m_iRefCount;

    bool            m_bEngineRes;

public:
    void AddRef() 
    { 
        ++m_iRefCount; 
    }
    void Release() 
    {
        --m_iRefCount;
        assert(!(m_iRefCount < 0));

        if (0 == m_iRefCount)
        {
            delete this;
        }
    }
public:
    int GetRefCount() const { return m_iRefCount; }

    // 파일로 저장
    virtual void Save(const wstring& _strRelativePath) = 0;

    // 파일로 부터 로딩
    virtual int Load(const wstring& _strFilePath) = 0;



public:
    const wstring& GetKey() const { return m_strKey; }
    const wstring& GetRelativePath() const { return m_strRelativePath; }
    RES_TYPE GetResType() const { return m_eResType; }
    bool IsEngineRes() const { return m_bEngineRes; }

protected:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }



protected:
    // 리소스의 경로 검사(파일과 1:1 매칭)
    bool CheckRelativePath(const wstring& _strRelativePath);

    void SaveKeyPath(FILE* _pFile);
    void LoadKeyPath(FILE* _pFile);

    virtual CRes* Clone() = 0;

public:
    CRes(RES_TYPE _eResType, bool _bEngineRes = false);
    CRes(const CRes& _other);
    ~CRes();

    friend class CResMgr;
    friend class CLandScape;
    friend class CFBXLoader;

    template<typename T>
    friend class Ptr;    
};

