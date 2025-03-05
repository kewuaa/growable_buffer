#include <utility>
#include <cassert>

#include <spdlog/spdlog.h>

#include "growable_buffer.hpp"


GrowableBuffer::View::View(GrowableBuffer& buf, size_t begin, size_t size) noexcept:
    _buf(buf),
    _begin(begin),
    _size(size)
{
    //
}


void GrowableBuffer::_swap_to_begin() noexcept {
    auto nbytes = readable_bytes();
    std::memcpy(
        _buffer.data(),
        _buffer.data() + _read_idx,
        nbytes
    );
    _read_idx = 0;
    _write_idx = nbytes;
}


void GrowableBuffer::_make_available(size_t nbytes) noexcept {
    auto diff = nbytes - writable_bytes();
    if (diff <= 0) {
        return;
    }
    if (diff <= _prependable_bytes()) {
        _swap_to_begin();
        return;
    }
    _buffer.resize(_buffer.size() + (diff < 15 ? 15 : diff));
}


GrowableBuffer::GrowableBuffer(size_t buffer_size) noexcept:
    _buffer(buffer_size, 0)
{
    //
}


GrowableBuffer::GrowableBuffer(GrowableBuffer&& buffer) noexcept:
    _buffer(std::move(buffer._buffer)),
    _read_idx(std::exchange(buffer._read_idx, 0)),
    _write_idx(std::exchange(buffer._write_idx, 0))
{
    //
}


GrowableBuffer& GrowableBuffer::operator=(GrowableBuffer&& buffer) noexcept {
    _buffer = std::move(buffer._buffer);
    _read_idx = std::exchange(buffer._read_idx, 0);
    _write_idx = std::exchange(buffer._write_idx, 0);
    return *this;
}


std::string_view GrowableBuffer::read(size_t nbytes) noexcept {
    assert(readable_bytes() >= nbytes && "no enough data to load");
    std::string_view res { _buffer.data() + _read_idx, nbytes };
    _read_idx += nbytes;
    if (_read_idx == _write_idx) {
        _read_idx = _write_idx = 0;
    }
    return res;
}


std::string_view GrowableBuffer::read_all() noexcept {
    std::string_view res { _buffer.data() + _read_idx, readable_bytes() };
    _read_idx = _write_idx = 0;
    return res;
}


GrowableBuffer::View GrowableBuffer::malloc(size_t nbytes) noexcept {
    _make_available(nbytes);
    View res = { *this, _write_idx, nbytes };
    _write_idx += nbytes;
    return res;
}


void GrowableBuffer::free(char* data) noexcept {
    assert(_buffer.data() + _write_idx - data >= 0);
    _write_idx = data - _buffer.data();
}


void GrowableBuffer::write(char byte) noexcept {
    _make_available(1);
    _buffer[_write_idx] = byte;
    _write_idx++;
}


GrowableBuffer::View GrowableBuffer::write(std::string_view data) noexcept {
    if (data.empty()) {
        SPDLOG_WARN("try to write empty data");
        return { *this, _write_idx, 0 };
    }
    auto view = this->malloc(data.size());
    std::copy(data.begin(), data.end(), view.data());
    return view;
}
