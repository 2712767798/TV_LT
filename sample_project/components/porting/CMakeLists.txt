file(GLOB_RECURSE SOURCES 	./*.c 
							)


idf_component_register(SRCS	${SOURCES}
                   INCLUDE_DIRS 
								.
                    REQUIRES lvgl
                    lvgl_esp32_drivers-master
                    fatfs
                    ui
                    heap
                    esp_wifi
                    esp_timer
                      )
