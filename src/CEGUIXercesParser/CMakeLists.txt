file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIXercesParser ${FILE_SOURCES} ${FILE_HEADERS})

find_package(XercesC CONFIG REQUIRED)

target_include_directories(CEGUIXercesParser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIXercesParser
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIXercesParser
    PRIVATE
        CEGUIBase
        XercesC::XercesC
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIXercesParser
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIXERCESPARSER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIXERCESPARSER_IMPORTS>
)

install_target(CEGUIXercesParser)