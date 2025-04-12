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
// $hash=5180bd0e954014fe861789404ca7d3829e0930bc$
//

#include "libcef_dll/ctocpp/waitable_event_ctocpp.h"

#include "libcef_dll/shutdown_checker.h"

// STATIC METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefRefPtr<CefWaitableEvent> CefWaitableEvent::CreateWaitableEvent(
    bool automatic_reset,
    bool initially_signaled) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval =
      cef_waitable_event_create(automatic_reset, initially_signaled);

  // Return type: refptr_same
  return CefWaitableEventCToCpp_Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") void CefWaitableEventCToCpp::Reset() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->reset) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->reset(_struct);
}

NO_SANITIZE("cfi-icall") void CefWaitableEventCToCpp::Signal() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->signal) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->signal(_struct);
}

NO_SANITIZE("cfi-icall") bool CefWaitableEventCToCpp::IsSignaled() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->is_signaled) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_signaled(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall") void CefWaitableEventCToCpp::Wait() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->wait) {
    return;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->wait(_struct);
}

NO_SANITIZE("cfi-icall")
bool CefWaitableEventCToCpp::TimedWait(int64_t max_ms) {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->timed_wait) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->timed_wait(_struct, max_ms);

  // Return type: bool
  return _retval ? true : false;
}

// CONSTRUCTOR - Do not edit by hand.

CefWaitableEventCToCpp::CefWaitableEventCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefWaitableEventCToCpp::~CefWaitableEventCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_waitable_event_t*
CefCToCppRefCounted<CefWaitableEventCToCpp,
                    CefWaitableEvent,
                    cef_waitable_event_t>::UnwrapDerived(CefWrapperType type,
                                                         CefWaitableEvent* c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefWaitableEventCToCpp,
                                   CefWaitableEvent,
                                   cef_waitable_event_t>::kWrapperType =
    WT_WAITABLE_EVENT;
