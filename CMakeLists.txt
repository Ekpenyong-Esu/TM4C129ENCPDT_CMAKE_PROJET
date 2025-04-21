cmake_minimum_required(VERSION 3.16)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake)
# Generate compile_commands.json for vscode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Setup project, output, and linker files
project(TM4C129ENCPDT)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/tm4c129encpdt.ld)
set(TIVAWARE_DIR ${CMAKE_SOURCE_DIR}/libraries/TivaWare)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Common compiler options for both the application and driverlib
set(COMMON_COMPILE_OPTIONS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffunction-sections
    -fdata-sections
)

# Build TivaWare driverlib and include CPU driver
file(GLOB DRIVERLIB_SOURCES 
    ${TIVAWARE_DIR}/driverlib/*.c
    ${TIVAWARE_DIR}/driverlib/cpu.c
)

add_library(driverlib STATIC ${DRIVERLIB_SOURCES})

target_include_directories(driverlib PRIVATE
    ${TIVAWARE_DIR}
)

target_compile_definitions(driverlib PRIVATE
    -DPART_TM4C129ENCPDT
    -DTARGET_IS_TM4C129_RA2
)

# Apply the common compiler options to driverlib
target_compile_options(driverlib PRIVATE
    ${COMMON_COMPILE_OPTIONS}
    -O2  # Optimize for speed
)

# Add CPU assembly file for intrinsics
if(EXISTS "${TIVAWARE_DIR}/driverlib/cpu.asm")
    target_sources(driverlib PRIVATE ${TIVAWARE_DIR}/driverlib/cpu.asm)
endif()
if(EXISTS "${TIVAWARE_DIR}/driverlib/cpu.s")
    target_sources(driverlib PRIVATE ${TIVAWARE_DIR}/driverlib/cpu.s)
endif()

set(APP_SRC
    src/startup.c
    src/main.c
    src/PLL.c
    src/SysTick.c

)

add_executable(${EXECUTABLE} ${APP_SRC})

# List of include directories
target_include_directories(${EXECUTABLE} PRIVATE
    inc
    ${TIVAWARE_DIR}
)

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
    -DPART_TM4C129ENCPDT
    -DTARGET_IS_TM4C129_RA2
)

target_link_libraries(${EXECUTABLE} PUBLIC
    driverlib
    c
    m
    rdimon
    gcc
)

# Compiler options using the common options
target_compile_options(${EXECUTABLE} PRIVATE
    ${COMMON_COMPILE_OPTIONS}
    -O0
    -g3
    # Compile warnings
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_FILE}
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=rdimon.specs
    -Wl,--entry=ResetISR
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Wl,-print-memory-usage
)

# Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE}
)

# Create hex, bin and S-Record files after the build
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(S19_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.s19)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE}
    # COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 $<TARGET_FILE:${EXECUTABLE}> ${S19_FILE}
    # COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${HEX_FILE}
    COMMENT "Building ${BIN_FILE}"
)

set(OPENOCD_CFG ${CMAKE_SOURCE_DIR}/openocd.cfg)
include(cmake/flash-target.cmake)
flash_target(${BIN_FILE} flash)

set(VSCODE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.vscode")
include(cmake/vscode-debug.cmake)
vscode_debug(${PROJECT_NAME} ${EXECUTABLE})
