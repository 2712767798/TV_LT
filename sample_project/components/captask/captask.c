
#include "captask.h"

void cap_task_get_free(void)
{
    char list[40 * 10];
    size_t free_ram;
    vTaskList(list);
    ESP_LOGE("", "\n栈名\t栈状态\t优先级\t栈的剩余空间");
    ESP_LOGE("", "\n%s", list);
    free_ram = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGE("", "\n%d/%d btyes", free_ram, 1024 * 1024 * 2 + 200);
}