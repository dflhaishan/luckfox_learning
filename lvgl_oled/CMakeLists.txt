cmake_minimum_required(VERSION 3.23)

set(CMAKE_C_COMPILER arm-rockchip830-linux-uclibcgnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-rockchip830-linux-uclibcgnueabihf-g++)

project(oled)

add_executable(oled)

target_sources(oled PRIVATE oled_main.c)

target_compile_options(oled PRIVATE -Wall)

set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h)

add_subdirectory(lvgl)

target_link_libraries(oled PRIVATE lvgl::lvgl lvgl::examples)
