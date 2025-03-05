ADD_EXECUTABLE(test_buffer)
TARGET_SOURCES(
    test_buffer
    PRIVATE test_buffer.cpp
)
TARGET_LINK_LIBRARIES(
    test_buffer
    PRIVATE
        ${PROJECT_NAME}
        Boost::ut
        fmt
)
ADD_TEST(NAME test_buffer COMMAND $<TARGET_FILE:test_buffer>)
