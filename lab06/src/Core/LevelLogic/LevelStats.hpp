#pragma once

struct LevelStats {
    float timeSpent = 0.f;
    int dashesUsed = 0;
    int tilesBroken = 0;
    int hostsPossessed = 0;
    int syringesCollected = 0;

    void Reset() {
        timeSpent = 0.f;
        dashesUsed = 0;
        tilesBroken = 0;
        hostsPossessed = 0;
        syringesCollected = 0;
    }
};