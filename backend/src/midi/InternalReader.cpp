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

	auto&& dataVec = *msg;

	msg->resize(3);

	auto data = Data{
		.time = timestamp,
		.bytes = {dataVec[0], dataVec[1], dataVec[2]}
	};

	bool noCallback = true;
	auto&& eventCallback = self._callbacks[_eventIdx(data.status())];
	if (self._generalCallback) {
		self._generalCallback(data);
		noCallback = false;
	}
	if (eventCallback) {
		eventCallback(data);
		noCallback = false;
	}
	if (noCallback) {
		self._data.push_back(data);
	}
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
	resetCallbacks();
	if (opened()) {
		_in.cancelCallback();
		_in.closePort();
	}
}

Data InternalReader::read() {
	if (_data.empty()) {
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

void InternalReader::setEventCallback(const Data::Status event, std::function<void(Data)> callback) noexcept {
	if (opened()) _callbacks[_eventIdx(event)] = std::move(callback);
}

void InternalReader::setGeneralCallback(std::function<void(Data)> callback) noexcept {
	if (opened()) _generalCallback = std::move(callback);
}

void InternalReader::resetEventCallback(const Data::Status event) noexcept {
	_callbacks[_eventIdx(event)] = {};
}

void InternalReader::resetEventCallbacks() noexcept {
	for (auto&& cb : _callbacks) {
		cb = {};
	}
}

void InternalReader::resetGeneralCallback() noexcept {
	_generalCallback = {};
}

void InternalReader::resetCallbacks() noexcept {
	resetEventCallbacks();
	resetGeneralCallback();
}

bool InternalReader::opened() const noexcept {
	return _in.isPortOpen();
}

InternalReader::operator bool() const noexcept {
	return opened();
}

void InternalReader::_assure() const {
	if (not opened()) {
		throw std::runtime_error("Reader was not opened");
	}
}

u32 InternalReader::_eventIdx(const Data::Status event) noexcept {
	return (event - Data::_offset) >> 4;
}

std::tuple<std::unique_lock<std::mutex>, InternalReader&> InternalReader::_locked() noexcept {
	return std::tuple_cat(std::tuple(std::unique_lock(_mutex)), std::tie(*this));
}

std::tuple<std::unique_lock<std::mutex>, const InternalReader&> InternalReader::_locked() const noexcept {
	return std::tuple_cat(std::tuple(std::unique_lock(_mutex)), std::tie(*this));
}

}