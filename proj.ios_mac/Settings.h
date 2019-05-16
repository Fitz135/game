#ifndef __SETTINGS_H__
#define __SETTINGS_H__


#include <cstdint>
#include <set>

namespace Settings
{
    namespace Font
    {
        namespace Size
        {
            constexpr int name = 18;
            constexpr int chat = 24;
            constexpr int title  = 60;
            constexpr int label  = 40;
            constexpr int normal = 32;
            constexpr int light  = 24;
            constexpr int result = 48;
        }
        namespace Type
        {
            constexpr char title[] = "fonts/Quicksand-Bold.ttf";  //字体
            constexpr char base[] = "fonts/OpenSans-Regular.ttf";
        };
    }
    namespace Map
    {
        constexpr char path[] = "GameItem/Map/";
        static std::set<const char *> list{
            "town_10"
        };
        namespace TileSize
        {
            constexpr int height = 40;
            constexpr int width = 40;
        }
    }
    
    namespace Player //TODO 一场游戏中具体有多少个Ai玩家？
    {
        constexpr char path[] = "GameItem/Player/";
        constexpr char* roleName[] = {
            "Player1",
            "Player2",
            "Player3"
        };
    }
    
    namespace Bubble
    {
        constexpr char path[] = "GameItem/Bubble/default.png";
    }
    namespace Prop
    {
        constexpr char path[] = "GameItem/Prop/";
        constexpr char * name[] = {
            "diamond.png",
            "heart.png",
        };
    }
    namespace Server
    {
        constexpr char addr[] = "ws://115.159.188.148:4000";
    }
}
#endif
