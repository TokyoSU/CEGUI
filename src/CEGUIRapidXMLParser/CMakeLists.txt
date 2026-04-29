file(GLOB_RECURSE FILE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE FILE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# Add library — type is controlled by BUILD_SHARED_LIBS (ON = DLL, OFF = static)
add_library(CEGUIRapidXMLParser ${FILE_SOURCES} ${FILE_HEADERS})

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")
target_include_directories(CEGUIRapidXMLParser PRIVATE ${RAPIDXML_INCLUDE_DIRS})

target_include_directories(CEGUIRapidXMLParser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(CEGUIRapidXMLParser
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/CEGUIBase>
)

target_link_libraries(CEGUIRapidXMLParser
    PRIVATE
        CEGUIBase
)

# Export macro for DLL visibility (important on Windows)
target_compile_definitions(CEGUIRapidXMLParser
    PRIVATE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIRAPIDXMLPARSER_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:CEGUIRAPIDXMLPARSER_IMPORTS>
)

install_target(CEGUIRapidXMLParser)