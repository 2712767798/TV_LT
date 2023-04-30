// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: TEST

#ifndef _TEST_UI_S_H
#define _TEST_UI_S_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "lv_task.h"
#include "http.h"
// #include "espnow_example.h"

    void ui_zayan(void);
    void ui_init(void);
    void shebei(void);   // 设备菜单
    void device(void);   // 设备1
    void send_hongwai(); // 红外控制页面
    void zayan_cb(void);
    void end_out(void);                 // 退出的时候关闭所有
    void input_in(void);                // 进入wifi时候加载
    void learn_or_send(void);           // 红外是否学习的界面
    void send_hongwai_1(lv_event_t *e); // 选择哪个遥控器的页面

    static void ui_event_arc1(void);
    void u_send(uint8_t *dat);
    static void ui_event_Switch1(lv_event_t *e);
    // 红外
    static void ui_event_hongwai(lv_event_t *e);
    static void ui_event_hongwai3(void);
    static void ui_event_hongwai4(void);
    static void ui_event_hongwai5(void);
    static void ui_event_hongwai6(void);

    static void ui_send_hongwai(lv_event_t *e);
    static void ui_send_hongwai3(void);
    static void ui_send_hongwai4(void);
    static void ui_send_hongwai5(void);
    static void ui_send_hongwai6(void);
    static void ui_send_clear(lv_event_t *e);// 清零

    static void seting_handle(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
