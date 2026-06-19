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

#//////////////////////////////////////////////////////////////////////////
#// CEGUI allows 3 different string class configurations
#//////////////////////////////////////////////////////////////////////////
set(CEGUI_STRING_CLASS_DEFINE "CEGUI_STRING_CLASS_STD" CACHE STRING "CEGUI string class selection")
set_property(CACHE CEGUI_STRING_CLASS_DEFINE PROPERTY STRINGS
    "CEGUI_STRING_CLASS_UNICODE" # Inbuilt Unicode (utf8 and utf32 aware)
    "CEGUI_STRING_CLASS_STD"     # plain std::string without allocators (makes pass by reference easy)
    "CEGUI_STRING_CLASS_STD_AO"  # std::basic_string allocated according to allocator config
)
if (CEGUI_STRING_CLASS_DEFINE STREQUAL "CEGUI_STRING_CLASS_UNICODE")
    set(CEGUI_STRING_CLASS 1)
elseif (CEGUI_STRING_CLASS_DEFINE STREQUAL "CEGUI_STRING_CLASS_STD")
    set(CEGUI_STRING_CLASS 2)
elseif (CEGUI_STRING_CLASS_DEFINE STREQUAL "CEGUI_STRING_CLASS_STD_AO")
    set(CEGUI_STRING_CLASS 3)
else()
    message(FATAL_ERROR "Invalid CEGUI_STRING_CLASS_DEFINE")
endif()

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIBase
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBASE_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIBASE_IMPORTS>
)

install_target(CEGUIBase)