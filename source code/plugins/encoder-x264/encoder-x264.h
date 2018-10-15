#pragma once


class CVideoEncoderX264 : public CPluginEncoderTemplate
{
	HMODULE			m_hX264;


	// config
	char			preset[64];
	char			rc[64];
	int				num_threads;
	bool			cabac;
	int				bitrate;
	int				buffersize;
	bool			use_cfr;
	int				keyint;
	int				bframes;

	// realtime
	x264_t*			m_lpEncoder;
	x264_param_t	m_Params;
	x264_picture_t	m_InputFrame;
	void*			m_lpAllocator;
	int				m_framessent, m_framesreceived;
	vector<unsigned char>	m_Header;

	bool			Encode(x264_picture_t *pInputFrame, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);


	// imgui
	CGuiCombobox	m_cbRateControl;
	CGuiCombobox	m_cbPreset;

	void			SetDefaults() override;
	void			LoadFromFile(const Json::Value& config) override;
	void			SaveToFile(Json::Value& config) override;

	BOOL			StartEncoder(unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData) override;
	BOOL			EncodeFrame(const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);
	BOOL			DrainFrames(GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);

	void			OnImGuiDraw();

public:
	GH_PLUGIN_VIDEO_ENCODER_CREATE_TEMPLATE_IMGUI(CVideoEncoderX264);

	CVideoEncoderX264(void* playclawdata);
	virtual ~CVideoEncoderX264();

};




