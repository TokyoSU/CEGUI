file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIDirect3D11Renderer ${FILE_SOURCES} ${FILE_HEADERS})

find_package(dxsdk-d3dx CONFIG REQUIRED)
find_package(effects11 CONFIG REQUIRED)
target_link_libraries(CEGUIDirect3D11Renderer PRIVATE Microsoft::D3DX10 Microsoft::D3DX11 Microsoft::Effects11)

target_include_directories(CEGUIDirect3D11Renderer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIDirect3D11Renderer
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIDirect3D11Renderer
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIDirect3D11Renderer
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDIRECT3D11RENDERER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDIRECT3D11RENDERER_IMPORTS>
)

install_target(CEGUIDirect3D11Renderer)