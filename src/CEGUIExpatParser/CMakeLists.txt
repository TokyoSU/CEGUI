file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIExpatParser ${FILE_SOURCES} ${FILE_HEADERS})

find_package(expat CONFIG REQUIRED)

target_include_directories(CEGUIExpatParser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIExpatParser
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIExpatParser
    PRIVATE
        CEGUIBase
        expat::expat
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIExpatParser
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIEXPATPARSER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIEXPATPARSER_IMPORTS>
)

install_target(CEGUIExpatParser)