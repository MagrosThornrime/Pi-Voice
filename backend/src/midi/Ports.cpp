#include <midi/Ports.hpp>
#include <stdexcept>

namespace midi {

std::mutex Ports::_mutex;
std::unordered_map<std::string_view, u32> Ports::_numbers;
std::vector<std::string> Ports::_names;

std::unique_lock<std::mutex> Ports::lock() noexcept {
    return std::unique_lock(_mutex);
}

void Ports::refresh() noexcept {
    RtMidiIn rtMidiIn;
    const u32 portCount = rtMidiIn.getPortCount();

    _names.clear();
    for(u32 i = 0; i != portCount; ++i) {
        _names.push_back(rtMidiIn.getPortName(i));
        _numbers[_names.back()] = i;
    }
}

std::vector<Port> Ports::list() noexcept {
    refresh();

    std::vector<Port> result;
    for(u32 i = 0; i != _names.size(); ++i) {
        result.push_back(Port{.name = _names[i], .num = i});
    }
    return result;
}

u32 Ports::count() noexcept {
    refresh();

    return _names.size();
}

Port Ports::getByName(std::string_view name) {
    refresh();

    auto found = _numbers.find(name);
    if(found == _numbers.end()) {
        return Port{.name = name, .num = (u32)-1};
    }

    return Port{.name = name, .num = found->second};
}

Port Ports::get(std::string_view name) {
    return getByName(name);
}

Port Ports::getByNum(const u32 num) {
    refresh();

    if(num >= _names.size()) {
        return Port{.name = {}, .num = num};
    }

    return Port{.name = _names[num], .num = num};
}

Port Ports::get(const u32 num) {
    return getByNum(num);
}

}