find_program(DOTNET_EXECUTABLE dotnet)
if(NOT DOTNET_EXECUTABLE)
    message(FATAL_ERROR "BUILD_CSHARP_BINDING requires the .NET SDK (dotnet) on PATH.")
endif()

set(CEGUI_MANAGED_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/CEGUI.Managed.csproj")

add_custom_target(CEGUI.Managed ALL
    COMMAND "${DOTNET_EXECUTABLE}" build "${CEGUI_MANAGED_PROJECT}"
            --configuration "$<IF:$<CONFIG:Debug>,Debug,Release>"
            --property:CEGUINativeLibraryPath="$<TARGET_FILE:CEGUI.Native>"
    DEPENDS CEGUI.Native
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Building CEGUI.Managed"
    VERBATIM
)
