#ifndef CAP_TASK_H
#define CAP_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

    void cap_task_get_free(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif