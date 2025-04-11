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
// $hash=943da8df2e5d11ec5039c4b54be44658d1b9bea9$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_TEST_API_VERSION_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_TEST_API_VERSION_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/test/cef_api_version_test_capi.h"
#include "include/test/cef_api_version_test.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

#if CEF_API_REMOVED(13301)

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefApiVersionTestRefPtrLibraryChildChildCToCpp
    : public CefCToCppRefCounted<
          CefApiVersionTestRefPtrLibraryChildChildCToCpp,
          CefApiVersionTestRefPtrLibraryChildChild,
          cef_api_version_test_ref_ptr_library_child_child_t> {
 public:
  CefApiVersionTestRefPtrLibraryChildChildCToCpp();
  virtual ~CefApiVersionTestRefPtrLibraryChildChildCToCpp();

  // CefApiVersionTestRefPtrLibraryChildChild methods.
  int GetOtherOtherValue() override;
  void SetOtherOtherValue(int value) override;

  // CefApiVersionTestRefPtrLibraryChild methods.
  int GetOtherValue() override;
  void SetOtherValue(int value) override;

  // CefApiVersionTestRefPtrLibrary methods.
  int GetValueLegacy() override;
  void SetValueLegacy(int value) override;
#if CEF_API_ADDED(CEF_EXPERIMENTAL)
  int GetValueExp() override;
#endif
#if CEF_API_ADDED(CEF_EXPERIMENTAL)
  void SetValueExp(int value) override;
#endif
#if CEF_API_REMOVED(13301)
  int GetValue() override;
#endif
#if CEF_API_REMOVED(13301)
  void SetValue(int value) override;
#endif
#if CEF_API_RANGE(13301, 13302)
  int GetValueV1() override;
#endif
#if CEF_API_RANGE(13301, 13302)
  void SetValueV1(int value) override;
#endif
#if CEF_API_ADDED(13302)
  int GetValueV2() override;
#endif
#if CEF_API_ADDED(13302)
  void SetValueV2(int value) override;
#endif
};

constexpr auto CefApiVersionTestRefPtrLibraryChildChildCToCpp_Wrap =
    CefApiVersionTestRefPtrLibraryChildChildCToCpp::Wrap;
constexpr auto CefApiVersionTestRefPtrLibraryChildChildCToCpp_Unwrap =
    CefApiVersionTestRefPtrLibraryChildChildCToCpp::Unwrap;

#endif  // CEF_API_REMOVED(13301)

#endif  // CEF_LIBCEF_DLL_CTOCPP_TEST_API_VERSION_TEST_REF_PTR_LIBRARY_CHILD_CHILD_CTOCPP_H_
