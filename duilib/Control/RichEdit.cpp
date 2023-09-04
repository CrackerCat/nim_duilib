#include "RichEdit.h"
#include "RichEditHost.h"
#include "RichEditCtrl.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/Window.h"
#include "duilib/Core/ScrollBar.h"
#include "duilib/Utils/StringUtil.h"
#include "duilib/Utils/Macros.h"
#include "duilib/Utils/OnScreenKeyboardManager.h"
#include "duilib/Utils/AttributeUtil.h"
#include "duilib/Render/IRender.h"
#include "duilib/Render/AutoClip.h"
#include "duilib/Animation/AnimationManager.h"
#include "duilib/Animation/AnimationPlayer.h"
#include "base/win32/path_util.h"
#include "base/thread/thread_manager.h"

namespace ui {

RichEdit::RichEdit() : 
	ScrollBox(new Layout),
	m_pRichHost(nullptr), 
	m_bVScrollBarFixing(false), 
	m_bWantTab(true),
	m_bNeedReturnMsg(false),
	m_bReturnMsgWantCtrl(false),
	m_bInited(false),
	m_bAllowPrompt(false),
	m_bSelAllEver(false),		 
	m_bNoSelOnKillFocus(true), 
	m_bSelAllOnFocus(false),	
	m_bNoCaretReadonly(false),
	m_bIsCaretVisiable(false),
	m_bIsComposition(false),
	m_iCaretPosX(0),
	m_iCaretPosY(0),
	m_iCaretWidth(0),
	m_iCaretHeight(0),
	m_sFontId(),
	m_sCurrentColor(),
	m_sTextColor(),
	m_sDisabledTextColor(),
	m_sPromptColor(),
	m_sPromptText(),
	m_drawCaretFlag(),
	m_timeFlagMap(),
	m_linkInfo(),
	m_pFocusedImage(nullptr)
{
	m_sCurrentColor = GlobalManager::Instance().Color().GetDefaultTextColor();
	m_sTextColor = m_sCurrentColor;
	m_sDisabledTextColor = m_sCurrentColor;

	//这个标记必须为false，否则绘制有问题
	SetUseCache(false);

	//创建RichEditHost接口
	m_pRichHost = new RichEditHost(this);
	ASSERT(m_pRichHost->GetTextServices() != nullptr);
	m_richCtrl.SetTextServices(m_pRichHost->GetTextServices());
}

RichEdit::~RichEdit()
{
    if( m_pRichHost != nullptr) {
		m_richCtrl.SetTextServices(nullptr);
        m_pRichHost->Release();
		m_pRichHost = nullptr;
    }
	if (m_pFocusedImage != nullptr) {
		delete m_pFocusedImage;
		m_pFocusedImage = nullptr;
	}
}

bool RichEdit::IsWantTab() const
{
    return m_bWantTab;
}

void RichEdit::SetWantTab(bool bWantTab)
{
    m_bWantTab = bWantTab;
}

bool RichEdit::CanPlaceCaptionBar() const
{
	return true;
}

bool RichEdit::IsNeedReturnMsg()
{
    return m_bNeedReturnMsg;
}

void RichEdit::SetNeedReturnMsg(bool bNeedReturnMsg)
{
    m_bNeedReturnMsg = bNeedReturnMsg;
}

bool RichEdit::IsReturnMsgWantCtrl()
{
    return m_bReturnMsgWantCtrl;
}

void RichEdit::SetReturnMsgWantCtrl(bool bReturnMsgWantCtrl)
{
    m_bReturnMsgWantCtrl = bReturnMsgWantCtrl;
}

bool RichEdit::IsRichText()
{
	bool bRich = false;
	if (m_pRichHost != nullptr) {
		bRich = m_pRichHost->IsRichText();
	}
	return bRich;
}

void RichEdit::SetRichText(bool bRich)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetRichText(bRich);
	}
}

bool RichEdit::IsReadOnly()
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsReadOnly();
	}
    return false;
}

void RichEdit::SetReadOnly(bool bReadOnly)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetReadOnly(bReadOnly);
	}
}

bool RichEdit::IsPassword()
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsPassword();
	}
	return false;
}

void RichEdit::SetPassword(bool bPassword )
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetPassword(bPassword);
	}
}

void RichEdit::SetShowPassword(bool bShow)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetShowPassword(bShow);
	}
}

bool RichEdit::IsShowPassword() const
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsShowPassword();
	}
	return false;
}

void RichEdit::SetPasswordChar(wchar_t ch)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetPasswordChar(ch);
	}
}

void RichEdit::SetFlashPasswordChar(bool bFlash)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetFlashPasswordChar(bFlash);
	}
}

bool RichEdit::GetFlashPasswordChar() const
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsFlashPasswordChar();
	}
	return false;
}

bool RichEdit::IsNumberOnly() const
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsNumberOnly();
	}
	return false;
}

void RichEdit::SetNumberOnly(bool bNumberOnly)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetNumberOnly(bNumberOnly);
	}
}

bool RichEdit::GetWordWrap()
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsWordWrap();
	}
	return false;
}

void RichEdit::SetWordWrap(bool bWordWrap)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetWordWrap(bWordWrap);
	}
}

bool RichEdit::GetMultiLine() const
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsMultiLine();
	}
	return false;
}

void RichEdit::SetMultiLine(bool bMultiLine)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetMultiLine(bMultiLine);
	}
}

std::wstring RichEdit::GetFontId() const
{
    return m_sFontId.c_str();
}

void RichEdit::SetFontId(const std::wstring& strFontId)
{
	if (m_sFontId != strFontId) {
		m_sFontId = strFontId;
		if (m_pRichHost != nullptr) {
			m_pRichHost->SetFontId(strFontId);
		}
	}
}

void RichEdit::SetTextColor(const std::wstring& dwTextColor)
{
	if (m_sCurrentColor != dwTextColor) {
		m_sCurrentColor = dwTextColor;
		UiColor dwTextColor2 = GetUiColor(dwTextColor);
		if (m_pRichHost) {
			m_pRichHost->SetColor(dwTextColor2.GetARGB());
		}
	}
}

std::wstring RichEdit::GetTextColor()
{
	return m_sCurrentColor.c_str();
}

UiColor RichEdit::GetTextColorValue()
{
	return GetUiColor(m_sCurrentColor.c_str());
}

int32_t RichEdit::GetLimitText() const
{
	return m_richCtrl.GetLimitText();
}

void RichEdit::SetLimitText(int32_t iChars)
{
	if (iChars < 0) {
		iChars = 0;
	}
	m_richCtrl.SetLimitText(iChars);
}

bool RichEdit::GetAllowBeep() const
{
	if (m_pRichHost != nullptr) {
		return m_pRichHost->IsAllowBeep();
	}
	return false;
}

void RichEdit::SetAllowBeep(bool bAllowBeep)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetAllowBeep(bAllowBeep);
	}
}

int32_t RichEdit::GetTextLength() const
{
	return m_richCtrl.GetTextLengthEx(GTL_DEFAULT, 1200);
}

std::wstring RichEdit::GetText() const
{
	int32_t nTextLen = GetTextLength();
	if (nTextLen < 1) {
		return std::wstring();
	}
	nTextLen += 1;
    wchar_t* pText = new wchar_t[nTextLen];
	memset(pText, 0, sizeof(wchar_t) * nTextLen);
	m_richCtrl.GetTextEx(pText, nTextLen, GTL_DEFAULT, 1200);
	std::wstring sText(pText);
	delete[] pText;
	pText = nullptr;
    return sText;
}

std::string RichEdit::GetUTF8Text() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetText(), strOut, CP_UTF8);
	return strOut;
}

void RichEdit::SetText(const std::wstring& strText)
{
	SetSel(0, -1);
	ReplaceSel(strText, FALSE);
	m_linkInfo.clear();
}

void RichEdit::SetTextId(const std::wstring& strTextId)
{
	std::wstring strText = GlobalManager::Instance().Lang().GetStringViaID(strTextId);
	SetText(strText);
}

void RichEdit::SetUTF8Text( const std::string& strText )
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	SetText(strOut);
}

bool RichEdit::GetModify() const
{ 
	return m_richCtrl.GetModify();
}

void RichEdit::SetModify(bool bModified)
{ 
	m_richCtrl.SetModify(bModified);
}

void RichEdit::GetSel(CHARRANGE& cr) const
{ 
	m_richCtrl.GetSel(cr);
}

void RichEdit::GetSel(long& nStartChar, long& nEndChar) const
{
	m_richCtrl.GetSel(nStartChar, nEndChar);
}

int RichEdit::SetSel(CHARRANGE& cr)
{ 
	return m_richCtrl.SetSel(cr);
}

int RichEdit::SetSel(long nStartChar, long nEndChar)
{
	return m_richCtrl.SetSel(nStartChar, nEndChar);
}

void RichEdit::ReplaceSel(const std::wstring& lpszNewText, bool bCanUndo)
{
	m_richCtrl.ReplaceSel(lpszNewText.c_str(), bCanUndo);
}

std::wstring RichEdit::GetSelText() const
{
	std::wstring text;
	m_richCtrl.GetSelText(text);
	return text;
}

int RichEdit::SetSelAll()
{
	return m_richCtrl.SetSelAll();
}

int RichEdit::SetSelNone()
{
	return m_richCtrl.SetSelNone();
}

bool RichEdit::GetZoom(int& nNum, int& nDen) const
{
	return m_richCtrl.GetZoom(nNum, nDen);
}

bool RichEdit::SetZoom(int nNum, int nDen)
{
	return m_richCtrl.SetZoom(nNum, nDen);
}

bool RichEdit::SetZoomOff()
{
	return m_richCtrl.SetZoomOff();
}

WORD RichEdit::GetSelectionType() const
{
	return m_richCtrl.GetSelectionType();
}

LONG RichEdit::FindRichText(DWORD dwFlags, FINDTEXT& ft) const
{
	return m_richCtrl.FindText(dwFlags, ft);
}

LONG RichEdit::FindRichText(DWORD dwFlags, FINDTEXTEX& ft) const
{
	return m_richCtrl.FindText(dwFlags, ft);
}

bool RichEdit::GetAutoURLDetect() const
{
	return m_richCtrl.GetAutoURLDetect();
}

bool RichEdit::SetAutoURLDetect(bool bAutoDetect)
{
	if (bAutoDetect) {
		ASSERT(m_richCtrl.GetEventMask() & ENM_LINK);
	}
	return m_richCtrl.SetAutoURLDetect(bAutoDetect ? TRUE : FALSE);
}

DWORD RichEdit::GetEventMask() const
{
	return m_richCtrl.GetEventMask();
}

DWORD RichEdit::SetEventMask(DWORD dwEventMask)
{
	return m_richCtrl.SetEventMask(dwEventMask);
}

std::wstring RichEdit::GetTextRange(long nStartChar, long nEndChar) const
{
    TEXTRANGEW tr = { 0 };
    tr.chrg.cpMin = nStartChar;
    tr.chrg.cpMax = nEndChar;
    LPWSTR lpText = NULL;
	const int32_t nLen = nEndChar - nStartChar + 1;
	if (nLen < 1) {
		return std::wstring();
	}
    lpText = new WCHAR[nLen];
	if (lpText == nullptr) {
		return std::wstring();
	}
    ::ZeroMemory(lpText, nLen * sizeof(WCHAR));
    tr.lpstrText = lpText;
	m_richCtrl.GetTextRange(&tr);
    std::wstring sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

void RichEdit::HideSelection(bool bHide, bool bChangeStyle)
{
	m_richCtrl.HideSelection(bHide, bChangeStyle);
}

void RichEdit::ScrollCaret()
{
	m_richCtrl.ScrollCaret();
}

int RichEdit::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
{
	return m_richCtrl.InsertText(nInsertAfterChar, lpstrText, bCanUndo);
}

int RichEdit::AppendText(const std::wstring& strText, bool bCanUndo)
{
	return m_richCtrl.AppendText(strText.c_str(), bCanUndo);
}

DWORD RichEdit::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
	return m_richCtrl.GetDefaultCharFormat(cf);
}

bool RichEdit::SetDefaultCharFormat(CHARFORMAT2& cf)
{
	if (m_richCtrl.SetDefaultCharFormat(cf)) {
		if (m_pRichHost != nullptr) {
			CHARFORMAT2 newCf;
			GetDefaultCharFormat(newCf);
			m_pRichHost->SetCharFormat(newCf);
		}
		return true;
	}
    return false;
}

DWORD RichEdit::GetSelectionCharFormat(CHARFORMAT2& cf) const
{
	return m_richCtrl.GetSelectionCharFormat(cf);
}

bool RichEdit::SetSelectionCharFormat(CHARFORMAT2& cf)
{
	return m_richCtrl.SetSelectionCharFormat(cf);
}

bool RichEdit::SetWordCharFormat(CHARFORMAT2 &cf)
{
	if (m_richCtrl.SetCharFormat(cf, SCF_SELECTION | SCF_WORD)) {
		if (m_pRichHost != nullptr) {
			CHARFORMAT2 newCf;
			GetDefaultCharFormat(newCf);
			m_pRichHost->SetCharFormat(newCf);
		}
		return true;
	}
	return false;
}

DWORD RichEdit::GetParaFormat(PARAFORMAT2& pf) const
{
	return m_richCtrl.GetParaFormat(pf);
}

bool RichEdit::SetParaFormat(PARAFORMAT2 &pf)
{
	if (m_richCtrl.SetParaFormat(pf)) {
		if (m_pRichHost != nullptr) {
			PARAFORMAT2 newPf;
			GetParaFormat(newPf);
			m_pRichHost->SetParaFormat(newPf);
		}
		return true;
	}
	return false;
}

bool RichEdit::Redo()
{
	return m_richCtrl.Redo();
}

bool RichEdit::Undo()
{
	return m_richCtrl.Undo();
}

void RichEdit::Clear()
{
	m_richCtrl.Clear();
}

void RichEdit::Copy()
{
	m_richCtrl.Copy();
}

void RichEdit::Cut()
{
	m_richCtrl.Cut();
}

void RichEdit::Paste()
{
	m_richCtrl.Paste();
}

BOOL RichEdit::CanPaste(UINT nFormat/* = 0*/)
{
	return m_richCtrl.CanPaste(nFormat);
}

void RichEdit::PasteSpecial(UINT uClipFormat, DWORD dwAspect/* = 0*/, HMETAFILE hMF/* = 0*/)
{
	return m_richCtrl.PasteSpecial(uClipFormat, dwAspect, hMF);
}

int RichEdit::GetLineCount() const
{
	return m_richCtrl.GetLineCount();
}

std::wstring RichEdit::GetLine(int nIndex, int nMaxLength) const
{
    LPWSTR lpText = nullptr;
	if (nMaxLength < 1) {
		return std::wstring();
	}
    lpText = new WCHAR[nMaxLength + 1];
	if (lpText == nullptr) {
		return std::wstring();
	}
    ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
    *(LPWORD)lpText = (WORD)nMaxLength;
	m_richCtrl.GetLine(nIndex, lpText);
    std::wstring sText = lpText;
    delete[] lpText;
    return sText;
}

int RichEdit::LineIndex(int nLine) const
{
	return m_richCtrl.LineIndex(nLine);
}

int RichEdit::LineLength(int nLine) const
{
	return m_richCtrl.LineLength(nLine);
}

bool RichEdit::LineScroll(int nLines)
{
	return m_richCtrl.LineScroll(nLines);
}

long RichEdit::LineFromChar(long nIndex) const
{
	return m_richCtrl.LineFromChar(nIndex);
}

UiPoint RichEdit::PosFromChar(long lChar) const
{ 
	POINT pt = m_richCtrl.PosFromChar(lChar);
	return UiPoint(pt.x, pt.y);
}

int RichEdit::CharFromPos(UiPoint pt) const
{
	POINT ptValue = { pt.x, pt.y };
	return m_richCtrl.CharFromPos(ptValue);
}

void RichEdit::EmptyUndoBuffer()
{
	m_richCtrl.EmptyUndoBuffer();
}

UINT RichEdit::SetUndoLimit(UINT nLimit)
{
	return m_richCtrl.SetUndoLimit(nLimit);
}

long RichEdit::StreamIn(UINT nFormat, EDITSTREAM &es)
{
	return m_richCtrl.StreamIn(nFormat, es);
}

long RichEdit::StreamOut(UINT nFormat, EDITSTREAM &es)
{
	return m_richCtrl.StreamOut(nFormat, es);
}

void RichEdit::OnTxNotify(DWORD iNotify, void *pv)
{
	switch(iNotify)
	{ 
	case EN_LINK:   
		{
			NMHDR* hdr = (NMHDR*) pv;
			ENLINK* link = (ENLINK*)hdr;

			if((link != nullptr) && (link->msg == WM_LBUTTONUP)) {
				CHARRANGE oldSel = {0, 0};
				GetSel(oldSel);
				SetSel(link->chrg);
				std::wstring url = GetSelText();
				SetSel(oldSel);
				if (!url.empty()) {
					this->SendEvent(kEventCustomLinkClick, (WPARAM)url.c_str());
				}
			}
		}
		break;
	case EN_CHANGE:
		//文本内容变化，发送事件
		SendEvent(kEventTextChange); 
		break;
	case EN_DROPFILES:   
	case EN_MSGFILTER:   
	case EN_OLEOPFAILED:    
	case EN_PROTECTED:
	case EN_SAVECLIPBOARD:   
	case EN_SELCHANGE:   
	case EN_STOPNOUNDO:   
	case EN_OBJECTPOSITIONS:   
	case EN_DRAGDROPDONE:   
		{
			if (pv) {   // Fill out NMHDR portion of pv   
				LONG nId =  ::GetWindowLong(GetWindowHandle(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = GetWindowHandle();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				SendMessage(GetWindowHandle(), WM_NOTIFY, (WPARAM)nId, (LPARAM)pv);
			}    
		}
		break;
	}
}

HWND RichEdit::GetWindowHandle()
{
	auto window = GetWindow();
	return window ? window->GetHWND() : nullptr;
}

HDC RichEdit::GetWindowDC()
{
	auto window = GetWindow();
	return window ? window->GetPaintDC() : nullptr;
}

UiSize RichEdit::GetNaturalSize(LONG width, LONG height)
{
	if (width < 0) {
		width = 0;
	}
	if (height < 0) {
		height = 0;
	}	
	UiSize sz(0,0);
	if ((m_cbGetNaturalSize != nullptr) && m_cbGetNaturalSize(width, height, sz)) {
		return sz;
	}
	LONG lWidth = width;
	LONG lHeight = height;
	SIZEL szExtent = { -1, -1 };

	ITextServices* pTextServices = nullptr;
	if (m_pRichHost) {
		pTextServices = m_pRichHost->GetTextServices();
	}
	if (pTextServices != nullptr) {
		pTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
										GetWindowDC(),
										NULL,
										NULL,
										TXTNS_FITTOCONTENT,
										&szExtent,
										&lWidth,
										&lHeight);
	}
	sz.cx = (int)lWidth;
	sz.cy = (int)lHeight;
	return sz;
}

void RichEdit::SetImmStatus(BOOL bOpen)
{
	HWND hwnd = GetWindowHandle();
	if (hwnd == nullptr) {
		return;
	}
	// 失去焦点时关闭输入法
	HIMC hImc = ::ImmGetContext(hwnd);
	// 失去焦点是会把关联的输入法去掉，导致无法无法输入中文
	//::ImmAssociateContext(hwnd, bOpen ? hImc : NULL);
	if (hImc != NULL) {
		if (::ImmGetOpenStatus(hImc)) {
			if (!bOpen) {
				::ImmSetOpenStatus(hImc, FALSE);
			}
		}
		else {
			if (bOpen) {
				::ImmSetOpenStatus(hImc, TRUE);
			}
		}
		::ImmReleaseContext(hwnd, hImc);
	}
}

void RichEdit::SetTimer(UINT idTimer, UINT uTimeout)
{
	auto timeFlag = m_timeFlagMap.find(idTimer);
	if (timeFlag != m_timeFlagMap.end()) {
		timeFlag->second.Cancel();
	}

	auto callback = [this, idTimer]() {
		m_richCtrl.TxSendMessage(WM_TIMER, idTimer, 0);
	};
	GlobalManager::Instance().Timer().AddCancelableTimer(m_timeFlagMap[idTimer].GetWeakFlag(), callback, uTimeout, TimerManager::REPEAT_FOREVER);
}

void RichEdit::KillTimer(UINT idTimer)
{
	auto timeFlag = m_timeFlagMap.find(idTimer);
	if (timeFlag != m_timeFlagMap.end()) {
		timeFlag->second.Cancel();
		m_timeFlagMap.erase(timeFlag);
	}
}

void RichEdit::ScreenToClient(UiPoint& pt)
{
	if (m_pRichHost != nullptr) {
		POINT point = {pt.x, pt.y};
		m_pRichHost->TxScreenToClient(&point);
		pt.x = point.x;
		pt.y = point.y;
	}
}

void RichEdit::ClientToScreen(UiPoint& pt)
{
	if (m_pRichHost != nullptr) {
		POINT point = { pt.x, pt.y };
		m_pRichHost->TxClientToScreen(&point);
		pt.x = point.x;
		pt.y = point.y;
	}
}

// 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
// 引入iPos就是为了修正这个bug
void RichEdit::SetScrollPos(UiSize64 szPos)
{
	int64_t cx = 0;
	int64_t cy = 0;
	ScrollBar* pVScrollBar = GetVScrollBar();
	ScrollBar* pHScrollBar = GetHScrollBar();
    if ((pVScrollBar != nullptr) && pVScrollBar->IsValid() ) {
        int64_t iLastScrollPos = pVScrollBar->GetScrollPos();
        pVScrollBar->SetScrollPos(szPos.cy);
        cy = pVScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if ((pHScrollBar != nullptr) && pHScrollBar->IsValid() ) {
		int64_t iLastScrollPos = pHScrollBar->GetScrollPos();
        pHScrollBar->SetScrollPos(szPos.cx);
        cx = pHScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( cy != 0 ) {
		int64_t iPos = 0;
		if (!IsRichText() && (pVScrollBar != nullptr) && pVScrollBar->IsValid()) {
			iPos = pVScrollBar->GetScrollPos();
		}
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, (pVScrollBar != nullptr) ? pVScrollBar->GetScrollPos() : 0);
        m_richCtrl.TxSendMessage(WM_VSCROLL, wParam, 0L);
        if(!IsRichText() && (pVScrollBar != nullptr) && pVScrollBar->IsValid() ) {
			if (cy > 0 && pVScrollBar->GetScrollPos() <= iPos) {
				pVScrollBar->SetScrollPos(iPos);
			}
        }
    }
    if( cx != 0 ) {
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, (pHScrollBar != nullptr) ? pHScrollBar->GetScrollPos() : 0);
		m_richCtrl.TxSendMessage(WM_HSCROLL, wParam, 0L);
    }
}

void RichEdit::LineUp()
{
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L);
}

void RichEdit::LineDown()
{
	int64_t iPos = 0;
	ScrollBar* pVScrollBar = GetVScrollBar();
	if (!IsRichText() && (pVScrollBar != nullptr) && pVScrollBar->IsValid()) {
		iPos = pVScrollBar->GetScrollPos();
	}
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L);
    if(!IsRichText() && (pVScrollBar != nullptr) && pVScrollBar->IsValid() ) {
		if (pVScrollBar->GetScrollPos() <= iPos) {
			pVScrollBar->SetScrollPos(pVScrollBar->GetScrollRange());
		}
    }
}

void RichEdit::PageUp()
{
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L);
}

void RichEdit::PageDown()
{
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L);
}

void RichEdit::HomeUp()
{
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_TOP, 0L);
}

void RichEdit::EndDown()
{
	m_richCtrl.TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L);
}

void RichEdit::LineLeft()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L);
}

void RichEdit::LineRight()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L);
}

void RichEdit::PageLeft()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L);
}

void RichEdit::PageRight()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L);
}

void RichEdit::HomeLeft()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_LEFT, 0L);
}

void RichEdit::EndRight()
{
	m_richCtrl.TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L);
}

std::wstring RichEdit::GetType() const { return DUI_CTR_RICHEDIT; }

void RichEdit::DoInit()
{
	if (m_bInited) {
		return;
	}
	m_bInited = true;

	//设置字体
	std::wstring fontId = GetFontId();
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetFontId(fontId);
	}

	ASSERT(m_pRichHost != nullptr);
	ScrollBar* pHScrollBar = GetHScrollBar();
	if (pHScrollBar != nullptr) {
		pHScrollBar->SetScrollRange(0);
	}
	ScrollBar* pVScrollBar = GetVScrollBar();
	if (pVScrollBar != nullptr) {
		pVScrollBar->SetScrollRange(0);
	}
}

void RichEdit::SetEnabled(bool bEnable /*= true*/)
{
	if (IsEnabled() == bEnable) {
		return;
	}
	__super::SetEnabled(bEnable);

	if (bEnable) {
		SetState(kControlStateNormal);
		SetTextColor(m_sTextColor.c_str());
	}
	else {
		SetState(kControlStateDisabled);
		SetTextColor(m_sDisabledTextColor.c_str());
	}
}

UiEstSize RichEdit::EstimateSize(UiSize /*szAvailable*/)
{
	ITextServices* pTextServices = nullptr;
	if (m_pRichHost != nullptr) {
		pTextServices = m_pRichHost->GetTextServices();
	}
	if (pTextServices == nullptr) {
		return UiEstSize();
	}
	UiFixedSize fixexSize = GetFixedSize();
	UiSize size(fixexSize.cx.GetInt32(), fixexSize.cy.GetInt32());
	if (fixexSize.cx.IsAuto() || fixexSize.cy.IsAuto()) {
		LONG iWidth = size.cx;
		LONG iHeight = size.cy;
		if (fixexSize.cy.IsAuto()) {
			ASSERT(!fixexSize.cx.IsAuto());
			iHeight = 0;
		}
		else if (fixexSize.cx.IsAuto()) {
			ASSERT(!fixexSize.cy.IsAuto());
			iWidth = 0;
		}
		SIZEL szExtent = {-1, -1};
		pTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
										GetWindowDC(),
										NULL,
										NULL,
										TXTNS_FITTOCONTENT,
										&szExtent,
										&iWidth,
										&iHeight) ;
		
		//返回大小需要包含内边距
		UiPadding rcPadding = GetControlPadding();
		UiPadding rcTextPadding = GetTextPadding();
		if (fixexSize.cy.IsAuto()) {
			size.cy = iHeight + (rcPadding.top + rcPadding.bottom) + (rcTextPadding.top + rcTextPadding.bottom);
			fixexSize.cy.SetInt32(size.cy);
		}
		else if (fixexSize.cx.IsAuto()) {
			size.cx = iWidth + (rcPadding.left + rcPadding.right) + (rcTextPadding.left + rcTextPadding.right);
			fixexSize.cx.SetInt32(size.cx);
		}
	}
    return MakeEstSize(fixexSize);
}

UiSize RichEdit::EstimateText(UiSize szAvailable)
{
	ITextServices* pTextServices = nullptr;
	if (m_pRichHost != nullptr) {
		pTextServices = m_pRichHost->GetTextServices();
	}
	if (pTextServices == nullptr) {
		return UiSize();
	}
	UiPadding rcPadding = GetControlPadding();
	UiPadding rcTextPadding = GetTextPadding();
	szAvailable.cx -= (rcPadding.left + rcPadding.right);
	szAvailable.cy -= (rcPadding.top + rcPadding.bottom);
	szAvailable.cx -= (rcTextPadding.left + rcTextPadding.right);
	szAvailable.cy -= (rcTextPadding.top + rcTextPadding.bottom);

    szAvailable.Validate();
    LONG iWidth = szAvailable.cx;
	if (iWidth < 0) {
		iWidth = 0;
	}
    LONG iHeight = 0;
    SIZEL szExtent = { -1, -1 };
	pTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
									GetWindow()->GetPaintDC(),
									NULL,
									NULL,
									TXTNS_FITTOCONTENT,
									&szExtent,
									&iWidth,
									&iHeight);

	iWidth = std::max((int32_t)iWidth, 0);
	iHeight = std::max((int32_t)iHeight, 0);
	iWidth += (rcPadding.left + rcPadding.right);
	iHeight += (rcPadding.top + rcPadding.bottom);

	iWidth += (rcTextPadding.left + rcTextPadding.right);
	iHeight += (rcTextPadding.top + rcTextPadding.bottom);

	szAvailable.cx = std::max((int32_t)iWidth, 0);
    szAvailable.cy = std::max((int32_t)iHeight, 0);
    return szAvailable;
}

void RichEdit::SetPos(UiRect rc)
{
    Control::SetPos(rc);
	ITextServices* pTextServices = nullptr;
	if (m_pRichHost != nullptr) {
		pTextServices = m_pRichHost->GetTextServices();
	}
	if (pTextServices == nullptr) {
		return;
	}

    rc = GetRectWithoutPadding();
    bool bVScrollBarVisible = false;
	ScrollBar* pVScrollBar = GetVScrollBar();
	if ((pVScrollBar != nullptr) && pVScrollBar->IsValid()) {
		bVScrollBarVisible = true;
        rc.right -= pVScrollBar->GetFixedWidth().GetInt32();
    }
	ScrollBar* pHScrollBar = GetHScrollBar();
	if ((pHScrollBar != nullptr) && pHScrollBar->IsValid()) {
        rc.bottom -= pHScrollBar->GetFixedHeight().GetInt32();
    }

    if (m_pRichHost != nullptr) {
		//调整编辑框的位置, 剪去文本内边距
		UiRect textRect = rc;
		UiPadding rcTextPadding = GetTextPadding();
		textRect.Deflate(rcTextPadding);
        m_pRichHost->SetClientRect(textRect);
        if (bVScrollBarVisible && (pVScrollBar == nullptr) && (!pVScrollBar->IsValid() || m_bVScrollBarFixing)) {
            LONG lWidth = rc.Width() + pVScrollBar->GetFixedWidth().GetInt32();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
			pTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
											GetWindowDC(),
											NULL,
											NULL,
											TXTNS_FITTOCONTENT,
											&szExtent,
											&lWidth,
											&lHeight);
            if (lHeight > rc.Height()) {
                pVScrollBar->SetScrollPos(0);
                m_bVScrollBarFixing = true;
            }
            else {
                if (m_bVScrollBarFixing) {
                    pVScrollBar->SetScrollRange(0);
                    m_bVScrollBarFixing = false;
                }
            }
        }
    }

    if ((pVScrollBar != nullptr) && pVScrollBar->IsValid()) {
        UiRect rcScrollBarPos(rc.right, rc.top, rc.right + pVScrollBar->GetFixedWidth().GetInt32(), rc.bottom);
        pVScrollBar->SetPos(rcScrollBarPos);
    }
    if (pHScrollBar != NULL && pHScrollBar->IsValid()) {
        UiRect rcScrollBarPos(rc.left, rc.bottom, rc.right, rc.bottom + pHScrollBar->GetFixedHeight().GetInt32());
        pHScrollBar->SetPos(rcScrollBarPos);
    }

    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        auto pControl = *it;
        if ((pControl == nullptr) || !pControl->IsVisible()){
            continue;
        }
        if (pControl->IsFloat()) {
            Layout::SetFloatPos(pControl, GetPos());
        }
        else {
            pControl->SetPos(rc); // 所有非float子控件放大到整个客户区
        }
    }
}

UINT RichEdit::GetControlFlags() const
{
	return IsEnabled() && IsAllowTabStop() ? UIFLAG_TABSTOP : UIFLAG_DEFAULT;
}

void RichEdit::HandleEvent(const EventArgs& msg)
{
	if (IsDisabledEvents(msg)) {
		//如果是鼠标键盘消息，并且控件是Disabled的，转发给上层控件
		Box* pParent = GetParent();
		if (pParent != nullptr) {
			pParent->SendEvent(msg);
		}
		else {
			__super::HandleEvent(msg);
		}
		return;
	}
	if (msg.Type == kEventMouseMove) {
		OnMouseMessage(WM_MOUSEMOVE, msg);
		return;
	}
	if (msg.Type == kEventMouseWheel) {
		if (::GetAsyncKeyState(VK_CONTROL) < 0) {
			return;
		}		
		ScrollBox::HandleEvent(msg);
		//OnMouseMessage(WM_MOUSEWHEEL, event);
		return;
	}

	if (msg.Type == kEventMouseButtonDown) {
		if (m_linkInfo.size() > 0)	{
			std::wstring url;
			if (HittestCustomLink(UiPoint(msg.ptMouse), url)) {
				SendEvent(kEventCustomLinkClick, (WPARAM)url.c_str());
				return;
			}
		}
		OnMouseMessage(WM_LBUTTONDOWN, msg);
		return;
	}
	if (msg.Type == kEventMouseButtonUp) {
		if (IsEnabled() && !m_bSelAllEver) {
			m_bSelAllEver = true;
			if (m_bSelAllOnFocus) {
				SetSelAll();
				if (GetMultiLine()) {
					HomeUp();
				}
				else {
					HomeLeft();
				}
			}
		}

		OnMouseMessage(WM_LBUTTONUP, msg);
		return;
	}
	if (msg.Type == kEventMouseDoubleClick) {
		if (IsReadOnly()) {
			SetSelAll();
			return;
		}

		OnMouseMessage(WM_LBUTTONDBLCLK, msg);
		return;
	}
	if (msg.Type == kEventMouseRButtonDown) {
		OnMouseMessage(WM_RBUTTONDOWN, msg);
		return;
	}
	if (msg.Type == kEventMouseRButtonUp) {
		OnMouseMessage(WM_RBUTTONUP, msg);
		return;
	}
	ScrollBox::HandleEvent(msg);
}

bool RichEdit::OnSetCursor(const EventArgs& msg)
{
	std::wstring strLink;
	if (HittestCustomLink(UiPoint(msg.ptMouse), strLink)) {
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		return true;
	}
	if (m_pRichHost && m_pRichHost->SetCursor(nullptr, &msg.ptMouse)) {
		return true;
	}
	else {
		::SetCursor(::LoadCursor(NULL, IsReadOnly() ? IDC_ARROW : IDC_IBEAM));
		return true;
	}
}

bool RichEdit::OnSetFocus(const EventArgs& /*msg*/)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->OnTxInPlaceActivate(NULL);
		m_richCtrl.TxSendMessage(WM_SETFOCUS, 0, 0);
		ShowCaret(true);
	}
	SetImmStatus(TRUE);
	Invalidate();
	return true;
}

bool RichEdit::OnKillFocus(const EventArgs& /*msg*/)
{
	if (m_pRichHost) {
		m_pRichHost->OnTxInPlaceActivate(NULL);
		m_richCtrl.TxSendMessage(WM_KILLFOCUS, 0, 0);
		ShowCaret(false);
	}

	m_bSelAllEver = false;
	if (m_bNoSelOnKillFocus && IsReadOnly() && IsEnabled()) {
		SetSelNone();
	}
	if (m_bSelAllOnFocus && IsEnabled()) {
		SetSelNone();
	}

	OnScreenKeyboardManager::GetInstance()->ShowOSK(false);
	SetImmStatus(FALSE);
	Invalidate();
	return true;
}

bool RichEdit::OnChar(const EventArgs& msg)
{
	//TAB
	if (::GetKeyState(VK_TAB) < 0 && !m_bWantTab) {
		SendEvent(kEventTab);
		return true;
	}
	//Number
	if (IsNumberOnly()) {
		if (msg.wParam < '0' || msg.wParam > '9') {
			return true;
		}
	}

	m_richCtrl.TxSendMessage(WM_CHAR, msg.wParam, msg.lParam);
	return true;
}

bool RichEdit::OnKeyDown(const EventArgs& msg)
{
	if (msg.wParam == VK_RETURN && ::GetAsyncKeyState(VK_SHIFT) >= 0)	{
		if (m_bNeedReturnMsg && ((m_bReturnMsgWantCtrl && ::GetAsyncKeyState(VK_CONTROL) < 0) ||
			(!m_bReturnMsgWantCtrl && ::GetAsyncKeyState(VK_CONTROL) >= 0))) {
			SendEvent(kEventReturn);
			return true;
		}
	}
	else if (IsNumberOnly() && msg.wParam == 'V' && ::GetKeyState(VK_CONTROL) < 0) {
		std::wstring strClipText;
		GetClipboardText(strClipText);
		if (!strClipText.empty()) {
			std::wstring strNum;
			for (auto it = strClipText.begin(); it != strClipText.end(); it++) {
				if ((*it) <= L'9' && (*it) >= L'0') {
					strNum.push_back((*it));
				}
			}
			if (strNum.empty()) {
				return true;
			}

			SetClipBoardText(strNum); //修改剪切板内容为纯数字
			nbase::ThreadManager::PostTask([strClipText]() { SetClipBoardText(strClipText); }); //粘贴完后又把剪切板内容改回来
		}
	}

	m_richCtrl.TxSendMessage(WM_KEYDOWN, msg.wParam, msg.lParam);
	return true;
}

bool RichEdit::OnImeStartComposition(const EventArgs& /*msg*/)
{
	HWND hWnd = GetWindowHandle();
	if (hWnd == nullptr) {
		return true;
	}

	HIMC hImc = ::ImmGetContext(hWnd);
	if (hImc == nullptr) {
		return true;
	}

	COMPOSITIONFORM	cfs = { 0, };
	UiPoint ptScrollOffset = GetScrollOffsetInScrollBox();
	POINT pt;
	pt.x = m_iCaretPosX - ptScrollOffset.x;
	pt.y = m_iCaretPosY - ptScrollOffset.y;

	//pt.y += (m_iCaretHeight + lf.lfHeight) / 4;
	cfs.dwStyle = CFS_POINT;
	if (pt.x < 1) {
		pt.x = 1;
	}
	if (pt.y < 1) {
		pt.y = 1;
	}
	cfs.ptCurrentPos = pt;
	::ImmSetCompositionWindow(hImc, &cfs);
	::ImmReleaseContext(hWnd, hImc);
	m_bIsComposition = true;
	return true;
}

bool RichEdit::OnImeEndComposition(const EventArgs& /*msg*/)
{
	m_bIsComposition = false;
	return true;
}

void RichEdit::OnMouseMessage(UINT uMsg, const EventArgs& msg)
{
	UiPoint pt(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
	pt.Offset(GetScrollOffsetInScrollBox());
	m_richCtrl.TxSendMessage(uMsg, msg.wParam, MAKELPARAM(pt.x, pt.y));
}

void RichEdit::Paint(IRender* pRender, const UiRect& rcPaint)
{
	//必须不使用缓存，否则绘制异常
	ASSERT(IsUseCache() == false);
    UiRect rcTemp;
	if (!UiRect::Intersect(rcTemp, rcPaint, GetRect())) {
		return;
	}

    Control::Paint(pRender, rcPaint);
	ITextServices* pTextServices = nullptr;
	if (m_pRichHost != nullptr) {
		pTextServices = m_pRichHost->GetTextServices();
	}
	if ((pTextServices == nullptr) || (m_pRichHost == nullptr)) {
		return;
	}

    UiRect rc;
	if (m_pRichHost != nullptr) {
		m_pRichHost->GetControlRect(&rc);
	}    
    // Remember wparam is actually the hdc and lparam is the update
    // rect because this message has been preprocessed by the window.
	HDC hdc = pRender->GetDC();
	RECT paintRect = { rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom };
	pTextServices->TxDraw(DVASPECT_CONTENT,  	// Draw Aspect
						  /*-1*/0,				// Lindex
						  NULL,					// Info for drawing optimazation
						  NULL,					// target device information
						  hdc,			        // Draw device HDC
						  NULL, 				// Target device HDC
						  (RECTL*)&rc,			// Bounding client rectangle
						  NULL, 		        // Clipping rectangle for metafiles
						  &paintRect,			// Update rectangle
						  NULL, 	   			// Call back function
						  NULL,					// Call back parameter
						  0);				    // What view of the object

	pRender->ReleaseDC(hdc);
	ScrollBar* pVScrollBar = GetVScrollBar();
	if (m_bVScrollBarFixing && (pVScrollBar != nullptr)) {
        LONG lWidth = rc.right - rc.left + pVScrollBar->GetFixedWidth().GetInt32();
		//LONG lWidth = rc.right - rc.left;
        LONG lHeight = 0;
        SIZEL szExtent = { -1, -1 };
		pTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
										GetWindowDC(), 
										NULL,
										NULL,
										TXTNS_FITTOCONTENT,
										&szExtent,
										&lWidth,
										&lHeight);

        if( lHeight <= rc.Height() ) {
            Arrange();
        }
    }
}

void RichEdit::PaintChild(IRender* pRender, const UiRect& rcPaint)
{
    UiRect rcTemp;
    if (!UiRect::Intersect(rcTemp, rcPaint, GetRect())) {
        return;
    }

    PaintCaret(pRender, rcPaint);

	ScrollBar* pVScrollBar = GetVScrollBar();
	ScrollBar* pHScrollBar = GetHScrollBar();
    if (m_items.size() > 0) {
        UiRect rc = GetRectWithoutPadding();
        if ((pVScrollBar != nullptr) && pVScrollBar->IsValid()) {
            rc.right -= pVScrollBar->GetFixedWidth().GetInt32();
        }
        if ((pHScrollBar != nullptr) && pHScrollBar->IsValid()) {
            rc.bottom -= pHScrollBar->GetFixedHeight().GetInt32();
        }

        if (!UiRect::Intersect(rcTemp, rcPaint, rc)) {
            for (auto it = m_items.begin(); it != m_items.end(); ++it) {
                auto pControl = *it;
                if ((pControl == nullptr) || !pControl->IsVisible()) {
                    continue;
                }
                UiRect controlPos = pControl->GetPos();
                if (!UiRect::Intersect(rcTemp, rcPaint, controlPos)) {
                    continue;
                }
                if (pControl->IsFloat()) {
                    if (!UiRect::Intersect(rcTemp, GetRect(), controlPos)) {
                        continue;
                    }
                    pControl->AlphaPaint(pRender, rcPaint);
                }
            }
        }
        else {
            AutoClip childClip(pRender, rcTemp);
            for (auto it = m_items.begin(); it != m_items.end(); ++it) {
                auto pControl = *it;
                if ((pControl == nullptr) || !pControl->IsVisible()) {
                    continue;
                }
                UiRect controlPos = pControl->GetPos();
                if (!UiRect::Intersect(rcTemp, rcPaint, controlPos)) {
                    continue;
                }
                if (pControl->IsFloat()) {
                    if (!UiRect::Intersect(rcTemp, GetRect(), controlPos)) {
                        continue;
                    }
                    pControl->AlphaPaint(pRender, rcPaint);
                }
                else {
                    if (!UiRect::Intersect(rcTemp, rc, controlPos)) {
                        continue;
                    }
                    pControl->AlphaPaint(pRender, rcPaint);
                }
            }
        }
    }

    if ((pVScrollBar != nullptr) && pVScrollBar->IsVisible()) {
        UiRect verBarPos = pVScrollBar->GetPos();
        if (UiRect::Intersect(rcTemp, rcPaint, verBarPos)) {
            pVScrollBar->AlphaPaint(pRender, rcPaint);
        }
    }

    if ((pHScrollBar != nullptr) && pHScrollBar->IsVisible()) {
        UiRect horBarPos = pHScrollBar->GetPos();
        if (UiRect::Intersect(rcTemp, rcPaint, horBarPos)) {
            pHScrollBar->AlphaPaint(pRender, rcPaint);
        }
    }
}

void RichEdit::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == L"vscrollbar") {
		//纵向滚动条
		if (strValue == L"true") {
			EnableScrollBar(true, GetHScrollBar() != NULL);
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetVScrollBar(true);
			}			
		}
		else {
			EnableScrollBar(false, GetHScrollBar() != NULL);
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetVScrollBar(false);
			}			
		}
	}
	else if (strName == L"hscrollbar") {
		//横向滚动条
		if (strValue == L"true") {
			EnableScrollBar(GetVScrollBar() != NULL, true);
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetHScrollBar(true);
			}			
		}
		else {
			EnableScrollBar(GetVScrollBar() != NULL, false);
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetHScrollBar(false);
			}			
		}
	}
	else if ((strName == L"auto_vscroll") || (strName == L"autovscroll")) {
		//当用户在最后一行按 ENTER 时，自动将文本向上滚动一页。
		if (m_pRichHost != nullptr) {
			m_pRichHost->SetAutoVScroll(strValue == L"true");
		}
	}
	else if ((strName == L"auto_hscroll") || (strName == L"autohscroll")){
		//当用户在行尾键入一个字符时，自动将文本向右滚动 10 个字符。
		//当用户按 Enter 时，控件会将所有文本滚动回零位置。
		if (m_pRichHost != nullptr) {
			m_pRichHost->SetAutoHScroll(strValue == L"true");
		}
	}
	else if ((strName == L"single_line") || (strName == L"singleline")) {
		SetMultiLine(strValue != L"true");
	}
	else if ((strName == L"multi_line") || (strName == L"multiline")) {
		SetMultiLine(strValue == L"true");
	}
	else if (strName == L"readonly") {
		SetReadOnly(strValue == L"true");
	}
	else if (strName == L"password") {
		SetPassword(strValue == L"true");
	}
	else if (strName == L"show_password") {
		SetShowPassword(strValue == L"true");
	}
	else if (strName == L"password_char") {
		if (!strValue.empty()) {
			SetPasswordChar(strValue.front());
		}
	}
	else if (strName == L"flash_password_char") {
		SetFlashPasswordChar(strValue == L"true");
	}
	else if (strName == L"number") {
		SetNumberOnly(strValue == L"true");
	}
	else if (strName == L"text_align") {
		if (strValue.find(L"left") != std::wstring::npos) {
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetHAlignType(kHorAlignLeft);
			}
		}		
		if (strValue.find(L"right") != std::wstring::npos) {			
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetHAlignType(kHorAlignRight);
			}
		}
		if (strValue.find(L"hcenter") != std::wstring::npos) {
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetHAlignType(kHorAlignCenter);
			}
		}

		if (strValue.find(L"top") != std::wstring::npos) {
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetVAlignType(kVerAlignTop);
			}
		}
		if (strValue.find(L"bottom") != std::wstring::npos) {
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetVAlignType(kVerAlignBottom);
			}
		}
		if (strValue.find(L"vcenter") != std::wstring::npos) {
			if (m_pRichHost != nullptr) {
				m_pRichHost->SetVAlignType(kVerAlignCenter);
			}
		}
	}
	else if ((strName == L"text_padding") || (strName == L"textpadding")) {
		UiPadding rcTextPadding;
		AttributeUtil::ParsePaddingValue(strValue.c_str(), rcTextPadding);
		SetTextPadding(rcTextPadding);
	}
	else if ((strName == L"normal_text_color") || (strName == L"normaltextcolor")){
		m_sTextColor = strValue;
		if (IsEnabled()) {
			SetTextColor(m_sTextColor.c_str());
		}
	}
	else if ((strName == L"disabled_text_color") || (strName == L"disabledtextcolor")){
		m_sDisabledTextColor = strValue;
		if (!IsEnabled()) {
			SetTextColor(m_sDisabledTextColor.c_str());
		}
	}
	else if ((strName == L"caret_color") || (strName == L"caretcolor")){
		//设置光标的颜色
		SetCaretColor(strValue);
	}
	else if ((strName == L"prompt_mode") || (strName == L"promptmode")){
		//提示模式
		m_bAllowPrompt = (strValue == L"true") ? true : false;
	}
	else if ((strName == L"prompt_color") || (strName == L"promptcolor")){
		//提示文字的颜色
		m_sPromptColor = strValue;
	}
	else if ((strName == L"prompt_text") || (strName == L"prompttext")) {
		//提示文字
		SetPromptText(strValue);
	}
	else if ((strName == L"prompt_textid") || (strName == L"prompttextid")){
		//提示文字ID
		SetPromptTextId(strValue);
	}
	else if ((strName == L"focused_image") || (strName == L"focusedimage")){
		SetFocusedImage(strValue);
	}
	else if (strName == L"font") {
		SetFontId(strValue);
	}
	else if (strName == L"text") {
		SetText(strValue);
	}
	else if ((strName == L"textid") || (strName == L"textid")){
		SetTextId(strValue);
	}
	else if ((strName == L"want_tab") || (strName == L"wanttab")){
		SetWantTab(strValue == L"true");
	}
	else if ((strName == L"want_return_msg") || (strName == L"wantreturnmsg")){
		SetNeedReturnMsg(strValue == L"true");
	}
	else if ((strName == L"return_msg_want_ctrl") || (strName == L"returnmsgwantctrl")){
		SetReturnMsgWantCtrl(strValue == L"true");
	}
	else if ((strName == L"rich_text") || (strName == L"rich")) {
		//是否为富文本属性
		SetRichText(strValue == L"true");
	}
	else if (strName == L"auto_detect_url") {
		//是否自动检测URL，如果是URL则显示为超链接
		SetAutoURLDetect(strValue == L"true");
	}
	else if ((strName == L"max_char") || (strName == L"maxchar")){
		//限制最多字符数（默认为32KB）
		SetLimitText(_wtoi(strValue.c_str()));
	}
	else if (strName == L"allow_beep") {
		//是否允许发出Beep声音
		SetAllowBeep(strValue == L"true");
	}
	else if (strName == L"word_wrap") {
		//是否自动换行
		SetWordWrap(strValue == L"true");
	}
	else if (strName == L"no_caret_readonly") {
		//只读模式，不显示光标
		SetNoCaretReadonly();
	}
	else if (strName == L"save_selection") {
		//如果 为 TRUE，则当控件处于非活动状态时，应保存所选内容的边界。
		//如果 为 FALSE，则当控件再次处于活动状态时，可以选择边界重置为 start = 0，length = 0。
		SetSaveSelection(strValue == L"true");
	}
	else if (strName == L"hide_selection") {
		//是否隐藏选项项
		SetHideSelection(strValue == L"true");
	}
	else {
		Box::SetAttribute(strName, strValue);
	}
}

BOOL RichEdit::CreateCaret(INT xWidth, INT yHeight)
{
	m_iCaretWidth = xWidth;
	m_iCaretHeight = yHeight;
	return true;
}

BOOL RichEdit::ShowCaret(BOOL fShow)
{
	if (fShow) {
		m_bIsCaretVisiable = true;
		m_drawCaretFlag.Cancel();
		std::function<void()> closure = nbase::Bind(&RichEdit::ChangeCaretVisiable, this);
		GlobalManager::Instance().Timer().AddCancelableTimer(m_drawCaretFlag.GetWeakFlag(), closure, 500, TimerManager::REPEAT_FOREVER);
	}
	else {
		m_bIsCaretVisiable = false;
		m_drawCaretFlag.Cancel();
	}

	Invalidate();
	return true;
}

void RichEdit::SetCaretColor(const std::wstring& dwColor)
{
	m_sCaretColor = dwColor;
}

std::wstring RichEdit::GetCaretColor()
{
	return m_sCaretColor.c_str();
}

RECT RichEdit::GetCaretRect()
{
	RECT rc = { m_iCaretPosX, m_iCaretPosY, m_iCaretPosX + m_iCaretWidth, m_iCaretPosY + m_iCaretHeight };
	return rc;
}

BOOL RichEdit::SetCaretPos(INT x, INT y)
{
	m_iCaretPosX = x;
	m_iCaretPosY = y;
	ShowCaret(GetSelText().empty());
	return true;
}

void RichEdit::ChangeCaretVisiable()
{
	m_bIsCaretVisiable = !m_bIsCaretVisiable;
	Invalidate();
}

void RichEdit::PaintCaret(IRender* pRender, const UiRect& /*rcPaint*/)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	if (IsReadOnly() && m_bNoCaretReadonly) {
		return;
	}

	if (m_bIsCaretVisiable && !m_bIsComposition) {
		UiRect rect(m_iCaretPosX, m_iCaretPosY, m_iCaretPosX, m_iCaretPosY + m_iCaretHeight);
		UiColor dwClrColor(0xff000000);
		if (!m_sCaretColor.empty()) {
			dwClrColor = this->GetUiColor(m_sCaretColor.c_str());
		}
		pRender->DrawLine(UiPoint(rect.left, rect.top), UiPoint(rect.right, rect.bottom), dwClrColor, m_iCaretWidth);
	}
}

void RichEdit::SetPromptMode(bool bPrompt)
{
	if(bPrompt == m_bAllowPrompt)
		return;
	m_bAllowPrompt = bPrompt;
	Invalidate();
}

std::wstring RichEdit::GetPromptText() const
{
	std::wstring strText = m_sPromptText.c_str();
	if (strText.empty() && !m_sPromptTextId.empty()) {
		strText = GlobalManager::Instance().Lang().GetStringViaID(m_sPromptTextId.c_str());
	}

	return strText;
}

std::string RichEdit::GetUTF8PromptText() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetPromptText(), strOut, CP_UTF8);
	return strOut;
}

void RichEdit::SetPromptText(const std::wstring& strText)
{
	if (m_sPromptText != strText) {
		m_sPromptText = strText;
		Invalidate();
	}
}

void RichEdit::SetUTF8PromptText(const std::string& strText)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	SetPromptText(strOut);
}

void RichEdit::SetPromptTextId(const std::wstring& strTextId)
{
	if (m_sPromptTextId == strTextId) {
		m_sPromptTextId = strTextId;
		Invalidate();
	}
}

void RichEdit::SetUTF8PromptTextId(const std::string& strTextId)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strTextId, strOut, CP_UTF8);
	SetPromptTextId(strOut);
}

void RichEdit::PaintPromptText(IRender* pRender)
{
	ASSERT(pRender != nullptr);
	if (pRender == nullptr) {
		return;
	}
	if (GetTextLength() != 0) {
		return;
	}

	if (m_pRichHost == nullptr) {
		return;
	}

	std::wstring strPrompt = GetPromptText();
	if (strPrompt.empty() || m_sPromptColor.empty()) {
		return;
	}

	UiRect rc;
	if (m_pRichHost != nullptr) {
		m_pRichHost->GetControlRect(&rc);
	}
	UiColor dwClrColor = GetUiColor(m_sPromptColor.c_str());
	UINT dwStyle = TEXT_NOCLIP;
	pRender->DrawString(rc, strPrompt, dwClrColor, m_sFontId.c_str(), dwStyle);
}

std::wstring RichEdit::GetFocusedImage()
{
	if (m_pFocusedImage != nullptr) {
		return m_pFocusedImage->GetImageString();
	}
	return std::wstring();
}

void RichEdit::SetFocusedImage( const std::wstring& strImage )
{
	if (m_pFocusedImage == nullptr) {
		m_pFocusedImage = new Image;
	}
	m_pFocusedImage->SetImageString(strImage);
	Invalidate();
}

void RichEdit::PaintStateImages(IRender* pRender)
{
	if (IsReadOnly()) {
		return;
	}

	if (IsFocused()) {
		if (m_pFocusedImage != nullptr) {
			PaintImage(pRender, m_pFocusedImage);
		}		
		PaintPromptText(pRender);
	}
	else {
		__super::PaintStateImages(pRender);
		PaintPromptText(pRender);
	}
}

void RichEdit::SetNoSelOnKillFocus(bool bNoSel)
{
	m_bNoSelOnKillFocus = bNoSel;
}

void RichEdit::SetSelAllOnFocus(bool bSelAll)
{
	m_bSelAllOnFocus = bSelAll;
}

void RichEdit::SetNoCaretReadonly()
{
	m_bNoCaretReadonly = true;
}

void RichEdit::SetSaveSelection(bool fSaveSelection)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetSaveSelection(fSaveSelection);
	}
}

void RichEdit::SetHideSelection(bool fHideSelection)
{
	if (m_pRichHost != nullptr) {
		m_pRichHost->SetHideSelection(fHideSelection);
	}
}

void RichEdit::AddColorText(const std::wstring &str, const std::wstring &color)
{
	if( !IsRichText() || str.empty() || color.empty() ) {
		ASSERT(FALSE);
		return;
	}
	UiColor dwColor = GetUiColor(color);

	CHARFORMAT2W cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2W);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = dwColor.ToCOLORREF();

	ReplaceSel(str, FALSE);
	int len = GetTextLength();
	SetSel(len - (int)str.size(), len);
	SetSelectionCharFormat(cf);

	SetSelNone();
	GetDefaultCharFormat(cf);
	SetSelectionCharFormat(cf);
}

void RichEdit::AddLinkColorText(const std::wstring &str, const std::wstring &color, const std::wstring &linkInfo)
{
	if( !IsRichText() || str.empty() || color.empty() ) {
		ASSERT(FALSE);
		return;
	}
	UiColor dwColor = GetUiColor(color);

	CHARFORMAT2W cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2W);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = dwColor.ToCOLORREF();

	ReplaceSel(str, FALSE);
	int len = GetTextLength();
	SetSel(len - (int)str.size(), len);
	SetSelectionCharFormat(cf);
	LinkInfo info;
	info.info = linkInfo;
	m_richCtrl.TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&(info.cr));
	m_linkInfo.push_back(info);

	SetSelNone();
	GetDefaultCharFormat(cf);
	SetSelectionCharFormat(cf);
}
void  RichEdit::AddLinkColorTextEx(const std::wstring& str, const std::wstring &color, const std::wstring &linkInfo, const std::wstring& strFontId)
{
	if (!IsRichText() || str.empty() || color.empty()) {
		ASSERT(FALSE);
		return;
	}
	
	std::string link;
	std::string text;
	std::string font_face;
	StringHelper::UnicodeToMBCS(linkInfo, link);
	StringHelper::UnicodeToMBCS(str, text);
	LOGFONT lf = {0,};
	if (strFontId.empty()) {
		RichEditHost::GetLogFont(m_sFontId.c_str(), lf);
	}
	else {
		RichEditHost::GetLogFont(strFontId, lf);
	}
	StringHelper::UnicodeToMBCS(lf.lfFaceName, font_face);
	UiColor dwTextColor = GlobalManager::Instance().Color().GetColor(color);
	static std::string font_format = "{\\fonttbl{\\f0\\fnil\\fcharset%d %s;}}";
	static std::string color_format = "{\\colortbl ;\\red%d\\green%d\\blue%d;}";
	static std::string link_format = "{\\rtf1%s%s\\f0\\fs%d{\\field{\\*\\fldinst{HYPERLINK \"%s\"}}{\\fldrslt{\\cf1 %s}}}}";
	char sfont[255] = { 0 };
	sprintf_s(sfont, font_format.c_str(), lf.lfCharSet, font_face.c_str());
	char scolor[255] = { 0 };
	sprintf_s(scolor, color_format.c_str(), dwTextColor.GetR(), dwTextColor.GetG(), dwTextColor.GetB());
	char slinke[1024] = { 0 };
	sprintf_s(slinke, link_format.c_str(), sfont, scolor, ((int)(-lf.lfHeight *1.5))/2*2, link.c_str(), text.c_str());
	std::wstring temp;
	StringHelper::MBCSToUnicode(slinke, temp);
	SETTEXTEX st;
	st.codepage = ((UINT32)~((UINT32)0));
	st.flags = ST_SELECTION | ST_KEEPUNDO;
	m_richCtrl.TxSendMessage(EM_SETTEXTEX, (WPARAM)&st, (LPARAM)(LPCTSTR)slinke);
	return;
}
void RichEdit::AddLinkInfo(const CHARRANGE cr, const std::wstring &linkInfo)
{
	LinkInfo info;
	info.info = linkInfo;
	info.cr = cr;
	m_linkInfo.push_back(info);
}

void RichEdit::AddLinkInfoEx(const CHARRANGE cr, const std::wstring& linkInfo)
{
	CHARFORMAT2 cf2;
	ZeroMemory(&cf2, sizeof(CHARFORMAT2));
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_LINK;
	cf2.dwEffects |= CFE_LINK;

	SetSel(cr.cpMin, cr.cpMax);
	SetSelectionCharFormat(cf2);

	AddLinkInfo(cr, linkInfo);
}

//根据point来hittest自定义link的数据，返回true表示在link上，info是link的自定义属性
bool RichEdit::HittestCustomLink(UiPoint pt, std::wstring& info)
{
	bool bLink = false;
	info.clear();
	if (!m_linkInfo.empty()) {
		pt.Offset(GetScrollOffsetInScrollBox());
		int nCharIndex = CharFromPos(pt);
		for (auto it = m_linkInfo.begin(); it != m_linkInfo.end(); it++) {
			if ((*it).cr.cpMin <= nCharIndex && (*it).cr.cpMax > nCharIndex) {
				info = (*it).info.c_str();
				bLink = true;
				break;
			}
		}
	}
	return bLink;
}

void RichEdit::ClearImageCache()
{
	__super::ClearImageCache();
	if (m_pFocusedImage != nullptr) {
		m_pFocusedImage->ClearImageCache();
	}	
}

void RichEdit::SetTextPadding(UiPadding padding, bool bNeedDpiScale)
{
	ASSERT((padding.left >= 0) && (padding.top >= 0) && (padding.right >= 0) && (padding.bottom >= 0));
	if ((padding.left < 0) || (padding.top < 0) ||
		(padding.right < 0) || (padding.bottom < 0)) {
		return;
	}
	if (bNeedDpiScale) {
		GlobalManager::Instance().Dpi().ScalePadding(padding);
	}
	if (!GetTextPadding().Equals(padding)) {
		m_rcTextPadding.left = TruncateToUInt16(padding.left);
		m_rcTextPadding.top = TruncateToUInt16(padding.top);
		m_rcTextPadding.right = TruncateToUInt16(padding.right);
		m_rcTextPadding.bottom = TruncateToUInt16(padding.bottom);
		RelayoutOrRedraw();
	}
}

UiPadding RichEdit::GetTextPadding() const
{
	return UiPadding(m_rcTextPadding.left, m_rcTextPadding.top, m_rcTextPadding.right, m_rcTextPadding.bottom);
}

//----------------下面函数用作辅助 字节数限制
bool RichEdit::IsAsciiChar(const wchar_t ch)
{
	return (ch <= 0x7e && ch >= 0x20);
}

int RichEdit::GetAsciiCharNumber(const std::wstring &str)
{
	int len = (int)str.size(), sum = 0;
	for( int i = 0; i < len; i++ ) {
		if (IsAsciiChar(str[i])) {
			sum += 1;
		}
		else {
			sum += 2;
		}
	}
	return sum;
}

void RichEdit::LimitAsciiNumber(std::wstring &src, int limit)
{
	int len = (int)src.size(), sum = 0;
	for( int i = 0; i < len; i++ ) {
		if (IsAsciiChar(src[i])) {
			sum += 1;
		}
		else {
			sum += 2;
		}
		if( sum > limit ) {
			src.erase(i);
			break;
		}
	}
}

void RichEdit::GetClipboardText( std::wstring &out )
{
	out.clear();
	BOOL ret = ::OpenClipboard(NULL);
	if(ret) {
		if(::IsClipboardFormatAvailable(CF_UNICODETEXT)) {
			HANDLE h = ::GetClipboardData(CF_UNICODETEXT);
			if(h != INVALID_HANDLE_VALUE) {
				wchar_t* buf = (wchar_t*)::GlobalLock(h);
				if(buf != NULL)	{
					std::wstring str(buf, GlobalSize(h)/sizeof(wchar_t));
					out = str;
					::GlobalUnlock(h);
				}
			}
		}
		else if(::IsClipboardFormatAvailable(CF_TEXT)) {
			HANDLE h = ::GetClipboardData(CF_TEXT);
			if(h != INVALID_HANDLE_VALUE) {
				char* buf = (char*)::GlobalLock(h);
				if(buf != NULL)	{
					std::string str(buf, GlobalSize(h));
					StringHelper::MBCSToUnicode(str, out);

					::GlobalUnlock(h);
				}
			}
		}
		::CloseClipboard();
	}
}

void RichEdit::SetClipBoardText(const std::wstring &str)
{
	if (!::OpenClipboard(NULL)) {
		return;
	}

	if (!::EmptyClipboard()) {
		::CloseClipboard();
		return;
	}

	size_t len = str.length();
	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t)); //分配全局内存  
	if (!hMem) {
		::CloseClipboard();
		return;
	}

	wchar_t* lpStr = (wchar_t*)::GlobalLock(hMem); //锁住内存区 
	if (lpStr) {
		::memcpy(lpStr, str.c_str(), len * sizeof(wchar_t)); //把数据拷贝考全局内存中
		lpStr[len] = wchar_t(0); //字符串末尾设为'\0'
		::GlobalUnlock(hMem); //释放锁 
	}

	::SetClipboardData(CF_UNICODETEXT, hMem); //把内存中的数据放到剪切板上
	::CloseClipboard(); //关闭剪切板	
}

} // namespace ui
