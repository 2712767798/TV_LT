#include "lv_task.h"

//建立回调函数
static void lv_timer_back(void *arg)
{
    //每一毫秒调用一次lv的“心脏”
    lv_tick_inc(10);
}

//创建注册timer
void lv_ti_init(void)
{
    static esp_timer_handle_t lv_timer_ti = NULL; // //先建立一个例子句柄
    esp_timer_create_args_t lv_timer_arg = {
        //建立初始化句柄的实例
        .callback = lv_timer_back,
        .name = "lv_timer_arg",
    };
    esp_err_t err = esp_timer_create(&lv_timer_arg, &lv_timer_ti);
    err = esp_timer_start_periodic(lv_timer_ti, 10000); // 1000us调用一次回调函数
    if (err == ESP_OK)
    {
        ESP_LOGI("main", "lvgl定时器创建成功\r\n");
    }
}

// void task_png(lv_timer_t *timer)
// {
//     lv_obj_t *img;
//     img = lv_img_create(lv_scr_act());
//     while (1)
//     {
//         lv_task_handler();
//         static char path[40];
//         static uint8_t i = 0;
//         sprintf(path, "/sdcard/emoj/per%d.png", (i++) % 17 + 1);
//         lv_img_set_src(img, path);
//         /* vTaskDelay(30); */
//     }
// }

IRAM_ATTR void lvgl_task_loop(void)
{
 
    lv_init();
    lv_ti_init();
    lv_port_disp_init();
    lv_port_indev_init();
    // xTaskCreatePinnedToCore(task_png, "png", 2048 * 2, NULL, 1, NULL, 1);
    /* lv_timer_create(task_png, 30, NULL); */
    ui_init();
    lv_img_cache_set_size(2);
    // lv_obj_t *img;
    // // // LV_IMG_DECLARE(img_bulb_gif);
    // img = lv_gif_create(lv_scr_act());
    // lv_gif_set_src(img, "/sdcard/emoj/zayan.gif");
    // lv_obj_align(img, LV_ALIGN_LEFT_MID, 0, 0);

    // img = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    // lv_img_set_src(img, image);
    // lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    while (1)
    {
        lv_task_handler();
        // vTaskDelay(5);
    }
}
