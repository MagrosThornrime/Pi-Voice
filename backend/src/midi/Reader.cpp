#include <midi/Reader.hpp>

namespace midi {

Reader::Reader(const Port port):
	_reader(port) {}

void Reader::open(const Port port) {
	auto&& [lock, reader] = locked();
	reader.open(port);
}

void Reader::close() noexcept {
	auto&& [lock, reader] = locked();
	reader.close();
}

Data Reader::read() {
	auto&& [lock, reader] = locked();
	return reader.read();
}

std::vector<Data> Reader::readSome(const u32 n) {
	auto&& [lock, reader] = locked();
	return reader.readSome(n);
}

std::vector<Data> Reader::readAll() {
	auto&& [lock, reader] = locked();
	return reader.readAll();
}

u32 Reader::queueSize() const {
	auto&& [lock, reader] = locked();
	return reader.queueSize();
}

void Reader::clear() noexcept {
	auto&& [lock, reader] = locked();
	reader.clear();
}

void Reader::setEventCallback(const Data::Status event, std::function<void(Data)> callback) noexcept {
	auto&& [lock, reader] = locked();
	reader.setEventCallback(event, callback);
}

void Reader::setGeneralCallback(std::function<void(Data)> callback) noexcept {
	auto&& [lock, reader] = locked();
	reader.setGeneralCallback(callback);
}

void Reader::resetEventCallback(const Data::Status event) noexcept {
	auto&& [lock, reader] = locked();
	reader.resetEventCallback(event);
}

void Reader::resetEventCallbacks() noexcept {
	auto&& [lock, reader] = locked();
	reader.resetEventCallbacks();
}

void Reader::resetGeneralCallback() noexcept {
	auto&& [lock, reader] = locked();
	reader.resetGeneralCallback();
}

void Reader::resetCallbacks() noexcept {
	auto&& [lock, reader] = locked();
	reader.resetCallbacks();
}

bool Reader::opened() const noexcept {
	auto&& [lock, reader] = locked();
	return reader.opened();
}

Reader::operator bool() const noexcept {
	auto&& [lock, reader] = locked();
	return (bool)reader;
}

std::tuple<std::unique_lock<std::mutex>, InternalReader&> Reader::locked() noexcept {
	return _reader._locked();
}

std::tuple<std::unique_lock<std::mutex>, const InternalReader&> Reader::locked() const noexcept {
	return _reader._locked();
}

}