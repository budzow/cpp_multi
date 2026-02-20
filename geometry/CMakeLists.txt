add_library(geometry
    src/point.cpp
    src/shape.cpp
    src/transform.cpp
)

target_include_directories(geometry
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
