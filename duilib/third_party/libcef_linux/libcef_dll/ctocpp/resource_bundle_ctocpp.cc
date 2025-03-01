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
// $hash=90ecb7cf96775fa6ad61f043ab2a1d4b6c65b660$
//

#include "libcef_dll/ctocpp/resource_bundle_ctocpp.h"

#include "libcef_dll/ctocpp/binary_value_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefRefPtr<CefResourceBundle> CefResourceBundle::GetGlobal() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = cef_resource_bundle_get_global();

  // Return type: refptr_same
  return CefResourceBundleCToCpp_Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefString CefResourceBundleCToCpp::GetLocalizedString(int string_id) {
  auto* _struct = GetStruct();
  if (!_struct->get_localized_string) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval =
      _struct->get_localized_string(_struct, string_id);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefBinaryValue> CefResourceBundleCToCpp::GetDataResource(
    int resource_id) {
  auto* _struct = GetStruct();
  if (!_struct->get_data_resource) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval = _struct->get_data_resource(_struct, resource_id);

  // Return type: refptr_same
  return CefBinaryValueCToCpp_Wrap(_retval);
}

NO_SANITIZE("cfi-icall")
CefRefPtr<CefBinaryValue> CefResourceBundleCToCpp::GetDataResourceForScale(
    int resource_id,
    ScaleFactor scale_factor) {
  auto* _struct = GetStruct();
  if (!_struct->get_data_resource_for_scale) {
    return nullptr;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  auto* _retval =
      _struct->get_data_resource_for_scale(_struct, resource_id, scale_factor);

  // Return type: refptr_same
  return CefBinaryValueCToCpp_Wrap(_retval);
}

// CONSTRUCTOR - Do not edit by hand.

CefResourceBundleCToCpp::CefResourceBundleCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefResourceBundleCToCpp::~CefResourceBundleCToCpp() {}

template <>
cef_resource_bundle_t* CefCToCppRefCounted<
    CefResourceBundleCToCpp,
    CefResourceBundle,
    cef_resource_bundle_t>::UnwrapDerived(CefWrapperType type,
                                          CefResourceBundle* c) {
  CHECK(false) << __func__ << " called with unexpected class type " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefResourceBundleCToCpp,
                                   CefResourceBundle,
                                   cef_resource_bundle_t>::kWrapperType =
    WT_RESOURCE_BUNDLE;
