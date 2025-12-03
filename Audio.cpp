#include "Audio.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void initAudio() {
    // Nothing required here for WAV + PlaySound
}

void playSound(SoundId id) {
    const char* file = nullptr;

    switch (id) {
    case SoundId::HONK:            file = "Sounds/HONK.wav"; break;
    case SoundId::COLLISION:       file = "Sounds/COLLISION.wav"; break;
    case SoundId::COIN:            file = "Sounds/COIN.wav"; break;
    case SoundId::NITRO:           file = "Sounds/NITRO.wav"; break;
    case SoundId::CLOCK:           file = "Sounds/CLOCK.wav"; break;
    case SoundId::SAYES1:          file = "Sounds/SAYES1.wav"; break;
    }

    if (file) {
        PlaySoundA(file, NULL, SND_FILENAME | SND_ASYNC);
    }
}

