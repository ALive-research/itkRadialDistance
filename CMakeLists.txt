cmake_minimum_required(VERSION 3.15)

project(itkRadialDistance)

find_package(ITK REQUIRED)

include(${ITK_USE_FILE})

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cxx)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ITK_LIBRARIES})
