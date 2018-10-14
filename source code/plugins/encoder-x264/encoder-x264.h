#pragma once


class CVideoEncoderX264 : public CPluginEncoderTemplate
{
	HMODULE			m_hX264;


	// config
	string			preset;
	string			rc;
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


/*
	CGuiCombobox	m_cbRateControl;

	// config
	int				keyint, bframes, bitrate, maxbitrate;
	int				async_depth;
	int				accuracy, convergence, icq_quality, la_depth;
	int				qpi, qpp, qpb;
	bool			cabac;

	// realtime
	qsv_t*			m_pEncoder;
	vector<BYTE>	m_Header;
	int				m_iFrameShift;
	INT64			m_DeltaTS;			// if incoming frame has ts=0, then decode ts will be negative :(
	void*			m_lpAllocator;

	unsigned		m_FrameWidth, m_FrameHeight, m_Fps1000;
	UINT64			m_FrameTime;
*/

	void			SetDefaults() override;
	void			LoadFromFile(const Json::Value& config) override;
	void			SaveToFile(Json::Value& config) override;

//	BOOL			ProcessEncodedFrame(mfxBitstream *pBS, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);
	BOOL			StartEncoder(unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData) override;
	BOOL			EncodeFrame(const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);
	BOOL			DrainFrames(GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData);

	void			OnImGuiDraw();

public:
	GH_PLUGIN_VIDEO_ENCODER_CREATE_TEMPLATE_IMGUI(CVideoEncoderX264);

	CVideoEncoderX264(void* playclawdata);
	virtual ~CVideoEncoderX264();

};




