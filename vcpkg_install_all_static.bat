@echo off
vcpkg install freetype[core,zlib,error-strings]:x64-windows-static
vcpkg install libiconv:x64-windows-static
vcpkg install pcre:x64-windows-static
vcpkg install glew:x64-windows-static
vcpkg install glad[core,extensions,gl-api-latest,loader]:x64-windows-static
vcpkg install glm:x64-windows-static
vcpkg install freeimage:x64-windows-static
vcpkg install devil[core,jasper,lcms,libjpeg,libpng,openexr,tiff]:x64-windows-static
vcpkg install stb:x64-windows-static
vcpkg install dxsdk-d3dx:x64-windows-static
vcpkg install effects11:x64-windows-static
vcpkg install expat:x64-windows-static
vcpkg install tinyxml:x64-windows-static
vcpkg install xerces-c:x64-windows-static
vcpkg install rapidxml:x64-windows-static
vcpkg install lua:x64-windows-static
pause