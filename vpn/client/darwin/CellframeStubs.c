/*
 * Cellframe SDK Stubs
 * 
 * Temporary stubs for Cellframe Core functions until SDK is built.
 * These stubs provide minimal functionality to allow compilation
 * without linking to actual Cellframe libraries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations for types that would come from headers
typedef struct dap_stream_session {
    uint32_t id;
    // Add other fields as needed
} dap_stream_session_t;

// Stub implementations
dap_stream_session_t* dap_stream_session_new(uint32_t media_id, int is_client) {
    printf("STUB: dap_stream_session_new(media_id=%u, is_client=%d)\n", 
           media_id, is_client);
    
    // Return a dummy pointer (not NULL to avoid crashes)
    static dap_stream_session_t dummy_session = {1}; // Set ID to 1
    return &dummy_session;
}

int dap_stream_session_open(dap_stream_session_t * a_session) {
    printf("STUB: dap_stream_session_open(session=%p)\n", (void*)a_session);
    return 0; // Success
}

int dap_stream_session_close_mt(uint32_t id) {
    printf("STUB: dap_stream_session_close_mt(id=%u)\n", id);
    return 0; // Success
}

int dap_stream_session_close(dap_stream_session_t * session) {
    printf("STUB: dap_stream_session_close(session=%p)\n", (void*)session);
    return 0; // Success
}

// Additional stubs that might be needed
void dap_stream_session_init(void) {
    printf("STUB: dap_stream_session_init()\n");
}

void dap_stream_session_deinit(void) {
    printf("STUB: dap_stream_session_deinit()\n");
}

dap_stream_session_t* dap_stream_session_id_mt(uint32_t a_id) {
    printf("STUB: dap_stream_session_id_mt(id=%u)\n", a_id);
    static dap_stream_session_t dummy_session = {0};
    return &dummy_session;
}

dap_stream_session_t* dap_stream_session_id_unsafe(uint32_t id) {
    printf("STUB: dap_stream_session_id_unsafe(id=%u)\n", id);
    static dap_stream_session_t dummy_session = {0};
    return &dummy_session;
}

// Additional functions for ID management
uint32_t dap_stream_session_get_id(dap_stream_session_t *sess) {
    printf("STUB: dap_stream_session_get_id(session=%p)\n", (void*)sess);
    return sess ? sess->id : 0;
}

dap_stream_session_t* dap_stream_session_from_id(uint32_t id) {
    printf("STUB: dap_stream_session_from_id(id=%u)\n", id);
    static dap_stream_session_t dummy_session = {0};
    dummy_session.id = id; // Set ID at runtime
    return &dummy_session;
}

void dap_stream_session_lock(void) {
    printf("STUB: dap_stream_session_lock()\n");
}

void dap_stream_session_unlock(void) {
    printf("STUB: dap_stream_session_unlock()\n");
}