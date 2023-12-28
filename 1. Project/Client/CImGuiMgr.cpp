#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine\CDevice.h>
#include <Engine/CKeyMgr.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UI.h"
#include "ParamUI.h"

#include "MenuUI.h"
#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutLinerUI.h"
#include "ListUI.h"

CImGuiMgr::CImGuiMgr()
    : m_NotifyHandle(nullptr)
{
}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);

    clear();

    // 이벤트 핸들 제거
    FindCloseChangeNotification(m_NotifyHandle);
}


void CImGuiMgr::init(HWND _hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // UI 생성
    CreateUI();

    m_NotifyHandle = FindFirstChangeNotification(
        CPathMgr::GetInst()->GetContentPath(), true
        , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
}


void CImGuiMgr::progress()
{

    // 알림 확인
    ObserveContent();
#ifdef _DEBUG
    // _DEBUG


    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::ParamCount = 0;
    {
        bool bTrue = true;
        ImGui::ShowDemoWindow(&bTrue);

        
        map<string, UI*>::iterator iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->update();
        }       

        iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->render();
        }
    }

   // return;
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
#else

#endif

}


void CImGuiMgr::ObserveContent()
{
    DWORD dwWateState = WaitForSingleObject(m_NotifyHandle, 0);

    if (dwWateState == WAIT_OBJECT_0)
    {
        ContentUI* pContentUI = (ContentUI*)FindUI("Content");
        pContentUI->ReloadContent();

        FindNextChangeNotification(m_NotifyHandle);
    }
}


void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}




void CImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;
    
    pUI = new InspectorUI;   
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
    
    pUI = new OutLinerUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ContentUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
    ((ContentUI*)pUI)->ReloadContent();
    ((ContentUI*)pUI)->ResetContent();

    pUI = new MenuUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    ListUI* listui = new ListUI;    
    listui->SetModal(true);
    m_mapUI.insert(make_pair(listui->GetName(), listui));
}


UI* CImGuiMgr::FindUI(const string& _name)
{
    map<string, UI*>::iterator iter =  m_mapUI.find(_name);
    if (iter == m_mapUI.end())
    {
        return nullptr;
    }

    return iter->second;
    
}