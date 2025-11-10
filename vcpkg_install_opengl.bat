@echo off
vcpkg install glew
vcpkg install glad[core,extensions,gl-api-latest,loader]
vcpkg install glm