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
// $hash=43efcad65a525c79f63c51049ab38f3c2ed4c56f$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_DOWNLOAD_HANDLER_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_DOWNLOAD_HANDLER_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_download_handler_capi.h"
#include "include/cef_download_handler.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefDownloadHandlerCppToC
    : public CefCppToCRefCounted<CefDownloadHandlerCppToC,
                                 CefDownloadHandler,
                                 cef_download_handler_t> {
 public:
  CefDownloadHandlerCppToC();
  virtual ~CefDownloadHandlerCppToC();
};

constexpr auto CefDownloadHandlerCppToC_Wrap = CefDownloadHandlerCppToC::Wrap;
constexpr auto CefDownloadHandlerCppToC_Unwrap =
    CefDownloadHandlerCppToC::Unwrap;

#endif  // CEF_LIBCEF_DLL_CPPTOC_DOWNLOAD_HANDLER_CPPTOC_H_
