#define MIDDLE 250
namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // Constants
    /////////////////////////////////////////////////////////////////////////////

    enum AUDIO_ID  				// 定義各種音效的編號
    {
        AUDIO_GAME,				// 0
        AUDIO_LASER,			// 1
        AUDIO_EAT_HEART,		// 2
        AUDIO_FLYING,			// 3
        AUDIO_BELL,				// 4
        AUDIO_SNATCH			// 5
    };

    class CGameMap;
    class MainGirl;
    class Man;
    class Girl;
    class Heart;
    class CHeartPoint;
    class UI;
}
#include "CGameMap.h"
#include "MainGirl.h"
#include "Man.h"
#include "Girl.h"
#include "Heart.h"
#include "CHeartPoint.h"
#include "UI.h"