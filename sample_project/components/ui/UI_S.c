#include "UI_S.h"
#include <stdio.h>
#include "espnow_example.h"
#include "esp_log.h"

LV_FONT_DECLARE(myFont);
lv_obj_t *ui_Screen1;
lv_obj_t *ui_Button1;
lv_obj_t *ui_Button2;
lv_obj_t *Seting_Button;
// lv_obj_t *ui_label;
lv_obj_t *ui_Label1;
lv_obj_t *ui_Label2;
lv_obj_t *Seting_Label;

lv_obj_t *gif_bq;
lv_obj_t *ui_Arc1;

uint16_t pre_arc = 0;       // 判断灯泡按下有10的跳跃，不然一直闪
uint8_t data[2] = {0, 0};   // 发送的数据
uint16_t select_device = 0; // 判断是哪个设备
bool is_learn;              // 红外时候判断是否为学习模式

void ui_sd(void)
{

    lv_obj_clean(ui_Screen1);
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_t *img;
    img = lv_gif_create(ui_Screen1);
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_gif_set_src(img, "/sdcard/bulb.gif");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(ui_Screen1, ui_zayan, LV_EVENT_CLICKED, NULL);
    lv_disp_load_scr(ui_Screen1);
}

void ui_menu(void)
{
    data[0] = 0xaf;
    data[1] = 0x01;
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    ui_Screen1 = lv_obj_create(NULL);
    // ui_Button1

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_t *img;
    img = lv_img_create(ui_Button1 /* lv_scr_act() */);
    lv_img_set_src(img, "/sdcard/ui/BT.bin");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 100);

    lv_obj_set_x(ui_Button1, 0);
    lv_obj_set_y(ui_Button1, 0);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 渐变
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x0EACF7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0xFFB802), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 背色
    //  lv_obj_set_style_bg_img_src(ui_Button1, &img, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button1, lv_color_hex(0xDB1212), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button1, ui_zayan, LV_EVENT_CLICKED, NULL);
    // ui_Button2
    ui_Button2 = lv_btn_create(ui_Screen1);

    lv_obj_t *img2;
    img2 = lv_img_create(ui_Button2 /* lv_scr_act() */);
    lv_img_set_src(img2, "/sdcard/ui/espnow.bin");
    lv_obj_align(img2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_width(ui_Button2, 100);
    lv_obj_set_height(ui_Button2, 100);

    lv_obj_set_x(ui_Button2, 150);
    lv_obj_set_y(ui_Button2, 0);

    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 渐变
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0x0EACF7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button2, lv_color_hex(0xFFB802), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button2, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 背色
    //  lv_obj_set_style_bg_img_src(ui_Button1, &img, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button2, lv_color_hex(0xDB1212), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button2, input_in, LV_EVENT_CLICKED, NULL);
    {
        // Seting_Button

        Seting_Button = lv_btn_create(ui_Screen1);

        lv_obj_set_width(Seting_Button, 100);
        lv_obj_set_height(Seting_Button, 100);

        lv_obj_set_x(Seting_Button, 300);
        lv_obj_set_y(Seting_Button, 0);

        lv_obj_set_align(Seting_Button, LV_ALIGN_CENTER);

        lv_obj_add_flag(Seting_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_clear_flag(Seting_Button, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_radius(Seting_Button, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(Seting_Button, lv_color_hex(0x12ADC2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(Seting_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(Seting_Button, lv_color_hex(0x0BE957), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_main_stop(Seting_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_stop(Seting_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(Seting_Button, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(Seting_Button, seting_handle, LV_EVENT_CLICKED, NULL);
    }
    // ////////////////////////////////setingLabel
    Seting_Label = lv_label_create(Seting_Button);

    lv_obj_set_width(Seting_Label, LV_SIZE_CONTENT);
    lv_obj_set_height(Seting_Label, LV_SIZE_CONTENT);

    lv_obj_set_x(Seting_Label, 0);
    lv_obj_set_y(Seting_Label, 0);

    lv_obj_set_align(Seting_Label, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(Seting_Label, &myFont, 0);
    lv_label_set_text(Seting_Label, "设置终端信息");

    lv_obj_set_style_text_color(Seting_Label, lv_color_hex(0xF61156), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(Seting_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(Seting_Label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_decor(Seting_Label, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_disp_load_scr(ui_Screen1);
}

void shebei(void)
{

    lv_obj_t *return_Button;
    lv_obj_t *return_Label;

    // ui_Screen1
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_set_style_border_side(ui_Screen1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        // ui_Button1

        ui_Button1 = lv_btn_create(ui_Screen1);

        lv_obj_set_width(ui_Button1, 100);
        lv_obj_set_height(ui_Button1, 100);

        lv_obj_set_x(ui_Button1, 0);
        lv_obj_set_y(ui_Button1, 0);

        lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

        lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_radius(ui_Button1, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x12ADC2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0x0BE957), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_main_stop(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_stop(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(ui_Button1, device, LV_EVENT_CLICKED, NULL);
    }
    {
        // ui_Button2

        ui_Button2 = lv_btn_create(ui_Screen1);

        lv_obj_set_width(ui_Button2, 100);
        lv_obj_set_height(ui_Button2, 100);

        lv_obj_set_x(ui_Button2, 150);
        lv_obj_set_y(ui_Button2, 0);

        lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

        lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_radius(ui_Button2, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0x12ADC2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Button2, lv_color_hex(0x0BE957), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_main_stop(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_stop(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(ui_Button2, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(ui_Button2, learn_or_send, LV_EVENT_CLICKED, NULL);
    }
    {
        // return_button
        return_Button = lv_btn_create(ui_Screen1);

        lv_obj_set_width(return_Button, 100);
        lv_obj_set_height(return_Button, 100);

        lv_obj_set_x(return_Button, 300);
        lv_obj_set_y(return_Button, 0);

        lv_obj_set_align(return_Button, LV_ALIGN_CENTER);

        lv_obj_add_flag(return_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_clear_flag(return_Button, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_radius(return_Button, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(return_Button, lv_color_hex(0x12ADC2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(return_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(return_Button, lv_color_hex(0x0BE957), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_main_stop(return_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_stop(return_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(return_Button, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(return_Button, end_out, LV_EVENT_CLICKED, NULL);
    }

    {
        // ui_Label1

        ui_Label1 = lv_label_create(ui_Screen1);

        lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
        lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

        lv_obj_set_x(ui_Label1, 0);
        lv_obj_set_y(ui_Label1, 75);

        lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

        lv_obj_set_style_text_font(ui_Label1, &myFont, 0);
        lv_label_set_text(ui_Label1, "设备1");

        lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xF61156), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui_Label1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_decor(ui_Label1, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

        // ui_Label2

        ui_Label2 = lv_label_create(ui_Screen1);

        lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
        lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);

        lv_obj_set_x(ui_Label2, 150);
        lv_obj_set_y(ui_Label2, 75);

        lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);

        lv_obj_set_style_text_font(ui_Label2, &myFont, 0);
        lv_label_set_text(ui_Label2, "设备2");

        lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xF61156), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui_Label2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_decor(ui_Label2, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

        // return_label
        return_Label = lv_label_create(return_Button);

        lv_obj_set_width(return_Label, LV_SIZE_CONTENT);
        lv_obj_set_height(return_Label, LV_SIZE_CONTENT);

        lv_obj_set_x(return_Label, 0);
        lv_obj_set_y(return_Label, 0);

        lv_obj_set_align(return_Label, LV_ALIGN_CENTER);

        lv_obj_set_style_text_font(return_Label, &myFont, 0);
        lv_label_set_text(return_Label, "返回到表情页");

        lv_obj_set_style_text_color(return_Label, lv_color_hex(0xF61156), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(return_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(return_Label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_decor(return_Label, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    {
        // image
        lv_obj_t *img;
        img = lv_img_create(ui_Button1 /* lv_scr_act() */);
        lv_img_set_src(img, "/sdcard/ui/shebei.bin");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t *img2;
        img = lv_img_create(ui_Button2 /* lv_scr_act() */);
        lv_img_set_src(img, "/sdcard/ui/shebei.bin");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }

    lv_disp_load_scr(ui_Screen1);
}
/////////设备1//////////
void device(void)
{
    lv_obj_t *ui_Switch1;

    // ui_Screen1
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_set_style_border_side(ui_Screen1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Switch1

    ui_Switch1 = lv_switch_create(ui_Screen1);

    lv_obj_set_width(ui_Switch1, 65);
    lv_obj_set_height(ui_Switch1, 40);

    lv_obj_set_x(ui_Switch1, 100);
    lv_obj_set_y(ui_Switch1, -70);

    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_Switch1, ui_event_Switch1, LV_EVENT_ALL, NULL);

    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0x032769), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0x06A9D1), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0xF50404), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Switch1, lv_color_hex(0xF5DD04), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Switch1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Switch1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Switch1, LV_GRAD_DIR_HOR, LV_PART_KNOB | LV_STATE_DEFAULT);

    // ui_Arc1

    ui_Arc1 = lv_arc_create(ui_Screen1);

    lv_obj_set_width(ui_Arc1, 150);
    lv_obj_set_height(ui_Arc1, 150);

    lv_obj_set_x(ui_Arc1, 0);
    lv_obj_set_y(ui_Arc1, 0);

    lv_obj_set_align(ui_Arc1, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc1, 0, 100);
    lv_arc_set_value(ui_Arc1, 0);
    lv_arc_set_bg_angles(ui_Arc1, 120, 60);

    lv_obj_set_style_arc_color(ui_Arc1, lv_color_hex(0x00D3EA), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_Arc1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_Arc1, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Arc1, lv_color_hex(0x050F80), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Arc1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Arc1, 100, 0);
    lv_obj_add_state(ui_Arc1, LV_STATE_DISABLED);

    lv_obj_add_event_cb(ui_Arc1, ui_event_arc1, LV_EVENT_VALUE_CHANGED, NULL);

    // ui_Label1
    ui_Label1 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(ui_Label1, &myFont, 0);
    lv_label_set_text(ui_Label1, "设置亮度");

    // ui_Image2

    lv_obj_t *img;
    img = lv_img_create(ui_Screen1);
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img, "/sdcard/ui/deng.bin");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_width(img, LV_SIZE_CONTENT);
    lv_obj_set_height(img, LV_SIZE_CONTENT);

    lv_obj_set_x(img, 97);
    lv_obj_set_y(img, -56);

    // lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);
    // lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Button1

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 60);
    lv_obj_set_height(ui_Button1, 50);

    lv_obj_set_x(ui_Button1, -100);
    lv_obj_set_y(ui_Button1, -70);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button1, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xEC04F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0xCFF504), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button1, shebei, LV_EVENT_CLICKED, NULL);

    // ui_Label3

    ui_Label2 = lv_label_create(ui_Button1);

    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label2, 0);
    lv_obj_set_y(ui_Label2, 0);

    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(ui_Label2, &myFont, 0);

    lv_label_set_text(ui_Label2, "返回");

    lv_disp_load_scr(ui_Screen1);
}

static void ui_event_Switch1(lv_event_t *e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(ta, LV_STATE_CHECKED))
    {
        lv_obj_set_style_opa(ui_Arc1, 100, 0);
        lv_obj_add_state(ui_Arc1, LV_STATE_DISABLED);
        data[0] = 0x01;
        data[1] = 0x00;
        u_send(data);
    }
    if (event == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(ta, LV_STATE_CHECKED))
    {
        lv_obj_set_style_opa(ui_Arc1, 255, 0);
        lv_obj_clear_state(ui_Arc1, LV_STATE_DISABLED);
        data[0] = 0x01;
        data[1] = (uint8_t)(lv_arc_get_value(ui_Arc1));
        u_send(data);
    }
}
static void ui_event_arc1(void)
{
    if ((lv_arc_get_value(ui_Arc1) - pre_arc >= 7) || (lv_arc_get_value(ui_Arc1) - pre_arc <= -7) || lv_arc_get_value(ui_Arc1) == 0 || lv_arc_get_value(ui_Arc1) == 100)
    {
        data[0] = 0x01;
        data[1] = (uint8_t)(lv_arc_get_value(ui_Arc1));
        u_send(data);
        // ESP_LOGI("ARC","PRE_ARC %d,value %d",pre_arc,lv_arc_get_value(ui_Arc1));
        pre_arc = lv_arc_get_value(ui_Arc1);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////红外部分/////////////////////////////////////////////////////
void send_hongwai(lv_event_t *e) // 红外的控制器
{
    lv_obj_t *ta = lv_event_get_target(e);
    // ESP_LOGI("espnow", "DOWNLIST IS %d", lv_dropdown_get_selected(ta));
    lv_obj_t *ui_Button5, *ui_Button6;
    lv_obj_t *ui_Button3;
    lv_obj_t *ui_Button4;
    lv_obj_t *ui_Label3;
    lv_obj_t *ui_Label4;
    data[0] = ((uint8_t)lv_dropdown_get_selected(ta) << 4) + 0x02; // 三个设备分别为012
    //  ESP_LOGI("espnow", "DOWNLIST IS %x", data[0]);

    // ui_Screen1
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Button1返回

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 80);
    lv_obj_set_height(ui_Button1, 50);

    lv_obj_set_x(ui_Button1, -90);
    lv_obj_set_y(ui_Button1, -75);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0x6B0101), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button1, send_hongwai_1, LV_EVENT_CLICKED, NULL);

    // ui_Label1

    ui_Label1 = lv_label_create(ui_Button1);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label1, &myFont, 0);
    lv_label_set_text(ui_Label1, "返回");

    // ui_Button2温度开关

    ui_Button2 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button2, 80);
    lv_obj_set_height(ui_Button2, 80);

    lv_obj_set_x(ui_Button2, 0);
    lv_obj_set_y(ui_Button2, 0);

    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xE24B05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button2, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label2

    ui_Label2 = lv_label_create(ui_Button2);

    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label2, 0);
    lv_obj_set_y(ui_Label2, 0);

    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label2, &myFont, 0);
    lv_label_set_text(ui_Label2, "温度开关");

    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0x2805E2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Button3上

    ui_Button3 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button3, 80);
    lv_obj_set_height(ui_Button3, 50);

    lv_obj_set_x(ui_Button3, 0);
    lv_obj_set_y(ui_Button3, -75);

    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button3, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(0xE20E05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button3, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label3

    ui_Label3 = lv_label_create(ui_Button3);

    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label3, 0);
    lv_obj_set_y(ui_Label3, 0);

    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label3, &myFont, 0);
    lv_label_set_text(ui_Label3, "升温");

    // ui_Button4下

    ui_Button4 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button4, 80);
    lv_obj_set_height(ui_Button4, 50);

    lv_obj_set_x(ui_Button4, 0);
    lv_obj_set_y(ui_Button4, 75);

    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button4, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0xE20E05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button4, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label4

    ui_Label4 = lv_label_create(ui_Button4);

    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label4, 0);
    lv_obj_set_y(ui_Label4, 0);

    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label4, &myFont, 0);
    lv_label_set_text(ui_Label4, "降温");

    // 事件
    lv_obj_add_event_cb(ui_Button1, send_hongwai, LV_EVENT_CLICKED, NULL);

    // ui_Button5左

    ui_Button5 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button5, 60);
    lv_obj_set_height(ui_Button5, 80);

    lv_obj_set_x(ui_Button5, -80);
    lv_obj_set_y(ui_Button5, 0);

    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button5, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xE20E05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button5, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Button6右
    ui_Button6 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button6, 60);
    lv_obj_set_height(ui_Button6, 80);

    lv_obj_set_x(ui_Button6, 80);
    lv_obj_set_y(ui_Button6, 0);

    lv_obj_set_align(ui_Button6, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button6, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button6, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button6, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(0xE20E05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button6, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (is_learn == true)
    {
        lv_obj_add_event_cb(ui_Button2, ui_event_hongwai, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button3, ui_event_hongwai3, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button4, ui_event_hongwai4, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button5, ui_event_hongwai5, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button6, ui_event_hongwai6, LV_EVENT_CLICKED, NULL);
    }
    else
    {
        lv_obj_add_event_cb(ui_Button2, ui_send_hongwai, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button3, ui_send_hongwai3, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button4, ui_send_hongwai4, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button5, ui_send_hongwai5, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_Button6, ui_send_hongwai6, LV_EVENT_CLICKED, NULL);
    }

    lv_disp_load_scr(ui_Screen1);
}

void send_hongwai_1(lv_event_t *e) // 选择哪一个的设备
{
    uint8_t drom_event_change = 0;
    lv_obj_t *ta = lv_event_get_target(e);
    if (ta == ui_Button1)
    {
        is_learn = true;
    }
    else if (ta == ui_Button2)
    {
        is_learn = false;
    }
    else
    {
        drom_event_change = 1;
    }
    lv_obj_t *ui_Dropdown1;
    // ui_Screen1
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);

    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Dropdown1

    ui_Dropdown1 = lv_dropdown_create(ui_Screen1);
    lv_obj_set_style_text_font(ui_Dropdown1, &myFont, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_dropdown_set_options(ui_Dropdown1, "设备 1\n设备 2\n设备 3");
    // lv_dropdown_set_text(ui_Dropdown1, "");

    lv_obj_set_width(ui_Dropdown1, 200);
    lv_obj_set_height(ui_Dropdown1, 50);

    lv_obj_set_x(ui_Dropdown1, 0);
    lv_obj_set_y(ui_Dropdown1, 0);

    lv_obj_set_align(ui_Dropdown1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Dropdown1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    if (drom_event_change == 1)
    {
        lv_obj_add_event_cb(ui_Dropdown1, ui_send_clear, LV_EVENT_VALUE_CHANGED, NULL);
    }
    else
    {
        lv_obj_add_event_cb(ui_Dropdown1, send_hongwai, LV_EVENT_VALUE_CHANGED, NULL);
    }
    // ui_Button1返回

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 80);
    lv_obj_set_height(ui_Button1, 50);

    lv_obj_set_x(ui_Button1, -80);
    lv_obj_set_y(ui_Button1, -70);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0x6B0101), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button1, learn_or_send, LV_EVENT_CLICKED, NULL);

    // ui_Label1

    ui_Label1 = lv_label_create(ui_Button1);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label1, &myFont, 0);
    lv_label_set_text(ui_Label1, "返回");

    lv_disp_load_scr(ui_Screen1);
}

void learn_or_send(void)
{
    lv_obj_t *ui_Button3;
    lv_obj_t *ui_Button4;
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    // ui_Button1判断是学习还是发送，如果是button1就是学习

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 100);

    lv_obj_set_x(ui_Button1, 0);
    lv_obj_set_y(ui_Button1, 0);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x00FEDF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button1, lv_color_hex(0x033A34), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button1, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button1, send_hongwai_1, LV_EVENT_CLICKED, NULL);

    // ui_Image1
    lv_obj_t *img;
    img = lv_img_create(ui_Button1);
    lv_img_set_src(img, "/sdcard/ui/xuexi.bin");

    lv_obj_set_width(img, LV_SIZE_CONTENT);
    lv_obj_set_height(img, LV_SIZE_CONTENT);

    lv_obj_set_x(img, 0);
    lv_obj_set_y(img, 0);

    lv_obj_set_align(img, LV_ALIGN_CENTER);

    lv_obj_add_flag(img, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Button2此为发送
    ui_Button2 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button2, 100);
    lv_obj_set_height(ui_Button2, 100);

    lv_obj_set_x(ui_Button2, 150);
    lv_obj_set_y(ui_Button2, 0);

    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0x00FEDF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button2, lv_color_hex(0x033A34), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button2, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button2, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button2, send_hongwai_1, LV_EVENT_CLICKED, NULL);

    // ui_Image2
    lv_obj_t *img2;
    img2 = lv_img_create(ui_Button2);
    lv_img_set_src(img2, "/sdcard/ui/jinru.bin");

    lv_obj_set_width(img2, LV_SIZE_CONTENT);
    lv_obj_set_height(img2, LV_SIZE_CONTENT);

    lv_obj_set_x(img2, 0);
    lv_obj_set_y(img2, 0);

    lv_obj_set_align(img2, LV_ALIGN_CENTER);

    lv_obj_add_flag(img2, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(img2, LV_OBJ_FLAG_SCROLLABLE);
    // ui_buttun3此为清除所有

    ui_Button3 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button3, 100);
    lv_obj_set_height(ui_Button3, 100);

    lv_obj_set_x(ui_Button3, 300);
    lv_obj_set_y(ui_Button3, 0);

    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button3, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(0x00FEDF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button3, lv_color_hex(0x033A34), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button3, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button3, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button3, send_hongwai_1, LV_EVENT_CLICKED, NULL);
    // ui_Label1
    ui_Label1 = lv_label_create(ui_Button3);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label1, &myFont, 0);
    lv_label_set_text(ui_Label1, "清除所有缓存");

    // ui_Button4
    ui_Button4 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button4, 100);
    lv_obj_set_height(ui_Button4, 100);

    lv_obj_set_x(ui_Button4, 450);
    lv_obj_set_y(ui_Button4, 0);

    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Button4, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0x00FEDF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button4, lv_color_hex(0x033A34), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button4, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button4, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button4, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Button4, shebei, LV_EVENT_CLICKED, NULL);
    // ui_Label2
    ui_Label2 = lv_label_create(ui_Button4);

    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label2, 0);
    lv_obj_set_y(ui_Label2, 0);

    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_Label2, &myFont, 0);
    lv_label_set_text(ui_Label2, "返回");

    lv_disp_load_scr(ui_Screen1);
}
//*学习时候的发送
static void ui_event_hongwai(lv_event_t *e)
{

    lv_obj_t *ta = lv_event_get_target(e);
    if (lv_obj_has_state(ta, LV_STATE_CHECKED))
    {
        lv_obj_clear_state(ta, LV_STATE_CHECKED);
        lv_label_set_text(ui_Label2, "关");
        // data[0] = 0x02;
        data[1] = 0x40; // 4号是关
        u_send(data);
    }
    else
    {
        lv_obj_add_state(ta, LV_STATE_CHECKED);
        lv_label_set_text(ui_Label2, "开");
        // data[0] = 0x02;
        data[1] = 0x10; // 1是开关
        u_send(data);
    }
}
static void ui_event_hongwai3(void)
{
    // data[0] = 0x02;
    data[1] = 0x20; // 2是升温
    u_send(data);
}
static void ui_event_hongwai4(void)
{
    // data[0] = 0x02;
    data[1] = 0x30; // 3是降温
    u_send(data);
}
static void ui_event_hongwai5(void)
{
    // data[0] = 0x02;
    data[1] = 0x50; // 4是关5是左
    u_send(data);
}
static void ui_event_hongwai6(void)
{
    // data[0] = 0x02;
    data[1] = 0x60; // 6是右
    u_send(data);
}
//*发送指令的发送
static void ui_send_hongwai(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    if (lv_obj_has_state(ta, LV_STATE_CHECKED))
    {
        lv_obj_clear_state(ta, LV_STATE_CHECKED);
        lv_label_set_text(ui_Label2, "关");
        // data[0] = 0x02;
        data[1] = 0x41; // 4号是关
        u_send(data);
    }
    else
    {
        lv_obj_add_state(ta, LV_STATE_CHECKED);
        lv_label_set_text(ui_Label2, "开");
        // data[0] = 0x02;
        data[1] = 0x11; // 1是开关
        u_send(data);
    }
}
static void ui_send_hongwai3(void)
{
    // data[0] = 0x02;
    data[1] = 0x21; // 2是升温
    u_send(data);
}
static void ui_send_hongwai4(void)
{
    // data[0] = 0x02;
    data[1] = 0x31; // 3是降温
    u_send(data);
}
static void ui_send_hongwai5(void)
{
    // data[0] = 0x02;
    data[1] = 0x51; // 5是左
    u_send(data);
}
static void ui_send_hongwai6(void)
{
    // data[0] = 0x02;
    data[1] = 0x61; // 6是右
    u_send(data);
}

static void ui_send_clear(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    data[0] = ((uint8_t)lv_dropdown_get_selected(ta) << 4) + 0x02; // 三个设备分别为012
    // data[0] = 0x02;
    data[1] = 0x00; // 00代表清除缓存
    u_send(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////http设置//////////////////////////////////////////////////
void seting_handle(void)
{
    //wifi_end();
    //xTaskCreatePinnedToCore(IF_http_init, "http task", 1024 * 6 ,NULL, 14, NULL, 0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////发送信号
void u_send(uint8_t *dat)
{
    // ESP_LOGI("espnow", "send data:%x%d", dat[0], dat[1]);

    esp_send(dat);
}
void end_out(void) // 退出的时候引用，关闭所有espnow与WiFi
{
    esp_now_all_deinit();
    ui_menu();
}
void input_in(void) // 进入的时候引用，开启一次espnow与WiFi
{
    espnow_init();
    shebei();
}

////////眨眼////////////////////////////////////
void ui_zayan(void)
{
    lv_obj_clean(ui_Screen1);
    lv_img_cache_invalidate_src(NULL);
    ui_Screen1 = lv_obj_create(NULL);

    gif_bq = lv_gif_create(ui_Screen1 /* lv_scr_act() */);
    lv_obj_add_event_cb(gif_bq, zayan_cb, LV_EVENT_READY, NULL);
    // ESP_LOGI("GIF", "set COUNT IS %d", ((lv_gif_t *)img)->gif->loop_count);
    lv_gif_set_src(gif_bq, "/sdcard/emoj/zayan.gif");
    ((lv_gif_t *)gif_bq)->gif->loop_count = 1;
    lv_obj_align(gif_bq, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(ui_Screen1, ui_menu, LV_EVENT_CLICKED, NULL);

    lv_disp_load_scr(ui_Screen1);
}

void zayan_cb(void)
{
    uint8_t i = (uint8_t)(rand() % 4); // 产生三个随机数
    switch (i)
    {
    case 0:
        lv_gif_set_src(gif_bq, "/sdcard/emoj/zayan.gif");
        break;
    case 1:
        lv_gif_set_src(gif_bq, "/sdcard/emoj/dengyan.gif");
        break;
    case 2:
        lv_gif_set_src(gif_bq, "/sdcard/emoj/nu.gif");
        break;
    case 3:
        lv_gif_set_src(gif_bq, "/sdcard/emoj/miyan.gif");
        break;
    default:
        break;
    }
    // ESP_LOGI("CB", "YES %d", i);
    ((lv_gif_t *)gif_bq)->gif->loop_count = 1;
    vTaskDelay(1);
}

void ui_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    ui_zayan();

    // 、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、

    // // ui_laber
    // LV_FONT_DECLARE(myFont);

    // // static lv_style_t label_style;                                  // 创建一个风格
    // // lv_style_init(&label_style);                                    // 初始化风格
    // // lv_style_set_text_font(&ui_label,&myFont); // 设置风格的字体

    // ui_label = lv_label_create(ui_Button1);
    // lv_obj_set_width(ui_label, 50);
    // lv_obj_set_height(ui_label, 25);
    // lv_obj_set_align(ui_label, LV_ALIGN_CENTER);
    // // lv_obj_set_style_text_font()

    // lv_obj_set_style_text_font(ui_label,&myFont,0);
    // lv_label_set_text(ui_label, "测试123..");
    // // lv_obj_add_style(ui_label,&label_style,0); // 应用效果风格
}