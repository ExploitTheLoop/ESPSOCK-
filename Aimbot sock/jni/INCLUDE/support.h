#include "socket.h"
#define PI 3.141592653589793238

ssize_t process_v(pid_t __pid,   struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
    return syscall(process_vm_readv_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}
int pvm(uintptr_t address, void* buffer,int size) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = (void*)buffer;
    local[0].iov_len = size;
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = size;

ssize_t bytes = process_v(pid, local, 1, remote, 1, 0);
    return bytes == size;
}

ssize_t process_vm_writev(pid_t __pid,   struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_writev_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}
int vm_writev(void* address, void* buffer,int size) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = (void*)buffer;
	local[0].iov_len = size;
	remote[0].iov_base = (void*)address;
	remote[0].iov_len = size;

ssize_t bytes = process_vm_writev(pid, local, 1, remote, 1, 0);
	return bytes == size;
}

void Write(uintptr_t address, float value) 
{
int aa = vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

struct D3DMatrix ToMatrixWithScale(struct Vec3 translation,struct Vec3 scale,struct Vec4 rot)
 {
struct D3DMatrix m;
 m._41 = translation.X;
 m._42 = translation.Y;
 m._43 = translation.Z;
            

float x2 = rot.X + rot.X;
float y2 = rot.Y + rot.Y;
float z2 = rot.Z + rot.Z;


float xx2 = rot.X * x2;
float yy2 = rot.Y * y2;
float zz2 = rot.Z * z2;

m._11 = (1.0f - (yy2 + zz2)) * scale.X;
m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
m._33 = (1.0f - (xx2 + yy2)) * scale.Z;


float yz2 = rot.Y * z2;
float wx2 = rot.W * x2;

m._32 = (yz2 - wx2) * scale.Z;
m._23 = (yz2 + wx2) * scale.Y;

float xy2 = rot.X * y2;
float wz2 = rot.W * z2;

m._21 = (xy2 - wz2) * scale.Y;
m._12 = (xy2 + wz2) * scale.X;


float xz2 = rot.X * z2;
float wy2 = rot.W * y2;

m._31 = (xz2 + wy2) * scale.Z;
m._13 = (xz2 - wy2) * scale.X;


m._14 = 0.0f;
m._24 = 0.0f;
m._34 = 0.0f;
m._44 = 1.0f;

return m;
}
struct Vec3 mat2Cord(struct D3DMatrix pM1,struct D3DMatrix pM2){
struct  Vec3 pOut;

pOut.X = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
pOut.Y = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
pOut.Z = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
           
return pOut;
}
uintptr_t getBase(){
    FILE *fp;
    uintptr_t addr = 0;
    char filename[40], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "libUE4.so")) {
                addr = (uintptr_t)strtoull(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}


D3DXMATRIX ToMatrixWithScale1(D3DXVECTOR4 Rotation, D3DVector Translation, D3DVector Scale3D)
{
	D3DXMATRIX M;
	float X2, Y2, Z2, xX2, Yy2, Zz2, Zy2, Wx2, Xy2, Wz2, Zx2, Wy2;
	M._41 = Translation.X;
	M._42 = Translation.Y;
	M._43 = Translation.Z;
	X2 = Rotation.X + Rotation.X;
	Y2 = Rotation.Y + Rotation.Y;
	Z2 = Rotation.Z + Rotation.Z;
	xX2 = Rotation.X * X2;
	Yy2 = Rotation.Y * Y2;
	Zz2 = Rotation.Z * Z2;
	M._11 = (1 - (Yy2 + Zz2)) * Scale3D.X;
	M._22 = (1 - (xX2 + Zz2)) * Scale3D.Y;
	M._33 = (1 - (xX2 + Yy2)) * Scale3D.Z;
	Zy2 = Rotation.Y * Z2;
	Wx2 = Rotation.W * X2;
	M._32 = (Zy2 - Wx2) * Scale3D.Z;
	M._23 = (Zy2 + Wx2) * Scale3D.Y;
	Xy2 = Rotation.X * Y2;
	Wz2 = Rotation.W * Z2;
	M._21 = (Xy2 - Wz2) * Scale3D.Y;
	M._12 = (Xy2 + Wz2) * Scale3D.X;
	Zx2 = Rotation.X * Z2;
	Wy2 = Rotation.W * Y2;
	M._31 = (Zx2 + Wy2) * Scale3D.Z;
	M._13 = (Zx2 - Wy2) * Scale3D.X;
	M._14 = 0;
	M._24 = 0;
	M._34 = 0;
	M._44 = 1;
	return M;
}

D3DXMATRIX D3DXMatrixMultiply1(D3DXMATRIX Pm1, D3DXMATRIX Pm2)
{
	D3DXMATRIX Pout;
	Pout._11 = Pm1._11 * Pm2._11 + Pm1._12 * Pm2._21 + Pm1._13 * Pm2._31 + Pm1._14 * Pm2._41;
	Pout._12 = Pm1._11 * Pm2._12 + Pm1._12 * Pm2._22 + Pm1._13 * Pm2._32 + Pm1._14 * Pm2._42;
	Pout._13 = Pm1._11 * Pm2._13 + Pm1._12 * Pm2._23 + Pm1._13 * Pm2._33 + Pm1._14 * Pm2._43;
	Pout._14 = Pm1._11 * Pm2._14 + Pm1._12 * Pm2._24 + Pm1._13 * Pm2._34 + Pm1._14 * Pm2._44;
	Pout._21 = Pm1._21 * Pm2._11 + Pm1._22 * Pm2._21 + Pm1._23 * Pm2._31 + Pm1._24 * Pm2._41;
	Pout._22 = Pm1._21 * Pm2._12 + Pm1._22 * Pm2._22 + Pm1._23 * Pm2._32 + Pm1._24 * Pm2._42;
	Pout._23 = Pm1._21 * Pm2._13 + Pm1._22 * Pm2._23 + Pm1._23 * Pm2._33 + Pm1._24 * Pm2._43;
	Pout._24 = Pm1._21 * Pm2._14 + Pm1._22 * Pm2._24 + Pm1._23 * Pm2._34 + Pm1._24 * Pm2._44;
	Pout._31 = Pm1._31 * Pm2._11 + Pm1._32 * Pm2._21 + Pm1._33 * Pm2._31 + Pm1._34 * Pm2._41;
	Pout._32 = Pm1._31 * Pm2._12 + Pm1._32 * Pm2._22 + Pm1._33 * Pm2._32 + Pm1._34 * Pm2._42;
	Pout._33 = Pm1._31 * Pm2._13 + Pm1._32 * Pm2._23 + Pm1._33 * Pm2._33 + Pm1._34 * Pm2._43;
	Pout._34 = Pm1._31 * Pm2._14 + Pm1._32 * Pm2._24 + Pm1._33 * Pm2._34 + Pm1._34 * Pm2._44;
	Pout._41 = Pm1._41 * Pm2._11 + Pm1._42 * Pm2._21 + Pm1._43 * Pm2._31 + Pm1._44 * Pm2._41;
	Pout._42 = Pm1._41 * Pm2._12 + Pm1._42 * Pm2._22 + Pm1._43 * Pm2._32 + Pm1._44 * Pm2._42;
	Pout._43 = Pm1._41 * Pm2._13 + Pm1._42 * Pm2._23 + Pm1._43 * Pm2._33 + Pm1._44 * Pm2._43;
	Pout._44 = Pm1._41 * Pm2._14 + Pm1._42 * Pm2._24 + Pm1._43 * Pm2._34 + Pm1._44 * Pm2._44;
	return Pout;
}

D3DVector D3dMatrixMultiply1(D3DXMATRIX bonematrix, D3DXMATRIX actormatrix)
{
	D3DVector result;
	result.X =
		bonematrix._41 * actormatrix._11 + bonematrix._42 * actormatrix._21 +
		bonematrix._43 * actormatrix._31 + bonematrix._44 * actormatrix._41;
	result.Y =
		bonematrix._41 * actormatrix._12 + bonematrix._42 * actormatrix._22 +
		bonematrix._43 * actormatrix._32 + bonematrix._44 * actormatrix._42;
	result.Z =
		bonematrix._41 * actormatrix._13 + bonematrix._42 * actormatrix._23 +
		bonematrix._43 * actormatrix._33 + bonematrix._44 * actormatrix._43;
	return result;
}


pid_t getPid(char * name){
    char text[69];
    pid_t pid = 0;
    sprintf(text,"pidof %s",name);
FILE *chkRun = popen(text, "r");
    if (chkRun){
        char output[10];
        fgets(output ,10,chkRun);
        pclose(chkRun);
        pid= atoi(output);
        }
    if (pid < 10) {
        DIR* dir = NULL;
        struct dirent* ptr = NULL;
        FILE* fp = NULL;
        char filepath[256];
        char filetext[128];
        dir = opendir("/proc");
        if (NULL != dir)
        {
            while ((ptr = readdir(dir)) != NULL)
            {
                if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                    continue;
                if (ptr->d_type != DT_DIR)
                    continue;
                sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
                fp = fopen(filepath, "r");
                if (NULL != fp)
                {
                    fgets(filetext, sizeof(filetext), fp);


                    if (strcmp(filetext, name) == 0)
                    {
                        fclose(fp);
                        break;
                    }
                    fclose(fp);
                }
            }
        }
        if (readdir(dir) == NULL)
        {
            closedir(dir);
            return 0;
        }
        closedir(dir);
        pid= atoi(ptr->d_name);
    }
        return pid;
}



int isValidItem(int id) {
    if (id >= 100000 && id < 999999)
        return 1;
    return 0;
}
float getF(uintptr_t address) {
    float buff;
    pvm(address, &buff, 4);
    return buff;
}
uintptr_t getA(uintptr_t address) {
    uintptr_t buff;
    pvm(address, &buff, SIZE);
    return buff;
}
int getI(uintptr_t address) {
    int buff;
    pvm(address, &buff, 4);
    return buff;
}
int isValid64(uintptr_t addr) {
    if (addr < 0x1000000000 || addr>0xefffffffff || addr % SIZE != 0)
        return 0;
    return 1;
}
int isValid32(uintptr_t addr) {
    if (addr < 0x10000000 || addr>0xefffffff || addr % SIZE != 0)
        return 0;
    return 1;
}

void Write1(unsigned int addr, float value)
{

char lj[64];
int handle;

sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek64(handle,0,SEEK_SET);
pwrite(handle,&value,4,addr);
close(handle);
}
float getDistance(struct Vec3 mxyz,struct Vec3 exyz){
return sqrt ((mxyz.X-exyz.X)*(mxyz.X-exyz.X)+(mxyz.Y-exyz.Y)*(mxyz.Y-exyz.Y)+(mxyz.Z-exyz.Z)*(mxyz.Z-exyz.Z))/100;
    
}

float get2dDistance(float x, float y, float x1, float y1)
{
	float xx1 = x - x1;
	float yy2 = y - y1;
	//
	return sqrt(xx1 * xx1 + yy2 * yy2);
}


float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y, float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	//
	return (float)(sqrt(x * x + y * y + z * z));
}

D2DVector rotateCoord(float angle, float objectRadar_x, float objectRadar_y)
{
	D2DVector radarCoordinate;
	float s = sin(angle * PI / 180);
	float c = cos(angle * PI / 180);
	radarCoordinate.X = objectRadar_x * c + objectRadar_y * s;
	radarCoordinate.Y = -objectRadar_x * s + objectRadar_y * c;
	return radarCoordinate;
}

FTransform ReadFTransform(uintptr_t address)
{
	FTransform Result;
	Result.Rotation.X = getF(address);	// Rotation_X 
	Result.Rotation.Y = getF(address + 4);	// Rotation_y
	Result.Rotation.Z = getF(address + 8);	// Rotation_z
	Result.Rotation.W = getF(address + 12);	// Rotation_w
	Result.Translation.X = getF(address + 16);	// /Translation_X
	Result.Translation.Y = getF(address + 20);	// Translation_y
	Result.Translation.Z = getF(address + 24);	// Translation_z
	Result.Scale3D.X = getF(address + 32);;	// Scale_X
	Result.Scale3D.Y = getF(address + 36);;	// Scale_y
	Result.Scale3D.Z = getF(address + 40);;	// Scale_z
	return Result;
}


D3DVector getBoneXYZ(uintptr_t humanAddr, uintptr_t boneAddr, int Part)
{
	FTransform Bone = ReadFTransform(boneAddr + Part * 48);
	
	FTransform Actor = ReadFTransform(humanAddr);
	D3DXMATRIX Bone_Matrix = ToMatrixWithScale1(Bone.Rotation, Bone.Translation, Bone.Scale3D);
	D3DXMATRIX Component_ToWorld_Matrix =ToMatrixWithScale1(Actor.Rotation, Actor.Translation, Actor.Scale3D);
	D3DVector result = D3dMatrixMultiply1(Bone_Matrix, Component_ToWorld_Matrix);
	return result;
}


struct Vec3 World2Screen(struct D3DMatrix viewMatrix, struct Vec3 pos) {
    struct Vec3 screen;
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z) + viewMatrix._44;

    if (screenW < 0.01f)
        screen.Z = 1;
    else
        screen.Z = 0;


    float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z) + viewMatrix._41;
    float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * pos.Z) + viewMatrix._42;
    screen.Y = (height / 2) - (height / 2) * screenY / screenW;
    screen.X = (width / 2) + (width / 2) * screenX / screenW;


    return screen;

}
struct Vec2 World2ScreenMain(struct D3DMatrix viewMatrix, struct Vec3 pos) {
    struct Vec2 screen;
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z) + viewMatrix._44;

    float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z) + viewMatrix._41;
    float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * pos.Z) + viewMatrix._42;
    screen.Y = (height / 2) - (height / 2) * screenY / screenW;
    screen.X = (width / 2) + (width / 2) * screenX / screenW;
    return screen;

}
struct D3DMatrix getOMatrix(uintptr_t boneAddr){
    float oMat[11];
    pvm(boneAddr,&oMat,sizeof(oMat));
    rot.X=oMat[0];
    rot.Y=oMat[1];
    rot.Z=oMat[2];
    rot.W=oMat[3];
            
    tran.X=oMat[4];
    tran.Y=oMat[5];
    tran.Z=oMat[6];
            
    scale.X=oMat[8];
    scale.Y=oMat[9];
    scale.Z=oMat[10];
            
    return ToMatrixWithScale(tran,scale,rot);
}


D2DVector getPointingAngle(uintptr_t uMyObject, uintptr_t ObjectAddrPointer, float object_x,float object_y, float object_z, float Self_x, float Self_y, float Self_z)
{
	D2DVector PointingAngle;

	
	float bulletVelocity = getF(getA(getA(uMyObject + 5124) + 0x900) + 0x31C);
	float ypjl = get_3D_Distance(object_x, object_y, object_z, Self_x, Self_y, Self_z);
	float xtime = ypjl / 88000;
	float ytime = ypjl / 88000;	// bulletVelocity

	uintptr_t playaddr = getA(ObjectAddrPointer + 0x140);
	float ypx = getF( playaddr + 0x1a0);
	float ypy = getF( playaddr + 0x1a0 +4);
    float ypz = getF( playaddr + 0x1a0 +8);


	
	object_x = object_x + ypx * xtime;
	object_y = object_y + ypy * xtime;
	object_z = object_z + 360 * ytime * ytime;


	float zbcx = object_x - Self_x;
	float zbcy = object_y - Self_y;
	float zbcz = object_z - Self_z;

	PointingAngle.X = atan2(zbcy, zbcx) * 180 / PI;	// 57.3
	PointingAngle.Y = atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI;
	return PointingAngle;

}


char* getText(uintptr_t addr) {
    static char txt[42];
    memset(txt, 0, 42);
    char buff[41];
    pvm(addr + 4+SIZE, &buff, 41);
    int i;
    for (i = 0; i < 41; i++) {
        if (buff[i] == 0)
            break;

        txt[i] = buff[i];

        if (buff[i] == 67 && i > 10)
            break;

    }
    txt[i + 1] = '\0';
    return txt;
}
void dump(const uintptr_t gaddr, const int gsize, char* name) {
    char buff[0x100000];
    uintptr_t addr = gaddr;
    int size = gsize;
    FILE* fp = fopen(name, "w");
    while (size > 0) {
        if (size < 0x100000) {
            pvm(addr, buff, size);

            for (int i = 0; i < size; i++)
                fwrite(&buff[i], 1, 1, fp);

        }
        else {
            pvm(addr, buff, 0x100000);

            for (int i = 0; i < 0x100000; i++)
                fwrite(&buff[i], 1, 1, fp);
        }

        addr += 0x100000;
        size -= 0x100000;
    }
    fclose(fp);

}

