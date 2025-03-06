#include "BrowserBox.h"
#include "browser/MultiBrowserForm.h"
#include "browser/MultiBrowserManager.h"
#include "taskbar/TaskbarManager.h"

using namespace ui;
using namespace std;

BrowserBox::BrowserBox(ui::Window* pWindow, std::string id):
    ui::VBox(pWindow)
{
#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
    m_pTaskBarItem = nullptr;
#endif
    m_pBrowserForm = nullptr;
    m_pCefControl = nullptr;
    m_browserId = id;
}

MultiBrowserForm* BrowserBox::GetBrowserForm() const
{
    ASSERT(nullptr != m_pBrowserForm);
    ASSERT(m_pBrowserForm->IsWindow());
    return m_pBrowserForm;
}

ui::CefControlBase* BrowserBox::GetCefControl()
{
    return m_pCefControl;
}

DString& BrowserBox::GetTitle()
{
    return m_title;
}

void BrowserBox::InitBrowserBox(const DString &url)
{
    m_pCefControl = static_cast<ui::CefControlBase*>(FindSubControl(_T("cef_control")));
    ASSERT(m_pCefControl != nullptr);
    if (m_pCefControl == nullptr) {
        return;
    }
    m_pCefControl->AttachBeforeContextMenu(UiBind(&BrowserBox::OnBeforeContextMenu, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    m_pCefControl->AttachContextMenuCommand(UiBind(&BrowserBox::OnContextMenuCommand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    m_pCefControl->AttachContextMenuDismissed(UiBind(&BrowserBox::OnContextMenuDismissed, this, std::placeholders::_1, std::placeholders::_2));

    m_pCefControl->AttachTitleChange(UiBind(&BrowserBox::OnTitleChange, this, std::placeholders::_1, std::placeholders::_2));
    m_pCefControl->AttachUrlChange(UiBind(&BrowserBox::OnUrlChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_pCefControl->AttachMainUrlChange(UiBind(&BrowserBox::OnMainUrlChange, this, std::placeholders::_1, std::placeholders::_2));

    m_pCefControl->AttachBeforePopup(UiBind(&BrowserBox::OnBeforePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8, std::placeholders::_9, std::placeholders::_10, std::placeholders::_11, std::placeholders::_12, std::placeholders::_13));
    m_pCefControl->AttachBeforePopupAborted(UiBind(&BrowserBox::OnBeforePopupAborted, this, std::placeholders::_1, std::placeholders::_2));

    m_pCefControl->AttachBeforeNavigate(UiBind(&BrowserBox::OnBeforeNavigate, this, std::placeholders::_1, std::placeholders::_2));
    m_pCefControl->AttachLoadingStateChange(UiBind(&BrowserBox::OnLoadingStateChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_pCefControl->AttachLoadStart(UiBind(&BrowserBox::OnLoadStart, this));
    m_pCefControl->AttachLoadEnd(UiBind(&BrowserBox::OnLoadEnd, this, std::placeholders::_1));
    m_pCefControl->AttachLoadError(UiBind(&BrowserBox::OnLoadError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 加载默认网页
    DString html_path = url;
    if (html_path.empty()) {
        ui::FilePath localPath = ui::FilePathUtil::GetCurrentModuleDirectory();
        localPath.NormalizeDirectoryPath();
#if defined (DUILIB_BUILD_FOR_WIN)
        localPath += _T("resources\\themes\\default\\cef\\cef.html");
#else
        localPath += _T("resources/themes/default/cef/cef.html");
#endif
        html_path = localPath.ToString();
    }
    m_pCefControl->LoadURL(html_path);

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
    // 初始化任务栏缩略图
    if (GetWindow()->IsLayeredWindow()) {
        m_pTaskBarItem = new TaskbarTabItem(this);
        if (m_pTaskBarItem) {
            m_pTaskBarItem->Init(url, m_browserId);
        }
    }
#endif

    // Box获取焦点时把焦点转移给Cef控件
    this->AttachSetFocus([this](const ui::EventArgs& param)->bool
    {
        m_pCefControl->SetFocus();
        return true;
    }); 
}

void BrowserBox::UninitBrowserBox()
{
    MultiBrowserManager::GetInstance()->RemoveBorwserBox(m_browserId, this);
#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
    if (m_pTaskBarItem) {
        m_pTaskBarItem->UnInit();
    }
#endif
}

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
TaskbarTabItem* BrowserBox::GetTaskbarItem()
{
    return m_pTaskBarItem;
}
#endif

void BrowserBox::SetWindow(Window* pWindow)
{
    m_pBrowserForm = dynamic_cast<MultiBrowserForm*>(pWindow);
    ASSERT(nullptr != m_pBrowserForm);

    BaseClass::SetWindow(pWindow);
}

void BrowserBox::Invalidate()
{
    BaseClass::Invalidate();
#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
    if (m_pTaskBarItem) {
        m_pTaskBarItem->InvalidateTab();
    }
#endif
}

void BrowserBox::SetPos(UiRect rc)
{
    BaseClass::SetPos(rc);
#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_BUILD_FOR_SDL)
    if (m_pTaskBarItem) {
        m_pTaskBarItem->InvalidateTab();
    }
#endif
}

void BrowserBox::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model)
{
}

bool BrowserBox::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefContextMenuParams> params,
                                      int command_id,
                                      CefContextMenuHandler::EventFlags event_flags)
{
    return false;
}

void BrowserBox::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
}

void BrowserBox::OnTitleChange(CefRefPtr<CefBrowser> browser, const DString& title)
{
    m_title = title;
    m_pBrowserForm->SetTabItemName(ui::StringConvert::UTF8ToT(m_browserId), title);
}

void BrowserBox::OnUrlChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const DString& url)
{
    m_url = url;
    m_pBrowserForm->SetURL(m_browserId, url);
}

void BrowserBox::OnMainUrlChange(const DString& oldUrl, const DString& newUrl)
{
}

bool BrowserBox::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int popup_id,
                               const CefString& target_url,
                               const CefString& target_frame_name,
                               CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                               bool user_gesture,
                               const CefPopupFeatures& popupFeatures,
                               CefWindowInfo& windowInfo,
                               CefRefPtr<CefClient>& client,
                               CefBrowserSettings& settings,
                               CefRefPtr<CefDictionaryValue>& extra_info,
                               bool* no_javascript_access)
{
    if (frame != nullptr) {
        frame->LoadURL(target_url);
    }
    return true;
}

void BrowserBox::OnBeforePopupAborted(CefRefPtr<CefBrowser> browser, int popup_id)
{

}

cef_return_value_t BrowserBox::OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect)
{
    // 返回RV_CANCEL截断导航
    return RV_CONTINUE;
}

void BrowserBox::OnLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward)
{
    return;
}

void BrowserBox::OnLoadStart()
{
    return;
}

void BrowserBox::OnLoadEnd(int httpStatusCode)
{
    // 注册一个方法提供前端调用
    m_pCefControl->RegisterCppFunc(_T("ShowMessageBox"), ToWeakCallback([this](const std::string& params, ui::ReportResultFunction callback) {
        DString value = ui::StringConvert::UTF8ToT(params);
        ui::SystemUtil::ShowMessageBox(GetWindow(), value.c_str(), _T("C++ 接收到 JavaScript 发来的消息"));
        callback(false, R"({ "message": "Success." })");
    }));
}

void BrowserBox::OnLoadError(CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
    return;
}
