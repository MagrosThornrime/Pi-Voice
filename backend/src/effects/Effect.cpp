#include <effects/Effect.hpp>
#include <effects/ChorusEffect.hpp>
#include <effects/ReverbEffect.hpp>
#include <effects/DelayEffect.hpp>
#include <effects/RobotifyEffect.hpp>
#include <effects/PhaserEffect.hpp>
#include <effects/SaturateEffect.hpp>
#include <effects/BitcrushEffect.hpp>

namespace effects {

std::shared_ptr<Effect> Effect::create(EffectType::Value effect, u32 channels, f32 sampleRate) {
    std::shared_ptr<Effect> result = nullptr;
    switch (effect) {
        case EffectType::chorus: result = std::make_shared<ChorusEffect>(channels, sampleRate); break;
        case EffectType::delay: result = std::make_shared<DelayEffect>(channels, sampleRate); break;
        case EffectType::reverb: result = std::make_shared<ReverbEffect>(channels, sampleRate); break;
        case EffectType::robotify: result = std::make_shared<RobotifyEffect>(channels, sampleRate); break;
        case EffectType::phaser: result = std::make_shared<PhaserEffect>(channels, sampleRate); break;
        case EffectType::saturate: result = std::make_shared<SaturateEffect>(channels, sampleRate); break;
        case EffectType::bitcrush: result = std::make_shared<BitcrushEffect>(channels, sampleRate); break;
    }
    if (result) {
        result->refresh();
    }

    return result;
}

Effect::Effect(const u32 channels, const f32 sampleRate) : Layer(channels, sampleRate) {
    refresh();
}

}