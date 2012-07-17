CMAKE_MINIMUM_REQUIRED(VERSION 1.4)

PROJECT (VSP)

SET(CMAKE_PREFIX_PATH
      "C:\OpenVSP Libraries\fltk"
      "C:\OpenVSP Libraries\libxml2"
      "C:\OpenVSP Libraries\jpeg-8d"
      "C:\OpenVSP Libraries\jpeg-8d\lib"
)

ADD_SUBDIRECTORY( util_code )
ADD_SUBDIRECTORY( triangle )
ADD_SUBDIRECTORY( load_jpg )
ADD_SUBDIRECTORY( xmlvsp )
ADD_SUBDIRECTORY( screens )
ADD_SUBDIRECTORY( sixseries )
ADD_SUBDIRECTORY( opengl_render_library )
ADD_SUBDIRECTORY( vsp )
