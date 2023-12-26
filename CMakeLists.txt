#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.26)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(SPI-detect C ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

FILE(GLOB C_DRIVER_FILES Drivers/STM32F4xx_HAL_Driver/Src/*.c)
FILE(GLOB CXX_DRIVER_FILES Drivers/STM32F4xx_HAL_Driver/Src/*.cpp)
FILE(GLOB C_USER_FILES Core/Src/*.c)
FILE(GLOB CXX_USER_FILES Core/Src/*.cpp)

# 链接脚本配置
set(SRC_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH_IAP.ld)
set(DST_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH_IAP_DST.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork --specs=nano.specs --specs=rdimon.specs -lc)
add_link_options(-T ${DST_LINKER_SCRIPT})

# 可执行文件
add_executable(${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# 源文件
target_sources(
        ${PROJECT_NAME}.elf
        PUBLIC startup_stm32f407vgtx.s  # 不采用MDK-ARM目录下的.s文件，是由于编译器不一样
        PUBLIC ${DST_LINKER_SCRIPT}     # 链接脚本作为源文件，否则在链接中报错未定义
        PUBLIC ${C_DRIVER_FILES} ${CXX_DRIVER_FILES}
        PRIVATE ${C_USER_FILES} ${CXX_USER_FILES}
)

# 头文件
target_include_directories(
        ${PROJECT_NAME}.elf
        PUBLIC Drivers/CMSIS/Include Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/STM32F4xx_HAL_Driver/Inc
        PRIVATE Core/Inc
)

add_definitions(-DDEBUG -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -DSTM32F407xx)

# 获取头文件搜索路径
#get_property(INC_DIRS DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#message("INC_DIRS : ${INC_DIRS}")

# 添加自定义指令和目标，参考: https://cmake.org/cmake/help/latest/command/add_custom_command.html#example-generating-files-for-multiple-targets
add_custom_command(OUTPUT ${DST_LINKER_SCRIPT}
        COMMAND ${CMAKE_C_COMPILER} -E -P -I${CMAKE_SOURCE_DIR}/Core/Inc
        -x c ${SRC_LINKER_SCRIPT} -o ${DST_LINKER_SCRIPT}
        DEPENDS ${SRC_LINKER_SCRIPT} Core/Inc/config.h
        COMMENT "根据宏定义生成链接脚本"
        VERBATIM
)
add_custom_target(gen_STM32F407VGTX_FLASH_IAP_DST DEPENDS ${DST_LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        DEPENDS ${DST_LINKER_SCRIPT}
        gen_STM32F407VGTX_FLASH_IAP_DST
        COMMENT "构建 ${HEX_FILE} 和 ${BIN_FILE}"
)