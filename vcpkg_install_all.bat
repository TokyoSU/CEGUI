@echo off
vcpkg install freetype[core,zlib,error-strings]
vcpkg install libiconv
vcpkg install pcre
vcpkg install glew
vcpkg install glad[core,extensions,gl-api-latest,loader]
vcpkg install glm
vcpkg install freeimage
vcpkg install devil[core,jasper,lcms,libjpeg,libpng,openexr,tiff]
vcpkg install stb
vcpkg install dxsdk-d3dx
vcpkg install effects11
vcpkg install expat
vcpkg install tinyxml
vcpkg install xerces-c
vcpkg install rapidxml
vcpkg install lua