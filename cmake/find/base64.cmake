if(ARCH_AMD64 OR ARCH_ARM)
    option (ENABLE_BASE64 "Enable base64" ${ENABLE_LIBRARIES})
elseif(ENABLE_BASE64)
    message (${RECONFIGURE_MESSAGE_LEVEL} "base64 library is only supported on x86_64 and aarch64")
endif()

if (NOT ENABLE_BASE64)
    return()
endif()

if (NOT EXISTS "${ClickHouse_SOURCE_DIR}/contrib/base64/LICENSE")
    set (MISSING_INTERNAL_BASE64_LIBRARY 1)
    message (WARNING "submodule contrib/base64 is missing. to fix try run: \n git submodule update --init --recursive")
endif ()

if (NOT EXISTS "${ClickHouse_SOURCE_DIR}/contrib/base64")
    message (WARNING "submodule contrib/base64 is missing. to fix try run: \n git submodule update --init --recursive")
else()
    set (BASE64_LIBRARY base64)
    set (USE_BASE64 1)
endif()

if (NOT USE_BASE64)
    message (${RECONFIGURE_MESSAGE_LEVEL} "Cannot enable base64")
endif()