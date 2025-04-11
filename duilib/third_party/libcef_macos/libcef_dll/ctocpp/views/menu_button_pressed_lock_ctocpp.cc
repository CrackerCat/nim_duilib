// Copyright (c) 2025 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=73be899e4fffe33233d25d242ea3a754a830f1e6$
//

#include "libcef_dll/ctocpp/views/menu_button_pressed_lock_ctocpp.h"

#include "libcef_dll/shutdown_checker.h"

// CONSTRUCTOR - Do not edit by hand.

CefMenuButtonPressedLockCToCpp::CefMenuButtonPressedLockCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefMenuButtonPressedLockCToCpp::~CefMenuButtonPressedLockCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_menu_button_pressed_lock_t* CefCToCppRefCounted<
    CefMenuButtonPressedLockCToCpp,
    CefMenuButtonPressedLock,
    cef_menu_button_pressed_lock_t>::UnwrapDerived(CefWrapperType type,
                                                   CefMenuButtonPressedLock*
                                                       c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType
    CefCToCppRefCounted<CefMenuButtonPressedLockCToCpp,
                        CefMenuButtonPressedLock,
                        cef_menu_button_pressed_lock_t>::kWrapperType =
        WT_MENU_BUTTON_PRESSED_LOCK;
