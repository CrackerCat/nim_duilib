#include "CefControlBase.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Utils/StringConvert.h"

#include "duilib/CEFControl/internal/app/CefJSBridge.h"
#include "duilib/CEFControl/internal/handler/CefBrowserHandler.h"

namespace ui {

CefControlBase::CefControlBase(ui::Window* pWindow):
    ui::Control(pWindow)
{
    m_bAttachedDevTools = false;
    //这个标记必须为false，否则绘制有问题
    SetUseCache(false);
}

CefControlBase::~CefControlBase(void)
{

}
void CefControlBase::LoadURL(const CefString& url)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        CefRefPtr<CefFrame> frame = m_pBrowserHandler->GetBrowser()->GetMainFrame();
        if (!frame) {
            return;
        }
        frame->LoadURL(url);
    }
    else {
        if (m_pBrowserHandler.get()) {
            ui::StdClosure cb = ToWeakCallback([this, url]() {
                LoadURL(url);
            });
            m_pBrowserHandler->AddAfterCreateTask(cb);
        }
    }
}

void CefControlBase::GoBack()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->GoBack();
    }
}

void CefControlBase::GoForward()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->GoForward();
    }
}

bool CefControlBase::CanGoBack()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->CanGoBack();
    }
    return false;
}

bool CefControlBase::CanGoForward()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->CanGoForward();
    }
    return false;
}

void CefControlBase::Refresh()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->Reload();
    }
}

void CefControlBase::StopLoad()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->StopLoad();
    }
}

bool CefControlBase::IsLoading()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->IsLoading();
    }
    return false;
}

void CefControlBase::StartDownload(const CefString& url)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        m_pBrowserHandler->GetBrowser()->GetHost()->StartDownload(url);
    }
}

void CefControlBase::SetZoomLevel(float zoom_level)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        m_pBrowserHandler->GetBrowser()->GetHost()->SetZoomLevel(zoom_level);
    }
}

HWND CefControlBase::GetCefHandle() const
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowserHost().get()) {
        return m_pBrowserHandler->GetBrowserHost()->GetWindowHandle();
    }
    return nullptr;
}

CefString CefControlBase::GetURL()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return m_pBrowserHandler->GetBrowser()->GetMainFrame()->GetURL();
    }
    return CefString();
}

std::string CefControlBase::GetUTF8URL()
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get()) {
        return ui::StringConvert::WStringToUTF8((const wchar_t*)GetURL().c_str());
    }
    return CefString();
}

CefString CefControlBase::GetMainURL(const CefString& url)
{
    std::string temp = url.ToString();
    size_t end_pos = temp.find("#") == std::string::npos ? temp.length() : temp.find("#");
    temp = temp.substr(0, end_pos);
    return CefString(temp.c_str());
}

bool CefControlBase::RegisterCppFunc(const DString& function_name, ui::CppFunction function, bool global_function/* = false*/)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get() && m_jsBridge.get()) {
        return m_jsBridge->RegisterCppFunc(ui::StringConvert::TToUTF8(function_name).c_str(), function, global_function ? nullptr : m_pBrowserHandler->GetBrowser());
    }
    return false;
}

void CefControlBase::UnRegisterCppFunc(const DString& function_name)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get() && m_jsBridge.get()) {
        m_jsBridge->UnRegisterCppFunc(ui::StringConvert::TToUTF8(function_name).c_str(), m_pBrowserHandler->GetBrowser());
    }
}

bool CefControlBase::CallJSFunction(const DString& js_function_name, const DString& params, ui::CallJsFunctionCallback callback, const DString& frame_name /*= _T("")*/)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get() && m_jsBridge.get()) {
        CefRefPtr<CefFrame> frame;
        if (frame_name.empty()) {
            frame = m_pBrowserHandler->GetBrowser()->GetMainFrame();
        }
        else {
#if CEF_VERSION_MAJOR <= 109
            //CEF 109版本
            frame = m_pBrowserHandler->GetBrowser()->GetFrame(frame_name);
#else
            //CEF 高版本
            frame = m_pBrowserHandler->GetBrowser()->GetFrameByName(frame_name);
#endif
        }

        if (!m_jsBridge->CallJSFunction(ui::StringConvert::TToUTF8(js_function_name).c_str(),
            ui::StringConvert::TToUTF8(params).c_str(), frame, callback)) {
            return false;
        }
        return true;
    }
    return false;
}

bool CefControlBase::CallJSFunction(const DString& js_function_name, const DString& params, ui::CallJsFunctionCallback callback, const CefString& frame_id)
{
    if (m_pBrowserHandler.get() && m_pBrowserHandler->GetBrowser().get() && m_jsBridge.get()) {
        CefRefPtr<CefFrame> frame;
        if (frame_id.empty()) {
            frame = m_pBrowserHandler->GetBrowser()->GetMainFrame();
        }
        else {
#if CEF_VERSION_MAJOR <= 109
            //CEF 109版本
            frame = m_pBrowserHandler->GetBrowser()->GetFrame(StringUtil::StringToInt64(frame_id.c_str()));
#else
            //CEF 高版本
            frame = m_pBrowserHandler->GetBrowser()->GetFrameByIdentifier(frame_id);
#endif
        }
        if (!m_jsBridge->CallJSFunction(ui::StringConvert::TToUTF8(js_function_name).c_str(),
            ui::StringConvert::TToUTF8(params).c_str(), frame, callback)) {
            return false;
        }
        return true;
    }
    return false;
}

void CefControlBase::RepairBrowser()
{
    ReCreateBrowser();
}

void CefControlBase::DettachDevTools()
{
    if (!m_bAttachedDevTools) {
        return;
    }
    auto browser = m_pBrowserHandler->GetBrowser();
    if ((browser != nullptr) && (browser->GetHost() != nullptr)) {
        browser->GetHost()->CloseDevTools();
        m_bAttachedDevTools = false;
        if (m_pfnDevToolVisibleChange != nullptr) {
            m_pfnDevToolVisibleChange(m_bAttachedDevTools);
        }
    }
    else {
        m_bAttachedDevTools = false;
    }
}

bool CefControlBase::IsAttachedDevTools() const
{
    return m_bAttachedDevTools;
}

void CefControlBase::SetAttachedDevTools(bool bAttachedDevTools)
{
    m_bAttachedDevTools = bAttachedDevTools;
}

void CefControlBase::OnDevToolsVisibleChanged()
{
    if (m_pfnDevToolVisibleChange != nullptr) {
        m_pfnDevToolVisibleChange(IsAttachedDevTools());
    }
}

void CefControlBase::OnPaint(CefRefPtr<CefBrowser> /*browser*/, CefRenderHandler::PaintElementType /*type*/, const CefRenderHandler::RectList& /*dirtyRects*/, const std::string* /*buffer*/, int /*width*/, int /*height*/)
{
    //必须不使用缓存，否则绘制异常
    ASSERT(IsUseCache() == false);
}

void CefControlBase::ClientToControl(UiPoint&/*pt*/)
{
}

void CefControlBase::OnPopupShow(CefRefPtr<CefBrowser> /*browser*/, bool /*show*/)
{
}

void CefControlBase::OnPopupSize(CefRefPtr<CefBrowser> /*rowser*/, const CefRect& /*rect*/)
{
}

void CefControlBase::UpdateWindowPos()
{
    this->SetPos(this->GetPos());
}

void CefControlBase::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    if (m_pfnBeforeMenu) {
        m_pfnBeforeMenu(params, model);
    }
}

bool CefControlBase::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags)
{
    if (m_pfnMenuCommand) {
        return m_pfnMenuCommand(params, command_id, event_flags);
    }
    return false;
}

void CefControlBase::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
    if (m_pfnUrlChange) {
        m_pfnUrlChange(url);
    }

    if (frame->IsMain()) {
        auto old_url = m_url;
        m_url = frame->GetURL();
        if (m_pfnMainUrlChange != nullptr && GetMainURL(old_url).compare(GetMainURL(m_url)) != 0) {
            m_pfnMainUrlChange(old_url, m_url);
        }
    }
}

void CefControlBase::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    if (m_pfnTitleChange) {
        m_pfnTitleChange(title);
    }
}

void CefControlBase::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
    if (m_pfnLoadStateChange) {
        m_pfnLoadStateChange(isLoading, canGoBack, canGoForward);
    }
}

void CefControlBase::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    if (m_pfnLoadStart) {
        m_pfnLoadStart();
    }
}

void CefControlBase::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
    if (m_pfnLoadEnd) {
        m_pfnLoadEnd(httpStatusCode);
    }
}

void CefControlBase::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
    if (m_pfnLoadError) {
        m_pfnLoadError(errorCode, errorText, failedUrl);
    }
}

bool CefControlBase::OnBeforePopup(CefRefPtr<CefBrowser> /*browser*/,
                                   CefRefPtr<CefFrame> /*frame*/,
                                   const CefString& target_url,
                                   const CefString& /*target_frame_name*/,
                                   CefLifeSpanHandler::WindowOpenDisposition /*target_disposition*/,
                                   bool /*user_gesture*/,
                                   const CefPopupFeatures& /*popupFeatures*/,
                                   CefWindowInfo& /*windowInfo*/,
                                   CefRefPtr<CefClient>& /*client*/,
                                   CefBrowserSettings& /*settings*/,
                                   bool* /*no_javascript_access*/)
{
    if (m_pfnLinkClick && !target_url.empty()) {
        return m_pfnLinkClick(target_url);
    }
    return false;
}

bool CefControlBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if (m_pfnAfterCreated) {
        return m_pfnAfterCreated(browser);
    }
    return false;
}

void CefControlBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if (m_pfnBeforeClose) {
        m_pfnBeforeClose(browser);
    }
}

bool CefControlBase::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
    bool result = false; 
    if (m_pfnBeforeBrowse) {
        result = m_pfnBeforeBrowse(browser, frame, request, is_redirect);
    }
    return result;
}

void CefControlBase::OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution)
{
    if (m_pfnProtocolExecution) {
        m_pfnProtocolExecution(browser, url, allow_os_execution);
    }
}

cef_return_value_t CefControlBase::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback)
{
    if (m_pfnBeforeResourceLoad) {
        return m_pfnBeforeResourceLoad(request, false);
    }
    return RV_CONTINUE;
}

void CefControlBase::OnRenderProcessTerminated(CefRefPtr<CefBrowser> /*browser*/, CefRequestHandler::TerminationStatus /*status*/)
{
    return;
}

void CefControlBase::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{
    if (m_pfnBeforeDownload) {
        m_pfnBeforeDownload(browser, download_item, suggested_name, callback);
    }
}

void CefControlBase::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
{
    if (m_pfnDownloadUpdated) {
        m_pfnDownloadUpdated(browser, download_item, callback);
    }
}

bool CefControlBase::OnFileDialog(CefRefPtr<CefBrowser> browser, CefDialogHandler::FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)
{
    if (m_pfnFileDialog) {
        return m_pfnFileDialog(mode, title, default_file_path, accept_filters, selected_accept_filter, callback);
    }
    return false;        
}

bool CefControlBase::OnExecuteCppFunc(const CefString& function_name, const CefString& params, int js_callback_id, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    if (m_jsBridge.get()) {
        m_jsCallbackThreadId = ui::GlobalManager::Instance().Thread().GetCurrentThreadIdentifier();
        return m_jsBridge->ExecuteCppFunc(function_name, params, js_callback_id, browser, frame);
    }
    return false;
}

bool CefControlBase::OnExecuteCppCallbackFunc(int cpp_callback_id, const CefString& json_string)
{
    if (m_jsBridge.get()) {
        if (m_jsCallbackThreadId != -1) {
            ui::GlobalManager::Instance().Thread().PostTask(m_jsCallbackThreadId, [this, cpp_callback_id, json_string]
            {
                m_jsBridge->ExecuteCppCallbackFunc(cpp_callback_id, json_string);
            });
        }
        else {
            return m_jsBridge->ExecuteCppCallbackFunc(cpp_callback_id, json_string);
        }

    }
    return false;
}

} //namespace ui
