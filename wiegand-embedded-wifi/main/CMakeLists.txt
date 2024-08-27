idf_component_register(SRCS "HF_reader.c" "HF_httpserver.c" "HF_sntp.c" "main.c" "ds3231.c" "i2cdev.c" "HF_sntp.c" "HF_gpio.c" "HF_reader.c" "HF_TCPsocket.c" "HF_nvs.c" "HF_mqtt.c" "HF_wifi.c" "HF_storage.c" 
                    INCLUDE_DIRS "."
                    EMBED_FILES   "./wieganHost/runtime.js.gz" "./wieganHost/polyfills.js.gz" "./wieganHost/styles.css.gz" "./wieganHost/index.html" "./wieganHost/assets/logo.png" "./wieganHost/main.js.gz" "./wieganHost/assets/nodata.png" )

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)
