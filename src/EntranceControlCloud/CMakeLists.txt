cmake_minimum_required(VERSION 3.14)

project(EntranceControlCloud LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network HttpServer)

add_executable(EntranceControlCloud
  main.cpp
  entranceservercloud.h entranceservercloud.cpp
  orm.h orm.cpp
  httpserver.h httpserver.cpp
  websocketserver.h websocketserver.cpp
  websocketrequest.h websocketrequest.cpp
  websocketresponse.h websocketresponse.cpp
  entrancerecord.h entrancerecord.cpp
  authenticator.h authenticator.cpp
  monitoringsystemuser.h monitoringsystemuser.cpp
  monitoringsystemauthenticator.h monitoringsystemauthenticator.cpp
  entrancehistory.h entrancehistory.cpp
  data/entrance_history.json data/users.json data/rfid_tags.json
)
target_link_libraries(EntranceControlCloud Qt${QT_VERSION_MAJOR}::Core Qt6::Network Qt6::HttpServer)

include(GNUInstallDirs)
install(TARGETS EntranceControlCloud
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
