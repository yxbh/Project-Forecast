#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T11:14:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectRainPacker
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/qtpropertybrowser/qtpropertybrowser.pri)


#
# Organisation, product & copyright details.
#
QMAKE_TARGET_COMPANY = Infinity Box Studio
QMAKE_TARGET_PRODUCT = Overwatch Player Log
QMAKE_TARGET_DESCRIPTION = Overwatch Player Log is a simple utility application that allows you to record your favourite (or not!) players that you have met in Overwatch.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2016 Benjamin Huang.
#

#
# Configure release & debug build directories.
#   - DESTDIR:      binary output
#   - OBJECTS_DIR:  object files output
#   - MOC_DIR:      MOC generated source output
#   - RCC_DIR:      resource file(s) output
#   - UI_DIR:       UI source output
#
BUILDDIRWIN32DBG = ../Build/ProjectRainPacker-Win32-Debug
BUILDDIRWIN32REL = ../Build/ProjectRainPacker-Win32-Release
BUILDDIRUNIXDBG  = ../Build/ProjectRainPacker-Unix-Debug
BUILDDIRUNIXREL  = ../Build/ProjectRainPacker-Unix-Release
BUILDDIROSXDBG   = ../Build/ProjectRainPacker-OSX-Debug
BUILDDIROSXREL   = ../Build/ProjectRainPacker-OSX-Release
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


SOURCES += \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format.cc \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ostream.cc \
    ../../KEngine/src/Common/Color.cpp \
    ../../KEngine/src/Common/Time.cpp \
    Controllers/MainWindow.cpp \
    Controllers/KengineResourceTableView.cpp \
    Controllers/ManifestGenerationDialog.cpp \
    Entities/Resources.cpp \
    Logic/UrlResourceTypeDetector.cpp \
    Logic/Constants.cpp \
    Logic/Context.cpp \
    Logic/ResourceManufacturer.cpp \
    Logic/ManifestEditor.cpp \
    Models/Models.cpp \
    main.cpp

HEADERS  += \
    ../../KEngine/include/KEngine/Common/Libs/atomicops.h \
    ../../KEngine/include/KEngine/Common/Libs/concurrentqueue.h \
    ../../KEngine/include/KEngine/Common/Libs/json.hpp \
    ../../KEngine/include/KEngine/Common/Libs/readerwriterqueue.h \
    ../../KEngine/include/KEngine/Common/Libs/SRDelegate.hpp \
    ../../KEngine/include/KEngine/Common/Color.hpp \
    ../../KEngine/include/KEngine/Common/Delegate.hpp \
    ../../KEngine/include/KEngine/Common/Json.hpp \
    ../../KEngine/include/KEngine/Common/macros.hpp \
    ../../KEngine/include/KEngine/Common/Point2D.hpp \
    ../../KEngine/include/KEngine/Common/Queues.hpp \
    ../../KEngine/include/KEngine/Common/ScopeFunc.hpp \
    ../../KEngine/include/KEngine/Common/String.hpp \
    ../../KEngine/include/KEngine/Common/ThreadPool.hpp \
    ../../KEngine/include/KEngine/Common/Time.hpp \
    ../../KEngine/include/KEngine/Interfaces/IResource.hpp \
    ../../KEngine/include/KEngine/Log/spdlog/details/async_log_helper.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/async_logger_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/file_helper.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/log_msg.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/logger_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/mpmc_bounded_q.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/null_mutex.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/os.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/pattern_formatter_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/registry.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/spdlog_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ostream.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/printf.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/fmt.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/ostr.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/android_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/ansicolor_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/base_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/dist_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/file_sinks.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/msvc_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/null_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/ostream_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/stdout_sinks.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/syslog_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/wincolor_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/async_logger.h \
    ../../KEngine/include/KEngine/Log/spdlog/common.h \
    ../../KEngine/include/KEngine/Log/spdlog/formatter.h \
    ../../KEngine/include/KEngine/Log/spdlog/logger.h \
    ../../KEngine/include/KEngine/Log/spdlog/spdlog.h \
    ../../KEngine/include/KEngine/Log/spdlog/tweakme.h \
    ../../KEngine/include/KEngine/Log/Log.hpp \
    Controllers/MainWindow.hpp \
    Controllers/KengineResourceTableView.hpp \
    Controllers/ManifestGenerationDialog.hpp \
    Entities/Resources.hpp \
    Logic/Constants.hpp \
    Logic/UrlResourceTypeDetector.hpp \
    Logic/Context.hpp \
    Logic/ResourceManufacturer.hpp \
    Logic/ManifestEditor.hpp \
    Models/Models.hpp

FORMS    += \
    UI/MainWindow.ui \
    UI/ManifestGenerationDialog.ui

INCLUDEPATH += \
    $$PWD \
    ../../KEngine/include \
    ../../KEngine/include/KEngine/Log # for spdlog
