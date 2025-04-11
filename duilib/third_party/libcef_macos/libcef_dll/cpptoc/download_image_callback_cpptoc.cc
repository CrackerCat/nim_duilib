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
// $hash=7f927fa7ea2b0ee7f9866df2434e9072728317e4$
//

#include "libcef_dll/cpptoc/download_image_callback_cpptoc.h"

#include "libcef_dll/ctocpp/image_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK download_image_callback_on_download_image_finished(
    struct _cef_download_image_callback_t* self,
    const cef_string_t* image_url,
    int http_status_code,
    struct _cef_image_t* image) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return;
  }
  // Verify param: image_url; type: string_byref_const
  DCHECK(image_url);
  if (!image_url) {
    return;
  }
  // Unverified params: image

  // Execute
  CefDownloadImageCallbackCppToC::Get(self)->OnDownloadImageFinished(
      CefString(image_url), http_status_code, CefImageCToCpp_Wrap(image));
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefDownloadImageCallbackCppToC::CefDownloadImageCallbackCppToC() {
  GetStruct()->on_download_image_finished =
      download_image_callback_on_download_image_finished;
}

// DESTRUCTOR - Do not edit by hand.

CefDownloadImageCallbackCppToC::~CefDownloadImageCallbackCppToC() {
  shutdown_checker::AssertNotShutdown();
}

template <>
CefRefPtr<CefDownloadImageCallback> CefCppToCRefCounted<
    CefDownloadImageCallbackCppToC,
    CefDownloadImageCallback,
    cef_download_image_callback_t>::UnwrapDerived(CefWrapperType type,
                                                  cef_download_image_callback_t*
                                                      s) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType
    CefCppToCRefCounted<CefDownloadImageCallbackCppToC,
                        CefDownloadImageCallback,
                        cef_download_image_callback_t>::kWrapperType =
        WT_DOWNLOAD_IMAGE_CALLBACK;
