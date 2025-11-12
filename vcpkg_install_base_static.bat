@echo off
vcpkg install freetype[core,zlib,error-strings]:x64-windows-static
vcpkg install libiconv:x64-windows-static
vcpkg install pcre:x64-windows-static