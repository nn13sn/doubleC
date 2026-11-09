cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DoubleC LANGUAGES CXX ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

set(ASMJIT_STATIC TRUE)
add_subdirectory(external/asmjit)

add_executable(DoubleC
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/analyzer.cpp
    src/runtime_value.cpp
    src/runtime_variable.cpp
    src/environment.cpp
    src/utils.cpp
    src/generator.cpp
    src/VM.cpp
    src/runtime_operations.cpp
    src/runtime_cast.cpp
    src/runtime_streams.cpp
    src/compiler.cpp
    src/runtimeJIT_operations.cpp
    src/runtime_ops.asm
    src/error_handle.cpp
    src/jit_return.cpp
    src/JIT_integration.asm
)

target_compile_features(DoubleC PRIVATE cxx_std_20)

target_link_libraries(DoubleC PRIVATE asmjit::asmjit)

target_compile_options(DoubleC PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>

    # Debug
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-gdwarf-4>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-g>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-O0>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-fsanitize=address>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-fno-omit-frame-pointer>

    # Release
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:-O3>
)

target_link_options(DoubleC PRIVATE
       $<$<CONFIG:Debug>:-fsanitize=address>
)

target_include_directories(DoubleC PRIVATE include)
