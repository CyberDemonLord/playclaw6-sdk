#include "stdafx.h"
#include "plugin-encoder-template.h"


BOOL CPluginEncoderTemplate::StartEncoder(void* instance, unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData)
{
	CPluginEncoderTemplate* baseclass = reinterpret_cast<CPluginEncoderTemplate*> (instance);
	if (!baseclass)
		return FALSE;
	return baseclass->StartEncoder(lpuWidth, lpuHeight, fps1000, lpInitData);
}


BOOL CPluginEncoderTemplate::EncodeFrame(void* instance, const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData)
{
	CPluginEncoderTemplate* baseclass = reinterpret_cast<CPluginEncoderTemplate*> (instance);
	if (!baseclass)
		return FALSE;
	return baseclass->EncodeFrame(lpFrameData, lpEncodedData);
}

BOOL CPluginEncoderTemplate::DrainFrames(void* instance, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData)
{
	CPluginEncoderTemplate* baseclass = reinterpret_cast<CPluginEncoderTemplate*> (instance);
	if (!baseclass)
		return FALSE;
	return baseclass->DrainFrames(lpEncodedData);
}