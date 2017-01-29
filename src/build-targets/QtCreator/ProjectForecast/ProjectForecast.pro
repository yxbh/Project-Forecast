TARGET = ProjectForecast
TEMPLATE = app

CONFIG += c++14

DEFINES += RUN_UNIT_TESTS

#
# Configure release & debug build directories.
#   - DESTDIR:      binary output
#   - OBJECTS_DIR:  object files output
#   - MOC_DIR:      MOC generated source output
#   - RCC_DIR:      resource file(s) output
#   - UI_DIR:       UI source output
#
BUILDDIRWIN32DBG = Build/ProjectForecast-Win32-Debug
BUILDDIRWIN32REL = Build/ProjectForecast-Win32-Release
BUILDDIRUNIXDBG  = Build/ProjectForecast-Unix-Debug
BUILDDIRUNIXREL  = Build/ProjectForecast-Unix-Release
BUILDDIROSXDBG   = Build/ProjectForecast-OSX-Debug
BUILDDIROSXREL   = Build/ProjectForecast-OSX-Release
CONFIG(debug, debug|release) { # Debug build dirs
    win32:      DESTDIR     = $${BUILDDIRWIN32DBG}/
    win32:      OBJECTS_DIR = $${BUILDDIRWIN32DBG}/obj
    win32:      MOC_DIR     = $${BUILDDIRWIN32DBG}/moc
    win32:      RCC_DIR     = $${BUILDDIRWIN32DBG}/rsc
    win32:      UI_DIR      = $${BUILDDIRWIN32DBG}/ui
    macx:       DESTDIR     = $${BUILDDIROSXDBG}/
    macx:       OBJECTS_DIR = $${BUILDDIROSXDBG}/obj
    macx:       MOC_DIR     = $${BUILDDIROSXDBG}/moc
    macx:       RCC_DIR     = $${BUILDDIROSXDBG}/rsc
    macx:       UI_DIR      = $${BUILDDIROSXDBG}/ui
    unix!macx:  DESTDIR     = $${BUILDDIRUNIXDBG}/
    unix!macx:  OBJECTS_DIR = $${BUILDDIRUNIXDBG}/obj
    unix!macx:  MOC_DIR     = $${BUILDDIRUNIXDBG}/moc
    unix!macx:  RCC_DIR     = $${BUILDDIRUNIXDBG}/rsc
    unix!macx:  UI_DIR      = $${BUILDDIRUNIXDBG}/ui
}
CONFIG(release, debug|release) { # Release build dirs
    win32:      DESTDIR     = $${BUILDDIRWIN32REL}/
    win32:      OBJECTS_DIR = $${BUILDDIRWIN32REL}/obj
    win32:      MOC_DIR     = $${BUILDDIRWIN32REL}/moc
    win32:      RCC_DIR     = $${BUILDDIRWIN32REL}/rsc
    win32:      UI_DIR      = $${BUILDDIRWIN32REL}/ui
    macx:       DESTDIR     = $${BUILDDIROSXREL}/
    macx:       OBJECTS_DIR = $${BUILDDIROSXREL}/obj
    macx:       MOC_DIR     = $${BUILDDIROSXREL}/moc
    macx:       RCC_DIR     = $${BUILDDIROSXREL}/rsc
    macx:       UI_DIR      = $${BUILDDIROSXREL}/ui
    unix!macx:  DESTDIR     = $${BUILDDIRUNIXREL}/
    unix!macx:  OBJECTS_DIR = $${BUILDDIRUNIXREL}/obj
    unix!macx:  MOC_DIR     = $${BUILDDIRUNIXREL}/moc
    unix!macx:  RCC_DIR     = $${BUILDDIRUNIXREL}/rsc
    unix!macx:  UI_DIR      = $${BUILDDIRUNIXREL}/ui
}
#

#
#
#
INCLUDEPATH += \
    ../../../KEngine/include/ \
    ../../../KEngine/include/KEngine/Log/
#

#
# Source files.
#
HEADERS += \
    ../../../KEngine/include/KEngine/Common/Delegate.hpp \
    ../../../KEngine/include/KEngine/Common/macro_common.hpp \
    ../../../KEngine/include/KEngine/Common/Queues.hpp \
    ../../../KEngine/include/KEngine/Common/SRDelegate.hpp \
    ../../../KEngine/include/KEngine/Common/StopWatch.hpp \
    ../../../KEngine/include/KEngine/Common/String.hpp \
    ../../../KEngine/include/KEngine/Common/Time.hpp \
    ../../../KEngine/include/KEngine/Core/EventManager.hpp \
    ../../../KEngine/include/KEngine/Core/EventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Interface/IApp.hpp \
    ../../../KEngine/include/KEngine/Interface/IEvent.hpp \
    ../../../KEngine/include/KEngine/Interface/IEventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Log/Log.hpp \
    ../../../KEngine/include/KEngine/UnitTest/catch.hpp \
    ../../../KEngine/include/KEngine/App.hpp

SOURCES += \
    ../../../KEngine/include/KEngine/Common/Queues.inl \
    ../../../KEngine/include/KEngine/Common/Time.inl \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format.cc \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ostream.cc \
    ../../../KEngine/src/Common/Time.cpp \
    ../../../KEngine/src/Core/EventManagerImpl.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Delegate.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventDelegates.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventManager.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Events.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Time.cpp \
    ../../../KEngine/src/App.cpp \
    ../../../ProjectForecast/main.cpp
#
