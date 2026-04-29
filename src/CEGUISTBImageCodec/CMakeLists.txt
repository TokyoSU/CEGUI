file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUISTBImageCodec ${FILE_SOURCES} ${FILE_HEADERS})

find_package(Stb REQUIRED)

target_include_directories(CEGUISTBImageCodec
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUISTBImageCodec
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(CEGUISTBImageCodec
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUISTBImageCodec
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUISTBIMAGECODEC_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUISTBIMAGECODEC_IMPORTS>
)

install_target(CEGUISTBImageCodec)