@echo off
vcpkg install glew:x64-windows-static
vcpkg install glad[core,extensions,gl-api-latest,loader]:x64-windows-static
vcpkg install glm:x64-windows-static