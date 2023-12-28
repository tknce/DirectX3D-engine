#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CTileMap.h>
#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/Ptr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CMeshRender.h>

#include "SliceMapImage.h"

#include "ParamUI.h"
#include "ListUI.h"
#include "imgui.h"
#include "CImGuiMgr.h"






TileMapUI::TileMapUI()
	:ComponentUI("TileMapUI",COMPONENT_TYPE::TILEMAP)
    , m_bEditMode(false)
    , m_vSliceMapImage{}
    , m_bSliceImage(false)
    , m_vTileMap{}
    , m_vLeftTopMouse()
    , m_vSliceMouse()
    , m_bTileMap(false)
    , m_bTileReset(false)
    , m_vGridOffset(ImVec2(4,4))
    , m_bTilecut(true)
    , m_fTileCount{}
{
    m_vLeftTop = ImVec2(128.f, 64.f);
    m_vSlice = ImVec2(36.f, 38.f);

    m_vSliceCount = ImVec2(1, 1);
    SliceMapImage* Map = new SliceMapImage;
    //Map->SetImage(GetTarget()->TileMap()->GetTileAtlas());
   // Map->SetUV(0, 0);
    Map->SetSize(Vec2(0, 0));
    Map->SetIdx(-1);
    Map->SetTarget(GetTarget());
    Map->SetTileMap(this);
    Map->SetImage(true);
    Map->SetSliceMode(SliceMode::END);
    m_vSliceMapImage.push_back(Map);

    m_fSlice = 64.f;

}

TileMapUI::~TileMapUI()
{

    Safe_Del_Vec(m_vTileMap);
    Safe_Del_Vec(m_vSliceMapImage);
}


void TileMapUI::update()
{
    if (nullptr != GetTarget())
    {

        m_pMaterial = GetTarget()->TileMap()->GetCurMaterial(0);
        

    }
    ComponentUI::update();
}

void TileMapUI::render_update()
{   
    ComponentUI::render_update();

    

    // 아틀라스 가져오기
    if (nullptr != GetTarget()->TileMap()->GetTileAtlas())
    {
        Ptr<CTexture> ObjectTex = GetTarget()->TileMap()->GetTileAtlas();
        string Texname = string(ObjectTex.Get()->GetKey().begin(), ObjectTex.Get()->GetKey().end());
        if (ParamUI::Param_Tex(Texname, ObjectTex, this, (FUNC_1)&TileMapUI::SetTexture))
        {
            m_bTilecut = true;
        }
    }
    // 아틀라스가 널포인트일땐 강제로 세팅
    else
    {
        GetTarget()->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
    }


    // tilemap 추가
    vector<tTile> TileInfo = GetTarget()->TileMap()->GetvecTile();

    if (TileInfo.size() != m_vTileMap.size())
    {
        for (int i = 0; i < m_vTileMap.size(); ++i)
        {
            delete m_vTileMap[i];
        }
        m_vTileMap.clear();


        

        //m_vSliceMapImage.push_back(Map);
        for (size_t i = 0; i < TileInfo.size(); ++i)
        {
            SliceMapImage* Map = new SliceMapImage;
            Map->SetImage(GetTarget()->TileMap()->GetTileAtlas());
            Map->SetUV(TileInfo[i].vLeftTop, TileInfo[i].vSlice);
            Map->SetSize(Vec2(64, 64));
            Map->SetIdx((int)i );
            Map->SetTarget(GetTarget());
            Map->SetTileMap(this);
            Map->SetSliceMode(SliceMode::TILEMAP);
            m_fTileCount[0] = GetTarget()->TileMap()->GetTileCount().x;
            m_fTileCount[1] = GetTarget()->TileMap()->GetTileCount().y;
            m_vTileMap.push_back(Map);
        }
    }
    // 타일맵과 동기화
  /*  for (int i = 0; i < TileInfo.size(); ++i)
    {
        m_vTileMap[i]->SetUV(TileInfo[i].vLeftTop, TileInfo[i].vSlice);
    }*/



   ImGuiIO& io = ImGui::GetIO();


   // TIle Mode 
   ImGui::Text("TileMap Mode  "); ImGui::SameLine(); ImGui::Checkbox("##TileMap", &m_bTileMap);
   if (m_bTileMap)
   {
       // 타일맵 이미지 가져오기
       ImGui::Begin("TileMap Image");
       Ptr<CTexture> ObjectTex = GetTarget()->TileMap()->GetTileAtlas();
       
       ImVec2 windowsize = ImGui::GetWindowSize();

       // 텍스트 가로 세로값 가져오기
       float my_tex_w = GetTarget()->Transform()->GetRelativeScale().x;
       float my_tex_h = GetTarget()->Transform()->GetRelativeScale().y;


       GetTarget()->Transform()->GetRelativeScale();
       //ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
       ImVec2 pos = ImGui::GetCursorScreenPos();
       ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
       ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

       
       ImVec2 a = m_vTileMap[0]->GetLeftTop();
       ImVec2 b = m_vTileMap[0]->GetRightDown();
       int Count = (int)GetTarget()->TileMap()->GetTileCount().x;
       // 타일맵과 엔진하고 연동
       for (UINT i = 0; i < m_vTileMap.size(); ++i)
       {
           if ((i) % Count != 0)
           {
                   ImGui::SameLine();
           }
           m_vTileMap[i]->SetUV(TileInfo[i].vLeftTop, TileInfo[i].vSlice);
           m_vTileMap[i]->tile_render();
       }
       ImGui::End();
   }

   //오리지널 이미지
   //ImGui::Text("Original Image"); ImGui::SameLine(); ImGui::Checkbox("##Image", &m_bOriginalImage);
   if (m_bTileMap)
   {
       // ImGuiWindowFlags_NoMove

       ImGui::Begin("DirectX11 Texture", &m_bTileMap);
       Ptr<CTexture> ObjectTex = GetTarget()->TileMap()->GetTileAtlas();

       float my_tex_w = (float)ObjectTex.Get()->GetWidth();
       float my_tex_h = (float)ObjectTex.Get()->GetHeight();

       ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
       
       

       ImVec2 pos = ImGui::GetCursorScreenPos();
       ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
       ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
       ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
       ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
       float zoom = 4.0f;

       // 자르기 모드
       
       ImVec2 nextpos = ImGui::GetCursorScreenPos();
       ImGui::Image((void*)ObjectTex.Get()->GetSRV().Get(), ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

       // 이미지 위에 마우스가 올라올 시
       if (ImGui::IsItemHovered())
       {
           // 드래그 시 이미지 자르기
           float region_sz = m_fSlice;
           float region_x = io.MousePos.x - pos.x - (region_sz ) * 0.5f;
           float region_y = io.MousePos.y - pos.y - (region_sz ) * 0.5f;

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
           if (region_x >= (my_tex_w - (region_sz )))
           {
               region_x += (region_sz / 2);
           }
           if (region_y >= (my_tex_h - (region_sz )))
           {
               region_y += (region_sz / 2);
           }
           ImVec2 uv1 = ImVec2((region_x + (region_sz / 2 )) / my_tex_w, (region_y + (region_sz / 2 )) / my_tex_h);

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


       // 이미지 자르기
       SetGirdSlice(nextpos, m_vSlice);



       // 자른 이미지 띄우기
       ImGui::Image((void*)ObjectTex.Get()->GetSRV().Get(), ImVec2(64.f * zoom, 64.f * zoom), m_vLeftTopMouse, m_vSliceMouse);
       // 만약 자른 이미지를 저장하고 싶으면 버튼 누르기
       if (ImGui::Button("Set##TileSet", Vec2(25.f, 15.f)))
       {
           m_vLeftTop = m_vLeftTopMouse;
           m_vSlice = m_vSliceMouse;
           SliceMapImage* Map = new SliceMapImage;
           Map->SetImage(GetTarget()->TileMap()->GetTileAtlas());
           Map->SetUV1(m_vLeftTop, m_vSliceMouse);
           Map->SetSize(Vec2(64, 64));
           Map->SetIdx(0);
           Map->SetTarget(GetTarget());
           Map->SetTileMap(this);
           Map->SetSliceMode(SliceMode::SLICE);
           m_vSliceMapImage.push_back(Map);
       }


       ImGui::End();
   }
   // 자른 이미지 완전 저장
   //ImGui::Text("Slice Mode    "); ImGui::SameLine(); ImGui::Checkbox("##sliceImage", &m_bSliceImage);
   if (m_bTileMap)
   {
       ImGui::Begin("Slice Image");
       Ptr<CTexture> ObjectTex = GetTarget()->TileMap()->GetTileAtlas();

       float my_tex_w = (float)ObjectTex.Get()->GetWidth();
       float my_tex_h = (float)ObjectTex.Get()->GetHeight();

       ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h); 

       // 잘린 이미지 수정
       ImGui::Text("EditMode  "); ImGui::SameLine(); ImGui::Checkbox("##EditMode", &m_bEditMode);
       ImGui::Text("CurImage");
       ImVec2 Slice;
       Slice.x = m_vSlice.x + m_vLeftTop.x;
       Slice.y = m_vSlice.y + m_vLeftTop.y;
       ImGui::Image((void*)ObjectTex.Get()->GetSRV().Get(), ImVec2(64.f, 64.f), m_vLeftTop, Slice);
      
       int Slice1 = m_vSliceCount.x;
       for (UINT i = 1; i < m_vSliceMapImage.size(); ++i)
       {
           if ((i - 1) % Slice1 != 0)
           {
               ImGui::SameLine();
           }
           m_vSliceMapImage[i]->tile_render();
       }
       ImGui::End();
   }

   // 타일 정보
   ImGui::Text("TileCount     ");  ImGui::SameLine(); ImGui::InputFloat2("##TileCount", m_fTileCount);  
   //ImGui::Text("Slice         ");  ImGui::SameLine(); ImGui::InputFloat("##Slice", &m_fSlice);
  //ImGui::SameLine();
   GetTarget()->TileMap()->GetTileCount();

   int CurCount = (int)(GetTarget()->TileMap()->GetTileCount().x * GetTarget()->TileMap()->GetTileCount().y);
   int Count = (int)(m_fTileCount[0] * m_fTileCount[1]);
   if (CurCount != Count)
   {
       if (GetTarget())
       {
           GetTarget()->TileMap()->SetCount((UINT)m_fTileCount[0], (UINT)m_fTileCount[1]);
       }
   }

   string Computename;
   if (nullptr != m_pMaterial)
   {
       Computename = string(m_pMaterial->GetKey().begin(), m_pMaterial->GetKey().end());
   }

   ImGui::Text("Material##Tilemap ");
   ImGui::SameLine();

   ImGui::InputText("##Tilemaptext", (char*)Computename.data(), Computename.length(), ImGuiInputTextFlags_ReadOnly);
   ImGui::SameLine();

   if (ImGui::Button("##Tilemapbtn", Vec2(15.f, 15.f)))
   {
       ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
       assert(pListUI);

       // 메쉬 목록을 받아와서, ListUI 에 전달
       const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
       static vector<wstring> vecRes;
       vecRes.clear();

       map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
       for (; iter != mapRes.end(); ++iter)
       {
           vecRes.push_back(iter->first);
       }
       pListUI->SetItemList(vecRes);
       pListUI->AddDynamicDBClicked(this, (FUNC_1)&TileMapUI::SetMaterial);

       pListUI->Open();
   }
   Computename;
   if (nullptr != GetTarget()->TileMap()->GetMesh())
   {
       Computename = string(GetTarget()->TileMap()->GetMesh()->GetKey().begin(), GetTarget()->TileMap()->GetMesh()->GetKey().end());
   }

   ImGui::Text("Mesh##Tilemap");
   ImGui::SameLine();

   ImGui::InputText("##MeshTilemap3", (char*)Computename.data(), Computename.length(), ImGuiInputTextFlags_ReadOnly);

   if (ImGui::Button("##MeshTilemap4", Vec2(15.f, 15.f)))
   {
       ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
       assert(pListUI);

       // 메쉬 목록을 받아와서, ListUI 에 전달
       const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MESH);
       static vector<wstring> vecRes;
       vecRes.clear();

       map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
       for (; iter != mapRes.end(); ++iter)
       {
           vecRes.push_back(iter->first);
       }
       pListUI->SetItemList(vecRes);
       pListUI->AddDynamicDBClicked(this, (FUNC_1)&TileMapUI::SetMesh);

       pListUI->Open();
   }



   
}

void TileMapUI::SetGirdSlice(ImVec2 setpos, ImVec2 slicesize)
{
    /*ImGui::Text("Slice"); ImGui::SameLine(); ImGui::InputFloat("##Slice", &m_fSlice);*/
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    Ptr<CTexture> ObjectTex = GetTarget()->TileMap()->GetTileAtlas();
    int widthsize = ObjectTex.Get()->GetWidth() / slicesize.x;
    int height = ObjectTex.Get()->GetHeight() / slicesize.y;
    setpos.x += m_vGridOffset.x;
    setpos.y += m_vGridOffset.y;
    for (int i = 0; i < m_vSliceCount.y; ++i)
    {
        for (int j = 0; j <= m_vSliceCount.x; ++j)
        {
            draw_list->AddRect(ImVec2(setpos.x +(j * slicesize.x), setpos.y), ImVec2(setpos.x + slicesize.x, setpos.y + slicesize.y), IM_COL32(0, 255, 0, 255));
       
        }
        setpos.y += slicesize.y;
    }
    
    ImVec2 lefttop = m_vGridOffset;
    
    ImGui::Text("Slice");  ImGui::SameLine(); ImGui::InputFloat2("##Slice", m_vSlice);
    ImGui::Text("Offset");  ImGui::SameLine(); ImGui::InputFloat2("##Offset", m_vGridOffset);
    ImGui::Text("WHCount");  ImGui::SameLine(); ImGui::InputFloat2("##WHCount", m_vSliceCount);
    if (ImGui::Button("RESET"))
    {
        for (int i = 0; i < m_vSliceMapImage.size(); ++i)
        {
            delete m_vSliceMapImage[i];
        }
        m_vSliceMapImage.clear();
        for (int i = 0; i < m_vSliceCount.y; ++i)
        {
            for (int j = 0; j <= m_vSliceCount.x; ++j)
            {
                /*lefttop = ImVec2( + j * slicesize ,  i * slicesize);*/
                
                //lefttop.y += i * slicesize.y;
                SliceMapImage* Map = new SliceMapImage;
                Map->SetImage(GetTarget()->TileMap()->GetTileAtlas());
                Map->SetLeftSlice(lefttop, slicesize);
                Map->SetSize(Vec2(64, 64));
                Map->SetIdx(0);
                Map->SetTarget(GetTarget());
                Map->SetTileMap(this);
                Map->SetSliceMode(SliceMode::SLICE);
                m_vSliceMapImage.push_back(Map);
                lefttop.x += slicesize.x;
            }
            lefttop.x = m_vGridOffset.x;
            lefttop.y += slicesize.y;
            //setpos.y += slicesize.y;
        }
        m_bTilecut = false;
    }

}

void TileMapUI::SetTexture(DWORD_PTR _strTexKey)
{
    string strKey = (char*)_strTexKey;
    wstring wstrKey = wstring(strKey.begin(), strKey.end());

    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
    assert(nullptr != pTex);

    GetTarget()->TileMap()->SetTileAtlas(pTex);
    for (int i = 0; i < m_vTileMap.size(); ++i)
    {
        // 이미지 동기화
        m_vTileMap[i]->SetImage(pTex);
    }
    
}

void TileMapUI::SetMaterial(DWORD_PTR _strTexKey)
{
    string strKey = (char*)_strTexKey;
    wstring wstrKey = wstring(strKey.begin(), strKey.end());

    GetTarget()->TileMap()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(wstrKey),0);
}

void TileMapUI::SetMesh(DWORD_PTR _strTexKey)
{
    string strKey = (char*)_strTexKey;
    wstring wstrKey = wstring(strKey.begin(), strKey.end());

    GetTarget()->TileMap()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(wstrKey));
}



// 텍스쳐 로딩 시키기
//Ptr<CTexture> texture = CResMgr::GetInst()->FindRes<CTexture>(L"TileTex");

//int width = texture.Get()->GetWidth();
//int height = texture.Get()->GetHeight();

//ImGui::Begin("DirectX11 Texture");
//ImGui::Text("pointer = %p", texture.Get()->GetSRV().Get());
//ImGui::Text("size = %d x %d", width, height);
//// Normalized coordinates of pixel (10,10) in a 256x256 texture.
//// 왼쪽 상단
//ImVec2 uv0 = ImVec2(0 / width, 0 / height);

//// Normalized coordinates of pixel (110,210) in a 256x256 texture.
//// 오른쪽 하단
//ImVec2 uv1 = ImVec2((64.f) / width, (64.f) / height);

//ImGui::Text("LeftTop = (%f, %f)", uv0.x, uv0.y);
//ImGui::Text("RightDown = (%f, %f)", uv1.x, uv1.y);


//ImGui::Image((void*)texture.Get()->GetSRV().Get(), ImVec2(width, height));
//ImGui::End();




