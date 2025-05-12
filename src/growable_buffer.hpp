#pragma once
#include <vector>
#include <format>
#include <cstring>
#include <cstddef>

#include "growable_buffer_export.hpp"
#include "growable_buffer_config.hpp"


class GROWABLE_BUFFER_EXPORT GrowableBuffer {
    private:
        std::vector<char> _buffer;
        size_t _read_idx { 0 };
        size_t _write_idx { 0 };

        void _swap_to_begin() noexcept;
        void _make_available(size_t n) noexcept;

        inline size_t _prependable_bytes() const noexcept {
            return _read_idx;
        }
    public:
        class View {
            friend GrowableBuffer;
            private:
                GrowableBuffer& _buf;
                size_t _begin;
                size_t _size;
                View(GrowableBuffer& buf, size_t begin, size_t size) noexcept;
            public:
                View() = delete;
                View& operator=(View&) = delete;
                View& operator=(View&&) = delete;
                View(View&) noexcept = default;
                View(View&&) noexcept = default;
                inline char* data() const noexcept {
                    return _buf._buffer.data() + _begin;
                }

                inline size_t size() const noexcept {
                    return _size;
                }
        };

        GrowableBuffer(size_t buffer_size = GROWABLE_BUFFER_DEFAULT_SIZE) noexcept;
        GrowableBuffer(GrowableBuffer&) = default;
        GrowableBuffer& operator=(GrowableBuffer&) = default;
        GrowableBuffer(GrowableBuffer&&) noexcept;
        GrowableBuffer& operator=(GrowableBuffer&& buffer) noexcept;
        [[nodiscard]] std::string_view read(size_t nbytes) noexcept;
        [[nodiscard]] std::string_view read_all() noexcept;
        [[nodiscard]] View malloc(size_t nbytes) noexcept;
        void backup(size_t size) noexcept;
        void write(char byte) noexcept;
        GrowableBuffer::View write(std::string_view data) noexcept;

        inline auto write(const char* data, size_t size) noexcept {
            return write({ data, size });
        }

        template<typename... Args>
        auto write(std::format_string<Args...> fmt, Args&&... args) noexcept {
            return write(std::format(fmt, std::forward<Args>(args)...));
        }

        inline size_t readable_bytes() const noexcept {
            return _write_idx - _read_idx;
        }

        inline size_t writable_bytes() const noexcept {
            return _buffer.size() - _write_idx;
        }

        inline size_t size() const noexcept {
            return _buffer.size();
        }
};
