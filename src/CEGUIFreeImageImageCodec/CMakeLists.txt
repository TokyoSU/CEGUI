file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIFreeImageImageCodec ${FILE_SOURCES} ${FILE_HEADERS})

find_package(freeimage CONFIG REQUIRED)

target_include_directories(CEGUIFreeImageImageCodec
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIFreeImageImageCodec
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIFreeImageImageCodec
    PRIVATE
        CEGUIBase
        freeimage::FreeImage
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIFreeImageImageCodec
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIFREEIMAGEIMAGECODEC_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIFREEIMAGEIMAGECODEC_IMPORTS>
)

install_target(CEGUIFreeImageImageCodec)