cmake_minimum_required(VERSION 3.7)
project(binapi)

set(CMAKE_CXX_STANDARD 23)

<<<<<<< HEAD
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")
=======
# Add the following lines to include Boost
set(Boost_USE_STATIC_LIBS ON)  # Link with Boost static libraries
set(Boost_USE_MULTITHREAD ON)

find_package(Boost 1.84 REQUIRED COMPONENTS system json options)
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")
>>>>>>> 145cbee74632b02c30d2f6e6de59d695fb67373e

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
)

include_directories(
    ./include ${Boost_INCLUDE_DIR} ${OpenSSL_INCLUDE_DIR}
)

set(BINAPI_HEADERS
    binapi/api.hpp
    binapi/flatjson.hpp
    binapi/dtf.hpp
    binapi/double_type.hpp
    binapi/enums.hpp
    binapi/errors.hpp
    binapi/invoker.hpp
    binapi/message.hpp
    binapi/pairslist.hpp
    binapi/reports.hpp
    binapi/tools.hpp
    binapi/types.hpp
    binapi/websocket.hpp
    binapi/renewable_websocket.hpp
)

set(BINAPI_SOURCES
    src/api.cpp
    src/enums.cpp
    src/errors.cpp
    src/pairslist.cpp
    src/reports.cpp
    src/tools.cpp
    src/types.cpp
    src/websocket.cpp
    src/renewable_websocket.cpp
)

ADD_LIBRARY( binapi STATIC
             src/api.cpp
             src/enums.cpp
             src/errors.cpp
             src/pairslist.cpp
             src/reports.cpp
             src/tools.cpp
             src/types.cpp
             src/websocket.cpp
             src/renewable_websocket.cpp )

set_target_properties(binapi PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if (DEFINED ${BOOST_LIB_DIR})
    target_link_libraries(
        ${PROJECT_NAME}
        -L${BOOST_LIB_DIR}
    )
endif()

target_link_libraries(
    ${PROJECT_NAME}
    z
    crypto
    ssl
    pthread
)