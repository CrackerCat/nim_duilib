// Copyright (c) 2023 The Chromium Embedded Framework Authors. All rights
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
// $hash=91946337612dd212dfcba794d6843b6dbe64fb69$
//

#include "libcef_dll/ctocpp/media_access_callback_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
void CefMediaAccessCallbackCToCpp::Continue(uint32 allowed_permissions) {
  shutdown_checker::AssertNotShutdown();

  cef_media_access_callback_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, cont))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cont(_struct, allowed_permissions);
}

NO_SANITIZE("cfi-icall") void CefMediaAccessCallbackCToCpp::Cancel() {
  shutdown_checker::AssertNotShutdown();

  cef_media_access_callback_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, cancel))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cancel(_struct);
}

// CONSTRUCTOR - Do not edit by hand.

CefMediaAccessCallbackCToCpp::CefMediaAccessCallbackCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefMediaAccessCallbackCToCpp::~CefMediaAccessCallbackCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_media_access_callback_t* CefCToCppRefCounted<
    CefMediaAccessCallbackCToCpp,
    CefMediaAccessCallback,
    cef_media_access_callback_t>::UnwrapDerived(CefWrapperType type,
                                                CefMediaAccessCallback* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefMediaAccessCallbackCToCpp,
                                   CefMediaAccessCallback,
                                   cef_media_access_callback_t>::kWrapperType =
    WT_MEDIA_ACCESS_CALLBACK;
