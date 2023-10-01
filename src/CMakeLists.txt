add_executable( main
    ./emulator/cpp/cpu8086.cpp
    ./emulator/cpp/Memory.cpp
    ./emulator/cpp/Tester.cpp
    ./emulator/cpp/TestingModule.cpp

    ./gui/cpp/ConsolePresenter.cpp
    ./gui/cpp/CreateLabFrame.cpp
    ./gui/cpp/DescriptionDialog.cpp
    ./gui/cpp/DescriptionFrame.cpp
    ./gui/cpp/EmulatorApp.cpp
    ./gui/cpp/main.cpp
    ./gui/cpp/MainFrame.cpp
    ./gui/cpp/VerifyLabFrame.cpp
)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/src/emulator/hdr)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/src/gui/hdr)

set(BOOST_ROOT D:\\Development\\Libs\\boost_1_83_0)
find_package(Boost)
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
target_link_directories(main PRIVATE ${BOOST_ROOT}/stage/lib)

set(wxWidgets_ROOT_DIR D:\\Development\\Libs\\wxWidgets)
find_package(wxWidgets)
include(${wxWidgets_USE_FILE})
target_include_directories(main PRIVATE ${wxWidgets_ROOT_DIR}/include)
target_include_directories(main PRIVATE ${wxWidgets_ROOT_DIR}/include/msvc)
target_link_directories(main PRIVATE ${wxWidgets_ROOT_DIR}/lib/vc_x64_lib)
target_link_libraries(main ${wxWidgets_LIBRARIES})
