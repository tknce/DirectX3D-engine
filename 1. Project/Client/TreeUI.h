#pragma once
#include "UI.h"

class TreeUI;

class TreeNode
{
private:
    string              m_strName;              // ���̵�
    DWORD_PTR           m_data;                 // ������ ������
    string              m_strDatename;

    vector<TreeNode*>   m_vecChildNode;         // ����� �ڽĵ�����
    TreeNode*           m_ParentNode;           // �θ� ���
    TreeUI*             m_TreeUI;               // �ϳ��� UI�� ����

    UINT                m_iIdx;                // �ڽ��� ���� �ĺ� ��ȣ

    bool                m_bFrame;               // �ڽ��� �ֻ��� �θ�� ������ �����
    bool                m_bSelected;            // ���� üũ
    bool                m_bPrefab;
    bool                m_bInstantiate;

public: 
    DWORD_PTR GetData() { return m_data; }              // ������ �ֱ�
    const string& GetName() { return m_strName; }       // �̸� ���ϱ�
    const string& GetDataName() { return m_strDatename; }      // ������ �̸�
    void ResetDataName() { m_strDatename.clear(); }
private:
    void render_update();
    void SetNodeName(const string& _name) { m_strName = _name; }    // ������ ��� �̸� ���ϱ�
    void SetData(DWORD_PTR _data) { m_data = _data; }               // ������ ��� ������ ���ϱ�
    void SetFrame(bool _bFrame) { m_bFrame = _bFrame; }             // Ŭ��üũ
    void AddChild(TreeNode* _ChildNode)                             // �� �Ʒ��� �ڽ� �ֱ�
    {
        _ChildNode->m_ParentNode = this;
        m_vecChildNode.push_back(_ChildNode);
    }


    const vector<TreeNode*>& GetChild() { return m_vecChildNode; }  // �ڽ� �ֱ�


public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};




class TreeUI :
    public UI
{
private:
    static UINT m_iNextNodeIdx;     // ���� ���̵� �������� �ֱ� ���� ����

private:
    TreeNode*   m_RootNode;         // �ֻ��� ���
    bool        m_bDummyRootUse;    // ��Ʈ��带 ���̷� ����Ұ��ΰ�

    TreeNode*   m_SelectedNode;
    TreeNode*   m_BeginDragNode;    // �巡�� ���� ���
    TreeNode*   m_DropTargetNode;   // ��� ��� ���

    UI*         m_SelectInst;       // ��������Ʈ
    FUNC_1      m_SelectFunc;       // ���õ�����

    UI*         m_DragDropInst;     // ��������Ʈ 
    FUNC_2      m_DragDropFunc;     // �巡�׵�����

    UI*         m_RightInst;       // ��������Ʈ 
    FUNC_1_1    m_RightFunc;       // ��Ŭ���� Prefab

    UI*         m_InstiateInst;       // ��������Ʈ
    FUNC_1_2    m_InstiateFunc;       // ��Ŭ����

    UI*         m_DeleteInst;       // ��������Ʈ
    FUNC_1      m_DeleteFunc;       // ��Ŭ���� delete

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeNode* AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame = false); // ��� ���� ��ϵ� �θ� ��ȯ
    void Clear();
    void SetDummyRoot(bool _bUse) { m_bDummyRootUse = _bUse; }
    void SetSelecteNode(TreeNode* _SelectedNode);
    void SetBeginDragNode(TreeNode* _BeginDragNode) { m_BeginDragNode = _BeginDragNode; }
    void SetDropTargetNode(TreeNode* _DropTargetNode);
    void SetPrefabNode(TreeNode* _DropTargetNode);
    void SetPrefabInstiate(TreeNode* _InstiateNode);
    void Delete(TreeNode* _Node);

    TreeNode* GetSelectedNode() { return m_SelectedNode; }


    void AddDynamic_Selected(UI* _Inst, FUNC_1 _Func)
    {
        m_SelectInst = _Inst;
        m_SelectFunc = _Func;
    }
    void AddDynamic_DragDrop(UI* _Inst, FUNC_2 _Func)
    {
        m_DragDropInst = _Inst;
        m_DragDropFunc = _Func;
    }
    void AddDynamic_RightMenu(UI* _Inst, FUNC_1_1 _Func)
    {
        m_RightInst = _Inst;
        m_RightFunc = _Func;
    }
    void AddDynamic_Instiate(UI* _Inst, FUNC_1_1 _Func)
    {
        m_InstiateInst = _Inst;
        m_InstiateFunc = _Func;
    }
    void AddDynamic_Delete(UI* _Inst, FUNC_1 _Func)
    {
        m_DeleteInst = _Inst;
        m_DeleteFunc = _Func;
    }


public:
    TreeUI(const string& _strName);
    ~TreeUI();
};

