#pragma once
#include "UI.h"

class TreeUI;

class TreeNode
{
private:
    string              m_strName;              // 아이디
    DWORD_PTR           m_data;                 // 전달할 데이터
    string              m_strDatename;

    vector<TreeNode*>   m_vecChildNode;         // 노드의 자식데이터
    TreeNode*           m_ParentNode;           // 부모 노드
    TreeUI*             m_TreeUI;               // 하나의 UI에 설정

    UINT                m_iIdx;                // 자신의 고유 식별 번호

    bool                m_bFrame;               // 자식이 최상위 부모면 프레임 씌우기
    bool                m_bSelected;            // 선택 체크
    bool                m_bPrefab;
    bool                m_bInstantiate;

public: 
    DWORD_PTR GetData() { return m_data; }              // 데이터 주기
    const string& GetName() { return m_strName; }       // 이름 정하기
    const string& GetDataName() { return m_strDatename; }      // 데이터 이름
    void ResetDataName() { m_strDatename.clear(); }
private:
    void render_update();
    void SetNodeName(const string& _name) { m_strName = _name; }    // 생성된 노드 이름 정하기
    void SetData(DWORD_PTR _data) { m_data = _data; }               // 생성된 노드 데이터 정하기
    void SetFrame(bool _bFrame) { m_bFrame = _bFrame; }             // 클릭체크
    void AddChild(TreeNode* _ChildNode)                             // 내 아래에 자식 넣기
    {
        _ChildNode->m_ParentNode = this;
        m_vecChildNode.push_back(_ChildNode);
    }


    const vector<TreeNode*>& GetChild() { return m_vecChildNode; }  // 자식 주기


public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};




class TreeUI :
    public UI
{
private:
    static UINT m_iNextNodeIdx;     // 노드들 아이디 고유성을 주기 위한 변수

private:
    TreeNode*   m_RootNode;         // 최상위 노드
    bool        m_bDummyRootUse;    // 루트노드를 더미로 취급할것인가

    TreeNode*   m_SelectedNode;
    TreeNode*   m_BeginDragNode;    // 드래그 시작 노드
    TreeNode*   m_DropTargetNode;   // 드랍 대상 노드

    UI*         m_SelectInst;       // 델리게이트
    FUNC_1      m_SelectFunc;       // 선택됐을때

    UI*         m_DragDropInst;     // 델리게이트 
    FUNC_2      m_DragDropFunc;     // 드래그됐을때

    UI*         m_RightInst;       // 델리게이트 
    FUNC_1_1    m_RightFunc;       // 우클릭시 Prefab

    UI*         m_InstiateInst;       // 델리게이트
    FUNC_1_2    m_InstiateFunc;       // 우클릭시

    UI*         m_DeleteInst;       // 델리게이트
    FUNC_1      m_DeleteFunc;       // 우클릭시 delete

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeNode* AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame = false); // 노드 생성 등록된 부모 반환
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

