file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIDevILImageCodec ${FILE_SOURCES} ${FILE_HEADERS})

find_path(DEVIL_INCLUDE_DIR NAMES IL/il.h)
find_library(DEVIL_LIBRARY NAMES DevIL ILU ILUT)

if(NOT DEVIL_INCLUDE_DIR OR NOT DEVIL_LIBRARY)
    message(FATAL_ERROR "DevIL library not found")
endif()

target_include_directories(CEGUIDevILImageCodec
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIDevILImageCodec
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
        ${DEVIL_INCLUDE_DIR}
)

target_link_libraries(CEGUIDevILImageCodec
    PRIVATE
        CEGUIBase
        ${DEVIL_LIBRARY}
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIDevILImageCodec
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDEVILIMAGECODEC_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIDEVILIMAGECODEC_IMPORTS>
)

install_target(CEGUIDevILImageCodec)