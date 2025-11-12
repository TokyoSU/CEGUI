@echo off
vcpkg install freetype[core,zlib,error-strings]
vcpkg install libiconv
vcpkg install pcre