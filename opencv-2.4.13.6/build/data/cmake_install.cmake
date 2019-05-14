# Install script for directory: /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data

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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/haarcascades" TYPE FILE FILES
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalface_alt_tree.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_fullbody.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_smile.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_righteye_2splits.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_upperbody.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_licence_plate_rus_16stages.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_eyepair_small.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalcatface.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_lefteye.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_upperbody.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_rightear.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_lowerbody.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_eyepair_big.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_profileface.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalface_alt.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_eye.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalface_alt2.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_righteye.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_nose.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalcatface_extended.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_frontalface_default.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_mouth.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_lefteye_2splits.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_mcs_leftear.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/haarcascades/haarcascade_russian_plate_number.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/lbpcascades" TYPE FILE FILES
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/lbpcascades/lbpcascade_silverware.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/lbpcascades/lbpcascade_frontalface.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/lbpcascades/lbpcascade_frontalcatface.xml"
    "/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/data/lbpcascades/lbpcascade_profileface.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")

