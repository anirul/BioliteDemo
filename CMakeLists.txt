cmake_minimum_required(VERSION 3.0)

project(BioliteDemo)

include("cmake/FindCXXFeatures.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_COMPILER_FLAGS}")

if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD c++11)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY libc++)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    set(CMAKE_LIBRARY_PREFIX /opt/local /usr/local)

    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(OPENAL_LIBRARY OpenAL)
    find_library(IOKIT_LIBRARY IOKit)

    # Locate all resourses
    file(GLOB COPY_MEDIA_RESOURCES "${PROJECT_SOURCE_DIR}/media/*")
    set_source_files_properties(
        ${COPY_MEDIA_RESOURCES}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION
        Resources/media
    )

    set(ICON_FILE_PATH
        "${PROJECT_SOURCE_DIR}/icon.icns"
    )
    set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} - Version ${VERSION}")
    set(MACOSX_BUNDLE_VERSION ${VERSION})
    set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.calodox.${PROJECT_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_RESOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources"
    )

    set_source_files_properties(
        ${ICON_FILE_PATH}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION
        Resources
    )

    set(BUNDLE_RESOURCES
        "${ICON_FILE_PATH};${COPY_MEDIA_RESOURCES}"
    )
endif(APPLE)

if(WIN32 OR APPLE)
    include_directories(
        /opt/local/include
        /usr/local/include
        /usr/include
        ${PROJECT_SOURCE_DIR}/sources
        ${PROJECT_SOURCE_DIR}/thirdparty/irrlicht-1.8.1/include
        ${PROJECT_SOURCE_DIR}/Thirdparty/Gorilla/include
    )
endif(WIN32 OR APPLE)

add_executable(BioliteDemo MACOSX_BUNDLE ${BUNDLE_RESOURCES}
    sources/CGUIColorPickDialog.cpp
    sources/CGUIColorPickDialog.h
    sources/CSceneNodeAnimatorCameraArcball.cpp
    sources/CSceneNodeAnimatorCameraArcball.h
    sources/IGUIColorPickDialog.h
    sources/ISceneNodeAnimatorCameraArcball.h
    sources/Makefile
    sources/ai.cpp
    sources/ai.h
    sources/game.cpp
    sources/game.h
    sources/game_logic.cpp
    sources/game_logic.h
    sources/game_math.cpp
    sources/game_math.h
    sources/game_state_xml_game.cpp
    sources/game_state_xml_game.h
    sources/game_state_xml_gui.cpp
    sources/game_state_xml_gui.h
    sources/game_state_xml_still.cpp
    sources/game_state_xml_still.h
    sources/main.cpp
    sources/main.h
    sources/media_collection.cpp
    sources/media_collection.h
    sources/parameter_set.cpp
    sources/parameter_set.h
    sources/persistant_set.cpp
    sources/persistant_set.h
    sources/planet.cpp
    sources/planet.h
    sources/plant.cpp
    sources/plant.h
    sources/resolution_chooser.cpp
    sources/resolution_chooser.h
    sources/sound.cpp
    sources/sound.h
    sources/win.cpp
    sources/win.h
    sources/world.cpp
    sources/world.h
    sources/xml_button.cpp
    sources/xml_button.h
    sources/xml_element.h
    sources/xml_game_logic.cpp
    sources/xml_game_logic.h
    sources/xml_image.cpp
    sources/xml_image.h
    sources/xml_irr_button.cpp
    sources/xml_irr_button.h
    sources/xml_irr_combo.cpp
    sources/xml_irr_combo.h
    sources/xml_irr_label.cpp
    sources/xml_irr_label.h
    sources/xml_irr_slider.cpp
    sources/xml_irr_slider.h
    sources/xml_irr_text.cpp
    sources/xml_irr_text.h
    sources/xml_menu.cpp
    sources/xml_menu.h
    sources/xml_misc.cpp
    sources/xml_misc.h
    sources/xml_planet.cpp
    sources/xml_planet.h
    sources/xml_set_value.cpp
    sources/xml_set_value.h
    sources/xml_toggle_button.cpp
    sources/xml_toggle_button.h
)

if(APPLE)
    set_target_properties(
        BioliteDemo
        PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        "${PROJECT_SOURCE_DIR}/MacOSXBundleInfo.plist.in"
    )

    find_library(IRRLICHT_LIBRARY
        libIrrlicht.a
        ${PROJECT_SOURCE_DIR}/thirdparty/irrlicht-1.8.1/lib/MacOSX
    )

    find_library(Z_LIBRARY
        libz.a
        /opt/local/lib
    )

    find_library(GORILLA_LIBRARY
        libgorilla.a
        ${PROJECT_SOURCE_DIR}/thirdparty/Gorilla/lib/MacOSX
    )

    target_link_libraries(BioliteDemo
		${COCOA_LIBRARY}
		${OPENGL_LIBRARY}
		${OPENAL_LIBRARY}
		${IOKIT_LIBRARY}
        ${Z_LIBRARY}
        ${IRRLICHT_LIBRARY}
        ${GORILLA_LIBRARY}
    )
endif(APPLE)

if(WIN32)
    find_library(IRRLICHT_LIBRARY
        Irrlicht
        ${PROJECT_SOURCE_DIR}/thirdparty/irrlicht-1.8.1/lib/Win64-visualStudio
    )

    find_library(GORILLA_LIBRARY
        gorilla
        ${PROJECT_SOURCE_DIR}/thirdparty/Gorilla/lib/win32
    )

    target_link_libraries(BioliteDemo
        ${IRRLICHT_LIBRARY}
        ${GORILLA_LIBRARY}
    )
endif(WIN32)
