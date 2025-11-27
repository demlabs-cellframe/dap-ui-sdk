/*
 * CoreBridgeSessionCompat.h
 * 
 * Compatibility layer for dap_stream_session API differences.
 * Provides unified interface for pointer-based and ID-based APIs.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

// Use our C API header instead of direct SDK includes
#include "CoreBridgeSessionCAPI.h"

// API Detection based on actual header analysis
// From dap_stream_session.h analysis:
// - dap_stream_session_new returns dap_stream_session_t*
// - dap_stream_session_open(dap_stream_session_t*) exists
// - dap_stream_session_close_mt(uint32_t id) exists (only MT version)
// - dap_stream_session_id_mt(uint32_t a_id) exists (get pointer from ID)
// - dap_stream_session_id_unsafe(uint32_t id) exists (get pointer from ID, unsafe)
// - dap_stream_session_t has uint32_t id field

#define CF_HAVE_NEW_RETURNS_PTR 1
#define CF_HAVE_OPEN_PTR 1
#define CF_HAVE_CLOSE_MT 1
#define CF_HAVE_CLOSE_PTR 0  // Only close_mt exists
#define CF_HAVE_GET_ID 1     // Access via session->id field
#define CF_HAVE_FROM_ID 1    // dap_stream_session_id_mt exists

// Unified types
using cf_session_t = dap_stream_session_t;

// Compatibility layer functions
static inline bool cf_have_close_ptr() {
    return CF_HAVE_CLOSE_PTR;
}

static inline bool cf_have_close_mt() {
    return CF_HAVE_CLOSE_MT;
}

// Create new session
static inline cf_session_t* cf_session_new(uint32_t ch, bool client, uint32_t* out_sid) {
    if (out_sid) *out_sid = 0;
    // Вызов строго с int, чтобы не было bool в ABI
    return dap_stream_session_new(ch, client ? 1 : 0);
}

// Open session
static inline int cf_session_open(cf_session_t* s) {
#if CF_HAVE_OPEN_PTR
    return dap_stream_session_open(s);
#else
    return -1; // Not available
#endif
}

// Get session ID from pointer
static inline uint32_t cf_session_get_id(cf_session_t* s) {
#if CF_HAVE_GET_ID
    return s ? dap_stream_session_get_id(s) : 0;
#else
    return 0;
#endif
}

// Get session pointer from ID
static inline cf_session_t* cf_session_from_id(uint32_t id) {
#if CF_HAVE_FROM_ID
    return dap_stream_session_from_id(id);
#else
    return nullptr;
#endif
}

// Close session (unified interface)
static inline int cf_session_close(cf_session_t*& s, uint32_t& sid) {
    if (sid) {
        int rc = dap_stream_session_close_mt(sid);
        sid = 0; s = nullptr;
        return rc;
    }
    // Если id не присваивали — просто зануляем указатель (ничего не делаем).
    s = nullptr; return 0;
}

// Convenience function for pointer-only close
static inline int cf_session_close_ptr(cf_session_t* s) {
    uint32_t dummy_id = 0;
    return cf_session_close(s, dummy_id);
}

// Convenience function for ID-only close
static inline int cf_session_close_id(uint32_t id) {
    cf_session_t* dummy_ptr = nullptr;
    return cf_session_close(dummy_ptr, id);
}