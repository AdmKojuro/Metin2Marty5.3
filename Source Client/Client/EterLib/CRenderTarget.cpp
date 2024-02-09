#include "StdAfx.h"
#include "CRenderTarget.h"
#include "../EterLib/Camera.h"
#include "../EterLib/CRenderTargetManager.h"
#include "../EterPythonLib/PythonGraphic.h"
#include "../UserInterface/Locale_inc.h"

#include "../EterBase/CRC32.h"
#include "../GameLib/GameType.h"
#include "../GameLib/MapType.h"
#include "../GameLib/ItemData.h"
#include "../GameLib/ActorInstance.h"
#include "../UserInterface/InstanceBase.h"
#include "../UserInterface/GameType.h"
#include "../UserInterface/AbstractPlayer.h"


#include "ResourceManager.h"


CRenderTarget::CRenderTarget(const DWORD width, const DWORD height) : m_pModel(nullptr),
	m_background(nullptr),
	m_modelRotation(0),
	m_visible(false) 
{
	auto pTex = new CGraphicRenderTargetTexture;
	if (!pTex->Create(width, height, D3DFMT_X8R8G8B8, D3DFMT_D16)) {
		delete pTex;
		TraceError("CRenderTarget::CRenderTarget: Could not create CGraphicRenderTargetTexture %dx%d", width, height);
		throw std::runtime_error("CRenderTarget::CRenderTarget: Could not create CGraphicRenderTargetTexture");
	}

	m_renderTargetTexture = std::unique_ptr<CGraphicRenderTargetTexture>(pTex);
}

CRenderTarget::~CRenderTarget()
{
}

void CRenderTarget::ChangeArmor(DWORD eArmor)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetArmor(eArmor);
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeHair(DWORD eHair)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetHair(eHair);
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeWeapon(DWORD eWeapon)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetWeapon(eWeapon);
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeAcce(DWORD acce)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetAcce(acce);
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeAura(DWORD eAura)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetAura(eAura);
	//m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeEffect()
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->GetGraphicThingInstanceRef().RenderAllAttachingEffect(); 
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}		

#ifdef ENABLE_SHINING_ITEMS
void CRenderTarget::ChangeWeaponShining(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetShining(0, vnum);
	//m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeArmorShining(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetShining(3, vnum);
	//m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}

void CRenderTarget::ChangeSpecialShining(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	m_pModel->SetShining(5, vnum);
	//m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
}
#endif

void CRenderTarget::SetVisibility(bool isShow)
{
	m_visible = isShow;
}

void CRenderTarget::RenderTexture() const
{
	m_renderTargetTexture->Render();
}

void CRenderTarget::SetRenderingRect(RECT* rect) const
{
	m_renderTargetTexture->SetRenderingRect(rect);
}

void CRenderTarget::CreateTextures() const
{
	m_renderTargetTexture->CreateTextures();
}

void CRenderTarget::ReleaseTextures() const
{
	m_renderTargetTexture->ReleaseTextures();
}

void CRenderTarget::SelectModel(const DWORD index)
{
	if (index == -1)
	//if (index == 0)
	{
		//delete m_pModel;
		m_pModel.reset();
		return;
	}

	CInstanceBase::SCreateData kCreateData{};


	//kCreateData.m_bType = CActorInstance::TYPE_PC; // Dynamic Type
	kCreateData.m_bType = index > 8 ? CActorInstance::TYPE_NPC : CActorInstance::TYPE_PC;
	kCreateData.m_dwRace = index;

	auto model = std::make_unique<CInstanceBase>();
	if (!model->Create(kCreateData))
	{
		if (m_pModel)
		{
			m_pModel.reset();
		}
		return;
	}

	m_pModel = std::move(model);

	m_pModel->NEW_SetPixelPosition(TPixelPosition(0, 0, 0));
	//m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
	m_modelRotation = 0.0f;
	m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
	m_pModel->SetLoopMotion(CRaceMotionData::NAME_WAIT);
	m_pModel->SetAlwaysRender(true);
	m_pModel->SetRotation(0.0f);
	m_modelZoom = 1500.0f;

	CActorInstance& rkActor = m_pModel->GetGraphicThingInstanceRef();
	float fHeight = rkActor.GetHeight();
	m_fEyeY = fHeight;
	m_fZoomY = 0.0f;
	m_fTargetY = 0.0f;
	m_fTargetHeight = m_fEyeY * 8.9f / 140.0f;

	auto& camera_manager = CCameraManager::instance();
	camera_manager.SetCurrentCamera(CCameraManager::SHOPDECO_CAMERA);
	camera_manager.GetCurrentCamera()->SetTargetHeight(110.0);
	camera_manager.ResetToPreviousCamera();
}

void CRenderTarget::SetZoom(bool bZoom)
{
	if (!m_pModel)
		return;

	if (bZoom)
	{
		m_fZoomY = m_fZoomY - m_fTargetHeight;
		float v3 = -(m_fEyeY * 8.9f - m_fEyeY * 3.0f);
		m_fZoomY = fmax(v3, m_fZoomY);
	}
	else
	{
		m_fZoomY = m_fZoomY + m_fTargetHeight;
		float v6 = 14000.0f - m_fEyeY * 8.9f;
		float v7 = m_fEyeY * 8.9f + m_fEyeY * 5.0f;
		m_fZoomY = fmin(m_fZoomY, v6);
		m_fZoomY = fmin(m_fZoomY, v7);
	}
}



bool CRenderTarget::CreateBackground(const char* imgPath, const DWORD width, const DWORD height)
{
	if (m_background)
		return false;

	m_background = std::make_unique<CGraphicImageInstance>();
	
	const auto graphic_image = dynamic_cast<CGraphicImage*>(CResourceManager::instance().GetResourcePointer(imgPath));
	if (!graphic_image)
	{
		m_background.reset();
		return false;
	}

	m_background->SetImagePointer(graphic_image);
	m_background->SetScale(static_cast<float>(width) / graphic_image->GetWidth(), static_cast<float>(height) / graphic_image->GetHeight());
	return true;
}

void CRenderTarget::RenderBackground() const
{
	if (!m_visible)
		return;
	if (!m_background)
		return;

	auto& rectRender = *m_renderTargetTexture->GetRenderingRect();
	m_renderTargetTexture->SetRenderTarget();

	CGraphicRenderTargetTexture::Clear();
	CPythonGraphic::Instance().SetInterfaceRenderState();

	const auto width = static_cast<float>(rectRender.right - rectRender.left);
	const auto height = static_cast<float>(rectRender.bottom - rectRender.top);

	CPythonGraphic::Instance().SetViewport(0.0f, 0.0f, width, height);

	m_background->Render();
	m_renderTargetTexture->ResetRenderTarget();
}

void CRenderTarget::UpdateModel()
{
	if (!m_visible || !m_pModel)
		return;

	if (m_modelRotation < 360.0f)
		m_modelRotation += 1.0f;
	else
		m_modelRotation = 0.0f;

	m_pModel->SetRotation(m_modelRotation);
	m_pModel->Transform();
	m_pModel->GetGraphicThingInstanceRef().RotationProcess();
}

void CRenderTarget::DeformModel() const
{
	if (!m_visible)
		return;

	if (m_pModel)
		m_pModel->Deform();
}

void CRenderTarget::RenderModel() const
{
	if (!m_visible)
	{
		return;
	}

	auto& python_graphic = CPythonGraphic::Instance();
	auto& camera_manager = CCameraManager::instance();
	auto& state_manager = CStateManager::Instance();

	auto& rectRender = *m_renderTargetTexture->GetRenderingRect();

	if (!m_pModel)
	{
		return;
	}

	m_renderTargetTexture->SetRenderTarget();

	if (!m_background)
	{
		m_renderTargetTexture->Clear();
	}


	python_graphic.ClearDepthBuffer();

	const auto fov = python_graphic.GetFOV();
	const auto aspect = python_graphic.GetAspect();
	const auto near_y = python_graphic.GetNear();
	const auto far_y = python_graphic.GetFar();

	const auto width = static_cast<float>(rectRender.right - rectRender.left);
	const auto height = static_cast<float>(rectRender.bottom - rectRender.top);


	state_manager.SetRenderState(D3DRS_FOGENABLE, FALSE);

	python_graphic.SetViewport(0.0f, 0.0f, width, height);

	python_graphic.PushState();
	D3DXVECTOR3 v3Eye(0.0f, m_modelZoom, 300.0f);

	// YMD.2021.01.12.Owsap - Model Height
	// NOTE : Actor's Height / 2 ( ~ mid camera position)
	CActorInstance& rkActor = m_pModel->GetGraphicThingInstanceRef();
	float fActorHeight = rkActor.GetHeight();

	camera_manager.SetCurrentCamera(CCameraManager::SHOPDECO_CAMERA);
	camera_manager.GetCurrentCamera()->SetViewParams(
		D3DXVECTOR3{0.0f, -(m_fEyeY * 8.9f + m_fZoomY), 0.0f},
		D3DXVECTOR3{0.0f, m_fTargetY, fActorHeight / 2},
		D3DXVECTOR3{0.0f, 0.0f, 1.0f}
	);


	python_graphic.UpdateViewMatrix();

	python_graphic.SetPerspective(10.0f, width / height, 100.0f, 15000.0f);

	m_pModel->Render();
	m_pModel->GetGraphicThingInstanceRef().RenderAllAttachingEffect();
	camera_manager.ResetToPreviousCamera();
	python_graphic.RestoreViewport();
	python_graphic.PopState();
	python_graphic.SetPerspective(fov, aspect, near_y, far_y);
	m_renderTargetTexture->ResetRenderTarget();
	state_manager.SetRenderState(D3DRS_FOGENABLE, FALSE);
}
