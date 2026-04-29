file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUICommonDialogs ${FILE_SOURCES} ${FILE_HEADERS})

target_include_directories(CEGUICommonDialogs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUICommonDialogs
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUICommonDialogs
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUICommonDialogs
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUICOMMONDIALOGS_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUICOMMONDIALOGS_IMPORTS>
)

install_target(CEGUICommonDialogs)