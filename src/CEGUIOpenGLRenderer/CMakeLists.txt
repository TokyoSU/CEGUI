file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIOpenGLRenderer ${FILE_SOURCES} ${FILE_HEADERS})

if(OPENGL_LIBRARY STREQUAL "GLAD")
    find_package(glad REQUIRED)
    target_link_libraries(CEGUIOpenGLRenderer PRIVATE glad::glad)
elseif(OPENGL_LIBRARY STREQUAL "EPOXY")
    find_package(epoxy REQUIRED)
    target_link_libraries(CEGUIOpenGLRenderer PRIVATE epoxy)
elseif(OPENGL_LIBRARY STREQUAL "GLEW")
    find_package(glew REQUIRED)
    target_link_libraries(CEGUIOpenGLRenderer PRIVATE glew)
else()
    message(FATAL_ERROR "Invalid OPENGL_LIBRARY='${OPENGL_LIBRARY}'. Expected GLAD, EPOXY, or GLEW.")
    message(FATAL_ERROR "Check if you have any packages installed for the selected OpenGL library.")
endif()

target_include_directories(CEGUIOpenGLRenderer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIOpenGLRenderer
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIOpenGLRenderer
    PRIVATE
        CEGUIBase
)

if(WIN32)
    target_link_libraries(CEGUIOpenGLRenderer
        PRIVATE
            opengl32
    )
endif()

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIOpenGLRenderer
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIOPENGLRENDERER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIOPENGLRENDERER_IMPORTS>
)

install_target(CEGUIOpenGLRenderer)