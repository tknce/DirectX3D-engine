#pragma once
#include "TileMapUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>
class CGameObject;
class TileMapUI;


class SliceMapImage 
{
private:
	Ptr<CTexture>	OrignalImage;

	CGameObject*	m_Target;
	TileMapUI*		m_TileMap;

	ImVec2			m_vSize;
	ImVec2			m_vLeftTopUV;
	ImVec2			m_vRightDownUV;

	SliceMode		m_Slicemode;

	int				m_iIdx;

	bool			m_bImage;
	bool			m_IsEdit;
public:

	void tile_render();

	void SetImage(Ptr<CTexture>	_OrignalImage) { OrignalImage = _OrignalImage; }
	void SetTarget(CGameObject* _Target) { m_Target = _Target; }
	void SetTileMap(TileMapUI* _Tilemap) { m_TileMap = _Tilemap; }
	void SetSliceMode(SliceMode _Mode) { m_Slicemode = _Mode; }

	void SetSize(ImVec2 _Size) { m_vSize = _Size; }
	ImVec2 GetSize() { return m_vSize; }

	void SetUV(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV);
	void SetUV1(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV);
	void SetLeftSlice(ImVec2 _vLeftTopUV, ImVec2 _vRightDownUV);
	ImVec2 GetLeftTop() { return m_vLeftTopUV; }

	ImVec2 GetRightDown() { return m_vRightDownUV; }

	void SetIdx(int _idx) { m_iIdx = _idx; }
	void SetLefttopSlice(ImVec2 _LeftTop, ImVec2 _Slice) { m_vLeftTopUV = _LeftTop; m_vRightDownUV = _Slice; }
	
	void MulLeftslice(ImVec2 _LeftTop, ImVec2 _Slice);

	void SetImage(bool _image) { m_bImage = _image; }

public:
	SliceMapImage();
	~SliceMapImage();


};

