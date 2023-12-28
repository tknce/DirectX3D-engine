#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI*             m_Tree;
    CGameObject*        m_Prefab;
    vector<wstring>     m_vecContentName;

    bool                m_Ins;
public:
    // ���� ���ҽ� ����� Ʈ���� ����
    void ResetContent();

    // Content ���� �ؿ� ���ϵ��� �޸𸮿� �ҷ���
    void ReloadContent();

private:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetResourcetoInspector(DWORD_PTR _res);
    void FindContentFileName(const wstring& _strFolderPath);
    
    RES_TYPE GetResTypeByExt(wstring _filename);

private:
    void InstantiatePrefab(DWORD_PTR _res);
    

public:
    ContentUI();
    ~ContentUI();
};

