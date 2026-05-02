include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

set(INSTALL_RBC_HOME "${CMAKE_INSTALL_INCLUDEDIR}/rbc")
set(CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/RbCConfig.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/RbCConfigVersion.cmake")
set(CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/RbC")

configure_package_config_file(
    "${RBC_HOME}/RbCConfig.cmake.in"
    "${CONFIG_FILE}"
    INSTALL_DESTINATION "${CONFIG_DESTINATION}")

write_basic_package_version_file("${CONFIG_VERSION_FILE}"
    COMPATIBILITY AnyNewerVersion)

install(TARGETS ${LIB}
    EXPORT RbCTargets
    INCLUDES DESTINATION "${INSTALL_RBC_HOME}")

install(EXPORT RbCTargets
    NAMESPACE RbC::
    DESTINATION "${CONFIG_DESTINATION}")

install(FILES "${CONFIG_FILE}" "${CONFIG_VERSION_FILE}"
    DESTINATION "${CONFIG_DESTINATION}")

install(FILES "${VERSION_FILE}" "${FEATURE_FILE}"
    DESTINATION "${INSTALL_RBC_HOME}/rb")

# setting the PUBLIC_HEADER property does not preserve the folder structure
# so instead we don't use it, iterate through public headers and install them manually
foreach(HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER_FILE_PATH "${RBC_HOME}" "${HEADER}")
    get_filename_component(HEADER_DIRECTORY_PATH "${HEADER_FILE_PATH}" DIRECTORY)
    install(FILES ${HEADER}
        DESTINATION "${INSTALL_RBC_HOME}/${HEADER_DIRECTORY_PATH}")
endforeach()
