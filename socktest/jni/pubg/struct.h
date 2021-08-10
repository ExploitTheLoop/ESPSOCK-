#include "support.h"
#include "init.h"
#include <string>
#define maxplayerCount 100
#define maxvehicleCount 50
#define maxitemsCount 400
#define maxgrenadeCount 10
using namespace std;


struct PlayerBone {
	bool isBone=false;
	Vec2 neck;
	Vec2 cheast;
	Vec2 pelvis;
	Vec2 lSh;
	Vec2 rSh;
	Vec2 lElb;
	Vec2 rElb;
	Vec2 lWr;
	Vec2 rWr;
	Vec2 lTh;
	Vec2 rTh;
	Vec2 lKn;
	Vec2 rKn;
	Vec2 lAn;
	Vec2 rAn;
};
struct PlayerWeapon {
    int EspCount;
	bool isWeapon=false;
	int id;
	int ammo;
};
enum Mode {
	InitMode = 1,
	ESPMode = 2,
	HackMode = 3,
	StopMode = 4,
};

struct Request {
	int Mode;
	int ScreenWidth;
	int ScreenHeight;
};

struct SetValue {
    int EspCount;
	int mode;
	int type;
};

struct VehicleData {
    int EspCount;
	char VehicleName[50];
	float Distance;
	Vec3 Location;
};

struct ItemData {
    int EspCount;
	char ItemName[50];
	float Distance;
	Vec3 Location;
};

struct GrenadeData {
    int EspCount;
	int type;
	float Distance;
	Vec3 Location;
};

struct PlayerData {
    int EspCount;
	char PlayerNameByte[100]; 
	int TeamID;
	float Health;
	float Distance;
	bool isBot;
	Vec3 HeadLocation;
	PlayerWeapon Weapon;
	PlayerBone Bone;
};

struct Response {
    int EspCount;
	bool Success;
	int PlayerCount;
	int VehicleCount;
	int ItemsCount;
	int GrenadeCount;
	float fov;
	PlayerData Players[maxplayerCount];
	VehicleData Vehicles[maxvehicleCount];
	ItemData Items[maxitemsCount];
	GrenadeData Grenade[maxgrenadeCount];
};