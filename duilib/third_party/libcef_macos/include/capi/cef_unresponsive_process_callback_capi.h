// Copyright (c) 2025 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=8c45f30dd1dc404020c91509d0f93e03fa363007$
//

#ifndef CEF_INCLUDE_CAPI_CEF_UNRESPONSIVE_PROCESS_CALLBACK_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_UNRESPONSIVE_PROCESS_CALLBACK_CAPI_H_
#pragma once

#if defined(BUILDING_CEF_SHARED)
#error This file cannot be included DLL-side
#endif

#include "include/capi/cef_base_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

///
/// Callback structure for asynchronous handling of an unresponsive process.
///
/// NOTE: This struct is allocated DLL-side.
///
typedef struct _cef_unresponsive_process_callback_t {
  ///
  /// Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  /// Reset the timeout for the unresponsive process.
  ///
  void(CEF_CALLBACK* wait)(struct _cef_unresponsive_process_callback_t* self);

  ///
  /// Terminate the unresponsive process.
  ///
  void(CEF_CALLBACK* terminate)(
      struct _cef_unresponsive_process_callback_t* self);
} cef_unresponsive_process_callback_t;

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_UNRESPONSIVE_PROCESS_CALLBACK_CAPI_H_
