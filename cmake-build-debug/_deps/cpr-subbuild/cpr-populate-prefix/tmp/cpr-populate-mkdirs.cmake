# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-src"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-build"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/tmp"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/src"
  "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ron0/development/cpp-projects/SpaceCadets/Challenge-1-email/cmake-build-debug/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
