#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T11:14:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectRainPacker
TEMPLATE = app

#CONFIG += console

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
QMAKE_TARGET_PRODUCT = RainPacker
QMAKE_TARGET_DESCRIPTION = KEngine resource manifest generator.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2019 Benjamin Huang.
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
    ../../KEngine/include/KEngine/Common/Libs/cxxopts.hpp \
    ../../KEngine/include/KEngine/Common/Libs/json.hpp \
    ../../KEngine/include/KEngine/Common/Libs/readerwriterqueue.h \
    ../../KEngine/include/KEngine/Common/Libs/SRDelegate.hpp \
    ../../KEngine/include/KEngine/Common/Color.hpp \
    ../../KEngine/include/KEngine/Common/Delegate.hpp \
    ../../KEngine/include/KEngine/Common/Dimension2D.hpp \
    ../../KEngine/include/KEngine/Common/HeartBeatGenerator.hpp \
    ../../KEngine/include/KEngine/Common/Json.hpp \
    ../../KEngine/include/KEngine/Common/macros.hpp \
    ../../KEngine/include/KEngine/Common/Point2D.hpp \
    ../../KEngine/include/KEngine/Common/Queues.hpp \
    ../../KEngine/include/KEngine/Common/Queues.inl \
    ../../KEngine/include/KEngine/Common/Rect2D.hpp \
    ../../KEngine/include/KEngine/Common/ScopeFunc.hpp \
    ../../KEngine/include/KEngine/Common/Size2D.hpp \
    ../../KEngine/include/KEngine/Common/StopWatch.hpp \
    ../../KEngine/include/KEngine/Common/String.hpp \
    ../../KEngine/include/KEngine/Common/ThreadPool.hpp \
    ../../KEngine/include/KEngine/Common/Time.hpp \
    ../../KEngine/include/KEngine/Common/Time.inl \
    ../../KEngine/include/KEngine/Common/Transform2D.hpp \
    ../../KEngine/include/KEngine/Core/Entity.hpp \
    ../../KEngine/include/KEngine/Core/EntityFactory.hpp \
    ../../KEngine/include/KEngine/Core/EntityManager.hpp \
    ../../KEngine/include/KEngine/Core/EventManager.hpp \
    ../../KEngine/include/KEngine/Core/EventManagerImpl.hpp \
    ../../KEngine/include/KEngine/Core/FrameRateCounter.hpp \
    ../../KEngine/include/KEngine/Core/Keyboard.hpp \
    ../../KEngine/include/KEngine/Core/Mouse.hpp \
    ../../KEngine/include/KEngine/Core/ResourceManager.hpp \
    ../../KEngine/include/KEngine/Core/StateMachine.hpp \
    ../../KEngine/include/KEngine/Core/SystemManager.hpp \
    ../../KEngine/include/KEngine/Entity/Builders/JsonEntityBuilder.hpp \
    ../../KEngine/include/KEngine/Entity/ComponentBuilders/EntityRenderableComponentBuilder.hpp \
    ../../KEngine/include/KEngine/Entity/Components/EntityCameraComponent.hpp \
    ../../KEngine/include/KEngine/Entity/Components/EntityRenderableComponents.hpp \
    ../../KEngine/include/KEngine/Events/SDL2/SDL2Event.hpp \
    ../../KEngine/include/KEngine/Events/SFML/SfmlEvent.hpp \
    ../../KEngine/include/KEngine/Events/SFML/SfmlEventTranslator.hpp \
    ../../KEngine/include/KEngine/Events/AppEvents.hpp \
    ../../KEngine/include/KEngine/Events/AppExitRequestedEvent.hpp \
    ../../KEngine/include/KEngine/Events/EntityTransformControlCommandEvents.hpp \
    ../../KEngine/include/KEngine/Events/EventLoopFrameEvent.hpp \
    ../../KEngine/include/KEngine/Events/GraphicsLoopFrameEvent.hpp \
    ../../KEngine/include/KEngine/Events/GraphicsLoopSetupFailureEvent.hpp \
    ../../KEngine/include/KEngine/Events/LogicLoopFrameEvent.hpp \
    ../../KEngine/include/KEngine/Events/LoopFrameEvent.hpp \
    ../../KEngine/include/KEngine/Events/OtherGraphicsEvents.hpp \
    ../../KEngine/include/KEngine/Graphics/SDL2/SDL2Window.hpp \
    ../../KEngine/include/KEngine/Graphics/SFML/CircleShapeRenderer.hpp \
    ../../KEngine/include/KEngine/Graphics/SFML/LineRenderer.hpp \
    ../../KEngine/include/KEngine/Graphics/SFML/SfmlHelper.hpp \
    ../../KEngine/include/KEngine/Graphics/SFML/SfmlWindow.hpp \
    ../../KEngine/include/KEngine/Graphics/SFML/SpriteRenderer.hpp \
    ../../KEngine/include/KEngine/Graphics/GraphicsCommand.hpp \
    ../../KEngine/include/KEngine/Graphics/RenderLayer.hpp \
    ../../KEngine/include/KEngine/Graphics/RenderSystem.hpp \
    ../../KEngine/include/KEngine/Graphics/Scene.hpp \
    ../../KEngine/include/KEngine/Graphics/SceneNodes.hpp \
    ../../KEngine/include/KEngine/Graphics/Window.hpp \
    ../../KEngine/include/KEngine/Graphics/WindowFactory.hpp \
    ../../KEngine/include/KEngine/Interfaces/IApp.hpp \
    ../../KEngine/include/KEngine/Interfaces/IAppView.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEntity.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEntityBuilder.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEntityComponent.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEntityComponentBuilder.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEvent.hpp \
    ../../KEngine/include/KEngine/Interfaces/IEventManagerImpl.hpp \
    ../../KEngine/include/KEngine/Interfaces/IGraphicsCommandRenderer.hpp \
    ../../KEngine/include/KEngine/Interfaces/IInputController.hpp \
    ../../KEngine/include/KEngine/Interfaces/IRenderCommand.hpp \
    ../../KEngine/include/KEngine/Interfaces/IResource.hpp \
    ../../KEngine/include/KEngine/Interfaces/IResourceLoader.hpp \
    ../../KEngine/include/KEngine/Interfaces/IScene.hpp \
    ../../KEngine/include/KEngine/Interfaces/ISceneNode.hpp \
    ../../KEngine/include/KEngine/Interfaces/IStateMachine.hpp \
    ../../KEngine/include/KEngine/Interfaces/ISystem.hpp \
    ../../KEngine/include/KEngine/Interfaces/IWindow.hpp \
    ../../KEngine/include/KEngine/Log/spdlog/details/async_logger_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/circular_q.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/console_globals.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/file_helper.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/fmt_helper.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/log_msg.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/logger_impl.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/mpmc_blocking_q.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/null_mutex.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/os.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/pattern_formatter.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/periodic_worker.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/registry.h \
    ../../KEngine/include/KEngine/Log/spdlog/details/thread_pool.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/chrono.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/color.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/core.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format-inl.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/format.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/locale.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ostream.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/posix.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/printf.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/ranges.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bundled/time.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/bin_to_hex.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/fmt.h \
    ../../KEngine/include/KEngine/Log/spdlog/fmt/ostr.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/android_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/ansicolor_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/base_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/basic_file_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/daily_file_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/dist_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/msvc_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/null_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/ostream_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/rotating_file_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/stdout_color_sinks.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/stdout_sinks.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/syslog_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/sinks/wincolor_sink.h \
    ../../KEngine/include/KEngine/Log/spdlog/async.h \
    ../../KEngine/include/KEngine/Log/spdlog/async_logger.h \
    ../../KEngine/include/KEngine/Log/spdlog/common.h \
    ../../KEngine/include/KEngine/Log/spdlog/formatter.h \
    ../../KEngine/include/KEngine/Log/spdlog/logger.h \
    ../../KEngine/include/KEngine/Log/spdlog/spdlog.h \
    ../../KEngine/include/KEngine/Log/spdlog/tweakme.h \
    ../../KEngine/include/KEngine/Log/spdlog/version.h \
    ../../KEngine/include/KEngine/Log/Log.hpp \
    ../../KEngine/include/KEngine/Resources/Loaders/ImageResourceLoader.hpp \
    ../../KEngine/include/KEngine/Resources/ImageResource.hpp \
    ../../KEngine/include/KEngine/Resources/ResourceManifestResource.hpp \
    ../../KEngine/include/KEngine/UnitTest/catch.hpp \
    ../../KEngine/include/KEngine/Utility/FileSystemHelper.hpp \
    ../../KEngine/include/KEngine/Views/HumanView.hpp \
    ../../KEngine/include/KEngine/App.hpp \
    ../../KEngine/include/KEngine/BaseAppLogic.hpp \
    ../../KEngine/include/KEngine/CommandLineOptions.hpp \
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
