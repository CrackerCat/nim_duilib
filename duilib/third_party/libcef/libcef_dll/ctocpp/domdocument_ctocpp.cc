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
// $hash=043aa1e62efb52e29f7dc663ab6f15ceade4af92$
//

#include "libcef_dll/ctocpp/domdocument_ctocpp.h"

#include "libcef_dll/ctocpp/domnode_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") CefDOMDocument::Type CefDOMDocumentCToCpp::GetType() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_type) {
    return DOM_DOCUMENT_TYPE_UNKNOWN;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_dom_document_type_t _retval = _struct->get_type(_struct);

  // Return type: simple
  return _retval;
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefDOMNode> CefDOMDocumentCToCpp::GetDocument() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_document) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_document(_struct);

  // Return type: refptr_same
  return CefDOMNodeCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") CefRefPtr<CefDOMNode> CefDOMDocumentCToCpp::GetBody() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_body) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_body(_struct);

  // Return type: refptr_same
  return CefDOMNodeCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") CefRefPtr<CefDOMNode> CefDOMDocumentCToCpp::GetHead() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_head) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_head(_struct);

  // Return type: refptr_same
  return CefDOMNodeCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") CefString CefDOMDocumentCToCpp::GetTitle() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_title) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_title(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefDOMNode> CefDOMDocumentCToCpp::GetElementById(
    const CefString& id) {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_element_by_id) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: id; type: string_byref_const
  DCHECK(!id.empty());
  if (id.empty()) {
    return nullptr;
  }

  // Execute
  auto* _retval = _struct->get_element_by_id(_struct, id.GetStruct());

  // Return type: refptr_same
  return CefDOMNodeCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefDOMNode> CefDOMDocumentCToCpp::GetFocusedNode() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_focused_node) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_focused_node(_struct);

  // Return type: refptr_same
  return CefDOMNodeCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall") bool CefDOMDocumentCToCpp::HasSelection() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->has_selection) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->has_selection(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall") int CefDOMDocumentCToCpp::GetSelectionStartOffset() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_selection_start_offset) {
    return 0;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_selection_start_offset(_struct);

  // Return type: simple
  return _retval;
}

NO_SANITIZE("cfi-icall") int CefDOMDocumentCToCpp::GetSelectionEndOffset() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_selection_end_offset) {
    return 0;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_selection_end_offset(_struct);

  // Return type: simple
  return _retval;
}

NO_SANITIZE("cfi-icall")
CefString CefDOMDocumentCToCpp::GetSelectionAsMarkup() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_selection_as_markup) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_selection_as_markup(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall") CefString CefDOMDocumentCToCpp::GetSelectionAsText() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_selection_as_text) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_selection_as_text(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall") CefString CefDOMDocumentCToCpp::GetBaseURL() {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_base_url) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_base_url(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall")
CefString CefDOMDocumentCToCpp::GetCompleteURL(const CefString& partialURL) {
  shutdown_checker::AssertNotShutdown();

  auto* _struct = GetStruct();
  if (!_struct->get_complete_url) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: partialURL; type: string_byref_const
  DCHECK(!partialURL.empty());
  if (partialURL.empty()) {
    return CefString();
  }

  // Execute
  cef_string_userfree_t _retval =
      _struct->get_complete_url(_struct, partialURL.GetStruct());

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

// CONSTRUCTOR - Do not edit by hand.

CefDOMDocumentCToCpp::CefDOMDocumentCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefDOMDocumentCToCpp::~CefDOMDocumentCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_domdocument_t*
CefCToCppRefCounted<CefDOMDocumentCToCpp, CefDOMDocument, cef_domdocument_t>::
    UnwrapDerived(CefWrapperType type, CefDOMDocument* c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefDOMDocumentCToCpp,
                                   CefDOMDocument,
                                   cef_domdocument_t>::kWrapperType =
    WT_DOMDOCUMENT;
