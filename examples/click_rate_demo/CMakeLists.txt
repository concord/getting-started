cmake_minimum_required (VERSION 2.6)
project (concord)
set(VERSION 0.1.0)

link_directories (
  /usr/local/lib
  /usr/lib/x86_64-linux-gnu
  )


set (CPP_CLIENT_LIBRARIES
  :libbolt_cpp_client.so
  :libglog.so.0
  :libthriftnb-0.9.3.so
  :libthrift-0.9.3.so
  :libthriftz-0.9.3.so
  :libglog.so.0.0.0
  :libpthread.so
  :libdouble-conversion.so.1.0
  :libboost_system.so.1.55.0
  :libevent.a # Hack couldn't get libevent to link
  :libglog.so.0
  :libstdc++.so.6
  :libssl.so.1.0.0
  :libcrypto.so.1.0.0
  :libgflags.so.2
  :libunwind.so.8
  :libpthread.so.0
  :libz.so.1.2.8
  )

set(BASE_FLAGS
  "-fPIC"
  "-fuse-ld=gold"
  "-Wall"
  "-Werror"
  "-Wextra"
  "-Wformat"
  "-pedantic"
  "-Wmissing-braces"
  "-Wparentheses"
  "-Wpointer-arith"
  "-Wformat-security"
  "-Wunused"
  "-Wno-unused-parameter"
  "-Wcast-align"
  "-Wmissing-field-initializers"
  "-O2"
  )

set (CXX_FLAG_LIST
  ${BASE_FLAGS}
  "-std=c++1y"
  "-ftemplate-depth-256"
  "-Wnon-virtual-dtor"
  "-Woverloaded-virtual"
  "-Wsign-promo"
  "-Wreorder"
  "-Wdelete-non-virtual-dtor"
  )


set (THRIFT_SOURCES
  ${PROJECT_SOURCE_DIR}/gen-cpp/click_types.cpp
  )

function (join VALUES SEP OUTPUT)
  string (REPLACE ";" "${SEP}" _TMP_STR "${VALUES}")
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction ()

add_custom_command(
  OUTPUT ${THRIFT_SOURCES}
  DEPENDS ${PROJECT_SOURCE_DIR}/thrift/click.thrift
  COMMAND thrift -v --gen cpp -o ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/thrift/click.thrift
  )

set (CMAKE_CXX_COMPILER g++-4.9)
set (CMAKE_C_COMPILER gcc-4.9)
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -v" )
join ("${CXX_FLAG_LIST}" " " CMAKE_CXX_FLAGS)
join ("${BASE_FLAGS}" " " CMAKE_C_FLAGS)

add_library(thrift_lib STATIC
  ${THRIFT_SOURCES}
  )

add_executable (ctr_calculator ${PROJECT_SOURCE_DIR}/ctr_calculator.cc)
target_link_libraries (ctr_calculator ${CPP_CLIENT_LIBRARIES} thrift_lib)
add_executable (click_generator ${PROJECT_SOURCE_DIR}/click_generator.cc)
target_link_libraries (click_generator ${CPP_CLIENT_LIBRARIES} thrift_lib)
add_executable (impression_generator ${PROJECT_SOURCE_DIR}/impression_generator.cc)
target_link_libraries (impression_generator ${CPP_CLIENT_LIBRARIES} thrift_lib)
