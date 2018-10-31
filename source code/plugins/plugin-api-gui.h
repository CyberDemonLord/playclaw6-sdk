#pragma once

#include "plugin-overlay-template.h"

typedef void (WINAPI *GH_GUI_MODAL_DIALOG_PROC)(void* p);
typedef bool (WINAPI *GH_GUI_MODAL_RESULT_CB)(void* p, int result);
struct GH_GUI_MODAL_CONFIG
{
	const char*					id;
	GH_GUI_MODAL_DIALOG_PROC	dlgProc;
	GH_GUI_MODAL_RESULT_CB		dlgResultCB;
	void*						param;
	int							minWidth;
	bool						fadeBack;
};

GAMEHOOK_API void			GH_GuiShowModalDialog(GH_GUI_MODAL_CONFIG* lpGuiModalConfig);
GAMEHOOK_API void			GH_GuiEndModal(int result);
GAMEHOOK_API void			GH_GuiShowSettings(void* playclawdata, GH_GUI_MODAL_CONFIG* lpGuiModalConfig);

GAMEHOOK_API void			GH_GuiMessageBox(const char* title, const char* message, unsigned type, int* piResult);
GAMEHOOK_API void			GH_GuiInputBox(const char* title, const char* message, char* buffer, unsigned bufferSize, int* piResult);

GAMEHOOK_API void			GH_GuiPushItemGrayed(bool condition = true, float alpha = 0.7f);
GAMEHOOK_API void			GH_GuiPopItemGrayed(bool condition = true);
GAMEHOOK_API void			GH_GuiPushItemDisabled(bool condition = true);
GAMEHOOK_API void			GH_GuiPopItemDisabled(bool condition = true);



inline float	GuiCalcIndent(float indent) { return max(indent, (ImGui::GetItemRectSize().x + ImGui::GetFontSize() * 2)); };
inline bool		GuiKeyPressed(int k) { return ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[k], false); };
inline bool		GuiEscapePressed() { return GuiKeyPressed(ImGuiKey_Escape); };



class CGuiCombobox
{
public:
	string			m_sName;
	vector<string>	m_Items;
	vector<INT_PTR>	m_ItemsData;
	int				m_iSelection;		//TODO move under protected
	string			m_sSelection;
	float			m_fWidth;

	CGuiCombobox()
	{
		m_sName = "";
		m_iSelection = -1;
		m_sSelection = "";
		m_Items.clear();
		m_ItemsData.clear();
		m_fWidth = 0;
	}

	bool draw()
	{
		bool selected = false;

		if (m_fWidth != 0)
			ImGui::PushItemWidth(m_fWidth);

		if (ImGui::BeginCombo(m_sName.c_str(), m_sSelection.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (unsigned n = 0; n < m_Items.size(); n++)
			{
				bool is_selected = (n == m_iSelection); // You can store your selection however you want, outside or inside your objects

				if (ImGui::Selectable(m_Items[n].c_str(), is_selected))
				{
					// new selection?
					if (!is_selected)
					{
						is_selected = true;
						m_iSelection = n;
						m_sSelection = m_Items[n];
						selected = true;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			}
			ImGui::EndCombo();
		}

		if (m_fWidth != 0)
			ImGui::PopItemWidth();

		return selected;
	}


	int selectBegin(const char* s)
	{
		m_iSelection = -1;
		m_sSelection = "";
		for (unsigned i = 0; i < m_Items.size(); i++)
		{
			if (strstr(m_Items[i].c_str(), s) == m_Items[i].c_str())
			{
				m_iSelection = i;
				m_sSelection = m_Items[i];
				break;
			}
		}
		return m_iSelection;
	}

	int select(const char* s)
	{
		m_iSelection = -1;
		m_sSelection = "";
		for (unsigned i = 0; i < m_Items.size(); i++)
		{
			if (_stricmp(m_Items[i].c_str(), s) == 0)
			{
				m_iSelection = i;
				m_sSelection = m_Items[i];
				break;
			}
		}
		return m_iSelection;
	}

	int select(const string& s)
	{
		return select(s.c_str());
	}

	int select(unsigned n)
	{
		if (m_Items.size() == 0)
		{
			m_iSelection = -1;
			m_sSelection = "";
			return m_iSelection;
		}

		n = min(m_Items.size() - 1, n);
		m_iSelection = n;
		m_sSelection = m_Items[n];

		return m_iSelection;
	}

	int getsel() const
	{
		return m_iSelection;
	}
};


//////////////////////////////////////////////////////////////////////////


#define GH_GUI_GROUP_BOX_START(id, locstr, border)		ImGui::Text(LocalizeStr8(locstr)); if (ImGui::BeginChild(id, ImVec2(0, 0), border))
#define GH_GUI_GROUP_BOX_END							ImGui::EndChild();
#define GH_GUI_GROUP_BOX_START_EX(id, locstr, border)	{static ImVec2 cp(0, 0); ImGui::Text(locstr); if (ImGui::BeginChild(id, ImVec2(0, cp.y + ImGui::GetStyle().WindowPadding.y), border))
#define GH_GUI_GROUP_BOX_END_EX							cp = ImGui::GetCursorPos(); ImGui::EndChild(); }




//////////////////////////////////////////////////////////////////////////


class CGuiAutoIndent
{
	float			m_fCollapseWidth;
	float			m_fIndent;
	bool			m_bWide;
protected:
	CGuiAutoIndent(float collapseWidth);

	void			autoIndentStart();
	void			autoIndent(bool calculate = true);
	bool			isWindowWide() const;
};


class CPluginOverlayTemplate;

// for internal use
class CGuiTextOverlayTemplate : public CPluginOverlayTemplate, public CGuiAutoIndent
{
	CGuiCombobox					m_cbOutlineCombo;
protected:
	shared_ptr<Gdiplus::Font>		m_MainFont;
	shared_ptr<Gdiplus::SolidBrush>	m_ShadowBrush;
	shared_ptr<Gdiplus::Pen>		m_OutlinePen;
	shared_ptr<Gdiplus::SolidBrush> m_BaseBrush;
	int								m_OutlineType;
	bool							m_bLeftAlign;

	// enable/disable options
	bool							m_bDisableAlign;
	bool							m_bDisableBaseColor;

	CGuiTextOverlayTemplate(void* _playclawdata, float collapseWidth = 0);

	bool							drawRenderOptions();

	void							SetDefaults() override;
	void							SaveToFile(Json::Value& config) override;
	void							LoadFromFile(const Json::Value& config) override;

	virtual void					clearData();
};


//////////////////////////////////////////////////////////////////////////

inline void GuiShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

namespace ImGuiGH
{
	inline void TextLabel(const char* label)
	{
		const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);
		ImGui::PushItemWidth(label_size.x);
		ImGui::LabelText("", label);
		ImGui::PopItemWidth();
	};
};

GAMEHOOK_API void GH_GuiShowSimpleTooltip(const char* str, unsigned delay = 500);
GAMEHOOK_API void GH_GuiShowHoverTooltip(const char* str, unsigned delay = 500);



GAMEHOOK_API	void*	GH_GuiLoadImage(const wchar_t* name, bool nameIsRelative, unsigned* lpWidth, unsigned* lpHeight);
GAMEHOOK_API	void	GH_GuiFreeImage(void* image);