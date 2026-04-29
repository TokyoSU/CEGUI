file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIDirect3D10Renderer ${FILE_SOURCES} ${FILE_HEADERS})

find_package(dxsdk-d3dx CONFIG REQUIRED)
target_link_libraries(CEGUIDirect3D10Renderer PRIVATE Microsoft::D3DX10)

target_include_directories(CEGUIDirect3D10Renderer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIDirect3D10Renderer
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIDirect3D10Renderer
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIDirect3D10Renderer
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDIRECT3D10RENDERER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDIRECT3D10RENDERER_IMPORTS>
)

install_target(CEGUIDirect3D10Renderer)