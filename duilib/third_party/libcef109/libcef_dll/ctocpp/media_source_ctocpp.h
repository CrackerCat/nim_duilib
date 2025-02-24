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
// $hash=36c26e21479231ade66738aee8f10c01f9d0cd22$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_MEDIA_SOURCE_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_MEDIA_SOURCE_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_media_router_capi.h"
#include "include/cef_media_router.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefMediaSourceCToCpp : public CefCToCppRefCounted<CefMediaSourceCToCpp,
                                                        CefMediaSource,
                                                        cef_media_source_t> {
 public:
  CefMediaSourceCToCpp();
  virtual ~CefMediaSourceCToCpp();

  // CefMediaSource methods.
  CefString GetId() override;
  bool IsCastSource() override;
  bool IsDialSource() override;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_MEDIA_SOURCE_CTOCPP_H_
