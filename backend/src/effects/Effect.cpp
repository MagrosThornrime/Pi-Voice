#include <effects/Effect.hpp>
#include <effects/ChorusEffect.hpp>
#include <effects/ReverbEffect.hpp>
#include <effects/DelayEffect.hpp>
#include <effects/RobotifyEffect.hpp>

namespace effects {

std::shared_ptr<Effect> Effect::create(EffectType::Value effect) {
    std::shared_ptr<Effect> result = nullptr;
    switch (effect) {
    case EffectType::chorus: result = std::make_shared<ChorusEffect>(); break;
    case EffectType::delay: result = std::make_shared<DelayEffect>(); break;
    case EffectType::reverb: result = std::make_shared<ReverbEffect>(); break;
    case EffectType::robotify: result = std::make_shared<RobotifyEffect>(); break;
    }
    if (result) {
        result->refresh();
    }

    return result;
}

Effect::Effect() {
    refresh();
}

}