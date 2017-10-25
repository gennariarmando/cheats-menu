#include <plugin_vc.h>
#include "game_vc\common.h"
#include "game_vc\CFont.h"
#include "game_vc\CMenuManager.h"
#include "game_vc\CHud.h"
#include "game_vc\CCivilianPed.h"
#include "game_vc\CPopulation.h"
#include "game_vc\CSprite2d.h"
#include "game_vc\CStreaming.h"
#include "game_vc\CTimer.h"
#include "game_vc\CWorld.h"
#include "CMenuSystem.h"

CMenuSystem MenuSystem;

auto PlayFrontEndSound = (char(__stdcall *)(__int16, float))0x5F9960;

int nTimeMenu = 0;
int nTimeMenuItem = 0;
int nActiveMenuItem = 0;

CRGBA selectedMenuItem = { 255, 149, 223, 255 };
CRGBA unselectedMenuItem = { 205, 205, 205, 255 };

wchar_t *pText0, *pText1, *pText2, *pText3, *pText4, *pText5, *pText6, *pText7, *pText8, *pText9, *pText10;

void CMenuSystem::InjectPatches() {
    plugin::Events::drawHudEvent += [] {
        CMenuSystem::DisplayMenu;
        CMenuSystem::DrawPermanentText;
        CMenuSystem::AirbreakMode;
        CMenuSystem::SetTime;
    };
}

void CMenuSystem::DisplayMenu() {
    CPed *player = FindPlayerPed();
    CPad *pad = CPad::GetPad(0);

    // ID: 0 - 10
    wchar_t* Header[11]{ L"Cheats", L"Weapons", L"Weather", L"Wanted Level", L"Time Cheats", L"Ped Cheats", L"Vehicle Cheats", L"Spawn Vehicles", L"Player Cheats", L"Gang Cheats", L"Miscellaneous" };

    // ID: 0 - 9 Where 0 is first menu option.
    wchar_t* Page0Lines[10]{ L"Weapons", L"Weather", L"Wanted Level", L"Time Cheats", L"Ped Cheats", L"Vehicle Cheats", L"Spawn Vehicles", L"Player Cheats", L"Gang Cheats", L"Miscellaneous" };

    // ID: 1 - 9 Where 1 is first menu option.
    wchar_t* Page1Lines[10]{ L"Get Weapon Set 1", L"Get Weapon Set 2", L"Get Weapon Set 3", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused"};
    wchar_t* Page3Lines[10]{ L"Faster", L"Normal", L"Slower", L"Time Stopper", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused" };
    wchar_t* Page9Lines[10]{ L"Show player coords", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused", L"Unused" };

    char EnableKey = plugin::KeyPressed(VK_F4);
    char DisableKey = plugin::KeyPressed(VK_RETURN);

    if (FrontendMenuManager.m_bMenuVisible)
        MenuSystem.bEnableDebugMenu = 0;

    if (!MenuSystem.bEnableDebugMenu && EnableKey && !MenuSystem.bEnableTimeStopper && CTimer::m_snTimeInMilliseconds > (nTimeMenu + 1000)) {
        MenuSystem.bEnableDebugMenu = 1;
        nTimeMenu = CTimer::m_snTimeInMilliseconds;
    }
    else if (MenuSystem.bEnableDebugMenu && DisableKey && CTimer::m_snTimeInMilliseconds > (nTimeMenu + 1000)) {
        if (MenuSystem.bMenu0)
            MenuSystem.bEnableDebugMenu = 0;
        else {
            MenuSystem.lineID = 0;
            MenuSystem.pageID = CHEATS;

            MenuSystem.bMenu0 = 1;
            MenuSystem.bMenu1 = 0;
            MenuSystem.bMenu2 = 0;
            MenuSystem.bMenu3 = 0;
            MenuSystem.bMenu4 = 0;
            MenuSystem.bMenu5 = 0;
            MenuSystem.bMenu6 = 0;
            MenuSystem.bMenu7 = 0;
            MenuSystem.bMenu8 = 0;
            MenuSystem.bMenu9 = 0;
            MenuSystem.bMenu10 = 0;
            nTimeMenu = CTimer::m_snTimeInMilliseconds;
        }
    }

    if (!MenuSystem.bEnableDebugMenu) {
        MenuSystem.lineID = 0;
        MenuSystem.pageID = CHEATS;

        MenuSystem.bMenu0 = true;
        MenuSystem.bMenu1 = false;
        MenuSystem.bMenu2 = false;
        MenuSystem.bMenu3 = false;
        MenuSystem.bMenu4 = false;
        MenuSystem.bMenu5 = false;
        MenuSystem.bMenu6 = false;
        MenuSystem.bMenu7 = false;
        MenuSystem.bMenu8 = false;
        MenuSystem.bMenu9 = false;
        MenuSystem.bMenu10 = false;
    }
    else {
        // Window
        CSprite2d::DrawRect(CRect(SCREEN_COORD_CENTER_X - SCREEN_COORD(512 / 2), SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2), SCREEN_COORD_CENTER_X - SCREEN_COORD(512 / 2) + SCREEN_COORD(512), SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2) + SCREEN_COORD(512)), CRGBA(0, 0, 0, 205));

        // Header
        CFont::SetPropOn();
        CFont::SetRightJustifyOff();
        CFont::SetCentreOff();
        CFont::SetBackgroundOff();
        CFont::SetJustifyOn();
        CFont::SetFontStyle(FONT_PRICEDOWN);
        CFont::SetScale(SCREEN_MULTIPLIER(1.89f), SCREEN_MULTIPLIER(3.89f));
        CFont::SetColor(CRGBA(0, 0, 0, 255));
        CFont::SetDropColor(CRGBA(0, 0, 0, 255));
        CFont::SetDropShadowPosition(1);
        // White
        CFont::SetScale(SCREEN_MULTIPLIER(1.78f), SCREEN_MULTIPLIER(3.56f));
        CFont::SetColor(CRGBA(selectedMenuItem));
        CFont::PrintString(SCREEN_COORD_CENTER_X - SCREEN_COORD(512 / 2 - 20.0f), SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 + 34.0f), Header[MenuSystem.pageID]);

        // Set selected menu item
        {
            {
                if (MenuSystem.lineID == 0)
                    DrawSelectedMenuItem(0);
                else if (MenuSystem.lineID == 1)
                    DrawSelectedMenuItem(1);
                else if (MenuSystem.lineID == 2)
                    DrawSelectedMenuItem(2);
                else if (MenuSystem.lineID == 3)
                    DrawSelectedMenuItem(3);
                else if (MenuSystem.lineID == 4)
                    DrawSelectedMenuItem(4);
                else if (MenuSystem.lineID == 5)
                    DrawSelectedMenuItem(5);
                else if (MenuSystem.lineID == 6)
                    DrawSelectedMenuItem(6);
                else if (MenuSystem.lineID == 7)
                    DrawSelectedMenuItem(7);
                else if (MenuSystem.lineID == 8)
                    DrawSelectedMenuItem(8);
                else if (MenuSystem.lineID == 9)
                    DrawSelectedMenuItem(9);
            }

            char UpKey;
            char DownKey;
            UpKey = plugin::KeyPressed(0x26);
            DownKey = plugin::KeyPressed(0x28);

            if (DownKey && CTimer::m_snTimeInMilliseconds > (nTimeMenuItem + 100)) {
                if (MenuSystem.lineID == 0)
                    DrawSelectedMenuItem(1);
                else if (MenuSystem.lineID == 1)
                    DrawSelectedMenuItem(2);
                else if (MenuSystem.lineID == 2)
                    DrawSelectedMenuItem(3);
                else if (MenuSystem.lineID == 3)
                    DrawSelectedMenuItem(4);
                else if (MenuSystem.lineID == 4)
                    DrawSelectedMenuItem(5);
                else if (MenuSystem.lineID == 5)
                    DrawSelectedMenuItem(6);
                else if (MenuSystem.lineID == 6)
                    DrawSelectedMenuItem(7);
                else if (MenuSystem.lineID == 7)
                    DrawSelectedMenuItem(8);
                else if (MenuSystem.lineID == 8)
                    DrawSelectedMenuItem(9);
                else if (MenuSystem.lineID == 9)
                    DrawSelectedMenuItem(0);

                PlayFrontEndSound(196, 0.0f);
                nTimeMenuItem = CTimer::m_snTimeInMilliseconds;
            }

            if (UpKey && CTimer::m_snTimeInMilliseconds > (nTimeMenuItem + 100)) {
                if (MenuSystem.lineID == 0)
                    DrawSelectedMenuItem(9);
                else if (MenuSystem.lineID == 1)
                    DrawSelectedMenuItem(0);
                else if (MenuSystem.lineID == 2)
                    DrawSelectedMenuItem(1);
                else if (MenuSystem.lineID == 3)
                    DrawSelectedMenuItem(2);
                else if (MenuSystem.lineID == 4)
                    DrawSelectedMenuItem(3);
                else if (MenuSystem.lineID == 5)
                    DrawSelectedMenuItem(4);
                else if (MenuSystem.lineID == 6)
                    DrawSelectedMenuItem(5);
                else if (MenuSystem.lineID == 7)
                    DrawSelectedMenuItem(6);
                else if (MenuSystem.lineID == 8)
                    DrawSelectedMenuItem(7);
                else if (MenuSystem.lineID == 9)
                    DrawSelectedMenuItem(8);

                PlayFrontEndSound(196, 0.0f);
                nTimeMenuItem = CTimer::m_snTimeInMilliseconds;
            }
        }

        char ActiveKey;
        ActiveKey = plugin::KeyPressed(VK_SPACE) && CTimer::m_snTimeInMilliseconds > (nActiveMenuItem + 100);

        // Start Page 0
        if (MenuSystem.bMenu0) {
            pText0 = Page0Lines[0];
            pText1 = Page0Lines[1];
            pText2 = Page0Lines[2];
            pText3 = Page0Lines[3];
            pText4 = Page0Lines[4];
            pText5 = Page0Lines[5];
            pText6 = Page0Lines[6];
            pText7 = Page0Lines[7];
            pText8 = Page0Lines[8];
            pText9 = Page0Lines[9];

            // If Key Pressed
            {
                if (MenuSystem.lineID == 0 && ActiveKey) {
                    MenuSystem.lineID = 0;
                    MenuSystem.pageID = WEAPONS;

                    MenuSystem.bMenu0 = false;
                    MenuSystem.bMenu1 = true;
                    MenuSystem.bMenu2 = false;
                    MenuSystem.bMenu3 = false;
                    MenuSystem.bMenu4 = false;
                    MenuSystem.bMenu5 = false;
                    MenuSystem.bMenu6 = false;
                    MenuSystem.bMenu7 = false;
                    MenuSystem.bMenu8 = false;
                    MenuSystem.bMenu9 = false;
                    nActiveMenuItem = CTimer::m_snTimeInMilliseconds;
                }

                if (MenuSystem.lineID == 3 && ActiveKey) {
                    MenuSystem.lineID = 0;
                    MenuSystem.pageID = TIME;

                    MenuSystem.bMenu0 = false;
                    MenuSystem.bMenu1 = false;
                    MenuSystem.bMenu2 = false;
                    MenuSystem.bMenu3 = true;
                    MenuSystem.bMenu4 = false;
                    MenuSystem.bMenu5 = false;
                    MenuSystem.bMenu6 = false;
                    MenuSystem.bMenu7 = false;
                    MenuSystem.bMenu8 = false;
                    MenuSystem.bMenu9 = false;
                    nActiveMenuItem = CTimer::m_snTimeInMilliseconds;
                }

                if (MenuSystem.lineID == 9 && ActiveKey) {
                    MenuSystem.lineID = 0;
                    MenuSystem.pageID = MISC;

                    MenuSystem.bMenu0 = false;
                    MenuSystem.bMenu1 = false;
                    MenuSystem.bMenu2 = false;
                    MenuSystem.bMenu3 = false;
                    MenuSystem.bMenu4 = false;
                    MenuSystem.bMenu5 = false;
                    MenuSystem.bMenu6 = false;
                    MenuSystem.bMenu7 = false;
                    MenuSystem.bMenu8 = false;
                    MenuSystem.bMenu9 = true;
                    nActiveMenuItem = CTimer::m_snTimeInMilliseconds;
                }
            }
        }
        // End Page 0

        // Start Page 1
        else if (MenuSystem.bMenu1) {
            pText0 = Page1Lines[0];
            pText1 = Page1Lines[1];
            pText2 = Page1Lines[2];
            pText3 = Page1Lines[3];
            pText4 = Page1Lines[4];
            pText5 = Page1Lines[5];
            pText6 = Page1Lines[6];
            pText7 = Page1Lines[7];
            pText8 = Page1Lines[8];
            pText9 = Page1Lines[9];

            if (MenuSystem.lineID == 0 && ActiveKey) {
                CStreaming::RequestModel(259, 1);
                CStreaming::RequestModel(264, 1);
                CStreaming::RequestModel(272, 1);
                CStreaming::RequestModel(274, 1);
                CStreaming::RequestModel(277, 1);
                CStreaming::RequestModel(281, 1);
                CStreaming::RequestModel(276, 1);
                CStreaming::RequestModel(285, 1);
                CStreaming::RequestModel(288, 1);
                CStreaming::LoadAllRequestedModels(0, 0);
                FindPlayerPed()->GiveWeapon((eWeaponType)1, 10, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)6, 1, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)15, 10, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)17, 100, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)19, 50, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)22, 150, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)27, 120, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)28, 25, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)31, 200, 1);
                CStreaming::SetModelIsDeletable(259);
                CStreaming::SetModelIsDeletable(274);
                CStreaming::SetModelIsDeletable(281);
                CStreaming::SetModelIsDeletable(264);
                CStreaming::SetModelIsDeletable(272);
                CStreaming::SetModelIsDeletable(277);
                CStreaming::SetModelIsDeletable(276);
                CStreaming::SetModelIsDeletable(285);
                MenuSystem.bEnableDebugMenu = 0;
                CHud::SetHelpMessage((const GtaChar*)L"Weapon Set 1 received", 1, 0, 1);
            }

            if (MenuSystem.lineID == 1 && ActiveKey) {
                CStreaming::RequestModel(268, 1);
                CStreaming::RequestModel(270, 1);
                CStreaming::RequestModel(291, 1);
                CStreaming::RequestModel(275, 1);
                CStreaming::RequestModel(279, 1);
                CStreaming::RequestModel(283, 1);
                CStreaming::RequestModel(280, 1);
                CStreaming::RequestModel(286, 1);
                CStreaming::RequestModel(287, 1);
                CStreaming::LoadAllRequestedModels(0, 0);
                FindPlayerPed()->GiveWeapon((eWeaponType)10, 0, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)13, 10, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)18, 40, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)21, 25, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)24, 100, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)26, 150, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)29, 21, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)30, 5, 1);
                CStreaming::SetModelIsDeletable(268);
                CStreaming::SetModelIsDeletable(270);
                CStreaming::SetModelIsDeletable(291);
                CStreaming::SetModelIsDeletable(275);
                CStreaming::SetModelIsDeletable(279);
                CStreaming::SetModelIsDeletable(283);
                CStreaming::SetModelIsDeletable(280);
                MenuSystem.bEnableDebugMenu = 0;
                CHud::SetHelpMessage((const GtaChar*)L"Weapon Set 2 received", 1, 0, 1);
            }

            if (MenuSystem.lineID == 2 && ActiveKey) {
                CStreaming::RequestModel(269, 1);
                CStreaming::RequestModel(270, 1);
                CStreaming::RequestModel(275, 1);
                CStreaming::RequestModel(278, 1);
                CStreaming::RequestModel(284, 1);
                CStreaming::RequestModel(280, 1);
                CStreaming::RequestModel(286, 1);
                CStreaming::RequestModel(290, 1);
                CStreaming::RequestModel(294, 1);
                CStreaming::LoadAllRequestedModels(0, 0);
                FindPlayerPed()->GiveWeapon((eWeaponType)11, 0, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)12, 10, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)18, 40, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)20, 30, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)25, 100, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)26, 150, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)29, 21, 1);
                FindPlayerPed()->GiveWeapon((eWeaponType)33, 500, 1);
                CStreaming::SetModelIsDeletable(269);
                CStreaming::SetModelIsDeletable(270);
                CStreaming::SetModelIsDeletable(275);
                CStreaming::SetModelIsDeletable(278);
                CStreaming::SetModelIsDeletable(284);
                CStreaming::SetModelIsDeletable(280);
                CStreaming::SetModelIsDeletable(286);
                CStreaming::SetModelIsDeletable(290);
                MenuSystem.bEnableDebugMenu = 0;
                CHud::SetHelpMessage((const GtaChar*)L"Weapon Set 3 received", 1, 0, 1);
            }

        }
        // End Page 1

        // Start Page 3
        else if (MenuSystem.bMenu3) {
            pText0 = Page3Lines[0];
            pText1 = Page3Lines[1];
            pText2 = Page3Lines[2];
            pText3 = Page3Lines[3];
            pText4 = Page3Lines[4];
            pText5 = Page3Lines[5];
            pText6 = Page3Lines[6];
            pText7 = Page3Lines[7];
            pText8 = Page3Lines[8];
            pText9 = Page3Lines[9];

            //
            if (MenuSystem.lineID == 0 && ActiveKey) {
                MenuSystem.bEnableDebugMenu = 0;

                CHud::SetHelpMessage((const GtaChar*)L"Time Faster Enabled", 0, 0, 1);
            }
            else if (MenuSystem.lineID == 1 && ActiveKey) {
                MenuSystem.bEnableDebugMenu = 0;
                CTimer::ms_fTimeScale = 1.0f;
                CTimer::ms_fTimeScale = 1.5f;

                CHud::SetHelpMessage((const GtaChar*)L"Time Normal Enabled", 0, 0, 1);
            }
            else if (MenuSystem.lineID == 2 && ActiveKey) {
                MenuSystem.bEnableDebugMenu = 0;
                CTimer::ms_fTimeScale = 0.5f;

                CHud::SetHelpMessage((const GtaChar*)L"Time Slower Enabled", 0, 0, 1);

            }
            else if (MenuSystem.lineID == 3 && ActiveKey) {
                MenuSystem.bEnableTimeStopper = 1;
                MenuSystem.bEnableDebugMenu = 0;

                CHud::SetMessage((const GtaChar*)L"Timer Stopper Enabled");
            }
            /*else if (MenuSystem.lineID == 4 && ActiveKey) {
                MenuSystem.bEnableSetTime = 1;
                MenuSystem.bEnableDebugMenu = 0;

                CHud::SetHelpMessage((const GtaChar*)L"Use the LEFT and RIGHT ARROWS to change the hour, the UP and DOWN ARROWS, to set the minutes, and SHIFT to advance the day. Press RETURN when done.", 0, 1, 1);
            }*/
        }
        // End Page 3

        // Start Page 9
        else if (MenuSystem.bMenu9) {
            pText0 = Page9Lines[0];
            pText1 = Page9Lines[1];
            pText2 = Page9Lines[2];
            pText3 = Page9Lines[3];
            pText4 = Page9Lines[4];
            pText5 = Page9Lines[5];
            pText6 = Page9Lines[6];
            pText7 = Page9Lines[7];
            pText8 = Page9Lines[8];
            pText9 = Page9Lines[9];

            // Player Coords
            if (MenuSystem.lineID == 0 && ActiveKey) {
                if (MenuSystem.bEnableCoords) {
                    MenuSystem.bEnableCoords = 0;
                    MenuSystem.bEnableDebugMenu = 0;

                    CHud::SetHelpMessage((const GtaChar*)L"Player Coords disabled", 1, 0, 1);
                }
                else {
                    MenuSystem.bEnableCoords = 1;
                    MenuSystem.bEnableDebugMenu = 0;

                    CHud::SetHelpMessage((const GtaChar*)L"Player Coords enabled", 1, 0, 1);
                }
            }
        }
        // End Page 10

        // Draw String from temp.
        CFont::SetColor(CRGBA(unselectedMenuItem));
        AddString(pText0, 0);
        AddString(pText1, 1);
        AddString(pText2, 2);
        AddString(pText3, 3);
        AddString(pText4, 4);
        AddString(pText5, 5);
        AddString(pText6, 6);
        AddString(pText7, 7);
        AddString(pText8, 8);
        AddString(pText9, 9);

        if (MenuSystem.lineID == 0) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText0, 0);
        }
        else if (MenuSystem.lineID == 1) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText1, 1);
        }
        else if (MenuSystem.lineID == 2) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText2, 2);
        }
        else if (MenuSystem.lineID == 3) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText3, 3);
        }
        else if (MenuSystem.lineID == 4) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText4, 4);
        }
        else if (MenuSystem.lineID == 5) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText5, 5);
        }
        else if (MenuSystem.lineID == 6) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText6, 6);
        }
        else if (MenuSystem.lineID == 7) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText7, 7);
        }
        else if (MenuSystem.lineID == 8) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText8, 8);
        }
        else if (MenuSystem.lineID == 9) {
            CFont::SetColor(selectedMenuItem);
            AddString(pText9, 9);
        }
    }
}

void CMenuSystem::DrawPermanentText() {
    wchar_t gString[256];
    float angle = FindPlayerHeading();
    CVector& coords = FindPlayerPed()->GetPosition();


    swprintf(gString, L"X = %.3f     Y = %.3f     Z = %.3f     ANGLE = %.3f", coords.x, coords.y, coords.z, angle);

    CFont::SetPropOn();
    CFont::SetRightJustifyOff;
    CFont::SetCentreOff();
    CFont::SetJustifyOn();
    CFont::SetColor(CRGBA(255, 255, 255, 255));
    CFont::SetBackgroundOff();
    CFont::SetFontStyle(1);
    CFont::SetDropShadowPosition(0);
    CFont::SetDropColor(CRGBA(0, 0, 0, 255));
    CFont::SetWrapx(SCREEN_WIDTH - 40);
    CFont::SetScale(SCREEN_MULTIPLIER(0.9f), SCREEN_MULTIPLIER(1.6f));

    // Coords
    if (MenuSystem.bEnableCoords)
        CFont::PrintString(SCREEN_COORD_LEFT(20.0f), SCREEN_COORD_TOP(20.0f), gString);
}

void CMenuSystem::AirbreakMode() {
    CPed *player = FindPlayerPed();
    CPad *pad = CPad::GetPad(0);
    CVector Forward;
    CVector Up;
    CVector Down;
    float angle = FindPlayerHeading();
    static bool restore;
    static bool set;

    // Set Mode
    if (MenuSystem.bEnableTimeStopper) {  
        Forward = FindPlayerPed()->m_placement * CVector(0, 2.0f, 0);
        Up = FindPlayerPed()->m_placement * CVector(0, 0.0f, 2.0f);
        Down = FindPlayerPed()->m_placement * CVector(0, 0.0f, -2.0f);

        *(float*)0x68F5F0 = 0.0f; // Set game gravity to 0.

        if (pad->CurrentKeyState.num8) { // Forward
            player->Teleport(Forward);
        }

        if (pad->CurrentKeyState.num9) { // Up
            player->Teleport(Up);
        }

        else if (pad->CurrentKeyState.num3) { // Down
            player->Teleport(Down);
        }

        if (pad->CurrentKeyState.num4) // Rotate left
            angle += 0.133f;
        else if (pad->CurrentKeyState.num6) // Rotate right
            angle -= 0.133f;

        player->m_placement.SetHeading(angle);
        FindPlayerPed()->WorkOutHeadingForMovingFirstPerson(angle);

        restore = false;

        char DisableKey = plugin::KeyPressed(VK_RETURN);

        if (DisableKey) {
            MenuSystem.bEnableTimeStopper = 0;
        }
    }
    else {
        if (!restore) {
            FindPlayerPed()->RestorePreviousState();
            FindPlayerPed()->Idle();

            *(float*)0x68F5F0 = 0.0080000004f; // Reset default game gravity

            restore = true;
        }

        set = false;
    }
}

void CMenuSystem::SetTime() {
    static bool restore;

    if (MenuSystem.bEnableSetTime) {

        restore = false;
    }
    else {
        if (!restore) {

            CHud::SetHelpMessage((const GtaChar*)L"", 1, 0, 0);

            restore = true;
        }
    }
}

void CMenuSystem::DrawSelectedMenuItem(int slotId) {
    // Set selected menu item
    float x = SCREEN_COORD_CENTER_X - SCREEN_COORD(512 / 2);
    float y0 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f);
    float y1 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - 39.6f);
    float y2 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 2));
    float y3 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 3));
    float y4 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 4));
    float y5 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 5));
    float y6 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 6));
    float y7 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 7));
    float y8 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 8));
    float y9 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 9));
    float y10 = SCREEN_COORD_CENTER_Y - SCREEN_COORD(512 / 2 - 44.0f - (39.6f * 10));

    float w = SCREEN_COORD(512);
    float h = SCREEN_COORD(40.0f);

    CRGBA color = { 255,255,255, 0 };

    // WheelSlot
    switch (slotId) {
    case 0:
        MenuSystem.lineID = 0;
        CSprite2d::DrawRect(CRect(x, y0, x + w, y0 + h), CRGBA(color));
        break;
    case 1:
        MenuSystem.lineID = 1;
        CSprite2d::DrawRect(CRect(x, y1, x + w, y1 + h), CRGBA(color));
        break;
    case 2:
        MenuSystem.lineID = 2;
        CSprite2d::DrawRect(CRect(x, y2, x + w, y2 + h), CRGBA(color));
        break;
    case 3:
        MenuSystem.lineID = 3;
        CSprite2d::DrawRect(CRect(x, y3, x + w, y3 + h), CRGBA(color));
        break;
    case 4:
        MenuSystem.lineID = 4;
        CSprite2d::DrawRect(CRect(x, y4, x + w, y4 + h), CRGBA(color));
        break;
    case 5:
        MenuSystem.lineID = 5;
        CSprite2d::DrawRect(CRect(x, y5, x + w, y5 + h), CRGBA(color));
        break;
    case 6:
        MenuSystem.lineID = 6;
        CSprite2d::DrawRect(CRect(x, y6, x + w, y6 + h), CRGBA(color));
        break;
    case 7:
        MenuSystem.lineID = 7;
        CSprite2d::DrawRect(CRect(x, y7, x + w, y7 + h), CRGBA(color));
        break;
    case 8:
        MenuSystem.lineID = 8;
        CSprite2d::DrawRect(CRect(x, y8, x + w, y8 + h), CRGBA(color));
        break;
    case 9:
        MenuSystem.lineID = 9;
        CSprite2d::DrawRect(CRect(x, y9, x + w, y9 + h), CRGBA(color));
        break;
    case 10:
        MenuSystem.lineID = 10;
        CSprite2d::DrawRect(CRect(x, y10, x + w, y10 + h), CRGBA(color));
        break;
    };
}

void CMenuSystem::AddString(wchar_t *nString, int nLine) {
    CFont::SetPropOn();
    CFont::SetRightJustifyOff();
    CFont::SetCentreOn();
    CFont::SetBackgroundOff();
    CFont::SetJustifyOff();
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetScale(SCREEN_MULTIPLIER(1.0f), SCREEN_MULTIPLIER(2.2f));
    CFont::SetDropColor(CRGBA(0, 0, 0, 0));
    CFont::SetDropShadowPosition(0);
    CFont::PrintString(SCREEN_COORD_CENTER_X + SCREEN_COORD(0), SCREEN_COORD_CENTER_Y - SCREEN_COORD(512.0f / 2 - ((nLine + 1) * 40.0f)), nString);
}