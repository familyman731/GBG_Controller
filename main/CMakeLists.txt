set(SOURCES main.c esp_hid_gap.c)
set(include_dirs . ../include)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ${include_dirs}
                    REQUIRES esp_hid
                    PRIV_REQUIRES nvs_flash)
