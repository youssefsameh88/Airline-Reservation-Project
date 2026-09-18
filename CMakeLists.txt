cmake_minimum_required(VERSION 3.16)

project(AirlineReservation
        VERSION 1.0.0
        DESCRIPTION "Console Airline Reservation System"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(MODEL_SOURCES
    src/models/Admin.cpp
    src/models/Aircraft.cpp
    src/models/BookingAgent.cpp
    src/models/CrewMember.cpp
    src/models/Flight.cpp
    src/models/FlightAttendant.cpp
    src/models/Maintenance.cpp
    src/models/Passenger.cpp
    src/models/Pilot.cpp
    src/models/Reservation.cpp
    src/models/Seat.cpp
    src/models/User.cpp
)

set(SYSTEM_SOURCES
    src/system/admin.cpp
    src/system/agent.cpp
    src/system/passenger.cpp
    src/system/reports.cpp
    src/system/session.cpp
    src/system/ui.cpp
    src/system/FileManager.cpp
)

set(SERVICE_SOURCES
    src/services/ReportManager.cpp
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${MODEL_SOURCES}
    ${SYSTEM_SOURCES}
    ${SERVICE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/services
    ${CMAKE_CURRENT_SOURCE_DIR}/include/system
    ${CMAKE_CURRENT_SOURCE_DIR}/include/enums
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# copy the data folder next to the executable so it can run from the build dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMENT "Copying data folder next to executable"
)