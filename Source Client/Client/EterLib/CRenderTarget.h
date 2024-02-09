#pragma once
#include "../Userinterface/Locale_Inc.h"
#include <memory>
#include "GrpRenderTargetTexture.h"

class CInstanceBase;
class CGraphicImageInstance;

class CRenderTarget
{
	using TCharacterInstanceMap = std::map<DWORD, CInstanceBase*>;
	
	public:
		CRenderTarget(DWORD width, DWORD height);
		~CRenderTarget();


		void SetVisibility(bool isShow);
		void RenderTexture() const;
		void SetRenderingRect(RECT* rect) const;

		void SelectModel(DWORD index);
		void ChangeArmor(DWORD dwVnum);
		void ChangeWeapon(DWORD dwVnum);
		void ChangeHair(DWORD dwVnum);
		void ChangeAcce(DWORD acce);
		void ChangeAura(DWORD eAura);
#ifdef ENABLE_SHINING_ITEMS
		void ChangeWeaponShining(DWORD dwVnum);
		void ChangeArmorShining(DWORD dwVnum);
		void ChangeSpecialShining(DWORD dwVnum);
#endif
		void ChangeEffect();
		bool CreateBackground(const char* imgPath, DWORD width, DWORD height);
		void RenderBackground() const;
		void UpdateModel();
		void DeformModel() const;
		void RenderModel() const;

		void CreateTextures() const;
		void ReleaseTextures() const;
		void SetZoom(bool bZoom);

	private:
		std::unique_ptr<CInstanceBase> m_pModel; 
		std::unique_ptr<CGraphicImageInstance> m_background;
		std::unique_ptr<CGraphicRenderTargetTexture> m_renderTargetTexture;
		float m_modelRotation;
		bool m_visible;
		float m_modelZoom;
		float m_fEyeY;
		float m_fTargetY;
		float m_fTargetHeight;
		float m_fZoomY;


};
