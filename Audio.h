#pragma once

enum class SoundId {
    HONK,
    COLLISION,
    COIN,
    NITRO,
    CLOCK,
    SAYES1,
    SAYES2,
    POLICE_WHISTLE
};

void initAudio();
void playSound(SoundId id);
