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
// $hash=d1a4aec1ec77e9b7c6acc2ddc9f8202e0bde2d59$
//

#include "libcef_dll/cpptoc/test/test_server_handler_cpptoc.h"

#include "libcef_dll/ctocpp/request_ctocpp.h"
#include "libcef_dll/ctocpp/test/test_server_connection_ctocpp.h"
#include "libcef_dll/ctocpp/test/test_server_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

int CEF_CALLBACK test_server_handler_on_test_server_request(
    struct _cef_test_server_handler_t* self,
    struct _cef_test_server_t* server,
    struct _cef_request_t* request,
    struct _cef_test_server_connection_t* connection) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return 0;
  }
  // Verify param: server; type: refptr_diff
  DCHECK(server);
  if (!server) {
    return 0;
  }
  // Verify param: request; type: refptr_diff
  DCHECK(request);
  if (!request) {
    return 0;
  }
  // Verify param: connection; type: refptr_diff
  DCHECK(connection);
  if (!connection) {
    return 0;
  }

  // Execute
  bool _retval = CefTestServerHandlerCppToC::Get(self)->OnTestServerRequest(
      CefTestServerCToCpp_Wrap(server), CefRequestCToCpp_Wrap(request),
      CefTestServerConnectionCToCpp_Wrap(connection));

  // Return type: bool
  return _retval;
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefTestServerHandlerCppToC::CefTestServerHandlerCppToC() {
  GetStruct()->on_test_server_request =
      test_server_handler_on_test_server_request;
}

// DESTRUCTOR - Do not edit by hand.

CefTestServerHandlerCppToC::~CefTestServerHandlerCppToC() {
  shutdown_checker::AssertNotShutdown();
}

template <>
CefRefPtr<CefTestServerHandler> CefCppToCRefCounted<
    CefTestServerHandlerCppToC,
    CefTestServerHandler,
    cef_test_server_handler_t>::UnwrapDerived(CefWrapperType type,
                                              cef_test_server_handler_t* s) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCppToCRefCounted<CefTestServerHandlerCppToC,
                                   CefTestServerHandler,
                                   cef_test_server_handler_t>::kWrapperType =
    WT_TEST_SERVER_HANDLER;
