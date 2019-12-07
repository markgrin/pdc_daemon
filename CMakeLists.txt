cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(pdc_daemon)

find_package(Boost 1.69 COMPONENTS program_options REQUIRED )

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)

include_directories(${Boost_INCLUDE_DIR} ${JSON_INC_PATH} ${JSONCPP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/server.cpp src/conf.cpp src/session.cpp)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES} jsoncpp pthread)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
