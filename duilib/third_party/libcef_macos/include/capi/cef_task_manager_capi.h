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
// $hash=98fbe5c90a7eb86068787b850ecf0b119719a0d4$
//

#ifndef CEF_INCLUDE_CAPI_CEF_TASK_MANAGER_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_TASK_MANAGER_CAPI_H_
#pragma once

#if defined(BUILDING_CEF_SHARED)
#error This file cannot be included DLL-side
#endif

#include "include/capi/cef_base_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

///
/// Structure that facilitates managing the browser-related tasks. The functions
/// of this structure may only be called on the UI thread.
///
/// NOTE: This struct is allocated DLL-side.
///
typedef struct _cef_task_manager_t {
  ///
  /// Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  /// Returns the number of tasks currently tracked by the task manager. Returns
  /// 0 if the function was called from the incorrect thread.
  ///
  size_t(CEF_CALLBACK* get_tasks_count)(struct _cef_task_manager_t* self);

  ///
  /// Gets the list of task IDs currently tracked by the task manager. Tasks
  /// that share the same process id will always be consecutive. The list will
  /// be sorted in a way that reflects the process tree: the browser process
  /// will be first, followed by the gpu process if it exists. Related processes
  /// (e.g., a subframe process and its parent) will be kept together if
  /// possible. Callers can expect this ordering to be stable when a process is
  /// added or removed. The task IDs are unique within the application lifespan.
  /// Returns false (0) if the function was called from the incorrect thread.
  ///
  int(CEF_CALLBACK* get_task_ids_list)(struct _cef_task_manager_t* self,
                                       size_t* task_idsCount,
                                       int64_t* task_ids);

  ///
  /// Gets information about the task with |task_id|. Returns true (1) if the
  /// information about the task was successfully retrieved and false (0) if the
  /// |task_id| is invalid or the function was called from the incorrect thread.
  ///
  int(CEF_CALLBACK* get_task_info)(struct _cef_task_manager_t* self,
                                   int64_t task_id,
                                   struct _cef_task_info_t* info);

  ///
  /// Attempts to terminate a task with |task_id|. Returns false (0) if the
  /// |task_id| is invalid, the call is made from an incorrect thread, or if the
  /// task cannot be terminated.
  ///
  int(CEF_CALLBACK* kill_task)(struct _cef_task_manager_t* self,
                               int64_t task_id);

  ///
  /// Returns the task ID associated with the main task for |browser_id| (value
  /// from cef_browser_t::GetIdentifier). Returns -1 if |browser_id| is invalid,
  /// does not currently have an associated task, or the function was called
  /// from the incorrect thread.
  ///
  int64_t(CEF_CALLBACK* get_task_id_for_browser_id)(
      struct _cef_task_manager_t* self,
      int browser_id);
} cef_task_manager_t;

///
/// Returns the global task manager object. Returns nullptr if the function was
/// called from the incorrect thread.
///
CEF_EXPORT cef_task_manager_t* cef_task_manager_get(void);

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_TASK_MANAGER_CAPI_H_
