set(CEGUI_NATIVE_SOURCES
    src/CEGUIC.cpp
    src/CEGUIWidgets.cpp
    src/CEGUIDirect3D11.cpp
)

set(CEGUI_NATIVE_HEADERS
    include/CEGUI/CEGUIC.h
    include/CEGUI/CEGUIWidgets.h
)

# The binding is always a DLL/shared object because it is intended to provide a
# stable C ABI for P/Invoke and other foreign-function interfaces. The CEGUI
# libraries it links against still follow BUILD_SHARED_LIBS.
add_library(CEGUI.Native SHARED
    ${CEGUI_NATIVE_SOURCES}
    ${CEGUI_NATIVE_HEADERS}
)

add_library(CEGUI::Native ALIAS CEGUI.Native)

target_compile_features(CEGUI.Native PRIVATE cxx_std_20)

target_include_directories(CEGUI.Native
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/CEGUIBase
)

target_link_libraries(CEGUI.Native
    PRIVATE
        CEGUIBase
)

target_compile_definitions(CEGUI.Native
    PRIVATE
        CEGUI_NATIVE_EXPORTS
)

if(WIN32 AND BUILD_RENDERER_D3D11)
    target_link_libraries(CEGUI.Native PRIVATE CEGUIDirect3D11Renderer d3d11)
    target_include_directories(CEGUI.Native PRIVATE ${CMAKE_SOURCE_DIR}/src/CEGUIDirect3D11Renderer)
    target_compile_definitions(CEGUI.Native PRIVATE CEGUI_NATIVE_HAS_D3D11)
endif()

# Static CEGUIBase references one statically selected parser and image codec.
# Resolve those factory symbols in the final C binding DLL without linking every
# factory module (which would introduce duplicate createParser/createImageCodec
# symbols).
if(NOT BUILD_SHARED_LIBS)
    if(BUILD_XML_RAPIDXML)
        target_link_libraries(CEGUI.Native PRIVATE CEGUIRapidXMLParser)
    elseif(BUILD_XML_TINYXML)
        target_link_libraries(CEGUI.Native PRIVATE CEGUITinyXMLParser)
    elseif(BUILD_XML_EXPAT)
        target_link_libraries(CEGUI.Native PRIVATE CEGUIExpatParser)
    elseif(BUILD_XML_XERCES)
        target_link_libraries(CEGUI.Native PRIVATE CEGUIXercesParser)
    else()
        message(FATAL_ERROR "BUILD_C_BINDING with static CEGUI requires one XML parser module.")
    endif()

    if(BUILD_IMAGE_FREEIMAGE)
        target_link_libraries(CEGUI.Native PRIVATE CEGUIFreeImageImageCodec)
    elseif(BUILD_IMAGE_STB)
        target_link_libraries(CEGUI.Native PRIVATE CEGUISTBImageCodec)
    elseif(BUILD_IMAGE_TGA)
        target_link_libraries(CEGUI.Native PRIVATE CEGUITGAImageCodec)
    elseif(BUILD_IMAGE_DEVIL)
        target_link_libraries(CEGUI.Native PRIVATE CEGUIDevILImageCodec)
    else()
        message(FATAL_ERROR "BUILD_C_BINDING with static CEGUI requires one image codec module.")
    endif()
endif()

if(MSVC)
    target_compile_options(CEGUI.Native PRIVATE /W4 /permissive-)
else()
    target_compile_options(CEGUI.Native PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CEGUI_NATIVE_ARCH x64)
else()
    set(CEGUI_NATIVE_ARCH Win32)
endif()

install(TARGETS CEGUI.Native
    RUNTIME DESTINATION "bin/${CEGUI_NATIVE_ARCH}/$<CONFIG>"
    LIBRARY DESTINATION "lib/${CEGUI_NATIVE_ARCH}/$<CONFIG>"
    ARCHIVE DESTINATION "lib/${CEGUI_NATIVE_ARCH}/$<CONFIG>"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/CEGUI"
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
