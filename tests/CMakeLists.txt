add_executable(test_buffer)
target_sources(
    test_buffer
    PRIVATE test_buffer.cpp
)
target_link_libraries(
    test_buffer
    PRIVATE
        ${PROJECT_NAME}
        Boost::ut
        fmt
)
add_test(NAME test_buffer COMMAND $<TARGET_FILE:test_buffer>)
