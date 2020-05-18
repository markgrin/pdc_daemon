cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(pdc_daemon)

find_package(Boost 1.69 COMPONENTS program_options REQUIRED )

include_directories(${Boost_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/server.cpp
    src/conf.cpp
    src/session.cpp
    src/berlang.cpp
    src/math/berlang.cpp
    src/math/bern_stat.cpp
    src/math/bin_poi.cpp
    src/math/emp_dist.cpp)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES} jsoncpp pthread)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
