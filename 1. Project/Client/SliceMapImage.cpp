#include "pch.h"
#include "SliceMapImage.h"

#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>



SliceMapImage::SliceMapImage()
    : m_bImage(true)
    , m_IsEdit(false)
{
}

SliceMapImage::~SliceMapImage()
{
}


void SliceMapImage::tile_render()
{
    if (m_bImage)
    ImGui::Image(OrignalImage.Get()->GetSRV().Get(), m_vSize, m_vLeftTopUV, m_vRightDownUV);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    switch (m_Slicemode)
    {
    case SliceMode::TILEMAP:
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                Vec2 LeftTop = Vec2(m_TileMap->GetLeftTop().x, m_TileMap->GetLeftTop().y);
                Vec2 Slice = Vec2(m_TileMap->GetSlice().x, m_TileMap->GetSlice().y);
                bool Setuv = true;
                //SetLeftSlice(LeftTop, Slice);
                if (LeftTop.x > 0)
                {
                    Setuv = false;
                }

                m_Target->TileMap()->SetIdxTile(m_iIdx, LeftTop, Slice, Setuv);
            }
        }
        break;
    case SliceMode::SLICE:
        if (ImGui::IsItemHovered())
        {

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                //ImVec2 _LeftTop = m_vLeftTopUV;

                //_LeftTop.x *= OrignalImage.Get()->GetWidth();
                //_LeftTop.y *= OrignalImage.Get()->GetHeight();

                ImVec2 a = ImGui::GetCursorPos();

                ImVec2 _Slice = m_vRightDownUV;
                _Slice.x -= m_vLeftTopUV.x;
                _Slice.y -= m_vLeftTopUV.y;



                m_TileMap->SetLeftTop(m_vLeftTopUV);
                m_TileMap->SetSlice(_Slice);
                if (m_TileMap->GetEditMode())
                {
                    m_IsEdit = m_TileMap->GetEditMode();
                }

            }
        }
        break;
    case SliceMode::END:
        break;
    default:
        break;
    }

    
    

    if (m_IsEdit)
    {
        ImGui::Begin("Texture##Edit");


        float my_tex_w = (float)OrignalImage.Get()->GetWidth();
        float my_tex_h = (float)OrignalImage.Get()->GetHeight();

        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

        ImGui::Image((void*)OrignalImage.Get()->GetSRV().Get(), ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
        ImGui::End();

        ImGui::Begin("SliceEditor");


        ImGui::Image((void*)OrignalImage.Get()->GetSRV().Get(), ImVec2(512, 512), m_vLeftTopUV, m_vRightDownUV);
        float LeftTop[2] = {};
        LeftTop[0] = m_vLeftTopUV.x;
        LeftTop[1] = m_vLeftTopUV.y;
        float Slice[2] = {};
        Slice[0] = m_vRightDownUV.x;
        Slice[1] = m_vRightDownUV.y;
        ImGui::Text("LeftTop     ");  ImGui::SameLine(); ImGui::InputFloat2("##LeftTopEdit", LeftTop);
        ImGui::Text("Slice       ");  ImGui::SameLine(); ImGui::InputFloat2("##SliceEdit", Slice);
        m_vLeftTopUV.x = LeftTop[0];
        m_vLeftTopUV.y = LeftTop[1];

        m_vRightDownUV.x = Slice[0];
        m_vRightDownUV.y = Slice[1];

        ImGui::End();

        m_IsEdit = m_TileMap->GetEditMode();
    }

}

void SliceMapImage::SetUV(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV)
{

	
	m_vLeftTopUV = _vLeftTopUV;
	m_vRightDownUV.x = _vLeftTopUV.x + _vRightDownUV.x;
	m_vRightDownUV.y = _vLeftTopUV.y + _vRightDownUV.y;
}

void SliceMapImage::SetUV1(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV)
{
    m_vLeftTopUV = _vLeftTopUV;
    m_vRightDownUV.x = _vRightDownUV.x;
    m_vRightDownUV.y = _vRightDownUV.y;
}

void SliceMapImage::SetLeftSlice(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV)
{
    if (_vLeftTopUV != ImVec2(0, 0))
    {
        _vLeftTopUV.x /= OrignalImage.Get()->GetWidth();
        _vLeftTopUV.y /= OrignalImage.Get()->GetHeight();
    }
    if (_vRightDownUV != ImVec2(0, 0))
    {
        _vRightDownUV.x /= OrignalImage.Get()->GetWidth();
        _vRightDownUV.y /= OrignalImage.Get()->GetHeight();
    }

    m_vLeftTopUV = _vLeftTopUV;
    m_vRightDownUV.x = _vLeftTopUV.x + _vRightDownUV.x;
    m_vRightDownUV.y = _vLeftTopUV.y + _vRightDownUV.y;
}
void SliceMapImage::MulLeftslice(ImVec2 _LeftTop, ImVec2 _Slice)
{
    _LeftTop.x *= OrignalImage.Get()->GetWidth();
    _LeftTop.y *= OrignalImage.Get()->GetHeight();


    _Slice.x *= OrignalImage.Get()->GetWidth();
    _Slice.y *= OrignalImage.Get()->GetHeight();

    m_vLeftTopUV = _LeftTop;
    m_vRightDownUV = _Slice;

}
