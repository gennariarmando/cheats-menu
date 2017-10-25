#pragma once

enum ePageID {
    CHEATS,
    WEAPONS,
    WANTED,
    TIME,
    PED,
    VEHICLE,
    SPAWN_VEHICLE,
    PLAYER,
    TRAFFIC,
    GANG,
    MISC
};

enum eFontStyle : unsigned __int8 {
    FONT_RAGE,
    FONT_SUBTITLES,
    FONT_PRICEDOWN
};

class CMenuSystem {
public:
    int lineID;
    int pageID;
    bool bMenu0;
    bool bMenu1;
    bool bMenu2;
    bool bMenu3;
    bool bMenu4;
    bool bMenu5;
    bool bMenu6;
    bool bMenu7;
    bool bMenu8;
    bool bMenu9;
    bool bMenu10;
    bool bEnableCoords;
    bool bEnableDebugMenu;
    bool bEnableTimeStopper;
    bool bEnableSetTime;

public:
    static void InjectPatches();
    static void DisplayMenu();

public:
    static void DrawSelectedMenuItem(int slotId);
    static void AddString(wchar_t * nString, int nLine);
    static void DrawPermanentText();
    static void AirbreakMode();
    static void SetTime();
};

extern CMenuSystem MenuSystem;