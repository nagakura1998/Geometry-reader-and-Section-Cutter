# Install script for directory: C:/Users/trong/Downloads/SimpleXlsx r0.40/Source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SimpleXlsx")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/build/Debug/SimpleXlsxd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/build/Release/SimpleXlsx.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/build/MinSizeRel/SimpleXlsx.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/build/RelWithDebInfo/SimpleXlsx.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/PathManager.hpp"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/UTF8Encoder.hpp"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/XMLWriter.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Xlsx" TYPE FILE FILES
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/Chart.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/Chartsheet.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/Drawing.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/SimpleXlsxDef.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/Workbook.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/Worksheet.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Xlsx/XlsxHeaders.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/XLSXColors" TYPE FILE FILES
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/XLSXColors/XLSXColorLib.h"
    "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/XLSXColors/clsRGBColorRecord.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Zip" TYPE FILE FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/Zip/zip.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/scratch" TYPE FILE FILES "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/scratch/clsScratchTable.h")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/trong/Downloads/SimpleXlsx r0.40/Source/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
