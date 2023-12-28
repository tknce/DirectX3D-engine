#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

SAVE_GAMEOBJECT CPrefab::Save_GameObject_Func = nullptr;
LOAD_GAMEOBJECT CPrefab::Load_GameObject_Func = nullptr;

CPrefab::CPrefab()
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _pProto)
    : CRes(RES_TYPE::PREFAB)
    , m_pProtoObj(_pProto)
{
    if (-1 != _pProto->GetLayerIdx())
    {
        m_pProtoObj = _pProto->Clone();
    }
}

CPrefab::~CPrefab()
{
    if (nullptr != m_pProtoObj)
        delete m_pProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
    if (nullptr == m_pProtoObj)
        return nullptr;

    //CLevel* curlevel = CLevelMgr::GetInst()->GetCurLevel();
    //wstring name = pObject->GetName();
    //int idx = 0;
    //while (true)
    //{
    //    name += std::to_wstring(idx);
    //    CGameObject* curobj = curlevel->FindObjectByName(name);

    //    if (curobj == nullptr)
    //        break;

    //    ++idx;
    //}

    //name += std::to_wstring(idx);
    //pObject->SetName(name);
    return m_pProtoObj->Clone();
}


void CPrefab::Save(const wstring& _strRelativePath)
{
    // ���ϰ� 1:1 ��Ī Ȯ��
    if (!CheckRelativePath(_strRelativePath))
        assert(nullptr);

    FILE* pFile = nullptr;

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    // Ű, ��� ����
    SaveKeyPath(pFile);

    // ���� ������Ʈ ���� ����
    Save_GameObject_Func(m_pProtoObj, pFile);

    fclose(pFile);
}

int CPrefab::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    LoadKeyPath(pFile);

    m_pProtoObj = Load_GameObject_Func(pFile);

    fclose(pFile);

    return S_OK;
}