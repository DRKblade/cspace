function(get_include_dirs output)
  get_filename_component(generated_headers_dir ${CMAKE_BINARY_DIR}/generated-headers ABSOLUTE)
  get_filename_component(public_headers_dir ${CMAKE_SOURCE_DIR}/include ABSOLUTE)
  get_filename_component(private_headers_dir ${CMAKE_SOURCE_DIR}/private-headers ABSOLUTE)
  set(${output} ${public_headers_dir} ${generated_headers_dir} ${private_headers_dir} PARENT_SCOPE)
endfunction()

function(get_sources_dir output)
  get_filename_component(src_dir ${CMAKE_SOURCE_DIR}/src ABSOLUTE)
  set(${output} ${src_dir} PARENT_SCOPE)
endfunction()
