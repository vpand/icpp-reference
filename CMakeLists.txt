if (UNIX)
  add_link_options(-Wl,-S, -Wl,-x)
endif()

set(LEXBOR_BUILD_STATIC OFF)
add_subdirectory(third/lexbor)
