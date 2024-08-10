idf_component_register(SRCS "hashtable.c/hashtable.c" "rpcbuff.c" "rpccall.c" "rpcclient.c" "rpcmsg.c" "rpcpack.c" "tqueque/tqueque.c" "rpcserver.c" "rpcstruct.c"
                    INCLUDE_DIRS "."
                    REQUIRES vfs esp_wifi nvs_flash esp_event fatfs esp_timer pthread freertos)

add_library(libffi STATIC IMPORTED GLOBAL)
set_property(TARGET libffi PROPERTY IMPORTED_LOCATION "${COMPONENT_DIR}/libffi/libffi.a")
target_link_libraries(${COMPONENT_TARGET} PUBLIC libffi)
