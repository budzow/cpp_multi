add_library(io
    src/logger.cpp
    src/file_writer.cpp
)

target_include_directories(io
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
