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
// This file was generated by the make_api_versions_header.py tool. Versions
// are managed using the version_manager.py tool. For usage details see
// https://bitbucket.org/chromiumembedded/cef/wiki/ApiVersioning.md
//

#ifndef CEF_INCLUDE_CEF_API_VERSIONS_H_
#define CEF_INCLUDE_CEF_API_VERSIONS_H_

#include "include/base/cef_build.h"

// Added February 21, 2025.
#define CEF_API_VERSION_13300 13300
#if defined(OS_WIN)
#define CEF_API_HASH_13300 "45d39c3669ba75467e3e609f626c31506c0eae22"
#elif defined(OS_MAC)
#define CEF_API_HASH_13300 "80c0b59ba9dd783aa71fae0aa5f7dad64620e8c9"
#elif defined(OS_LINUX)
#define CEF_API_HASH_13300 "2508f3f0b0e5dfa191036fa6c04f8dcfa18c94b9"
#endif

// Added February 21, 2025.
#define CEF_API_VERSION_13301 13301
#if defined(OS_WIN)
#define CEF_API_HASH_13301 "7109702038d51512d35dd2ed77231f9100e38214"
#elif defined(OS_MAC)
#define CEF_API_HASH_13301 "fda40a5df44628cac50a589ff979c0746011591e"
#elif defined(OS_LINUX)
#define CEF_API_HASH_13301 "aa073dd1c586812503ca293c718358460d8c2dd6"
#endif

// Added February 21, 2025.
#define CEF_API_VERSION_13302 13302
#if defined(OS_WIN)
#define CEF_API_HASH_13302 "18799961f4461a9cbae2aed89ac04b73ab7c37f3"
#elif defined(OS_MAC)
#define CEF_API_HASH_13302 "4aa24470ba3a4bd9c06bc0e4a201b896394a86b5"
#elif defined(OS_LINUX)
#define CEF_API_HASH_13302 "d5597ebfa30081953425e897209a8387b9584205"
#endif

// Added February 21, 2025.
#define CEF_API_VERSION_13303 13303
#if defined(OS_WIN)
#define CEF_API_HASH_13303 "20016fd6a9b87ef4c539cd1f42bf1ca09b6903ca"
#elif defined(OS_MAC)
#define CEF_API_HASH_13303 "f2cdce2b9a4b635c28b5b92c42c35625a937380c"
#elif defined(OS_LINUX)
#define CEF_API_HASH_13303 "f3a696ee30ce1e00490a58df017393c126c89709"
#endif

// Added February 21, 2025.
#define CEF_API_VERSION_13304 13304
#if defined(OS_WIN)
#define CEF_API_HASH_13304 "19c014af0082aa901398e006381b6980e4f806e9"
#elif defined(OS_MAC)
#define CEF_API_HASH_13304 "98964c37b8917d83da4b173e22905503d38ad08f"
#elif defined(OS_LINUX)
#define CEF_API_HASH_13304 "f1ababb4ff51ecbf77c481cee3721ef0eca9c8ca"
#endif

// Next version last updated March 09, 2025.
#define CEF_API_VERSION_999998 999998
#if defined(OS_WIN)
#define CEF_API_HASH_999998 "19c014af0082aa901398e006381b6980e4f806e9"
#elif defined(OS_MAC)
#define CEF_API_HASH_999998 "98964c37b8917d83da4b173e22905503d38ad08f"
#elif defined(OS_LINUX)
#define CEF_API_HASH_999998 "f1ababb4ff51ecbf77c481cee3721ef0eca9c8ca"
#endif

// Experimental version last updated March 09, 2025.
#define CEF_API_VERSION_999999 999999
#if defined(OS_WIN)
#define CEF_API_HASH_999999 "a9dbeff8d358f02b58576becf2cede342bd2a4b5"
#elif defined(OS_MAC)
#define CEF_API_HASH_999999 "81b7392d0b6962e8ae63dcdcaee689809fddf1cf"
#elif defined(OS_LINUX)
#define CEF_API_HASH_999999 "8c15abf29bd50c124bc1d19a9c7948755634965e"
#endif

// Oldest supported CEF version.
#define CEF_API_VERSION_MIN CEF_API_VERSION_13300

// Newest supported CEF version.
#define CEF_API_VERSION_LAST CEF_API_VERSION_13304

#endif  // CEF_INCLUDE_CEF_API_VERSIONS_H_
