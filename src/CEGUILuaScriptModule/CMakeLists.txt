file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUILuaScriptModule ${FILE_SOURCES} ${FILE_HEADERS})

find_package(Lua REQUIRED)
target_include_directories(CEGUILuaScriptModule PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(CEGUILuaScriptModule PRIVATE ${LUA_LIBRARIES})

target_include_directories(CEGUILuaScriptModule
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUILuaScriptModule
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUILuaScriptModule
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUILuaScriptModule
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUILUASCRIPTMODULE_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUILUASCRIPTMODULE_IMPORTS>
)

install_target(CEGUILuaScriptModule)