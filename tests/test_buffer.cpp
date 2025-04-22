#include <boost/ut.hpp>

#include "growable_buffer.hpp"
using namespace boost::ut;


int main() {
    "buffer"_test = [] {
        GrowableBuffer buf { 1024 };
        expect(buf.size() == 1024);
        auto view = buf.malloc(128);
        expect(view.size() == 128);
        std::memset(view.data(), 0, 64 * sizeof(char));
        buf.backup(64);
        expect(buf.readable_bytes() == 64);
        expect(buf.writable_bytes() == 64);
        auto _ = buf.read(32);
        expect(buf.readable_bytes() == 32);
        expect(buf.writable_bytes() == 64);
    };
}
