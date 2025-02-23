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
// $hash=fb0a7892e3f9dc45854e94016424c3fc41bb4fec$
//

#include "libcef_dll/ctocpp/media_route_ctocpp.h"

#include "libcef_dll/ctocpp/media_sink_ctocpp.h"
#include "libcef_dll/ctocpp/media_source_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") CefString CefMediaRouteCToCpp::GetId() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_id) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_id(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefMediaSource> CefMediaRouteCToCpp::GetSource() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_source) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_source(_struct);

  // Return type: refptr_same
  return CefMediaSourceCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefMediaSink> CefMediaRouteCToCpp::GetSink() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_sink) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_sink(_struct);

  // Return type: refptr_same
  return CefMediaSinkCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall")
void CefMediaRouteCToCpp::SendRouteMessage(const void* message,
                                           size_t message_size) {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->send_route_message) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: message; type: simple_byaddr
  DCHECK(message);
  if (!message) {
    return;
  }

  // Execute
  _struct->send_route_message(_struct, message, message_size);
}

NO_SANITIZE("cfi-icall") void CefMediaRouteCToCpp::Terminate() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->terminate) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->terminate(_struct);
}

// CONSTRUCTOR - Do not edit by hand.

CefMediaRouteCToCpp::CefMediaRouteCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefMediaRouteCToCpp::~CefMediaRouteCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_media_route_t*
CefCToCppRefCounted<CefMediaRouteCToCpp, CefMediaRoute, cef_media_route_t>::
    UnwrapDerived(CefWrapperType type, CefMediaRoute* c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefMediaRouteCToCpp,
                                   CefMediaRoute,
                                   cef_media_route_t>::kWrapperType =
    WT_MEDIA_ROUTE;
