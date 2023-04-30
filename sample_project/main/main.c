#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "mbedtls/md5.h"
#include "lv_task.h"
#include "sdmcc.h"
#include "sock.h"
#include "espnow_example.h"
#include "captask.h"
#include "http.h"

static const char *TAG = "example";

// static void read_hello_txt(void)
// {
//     ESP_LOGI(TAG, "Reading hello.txt");

//     // Open for reading hello.txt
//     FILE *f = fopen("/sdcard/hello.txt", "r");
//     if (f == NULL)
//     {
//         ESP_LOGE(TAG, "Failed to open hello.txt");
//         return;
//     }

//     char buf[64];
//     memset(buf, 0, sizeof(buf));
//     fread(buf, 1, sizeof(buf), f);
//     fclose(f);

//     // Display the read contents from the file
//     ESP_LOGI(TAG, "Read from hello.txt: %s", buf);
// }

// static void compute_alice_txt_md5(void)
// {
//     ESP_LOGI(TAG, "Computing alice.txt MD5 hash");

//     // 文件alice.txt位于子目录下，尽管SPIFFS本身是扁平的 The file alice.txt lives under a subdirectory, though SPIFFS itself is flat
//     FILE *f = fopen("/spiffs/sub/alice.txt", "r");
//     if (f == NULL)
//     {
//         ESP_LOGE(TAG, "Failed to open alice.txt");
//         return;
//     }

// // Read file and compute the digest chunk by chunk
// #define MD5_MAX_LEN 16

//     char buf[64];
//     mbedtls_md5_context ctx;
//     unsigned char digest[MD5_MAX_LEN];

//     mbedtls_md5_init(&ctx);
//     mbedtls_md5_starts_ret(&ctx);

//     size_t read;

//     do
//     {
//         read = fread((void *)buf, 1, sizeof(buf), f);
//         mbedtls_md5_update_ret(&ctx, (unsigned const char *)buf, read);
//     } while (read == sizeof(buf));

//     mbedtls_md5_finish_ret(&ctx, digest);

//     // Create a string of the digest
//     char digest_str[MD5_MAX_LEN * 2];

//     for (int i = 0; i < MD5_MAX_LEN; i++)
//     {
//         sprintf(&digest_str[i * 2], "%02x", (unsigned int)digest[i]);
//     }

//     // For reference, MD5 should be deeb71f585cbb3ae5f7976d5127faf2a
//     ESP_LOGI(TAG, "Computed MD5 hash of alice.txt: %s", digest_str);

//     fclose(f);
// }

void app_main(void)
{
    sdmcc_initt();

    // sock_init();

    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // 使用上面定义的设置来初始化和挂载SPIFFS文件系统。
    // 注:esp_vfs_spiffs_register是一个一体化的方便函数
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    xTaskCreatePinnedToCore(lvgl_task_loop, "loop task", 1024 * 20 + 512, NULL, 21, NULL, 1);
    //vTaskDelete(NULL);
    // xTaskCreatePinnedToCore(sdmcc_initt,"SDMCC",1024*8,NULL,1,NULL,1);

    while (1)
    {
        cap_task_get_free();
        vTaskDelay(5000);
    }

    // while (1)
    // {

    // }
    // // while (1)
    // // {
    // //     vTaskDelay(1);
    // // }
}
