file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUICoreWindowRendererSet ${FILE_SOURCES} ${FILE_HEADERS})

target_include_directories(CEGUICoreWindowRendererSet
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUICoreWindowRendererSet
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUICoreWindowRendererSet
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUICoreWindowRendererSet
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUICOREWINDOWRENDERERSET_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUICOREWINDOWRENDERERSET_IMPORTS>
)

install_target(CEGUICoreWindowRendererSet)