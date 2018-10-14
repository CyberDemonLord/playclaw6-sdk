#pragma once



#include "plugin-template.h"
#include "plugin-api.h"

class CPluginEncoderTemplate : public CPluginTemplate
{
protected:


	virtual BOOL	StartEncoder(unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData) { return FALSE;};
	virtual BOOL	EncodeFrame(const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData) { return FALSE; };
	virtual BOOL	DrainFrames(GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData) { return FALSE; };

public:
	CPluginEncoderTemplate(void* playclawdata) : CPluginTemplate(playclawdata)
	{
	};

	static BOOL StartEncoder(void* instance, unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData);
	static BOOL EncodeFrame(void* instance, const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);
	static BOOL DrainFrames(void* instance, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);

};

#define GH_PLUGIN_VIDEO_ENCODER_CREATE_TEMPLATE_IMGUI(cppclassname)		\
		GH_PLUGIN_CREATE_TEMPLATE_EX(cppclassname)					\
		void	OnImGuiConfigure(void* param) override;						\
		static GH_VIDEO_ENCODER_EXTRA_DATA	videnc_desc;			\


#define GH_PLUGIN_VIDEO_ENCODER_DESC_TEMPLATE(cppclassname, extensionClassID, titleStr, isAvailable, flags)	\
		GH_VIDEO_ENCODER_EXTRA_DATA cppclassname::videnc_desc = {						\
		isAvailable,		\
		CPluginEncoderTemplate::StartEncoder,		\
		CPluginEncoderTemplate::EncodeFrame,		\
		CPluginEncoderTemplate::DrainFrames,		\
		};		\
		GH_PLUGIN_DESC_TEMPLATE_EX(GH_PLUGIN_TYPE_VIDEO_ENCODER, cppclassname, extensionClassID, titleStr, flags, &cppclassname::videnc_desc);	\


