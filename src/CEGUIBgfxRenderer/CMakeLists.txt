find_package(bgfx CONFIG REQUIRED)

if(NOT TARGET bgfx::bgfx)
    message(FATAL_ERROR "The bgfx CMake package does not provide bgfx::bgfx.")
endif()
if(NOT TARGET bgfx::shaderc)
    message(FATAL_ERROR "bgfx::shaderc is unavailable. Install bgfx with the vcpkg 'tools' feature.")
endif()
if(NOT DEFINED BGFX_SHADER_INCLUDE_PATH OR
   NOT EXISTS "${BGFX_SHADER_INCLUDE_PATH}/bgfx_shader.sh")
    message(FATAL_ERROR "The bgfx package did not expose a valid BGFX_SHADER_INCLUDE_PATH.")
endif()

file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

add_library(CEGUIBgfxRenderer ${FILE_SOURCES} ${FILE_HEADERS})
add_library(CEGUI::BgfxRenderer ALIAS CEGUIBgfxRenderer)

target_include_directories(CEGUIBgfxRenderer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIBgfxRenderer
    PUBLIC bgfx::bgfx
    PRIVATE CEGUIBase
)

target_compile_definitions(CEGUIBgfxRenderer
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBGFXRENDERER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBGFXRENDERER_IMPORTS>
)

if(MSVC)
    # bx validates these conformance switches when included by MSVC. Keep them
    # scoped to the bgfx renderer so legacy CEGUI modules are unaffected.
    target_compile_options(CEGUIBgfxRenderer PRIVATE /Zc:__cplusplus /Zc:preprocessor)
endif()

set(CEGUI_BGFX_SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CEGUI/RendererModules/Bgfx/Shaders")
set(CEGUI_BGFX_SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/Shaders/bgfx")
set(CEGUI_BGFX_VARYING_DEF "${CEGUI_BGFX_SHADER_SOURCE_DIR}/varying.def.sc")

function(cegui_bgfx_compile_shader OUT_VAR SOURCE TYPE BACKEND PLATFORM PROFILE)
    get_filename_component(_name "${SOURCE}" NAME_WE)
    set(_out_dir "${CEGUI_BGFX_SHADER_BINARY_DIR}/${BACKEND}")
    set(_output "${_out_dir}/${_name}.bin")
    add_custom_command(
        OUTPUT "${_output}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_out_dir}"
        COMMAND bgfx::shaderc
            -f "${SOURCE}"
            -o "${_output}"
            --type "${TYPE}"
            --platform "${PLATFORM}"
            -p "${PROFILE}"
            --varyingdef "${CEGUI_BGFX_VARYING_DEF}"
            -i "${BGFX_SHADER_INCLUDE_PATH}"
        DEPENDS "${SOURCE}" "${CEGUI_BGFX_VARYING_DEF}" bgfx::shaderc
        VERBATIM
        COMMENT "Compiling CEGUI bgfx ${BACKEND} ${TYPE} shader ${_name}"
    )
    set(${OUT_VAR} "${_output}" PARENT_SCOPE)
endfunction()

set(CEGUI_BGFX_COMPILED_SHADERS)
if(WIN32)
    cegui_bgfx_compile_shader(_vs_dx11 "${CEGUI_BGFX_SHADER_SOURCE_DIR}/vs_cegui.sc" vertex dx11 windows s_5_0)
    cegui_bgfx_compile_shader(_fs_dx11 "${CEGUI_BGFX_SHADER_SOURCE_DIR}/fs_cegui.sc" fragment dx11 windows s_5_0)
    cegui_bgfx_compile_shader(_vs_glsl "${CEGUI_BGFX_SHADER_SOURCE_DIR}/vs_cegui.sc" vertex glsl windows 150)
    cegui_bgfx_compile_shader(_fs_glsl "${CEGUI_BGFX_SHADER_SOURCE_DIR}/fs_cegui.sc" fragment glsl windows 150)
else()
    cegui_bgfx_compile_shader(_vs_glsl "${CEGUI_BGFX_SHADER_SOURCE_DIR}/vs_cegui.sc" vertex glsl linux 150)
    cegui_bgfx_compile_shader(_fs_glsl "${CEGUI_BGFX_SHADER_SOURCE_DIR}/fs_cegui.sc" fragment glsl linux 150)
endif()
cegui_bgfx_compile_shader(_vs_spirv "${CEGUI_BGFX_SHADER_SOURCE_DIR}/vs_cegui.sc" vertex spirv linux spirv)
cegui_bgfx_compile_shader(_fs_spirv "${CEGUI_BGFX_SHADER_SOURCE_DIR}/fs_cegui.sc" fragment spirv linux spirv)

if(WIN32)
    list(APPEND CEGUI_BGFX_COMPILED_SHADERS "${_vs_dx11}" "${_fs_dx11}")
endif()
list(APPEND CEGUI_BGFX_COMPILED_SHADERS "${_vs_glsl}" "${_fs_glsl}" "${_vs_spirv}" "${_fs_spirv}")

add_custom_target(CEGUIBgfxShaders DEPENDS ${CEGUI_BGFX_COMPILED_SHADERS})
add_dependencies(CEGUIBgfxRenderer CEGUIBgfxShaders)

target_compile_definitions(CEGUIBgfxRenderer PRIVATE
    CEGUI_BGFX_DEFAULT_SHADER_ROOT="${CEGUI_BGFX_SHADER_BINARY_DIR}"
)

install_target(CEGUIBgfxRenderer HEADER_DIRS CEGUI)
install(DIRECTORY "${CEGUI_BGFX_SHADER_BINARY_DIR}/"
    DESTINATION "share/CEGUI/Shaders/bgfx"
)
