# Add compile definitions for static library builds
add_compile_definitions(
	$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:CEGUI_STATIC>
	$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:CEGUI_BUILD_STATIC_FACTORY_MODULE>
)

# Required definitions
set(OPENGL_LIBRARY "GLAD" CACHE STRING "OpenGL Function Loader (GLAD, EPOXY, GLEW)") # Default to GLAD, but allow user to override
set_property(CACHE OPENGL_LIBRARY PROPERTY STRINGS "GLAD" "EPOXY" "GLEW")
if(OPENGL_LIBRARY STREQUAL "GLAD")
    add_compile_definitions(CEGUI_USE_GLAD)
elseif(OPENGL_LIBRARY STREQUAL "EPOXY")
    add_compile_definitions(CEGUI_USE_EPOXY)
elseif(OPENGL_LIBRARY STREQUAL "GLEW")
    add_compile_definitions(CEGUI_USE_GLEW)
else()
    message(FATAL_ERROR "Invalid OPENGL_LIBRARY='${OPENGL_LIBRARY}'. Expected GLAD, EPOXY, or GLEW.")
endif()

#//////////////////////////////////////////////////////////////////////////
#// CEGUI allows 3 different string class configurations
#//////////////////////////////////////////////////////////////////////////
set(CEGUI_STRING_CLASS "CEGUI_STRING_CLASS_STD" CACHE STRING "CEGUI string class selection (CEGUI_STRING_CLASS_UNICODE, CEGUI_STRING_CLASS_STD, CEGUI_STRING_CLASS_STD_AO)")
set_property(CACHE CEGUI_STRING_CLASS PROPERTY STRINGS
    "CEGUI_STRING_CLASS_UNICODE" # Inbuilt Unicode (utf8 and utf32 aware)
    "CEGUI_STRING_CLASS_STD"     # plain std::string without allocators (makes pass by reference easy)
    "CEGUI_STRING_CLASS_STD_AO"  # std::basic_string allocated according to allocator config
)
if(CEGUI_STRING_CLASS STREQUAL "CEGUI_STRING_CLASS_UNICODE")
    add_compile_definitions(CEGUI_STRING_CLASS_UNICODE)
elseif(CEGUI_STRING_CLASS STREQUAL "CEGUI_STRING_CLASS_STD")
    add_compile_definitions(CEGUI_STRING_CLASS_STD)
elseif(CEGUI_STRING_CLASS STREQUAL "CEGUI_STRING_CLASS_STD_AO")
    add_compile_definitions(CEGUI_STRING_CLASS_STD_AO)
else()
    message(FATAL_ERROR "Invalid CEGUI_STRING_CLASS: ${CEGUI_STRING_CLASS}")
endif()

# Required modules

add_subdirectory("CEGUIBase")
add_subdirectory("CEGUICommonDialogs")
add_subdirectory("CEGUICoreWindowRendererSet")

# Optional modules
# Renderers
if(BUILD_RENDERER_OPENGL)
	add_subdirectory("CEGUIOpenGLRenderer")
endif()
if(BUILD_RENDERER_D3D9)
	add_subdirectory("CEGUIDirect3D9Renderer")
endif()
if(BUILD_RENDERER_D3D10)
	add_subdirectory("CEGUIDirect3D10Renderer")
endif()
if(BUILD_RENDERER_D3D11)
	add_subdirectory("CEGUIDirect3D11Renderer")
endif()
if(BUILD_RENDERER_NULL)
	add_subdirectory("CEGUINullRenderer")
endif()

# Scripting
if(BUILD_SCRIPT_LUA)
	add_subdirectory("CEGUILuaScriptModule")
endif()

# Image codecs
if(BUILD_IMAGE_DEVIL)
	add_subdirectory("CEGUIDevILImageCodec")
endif()
if(BUILD_IMAGE_FREEIMAGE)
	add_subdirectory("CEGUIFreeImageImageCodec")
endif()
if(BUILD_IMAGE_STB)
	add_subdirectory("CEGUISTBImageCodec")
endif()
if(BUILD_IMAGE_TGA)
	add_subdirectory("CEGUITGAImageCodec")
endif()

# XML codecs
if(BUILD_XML_EXPAT)
	add_subdirectory("CEGUIExpatParser")
endif()
if(BUILD_XML_RAPIDXML)
	add_subdirectory("CEGUIRapidXMLParser")
endif()
if(BUILD_XML_TINYXML)
	add_subdirectory("CEGUITinyXMLParser")
endif()
if(BUILD_XML_XERCES)
	add_subdirectory("CEGUIXercesParser")
endif()
