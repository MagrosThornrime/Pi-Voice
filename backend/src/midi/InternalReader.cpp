#include <midi/InternalReader.hpp>
#include <range/v3/all.hpp>
#include <midi/Reader.hpp>

namespace midi {

InternalReader::InternalReader(const Port port) {
    open(port);
}

InternalReader::~InternalReader() noexcept {
    close();
}

void InternalReader::receive(double timestamp, std::vector<u8>* msg, void* myData) {
	auto&& [lock, self] = ((InternalReader*)myData)->_locked();
    self._assure();

	auto&& data = *msg;

	msg->resize(3);
	self._data.push_back(Data{
		.time = timestamp,
		.bytes = {
			data[0], data[1], data[2]
		}
	});

	if(self._callback) self._callback(self);
}

void InternalReader::open(const Port port) {
    port.assure();
	// close if opened
	close();

    _in.openPort(port.num);
    _in.ignoreTypes(false, false, false);
    _in.setCallback(receive, this);
}

void InternalReader::close() noexcept {
    clear();
	resetCallback();
    if(opened()) {
		_in.cancelCallback();
        _in.closePort();
    }
}

Data InternalReader::read() {
    if(_data.empty()) {
        return Data();
    }

    auto result = _data.front();
    _data.pop_front();
    return result;
}

std::vector<Data> InternalReader::readSome(const u32 n) {
    auto result = _data
        | ranges::views::take(n)
        | ranges::to<std::vector>;
    _data.erase(_data.begin(), _data.begin() + n);

    return result;
}

std::vector<Data> InternalReader::readAll() {
    auto result = _data
        | ranges::to<std::vector>;
    _data.clear();

    return result;
}

u32 InternalReader::queueSize() const {
    _assure();
    return _data.size();
}

void InternalReader::clear() noexcept {
    _data.clear();
}

void InternalReader::setCallback(std::function<void(InternalReader&)> callback) noexcept {
	if(opened()) _callback = std::move(callback);
}

void InternalReader::resetCallback() noexcept {
	_callback = {};
}

bool InternalReader::opened() const noexcept {
    return _in.isPortOpen();
}

InternalReader::operator bool() const noexcept {
    return opened();
}

void InternalReader::_assure() const {
    if(not opened()){
        throw std::runtime_error("Reader was not opened");
    }
}

std::tuple<std::unique_lock<std::mutex>, InternalReader&> InternalReader::_locked() noexcept {
	return std::tuple_cat(std::tuple(std::unique_lock(_mutex)), std::tie(*this));
}

std::tuple<std::unique_lock<std::mutex>, const InternalReader&> InternalReader::_locked() const noexcept {
	return std::tuple_cat(std::tuple(std::unique_lock(_mutex)), std::tie(*this));
}

}