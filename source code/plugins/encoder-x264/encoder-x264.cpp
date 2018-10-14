#include "stdafx.h"
#include "encoder-x264.h"


BOOL IsAvailableX264();
HMODULE LoadX264();


static void DPrint_x264(void *param, int level, const char *format, va_list args)
{
	char str[1024];
	vsnprintf(str, 1024, format, args);
	GH_DPrint("%s", str);
}


GH_PLUGIN_VIDEO_ENCODER_DESC_TEMPLATE(CVideoEncoderX264, "video-encoder-x264", "x264 software", IsAvailableX264, GH_FLAG_VIDEO_ENCODER_H264);

CVideoEncoderX264::CVideoEncoderX264(void* playclawdata) : CPluginEncoderTemplate(playclawdata)
	, m_lpEncoder(nullptr)
	, m_lpAllocator(nullptr)
	, m_framessent(0)
	, m_framesreceived(0)
{
}

CVideoEncoderX264::~CVideoEncoderX264()
{
}



void CVideoEncoderX264::SetDefaults()
{
	preset = x264_preset_names[0];
	num_threads = X264_THREADS_AUTO;
	cabac = true;
	bitrate = 50000;
	buffersize = 0;
	rc = "CBR";
	use_cfr = true;
	keyint = 2;
	bframes = 0;
/*
	// prepare combos
	m_cbRateControl.m_sName = "##h264-rate-control";
	m_cbRateControl.m_fWidth = -50;
	m_cbRateControl.m_Items.clear();
	auto qsv_platform = qsv_get_cpu_platform();
	for each (const auto& p in qsv_rate_controls)
	{
		if (qsv_platform >= QSV_CPU_PLATFORM_HSW || !p.second)
		{
			string locstr = "RC_";
			locstr += p.first;

			string str = p.first;
			str += " - ";
			str += LocalizeStr8(locstr.c_str());

			m_cbRateControl.m_Items.push_back(str);
		}
	}
*/
}


void CVideoEncoderX264::LoadFromFile(const Json::Value& config)
{
	const Json::Value& x264 = config["x264"];

/*
	jsonLoad(rc, qsv["rc"]);
	jsonLoad(rc, config["force-rc"], false);
	jsonLoad(qpi, qsv["qpi"], false);
	jsonLoad(qpp, qsv["qpp"], false);
	jsonLoad(qpb, qsv["qpb"], false);
	jsonLoad(async_depth, qsv["async-depth"], false);

	jsonLoad(keyint, config["keyint"]);
	jsonLoad(bframes, config["b-frames"]);
	jsonLoad(bitrate, config["bitrate"]);
	jsonLoad(maxbitrate, config["max-bitrate"]);
	jsonLoad(cabac, config["cabac"]);

	for (unsigned i = 0; i < ARRAYSIZE(qsv_rate_controls); i++)
	{
		if (qsv_rate_controls[i].first == rc)
			m_cbRateControl.select(i);
	}

	if (m_cbRateControl.m_iSelection == -1)
		m_cbRateControl.select(0u);
*/
}


void CVideoEncoderX264::SaveToFile(Json::Value& config)
{
	Json::Value& x264 = config["x264"];

/*
	jsonSave(rc, qsv["rc"]);
	jsonSave(qpi, qsv["qpi"]);
	jsonSave(qpp, qsv["qpp"]);
	jsonSave(qpb, qsv["qpb"]);
	jsonSave(async_depth, qsv["async-depth"]);

	jsonSave(keyint, config["keyint"]);
	jsonSave(bframes, config["b-frames"]);
	jsonSave(bitrate, config["bitrate"]);
	jsonSave(maxbitrate, config["max-bitrate"]);
	jsonSave(cabac, config["cabac"]);
*/
}

#pragma region	x264 Imgui Configuration

void CVideoEncoderX264::OnImGuiDraw()
{
	/*
	bool opt1 = true;
	bool opt2 = true;
	ImGui::SetNextTreeNodeOpen(opt1, ImGuiCond_FirstUseEver);
	opt1 = ImGui::CollapsingHeader(LocalizeStr8("H264_SEQ_CTRL"));
	if (opt1)
	{
		ImGui::PushItemWidth(120);	// for InputInts

		ImGui::Spacing();
		// key int
		ImGui::Text(LocalizeStr8("H264_KEYINT"));
		ImGui::InputInt("##h264-keyint", &keyint);
		keyint = min(max(1, keyint), 100);

		// b-frames
		ImGui::Text(LocalizeStr8("H264_QSV_BFRAMES"));
		ImGui::InputInt("##h264-bframes", &bframes);
		bframes = min(max(0, bframes), 10);

		ImGui::PopItemWidth();
	}

	ImGui::Spacing();

	ImGui::SetNextTreeNodeOpen(opt2, ImGuiCond_FirstUseEver);
	opt2 = ImGui::CollapsingHeader(LocalizeStr8("H264_BITRATE_CTRL"));
	if (opt2)
	{
		ImGui::Spacing();

		// rate control
		ImGui::Text(LocalizeStr8("H264_BITRATE_METHOD"));
		if (m_cbRateControl.draw())
		{
			rc = qsv_rate_controls[m_cbRateControl.m_iSelection].first;
		}

		ImGui::PushItemWidth(120);	// for InputInts

									// bitrate
		if (!(rc == H264_QSV_RATE_CONTROL_LA || rc == H264_QSV_RATE_CONTROL_LAICQ || rc == H264_RATE_CONTROL_CQP))
		{
			ImGui::Text(LocalizeStr8("H264_BITRATE"));
			ImGui::InputInt("##h264-bitrate", &bitrate);
			bitrate = min(max(bitrate, 100), 200000);
		}

		// max bitrate
		if (rc == H264_RATE_CONTROL_VBR || rc == H264_QSV_RATE_CONTROL_VCM)
		{
			ImGui::Text(LocalizeStr8("H264_MAX_BITRATE"));
			ImGui::InputInt("##h264-max-bitrate", &maxbitrate);
			maxbitrate = min(max(maxbitrate, 100), 200000);
		}

		// async depth
		ImGui::Text(LocalizeStr8("H264_QSV_ASYNC"));
		ImGui::InputInt("##h264-qsv-async-depth", &async_depth);
		async_depth = min(max(async_depth, 1), 7);

		// accuracy
		if (rc == H264_QSV_RATE_CONTROL_AVBR)
		{
			ImGui::Text(LocalizeStr8("H264_ACC"));
			ImGui::InputInt("##h264-accuracy", &accuracy);
			//			maxbitrate = min(max(maxbitrate, 100), 200000);
			ImGui::Text(LocalizeStr8("H264_CONV"));
			ImGui::InputInt("##h264-convergence", &convergence);
		}

		// QP
		if (rc == H264_RATE_CONTROL_CQP)
		{
			ImGui::Text("QPI, QPP, QPB (0-51)");
			int qp[3] = { qpi, qpp, qpb };
			ImGui::InputInt3("##h264-qp", qp);
			qpi = max(min(qp[0], 51), 0);
			qpp = max(min(qp[1], 51), 0);
			qpb = max(min(qp[2], 51), 0);
		}

		// LA ICQ
		if (rc == H264_QSV_RATE_CONTROL_LAICQ || rc == H264_QSV_RATE_CONTROL_ICQ)
		{
			ImGui::Text(LocalizeStr8("H264_ICQ"));
			ImGui::InputInt("##h264-icq", &icq_quality);
			ImGui::Text(LocalizeStr8("H264_LA"));
			ImGui::InputInt("##h264-ladepth", &la_depth);
		}

		ImGui::PopItemWidth();
	}

	*/
}

void CVideoEncoderX264::OnImGuiConfigure(void* param)
{
	static int* lpResult = NULL;
	lpResult = (int*)param;

	auto gui_draw = [](void*p)
	{
		CVideoEncoderX264* data = (CVideoEncoderX264*)p;
		data->OnImGuiDraw();
	};

	auto gui_result = [](void*p, int res) -> bool
	{
		CVideoEncoderX264* data = (CVideoEncoderX264*)p;
		if (lpResult)
			*lpResult = res;
		return true;
	};

	GH_GUI_MODAL_CONFIG c = {};
	c.id = GH_LocalizeStr8(m_playclawdata, CVideoEncoderX264::plugin_desc.szTitleStr);
	c.dlgProc = gui_draw;
	c.dlgResultCB = gui_result;
	c.param = this;
	c.fadeBack = true;
	c.minWidth = 400;
	GH_GuiShowSettings(m_playclawdata, &c);
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

static void CheckResolution(int& width, int& height)
{
}

//
//	Convert float framerate to integer
//
static void ConvertFrameRate(double dFrameRate, unsigned int* pnFrameRateExtN, unsigned int* pnFrameRateExtD)
{
	unsigned int fr;

	fr = (unsigned int)(dFrameRate + .5);

	if (fabs(fr - dFrameRate) < 0.0001)
	{
		*pnFrameRateExtN = fr;
		*pnFrameRateExtD = 1;
		return;
	}

	fr = (unsigned int)(dFrameRate * 1.001 + .5);

	if (fabs(fr * 1000 - dFrameRate * 1001) < 10)
	{
		*pnFrameRateExtN = fr * 1000;
		*pnFrameRateExtD = 1001;
	}

	*pnFrameRateExtN = (unsigned int)(dFrameRate * 10000 + .5);
	*pnFrameRateExtD = 10000;
}


#define X264_DECLARE_PROC(proc) static decltype(&x264_##proc)		X264_##proc = 0;
#define X264_GET_PROC(proc) X264_##proc =		(decltype(&x264_##proc))	GetProcAddress(m_hX264, "x264_"#proc""); if (X264_##proc == 0) { GH_Print("ERROR: Cannot get proc x264_" #proc ); return false;};

X264_DECLARE_PROC(param_default_preset);
X264_DECLARE_PROC(encoder_open);
X264_DECLARE_PROC(param_apply_profile);
X264_DECLARE_PROC(encoder_close);
X264_DECLARE_PROC(picture_init);
X264_DECLARE_PROC(picture_alloc);
X264_DECLARE_PROC(picture_clean);
X264_DECLARE_PROC(encoder_encode);


BOOL CVideoEncoderX264::StartEncoder(unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA* lpInitData)
{
	m_hX264 = LoadX264();
	if (!m_hX264)
	{
		unsigned er = GetLastError();
		GH_Print("ERROR: Cannot load libx264.dll: %d", er);
	}

	X264_GET_PROC(param_default_preset);
	X264_GET_PROC(param_apply_profile);
	X264_GET_PROC(encoder_close);
	X264_GET_PROC(picture_init);
	X264_GET_PROC(picture_alloc);
	X264_GET_PROC(picture_clean);
	X264_GET_PROC(encoder_encode);

	char str[128];
	sprintf_s(str, "x264_encoder_open_%d", X264_BUILD);
	X264_encoder_open = (decltype(&x264_encoder_open))GetProcAddress(m_hX264, str);
	if (X264_encoder_open == 0)
	{
		GH_Print("ERROR: Cannot get proc %s", str);
		return 0;
	}

	// we need nv12 data on input
	m_lpAllocator = lpInitData->lpAllocator;
	lpInitData->colorSpace = GH_VIDEO_ENCODER_EXTRA_DATA::INIT_DATA::COLOR_NV12;
	lpInitData->allFrames = true;


	// init default params
	m_Params.pf_log = DPrint_x264;
//	m_Params.p_log_private = this;
	m_Params.i_log_level = X264_LOG_WARNING;//*/X264_LOG_DEBUG;

	if (preset.empty())
		preset = x264_preset_names[0];
	X264_param_default_preset(&m_Params, preset.c_str(), "");

	m_Params.b_deterministic = false;		// optimizations
	m_Params.i_threads = max(num_threads, X264_THREADS_AUTO);	// num threads
	m_Params.b_cabac = cabac;				// use cabac
	
	m_Params.rc.i_bitrate = bitrate;
	m_Params.rc.i_vbv_max_bitrate = bitrate;
	m_Params.rc.i_vbv_buffer_size = buffersize;

	if (rc == "CBR")
	{
		m_Params.rc.i_rc_method = X264_RC_ABR;
		m_Params.rc.f_rf_constant = 0.0f;
	}
	else
	{
		const float baseCRF = 22.0f;
		int quality = 8;	// by default?

		m_Params.rc.i_rc_method = X264_RC_CRF;
		m_Params.rc.f_rf_constant = baseCRF + float(10 - quality);
	}
	
	m_Params.b_vfr_input = !use_cfr;		// constant frame rate
	m_Params.i_width = *lpuWidth;
	m_Params.i_height = *lpuHeight;
	CheckResolution(m_Params.i_width, m_Params.i_height);
	m_Params.i_keyint_max = keyint * fps1000 / 1000;	// key interval
	m_Params.i_bframe = bframes;		// b-frames
	m_Params.b_open_gop = 0;			// closed GOP is required for YT streaming

	// frame rate
	ConvertFrameRate((double)fps1000 / 1000.0, &m_Params.i_fps_num, &m_Params.i_fps_den);
	m_Params.i_timebase_num = 1;
	m_Params.i_timebase_den = 1000000;	// microseconds?
	m_Params.i_csp = X264_CSP_NV12;						// color space

	// print config
	GH_DPrint("  Frame rate          : %.3f", float(fps1000) / 1000.0f);
	GH_DPrint("  Preset              : %s", preset.c_str());
	GH_DPrint("  Rate control        : %s", rc.c_str());
	GH_DPrint("  Bitrate             : %d", m_Params.rc.i_bitrate);
	GH_DPrint("  I-frames interval   : %d", m_Params.i_keyint_max);
	GH_DPrint("  B-frames interval   : %d", m_Params.i_bframe);
	GH_DPrint("  CABAC               : %s", cabac ? "ON" : "OFF");
	GH_DPrint("--------------------------");


	//x264_profile_names
	if (X264_param_apply_profile(&m_Params, "high"))
	{
		//	failed to set x264 profile
	}

	m_lpEncoder = X264_encoder_open(&m_Params);
	if (!m_lpEncoder)
	{
		GH_Print("ERROR: Cannot open x264 encoder");
		return false;
	}


	X264_picture_init(&m_InputFrame);
	X264_picture_alloc(&m_InputFrame, X264_CSP_NV12, m_Params.i_width, m_Params.i_height);

	return TRUE;
}



BOOL CVideoEncoderX264::EncodeFrame(const GH_VIDEO_ENCODER_EXTRA_DATA::FRAME_DATA* lpFrameData, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData)
{
	m_framesreceived++;

	// prepare frame
	m_InputFrame.i_pts = lpFrameData->timestamp;
	m_InputFrame.i_type = X264_TYPE_AUTO;// bKeyframe ? X264_TYPE_IDR : X264_TYPE_AUTO;

	memcpy(m_InputFrame.img.plane[0], lpFrameData->lpData, m_Params.i_width * m_Params.i_height);

	unsigned ybuffer = m_Params.i_width * m_Params.i_height;
	ybuffer = (ybuffer + 15) & (~15);	// align size to 16
	memcpy(m_InputFrame.img.plane[1], lpFrameData->lpData + ybuffer, m_Params.i_width * m_Params.i_height / 2);

	return Encode(&m_InputFrame, lpEncodedData);
}


BOOL CVideoEncoderX264::DrainFrames(GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData)
{
	if (!m_lpEncoder)
		return false;

	// flush all frames from encoder
	if (m_framesreceived != m_framessent)
	{
		if (Encode(nullptr, lpEncodedData))
			return true;
	}

	// close encoder
	if (m_lpEncoder)
		X264_encoder_close(m_lpEncoder);
	m_lpEncoder = nullptr;

	// clear image
	X264_picture_clean(&m_InputFrame);

	// free library
	if (m_hX264)
		FreeLibrary(m_hX264);
	m_hX264 = 0;

	return FALSE;
}


// encode frame
bool CVideoEncoderX264::Encode(x264_picture_t *pInputFrame, GH_VIDEO_ENCODER_EXTRA_DATA::ENCODED_DATA* lpEncodedData)
{
	// output data
	x264_picture_t OutputFrame;
	x264_nal_t *lpNalOut = 0;
	int nalNum;

	int ret = X264_encoder_encode(m_lpEncoder, &lpNalOut, &nalNum, pInputFrame, &OutputFrame);
	if (ret < 0)
		return false;

	if (nalNum == 0)
		return true;

	// combine NALs in one buffer if required
	unsigned char* lpEncoded = lpNalOut[0].p_payload;
	unsigned encodedSize = lpNalOut[0].i_payload;
	bool combine = false;
	for (int i = 0; i < nalNum - 1; i++)
	{
		if (lpNalOut[i].p_payload + lpNalOut[i].i_payload != lpNalOut[i + 1].p_payload)
		{
			combine = true;
			break;
		}
		encodedSize += lpNalOut[i + 1].i_payload;
	}

	if (combine)
	{
		static vector<unsigned char>	encodedData;
		unsigned filled = 0;
		for (int i = 0; i < nalNum; i++)
		{
			if (encodedData.size() < filled + lpNalOut[i].i_payload)
				encodedData.resize(filled + lpNalOut[i].i_payload);
			memcpy(encodedData.data() + filled, lpNalOut[i].p_payload, lpNalOut[i].i_payload);
			filled += lpNalOut[i].i_payload;
		}
		lpEncoded = encodedData.data();
		encodedSize = filled;
	}

	// apply h264 annex B
	unsigned dataSize = 0;
	unsigned priority;
	unsigned char* lpData = GH_H264BeginAnnexB(lpEncoded, encodedSize, 0, -1, &priority, &dataSize);

	if (m_Header.empty())
	{
		unsigned char headertmp[2048];
		unsigned headerSize = GH_H264PrepareVideoHeader(lpData, dataSize, headertmp);
		m_Header.assign(headertmp, headertmp + headerSize);
	}

	// new sample
	lpEncodedData->lpMediaSample = GH_VideoEncoderCreateSample(m_lpAllocator, lpData, dataSize);
	GH_H264EndAnnexB();

	// fill
	lpEncodedData->timestamp = OutputFrame.i_pts;
	lpEncodedData->framenum = m_framessent; // unfortunately, hack. we cannot store framenum information of previously frame sent to encoder
	lpEncodedData->h264 = true;
	lpEncodedData->keyFrame = OutputFrame.b_keyframe != 0;// priority == NAL_PRIORITY_HIGHEST;
	lpEncodedData->priority = priority;
	if (lpEncodedData->keyFrame)
	{
		lpEncodedData->lpPrivateData = m_Header.data();
		lpEncodedData->privateDataSize = m_Header.size();
	}

	m_framessent++;

	return true;
}