if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
        "No se permite compilar directamente en el directorio fuente.\n"
    )
endif()

cmake_minimum_required(VERSION 3.16)

project(4ChessViewer
    VERSION 1.0.0
    DESCRIPTION "Visor para ajedrez de cuatro personas FFA"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(VERSION_wxW 3.2.4)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

find_package(wxWidgets ${VERSION_wxW} REQUIRED COMPONENTS core base richtext)

if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "No se encontró wxWidgets ${VERSION_wxW}.")
endif()

include(${wxWidgets_USE_FILE})

set(SOURCES
    src/ChessBook.cpp
    src/ChessBoard.cpp
    src/ChessNotation.cpp
    src/ajedrez4App.cpp
    src/ChessFiles.cpp
    src/ChessFrame.cpp
)

set(HEADERS
    src/Common.h
    src/CBTurn.h
    src/ChessNotation.h
    src/ChessParameters.h
    src/CBPagina.h
    src/CBJugador.h
    src/ChessFiles.h
    src/ChessFrame.h
    src/ChessBoard.h
    src/ChessStyleBoard.h
    src/ChessBook.h

)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${wxWidgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${wxWidgets_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${wxWidgets_CXX_FLAGS}
)

include(GNUInstallDirs)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "4chessviewer"
)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES data/4ChessViewer.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps
)

install(FILES data/4ChessViewer.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

install(FILES data/4ChessViewer.appdata.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/appdata
)
