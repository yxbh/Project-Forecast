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
BUILDDIRWIN32DBG = Build/OPL-Win32-Debug
BUILDDIRWIN32REL = Build/OPL-Win32-Release
BUILDDIRUNIXDBG  = Build/OPL-Unix-Debug
BUILDDIRUNIXREL  = Build/OPL-Unix-Release
BUILDDIROSXDBG   = Build/OPL-OSX-Debug
BUILDDIROSXREL   = Build/OPL-OSX-Release
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
    ../../../KEngine/include/KEngine/Log/spdlog/details/async_log_helper.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/async_logger_impl.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/file_helper.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/log_msg.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/logger_impl.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/mpmc_bounded_q.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/null_mutex.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/os.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/pattern_formatter_impl.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/registry.h \
    ../../../KEngine/include/KEngine/Log/spdlog/details/spdlog_impl.h \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format.h \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ostream.h \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/printf.h \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/fmt.h \
    ../../../KEngine/include/KEngine/Log/spdlog/fmt/ostr.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/android_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/ansicolor_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/base_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/dist_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/file_sinks.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/msvc_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/null_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/ostream_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/stdout_sinks.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/syslog_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/sinks/wincolor_sink.h \
    ../../../KEngine/include/KEngine/Log/spdlog/async_logger.h \
    ../../../KEngine/include/KEngine/Log/spdlog/common.h \
    ../../../KEngine/include/KEngine/Log/spdlog/formatter.h \
    ../../../KEngine/include/KEngine/Log/spdlog/logger.h \
    ../../../KEngine/include/KEngine/Log/spdlog/spdlog.h \
    ../../../KEngine/include/KEngine/Log/spdlog/tweakme.h \
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
