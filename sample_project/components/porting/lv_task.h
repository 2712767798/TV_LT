#ifndef LV_TASK_H
#define  LV_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_timer.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include "lvgl_helpers.h"
// #include "demos/lv_demos.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "UI_S.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void lvgl_task_loop(void);

#ifdef __cplusplus
 }
#endif

#endif
