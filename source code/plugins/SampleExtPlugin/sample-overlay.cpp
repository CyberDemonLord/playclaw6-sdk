#include "stdafx.h"
#include "sample-overlay.h"

GH_PLUGIN_OVERLAY_DESC_TEMPLATE(CSampleOverlay, "sample-overlay", "SAMPLE_OVERLAY", 0, 30);

CSampleOverlay::CSampleOverlay(void* playclawdata) : CPluginOverlayTemplate(playclawdata)
{
	m_pRenderHelper = new PluginOverlayGdiPlusHelper(playclawdata);
}

CSampleOverlay::~CSampleOverlay()
{
//	clearData();
	if (m_pRenderHelper)
		delete m_pRenderHelper;
}

void CSampleOverlay::SetDefaults()
{

/*
	m_TextSource = TEXT_SOURCE_DIRECT;
	m_TextValue = L"Sample text";
	m_TextFilePath = L"";

	// prepare combos
	m_cbTextModeCombo.m_sName = "##text-mode";
	m_cbTextModeCombo.m_Items.clear();
	m_cbTextModeCombo.m_Items.push_back(LocalizeStr8("TEXT_DIRECT"));
	m_cbTextModeCombo.m_Items.push_back(LocalizeStr8("TEXT_FILE"));
	m_cbTextModeCombo.select(m_TextSource);
	m_cbTextModeCombo.m_fWidth = -1;
*/

}


void CSampleOverlay::SaveToFile(Json::Value& config)
{

/*
	// specific
	config["mode"] = text_mode_str[m_TextSource];
	jsonSave(m_TextValue, config["direct-text"]);
	jsonSave(m_TextFilePath, config["file-text"]);
*/
}

void CSampleOverlay::LoadFromFile(const Json::Value& config)
{

/*
	//TODO for compatibility, remove in october 2018
	if (!config["right-align"].isNull())
	{
		jsonLoad(m_bLeftAlign, config["right-align"]);
		m_bLeftAlign = !m_bLeftAlign;
	}

	LOADCONFIG(config);

	m_TextSource = TEXT_SOURCE_DIRECT;
	LOADVARDecl(string, text_mode, ["mode"]);
	for (unsigned i = 0; i < TEXT_SOURCE_COUNT; i++)
		if (text_mode == text_mode_str[i])
		{
			m_TextSource = i;
			break;
		}

	m_cbTextModeCombo.select(m_TextSource);
	jsonLoad(m_TextValue, config["direct-text"]);
	jsonLoad(m_TextFilePath, config["file-text"]);

	LOADEND;

	SAFE_DELETE(m_pWatchFile);

*/
}





//////////////////////////////////////////////////////////////////////////



void CSampleOverlay::OnImGuiConfigure(void* param)
{
	/*
	autoIndentStart();

	GH_GUI_GROUP_BOX_START_EX("##render-options", GH_LocalizeStr8(0, "RENDER_OPTIONS"), true)
	{
		drawRenderOptions();
	}
	GH_GUI_GROUP_BOX_END_EX;

	ImGui::Spacing();

	GH_GUI_GROUP_BOX_START("##other-options", "", true)
	{
		// text mode
		ImGui::Text(LocalizeStr8("TEXT_SOURCE"));
		autoIndent();
		if (m_cbTextModeCombo.draw())
		{
			m_TextSource = m_cbTextModeCombo.m_iSelection;
		}

		// draw elements according the mode
		if (m_TextSource == TEXT_SOURCE_DIRECT)
		{
			ImGui::Text(LocalizeStr8("TEXT_VALUE"));

			char dtext[1024];
			strcpy_s(dtext, wchar_to_utf8(m_TextValue).c_str());
			ImGui::PushItemWidth(-1);
			if (ImGui::InputText("##text-overlay-direct-value", dtext, sizeof(dtext)))
			{
				LockVars();
				m_TextValue = utf8_to_wchar(dtext);
				UnlockVars();
			}
			ImGui::PopItemWidth();
		}

		if (m_TextSource == TEXT_SOURCE_FILE)
		{
			ImGui::Text(LocalizeStr8("TEXT_FILE_PATH"));

			// path button
			string pathstr = wchar_to_utf8(m_TextFilePath) + "##text-overlay-path";
			if (ImGui::Button(pathstr.c_str(), ImVec2(-1, 0)))
			{
				wchar_t fname[MAX_PATH] = L"";
				OPENFILENAME ofn = { 0 };
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GH_GetAppWindow();
				ofn.lpstrFilter = L"Text files\0*.txt\0Any file\0*.*\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFile = fname;
				ofn.nMaxFile = sizeof(fname);
				ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
				if (GetOpenFileName(&ofn))
				{
					LockVars();
					m_TextFilePath = fname;
					UnlockVars();
				}
			}
		}
	}
	GH_GUI_GROUP_BOX_END;

	*/
}


//////////////////////////////////////////////////////////////////////////


//
//	Draw overlay
//
void CSampleOverlay::UpdateOverlay()
{
	if (!m_pRenderHelper)
		return;

	// lock overlay image
	auto pLock = m_pRenderHelper->BeginFrame();
	if (!pLock)
		return;

	unsigned sizex = pLock->dwWidth;
	unsigned sizey = pLock->dwHeight;

	Gdiplus::Graphics *pGraphics = m_pRenderHelper->GetGraphics();

	// set options
	pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	pGraphics->Clear(Gdiplus::Color(0));

	pGraphics->FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(255, 200, 100, 255)), 10, 20, 50, 60);


	// fill overlay image
	m_pRenderHelper->EndFrame();
}
