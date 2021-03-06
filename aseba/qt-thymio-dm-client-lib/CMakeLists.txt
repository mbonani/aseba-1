set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt5 COMPONENTS Widgets Qml Quick QuickWidgets REQUIRED)

add_library(qt-thymio-dm-client-lib STATIC
    thymiodevicemanagerclient.cpp
    thymiodevicemanagerclient.h
    thymiodevicemanagerclientendpoint.cpp
    thymiodevicemanagerclientendpoint.h
    thymiodevicesmodel.cpp
    thymiodevicesmodel.h
    thymionode.cpp
    thymionode.h
    thymio-api.h
    qflatbuffers.h
    request.h
    request.cpp
)

add_library(qt-thymio-dm-client-lib-qml OBJECT
    qml/quickrequest.h
    qml/quickrequest.cpp
)


target_link_libraries(qt-thymio-dm-client-lib  PUBLIC
    Qt5::Core
    Qt5::Network
    QtZeroConf
    thymio-flatbuffers
    range-v3
)

target_link_libraries(qt-thymio-dm-client-lib-qml  PUBLIC
   qt-thymio-dm-client-lib
   Qt5::Quick
)

target_include_directories(qt-thymio-dm-client-lib PUBLIC .)
