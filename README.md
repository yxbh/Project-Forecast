# ProjectForecast
ProjectForecast is a work-in-progress experimental [Risk of Rain](http://riskofraingame.com/) (RoR) clone. The source is written in modern C++ using [SFML](https://www.sfml-dev.org/), [SDL2](https://www.libsdl.org/), [spdlog](https://github.com/gabime/spdlog) and a few other libraries and utilties.

The aim of the project is to produce a close recreation of the Risk of Rain game with much better performance (hopefully) and modability. There is no ETA.

The clone is being developed with the WIP KEngine game engine. KEngine is my own little custom game framework that is being built in parallel to ProjectForecast. The engine design takes influence from the Game Coding Complete 4th Edition book.

## Folder Structure
The `src/KEngine` folder contains all KEngine specific source code (as the name implies). All RoR specific implementation stuff are in the `src/ProjectForecast` folder.

To build the source, MSVS solutions and QtCreator project files are available under the `/src/build-targets` folder. See the [Building](#Building) section.

Third party dependencies such as library source and binary files are stored under `src/lib`. Folder content changes are minimised at a best effort level to reduce load on the repo.

## Building
### Windows (MSVC)
An up to date MSVC2017 target is provided under the `src/build-targets/VS2015` directory. The VC solution should compile out of the box. However the execution of built excutable may not as it may refer to local paths for resources as I am the only one developing this at the moment.

### OSX (QtCreator/qmake)
A QtCreator(QMake) project file is provided in the repo under the `src/build-targets/QtCreator/ProjectForecast` directory.  
Before building, run the \*.dmg files under the `src/lib/libs-osx` directory in the repo and copy the \*.framework files to the `/Library/Frameworks` directory on your OSX machine.  
You should then be able to open .pro file in QtCreator, run qmake and build the project.

The QtCreator project may not always be up to date as it is only used to test compilation on other compilers and OSes at this stage.

## Unit Tests
To build and run the unit tests, first follow the instructions in the [Building](## Building) section above for the respective target platforms before following the following instructions.

### Windows
The VC solution comes with preconfigured Releast and Debug configurations for unit testing. Switch to `Debug_UnitTest` or `Release_UnitTest` then build and run.

### OSX (QtCreator/qmake)
If there isn't one already. Create a new Build Configuration in QtCreator's `Projects` pane. In the qmake section under `Build steps`,  add `CONFIG+=run_unit_tests` to the `Additional Arguments` text field.  
Switch to the new build configuration in QtCreator and carry out the usual build and run steps to build and run the unit tests.
