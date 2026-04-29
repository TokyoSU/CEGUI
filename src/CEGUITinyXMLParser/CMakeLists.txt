file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUITinyXMLParser ${FILE_SOURCES} ${FILE_HEADERS})

find_package(tinyxml CONFIG REQUIRED)

target_include_directories(CEGUITinyXMLParser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUITinyXMLParser
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUITinyXMLParser
    PRIVATE
        CEGUIBase
        unofficial-tinyxml::unofficial-tinyxml
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUITinyXMLParser
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUITINYXMLPARSER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUITINYXMLPARSER_IMPORTS>
)

install_target(CEGUITinyXMLParser)