project(cheetah-app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick)

qt_policy(
    SET QTP0001 NEW
)
qt_policy(
    SET QTP0004 NEW
)

include_directories(qml)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(app-cheetah-app
    cheetah-app.cpp
    ModelDownloader.cpp
)

qt_add_qml_module(app-cheetah-app
    URI cheetah-app
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml/
    QML_FILES qml/cheetah-app-window.qml
    DEPENDENCIES QtQuick
    SOURCES include/ConversationAnalyzer.h
    SOURCES include/PromptGenerator.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app-cheetah-app PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app=cheetah-app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app-cheetah-app
    PRIVATE Qt6::Quick Qt6::Core LibWhisper LibOpenAI
)

# Specify the include directories
target_include_directories(app-cheetah-app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GNUInstallDirs)
install(TARGETS app-cheetah-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
