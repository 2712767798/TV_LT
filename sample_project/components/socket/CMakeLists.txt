
file(GLOB_RECURSE SOURCES 	./*.c 
							)


idf_component_register(SRCS	${SOURCES}
                   INCLUDE_DIRS 
								.
                    REQUIRES nvs_flash
                    protocol_examples_common
                    esp_https_server
                      )