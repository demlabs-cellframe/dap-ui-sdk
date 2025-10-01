// CoreBridgeSessionCAPI.h
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration
typedef struct dap_stream_session dap_stream_session_t;

// Minimal struct definition for accessing fields
struct dap_stream_session {
    uint32_t id;
    // Add other fields as needed
};

// ВАЖНО: только C-прототипы, без bool!
dap_stream_session_t *dap_stream_session_new(uint32_t channel_id, int is_client);
int dap_stream_session_open(dap_stream_session_t *sess);
int dap_stream_session_close_mt(uint32_t id);

// Additional functions for ID management
uint32_t dap_stream_session_get_id(dap_stream_session_t *sess);
dap_stream_session_t *dap_stream_session_from_id(uint32_t id);

#ifdef __cplusplus
} // extern "C"
#endif