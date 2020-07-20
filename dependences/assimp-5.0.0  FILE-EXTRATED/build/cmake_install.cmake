# Install script for directory: C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/assimp-5.0.0

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/assimp-5.0" TYPE FILE FILES
    "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimp-config.cmake"
    "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimp-config-version.cmake"
    "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimpTargets.cmake"
    "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimpTargets-debug.cmake"
    "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimpTargets-release.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.0.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/assimp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/contrib/zlib/cmake_install.cmake")
  include("C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/contrib/cmake_install.cmake")
  include("C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/code/cmake_install.cmake")
  include("C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/tools/assimp_cmd/cmake_install.cmake")
  include("C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Icaro/source/repos/Tugevus/dependences/assimp-5.0.0  FILE-EXTRATED/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
