#ifndef UI_CORE_WINDOW_H_
#define UI_CORE_WINDOW_H_

#pragma once

#include "duilib/Utils/Delegate.h"
#include "base/callback/callback.h"
#include "duilib/Core/ControlFinder.h"
#include "duilib/Core/ColorManager.h"
#include "duilib/Core/UiTypes.h"

#include <string>

namespace ui 
{

class Box;
class Control;
class IRender;
class Shadow;
class ToolTip;

/** @brief 窗口消息过滤接口，用于截获窗口过程的消息，优先于Window类进行消息处理
*/
class IUIMessageFilter
{
public:
	/** @brief 消息处理函数，处理优先级高于Window类的消息处理函数
	 * @param[in] uMsg 消息内容
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 返回 false 则继续派发该消息，返回 true 表示不再派发该消息
	 * @return 返回消息处理结果
	 */
	virtual LRESULT FilterMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/**@brief 窗口类
*  外部调用需要初始化的基本流程：
*  1. 调用Window::CreateWnd创建窗口；
*  2. Window::InitWnd；
*  3. Window::SetResourcePath；
*  4. WindowBuilder::Create, 得到Box* pRoot；
*  5. Window::AttachShadow(pRoot), 得到附加阴影的Box* pRoot, 以支持窗口阴影效果
*  6. Window::AttachBox(pRoot)
*/
class UILIB_API Window : public virtual nbase::SupportWeakCallback
{
public:
    Window();
	virtual ~Window();

public:
	/** @name 窗口创建、销毁、事件监听等相关接口
	* @{ 
	*/

	/**@brief 创建窗口
	 * @param[in] hwndParent 父窗口句柄
	 * @param[in] windowName 窗口名称
	 * @param[in] dwStyle 窗口样式
	 * @param[in] dwExStyle 窗口拓展样式, 可以设置层窗口（WS_EX_LAYERED）等属性
	 * @param[in] rc 窗口大小
	 * @return 返回窗口句柄
	 */
	virtual HWND CreateWnd(HWND hwndParent,
						   const wchar_t* windowName,
						   uint32_t dwStyle, 
						   uint32_t dwExStyle, 
						   const UiRect& rc = UiRect(0, 0, 0, 0));

	/**@brief 获取窗口所属的 Windows 句柄
	 */
    HWND GetHWND() const;

	/**@brief 子类化窗口（更改窗口过程函数）
	 * @param[in] 窗口句柄
	 * @return 新的窗口句柄
	 */
	HWND Subclass(HWND hWnd);

	/**@brief 取消子类化窗口（恢复原来的窗口过程函数）
	 */
	void Unsubclass();

	/**@brief 关闭窗口, 异步关闭，当函数返回后，IsClosing() 状态为true
	 * @param[in] 关闭消息
	 */
	virtual void CloseWnd(UINT nRet = IDOK);

	/**@brief 是否将要关闭
	 */
	bool IsClosingWnd() const { return m_bCloseing; };

	/**@brief 监听窗口关闭事件
	 * @param[in] callback 指定关闭后的回调函数
	 */
	void AttachWindowClose(const EventCallback& callback);

protected:
	/**@brief 创建窗口时被调用，由子类实现用以获取窗口唯一的类名称
	 * @return 基类返回空串，在子类中需实现并返回窗口唯一的类名称
	 */
	virtual std::wstring GetWindowClassName() const;

	/**@brief 获取控件窗口类
	 * @return 返回控件窗口类，该基类返回空串
	 */
	virtual std::wstring GetSuperClassName() const;

	/**@brief 获取窗口类的样式，该方法由实例化的子类实现，https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
	 * @return 返回窗口类的样式，该方法基类返回 CS_DBLCLKS
	 */
	virtual UINT GetClassStyle() const;

	/** 获取资源的句柄
	* @return 默认返回当前进程exe的句柄
	*/
	virtual HMODULE GetResModuleHandle() const;

	/**@brief 窗口接收到 WM_CREATE 消息时会被调用
	 * @param[in] hWnd 窗口句柄
	 */
	void InitWnd(HWND hWnd);

	/**@brief 在窗口收到 WM_NCDESTROY 消息时会被调用
	 * @param[in] hWnd 窗口句柄
	 */
	virtual void OnFinalMessage(HWND hWnd);

private:
	/**@brief 注册窗口类
	 */
    bool RegisterWindowClass();

	/**@brief 注册控件窗口类（与窗口的过程函数不同）
	 */
    bool RegisterSuperClass();

	/**@brief 窗口过程函数(当GetSuperClassName()函数返回不为空串时使用)
	 * @param[in] hWnd 窗口句柄
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息处理结果
	 */
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**@brief 窗口过程函数(当GetSuperClassName()函数返回空串时使用)
	 * @param[in] hWnd 窗口句柄
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息处理结果
	 */
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/** @} */

public:
	/** @name 窗口显示、隐藏、大小、位置等状态相关接口
	* @{
	*/

	/**@brief 显示或隐藏窗口
	 * @param[in] bShow 为 true 时显示窗口，为 false 时为隐藏窗口，默认为 true
	 * @param[in] bTakeFocus 是否获得焦点（激活窗口），默认为 true
	*/
	virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	/**@brief 显示模态对话框
	 * @param[in] parent_hwnd 父窗口句柄
	 */
	void ShowModalFake(HWND parent_hwnd);

	/**@brief 是否是模态显示
	*/
	bool IsFakeModal() const;

	/**@brief 居中窗口，支持扩展屏幕
	*/
	void CenterWindow();

	/** @brief 将窗口设置为置顶窗口
	*/
	void ToTopMost();

	/** @brief 将窗口调整为顶层窗口
	*/
	void BringToTop();

	/** @brief 激活窗口
	*          如果当前窗口为最小化，则进行还原；
	*          如果当前窗口为隐藏，则显示，并设置为前端窗口
	*          如果当前窗口为显示，则设置为前端窗口
	*/
	void ActiveWindow();

	/** 窗口是否为最大化状态
	*/
	bool IsWindowMaximized() const;

	/**@brief 获取窗口位置信息
	 * @param[in] bContainShadow 是否包含阴影，true 为包含，默认为 false 不包含
	 */
	UiRect GetPos(bool bContainShadow = false) const;

	/**@brief 设置窗口位置（对 SetWindowPos 的一层封装）
	 * @param[in] rc 窗口位置
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @param[in] uFlags 对应 SetWindowPos 的 uFlags 选项
	 * @param[in] hWndInsertAfter 对应 SetWindowPos 的 hWndInsertAfter 选项
	 * @param[in] bContainShadow rc区域是否包含阴影范围，默认为 false
	 */
	void SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false);

	/**@brief 重置窗口大小
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不根据 DPI 调整
	 */
	void Resize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief 设置窗口图标
	*  @param[in] nRes 窗口图标资源 ID
	*/
	void SetIcon(UINT nRes);

	/**@brief 设置窗口标题栏文本
	 * @param[in] strText 窗口标题栏文本
	 */
	void SetText(const std::wstring& strText);

	/**@brief 根据语言列表中的文本 ID， 根据ID设置窗口标题栏文本
	 * @param[in] strTextId 语言 ID，该 ID 必须在语言文件中存在
	 */
	void SetTextId(const std::wstring& strTextId);

	/** @} */

public:
	/** @name 窗口初始大小、标题栏等相关接口
	* @{
	*/
	/**@brief 获取窗口四边可拉伸范围的大小
	 */
	const UiRect& GetSizeBox() const;

	/**@brief 设置窗口四边可拉伸范围的大小
	 * @param[in] rcSizeBox 要设置的大小
	 */
	void SetSizeBox(const UiRect& rcSizeBox);

	/**@brief 获取窗口标题栏区域（可拖动区域），对应 XML 中 caption 属性
	 */
	const UiRect& GetCaptionRect() const;

	/**@brief 设置窗口标题栏区域
	 * @param[in] rcCaption 要设置的区域范围
	 */
	void SetCaptionRect(const UiRect& rcCaption);

	/**@brief 获取窗口圆角大小，对应 XML 中 roundcorner 属性
	 */
	const UiSize& GetRoundCorner() const;

	/**@brief 设置窗口圆角大小
	 * @param[in] cx 圆角宽
	 * @param[in] cy 圆角高
	 */
	void SetRoundCorner(int cx, int cy);

	/**@brief 获取窗口最大化信息
	 */
	const UiRect& GetMaximizeInfo() const;

	/**@brief 设置窗口最大化信息
	 * @param[in] rcMaximize 要设置的最大化数值
	 */
	void SetMaximizeInfo(const UiRect& rcMaximize);

	/**@brief 获取透明通道修补范围的的九宫格描述，对应 XML 中 alphafixcorner 属性
	 */
	const UiRect& GetAlphaFixCorner() const;

	/**@brief 设置透明通道修补范围的的九宫格描述
	 * @param[in] rc 要设置的修补范围
	 */
	void SetAlphaFixCorner(const UiRect& rc);

	/**@brief 获取窗口最小范围，对应 XML 中 mininfo 属性
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 */
	UiSize GetMinInfo(bool bContainShadow = false) const;

	/**@brief 设置窗口最小范围
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 */
	void SetMinInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief 获取窗口最大范围，对应 XML 中 maxinfo 属性
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 */
	UiSize GetMaxInfo(bool bContainShadow = false) const;

	/**@brief 设置窗口最大范围
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 */
	void SetMaxInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**@brief 获取窗口初始大小
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 */
	UiSize GetInitSize(bool bContainShadow = false) const;

	/**@brief 设置窗口初始大小
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不根据 DPI 调整
	 */
	void SetInitSize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/** @}*/

public:
	/** @name 窗口布局相关接口
	* @{
	*/
	/**@brief 绑定窗口的顶层容器
	 * @param[in] pRoot 容器指针
	 */
	bool AttachBox(Box* pRoot);

	/**@brief 获取窗口最外层的容器
	*/
	Box* GetRoot() const;

	/**@brief 获取窗口资源路径
	*/
	const std::wstring& GetResourcePath() const;

	/**@brief 设置窗口资源路径
	 * @param[in] strPath 要设置的路径
	 */
	void SetResourcePath(const std::wstring& strPath);

	/**@brief 初始化控件，在容器中添加控件时会被调用（用于对控件名称做缓存）
	 * @param[in] pControl 控件指针
	 * @param[in] pParent 控件父容器
	 */
	bool InitControls(Control* pControl, Box* pParent = NULL);

	/**@brief 初始化布局
	*/
	virtual void OnInitLayout();

	/**@brief 回收控件
	 * @param[in] pControl 控件指针
	 */
	void ReapObjects(Control* pControl);

	/**@brief 延迟销毁一个控件
	 * @param[in] pControl 控件指针
	 */
	void AddDelayedCleanup(Control* pControl);

	/**@brief 添加一个通用样式
	 * @param[in] strClassName 通用样式的名称
	 * @param[in] strControlAttrList 通用样式的 XML 转义格式数据
	 */
	void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/**@brief 获取指定通用样式的内容
	 * @param[in] strClassName 通用样式名称
	 * @return 返回指定名称的通用样式内容，XML 转义格式数据
	 */
	std::wstring GetClassAttributes(const std::wstring& strClassName) const;

	/**@brief 删除一个通用样式
	 * @param[in] strClassName 要删除的通用样式名称
	 */
	bool RemoveClass(const std::wstring& strClassName);

	/**@brief 删除所有通用样式
	 */
	void RemoveAllClass();

	/**@brief 添加一个颜色值提供窗口内使用
	 * @param[in] strName 颜色名称（如 white）
	 * @param[in] strValue 颜色具体数值（如 #FFFFFFFF）
	 */
	void AddTextColor(const std::wstring& strName, const std::wstring& strValue);

	/**@brief 添加一个颜色值提供窗口内使用
	 * @param[in] strName 颜色名称（如 white）
	 * @param[in] argb 颜色具体数值, 以ARGB格式表示
	 */
	void AddTextColor(const std::wstring& strName, UiColor argb);

	/**@brief 根据名称获取一个颜色的具体数值
	 * @param[in] strName 要获取的颜色名称
	 * @return 返回 DWORD 格式的颜色描述值
	 */
	UiColor GetTextColor(const std::wstring& strName) const;

	/**@brief 添加一个选项组
	 * @param[in] strGroupName 组名称
	 * @param[in] pControl 控件指针
	 */
	bool AddOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**@brief 获取指定选项组中控件列表
	 * @param[in] strGroupName 指定组名称
	 * @return 返回该组下的所有控件列表
	 */
	std::vector<Control*>* GetOptionGroup(const std::wstring& strGroupName);

	/**@brief 删除一个选项组
	 * @param[in] strGroupName 组名称
	 * @param[in] pControl 控件名称
	 */
	void RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**@brief 删除所有选项组
	 */
	void RemoveAllOptionGroups();

	/** @}*/

public:
	/** @name 窗口消息相关的接口
	* @{
	*/
	/**@brief 添加一个消息过滤器，此时消息已经派发，该接口的消息处理优先级高于Window类的消息处理函数
	 * @param[in] pFilter 一个继承了 IUIMessageFilter 的对象实例，需要实现接口中的方法
	 */
	bool AddMessageFilter(IUIMessageFilter* pFilter);

	/**@brief 移除一个消息过滤器
	 * @param[in] pFilter 一个继承了 IUIMessageFilter 的对象实例
	 */
	bool RemoveMessageFilter(IUIMessageFilter* pFilter);

	/**@brief 发送消息，对 Windows SendMessage 的一层封装
	 * @param[in] uMsg 消息类型
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回窗口对消息的处理结果
	 */
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**@brief 投递一个消息到消息队列
	 * @param[in] uMsg 消息类型
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回窗口对消息的处理结果
	 */
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**@brief 主动发起一个消息, 发送给该窗口的事件回调管理器（m_OnEvent）中注册的消息处理函数
	 * @param[in] eventType 转化后的消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 */
	bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0);

	/**@brief 调用系统默认的窗口处理函数，对 CallWindowProc API 的一层封装
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息处理结果
	 */
	LRESULT CallDefaultWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	/**@brief 窗口消息的处理函数, 从系统接收到消息后，进入的第一个处理函数
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息的处理结果
	 */
	LRESULT WindowMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**@brief 窗口消息的派发函数
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 消息是否已经处理，
	               返回 true  表明已经成功处理消息，不需要再传递给窗口过程；
				   返回 false 表示将消息继续传递给窗口过程处理
	 * @return 返回消息的处理结果
	 */
	virtual LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	/**@brief 窗口消息的派发函数，优先调用内部处理函数，如果内部无处理逻辑，则调用Windows默认处理函数处理此消息
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 消息是否已经处理，返回 true 表明已经成功处理消息，否则将消息继续传递给窗口过程
	 * @return 返回消息的处理结果
	 */
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	/** @}*/

private:
	/** @name 私有窗口消息处理相关
	* @{
	*/
	//部分消息处理函数，以实现基本功能
	LRESULT OnCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcActivateMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcCalcSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNcHitTestMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnGetMinMaxInfoMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnWindowPosChangingMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnEraseBkGndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnPaintMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnMouseHoverMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseLeaveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseMoveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMouseWheelMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnRButtonDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonDoubleClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnRButtonDoubleClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnLButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnRButtonUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnContextMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnIMEStartCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnIMEEndCompositionMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSetFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKillFocusMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCharMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKeyDownMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnKeyUpMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSetCusorMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnNotifyMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCommandMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnCtlColorMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	LRESULT OnTouchMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnPointerMsgs(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);	

	//鼠标等按下消息处理函数
	void OnButtonDown(EventType eventType, WPARAM wParam, LPARAM lParam, const UiPoint& pt);

	//鼠标等弹起消息处理函数
	void OnButtonUp(EventType eventType, WPARAM wParam, LPARAM lParam, const UiPoint& pt);

	//鼠标移动消息处理函数
	void OnMouseMove(WPARAM wParam, LPARAM lParam, const UiPoint& pt);

	/**@brief 鼠标滚轮消息处理函数
	*  @param [in] wParam 滚轮旋转的距离，正值表示滚轮向前旋转，远离用户;负值表示滚轮向后向用户旋转。
	*  @param [in] lParam 含有特殊处理， 0表示严格按照wParam指定的距离滚动, 非0表示按照常规逻辑处理滚轮旋转的距离 
	*  @param [in] pt 鼠标所在的坐标值，为窗口客户区坐标 
	*/
	void OnMouseWheel(WPARAM wParam, LPARAM lParam, const UiPoint& pt);

	//清除鼠标键盘操作状态
	void ClearStatus();

	/**@brief 判断是否需要发送鼠标进入或离开消息
	 * @param[in] pt 鼠标当前位置
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回 true 需要发送鼠标进入或离开消息，返回 false 为不需要
	 */
	bool HandleMouseEnterLeave(const UiPoint& pt, WPARAM wParam, LPARAM lParam);

	/** @}*/

public:
	/** @name 窗口焦点相关接口
	* @{
	*/
	/**@brief 获取当前持有焦点的控件
	 */
	Control* GetFocus() const;

	/**@brief 设置焦点到指定控件上(设置窗口为焦点窗口，并设置该控件为焦点控件)
	 * @param[in] pControl 控件指针
	 */
	void SetFocus(Control* pControl);

	/**@brief 让控件失去焦点（不影响窗口焦点）
	 */
	void KillFocus();

	/**@brief 设置当要捕获的鼠标窗口句柄为当前绘制窗口
	 * @param[in]
	 */
	void SetCapture();

	/**@brief 当窗口不需要鼠标输入时释放资源
	 */
	void ReleaseCapture();

	/**@brief 判断当前是否捕获鼠标输入
	 */
	bool IsCaptured() const;

	/**@brief 获取当前鼠标在哪个控件上
	 */
	Control* GetHoverControl() const;

	/**@brief 获取鼠标最后的坐标
	 */
	const UiPoint& GetLastMousePos() const;

	/**@brief 切换控件焦点到下一个（或上一个）控件
	 * @param[in] bForward true 为上一个控件，否则为 false，默认为 true
	 */
	bool SetNextTabControl(bool bForward = true);

	/** @}*/

public:
	/** @name 窗口阴影、层窗口透明度相关接口
	* @{
	*/
	/** 附加窗口阴影
	*/
	virtual Box* AttachShadow(Box* pRoot);

	/**@brief 设置窗口是否附加阴影效果
	 * @param[in] bShadowAttached 为 true 时附加，false 时不附加
	 */
	void SetShadowAttached(bool bShadowAttached);

	/**@brief 获取是否附加阴影效果
	*/
	bool IsShadowAttached() const;

	/** 当前阴影效果值，是否为默认值
	*/
	bool IsUseDefaultShadowAttached() const;

	/** 设置当前阴影效果值，是否为默认值
	*/
	void SetUseDefaultShadowAttached(bool isDefault);

	/**@brief 获取阴影图片
	 */
	const std::wstring& GetShadowImage() const;

	/**@brief 设置窗口阴影图片
	 * @param[in] strImage 图片位置
	 */
	void SetShadowImage(const std::wstring& strImage);

	/**@brief 获取阴影的九宫格描述信息
	 */
	UiPadding GetShadowCorner() const;

	/**@brief 指定阴影素材的九宫格描述
	 * @param[in] padding 九宫格描述信息
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 */
	void SetShadowCorner(const UiPadding& padding, bool bNeedDpiScale = true);

	/**@brief 设置窗口透明度
	 * @param[in] nAlpha 透明度数值[0, 255]
	 */
	void SetWindowAlpha(int nAlpha);

	/**@brief 获取窗口透明度
	 * @param[in] nAlpha 透明度数值[0, 255]
	 */
	uint8_t GetWindowAlpha() const;

	/**@brief 设置是否为层窗口
	*/
	void SetLayeredWindow(bool bIsLayeredWindow);

	/**@brief 是否为层窗口
	*/
	bool IsLayeredWindow() const;

	/** @}*/

public:
	/** @name 窗口绘制相关接口
	* @{
	*/
	/**@brief 设置控件是否已经布局
	 * @param[in] bArrange true 为已经排列，否则为 false
	 */
	void SetArrange(bool bArrange);

	/**@brief 发出重绘消息
	 * @param[in] rcItem 重绘范围
	 */
	void Invalidate(const UiRect& rcItem);

	/**@brief 获取绘制区域 DC
	 */
	HDC GetPaintDC() const;

	/**@brief 获取绘制对象
	 */
	IRender* GetRender() const;

	/**@brief 绘制函数体
	 */
	void Paint();

	/**@brief 判断当前是否渲染透明图层
	 */
	bool IsRenderTransparent() const;

	/**@brief 设置渲染透明图层
	 * @param[in] bCanvasTransparent 设置 true 为渲染透明图层，否则为 false
	 */
	bool SetRenderTransparent(bool bCanvasTransparent);

	/**@brief 清理图片缓存
	*/
	void ClearImageCache();

	/** @} */

private:
	/**@name 动画效果相关接口
	* @{
	*/
	/**@brief 设置绘制偏移
	 * @param[in] renderOffset 偏移值
	 */
	void SetRenderOffset(UiPoint renderOffset);

	/**@brief 设置绘制偏移 x 坐标
	 * @param[in] renderOffsetX 坐标值
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**@brief 设置绘制偏移 y 坐标
	 * @param[in] renderOffsetY 坐标值
	 */
	void SetRenderOffsetY(int renderOffsetY);

	/** @} */

public:

	/**@name 控件查找相关接口
	* @{
	*/
	/**@brief 根据坐标查找指定控件，采用默认属性：UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST
	 * @param[in] pt 指定坐标
	 */
	Control* FindControl(const UiPoint& pt) const;

	/**
	 * @brief 根据坐标查找可以响应WM_CONTEXTMENU的控件
	 * @param[in] pt 指定坐标
	 */
	Control* FindContextMenuControl(const UiPoint* pt) const;

	/**@brief 根据控件名称查找控件
	 * @param[in] strName 控件名称
	 */
	Control* FindControl(const std::wstring& strName) const;

	/**@brief 根据坐标查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] pt 要查找的坐标
	 */
	Control* FindSubControlByPoint(Control* pParent, const UiPoint& pt) const;

	/**@brief 根据名字查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] strName 要查找的名称
	 */
	Control* FindSubControlByName(Control* pParent, const std::wstring& strName) const;

	/** @} */

public:
	/** 获取当前窗口的客户区矩形
	*/
	void GetClientRect(UiRect& rcClient) const;

	/** 获取指定窗口的客户区矩形
	*/
	void GetClientRect(HWND hWnd, UiRect& rcClient) const;

	/** 获取当前窗口的窗口区矩形
	*/
	void GetWindowRect(UiRect& rcWindow) const;

	/** 获取指定窗口的窗口区矩形
	*/
	void GetWindowRect(HWND hWnd, UiRect& rcWindow) const;

	/** 将屏幕坐标转换为当前窗口的客户区坐标
	*/
	void ScreenToClient(UiPoint& pt) const;

	/** 将屏幕坐标转换为指定窗口的客户区坐标
	*/
	void ScreenToClient(HWND hWnd, UiPoint& pt) const;

	/** 将当前窗口的客户区坐标转换为屏幕坐标
	*/
	void ClientToScreen(UiPoint& pt) const;

	/** 将指定窗口的客户区坐标转换为屏幕坐标
	*/
	void ClientToScreen(HWND hWnd, UiPoint& pt) const;

	/** 获取当前鼠标所在坐标
	*/
	void GetCursorPos(UiPoint& pt) const;

	/* 将rc的左上角坐标和右下角坐标点从相对于一个窗口的坐标空间转换为相对于另一个窗口的坐标空间
	*/
	void MapWindowRect(HWND hwndFrom, HWND hwndTo, UiRect& rc) const;

	/** 获取一个点对应的窗口句柄
	*/
	HWND WindowFromPoint(const UiPoint& pt) const;

	/** 获取当前窗口Owner窗口句柄
	*/
	HWND GetWindowOwner() const;

	/** 替换窗口函数
	*/
	WNDPROC SubclassWindow(HWND hWnd, WNDPROC pfnWndProc) const;

	/** 获取当前窗口所在显示器的工作区矩形，以虚拟屏幕坐标表示。
		请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
	*/
	bool GetMonitorWorkRect(UiRect& rcWork) const;

	/** 获取指定窗口所在显示器的工作区矩形，以虚拟屏幕坐标表示。 
	    请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
	*/
	bool GetMonitorWorkRect(HWND hWnd, UiRect& rcWork) const;

	/** 获取指定点所在显示器的工作区矩形，以虚拟屏幕坐标表示。
		请注意，如果显示器不是主显示器，则一些矩形的坐标可能是负值。
	*/
	bool GetMonitorWorkRect(const UiPoint& pt, UiRect& rcWork) const;

	/** 获取指定窗口所在显示器的显示器矩形和工作区矩形
	*/
	bool GetMonitorRect(HWND hWnd, UiRect& rcMonitor, UiRect& rcWork) const;

private:
	/** 检查并确保当前窗口为焦点窗口
	*/
	void CheckSetFocusWindow();

private:
	//窗口句柄
	HWND m_hWnd;

	//原来的窗口过程函数
	WNDPROC m_OldWndProc;

	//是否子类化（Subclass函数）
	bool m_bSubclassed;

	//事件回调管理器
	EventMap m_OnEvent;

private:
	//窗口的初始化大小
	UiSize m_szInitWindowSize;

	//窗口最小信息
	UiSize m_szMinWindow;

	//窗口最大信息
	UiSize m_szMaxWindow;	

	//窗口最大化信息
	UiRect m_rcMaximizeInfo;

	//窗口四边可拉伸范围信息
	UiRect m_rcSizeBox;

	//透明通道修补范围的的九宫格描述
	UiRect m_rcAlphaFix;

	//窗口圆角信息
	UiSize m_szRoundCorner;

	//标题栏区域信息
	UiRect m_rcCaption;

	//布局是否变化，如果变化(true)则需要重新计算布局
	bool m_bIsArranged;

	//布局是否需要初始化
	bool m_bFirstLayout;

private:
	//绘制DC
	HDC m_hDcPaint;

	//是否为层窗口
	bool m_bIsLayeredWindow;

	//窗口透明度(仅当使用层窗口时有效)
	uint8_t m_nWindowAlpha;

	//绘制时的偏移量（动画用）
	UiPoint m_renderOffset;

	//绘制引擎
	std::unique_ptr<IRender> m_render;

private:

	//绘制引擎
	std::unique_ptr<ToolTip> m_toolTip;
	
	//焦点控件
	Control* m_pFocus;
	
	//鼠标在悬停控件
	Control* m_pEventHover;

	/** 点击的控件：
		在 WM_LBUTTONDOWN/WM_RBUTTONDOWN/WM_LBUTTONDBLCLK 赋值
		在 WM_LBUTTONUP中/WM_KILLFOCUS 清空
	*/
	Control* m_pEventClick;

	/** 键盘按下的控件
	*   在 WM_KEYDOWN 赋值
	*   在 WM_KEYUP 清空
	*/
	Control* m_pEventKey;

	//鼠标所在位置
	UiPoint m_ptLastMousePos;

	//鼠标事件的捕获状态
	bool m_bMouseCapture;

private:
	//每个窗口的资源路径(相对于资源根目录的路径)
	std::wstring m_strResourcePath;

	//窗口配置中class名称与属性映射关系
	std::map<std::wstring, std::wstring> m_defaultAttrHash;

	//窗口颜色字符串与颜色值（ARGB）的映射关系
	ColorMap m_colorMap;

	//该窗口下每个Option group下的控件（即单选控件是分组的）
	std::map<std::wstring, std::vector<Control*>> m_mOptionGroup;

	//该窗口消息过滤器列表
	std::vector<IUIMessageFilter*> m_aMessageFilters;

	//延迟释放的控件接口
	std::vector<Control*> m_aDelayedCleanup;

	//异步关闭窗口回调函数资源有效性标志
	nbase::WeakCallbackFlag m_closeFlag;
	
	//窗口阴影
	std::unique_ptr<Shadow> m_shadow;

	//当前窗口是否显示为模态对话框
	bool m_bFakeModal = false;
	
	//窗口已经延迟关闭：add by djj 20200428 调用Close时会延迟Post WM_CLOSE, 这个期间需要有一个标识此种'待关闭状态'
	bool m_bCloseing = false;

private:
	/** 控件查找辅助类
	*/
	ControlFinder m_controlFinder;

	/** 窗口关联的容器，根节点
	*/
	Box* m_pRoot;
};

} // namespace ui

#endif // UI_CORE_WINDOW_H_
