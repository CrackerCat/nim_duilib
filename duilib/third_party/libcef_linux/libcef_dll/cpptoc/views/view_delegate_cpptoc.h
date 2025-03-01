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
// $hash=db736a11e47229a28b185549646379fdce00fd14$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_DELEGATE_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_DELEGATE_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/views/cef_view_capi.h"
#include "include/capi/views/cef_view_delegate_capi.h"
#include "include/views/cef_view.h"
#include "include/views/cef_view_delegate.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefViewDelegateCppToC : public CefCppToCRefCounted<CefViewDelegateCppToC,
                                                         CefViewDelegate,
                                                         cef_view_delegate_t> {
 public:
  CefViewDelegateCppToC();
  virtual ~CefViewDelegateCppToC();
};

constexpr auto CefViewDelegateCppToC_Wrap = CefViewDelegateCppToC::Wrap;
constexpr auto CefViewDelegateCppToC_Unwrap = CefViewDelegateCppToC::Unwrap;

#endif  // CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_DELEGATE_CPPTOC_H_
