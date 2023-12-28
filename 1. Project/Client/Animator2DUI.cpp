#include "pch.h"
#include "Animator2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include "ParamUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2DUI", COMPONENT_TYPE::ANIMATOR2D)
    , m_iItemCurIdx(0)
    , m_bPlay(false)
    , m_bOrginNomove(true)
    , m_bAnimation(false)
    , m_vSceneSize(128,128)
    , m_bnewani(false)
    , m_Acctime()
    , m_bSign(false)
    , m_Revise(false)
    , m_PreName{}
{
    m_vSliceEX = Vec2(46, 43);
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::update()
{
	ComponentUI::update();

    if (GetTarget())
    {
        
        if (nullptr == m_AtlasTex)
        {
            //m_Animation = GetTarget()->Animator2D()->FindAnimation(L"LeftWalk");

        }
    }
	

}

void Animator2DUI::render_update()
{

    ComponentUI::render_update();
    Reset();

    ImGui::Checkbox("Animation Mode", &m_bAnimation);
    //static int m_iItemCurIdx = 0;
    if (!m_vCurAni.empty())
    {
        m_bSign = false;
        ImGui::Text("CurAnimationName"); ImGui::SameLine(); ImGui::InputText("##curAnim", (char*)m_vCurAni[m_iItemCurIdx].data(), m_vCurAni[m_iItemCurIdx].length(), ImGuiInputTextFlags_ReadOnly);
        wstring Find;
        Find.assign(m_vCurAni[m_iItemCurIdx].begin(), m_vCurAni[m_iItemCurIdx].end());
        m_Animation = GetTarget()->Animator2D()->FindAnimation(Find);

            if(m_Animation->GetAtlas() == nullptr)
            m_Animation->SetAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"));

            m_vecFrmUI = m_Animation->GetAnimFrm();
            m_AtlasTex = m_Animation->GetAtlas();
        

    }
    m_vCurAni.clear();
    // 새로운 애니메이션 만들기 버튼
    if(ImGui::Button("New Ani") || m_bnewani)
    {
        m_bnewani = true;
        NewFrm();
    }
    // 애니메이션 이름 수정
    ImGui::SameLine();
    if (ImGui::Button("ReName") || m_Revise)
    {
        m_Revise = true;
        ReName();

    }


    if (nullptr != m_AtlasTex)
    {
        Ptr<CTexture> ObjectTex = m_Animation->GetAtlas();
        string Texname = string(ObjectTex.Get()->GetKey().begin(), ObjectTex.Get()->GetKey().end());
        if (ParamUI::Param_Tex(Texname, ObjectTex, this, (FUNC_1)&Animator2DUI::SetTexture))
        {

        }
    }

    const map<wstring, CAnimation2D*>& Animaiton = GetTarget()->Animator2D()->GetMapAnim();

    // 애니메이션 들고 있는거 추출
    map<wstring, CAnimation2D*>::const_iterator iter = Animaiton.begin();
    wstring strName;
    string str;
    for (; iter != Animaiton.end(); ++iter)
    {
        strName = iter->first;
        str = string(strName.begin(), strName.end());
        m_vCurAni.push_back(str);
    }



    // 애니메이션 이름 나열
    if (ImGui::BeginListBox("##"))
    {
        for (int n = 0; n < m_vCurAni.size(); n++)
        {
            const bool is_selected = (m_iItemCurIdx == n);
            if (ImGui::Selectable(m_vCurAni[n].c_str(), is_selected))
                m_iItemCurIdx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
                m_PreName = m_vCurAni[n];
            }
        }
        if(!m_vCurAni.empty())
        if (m_PreName != m_vCurAni[m_iItemCurIdx])
            m_bSign = true;

        ImGui::EndListBox();
    }


    // 원본이미지 자르기
    if (m_bAnimation)
    {
        OriginalImage();
    }
    
    
    // 이미지 자르기
    if (m_bAnimation)
    {
        ImGui::Begin("Animation", 0, ImGuiWindowFlags_HorizontalScrollbar);
        
        // 현재이미지
        ImVec2 CurPos = ImGui::GetCursorScreenPos();
        int preIdx = m_iSelectedIdx -1;
        if (preIdx <= m_vecFrmUI.size() - m_vecFrmUI.size())
        {
            preIdx = 0;
        }
        if (preIdx > m_vecFrmUI.size())
        {
            preIdx = m_vecFrmUI.size() -1;
        }
        ImGui::Image((void*)m_AtlasTex.Get()->GetSRV().Get(), ImVec2(256 , 256 ), m_vecFrmUI[preIdx].vLeftTop , m_vecFrmUI[preIdx].vLeftTop + m_vecFrmUI[preIdx].vSlice, ImVec4(1,1,1,0.2));
        
        ImGui::GetWindowDrawList()->AddImage(m_AtlasTex.Get()->GetSRV().Get(), CurPos, ImVec2(CurPos.x + 256, CurPos.y + 256), m_vecFrmUI[m_iSelectedIdx].vLeftTop, m_vecFrmUI[m_iSelectedIdx].vLeftTop + m_vecFrmUI[m_iSelectedIdx].vSlice);

        ImGui::SameLine();
        // 이미지 플레이 and 스탑
        if (ImGui::Button("Play##Animation") || m_bPlay)
        {
            

            // 시간 체크
            m_Acctime += DT;

            // 누적 시간이 해당 프레임 유지시간을 넘어서면 다음프레임으로 넘어감
            if (m_vecFrmUI[m_iSelectedIdx].fDuration < m_Acctime)
            {
                m_Acctime = 0.f;
                ++m_iSelectedIdx;
            }
            if (m_iSelectedIdx > m_vecFrmUI.size()-1)
            {
                m_iSelectedIdx = 0;
            }
            m_bPlay = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop##Animation"))
        {
            m_bPlay = false;
            m_iSelectedIdx = m_AnimPreviewIdx;
        }
        ImGui::SameLine();
        if (ImGui::Button("Set##Animation"))
        {
            m_vecFrmUI[m_iSelectedIdx].vLeftTop = Vec2(m_vLeftTopMouse.x, m_vLeftTopMouse.y);

            ImVec2 uv1 = m_vSliceMouse - m_vLeftTopMouse;
            m_vecFrmUI[m_iSelectedIdx].vSlice = Vec2(uv1.x, uv1.y);

            m_vecFrmUI[m_iSelectedIdx].fDuration = 0.1;
            
            m_vecFrmUI[m_iSelectedIdx].vFullSize = Vec2(100.f/ m_AtlasTex->GetWidth(), 100.f / m_AtlasTex->GetHeight());

        }
        ImGui::SameLine();
        if (ImGui::Button("Delete##Animation"))
        {
            vector<tAnim2DFrm>::iterator iter = m_vecFrmUI.begin();
            int i = 0;
            for (; iter != m_vecFrmUI.end();)
            {
                if (i == m_iSelectedIdx)
                {
                    ++i;
                    m_iSelectedIdx = 0;
                    iter = m_vecFrmUI.erase(iter);
                }
                else
                {
                    ++i;
                    ++iter;
                }
            }
        }
        Animation();

        SetInfo();

        ImGui::End();
    }

    
}




void Animator2DUI::OriginalImage()
{
    ImVec2 uv1;
        ImGuiWindowFlags_ nomove = ImGuiWindowFlags_None;
        if (m_bOrginNomove)
        {
            nomove = ImGuiWindowFlags_NoMove;
        }
        else
        {
            nomove = ImGuiWindowFlags_None;
        }
        
        ImGui::Begin("DirectX11 Texture1", 0, nomove);
        ImGui::Checkbox("WindowMove##Animation", &m_bOrginNomove);
        ImGuiIO& io = ImGui::GetIO();
        float my_tex_w = (float)m_AtlasTex.Get()->GetWidth();
        float my_tex_h = (float)m_AtlasTex.Get()->GetHeight();
        ImGui::Text("%.0fx%.0f##1", my_tex_w, my_tex_h);
        /*my_tex_w /= 3;
        my_tex_h /= 3;*/
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        float zoom = 4.0f;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 Lefttop = ImVec2(m_vSlice.x, m_vSlice.y);
        ImVec2 Screenpos = ImGui::GetCursorScreenPos();
        Vec2 TEMLeftTop = Vec2(Lefttop.x + Screenpos.x, Lefttop.y + Screenpos.y);
        Vec2 TEMRightBot = Vec2(m_vSlice.z + Screenpos.x, m_vSlice.w + Screenpos.y);
        

        draw_list->AddRect(TEMLeftTop, TEMRightBot, IM_COL32(0, 255, 0, 255));
        ImGui::Image((void*)m_AtlasTex.Get()->GetSRV().Get(), ImVec2(my_tex_w , my_tex_h ), uv_min, uv_max, tint_col, border_col);
        if (ImGui::IsItemHovered())
        {

            float region_sz = 64;
            float region_x = io.MousePos.x - pos.x - ((region_sz) * 0.5f);
            float region_y = io.MousePos.y - pos.y - ((region_sz) * 0.5f);

            if (region_x < 0.0f) { region_x = 0.0f; }
            else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
            if (region_y < 0.0f) { region_y = 0.0f; }
            else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }

            if (region_x == 0)
            {
                region_x -= (region_sz / 2);
                region_y -= (region_sz / 2);
            }

            ImVec2 uv0 = ImVec2((region_x + (region_sz / 2)) / my_tex_w, (region_y + (region_sz / 2)) / my_tex_h);
            if (region_x >= (my_tex_w - (region_sz)))
            {
                region_x += (region_sz / 2);
            }
            if (region_y >= (my_tex_h - (region_sz)))
            {
                region_y += (region_sz / 2);
            }
            uv1 = ImVec2((region_x + (region_sz / 2)) / my_tex_w, (region_y + (region_sz / 2)) / my_tex_h);

            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            { 
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                //SetIsDrag();
                m_vLeftTopMouse = uv0;
                vector<vector<tBGRA>> _inVec = {};
                m_AtlasTex.Get()->GetPixelVector(_inVec);
                
                float region_x = io.MousePos.x - pos.x;
                float region_y = io.MousePos.y - pos.y;
                m_vSlice = m_AtlasTex.Get()->WIdthSearch(_inVec, Vec2(region_x, region_y));
                m_vLeftTopMouse = ImVec2(m_vSlice.x, m_vSlice.y);
                m_vSliceMouse = ImVec2(m_vSlice.z, m_vSlice.w);
                m_vLeftTopMouse.x /= m_AtlasTex.Get()->GetWidth();
                m_vLeftTopMouse.y /= m_AtlasTex.Get()->GetHeight();
                m_vSliceMouse.x /= m_AtlasTex.Get()->GetWidth();
                m_vSliceMouse.y /= m_AtlasTex.Get()->GetHeight();
            }


            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                m_vSliceMouse =  uv1;

            }

        }
 
        ImGui::Image((void*)m_AtlasTex.Get()->GetSRV().Get(), ImVec2(64.f * zoom, 64.f * zoom), m_vLeftTopMouse, m_vSliceMouse);



        ImGui::End();
        
}

void Animator2DUI::Animation()
{

   
   ImGui::Text("Animation Frm Composition");

   ImGuiStyle& style = ImGui::GetStyle();

   ImGui::Spacing();
   ImGui::PushID("##ANED_HorizonScroll");
   float child_height = m_vSceneSize.y + style.ScrollbarSize + style.WindowPadding.y * 2.0f; //ImGui::GetTextLineHeight()

   ImVec2 scrolling_child_size = ImVec2(-100, child_height);
   if (ImGui::BeginChild("##ANED_ScrollChild", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar)) // Avoid calling SetScrollHereY when running with culled items
   {
       for (int i = 0; i < m_vecFrmUI.size(); i++)
       {
           // 동일 선상에 생성
           if (i > 0)
               ImGui::SameLine();


           ImVec2 CursorPos = ImGui::GetCursorPos();
           // 선택 가능한 버튼 생성
           // 선택 버튼 id 만들기
           string SelectId = "";
           char id[50] = "";
           sprintf_s(id, 50, "[%d]", i);
           SelectId += id;
           SelectId += "##ANED_Frm";

           bool selected = (m_iSelectedIdx == i);
           if (ImGui::Selectable(SelectId.c_str(), selected, 0, m_vSceneSize) )
           {
               // 해당 버튼을 클릭하면, 해당 프레임 정보가 아래에 떠야한다.
               // 이곳에서는 버튼을 클릭하면 해당 프레임 정보로 바꾸어주는 기능이 필요하다.
               m_iSelectedIdx = i;
               m_AnimPreviewIdx = i;
           }


           if (selected)
               ImGui::SetItemDefaultFocus();

           // Selected 위치에 이미지 그려주기
           ImGui::SetCursorPos(CursorPos);
           ImTextureID my_tex_id = m_AtlasTex->GetSRV().Get();
           ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
           ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white ImVec2(m_TexWH.x, m_TexWH.y)
           ImVec2 uv_min = ImVec2(m_vecFrmUI[i].vLeftTop);                     // Top-left
           ImVec2 uv_max = ImVec2(m_vecFrmUI[i].vSlice + m_vecFrmUI[i].vLeftTop);      // Lower-right
           
           ImGui::Image(my_tex_id, m_vSceneSize, uv_min, uv_max, tint_col, border_col);


       }
   }
 
   float scroll_x = ImGui::GetScrollX();
   float scroll_max_x = ImGui::GetScrollMaxX();
   ImGui::EndChild();
   ImGui::SameLine();

   // 프레임 추가 버튼 만들기
   if (ImGui::Button("+##ANED_AddFrm", ImVec2(30.f, 30.f)))
   {
       // 새로운 프레임을 추가해 넣어준다.
       tAnim2DFrm NewFrm = {};
       NewFrm.vFullSize = Vec2(100.f / m_AtlasTex.Get()->GetWidth(), 100.f / m_AtlasTex.Get()->GetHeight());
       NewFrm.vSlice = m_vSliceEX;
       m_vecFrmUI.push_back(NewFrm);
   }

   ImGui::Spacing();

   ImGui::PopID();
   
   

}
void Animator2DUI::SetInfo()
{
    Vec3 Vecfrm;
    Vecfrm = Vec3(m_vecFrmUI[m_iSelectedIdx].vLeftTop.x * m_AtlasTex.Get()->GetWidth(), m_vecFrmUI[m_iSelectedIdx].vLeftTop.y * m_AtlasTex.Get()->GetHeight(), 0.f);
    ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", Vecfrm);
    m_vecFrmUI[m_iSelectedIdx].vLeftTop = Vec2(Vecfrm.x / m_AtlasTex.Get()->GetWidth(), Vecfrm.y / m_AtlasTex.Get()->GetHeight());

    Vecfrm = Vec3(m_vecFrmUI[m_iSelectedIdx].vSlice.x * m_AtlasTex.Get()->GetWidth(), m_vecFrmUI[m_iSelectedIdx].vSlice.y * m_AtlasTex.Get()->GetHeight(), 0.f);
    ImGui::Text("Slice   "); ImGui::SameLine(); ImGui::InputFloat2("##Slice", Vecfrm);
    m_vecFrmUI[m_iSelectedIdx].vSlice = Vec2(Vecfrm.x / m_AtlasTex.Get()->GetWidth(), Vecfrm.y / m_AtlasTex.Get()->GetHeight());
    ImGui::SameLine();
    if (ImGui::Button("Set##allslice"))
    {
        for (size_t i = 0; i < m_vecFrmUI.size(); ++i)
        {
            m_vecFrmUI[i].vSlice = Vec2(Vecfrm.x / m_AtlasTex.Get()->GetWidth(), Vecfrm.y / m_AtlasTex.Get()->GetHeight());
        }
    }
    Vecfrm = Vec3(m_vecFrmUI[m_iSelectedIdx].vOffset.x, m_vecFrmUI[m_iSelectedIdx].vOffset.y, 0.f);
    ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", Vecfrm);
    m_vecFrmUI[m_iSelectedIdx].vOffset = Vec2(Vecfrm.x, Vecfrm.y);

    Vecfrm = Vec3(m_vecFrmUI[m_iSelectedIdx].vFullSize.x * m_AtlasTex.Get()->GetWidth(), m_vecFrmUI[m_iSelectedIdx].vFullSize.y * m_AtlasTex.Get()->GetHeight(), 0.f);
    ImGui::Text("Fullsize"); ImGui::SameLine(); ImGui::InputFloat2("##Fullsize", Vecfrm);
    m_vecFrmUI[m_iSelectedIdx].vFullSize = Vec2(Vecfrm.x / m_AtlasTex.Get()->GetWidth(), Vecfrm.y / m_AtlasTex.Get()->GetHeight());
    ImGui::SameLine();
    if (ImGui::Button("Set##allFullsize"))
    {
        for (size_t i = 0; i < m_vecFrmUI.size(); ++i)
        {
            m_vecFrmUI[i].vFullSize = Vec2(Vecfrm.x / m_AtlasTex.Get()->GetWidth(), Vecfrm.y / m_AtlasTex.Get()->GetHeight());
        }
    }
    m_fFPS = m_vecFrmUI[m_iSelectedIdx].fDuration;



    float FPS = m_fFPS;
    float time = 10;
    while (true)
    {
        FPS *= 10;
        if (FPS > 10)
        {
            time /= FPS;
            m_fFPS = time;
            break;
        }
        time *= 10;
    }
    
    ImGui::Text("Duration"); ImGui::SameLine(); ImGui::InputFloat("##Duration", &m_fFPS);

    for (int i = 0; i < m_vecFrmUI.size(); ++i)
    {
        m_vecFrmUI[i].fDuration = 1 / m_fFPS;
    }
    



    if (GetTarget())
    {
        m_Animation->SetVector(m_vecFrmUI);
    }
}

void Animator2DUI::NewFrm()
{
    ImGui::Begin("New Frm");

    static char name[32];
    ImGui::InputText("##Anim", name, IM_ARRAYSIZE(name));
    //ImGui::InputText("NEWNAME##Anim", (char*)m_sNewName.data(), m_sNewName.length());
    if (ImGui::Button("New"))
    {
        m_sNewName = name;
        wstring NewName = wstring(m_sNewName.begin(), m_sNewName.end());
        GetTarget()->Animator2D()->CreateAnimation(NewName, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"), Vec2(0, 0), m_vSliceEX, 30, 6, 12);
    }

    if (ImGui::Button("OK##NewFrm"))
        m_bnewani = false;
    
    ImGui::End();
}

void Animator2DUI::ReName()
{
    // 애니메이션 이름 바꾸기
    ImGui::Begin("rename");
    
    static char rename[32];
    string _preName = m_PreName;
    strncpy_s(rename, m_sNewName.c_str(), sizeof(m_sNewName) - 1);
    ImGui::InputText("##Rename", rename, sizeof(rename)-1);
    m_sNewName = rename;
	wstring NewName = wstring(m_sNewName.begin(), m_sNewName.end());

    // 셋버튼 클릭시 이름 바꿈
    if (ImGui::Button("Set##rename"))
    {
        wstring Newname1 = wstring(_preName.begin(), _preName.end());
        // 이전 이름을 지우고 새로운 네임을 넣는다
        GetTarget()->Animator2D()->ChangeName(Newname1, NewName);
        _preName = m_sNewName;
    }

    if (ImGui::Button("OK##rename"))
    {
        m_PreName = _preName;
        m_Revise = false;
    }
    ImGui::End();
}

void Animator2DUI::SetIsDrag()
{

        // 드래그 시 이미지 자르기
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float region_sz = 64;
        float region_x = io.MousePos.x - pos.x - (region_sz) * 0.5f;
        float region_y = io.MousePos.y - pos.y - (region_sz) * 0.5f;
        float my_tex_w = m_AtlasTex.Get()->GetWidth();
        float my_tex_h = m_AtlasTex.Get()->GetHeight();

        if (region_x < 0.0f) { region_x = 0.0f; }
        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
        if (region_y < 0.0f) { region_y = 0.0f; }
        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        ImGui::Text("Max: (%.2f, %.2f)", region_x + (region_sz / 2), region_y + (region_sz / 2));

        if (region_x == 0)
        {
            region_x -= (region_sz / 2);
            region_y -= (region_sz / 2);
        }

        ImVec2 uv0 = ImVec2((region_x + (region_sz / 2)) / my_tex_w, (region_y + (region_sz / 2)) / my_tex_h);
        if (region_x >= (my_tex_w - (region_sz)))
        {
            region_x += (region_sz / 2);
        }
        if (region_y >= (my_tex_h - (region_sz)))
        {
            region_y += (region_sz / 2);
        }
        ImVec2 uv1 = ImVec2((region_x + (region_sz / 2)) / my_tex_w, (region_y + (region_sz / 2)) / my_tex_h);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            m_vLeftTopMouse = uv0;

        }


        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            m_vSliceMouse = uv1;

        }
        IsDrag(io.MousePos);

        //Vec2 TEMLeftTop = Vec2(m_vTemLeftTop.x + Screenpos.x, m_vTemLeftTop.y + Screenpos.y);
        //Vec2 TEMRightBot = Vec2(m_vTemRigthBot.x + Screenpos.x, m_vTemRigthBot.y + Screenpos.y);
    
}

void Animator2DUI::Reset()
{
    if (m_PreOwnerName != GetTarget()->GetName())
    {
        m_AtlasTex = nullptr;
        m_Animation = nullptr;
        m_vecFrmUI.clear();
        m_vCurAni.clear();

        m_sNewName.clear();
        m_PreName.clear();
        m_iItemCurIdx = 0;
        m_PreOwnerName = GetTarget()->GetName();
    }
}

void Animator2DUI::SetTexture(DWORD_PTR _strTexKey)
{
    string strKey = (char*)_strTexKey;
    wstring wstrKey = wstring(strKey.begin(), strKey.end());

    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
    assert(nullptr != pTex);
    m_Animation->SetAtlas(pTex);
    

}

