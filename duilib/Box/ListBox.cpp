#include "ListBox.h"
#include "duilib/Core/ScrollBar.h"

namespace ui 
{

//多选的时候，是否显示选择背景色: 0 - 默认规则; 1 - 显示背景色; 2: 不显示背景色
enum ePaintSelectedColors
{
	PAINT_SELECTED_COLORS_DEFAULT = 0,
	PAINT_SELECTED_COLORS_YES = 1,
	PAINT_SELECTED_COLORS_NO = 2
};

ListBox::ListBox(Layout* pLayout) :
	ScrollBox(pLayout),
	m_bScrollSelect(false),
	m_bMultiSelect(false),
	m_uPaintSelectedColors(PAINT_SELECTED_COLORS_DEFAULT),
	m_bSelectNextWhenActiveRemoved(false),
	m_iCurSel(Box::InvalidIndex),
	m_pCompareFunc(nullptr),
	m_pCompareContext(nullptr)
{
}

std::wstring ListBox::GetType() const { return L"ListBox"; }

void ListBox::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == L"multi_select") {
		SetMultiSelect(strValue == L"true");
	}
	else if (strName == L"paint_selected_colors") {
		if (strValue == L"true") {
			m_uPaintSelectedColors = PAINT_SELECTED_COLORS_YES;
		}
		else {
			m_uPaintSelectedColors = PAINT_SELECTED_COLORS_NO;
		}
	}
	else if ((strName == L"scroll_select") || (strName == L"scrollselect")) {
		SetScrollSelect(strValue == L"true");
	}
	else if (strName == L"select_next_when_active_removed") {
		SetSelectNextWhenActiveRemoved(strValue == L"true");
	}
	else {
		ScrollBox::SetAttribute(strName, strValue);
	}
}

void ListBox::HandleEvent(const EventArgs& msg)
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
	if (IsMultiSelect() || (GetItemCount() == 0) || (GetCurSel() >= GetItemCount())) {
		//在无数据、支持多选、无选中项的情况下，不支持下面的单选逻辑
		bool bHandled = false;
		if (msg.Type == kEventKeyDown) {
			if (msg.chKey == VK_HOME) {
				if (IsHorizontalScrollBar()) {
					HomeLeft();
				}
				else {
					HomeUp();
				}
				bHandled = true;
			}
			else if (msg.chKey == VK_END) {
				if (IsHorizontalScrollBar()) {
					EndRight();
				}
				else {
					EndDown(false, false);
				}
				bHandled = true;
			}
		}
		if (!bHandled) {
			__super::HandleEvent(msg);			
		}
		return;
	}

	switch (msg.Type) {
	case kEventKeyDown:
		switch (msg.chKey) {
		case VK_UP:
			if (IsHorizontalScrollBar()) {
				//横向滚动条，向上1条
				SelectItemPrevious(true, true);
			}
			else {
				//不是横向滚动条，向上1行
				size_t nColumns = 0;
				size_t nRows = 0;
				GetDisplayItemCount(false, nColumns, nRows);
				SelectItemCountN(true, true, false, nColumns);
			}
			return;
		case VK_DOWN:
			if (IsHorizontalScrollBar()) {
				//横向滚动条，向下1条
				SelectItemNext(true, true);
			}
			else {
				//不是横向滚动条，向下1行
				size_t nColumns = 0;
				size_t nRows = 0;
				GetDisplayItemCount(false, nColumns, nRows);
				SelectItemCountN(true, true, true, nColumns);
			}
			return;
		case VK_LEFT:
			if (IsHorizontalScrollBar()) {
				//横向滚动条，向上1列
				size_t nColumns = 0;
				size_t nRows = 0;
				GetDisplayItemCount(false, nColumns, nRows);
				SelectItemCountN(true, true, false, nRows);
			}
			else {
				//不是横向滚动条，向上1条
				SelectItemPrevious(true, true);
			}
			return;
		case VK_RIGHT:
			if (IsHorizontalScrollBar()) {
				//横向滚动条，向下1行
				size_t nColumns = 0;
				size_t nRows = 0;
				GetDisplayItemCount(false, nColumns, nRows);
				SelectItemCountN(true, true, true, nRows);
			}
			else {
				//不是横向滚动条，向下1条
				SelectItemNext(true, true);
			}
			return;
		case VK_PRIOR:
			SelectItemPage(true, true, false, 0);
			return;
		case VK_NEXT:
			SelectItemPage(true, true, true, 0);
			return;
		case VK_HOME:
			SelectItemHome(true, true);
			return;
		case VK_END:
			SelectItemEnd(true, true);
			return;
		}
		break;
	case kEventMouseWheel:
	{
		int32_t deltaValue = GET_WHEEL_DELTA_WPARAM(msg.wParam);
		if (deltaValue > 0) {
			if (m_bScrollSelect) {
				SelectItemPage(true, true, false, deltaValue);
				return;
			}
			break;
		}
		else if(deltaValue < 0) {
			if (m_bScrollSelect) {
				SelectItemPage(true, true, true, deltaValue);
				return;
			}
			break;
		}
	}
	break;
	}

	ScrollBox::HandleEvent(msg);
}

bool ListBox::SelectItem(size_t iIndex)
{
	return SelectItem(iIndex, true, true);
}

size_t ListBox::SelectItemPrevious(bool bTakeFocus, bool bTriggerEvent)
{
	return SelectItemCountN(bTakeFocus, bTriggerEvent, false, 1);
}

size_t ListBox::SelectItemNext(bool bTakeFocus, bool bTriggerEvent)
{
	return SelectItemCountN(bTakeFocus, bTriggerEvent, true, 1);
}

size_t ListBox::SelectItemPageUp(bool bTakeFocus, bool bTriggerEvent)
{
	return SelectItemPage(bTakeFocus, bTriggerEvent, false, 0);
}

size_t ListBox::SelectItemPageDown(bool bTakeFocus, bool bTriggerEvent)
{
	return SelectItemPage(bTakeFocus, bTriggerEvent, true, 0);
}

size_t ListBox::SelectItemHome(bool bTakeFocus, bool bTriggerEvent)
{
	if (GetItemCount() == 0) {
		return Box::InvalidIndex;
	}
	size_t iIndex = 0;
	size_t nDestItemIndex = Box::InvalidIndex;
	if (OnFindSelectable(m_iCurSel, false, 1, true, false, nDestItemIndex)) {
		iIndex = nDestItemIndex;
	}
	size_t itemIndex = FindSelectable(iIndex, true);
	if (Box::IsValidItemIndex(itemIndex)) {		
		SelectItem(itemIndex, false, bTriggerEvent);
		itemIndex = SelectEnsureVisible(itemIndex, bTakeFocus);
	}
	return itemIndex;
}

size_t ListBox::SelectItemEnd(bool bTakeFocus, bool bTriggerEvent)
{
	if (GetItemCount() == 0) {
		return Box::InvalidIndex;
	}
	size_t iIndex = GetItemCount() - 1;
	size_t nDestItemIndex = Box::InvalidIndex;
	if (OnFindSelectable(m_iCurSel, true, 1, false, true, nDestItemIndex)) {
		iIndex = nDestItemIndex;
	}
	size_t itemIndex = FindSelectable(iIndex, false);
	if (Box::IsValidItemIndex(itemIndex)) {
		SelectItem(itemIndex, false, bTriggerEvent);
		itemIndex = SelectEnsureVisible(itemIndex, bTakeFocus);
	}
	return itemIndex;
}

size_t ListBox::SelectItemPage(bool bTakeFocus, bool bTriggerEvent, bool bForward, int32_t nDeltaValue)
{
	//Page Up / Page Down 键的翻页逻辑
	size_t itemIndex = Box::InvalidIndex;
	const size_t itemCount = GetItemCount();
	if (itemCount == 0) {
		return itemIndex;
	}

	bool bIsHorizontal = IsHorizontalScrollBar(); //是否为横向滚动条
	if (nDeltaValue == 0) {
		//计算nDeltaValue值
		nDeltaValue = bIsHorizontal ? GetRect().Width() : GetRect().Height();
	}
	if (nDeltaValue == 0) {
		if (m_iCurSel < itemCount) {
			EnsureVisible(m_iCurSel);
		}
		return itemIndex;
	}
	size_t nCountPerPage = 1;
	size_t nColumns = 0;
	size_t nRows = 0;
	size_t nTotalDisplayCount = GetDisplayItemCount(bIsHorizontal, nColumns, nRows);
	if ((nColumns > 0) && (nRows > 0)){
		nTotalDisplayCount = nColumns * nRows;
	}
	if (nTotalDisplayCount < 1) {
		nTotalDisplayCount = 1;
	}
	if (bIsHorizontal) {
		//只有横向滚动条，按横向处理
		nCountPerPage = nTotalDisplayCount * std::abs(nDeltaValue) / GetRect().Width();
		if (nCountPerPage > nRows) {
			nCountPerPage -= nRows; //减掉1行
		}		
	}
	else {
		//其他情况，按纵向滚动处理
		nCountPerPage = nTotalDisplayCount * std::abs(nDeltaValue) / GetRect().Height();
		if (nCountPerPage > nColumns) {
			nCountPerPage -= nColumns; //减掉1列
		}
	}
	if (nCountPerPage < 1) {
		nCountPerPage = 1;
	}
	return SelectItemCountN(bTakeFocus, bTriggerEvent, bForward, nCountPerPage);
}

size_t ListBox::SelectItemCountN(bool bTakeFocus, bool bTriggerEvent, bool bForward, size_t nCount)
{
	if (m_iCurSel >= GetItemCount()) {
		//当前无有效的选择项，无法操作
		return Box::InvalidIndex;
	}
	if (!IsSelectableItem(m_iCurSel)) {
		//如果当前选中项为不可选择项，无法操作
		return Box::InvalidIndex;
	}
	if ((nCount == 0) || (nCount == Box::InvalidIndex)){
		nCount = 1;
	}
	bool bExceedFirst = false; //已经到达第1条
	size_t iIndex = Box::InvalidIndex;
	if (!bForward) {
		//Page Up
		if (m_iCurSel > nCount) {
			iIndex = m_iCurSel - nCount;
		}
		else {
			bExceedFirst = true;
		}
	}
	else {
		//Page Down
		iIndex = m_iCurSel + nCount;
	}
	if (IsSelectableItem(iIndex)) {
		//目标子项存在，直接返回
		size_t itemIndex = iIndex;
		if (itemIndex < GetItemCount()) {
			SelectItem(itemIndex, false, bTriggerEvent);
			itemIndex = SelectEnsureVisible(itemIndex, bTakeFocus);
		}
		return itemIndex;
	}

	//可能需要预加载数据，如果有预加载行为，m_iCurSel的值可能发生变化
	size_t nDestItemIndex = Box::InvalidIndex;
	if (OnFindSelectable(m_iCurSel, bForward, nCount, false, false, nDestItemIndex)) {
		iIndex = nDestItemIndex;
		ASSERT(iIndex < GetItemCount());
		if (iIndex >= GetItemCount()) {
			return Box::InvalidIndex;
		}
	}
	else {
		if (bExceedFirst) {
			iIndex = 0;
		}
	}
	const size_t itemCount = GetItemCount();
	if (iIndex >= itemCount) {
		iIndex = itemCount - 1;
	}	
	size_t itemIndex = FindSelectable(iIndex, bForward);
	if (itemIndex < itemCount) {		
		SelectItem(itemIndex, false, bTriggerEvent);
		itemIndex = SelectEnsureVisible(itemIndex, bTakeFocus);
	}
	return itemIndex;
}

size_t ListBox::SelectEnsureVisible(size_t itemIndex, bool bTakeFocus)
{
	itemIndex = EnsureVisible(itemIndex);
	if (bTakeFocus) {
		Control* pSelectedControl = GetItemAt(itemIndex);
		if ((pSelectedControl != nullptr) && pSelectedControl->IsVisible()) {
			pSelectedControl->SetFocus();
		}
		ASSERT(pSelectedControl != nullptr);
		ASSERT(pSelectedControl->IsVisible());
		ASSERT(GetWindow()->GetFocus() == pSelectedControl);
	}
	return itemIndex;
}

bool ListBox::IsSelectableItem(size_t itemIndex) const
{
	bool bSelectable = false;
	Control* pControl = GetItemAt(itemIndex);
	if ((pControl != nullptr) &&
		pControl->IsSelectableType() &&
		pControl->IsVisible() &&
		pControl->IsEnabled()) {
		bSelectable = true;
	}
	return bSelectable;
}

size_t ListBox::GetDisplayItemCount(bool bIsHorizontal, size_t& nColumns, size_t& nRows) const
{
	std::map<int32_t, int32_t> rows;
	std::map<int32_t, int32_t> columns;
	size_t nCount = 0;
	UiRect boxRect = GetRect();
	const size_t nItemCount = GetItemCount();
	for (size_t nItemIndex = 0; nItemIndex < nItemCount; ++nItemIndex) {
		Control* pControl = GetItemAt(nItemIndex);
		if ((pControl == nullptr) || !pControl->IsVisible() || pControl->IsFloat()){
			continue;
		}

		bool bDisplayItem = false;
		const UiRect& rc = pControl->GetRect();
		if (bIsHorizontal) {
			if ((rc.left >= boxRect.left) && 
				(rc.right <= boxRect.right)) {
				if ((rc.top >= boxRect.top) && (rc.top < boxRect.bottom)) {
					bDisplayItem = true;
				}
				else if ((rc.bottom >= boxRect.top) && (rc.top < boxRect.bottom)) {
					bDisplayItem = true;
				}
			}
		}
		else {
			if ((rc.top >= boxRect.top) &&
				(rc.bottom <= boxRect.bottom)) {
				if ((rc.left >= boxRect.left) && (rc.left < boxRect.right)) {
					bDisplayItem = true;
				}
				else if ((rc.right >= boxRect.left) && (rc.right < boxRect.right)) {
					bDisplayItem = true;
				}
			}
		}
		if (bDisplayItem) {
			rows[pControl->GetRect().top] = 0;
			columns[pControl->GetRect().left] = 0;
			++nCount;
		}
	}
	nColumns = columns.size();
	nRows = rows.size();
	return nCount;
}

void ListBox::SendEvent(EventType eventType, WPARAM wParam, LPARAM lParam, TCHAR tChar, const UiPoint& mousePos)
{
	return ScrollBox::SendEvent(eventType, wParam, lParam, tChar, mousePos);
}

void ListBox::SendEvent(const EventArgs& event)
{
	ScrollBox::SendEvent(event);
}

size_t ListBox::GetCurSel() const
{
	return m_iCurSel;
}

void ListBox::SetCurSel(size_t iIndex)
{
	if (Box::IsValidItemIndex(iIndex)) {
		ASSERT(iIndex < GetItemCount());
		if (iIndex > GetItemCount()) {
			return;
		}
	}	
	m_iCurSel = iIndex;
}

bool ListBox::IsScrollSelect() const
{
	return m_bScrollSelect;
}

void ListBox::SetScrollSelect(bool bScrollSelect)
{
	m_bScrollSelect = bScrollSelect;
}

bool ListBox::IsSelectNextWhenActiveRemoved() const
{
	return m_bSelectNextWhenActiveRemoved;
}

void ListBox::SetSelectNextWhenActiveRemoved(bool bSelectNextItem)
{
	m_bSelectNextWhenActiveRemoved = bSelectNextItem;
}

void ListBox::GetSelectedItems(std::vector<size_t>& selectedIndexs) const
{
	selectedIndexs.clear();
	const size_t itemCount = GetItemCount();
	for (size_t iIndex = 0; iIndex < itemCount; ++iIndex) {
		Control* pControl = m_items[iIndex];
		if ((pControl == nullptr) || !pControl->IsVisible() || !pControl->IsEnabled()) {
			continue;
		}
		IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
		if (pListItem != nullptr) {
			if (pListItem->IsSelected()) {
				selectedIndexs.push_back(iIndex);
			}
		}
	}
}

size_t ListBox::FindSelectable(size_t iIndex, bool bForward) const
{
	return __super::FindSelectable(iIndex, bForward);
}

bool ListBox::OnFindSelectable(size_t /*nCurSel*/,
							   bool /*bForward*/, size_t /*nCount*/,
							   bool /*bHome*/, bool /*bEnd*/,
							   size_t& /*nDestItemIndex*/)
{
	return false;
}

bool ListBox::SelectItem(size_t iIndex, bool bTakeFocus, bool bTriggerEvent, uint64_t /*vkFlag*/)
{
	if (IsMultiSelect()) {
		//多选
		return SelectItemMulti(iIndex, bTakeFocus, bTriggerEvent);
	}
	else {
		//单选
		return SelectItemSingle(iIndex, bTakeFocus, bTriggerEvent);
	}
}

bool ListBox::UnSelectItem(size_t iIndex, bool bTriggerEvent)
{
	Control* pControl = GetItemAt(iIndex);
	if (pControl != nullptr) {
		IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
		if ((pListItem != nullptr) && pListItem->IsSelected()) {
			pListItem->OptionSelected(false, bTriggerEvent);
			if (bTriggerEvent) {
				SendEvent(kEventUnSelect, iIndex, Box::InvalidIndex);
			}
			//仅在状态变化时重绘
			Invalidate();
		}
	}
	if (iIndex == m_iCurSel) {
		m_iCurSel = Box::InvalidIndex;
	}
	return true;
}

void ListBox::OnItemSelectedChanged(size_t /*iIndex*/, IListBoxItem* /*pListBoxItem*/)
{
}

bool ListBox::SelectItemSingle(size_t iIndex, bool bTakeFocus, bool bTriggerEvent)
{
	//单选
	if (iIndex == m_iCurSel) {
		Control* pControl = GetItemAt(iIndex);
		if (pControl == nullptr) {
			m_iCurSel = Box::InvalidIndex;
			return false;
		}		
		//确保可见，然后返回		
		if (bTakeFocus) {
			pControl->SetFocus();
		}
		bool bChanged = false;
		IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
		if ((pListItem != nullptr) && !pListItem->IsSelected()) {
			bChanged = true;
			pListItem->OptionSelected(true, bTriggerEvent);
		}
		if (bChanged && bTriggerEvent) {
			SendEvent(kEventSelect, m_iCurSel, m_iCurSel);
		}
		Invalidate();
		return true;
	}
	const size_t iOldSel = m_iCurSel;
	if (Box::IsValidItemIndex(iOldSel)) {
		//取消旧选择项的选择状态
		Control* pControl = GetItemAt(iOldSel);
		if (pControl != nullptr) {
			IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
			if ((pListItem != nullptr) && pListItem->IsSelected()) {
				pListItem->OptionSelected(false, bTriggerEvent);
				if (bTriggerEvent) {
					SendEvent(kEventUnSelect, iOldSel, Box::InvalidIndex);
				}
			}
		}
		m_iCurSel = Box::InvalidIndex;
	}
	if (!Box::IsValidItemIndex(iIndex)) {
		Invalidate();
		return false;
	}

	Control* pControl = GetItemAt(iIndex);
	if ((pControl == nullptr) || !pControl->IsVisible() || !pControl->IsEnabled()) {
		Invalidate();
		return false;
	}
	IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
	if (pListItem == nullptr) {
		Invalidate();
		return false;
	}
	m_iCurSel = iIndex;
	//设置选择状态
	pListItem->OptionSelected(true, bTriggerEvent);
	pControl = GetItemAt(m_iCurSel);
	if (pControl != nullptr) {		
		if (bTakeFocus) {
			pControl->SetFocus();
		}
	}

	if (bTriggerEvent) {
		SendEvent(kEventSelect, m_iCurSel, iOldSel);
	}
	Invalidate();
	return true;
}

bool ListBox::SelectItemMulti(size_t iIndex, bool bTakeFocus, bool bTriggerEvent)
{
	//多选: m_iCurSel 始终执行最后一个选中项
	size_t iOldSel = m_iCurSel;
	m_iCurSel = Box::InvalidIndex;
	if (!Box::IsValidItemIndex(iIndex)) {
		Invalidate();
		return false;
	}
	Control* pControl = GetItemAt(iIndex);
	if ((pControl == nullptr) || !pControl->IsVisible() || !pControl->IsEnabled()){
		Invalidate();
		return false;
	}
	IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
	if (pListItem == nullptr) {
		Invalidate();
		return false;
	}
	
	if (pListItem->IsSelected()) {
		//多选时，再次选择时，按取消选择处理
		pListItem->OptionSelected(false, bTriggerEvent);
		if (bTriggerEvent) {
			SendEvent(kEventUnSelect, iIndex, Box::InvalidIndex);
		}
	}
	else {
		//如果原来是非选择状态，更新为选择状态
		m_iCurSel = iIndex;
		pListItem->OptionSelected(true, bTriggerEvent);		
		if (bTakeFocus) {			
			pControl->SetFocus();
		}
		if (bTriggerEvent) {
			SendEvent(kEventSelect, iIndex, iOldSel);
		}
	}	
	Invalidate();
	return true;
}

void ListBox::EnsureVisible(const UiRect& rcItem,
							ListBoxVerVisible vVisibleType,
							ListBoxHorVisible hVisibleType)
{
	UiRect rcNewItem = rcItem;
	UiSize scrollOffset = GetScrollOffset();
	rcNewItem.Offset(-scrollOffset.cx, -scrollOffset.cy);
	UiRect rcList = GetPos();
	rcList.Deflate(GetPadding());

	ScrollBar* pVScrollBar = GetVScrollBar();
	if (pVScrollBar && pVScrollBar->IsValid()) {
		if (IsVScrollBarAtLeft()) {
			ASSERT(pVScrollBar->GetFixedWidth().GetInt32() > 0);
			rcList.left += pVScrollBar->GetFixedWidth().GetInt32();
		}
		else {
			ASSERT(m_pVScrollBar->GetFixedWidth().GetInt32() > 0);
			rcList.right -= pVScrollBar->GetFixedWidth().GetInt32();
		}
	}

	ScrollBar* pHScrollBar = GetHScrollBar();
	if (pHScrollBar && pHScrollBar->IsValid()) {
		ASSERT(pHScrollBar->GetFixedHeight().GetInt32() > 0);
		rcList.bottom -= pHScrollBar->GetFixedHeight().GetInt32();
	}

	if ((rcNewItem.left >= rcList.left) && (rcNewItem.top >= rcList.top) && 
		(rcNewItem.right <= rcList.right) && (rcNewItem.bottom <= rcList.bottom)) {
		IListBoxItem* listBoxElement = dynamic_cast<IListBoxItem*>(GetParent());
		IListBoxOwner* lisBoxOwner = nullptr;
		if (listBoxElement != nullptr) {
			lisBoxOwner = listBoxElement->GetOwner();
		}
		if (lisBoxOwner != nullptr) {
			lisBoxOwner->EnsureVisible(rcNewItem, vVisibleType, hVisibleType);
		}		
		return;
	}
	//水平滚动条
	int32_t dx = 0;
	if (hVisibleType == ListBoxHorVisible::kVisibleAtCenter) {
		//居中显示
		if (rcNewItem.left < rcList.CenterX()) {
			dx = rcNewItem.left - rcList.CenterX();
		}
		if (rcNewItem.right > rcList.CenterX()) {
			dx = rcNewItem.right - rcList.CenterX();
		}
	}
	else if (hVisibleType == ListBoxHorVisible::kVisibleAtLeft) {
		//靠左显示
		dx = rcNewItem.left - rcList.left;
	}
	else if (hVisibleType == ListBoxHorVisible::kVisibleAtRight) {
		//靠右显示
		dx = rcNewItem.right - rcList.right;
	}
	else {
		if (rcNewItem.left < rcList.left) {
			dx = rcNewItem.left - rcList.left;
		}
		if (rcNewItem.right > rcList.right) {
			dx = rcNewItem.right - rcList.right;
		}
		UiRect rcNewList = rcList;
		rcNewList.top = rcNewItem.top;
		rcNewList.bottom = rcNewItem.bottom;
		if (rcNewItem.ContainsRect(rcNewList)) {
			//子项的区域，已经在可见区域，不调整，避免鼠标点击后左右晃动
			dx = 0;
		}
	}
	//垂直滚动条
	int32_t dy = 0;
	if (vVisibleType == ListBoxVerVisible::kVisibleAtCenter) {
		//居中显示
		if (rcNewItem.top < rcList.CenterY()) {
			dy = rcNewItem.top - rcList.CenterY();
		}
		if (rcNewItem.bottom > rcList.CenterY()) {
			dy = rcNewItem.bottom - rcList.CenterY();
		}
	}
	else if (vVisibleType == ListBoxVerVisible::kVisibleAtTop) {
		//顶部对齐
		dy = rcNewItem.top - rcList.top;
	}
	else if (vVisibleType == ListBoxVerVisible::kVisibleAtBottom) {
		//底部对齐
		dy = rcNewItem.bottom - rcList.bottom;
	}
	else {
		//只要可见即可
		if (rcNewItem.top < rcList.top) {
			dy = rcNewItem.top - rcList.top;
		}
		if (rcNewItem.bottom > rcList.bottom) {
			dy = rcNewItem.bottom - rcList.bottom;
		}
		UiRect rcNewList = rcList;
		rcNewList.left = rcNewItem.left;
		rcNewList.right = rcNewItem.right;
		if (rcNewItem.ContainsRect(rcNewList)) {
			//子项的区域，已经在可见区域，不调整，避免鼠标点击后上下晃动
			dy = 0;
		}
	}
	if ((dx != 0) || (dy != 0)) {
		UiSize64 sz = GetScrollPos();
		SetScrollPos(UiSize64(sz.cx + dx, sz.cy + dy));
		Invalidate();
	}
}

void ListBox::StopScroll()
{
	StopScrollAnimation();
}

bool ListBox::CanPaintSelectedColors(bool bHasStateImages) const
{
	if (m_uPaintSelectedColors == PAINT_SELECTED_COLORS_YES) {
		return true;
	}
	else if (m_uPaintSelectedColors == PAINT_SELECTED_COLORS_NO) {
		return false;
	}
	if (bHasStateImages && IsMultiSelect()) {
		//如果有CheckBox，多选的时候，默认不显示选择背景色
		return false;
	}
	return true;
}

bool ListBox::ButtonDown(const EventArgs& msg)
{
	bool ret = __super::ButtonDown(msg);
	StopScroll();
	return ret;
}

bool ListBox::IsHorizontalLayout() const
{
	LayoutType type = GetLayout()->GetLayoutType();
	bool bHorizontal = false;
	if ((type == LayoutType::HLayout) ||
		(type == LayoutType::HTileLayout) ||
		(type == LayoutType::VirtualHLayout) ||
		(type == LayoutType::VirtualHTileLayout)) {
		bHorizontal = true;
	}
	return bHorizontal;
}

bool ListBox::IsHorizontalScrollBar() const
{
	bool bHasVScrollBar = false;
	ScrollBar* pVScrollBar = GetVScrollBar();
	if (pVScrollBar && pVScrollBar->IsValid()) {
		bHasVScrollBar = true;
	}

	bool bHasHScrollBar = false;
	ScrollBar* pHScrollBar = GetHScrollBar();
	if (pHScrollBar && pHScrollBar->IsValid()) {
		bHasHScrollBar = true;
	}

	bool bIsHorizontal = (bHasHScrollBar && !bHasVScrollBar) ? true : false;
	LayoutType type = GetLayout()->GetLayoutType();
	if ((type == LayoutType::HLayout) ||
		(type == LayoutType::HTileLayout) ||
		(type == LayoutType::VirtualHLayout) ||
		(type == LayoutType::VirtualHTileLayout)) {
		//确定是横向布局
		bIsHorizontal = true;
	}
	else if (type == LayoutType::ListCtrlReportLayout) {
		bIsHorizontal = false;
	}
	return bIsHorizontal;
}

bool ListBox::ScrollItemToTop(size_t iIndex)
{
	Control* pControl = GetItemAt(iIndex);
	if ((pControl == nullptr) || !pControl->IsVisible()) {
		return false;
	}
	if (IsHorizontalLayout()) {
		//横向布局
		if (GetScrollRange().cx != 0) {
			UiSize64 scrollPos = GetScrollPos();
			scrollPos.cx = pControl->GetPos().left - GetPosWithoutPadding().left;
			if (scrollPos.cx >= 0) {
				SetScrollPos(scrollPos);
				return true;
			}
		}
	}
	else {
		//纵向布局
		if (GetScrollRange().cy != 0) {
			UiSize64 scrollPos = GetScrollPos();
			scrollPos.cy = pControl->GetPos().top - GetPosWithoutPadding().top;
			if (scrollPos.cy >= 0) {
				SetScrollPos(scrollPos);
				return true;
			}
		}
	}
	return false;
}

bool ListBox::ScrollItemToTop(const std::wstring& itemName)
{
	const size_t itemCount = m_items.size();
	for (size_t iIndex = 0; iIndex < itemCount; ++iIndex) {
		Control* pControl = m_items[iIndex];
		if ((pControl == nullptr) || !pControl->IsVisible()) {
			continue;
		}
		if (pControl->IsNameEquals(itemName)) {
			return ScrollItemToTop(iIndex);
		}
	}
	return false;
}

Control* ListBox::GetTopItem() const
{
	if (IsHorizontalLayout()) {
		//横向布局
		int32_t listLeft = GetPos().left + GetPadding().left + GetScrollOffset().cx;
		for (Control* pControl : m_items) {
			ASSERT(pControl != nullptr);
			if (pControl->IsVisible() && !pControl->IsFloat() && pControl->GetPos().right >= listLeft) {
				return pControl;
			}
		}
	}
	else {
		//纵向布局
		int32_t listTop = GetPos().top + GetPadding().top + GetScrollOffset().cy;
		for (Control* pControl : m_items) {
			ASSERT(pControl != nullptr);
			if (pControl->IsVisible() && !pControl->IsFloat() && pControl->GetPos().bottom >= listTop) {
				return pControl;
			}
		}
	}
	return nullptr;
}

bool ListBox::SetItemIndex(Control* pControl, size_t iIndex)
{
	size_t iOrginIndex = GetItemIndex(pControl);
	if (!Box::IsValidItemIndex(iOrginIndex)) {
		return false;
	}
	if (iOrginIndex == iIndex) {
		return true;
	}

	IListBoxItem* pSelectedListItem = nullptr;
	if (Box::IsValidItemIndex(m_iCurSel)) {
		pSelectedListItem = dynamic_cast<IListBoxItem*>(GetItemAt(m_iCurSel));
	}
	if (!ScrollBox::SetItemIndex(pControl, iIndex)) {
		return false;
	}
	size_t iMinIndex = std::min(iOrginIndex, iIndex);
	size_t iMaxIndex = std::max(iOrginIndex, iIndex);
	for(size_t i = iMinIndex; i < iMaxIndex + 1; ++i) {
		Control* pItemControl = GetItemAt(i);
		IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pItemControl);
		if( pListItem != NULL ) {
			pListItem->SetListBoxIndex(i);
		}
	}
	if (Box::IsValidItemIndex(m_iCurSel) && pSelectedListItem != nullptr) {
		m_iCurSel = pSelectedListItem->GetListBoxIndex();
	}
	return true;
}

size_t ListBox::EnsureVisible(size_t iIndex, ListBoxVerVisible vVisibleType, ListBoxHorVisible hVisibleType)
{
	Control* pControl = GetItemAt(iIndex);
	ASSERT(pControl != nullptr);
	if (pControl != nullptr) {
		UiRect rcItem = pControl->GetPos();
		EnsureVisible(rcItem, vVisibleType, hVisibleType);
		ASSERT(GetItemAt(iIndex) == pControl);
	}
	return iIndex;
}

bool ListBox::AddItem(Control* pControl)
{
	// Override the AddItem() method so we can add items specifically to
	// the intended widgets. Headers are assumed to be
	// answer the correct interface so we can add multiple list headers.
	// The list items should know about us
	IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
	if( pListItem != nullptr) {
		pListItem->SetOwner(this);
		pListItem->SetListBoxIndex(GetItemCount());
	}
	return ScrollBox::AddItem(pControl);
}

bool ListBox::AddItemAt(Control* pControl, size_t iIndex)
{
	// Override the AddItemAt() method so we can add items specifically to
	// the intended widgets. Headers and are assumed to be
	// answer the correct interface so we can add multiple list headers.

	if (!ScrollBox::AddItemAt(pControl, iIndex)) {
		return false;
	}

	// The list items should know about us
	IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(pControl);
	if( pListItem != nullptr ) {
		pListItem->SetOwner(this);
		pListItem->SetListBoxIndex(iIndex);
	}

	const size_t itemCount = GetItemCount();
	for(size_t i = iIndex + 1; i < itemCount; ++i) {
		Control* p = GetItemAt(i);
		pListItem = dynamic_cast<IListBoxItem*>(p);
		if( pListItem != nullptr ) {
			pListItem->SetListBoxIndex(i);
		}
	}
	if (Box::IsValidItemIndex(m_iCurSel) && (m_iCurSel >= iIndex)) {
		m_iCurSel += 1;
	}
	return true;
}

bool ListBox::RemoveItem(Control* pControl)
{
	size_t iIndex = GetItemIndex(pControl);
	if (!Box::IsValidItemIndex(iIndex)) {
		return false;
	}
	return RemoveItemAt(iIndex);
}

bool ListBox::RemoveItemAt(size_t iIndex)
{
	if (!ScrollBox::RemoveItemAt(iIndex)) {
		return false;
	}
	const size_t itemCount = GetItemCount();
	for(size_t i = iIndex; i < itemCount; ++i) {
		Control* p = GetItemAt(i);
		IListBoxItem* pListItem = dynamic_cast<IListBoxItem*>(p);
		if (pListItem != nullptr) {
			pListItem->SetListBoxIndex(i);
		}
	}

	if (Box::IsValidItemIndex(m_iCurSel)) {
		if (iIndex == m_iCurSel) {
			if (!IsMultiSelect() && m_bSelectNextWhenActiveRemoved) {
				SelectItem(FindSelectable(m_iCurSel--, false));
			}
			else {
				m_iCurSel = Box::InvalidIndex;
			}
		}
		else if (iIndex < m_iCurSel) {
			m_iCurSel -= 1;
		}
	}
	return true;
}

void ListBox::RemoveAllItems()
{
	m_iCurSel = Box::InvalidIndex;
	ScrollBox::RemoveAllItems();
}

bool ListBox::SortItems(PFNCompareFunc pfnCompare, void* pCompareContext)
{
	if (pfnCompare == nullptr) {
		return false;
	}		
	if (m_items.empty()) {
		return true;
	}

	m_pCompareFunc = pfnCompare;
	m_pCompareContext = pCompareContext;
	qsort_s(&(*m_items.begin()), m_items.size(), sizeof(Control*), ListBox::ItemComareFunc, this);	
	IListBoxItem* pItem = nullptr;
	const size_t itemCount = m_items.size();
	for (size_t i = 0; i < itemCount; ++i) {
		pItem = dynamic_cast<IListBoxItem*>(m_items[i]);
		if (pItem != nullptr) {
			pItem->SetListBoxIndex(i);
			//取消界面上所有的选择项
			pItem->OptionSelected(false, false);
		}
	}
	SelectItem(Box::InvalidIndex);
	SetPos(GetPos());
	Invalidate();
	return true;
}

int __cdecl ListBox::ItemComareFunc(void *pvlocale, const void *item1, const void *item2)
{
	ListBox *pThis = (ListBox*)pvlocale;
	if (!pThis || !item1 || !item2) {
		return 0;
	}
	return pThis->ItemComareFunc(item1, item2);
}

int __cdecl ListBox::ItemComareFunc(const void *item1, const void *item2)
{
	Control *pControl1 = *(Control**)item1;
	Control *pControl2 = *(Control**)item2;
	return m_pCompareFunc(pControl1, pControl2, m_pCompareContext);
}

bool ListBox::IsMultiSelect() const
{
	return m_bMultiSelect;
}

void ListBox::SetMultiSelect(bool bMultiSelect)
{
	m_bMultiSelect = bMultiSelect;
	if (!bMultiSelect) {
		//只要bMultiSelect为false，就执行取消现有多选的逻辑
		//此处不能判断与原值是否变化来决定是否执行下面的代码，否则可能会影响子类（TreeView的逻辑）
		if (OnSwitchToSingleSelect()) {
			Invalidate();
		}
	}
}

void ListBox::EnsureSingleSelection()
{
	if (!IsMultiSelect()) {
		OnSwitchToSingleSelect();
	}	
}

bool ListBox::OnSwitchToSingleSelect()
{
	bool bChanged = false;
	IListBoxItem* pItem = nullptr;
	const size_t itemCount = m_items.size();
	if (m_iCurSel > itemCount) { 
		//如果单选状态不同步，使用第一个选择的作为最终单选的选择项
		for (size_t i = 0; i < itemCount; ++i) {
			pItem = dynamic_cast<IListBoxItem*>(m_items[i]);
			if ((pItem != nullptr) && pItem->IsSelected()) {
				m_iCurSel = i;
				break;
			}
		}
	}
	for (size_t i = 0; i < itemCount; ++i) {
		pItem = dynamic_cast<IListBoxItem*>(m_items[i]);
		if ((pItem != nullptr) && pItem->IsSelected()) {
			if (m_iCurSel != i) {
				pItem->OptionSelected(false, false); //不触发Select事件
				bChanged = true;
			}
		}
	}
	if (UpdateCurSelItemSelectStatus()) {
		bChanged = true;
	}
	return bChanged;
}

bool ListBox::UpdateCurSelItemSelectStatus()
{
	//同步当前选择项的状态
	bool bChanged = false;
	size_t curSelIndex = GetCurSel();
	if (Box::IsValidItemIndex(curSelIndex)) {
		bool bSelectItem = false;
		IListBoxItem* pItem = dynamic_cast<IListBoxItem*>(GetItemAt(curSelIndex));
		if (pItem != nullptr) {
			bSelectItem = pItem->IsSelected();
		}
		if (!bSelectItem) {
			SetCurSel(Box::InvalidIndex);
			bChanged = true;
		}
	}
	return bChanged;
}

} // namespace ui
