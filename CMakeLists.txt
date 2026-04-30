cmake_minimum_required(VERSION 3.10)

project(WebRichPresence)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)  

add_executable(WebRichPresence src/main.cpp)
add_executable(WebRichPresenceW WIN32 src/main.cpp)

function(configure_target tgt)

    set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/lib/discord_social_sdk")
    set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_ROOT}/lib/release")
    set(DISCORD_SDK_BIN_DIR "${DISCORD_SDK_ROOT}/bin/release")
    set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_ROOT}/include") 

    target_include_directories(${tgt} PRIVATE
        ${DISCORD_SDK_INCLUDE_DIR}
        include
    )

    if(WIN32)
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
    elseif(APPLE)
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
    else()
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
    endif()

    target_link_libraries(${tgt} PRIVATE
        ${DISCORD_LIB_PATH}
        ZLIB::ZLIB
        OpenSSL::Crypto
        ws2_32
        mswsock
    )

    if(UNIX)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        set(CMAKE_INSTALL_RPATH "$ORIGIN")
        if(APPLE)
            set(CMAKE_INSTALL_RPATH "@executable_path")
        endif()
    endif()

    add_custom_command(TARGET ${tgt} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DISCORD_SHARED_LIB}"
            $<TARGET_FILE_DIR:${tgt}>
    )

    target_link_options(${tgt} PRIVATE
        -static
    )
endfunction()

configure_target(WebRichPresence)
configure_target(WebRichPresenceW)