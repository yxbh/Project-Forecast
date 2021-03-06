TARGET = ProjectForecast
TEMPLATE = app

CONFIG += console

#
# Generate the object files next to their source to allow objects with the same names.
#
CONFIG += object_parallel_to_source
#


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
    ../../../KEngine/include/KEngine/Common/Json.hpp \
    ../../../KEngine/include/KEngine/Common/ThreadPool.hpp \
    ../../../KEngine/include/KEngine/Common/HeartBeatGenerator.hpp \
    ../../../KEngine/include/KEngine/Common/Point2D.hpp \
    ../../../KEngine/include/KEngine/Common/Color.hpp \
    ../../../KEngine/include/KEngine/Common/Dimension2D.hpp \
    ../../../KEngine/include/KEngine/Common/Transform2D.hpp \
    ../../../KEngine/include/KEngine/Common/Libs/json.hpp \
    ../../../KEngine/include/KEngine/Common/Libs/SRDelegate.hpp \
    ../../../KEngine/include/KEngine/Common/Libs/atomicops.h \
    ../../../KEngine/include/KEngine/Common/Libs/concurrentqueue.h \
    ../../../KEngine/include/KEngine/Common/Libs/readerwriterqueue.h \
    ../../../KEngine/include/KEngine/Common/Rect2D.hpp \
    ../../../KEngine/include/KEngine/Common/Size2D.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IApp.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IAppView.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IEntity.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IEntityComponent.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IEntityComponentJsonLoader.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IEvent.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IEventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IGraphicsCommandRenderer.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IInputController.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IRenderCommand.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IResource.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IResourceLoader.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IScene.hpp \
    ../../../KEngine/include/KEngine/Interfaces/ISceneNode.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IStateMachine.hpp \
    ../../../KEngine/include/KEngine/Interfaces/ISystem.hpp \
    ../../../KEngine/include/KEngine/Interfaces/IWindow.hpp \
    ../../../KEngine/include/KEngine/Core/EventManager.hpp \
    ../../../KEngine/include/KEngine/Core/EventManagerImpl.hpp \
    ../../../KEngine/include/KEngine/Core/Entity.hpp \
    ../../../KEngine/include/KEngine/Core/FrameRateCounter.hpp \
    ../../../KEngine/include/KEngine/Core/StateMachine.hpp \
    ../../../KEngine/include/KEngine/Core/ResourceManager.hpp \
    ../../../KEngine/include/KEngine/Core/EntityFactory.hpp \
    ../../../KEngine/include/KEngine/Core/EntityManager.hpp \
    ../../../KEngine/include/KEngine/Core/SystemManager.hpp \
    ../../../KEngine/include/KEngine/Core/Keyboard.hpp \
    ../../../KEngine/include/KEngine/Core/Mouse.hpp \
    ../../../KEngine/include/KEngine/Entity/Components/EntityRenderableComponents.hpp \
    ../../../KEngine/include/KEngine/Entity/Components/EntityCameraComponent.hpp \
    ../../../KEngine/include/KEngine/Entity/ComponentLoaders/EntityRenderableComponentLoader.hpp \
    ../../../KEngine/include/KEngine/Events/SDL2/SDL2Event.hpp \
    ../../../KEngine/include/KEngine/Events/SFML/SfmlEvent.hpp \
    ../../../KEngine/include/KEngine/Events/SFML/SfmlEventTranslator.hpp \
    ../../../KEngine/include/KEngine/Events/AppExitRequestedEvent.hpp \
    ../../../KEngine/include/KEngine/Events/AppEvents.hpp \
    ../../../KEngine/include/KEngine/Events/EventLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/GraphicsLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/LogicLoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/LoopFrameEvent.hpp \
    ../../../KEngine/include/KEngine/Events/GraphicsLoopSetupFailureEvent.hpp \
    ../../../KEngine/include/KEngine/Events/EntityTransformControlCommandEvents.hpp \
    ../../../KEngine/include/KEngine/Events/OtherGraphicsEvents.hpp \
    ../../../KEngine/include/KEngine/Graphics/WindowFactory.hpp \
    ../../../KEngine/include/KEngine/Graphics/SFML/SfmlWindow.hpp \
    ../../../KEngine/include/KEngine/Graphics/SFML/CircleShapeRenderer.hpp \
    ../../../KEngine/include/KEngine/Graphics/SDL2/SDL2Window.hpp \
    ../../../KEngine/include/KEngine/Graphics/RenderSystem.hpp \
    ../../../KEngine/include/KEngine/Graphics/Scene.hpp \
    ../../../KEngine/include/KEngine/Graphics/Window.hpp \
    ../../../KEngine/include/KEngine/Graphics/GraphicsCommand.hpp \
    ../../../KEngine/include/KEngine/Graphics/SceneNodes.hpp \
    ../../../KEngine/include/KEngine/Graphics/RenderLayer.hpp \
    ../../../KEngine/include/KEngine/Log/Log.hpp \
    ../../../KEngine/include/KEngine/UnitTest/catch.hpp \
    ../../../KEngine/include/KEngine/App.hpp \
    ../../../KEngine/include/KEngine/BaseAppLogic.hpp \
    ../../../KEngine/include/KEngine/Views/HumanView.hpp \
    ../../../ProjectForecast/Systems/DebugSceneGridSystem.hpp \
    ../../../ProjectForecast/Systems/GMSAssetResourceManagementSystem.hpp \
    ../../../ProjectForecast/Systems/GMSRoomManagementSystem.hpp \
    ../../../ProjectForecast/Events/GMSRoomLoadRequestEvent.hpp \
    ../../../ProjectForecast/ProjectForecastApp.hpp \
    ../../../ProjectForecast/Views/HumanView.hpp \
    ../../../ProjectForecast/Events/RequestDrawDebugDotEvent.hpp \
    ../../../ProjectForecast/Systems/DebugDrawSystem.hpp \
    ../../../ProjectForecast/Systems/EventFeedbackSystem.hpp \
    ../../../ProjectForecast/InputControllers/InputControllers.hpp \
    ../../../ProjectForecast/Systems/EntityTransformControlSystem.hpp

SOURCES += \
    ../../../KEngine/include/KEngine/Common/Queues.inl \
    ../../../KEngine/include/KEngine/Common/Time.inl \
    ../../../KEngine/src/Common/Color.cpp \
    ../../../KEngine/src/Common/Time.cpp \
    ../../../KEngine/src/Core/EventManagerImpl.cpp \
    ../../../KEngine/src/Core/EntityFactory.cpp \
    ../../../KEngine/src/Core/Entity.cpp \
    ../../../KEngine/src/Core/StateMachine.cpp \
    ../../../KEngine/src/Core/EntityManager.cpp \
    ../../../KEngine/src/Core/SystemManager.cpp \
    ../../../KEngine/src/Core/ResourceManager.cpp \
    ../../../KEngine/src/Core/Keyboard.cpp \
    ../../../KEngine/src/Core/Mouse.cpp \
    ../../../KEngine/src/Entity/EntityComponentLoaders/EntityRenderComponentLoader.cpp \
    ../../../KEngine/src/Events/SFML/SfmlEventTranslator.cpp \
    ../../../KEngine/src/Graphics/SDL2/SDL2Window.cpp \
    ../../../KEngine/src/Graphics/SFML/SfmlWindow.cpp \
    ../../../KEngine/src/Graphics/WindowFactory.cpp \
    ../../../KEngine/src/Graphics/RenderSystem.cpp \
    ../../../KEngine/src/Graphics/Scene.cpp \
    ../../../KEngine/src/Graphics/SceneNodes.cpp \
    ../../../KEngine/src/Utility/FileSystemHelper.cpp \
    ../../../KEngine/src/Views/HumanView.cpp \
    ../../../KEngine/src/App.cpp \
    ../../../KEngine/src/BaseAppLogic.cpp \
    ../../../KEngine/src/dump.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Delegate.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventDelegates.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EventManager.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Events.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_Time.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EntityAndComponents.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_StateMachine.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EntityFactory.cpp \
    ../../../KEngine/src/UnitTest/UnitTest_EntityManager.cpp \
    ../../../ProjectForecast/ProjectForecastApp.cpp \
    ../../../ProjectForecast/Views/HumanView.cpp \
    ../../../ProjectForecast/Systems/DebugDrawSystem.cpp \
    ../../../ProjectForecast/Systems/EventFeedbackSystem.cpp \
    ../../../ProjectForecast/Systems/EntityTransformControlSystem.cpp \
    ../../../ProjectForecast/InputControllers/InputControllers.cpp \
    ../../../ProjectForecast/main.cpp \
    ../../../ProjectForecast/Systems/DebugSceneGridSystem.cpp \
    ../../../ProjectForecast/Systems/GMSAssetResourceManagementSystem.cpp \
    ../../../ProjectForecast/Systems/GMSRoomManagementSystem.cpp
#

#
# Setup external library dependencies.
#
macx {
    #
    # SDL
    #
    INCLUDEPATH += /Library/Frameworks/SDL2.framework/Headers
    LIBS += -F/Library/Frameworks
    LIBS += -framework SDL2
    #

    #
    # SFML
    #
    INCLUDEPATH += ../../../lib/libs-osx/SFML-2.4.1-osx-clang/include
    DEPENDPATH += ../../../lib/libs-osx/SFML-2.4.1-osx-clang/include
    `MAKE_LFLAGS += -F../../../lib/libs-osx/SFML-2.4.1-osx-clang/Frameworks
    LIBS += -framework SFML -framework sfml-system -framework sfml-window -framework sfml-graphics
    QMAKE_RPATHDIR += @executable_path/../../../ #this is pointing to the folder where the app file is.
    QMAKE_RPATHDIR += @executable_path/../Frameworks # this is pointing to inside the app file.
    #
}
#
