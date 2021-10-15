#include <sys/uio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<math.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <sys/un.h>
#include <time.h>
#include <ctype.h>
#include <iostream>



#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

#define LEN sizeof(struct MAPS)



struct Vec4 {
    float  X, Y, Z, W;
};
struct Vec3 {
    float X, Y, Z;
};
struct Vec2 {
    float X, Y;
};
struct D3DMatrix

{

    float _11, _12, _13, _14;

    float _21, _22, _23, _24;

    float _31, _32, _33, _34;

    float _41, _42, _43, _44;

};

struct D3DXVECTOR4
{
	float X;
	float Y;
	float Z;
	float W;
};

struct D3DVector
{
	float X;
	float Y;
	float Z;
};

struct D2DVector
{
	float X;
	float Y;
};

struct FTransform
{
	D3DXVECTOR4 Rotation;
	D3DVector Translation;
	D3DVector Scale3D;
};

struct D3DXMATRIX
{
	float _11;
	float _12;
	float _13;
	float _14;
	float _21;
	float _22;
	float _23;
	float _24;
	float _31;
	float _32;
	float _33;
	float _34;
	float _41;
	float _42;
	float _43;
	float _44;
};


struct Vec4 rot;
struct Vec3 scale, tran;

//deta
int height = 1080;
int width = 2340;
int pid = 0;
int isBeta, nByte;
float mx = 0, my = 0, mz = 0;


int openMirror;		
int fire;						
uintptr_t aimatRecord = 0;		
int aimbotmode = 1;		//Head		
int openState = 1;				
int aimingState = 3;	//Fire + Scope		
bool tracingStatus = true;		
int priority = 1;	//CrosaHair			
int pour = 0;				
int aimingRange = 0;			
int BSpeed = 88000;
uintptr_t zmAddr;
uintptr_t zmAddrx, zmAddry;		
uintptr_t qunViewAngle_x;	

float px;						
float py;




struct MAPS
{
    long int fAddr;
    long int lAddr;
    struct MAPS* next;
};
struct Ulevel {
    uintptr_t addr;
    int size;
};
typedef struct MAPS* PMAPS;

#define SIZE sizeof(uintptr_t)

