cmake_minimum_required(VERSION 3.20.0)
project(openfiles VERSION 5.0.1 DESCRIPTION "Multi-Platform Event-Driven Application Framework")

enable_testing()

if (NOT DEFINED OPENFILE_CONFIG)
   set(OPENFILE_CONFIG "configs/android-smb")
   # set(JAVA_HOME "/Library/Java/JavaVirtualMachines/openlogic-openjdk-11.jdk/Contents/Home")
endif()

include(${OPENFILE_CONFIG})
configure_file(include/ofc/config.h.in ofc/config.h @ONLY)
include_directories(${openfiles_BINARY_DIR})

#
# Tell cmake where openssl and mbedtls are for Android and Darwin.
# Linux is much more cooperative
#
if(ANDROID)
  set(OPENSSL_NDK /Users/rschmitt/git/openssl/install-ndk/${CMAKE_ANDROID_ARCH_ABI})
  set(OPENSSL_ROOT_DIR ${OPENSSL_NDK})
  set(OPENSSL_INCLUDE_DIR ${OPENSSL_NDK}/include)
  set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_NDK}/lib/libcrypto.so)
  set(OPENSSL_SSL_LIBRARY ${OPENSSL_NDK}/lib/libssl.so)

  set(MBEDTLS_NDK /Users/rschmitt/git/mbedtls/install-ndk/${CMAKE_ANDROID_ARCH_ABI})
  set(MBEDTLS_ROOT_DIR ${MBEDTLS_NDK})
  set(MBEDTLS_INCLUDE_DIR ${MBEDTLS_NDK}/include)
  set(MBEDTLS_CRYPTO_LIBRARY ${MBEDTLS_NDK}/lib/libmbedcrypto.a)

endif()
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(OPENSSL_ROOT_DIR "/opt/homebrew/Cellar/openssl@3/3.0.7")
  set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
  set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib)
  set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib)

  set(MBEDTLS_ROOT_DIR "/opt/homebrew/Cellar/mbedtls/3.2.1")
  set(MBEDTLS_INCLUDE_DIR ${MBEDTLS_ROOT_DIR}/include)
  set(MBEDTLS_CRYPTO_LIBRARY ${MBEDTLS_ROOT_DIR}/lib/libmbedcrypto.dylib)
endif()
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # for linux, pass in MBEDTLS_ROOT_DIR
  set(MBEDTLS_INCLUDE_DIR ${MBEDTLS_ROOT_DIR}/include)
  set(MBEDTLS_CRYPTO_LIBRARY ${MBEDTLS_ROOT_DIR}/lib/libmbedcrypto.so)
endif()

if(OF_COV)
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
endif()

#
# Get the Library Variables all set up
#
if(OF_OPENSSL)
  find_package(OpenSSL)
  if (OPENSSL_FOUND)
     list(APPEND crypto_library_list ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
  endif()
endif()

if(OF_MBEDTLS)
  list(APPEND crypto_library_list ${MBEDTLS_CRYPTO_LIBRARY})
endif()

if(OF_GNUTLS)
  find_package(GnuTLS)
  if (GNUTLS_FOUND)
    list(APPEND ssl_library_list GnuTLS::GnuTLS)
  endif()
endif()

if(ANDROID AND OFC_KERBEROS)
set(KRB5_NDK /Users/rschmitt/git/krb5.of/install-ndk/${CMAKE_ANDROID_ARCH_ABI})
set(KRB5_INCLUDE_PATH ${KRB5_NDK}/include)
set(KRB5_LIBRARY_PATH ${KRB5_NDK}/lib)
set(KRB5_LIBS ${KRB5_LIBRARY_PATH}/libgssapi_krb5.a ${KRB5_LIBRARY_PATH}/libkrb5support.a ${KRB5_LIBRARY_PATH}/libcom_err.a ${KRB5_LIBRARY_PATH}/libkrb5.a ${KRB5_LIBRARY_PATH}/libk5crypto.a)
endif()

add_subdirectory(of_core)
include_directories(${of_core_SOURCE_DIR}/include)

add_subdirectory(of_core_binheap)
add_subdirectory(of_core_cheap)

if(OF_RESOLVER_FS)
  add_subdirectory(of_resolver_fs)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_subdirectory(of_core_android)
    add_subdirectory(of_core_fs_android)
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_subdirectory(of_core_darwin)
    add_subdirectory(of_core_fs_darwin)
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(of_core_linux)
    add_subdirectory(of_core_fs_linux)
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_subdirectory(of_core_windows)
    add_subdirectory(of_core_fs_windows)
endif ()
add_subdirectory(of_core_fs_bookmarks)

if(OFC_INCLUDE_JNI)
  add_subdirectory(of_core_jni)
endif()

if(OFC_FS_PIPE)
  add_subdirectory(of_core_fs_pipe)
endif()

if(OF_SMB)
  add_subdirectory(of_smb)
  add_subdirectory(of_security)
  if(OF_SMB_SERVER)
    add_subdirectory(of_smb_server)
  endif()
  if(OF_SMB_CLIENT)
    add_subdirectory(of_smb_client)
    add_subdirectory(of_smb_fs)
    add_subdirectory(of_smb_browser)
  endif()
  if(OPENFILES_SMB_JNI)
    add_subdirectory(of_smb_jni)
  endif()
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "Open Files")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX YES)
    set(DOXYGEN_GENERATE_PDF YES)
    set(DOXYGEN_USE_PDFLATEX YES)
    set(DOXYGEN_PDF_HYPERLINKS YES)
    set(DOXYGEN_PROJECT_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/documentation/connected.png)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_FULL_SIDEBAR YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/documentation/headerFile)
    set(DOXYGEN_INTERNAL_DOCS NO)
    list(APPEND DOXYGEN_HTML_EXTRA_FILES ${CMAKE_CURRENT_SOURCE_DIR}/documentation/connected.ico)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.openfiles)
    set(DOXYGEN_EXAMPLE_PATH ${of_core_SOURCE_DIR}/test)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation)
    set(DOXYGEN_EXCLUDE_PATTERNS
	*/file_call.h
	*/net_internal.h
	*/fs.h
	*/fstypes.h
	*/impl/*
	*.c
	README.md
	)

if (OFC_INCLUDE_JNI)
   set(JNI_FILES
       of_core_jni/java
       of_core_jni/documentation/openfiles_jni.h
       )
    list(APPEND DOXYGEN_EXAMPLE_PATH ${of_core_jni_SOURCE_DIR}/test)
endif()

    doxygen_add_docs(
        openfiles
	${CMAKE_CURRENT_SOURCE_DIR}/documentation/openfiles.h
	${CMAKE_CURRENT_SOURCE_DIR}/documentation/config.h
        of_core
	${JNI_FILES}
	)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_target(
            doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
            )
    endif()
endif()

