file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIBase ${FILE_SOURCES} ${FILE_HEADERS})
find_package(Freetype CONFIG REQUIRED)
find_package(unofficial-pcre CONFIG REQUIRED)

# Include directories for this library
target_include_directories(CEGUIBase
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link against dependencies
target_link_libraries(CEGUIBase
    PRIVATE
        Freetype::Freetype
        unofficial::pcre::pcre
)

if(WIN32)
    target_link_libraries(CEGUIBase
        PRIVATE
            dbghelp
            winmm
    )
endif()

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIBase
    PRIVATE
        $<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:CEGUI_BUILD_STATIC_FACTORY_MODULE>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBASE_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBASE_IMPORTS>
)

install_target(CEGUIBase)