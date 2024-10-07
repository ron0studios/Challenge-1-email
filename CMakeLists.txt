cmake_minimum_required(VERSION 3.28)
project(Challenge_1_email)

set(CMAKE_CXX_STANDARD 23)

add_executable(Challenge_1_email main.cpp)


include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
target_link_libraries(Challenge_1_email PRIVATE cpr::cpr)