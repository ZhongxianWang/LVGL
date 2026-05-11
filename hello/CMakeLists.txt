cmake_minimum_required(VERSION 3.12.4)
project(hello_lvgl LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

# ---- SDL2 (source build) ----
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/sdl2)

# Make SDL2 headers available globally (needed by LVGL's SDL driver)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sdl2/include)

if(WIN32)
    # 告诉 SDL 我们使用标准的 main 函数
    add_definitions(-DSDL_MAIN_HANDLED)
endif()

# ---- LVGL ----
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "Disable demos" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)
add_subdirectory(thirdparty/lvgl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lvgl/include)

# ---- Main executable ----
add_executable(hello main.cpp)

# Link LVGL and SDL2
target_link_libraries(hello PRIVATE lvgl::lvgl SDL2::SDL2)
