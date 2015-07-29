message(VARIABLES " =${yahtml-parser_INCLUDES} ${yahtml-parser_LIBS}")

include_directories(${yahtml-parser_INCLUDES})

add_executable(parser_test parser_test.cc)
add_executable(dom_test dom_test.cc)

target_link_libraries(parser_test gtest gtest_main ${yahtml-parser_LIBS})
target_link_libraries(dom_test gtest gtest_main ${yahtml-parser_LIBS})

add_test(NAME parser_test COMMAND parser_test)
add_test(NAME dom_test COMMAND dom_test)

add_custom_command(
  TARGET parser_test PRE_BUILD
  COMMAND ${CMAKE_COMMAND}
  -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:parser_test>/assets
)

