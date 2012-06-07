CMAKE_MINIMUM_REQUIRED(VERSION 1.4)

FIND_PACKAGE(LibXml2 REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(FLTK REQUIRED)

SET(VSP_RENDERERS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to vsp_renderers headers")

INCLUDE_DIRECTORIES( ${UTIL_CODE_INCLUDE_DIR}
    ${XMLVSP_INCLUDE_DIR}
    ${VSP_SOURCE_DIR}/vsp_common
    ${LIBXML2_INCLUDE_DIR}
    ${LOAD_JPG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${FLTK_INCLUDE_DIR}
)

ADD_LIBRARY(vsp_renderers
    geom_renderer.cpp
    cfd_mesh_renderer.cpp
    fea_mesh_renderer.cpp
    grid_density_renderer.cpp
    polygon_mesh_renderer.cpp
    material.cpp
    texture.cpp
    tri_mesh_renderer.cpp
)

TARGET_LINK_LIBRARIES(vsp_renderers
    util_code
    xmlvsp
)