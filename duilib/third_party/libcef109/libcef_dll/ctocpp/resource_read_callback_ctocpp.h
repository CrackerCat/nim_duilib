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
// $hash=aeb2eaecc30bb2498b709af0ec45dd6b5dc9b392$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_RESOURCE_READ_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_RESOURCE_READ_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_resource_handler_capi.h"
#include "include/cef_resource_handler.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefResourceReadCallbackCToCpp
    : public CefCToCppRefCounted<CefResourceReadCallbackCToCpp,
                                 CefResourceReadCallback,
                                 cef_resource_read_callback_t> {
 public:
  CefResourceReadCallbackCToCpp();
  virtual ~CefResourceReadCallbackCToCpp();

  // CefResourceReadCallback methods.
  void Continue(int bytes_read) override;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_RESOURCE_READ_CALLBACK_CTOCPP_H_
