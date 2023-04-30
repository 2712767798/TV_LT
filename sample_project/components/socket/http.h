#ifndef HTTP_H
#define HTTP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "espnow_example.h"
#include "protocol_examples_common.h"
#include <esp_https_server.h>
#include "esp_tls_crypto.h"
#include <sys/param.h>

IRAM_ATTR void IF_http_init(void);

#ifdef __cplusplus
}
#endif

#endif
