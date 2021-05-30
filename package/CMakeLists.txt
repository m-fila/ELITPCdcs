set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_FILE_NAME
    ${CPACK_PACKAGE_NAME}-v${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "mfila")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE
    "https://dracula.hep.fuw.edu.pl:8888/mfila/dcs")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Detector Control System for ELITPC")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ELITPC dcs")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/postinst.in
               ${CMAKE_CURRENT_BINARY_DIR}/postinst @ONLY)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/conffiles;${CMAKE_CURRENT_BINARY_DIR}/postinst;${CMAKE_CURRENT_SOURCE_DIR}/postrm"
)
if(BUILD_SERVER)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dcs-server.service.in
                 ${CMAKE_CURRENT_BINARY_DIR}/dcs-server.service @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dcs-server.service
          DESTINATION scripts)
endif(BUILD_SERVER)
configure_file(${PROJECT_SOURCE_DIR}/scripts/gitlab/deploy-production.sh.in
               ${PROJECT_SOURCE_DIR}/scripts/gitlab/deploy-production.sh @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/scripts/gitlab/release-assets.sh.in
               ${PROJECT_SOURCE_DIR}/scripts/gitlab/release-assets.sh @ONLY)

include(CPack)
