include (platform/nk)

# Set compilation flags.
project_header_default ("STANDARD_GNU_11:YES" "EXTRA_STRICT_WARNINGS:YES")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

set (INCLUDE_DIR ${CMAKE_SOURCE_DIR}/az/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package (Threads REQUIRED)

add_nk_edl (TestEntity_edl "${RESOURCES}/TestEntity.edl"
            NK_MODULE "highlevel"
            LANG "CXX")

add_executable (TestEntity
    "src/main.cpp"
    "${CMAKE_SOURCE_DIR}/kettle/src/logic.cpp"
    )

add_dependencies (TestEntity TestEntity_edl)

target_link_libraries(TestEntity PRIVATE
                    gtest
                    gmock
                    Threads::Threads)

target_include_directories (TestEntity PRIVATE ${INCLUDE_DIR})