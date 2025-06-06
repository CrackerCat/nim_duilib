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
// $hash=bb73cc443cc33cb2d5001ef2496f4925c0119490$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_VIEWS_OVERLAY_CONTROLLER_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_VIEWS_OVERLAY_CONTROLLER_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/views/cef_overlay_controller_capi.h"
#include "include/capi/views/cef_view_capi.h"
#include "include/capi/views/cef_window_capi.h"
#include "include/views/cef_overlay_controller.h"
#include "include/views/cef_view.h"
#include "include/views/cef_window.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefOverlayControllerCToCpp
    : public CefCToCppRefCounted<CefOverlayControllerCToCpp,
                                 CefOverlayController,
                                 cef_overlay_controller_t> {
 public:
  CefOverlayControllerCToCpp();
  virtual ~CefOverlayControllerCToCpp();

  // CefOverlayController methods.
  bool IsValid() override;
  bool IsSame(CefRefPtr<CefOverlayController> that) override;
  CefRefPtr<CefView> GetContentsView() override;
  CefRefPtr<CefWindow> GetWindow() override;
  cef_docking_mode_t GetDockingMode() override;
  void Destroy() override;
  void SetBounds(const CefRect& bounds) override;
  CefRect GetBounds() override;
  CefRect GetBoundsInScreen() override;
  void SetSize(const CefSize& size) override;
  CefSize GetSize() override;
  void SetPosition(const CefPoint& position) override;
  CefPoint GetPosition() override;
  void SetInsets(const CefInsets& insets) override;
  CefInsets GetInsets() override;
  void SizeToPreferredSize() override;
  void SetVisible(bool visible) override;
  bool IsVisible() override;
  bool IsDrawn() override;
};

constexpr auto CefOverlayControllerCToCpp_Wrap =
    CefOverlayControllerCToCpp::Wrap;
constexpr auto CefOverlayControllerCToCpp_Unwrap =
    CefOverlayControllerCToCpp::Unwrap;

#endif  // CEF_LIBCEF_DLL_CTOCPP_VIEWS_OVERLAY_CONTROLLER_CTOCPP_H_
