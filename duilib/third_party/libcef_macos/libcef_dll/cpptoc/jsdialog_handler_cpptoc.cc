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
// $hash=293c76377319c0b43b0b37d5caecebf6fb22130d$
//

#include "libcef_dll/cpptoc/jsdialog_handler_cpptoc.h"

#include "libcef_dll/ctocpp/browser_ctocpp.h"
#include "libcef_dll/ctocpp/jsdialog_callback_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

int CEF_CALLBACK
jsdialog_handler_on_jsdialog(struct _cef_jsdialog_handler_t* self,
                             struct _cef_browser_t* browser,
                             const cef_string_t* origin_url,
                             cef_jsdialog_type_t dialog_type,
                             const cef_string_t* message_text,
                             const cef_string_t* default_prompt_text,
                             struct _cef_jsdialog_callback_t* callback,
                             int* suppress_message) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return 0;
  }
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser) {
    return 0;
  }
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback) {
    return 0;
  }
  // Verify param: suppress_message; type: bool_byref
  DCHECK(suppress_message);
  if (!suppress_message) {
    return 0;
  }
  // Unverified params: origin_url, message_text, default_prompt_text

  // Translate param: suppress_message; type: bool_byref
  bool suppress_messageBool =
      (suppress_message && *suppress_message) ? true : false;

  // Execute
  bool _retval = CefJSDialogHandlerCppToC::Get(self)->OnJSDialog(
      CefBrowserCToCpp_Wrap(browser), CefString(origin_url), dialog_type,
      CefString(message_text), CefString(default_prompt_text),
      CefJSDialogCallbackCToCpp_Wrap(callback), suppress_messageBool);

  // Restore param: suppress_message; type: bool_byref
  if (suppress_message) {
    *suppress_message = suppress_messageBool ? true : false;
  }

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK jsdialog_handler_on_before_unload_dialog(
    struct _cef_jsdialog_handler_t* self,
    struct _cef_browser_t* browser,
    const cef_string_t* message_text,
    int is_reload,
    struct _cef_jsdialog_callback_t* callback) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return 0;
  }
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser) {
    return 0;
  }
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback) {
    return 0;
  }
  // Unverified params: message_text

  // Execute
  bool _retval = CefJSDialogHandlerCppToC::Get(self)->OnBeforeUnloadDialog(
      CefBrowserCToCpp_Wrap(browser), CefString(message_text),
      is_reload ? true : false, CefJSDialogCallbackCToCpp_Wrap(callback));

  // Return type: bool
  return _retval;
}

void CEF_CALLBACK
jsdialog_handler_on_reset_dialog_state(struct _cef_jsdialog_handler_t* self,
                                       struct _cef_browser_t* browser) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return;
  }
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser) {
    return;
  }

  // Execute
  CefJSDialogHandlerCppToC::Get(self)->OnResetDialogState(
      CefBrowserCToCpp_Wrap(browser));
}

void CEF_CALLBACK
jsdialog_handler_on_dialog_closed(struct _cef_jsdialog_handler_t* self,
                                  struct _cef_browser_t* browser) {
  shutdown_checker::AssertNotShutdown();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self) {
    return;
  }
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser) {
    return;
  }

  // Execute
  CefJSDialogHandlerCppToC::Get(self)->OnDialogClosed(
      CefBrowserCToCpp_Wrap(browser));
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefJSDialogHandlerCppToC::CefJSDialogHandlerCppToC() {
  GetStruct()->on_jsdialog = jsdialog_handler_on_jsdialog;
  GetStruct()->on_before_unload_dialog =
      jsdialog_handler_on_before_unload_dialog;
  GetStruct()->on_reset_dialog_state = jsdialog_handler_on_reset_dialog_state;
  GetStruct()->on_dialog_closed = jsdialog_handler_on_dialog_closed;
}

// DESTRUCTOR - Do not edit by hand.

CefJSDialogHandlerCppToC::~CefJSDialogHandlerCppToC() {
  shutdown_checker::AssertNotShutdown();
}

template <>
CefRefPtr<CefJSDialogHandler> CefCppToCRefCounted<
    CefJSDialogHandlerCppToC,
    CefJSDialogHandler,
    cef_jsdialog_handler_t>::UnwrapDerived(CefWrapperType type,
                                           cef_jsdialog_handler_t* s) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCppToCRefCounted<CefJSDialogHandlerCppToC,
                                   CefJSDialogHandler,
                                   cef_jsdialog_handler_t>::kWrapperType =
    WT_JSDIALOG_HANDLER;
