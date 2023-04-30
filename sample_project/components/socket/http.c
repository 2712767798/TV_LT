#include "http.h"



/* An HTTP GET handler */
esp_err_t hello_get_handler(httpd_req_t *req)
{
    char *buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI("TAG", "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Accept-Language") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Accept-Language", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI("TAG", "Found header => Accept-Language: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Connection") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Connection", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI("TAG", "Found header => Connection: %s", buf);
        }
        free(buf);
    }

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1; // 超级链接，可以通过不同的链接返回不同的值
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
        {
            ESP_LOGI("TAG", "Found URL query => %s", buf);
            char param[32];
            char *bufff;
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "id", param, sizeof(param)) == ESP_OK) // 这里取？等于后的数字
            {                                                                     // 设置为id
                asprintf(&bufff, "{\"啊\": true,\"user\": \"是\"}");
                req->user_ctx = bufff /*"id 为 1"*/;
                ESP_LOGI("TAG", "Found URL query parameter => param=%s", param);
            }
            if (httpd_query_key_value(buf, "number", param, sizeof(param)) == ESP_OK)
            { // 设置为number
                req->user_ctx = "number 为 5";
                ESP_LOGI("TAG", "Found URL query parameter => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "post", param, sizeof(param)) == ESP_OK)
            {
                // req->user_ctx = from;
                ESP_LOGI("TAG", "Found URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    /* Set some custom headers
    设置一些自定义头
    即应答的头部信息*/
    // httpd_resp_set_type(req, "application/json;charset=UTF-8");
    httpd_resp_set_type(req, "html;charset=UTF-8");
    httpd_resp_set_hdr(req, "Connection", "keep-alive");
    httpd_resp_set_hdr(req, "sever name", "xiaomo");
    httpd_resp_set_hdr(req, "sever size", "5M");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    /*发送响应，自定义报头和正文设置为用户上下文中传递的字符串*/
    const char *resp_str = (const char *)req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN); // 在这返回正文信息

    /* After sending the HTTP response the old HTTP request在发送HTTP响应后，旧的HTTP请求
     * headers are lost. Check if HTTP request headers can be read now.头文件丢失。检查HTTP请求头现在是否可以读取。 */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0)
    {
        ESP_LOGI("TAG", "Request headers lost");
    }
    return ESP_OK;
}

const httpd_uri_t hello = {
    .uri = "/hello",
    .method = HTTP_GET,
    .handler = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = "Hello World!"};

/* An HTTP POST handler */
esp_err_t echo_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;
    while (remaining > 0)
    {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                                  MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        /* Send back the same data */
        httpd_resp_send(req, buf, ret);
        // httpd_resp_send_chunk(req, http_auth_debasic(buf), ret);
        remaining -= ret;

        /* Log data received */
        ESP_LOGI("TAG", "=========== RECEIVED DATA ==========");
        ESP_LOGI("TAG", "%.*s", ret, buf);
        ESP_LOGI("TAG", "====================================");
        ESP_LOGI("TAG", "=========== RECEIVED DATA ==========");
        ESP_LOGI("TAG", "你好，再见");
        ESP_LOGI("TAG", "====================================");
    }

    // End response
    // httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

const httpd_uri_t echo = {
    .uri = "/echo",
    .method = HTTP_POST,
    .handler = echo_post_handler,
    .user_ctx = NULL};

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI("http", "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) // 开始http
    {
        // Set URI handlers
        ESP_LOGI("http", "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        httpd_register_uri_handler(server, &echo);
        // httpd_register_uri_handler(server, &ctrl);
#if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
#endif
        return server;
    }

    ESP_LOGI("http", "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_ssl_stop(server);
}

void disconnect_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server)
    {
        ESP_LOGI("http", "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

void connect_handler(void *arg, esp_event_base_t event_base,
                     int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server == NULL)
    {
        ESP_LOGI("http", "Starting webserver");
        *server = start_webserver();
    }
}

IRAM_ATTR void IF_http_init(void)
{
    httpd_handle_t server = NULL;

    // ESP_ERROR_CHECK(nvs_flash_init());
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

/* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
 * Read "Establishing Wi-Fi or Ethernet Connection" section in
 * examples/protocols/README.md for more information about this function.
 */

    ESP_ERROR_CHECK(example_connect()); // wifi的连接，等待ip
    /* Register event handlers to stop the server when Wi-Fi or Ethernet is disconnected,
     * and re-start it upon connection.
     */
    // uart_event_type_t
    // ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));              // 当得到ip时候运行
    // ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server)); // 当wifi断开时运行
    // 第一给是基础名字第二个是id第三个是处理函数第四个是数据

    /* Start the server for the first time */
    server = start_webserver(); // 在这儿开启并获得句柄
    vTaskDelete(NULL);
}