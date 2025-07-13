#include <VoiceManager.hpp>

VoiceManager::VoiceManager(int voicesNumber){
    for(i32 i = 0; i < voicesNumber; i++){
        _voices.emplace_back({i, 1.0f / voicesNumber});
    }
}