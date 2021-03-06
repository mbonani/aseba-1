set (ASEBASIM_SRC
	EnkiGlue.cpp
	AsebaGlue.cpp
	DirectAsebaGlue.cpp
	Door.cpp
	robots/e-puck/EPuck.cpp
	robots/e-puck/EPuck-descriptions.c
	robots/thymio2/Thymio2.cpp
	robots/thymio2/Thymio2-natives.cpp
	robots/thymio2/Thymio2-descriptions.c
)

add_library(asebasim STATIC ${ASEBASIM_SRC})
set_target_properties(asebasim PROPERTIES VERSION ${LIB_VERSION_STRING}
										SOVERSION ${LIB_VERSION_MAJOR})


target_link_libraries(asebasim PUBLIC aseba_conf enki QtZeroConf)

if (Qt5Widgets_FOUND AND Qt5OpenGL_FOUND AND Qt5Xml_FOUND)
	find_package(OpenGL REQUIRED)
	if (Qt5DBus_FOUND AND NOT WIN32)
		# force disable D-Bus on Windows, because it generates compilation
		# errors in interaction with QtOpenGL includes
		set(EXTRA_LIBS ${EXTRA_LIBS} Qt5::DBus)
		add_definitions("-DHAVE_DBUS")
	endif ()
	set(CMAKE_AUTOMOC ON)

	file(GLOB TRANSLATIONS_SOURCES "asebaplayground*.ts")
	set_source_files_properties(${TRANSLATIONS_SOURCES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}")
	qt5_add_translation(TRANSLATIONS ${TRANSLATIONS_SOURCES} )
	qt5_add_resources(resfiles asebaplayground.qrc)

	set(playground_SRCS
		DashelAsebaGlue.cpp
		PlaygroundViewer.cpp
		PlaygroundDBusAdaptors.cpp
		playground.cpp
	)

	if(APPLE)
		SET(PLAYGROUND_EXECUTABLE_NAME AsebaPlayground)
	else()
		SET(PLAYGROUND_EXECUTABLE_NAME asebaplayground)
	endif()


	add_executable(${PLAYGROUND_EXECUTABLE_NAME} WIN32 MACOSX_BUNDLE  ${playground_SRCS} ${playground_MOCS} ${resfiles})

    target_link_libraries(${PLAYGROUND_EXECUTABLE_NAME}
        asebasim
        asebacommon
        asebavmbuffer
        asebavm
        asebaqtabout
        enkiviewer
        QtZeroConf
        quazip_static
        Qt5::Xml Qt5::Svg Qt5::Network ${EXTRA_LIBS}
        ${Qt5Core_QTMAIN_LIBRARIES})

	install_qt_app(${PLAYGROUND_EXECUTABLE_NAME})
	codesign(${PLAYGROUND_EXECUTABLE_NAME})

	if(APPLE)
		set(MACOSX_BUNDLE_BUNDLE_VERSION ${ASEBA_VERSION})
		set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${ASEBA_VERSION})
		set(MACOSX_BUNDLE_LONG_VERSION_STRING "${ASEBA_VERSION}-${GIT_REVISION}")
		set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.mobsya.AsebaPlayground")
		set(MACOSX_BUNDLE_ICON_FILE asebaplayground.icns)
		set(playground_icon ${PROJECT_SOURCE_DIR}/menu/osx/asebaplayground.icns)
		set_source_files_properties(${playground_icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
		target_sources(${PLAYGROUND_EXECUTABLE_NAME} PRIVATE ${playground_icon})
	endif()

endif ()
