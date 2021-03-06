#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME BrusselsGame)

project(${APP_NAME})

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()
include_directories(D:/cricket/inc)
link_directories(D:/cricket/lib/android/ndk/${ANDROID_ABI}/debug)
link_libraries(D:/cricket/lib/android/ndk/${ANDROID_ABI}/debug/libck.a)

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
     Classes/Action/ACTION.cpp
     Classes/Action/ActionConvey.cpp
     Classes/Button/ButtonLayer.cpp
     Classes/Camera/CameraOBJ.cpp
     Classes/ClearLayer/ClearLayer.cpp
     Classes/HP/HPMng.cpp
     Classes/menu/setting/SettingLayer.cpp
     Classes/menu/MenuLayer.cpp
     Classes/Obj/Obstacles/BlackLadybug/BlackLadybug.cpp
     Classes/Obj/Obstacles/Bomb/Bomb.cpp
     Classes/Obj/Obstacles/Buds/Buds.cpp
     Classes/Obj/Obstacles/CheckPoint/CheckPoint.cpp
     Classes/Obj/Obstacles/AddObstacles.cpp
     Classes/Obj/Obstacles/Obstacles.cpp
     Classes/Obj/Player/Player.cpp
     Classes/Obj/Obj.cpp
     Classes/RankCheck/RankCheck.cpp
     Classes/Scene/BaseScene.cpp
     Classes/Scene/GameScene.cpp
     Classes/Scene/StageSelectScene.cpp
     Classes/Scene/TitleScene.cpp
     Classes/Split/Split.cpp
     Classes/stage/StageLayer.cpp
     Classes/AppDelegate.cpp
     )
#file(GLOB_RECURSE GAME_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS "Classes/*.cpp")

list(APPEND GAME_HEADER

        Classes/Action/ACTION.h
        Classes/Action/ActionConvey.h
        Classes/Button/ButtonLayer.h
        Classes/Camera/CameraOBJ.h
        Classes/ClearLayer/ClearLayer.h
        Classes/HP/HPMng.h
        Classes/menu/setting/SettingLayer.h
        Classes/menu/MenuLayer.h
        Classes/Obj/Obstacles/BlackLadybug/BlackLadybug.h
        Classes/Obj/Obstacles/Bomb/Bomb.h
        Classes/Obj/Obstacles/Buds/Buds.h
        Classes/Obj/Obstacles/CheckPoint/CheckPoint.h
        Classes/Obj/Obstacles/AddObstacles.h
        Classes/Obj/Obstacles/Obstacles.h
        Classes/Obj/Player/Player.h
        Classes/Obj/Obj.h
        Classes/Obj/OBJ_COLLAR.h
        Classes/RankCheck/RankCheck.h
        Classes/Scene/BaseScene.h
        Classes/Scene/GameScene.h
        Classes/Scene/StageSelectScene.h
        Classes/Scene/TitleScene.h
        Classes/Split/Split.h
        Classes/stage/StageLayer.h
        Classes/AppDelegate.h
     )
#file(GLOB_RECURSE GAME_HEADER RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS "Classes/*.h")

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
