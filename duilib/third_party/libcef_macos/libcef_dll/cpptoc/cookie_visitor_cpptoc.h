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
// $hash=a433ca11b5e260d855c2e98534e95a849cde7f83$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_cookie_capi.h"
#include "include/cef_cookie.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefCookieVisitorCppToC
    : public CefCppToCRefCounted<CefCookieVisitorCppToC,
                                 CefCookieVisitor,
                                 cef_cookie_visitor_t> {
 public:
  CefCookieVisitorCppToC();
  virtual ~CefCookieVisitorCppToC();
};

constexpr auto CefCookieVisitorCppToC_Wrap = CefCookieVisitorCppToC::Wrap;
constexpr auto CefCookieVisitorCppToC_Unwrap = CefCookieVisitorCppToC::Unwrap;

#endif  // CEF_LIBCEF_DLL_CPPTOC_COOKIE_VISITOR_CPPTOC_H_
