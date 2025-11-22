#pragma once

#include <Portaudio.hpp>
#include <any>
#include <Types.hpp>

namespace pipeline {

class Layer: public pa::CallbackInterface {
public:
	Layer() = default;

	virtual Layer& setParam(const u32 param, std::any value) = 0;
	virtual std::any getParam(const u32 param) = 0;

	virtual void refresh();
};

}