@echo off
vcpkg install freetype[core,zlib,error-strings]:x64-windows
vcpkg install libiconv:x64-windows
vcpkg install pcre:x64-windows
vcpkg install glew:x64-windows
vcpkg install glad[core,extensions,gl-api-latest,loader,wgl]:x64-windows
vcpkg install glm:x64-windows
vcpkg install freeimage:x64-windows
vcpkg install devil[core,jasper,lcms,libjpeg,libpng,openexr,tiff]:x64-windows
vcpkg install stb:x64-windows
vcpkg install dxsdk-d3dx:x64-windows
vcpkg install effects11:x64-windows
vcpkg install expat:x64-windows
vcpkg install tinyxml:x64-windows
vcpkg install xerces-c:x64-windows
vcpkg install rapidxml:x64-windows
vcpkg install lua:x64-windows