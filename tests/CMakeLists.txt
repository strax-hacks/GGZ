cmake_minimum_required(VERSION 3.16)

add_executable(smoke_test
    smoke_test.cpp
)

target_link_libraries(smoke_test PRIVATE
    openggs_core
)

add_executable(capture_screens
    capture_screens.cpp
)

target_link_libraries(capture_screens PRIVATE
    openggs_core
)

add_test(NAME SmokeTest COMMAND smoke_test)
add_test(NAME CaptureScreens COMMAND capture_screens)
