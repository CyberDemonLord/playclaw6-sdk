#pragma once

#include "../plugin-overlay-template.h"


class CSampleOverlay : public CPluginOverlayTemplate
{
	PluginOverlayGdiPlusHelper*			m_pRenderHelper;


	void				SetDefaults() override;
	void				SaveToFile(Json::Value& config) override;
	void				LoadFromFile(const Json::Value& config) override;

	void				UpdateOverlay() override;

public:
	GH_PLUGIN_OVERLAY_CREATE_TEMPLATE_IMGUI(CSampleOverlay);

	CSampleOverlay(void* playclawdata);
	virtual ~CSampleOverlay();
};

