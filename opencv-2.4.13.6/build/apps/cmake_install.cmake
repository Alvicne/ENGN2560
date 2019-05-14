# Install script for directory: /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/apps

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/users/guest438/scratch/ENGN2560/CV2")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/apps/haartraining/cmake_install.cmake")
  INCLUDE("/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/apps/traincascade/cmake_install.cmake")
  INCLUDE("/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/apps/annotation/cmake_install.cmake")
  INCLUDE("/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/apps/visualisation/cmake_install.cmake")
  INCLUDE("/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/apps/version/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

