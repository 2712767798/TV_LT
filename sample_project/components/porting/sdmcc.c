#include "sdmcc.h"

// sdmmc_card_init

static const char *TAG = "example";

#define MOUNT_POINT "/sdcard"

void sdmcc_initt(void)
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.

    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
     host.max_freq_khz = SDMMC_FREQ_HIGHSPEED; //在可以40mhz的电路中可以设置成高速模式
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, change this to 1:
    slot_config.width = 4;

    // On chips where the GPIOs used for SD card can be configured, set them in
    // the slot_config structure:
#ifdef SOC_SDMMC_USE_GPIO_MATRIX
    slot_config.clk = GPIO_NUM_36;
    slot_config.cmd = GPIO_NUM_37;
    slot_config.d0 = GPIO_NUM_17;
    slot_config.d1 = GPIO_NUM_15;
    slot_config.d2 = GPIO_NUM_6;
    slot_config.d3 = GPIO_NUM_18;
#endif

    // Enable internal pullups on enabled pins. The internal pullups//在已启用的引脚上启用内部拉出。内部引体向上
    // are insufficient however, please make sure 10k external pullups are//不足，但是，请确保10k外部拉出
    // connected on the bus. This is for debug / example purpose only.但是，一定要把它放在外面//在总线上连接。这仅用于调试/示例目的。
    slot_config.flags |=SDMMC_SLOT_WIDTH_DEFAULT|SDMMC_SLOT_NO_WP|SDMMC_SLOT_NO_CD /* | SDMMC_SLOT_FLAG_INTERNAL_PULLUP */ ;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);//结束后会填充card的信息

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties//卡已初始化，打印其属性
    sdmmc_card_print_info(stdout, card);//打印sd卡数据
    // // All done, unmount partition and disable SDMMC peripheral
    // esp_vfs_fat_sdcard_unmount(mount_point, card);//卸载分区和禁用dsmmc的
    // ESP_LOGI(TAG, "Card unmounted");
}