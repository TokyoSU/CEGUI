@echo off
vcpkg install freetype[core,zlib,error-strings]:x86-windows-static
vcpkg install libiconv:x86-windows-static
vcpkg install pcre:x86-windows-static
vcpkg install glew:x86-windows-static
vcpkg install glad[core,extensions,gl-api-latest,loader,wgl]:x86-windows-static
vcpkg install glm:x86-windows-static
vcpkg install freeimage:x86-windows-static
vcpkg install devil[core,jasper,lcms,libjpeg,libpng,openexr,tiff]:x86-windows-static
vcpkg install stb:x86-windows-static
vcpkg install dxsdk-d3dx:x86-windows-static
vcpkg install effects11:x86-windows-static
vcpkg install expat:x86-windows-static
vcpkg install tinyxml:x86-windows-static
vcpkg install xerces-c:x86-windows-static
vcpkg install rapidxml:x86-windows-static
vcpkg install lua:x86-windows-static