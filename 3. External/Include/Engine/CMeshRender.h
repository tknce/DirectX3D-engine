#pragma once
#include "CRenderComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;


    CLONE(CMeshRender);
public:
    CMeshRender();
    virtual ~CMeshRender();
};

