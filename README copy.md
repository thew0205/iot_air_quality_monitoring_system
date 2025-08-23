# IoT Air Quality Monitoring System

This repository contains the electronic and firmware files for the **IoT Air Quality Monitoring System**.

The project is divided into two main parts:  
1. **iaq_hardware**  
2. **iaq_firmware**

---

## Skills Required

To contribute effectively, it is helpful to have some familiarity with the following:  
- **Raspberry Pi Pico** (development board)  
- **CMake** (build system for modular projects)  
- **KiCad** (PCB and hardware design)  
- **Basic scripting** (for automation and build tools)

---

## Project Overview

- **Hardware (`iaq_hardware`)**:  
  This is a **KiCad project** that contains reference schematics, connections, and pin assignments. The goal is to provide an easy way to understand the hardware setup for the prototype. The hardware section will mostly include modules for easy development and validation.

- **Firmware (`iaq_firmware`)**:  
  The firmware is designed to be **modular** and **collaborator-friendly**. CMake is used as the build system since it provides flexibility and modularity.  

  A key goal of the firmware is **professionalism**—the codebase is structured like a production-ready embedded systems project. This means it will contain:
  - Clean separation of functions  
  - Hardware abstraction (decoupling hardware-specific code from general code)  
  - **Testing support**, especially **unit tests**  

---

## Testing

Testing is a **major part** of this project. To achieve this:  
- We use **CppUTest** as the testing framework.  
- The project integrates **dual target testability**.  
  - This means we can test code both on the **host system** (PC) and the **embedded target** (Raspberry Pi Pico).  
  - This setup makes development faster and ensures reliability before deploying to hardware.  

Scripts in the `tools/` directory simplify building and running these tests.

---

## Project Structure

Below is the current structure of the repository:

.  
├── build  
├── CMakeLists.txt  
├── docs
│ ├── Doxyfile  
│ ├── html  
│ └── latex  
├─| include  
│ └── blink.h  
├── pico_sdk_import.cmake  
├── README.md  
├── src
│ ├── CMakeLists.txt  
│ ├── main.cpp  
│ ├── pico  
│ ├── sensors  
│ │ ├── pico  
│ │ ├── CMakeLists.txt  
│ │ ├── sensor.cpp  
│ │ └── sensor.h  
│ └── wifi  
│ ├── pico  
│ ├── CMakeLists.txt  
│ ├── wifi.cpp  
│ └── wifi.h  
├── tests  
│ ├── host  
│ ├── target  
│ └── third_party  
├── third_party  
└── tools  
    ├── dual_target_test_build_run.sh  
    └── dual_target_test_build.sh  



---

## Contribution Guide

1. Clone the repo and set up your environment.  
2. Explore the `iaq_hardware` (for hardware setup) or `iaq_firmware` (for firmware development).  
3. Use **CMake** for building the firmware.  
4. Write unit tests for new features and verify with dual-target testing.  
5. Keep things simple and modular to help new collaborators onboard quickly.  

---

## Goal

The **ultimate goal** of this project is to create a **professional, collaborative, and testable IoT air quality monitoring system** that can be easily extended and validated by contributors of all skill levels.


# iot_air_quality_board
This repo contains electonic files for IoT air quality monitoring system.



The git hub main repo divides the project into two part
1. iaq_hardware 
2. iaq_firmware 

For easy flow of the project, the follow skills are needed:
1. pi pico
2. cmake
3. kicad
4. a little bit of scripting

Most of the development process will be done in the iaq_firmware as the iaq_hardware will majorly comprise of module for easy development and validation of the prototype. 

The iaq_harware is a kicad project. The intent is for it to contains reference for the connections made and pin assignments.

The iaq_firmware is designed to be as modular and possible to allow for easy collaboration and testability. cmake is used as the build system since it allows for modularity.
The one goal this project is to be as professional as possible so it will be comprising of major components that makes up an embedded systems project, one major thing is testing (especially unit test). To facilitate this, we will need to integrate dual target testability which could allow us to test some of the code on our host system. This will required the decoupling of hardware specific code from general code.

The testing framework to be employed is cpputest. 

The project structure as be implemented to allow for the separation of functions.

.
├── build
├── CMakeLists.txt
├── docs
│   ├── Doxyfile
│   ├── html
│   └── latex
├── include
│   └── blink.h
├── pico_sdk_import.cmake
├── README.md
├── src
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── pico
│   ├── sensors
|       |──pico
|       |──CMakeLists.txt
|       |──sensor.cpp
|       |──sensor.h
│   └── wifi
|      |──pico
|      |──CMakeLists.txt
|      |──wifi.cpp
|      |──wifi.h
├── tests
│   ├── host
│   ├── target
│   └── third_party
├── third_party
└── tools
    ├── dual_target_test_build_run.sh
    └── dual_target_test_build.sh

### Explain what each represent


### Getting STarted with the iaq_firmware
Set up the pi pico sdk preferable with the vscode extension.
clone the Repo Check out  from the dev branch.
Before you start development create a new branch to for each feature you are working on. You can copy and existing section that is similar to the feature or test you want to run and just change the name and the files also don't forget to add it to the appropriate cmake file.

There are script to help to automatically build, flash and run test to host and target. Vscode tasks and task buttons have been set up to allow easy use. To use the task buttons install the tasks buttons extension. 

Note: To build the main app you you use the manual cmake building process from the build directory run 
cmake -S .. -B . && cmake --build . -j$(nproc)

NOte: To use the task ensure the iaq_firmware is the root of the workspacefolder

dual_target_test_build.sh This build  all the test in either the target or host. It takes one arguments, the argument is either to build for host or for test [values: host | target].

dual_target_test_build_run.sh This build and run all the test in either the target or host and run a specific test depenting on the argument. It takes two arguments the first is either to build for host or for test [values: host | target] the second is the test to be run, it should be the name of the folder of the test to be run. 