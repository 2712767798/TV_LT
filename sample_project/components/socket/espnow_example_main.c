/* ESPNOW Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
   This example shows how to use ESPNOW.
   Prepare two device, one for sending ESPNOW data and another for receiving
   ESPNOW data.
*/

#include "espnow_example.h"
#include "esp_mac.h"

#define ESPNOW_MAXDELAY 512

static const char *TAG = "espnow_example";

static QueueHandle_t s_example_espnow_queue;

static uint8_t s_example_broadcast_mac[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 广播发送的地址
static uint8_t s_example_broadcast_macs[2][ESP_NOW_ETH_ALEN] = {{0x60, 0x55, 0xF9, 0x79, 0x84, 0xc4}, {0x60, 0x55, 0xf9, 0x74, 0x78, 0xb4}};
static uint16_t s_example_espnow_seq[EXAMPLE_ESPNOW_DATA_MAX] = {0, 0}; // 这个估计是随着我增加的设备回自动调整发送给哪个设备的发送次数
static bool wifi_open = false;

static void example_espnow_deinit(example_espnow_send_param_t *send_param);

/* WiFi should start before using ESPNOW */
static void example_wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH /* WIFI_STORAGE_RAM */));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());
    wifi_open = true;

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif
}

/* ESPNOW sending or receiving callback function is called in WiFi task.
 * Users should not do lengthy operations from this task. Instead, post
 * necessary data to a queue and handle it from a lower priority task.
 ESPNOW发送或接收回调函数在WiFi任务中调用
 用户不应该从这个任务中进行长时间的操作。相反，将必要的数据发布到队列中，并从较低优先级的任务中处理它。*/
static void example_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    example_espnow_event_t evt;
    example_espnow_event_send_cb_t *send_cb = &evt.info.send_cb;

    if (mac_addr == NULL)
    {
        ESP_LOGE(TAG, "Send cb arg error");
        return;
    }

    evt.id = EXAMPLE_ESPNOW_SEND_CB;
    memcpy(send_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    send_cb->status = status;
    if (xQueueSend(s_example_espnow_queue, &evt, ESPNOW_MAXDELAY) != pdTRUE)
    {
        ESP_LOGW(TAG, "Send send queue fail");
    }
}

static void example_espnow_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    example_espnow_event_t evt;
    example_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;

    if (mac_addr == NULL || data == NULL || len <= 0)
    {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    evt.id = EXAMPLE_ESPNOW_RECV_CB;
    memcpy(recv_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    recv_cb->data = malloc(len);
    if (recv_cb->data == NULL)
    {
        ESP_LOGE(TAG, "Malloc receive data fail");
        return;
    }
    memcpy(recv_cb->data, data, len);
    recv_cb->data_len = len;
    if (xQueueSend(s_example_espnow_queue, &evt, ESPNOW_MAXDELAY) != pdTRUE)
    {
        ESP_LOGW(TAG, "Send receive queue fail");
        free(recv_cb->data);
    }
}

void esp_send(uint8_t *dat)
{
    example_espnow_event_t evt;

    evt.id = EXAMPLE_ESPNOW_SEND_ED;
    memcpy(evt.payload, dat, 2);
    if (xQueueSend(s_example_espnow_queue, &evt, ESPNOW_MAXDELAY) != pdTRUE)
    {
        ESP_LOGW(TAG, "Send send queue fail");
    }
}
////////////////////////////////////////////////////////////
/* Parse received ESPNOW data.解析接收到的ESPNOW数据。 */
int example_espnow_data_parse(uint8_t *data, uint16_t data_len, uint8_t *state, uint16_t *seq, int *magic, uint8_t *dat)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)data;
    uint16_t crc, crc_cal = 0;

    if (data_len < sizeof(example_espnow_data_t))
    {
        ESP_LOGE(TAG, "Receive ESPNOW data too short, len:%d", data_len);
        return -1;
    }
    // ESP_LOGE(TAG, "Receive Status, len:%d", buf->state);
    *state = buf->state; //
    *seq = buf->seq_num;
    *magic = buf->magic;
    memcpy(dat, buf->payload, sizeof(buf->payload));
    // ESP_LOGI("data","%s",dat);
    crc = buf->crc;
    buf->crc = 0;
    crc_cal = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, data_len);

    if (crc_cal == crc)
    {
        return buf->type;
    }

    return -1;
}

/* Prepare ESPNOW data to be sent.准备要发送的ESPNOW数据。 */
void espnow_data_prepare(example_espnow_send_param_t *send_param, uint8_t *dat)
{

    example_espnow_data_t *buf = (example_espnow_data_t *)send_param->buffer;

    assert(send_param->len >= sizeof(example_espnow_data_t));
    buf->type = EXAMPLE_ESPNOW_DATA_UNICAST;          //
    buf->state = send_param->state;                   // 一开始设置的是0
    buf->seq_num = s_example_espnow_seq[buf->type]++; // 计算发送了数据的序列
    buf->crc = 0;
    buf->magic = send_param->magic;
    buf->payload[0] = dat[0]; // 装置序号，1表示非红外控制2表示红外
    buf->payload[1] = dat[1]; // 装置状态，在此1测试表示为开、红外前四位按钮后四位0学习1开启（升）2关闭（降）
    if (dat[0] & 0x01)
    {
        memcpy(send_param->dest_mac, s_example_broadcast_macs[0], ESP_NOW_ETH_ALEN);
        // ESP_LOGI(TAG, "send to : " MACSTR ",", MAC2STR(send_param->dest_mac));
    }
    else
    {
        memcpy(send_param->dest_mac, s_example_broadcast_macs[1], ESP_NOW_ETH_ALEN);
    }
    buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, send_param->len); // crc16校验用的
}

void example_espnow_data_prepare(example_espnow_send_param_t *send_param)
{
    example_espnow_data_t *buf = (example_espnow_data_t *)send_param->buffer;

    assert(send_param->len >= sizeof(example_espnow_data_t));
    buf->type = IS_BROADCAST_ADDR(send_param->dest_mac) ? EXAMPLE_ESPNOW_DATA_BROADCAST : EXAMPLE_ESPNOW_DATA_UNICAST; //
    buf->state = send_param->state;                                                                                    // 一开始设置的是0
    buf->seq_num = s_example_espnow_seq[buf->type]++;                                                                  // 计算发送了数据的序列，估计是发送了多少次
    buf->crc = 0;                                                                                                      // 校验用的
    buf->magic = send_param->magic;                                                                                    // 是主机还是从机
    /* Fill all remaining bytes after the data with random values */
    /*用随机值填充数据后的所有剩余字节*/
    // esp_fill_random(buf->payload, send_param->len - sizeof(example_espnow_data_t));
    // buf->payload=(uint8_t*)(da);
    // memcpy(da, "niha", 5);//写入的数据
    // ESP_LOGE("DATA", "%d", send_param->len - sizeof(example_espnow_data_t));
    buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const *)buf, send_param->len); // 校验用的
}

static void example_espnow_task(void *pvParameter)
{
    example_espnow_event_t evt;
    uint8_t dat[2];         // 第一位为0的时候发送的时候结束进程
    uint8_t recv_state = 0; // 判断接收数据的广播还是单独的
    uint16_t recv_seq = 0;
    int recv_magic = 0; // 判断接收数据的优先级
    bool is_broadcast = false;
    bool is_link = true;
    int ret;

    example_espnow_send_param_t *send_param = (example_espnow_send_param_t *)pvParameter;

    while (xQueueReceive(s_example_espnow_queue, &evt, portMAX_DELAY) == pdTRUE)
    {
        switch (evt.id)
        {
        case EXAMPLE_ESPNOW_SEND_ED:
        {
            if (evt.payload[0] == 0) // 如果发送的内容为0则自毁
            {
                ESP_LOGE(TAG, "end task");
                example_espnow_deinit(send_param);
                vTaskDelete(NULL);
            }
            memcpy(dat, evt.payload, 2);   // 传入数据
            send_param->broadcast = false; // 关掉广播，
            send_param->unicast = true;    // 开启私播

            espnow_data_prepare(send_param, evt.payload); // 传入数据
            if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK)
            {
                ESP_LOGE(TAG, "Send error1");
                example_espnow_deinit(send_param);
                vTaskDelete(NULL);
            }
            break;
        }
        case EXAMPLE_ESPNOW_SEND_CB:
        {
            if (is_link == false)
            {
                example_espnow_event_send_cb_t *send_cb = &evt.info.send_cb;
                is_broadcast = IS_BROADCAST_ADDR(send_cb->mac_addr); // 比较广播地址，要是不一样则为斯波

                ESP_LOGD(TAG, "Send data to " MACSTR ", status1: %d", MAC2STR(send_cb->mac_addr), send_cb->status);

                if (is_broadcast && (send_param->broadcast == false))
                {
                    break; // 如果已经广播出去但是没有数据里没说广播成功的话就break
                }

                if (!is_broadcast)
                {
                    ESP_LOGI(TAG, "link success");
                    is_link = true;
                    break;
                    // example_espnow_deinit(send_param);
                    // vTaskDelete(NULL);
                }
                if (send_param->count == 0) // 如果在发送读秒发完没发送出去则说明发送完了
                {
                    ESP_LOGI(TAG, "Send done");
                    // example_espnow_deinit(send_param);
                    vTaskDelete(NULL);
                }

                /* Delay a while before sending the next data. 在发送下一个数据之前延迟一段时间。
                之前的哪个数据帧里面的延迟时间是在这里用的*/
                if (send_param->delay > 0)
                {
                    vTaskDelay(send_param->delay / portTICK_PERIOD_MS);
                }

                ESP_LOGI(TAG, "send data to " MACSTR "", MAC2STR(send_cb->mac_addr));

                memcpy(send_param->dest_mac, send_cb->mac_addr, ESP_NOW_ETH_ALEN);
                example_espnow_data_prepare(send_param);

                /* Send the next data after the previous data is sent. */
                /*上一个数据发送后再发送下一个数据。*/
                if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK)
                {
                    ESP_LOGE(TAG, "Send error2");
                    example_espnow_deinit(send_param);
                    vTaskDelete(NULL);
                }
                send_param->count--;
                break;
            }
            else
            {
                // ESP_LOGI("espnow", "send success");
                break;
            }
        }
        case EXAMPLE_ESPNOW_RECV_CB:
        {
            example_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;

            ret = example_espnow_data_parse(recv_cb->data, recv_cb->data_len, &recv_state, &recv_seq, &recv_magic, dat);
            free(recv_cb->data);
            ESP_LOGI("data", "%s", dat); // 此为收到的数据
            if (ret == EXAMPLE_ESPNOW_DATA_BROADCAST)
            {
                ESP_LOGI(TAG, "Receive %dth broadcast data from: " MACSTR ", len: %d", recv_seq, MAC2STR(recv_cb->mac_addr), recv_cb->data_len);

                /* If MAC address does not exist in peer list, add it to peer list. */
                /*如果对等体列表中没有MAC地址，则将MAC地址添加到对等体列表中。*/
                if (esp_now_is_peer_exist(recv_cb->mac_addr) == false)
                {
                    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
                    if (peer == NULL)
                    {
                        ESP_LOGE(TAG, "Malloc peer information fail");
                        example_espnow_deinit(send_param);
                        vTaskDelete(NULL);
                    }
                    memset(peer, 0, sizeof(esp_now_peer_info_t));
                    peer->channel = CONFIG_ESPNOW_CHANNEL;
                    peer->ifidx = ESPNOW_WIFI_IF;
                    peer->encrypt = true;
                    memcpy(peer->lmk, CONFIG_ESPNOW_LMK, ESP_NOW_KEY_LEN);
                    memcpy(peer->peer_addr, recv_cb->mac_addr, ESP_NOW_ETH_ALEN);
                    ESP_ERROR_CHECK(esp_now_add_peer(peer));
                    free(peer);
                }

                /* Indicates that the device has received broadcast ESPNOW data. */
                /*设备收到广播ESPNOW数据。*/
                if (send_param->state == 0)
                {
                    send_param->state = 1;
                } // 收到广播猴将state置一表示已经收到

                /* If receive broadcast ESPNOW data which indicates that the other device has received
                 * broadcast ESPNOW data and the local magic number is bigger than that in the received
                 * broadcast ESPNOW data, stop sending broadcast ESPNOW data and start sending unicast
                 * ESPNOW data.
                 * *如果接收到广播ESPNOW数据，表示另一台设备已经接收到*广播ESPNOW数据，
                 * 且本地魔术数大于接收到的*广播ESPNOW数据，则停止发送广播ESPNOW数据，开始发送单播数据。
                 */
                if (recv_state == 1)
                {
                    /* The device which has the bigger magic number sends ESPNOW data, the other one
                     receives ESPNOW data.
                     魔数大的设备发送ESPNOW数据，其他的设备接收ESPNOW数据。*/
                    if (send_param->unicast == false && send_param->magic >= recv_magic)
                    {
                        ESP_LOGI(TAG, "Start sending unicast data");
                        ESP_LOGI(TAG, "send data to " MACSTR "", MAC2STR(recv_cb->mac_addr));

                        /* Start sending unicast ESPNOW data. */
                        memcpy(send_param->dest_mac, recv_cb->mac_addr, ESP_NOW_ETH_ALEN);
                        example_espnow_data_prepare(send_param);
                        if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK)
                        {
                            ESP_LOGE(TAG, "Send error3");
                            example_espnow_deinit(send_param);
                            vTaskDelete(NULL);
                        }
                        else
                        {
                            send_param->broadcast = false;
                            send_param->unicast = true;
                        }
                    }
                }
            }
            else if (ret == EXAMPLE_ESPNOW_DATA_UNICAST)
            {
                ESP_LOGI(TAG, "Receive %dth unicast data from: " MACSTR ", len: %d", recv_seq, MAC2STR(recv_cb->mac_addr), recv_cb->data_len);

                /* If receive unicast ESPNOW data, also stop sending broadcast ESPNOW data. */
                /*如果收到单播ESPNOW数据，也停止发送广播ESPNOW数据。*/
                send_param->broadcast = false;
            }
            else
            {
                ESP_LOGI(TAG, "Receive error data from: " MACSTR "", MAC2STR(recv_cb->mac_addr));
            }
            break;
        }
        default:
            ESP_LOGE(TAG, "Callback type error: %d", evt.id);
            break;
        }
    }
}
// static void espnow_send_task(void *pvParameter)
// {
//     if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK)
//     {
//         ESP_LOGE(TAG, "Send error");
//         example_espnow_deinit(send_param);
//         vTaskDelete(NULL);
//     }
// }

static esp_err_t example_espnow_init(void)
{
    example_espnow_send_param_t *send_param;

    s_example_espnow_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(example_espnow_event_t));
    if (s_example_espnow_queue == NULL)
    {
        ESP_LOGE(TAG, "Create mutex fail");
        return ESP_FAIL;
    }

    /* Initialize ESPNOW and register sending and receiving callback function. 初始化ESPNOW并注册发送和接收回调函数*/
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(example_espnow_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(example_espnow_recv_cb));

    /* Set primary master key.设置主主键，加密用的*/
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK));
    // 添加设备
    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
    if (peer == NULL)
    {
        ESP_LOGE(TAG, "Malloc peer information fail");
        vSemaphoreDelete(s_example_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = CONFIG_ESPNOW_CHANNEL;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    memcpy(peer->peer_addr, s_example_broadcast_mac, ESP_NOW_ETH_ALEN);
    ESP_ERROR_CHECK(esp_now_add_peer(peer)); // 广播数据
    memcpy(peer->peer_addr, s_example_broadcast_macs[0], ESP_NOW_ETH_ALEN);
    ESP_ERROR_CHECK(esp_now_add_peer(peer)); // 第一个设备地址
    memcpy(peer->peer_addr, s_example_broadcast_macs[1], ESP_NOW_ETH_ALEN);
    ESP_ERROR_CHECK(esp_now_add_peer(peer)); // 第二个
    free(peer);                              // 用完就扔

    /* Initialize sending parameters. */
    send_param = malloc(sizeof(example_espnow_send_param_t));   // 发送前的数据的准备
    memset(send_param, 0, sizeof(example_espnow_send_param_t)); // 清零
    if (send_param == NULL)
    {
        ESP_LOGE(TAG, "Malloc send parameter fail");
        vSemaphoreDelete(s_example_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    send_param->unicast = false;
    send_param->broadcast = true;                                       // 广播开启
    send_param->state = 0;                                              // 默认没收到回值
    send_param->magic = 1 /* esp_random() */;                           // 这里看谁是主机
    send_param->count = CONFIG_ESPNOW_SEND_COUNT;                       // 发送次数
    send_param->delay = CONFIG_ESPNOW_SEND_DELAY;                       // 最低延迟，若延迟内不回当。。
    send_param->len = CONFIG_ESPNOW_SEND_LEN;                           // 数据的长度
    send_param->buffer = /* '10011'  */ malloc(CONFIG_ESPNOW_SEND_LEN); // 开辟12个字节的空间
    if (send_param->buffer == NULL)
    {
        ESP_LOGE(TAG, "Malloc send buffer fail");
        free(send_param);
        vSemaphoreDelete(s_example_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memcpy(send_param->dest_mac, s_example_broadcast_mac, ESP_NOW_ETH_ALEN); // 广播的地址
    example_espnow_data_prepare(send_param);                                 // 交给预发送部

    xTaskCreate(example_espnow_task, "espnow_task", 1024 * 3, send_param, 4, NULL);

    return ESP_OK;
}

static void example_espnow_deinit(example_espnow_send_param_t *send_param)
{
    free(send_param->buffer);
    free(send_param);
    vSemaphoreDelete(s_example_espnow_queue);
    esp_now_deinit();
}

static void wifi_deinit(void)
{
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());               // wifi的任务
    ESP_ERROR_CHECK(esp_event_loop_delete_default()); // wi_evt的任务
    esp_wifi_restore();
    wifi_open = false;
    // ESP_ERROR_CHECK(esp_netif_deinit());              // ti目前并不能去初始化
}
void wifi_end(void)
{
    wifi_deinit();
}
void esp_now_all_deinit(void)
{
    uint8_t dat[2] = {0, 0};
    esp_send(dat); // 结束进程
    wifi_deinit();
}

void espnow_init(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if (wifi_open == false)
    {
        example_wifi_init();
    }
    else
    {
        ESP_ERROR_CHECK(esp_wifi_start());
    }
    example_espnow_init();
}
