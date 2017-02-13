TARGET = ProjectForecast
TEMPLATE = app

CONFIG += console



#
# C++ flags to get C++1z features.
#
win32: CONFIG += c++14
unix:  CONFIG += c++1z
unix:  QMAKE_CXXFLAGS += -std=c++1z
#


#
# For unit testing:
#   create new Build Configuration and add 'CONFIG+=run_unit_tests' to the qmake additional arguments field.
#
run_unit_tests {
    DEFINES += RUN_UNIT_TESTS
}
#


#
#
#
#DEFINES += USE_SDL
DEFINES += USE_SFML
#


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
    ../../../KEngine/include/KEngine/Common/Queues.hpp \
    ../../../KEngine/include/KEngine/Common/StopWatch.hpp \
    ../../../KEngine/include/KEngine/Common/String.hpp \
    ../../../KEngine/include/KEngine/Common/Time.hpp \
    ../../../KEngine/include/KEngine/Common/macros.hpp \
    ../../../KEngine/include/KEngine/Common/ScopeFunc.hpp \
    ../../../KEngine/include/KEngine/Common/Libs/json.hpp \
    ../../../KEngine/include/KEngine/Common/Libs/SRDelegate.hpp \
    ../../../KEngine/include/KEngine/Common/Json.hpp \
    ../../../KEngine/include/KEngine/Common/ThreadPool.hpp \
    ../../../KEngine/include/KEngine/Common/HeartBeatGenerator.hpp \
    ../../../KEngine/include/KEngine/Core/EventManager.hpp \
    ../../../KEngine/include/KEngine/Core/EventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Core/Entity.hpp \
    ../../../KEngine/include/KEngine/Core/StateMachine.hpp \
    ../../../KEngine/include/KEngine/Core/ResourceManager.hpp \
    ../../../KEngine/include/KEngine/Core/EntityFactory.hpp \
    ../../../KEngine/include/KEngine/Core/EntityManager.hpp \
    ../../../KEngine/include/KEngine/Core/SystemManager.hpp \
    ../../../KEngine/include/KEngine/Interface/IApp.hpp \
    ../../../KEngine/include/KEngine/Interface/IEvent.hpp \
    ../../../KEngine/include/KEngine/Interface/IEventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Interface/ISystem.hpp \
    ../../../KEngine/include/KEngine/Interface/IWindow.hpp \
    ../../../KEngine/include/KEngine/Interface/IEntity.hpp \
    ../../../KEngine/include/KEngine/Interface/IEntityComponent.hpp \
    ../../../KEngine/include/KEngine/Interface/IRenderCommand.hpp \
    ../../../KEngine/include/KEngine/Interface/IStateMachine.hpp \
    ../../../KEngine/include/KEngine/Interface/IEntityComponentJsonLoader.hpp \
    ../../../KEngine/include/KEngine/Interface/IAppView.hpp \
    ../../../KEngine/include/KEngine/Interface/IScene.hpp \
    ../../../KEngine/include/KEngine/Interface/ISceneNode.hpp \
    ../../../KEngine/include/KEngine/Events/SDL2/SDL2Event.hpp \
    ../../../KEngine/include/KEngine/Events/SFML/SfmlEvent.hpp \
    ../../../KEngine/include/KEngine/Events/AppExitRequestedEvent.hpp \
    ../../../KEngine/include/KEngine/Events/EventLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/GraphicsLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/LogicLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/LoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/GraphicsLoopSetupFailureEvent.hpp \
    ../../../KEngine/include/KEngine/Graphics/WindowFactory.hpp \
    ../../../KEngine/include/KEngine/Graphics/SFML/SfmlWindow.hpp \
    ../../../KEngine/include/KEngine/Graphics/SDL2/SDL2Window.hpp \
    ../../../KEngine/include/KEngine/Graphics/RenderCommands/TestRenderCommands.hpp \
    ../../../KEngine/include/KEngine/Graphics/RenderSystem.hpp \
    ../../../KEngine/include/KEngine/Graphics/Scene.hpp \
    ../../../KEngine/include/KEngine/Graphics/Window.hpp \
    ../../../KEngine/include/KEngine/Log/Log.hpp \
    ../../../KEngine/include/KEngine/UnitTest/catch.hpp \
    ../../../KEngine/include/KEngine/App.hpp \
    ../../../KEngine/include/KEngine/BaseAppLogic.hpp \
    ../../../KEngine/include/KEngine/Views/HumanView.hpp

SOURCES += \
    ../../../KEngine/include/KEngine/Common/Queues.inl \
    ../../../KEngine/include/KEngine/Common/Time.inl \
    ../../../KEngine/src/Common/Time.cpp \
    ../../../KEngine/src/Core/EventManagerImpl.cpp \
    ../../../KEngine/src/Core/EntityFactory.cpp \
    ../../../KEngine/src/Core/Entity.cpp \
    ../../../KEngine/src/Core/StateMachine.cpp \
    ../../../KEngine/src/Core/EntityManager.cpp \
    ../../../KEngine/src/Core/SystemManager.cpp \
    ../../../KEngine/src/Core/ResourceManager.cpp \
    ../../../KEngine/src/Graphics/SDL2/SDL2Window.cpp \
    ../../../KEngine/src/Graphics/SFML/SfmlWindow.cpp \
    ../../../KEngine/src/Graphics/WindowFactory.cpp \
    ../../../KEngine/src/Graphics/RenderSystem.cpp \
    ../../../KEngine/src/Graphics/Scene.cpp \
    ../../../KEngine/src/Views/HumanView.cpp \
    ../../../KEngine/src/App.cpp \
    ../../../KEngine/src/BaseAppLogic.cpp \
    ../../../ProjectForecast/main.cpp \
    ../../../KEngine/src/dump.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Delegate.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventDelegates.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventManager.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Events.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Time.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EntityAndComponents.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_StateMachine.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EntityFactory.cpp
#

#
# Setup external library dependencies.
#
macx {
    INCLUDEPATH += /Library/Frameworks/SDL2.framework/Headers
    LIBS += -F/Library/Frameworks
    LIBS += -framework SDL2
}
#
