#include "struct.h"

uintptr_t getMatrix(uintptr_t base);
char* getBone(uintptr_t pBase, struct D3DMatrix viewMatrix);
uintptr_t getEntityAddr(uintptr_t base);
char* getWeaponId(uintptr_t base);
char* getNameByte(uintptr_t address);
PlayerWeapon getPlayerWeapon(uintptr_t base);
PlayerBone getPlayerBone(uintptr_t pBase, struct D3DMatrix viewMatrix);

int main()
{
	
	//isBeta = 1;
	char sText[400];
	if (isBeta == 0)
	{
		if (!Create())
		{
			//if (isBeta == 1)
				perror("Creation Failed");
			return 0;
		}
		if (!Connect())
		{
			//if (isBeta == 1)
				perror("Connection Failed");
			return 0;
		}
		int no;
		SetValue sv{};
	
		receive((void*)&sv);
		no = sv.mode;
		if (sv.type == 2)
			isPremium = true;
		
		if (no == 1)
			strcpy(version, "com.tencent.ig");
		else if (no == 2)
			strcpy(version, "com.pubg.krmobile");
		else if (no == 3)
			strcpy(version, "com.vng.pubgmobile");
		else
			strcpy(version, "com.rekoo.pubgm");

	}
	else isPremium = true;
	// betaend


//strcpy(version,"com.vng.pubgmobile");
	pid = getPid(version);
	if (pid < 10)
	{
		printf("\nGame is not running");
		Close();
		return 0;
	}
	if (isBeta == 1)
		printf("\n Game Pid: %d", pid);

	uintptr_t base = getBase();
	if (isBeta == 1)
		printf("\n Base: %lX\n", base);

	uintptr_t vMatrix = getMatrix(base);
	if (!vMatrix)
	{
		if (isBeta == 1)
			puts("\nMatrix Not Found");
		return 0;
	}
	if (isBeta == 1)
		printf("\nvMatrix: %lX", vMatrix);
	


	// looooooooooooooop
	uintptr_t enAddrPntr;
	float xy0, xy1;
	struct Vec3 xyz;
	struct Vec3 screen;
	struct Vec3 exyz;
	int isBack = 0, type = 69;
	int changed = 1;
	int myteamID = 101;
	uintptr_t cLoc = vMatrix + 0x740;
	uintptr_t fovPntr = vMatrix + 0x650;
	vMatrix = vMatrix + 0x750;
	char loaded[0x4000], loadedpn[20];
	char name[100];

	uintptr_t gname_buff[30];
	uintptr_t gname = getA(base + 0x956D338); 
	pvm(gname, &gname_buff, sizeof(gname_buff));
	char cont[0x500];
	char boneData[1024];
	char weaponData[100];

	Request request{};
	Response response{};
	
	while (isBeta ||  (receive((void*)&request) > 0)) {
		if (!isBeta) {
			height = request.ScreenHeight;
			width = request.ScreenWidth;
		}
		response.Success = false;
		response.PlayerCount = 0;
		response.VehicleCount = 0;
		response.ItemsCount = 0;
		response.GrenadeCount = 0;
		pvm(cLoc, &xyz, sizeof(xyz));
		if ((xyz.Z == 88.441124f || xyz.X == 0 || xyz.Z == 5278.43f || xyz.Z == 88.440918f) && !isBeta)
		{
			changed = 1;
			send((void*)&response, sizeof(response));
			continue;
		}
		pvm(fovPntr, &response.fov, 4);

		struct D3DMatrix vMat;
		pvm(vMatrix, &vMat, sizeof(vMat));
		if (isBeta)
			printf("\nvMatChk: %0.1f | FOV: %0.2f | XYZ: %f %f %f", vMat._43, response.fov, xyz.X, xyz.Y, xyz.Z);
		
		if (changed == 1){
			enAddrPntr = getEntityAddr(base);
			changed = 0;
		}
		Ulevel ulevel;
		pvm(enAddrPntr, &ulevel, sizeof(ulevel));
		if (ulevel.size < 1  || ulevel.size > 1024 || !isValid64(ulevel.addr)){
			if (isBeta)
				puts("\nWrong Entity Address");
			changed = 1;
			if (isBeta) {
				send((void*)&response, sizeof(response));
				continue;
			}
		}
		if (isBeta)
			printf("\nEntity Address: %lX | Size: %d", enAddrPntr, ulevel.size);
		
		memset(loaded, 0, 1000);
		for (int i = 0; i < ulevel.size; i++) {
			uintptr_t pBase = getA(ulevel.addr + i * SIZE);
			if (!isValid64(pBase))
				continue;
			if (getI(pBase + SIZE) != 8)
				continue;
			int ids = getI(pBase + 8 + 2 * SIZE);
			int page = ids / 0x4000;
			int index = ids % 0x4000;
			if (page < 1 || page>30)
				continue;
			if (gname_buff[page] == 0)
				gname_buff[page] = getA(gname + page * SIZE);
			strcpy(name, getText(getA(gname_buff[page] + index * SIZE)));
			if (strlen(name) < 5)
				continue;
			
			if (strstr(name, "BP_PlayerPawn")) {//Player
				
				char loadedpn[20];
				sprintf(loadedpn, "%lx,", pBase);
				if (strstr(loaded, loadedpn))
					continue;
				strcat(loaded, loadedpn);

				int oType = getI(pBase + 0x88);
				if (oType != 0x3ec2a00 && oType != 0x3ec2800)
					continue;

				if (getI(pBase + 0xb24))
					continue;
				pvm(pBase + 0xae0, healthbuff, sizeof(healthbuff));
				if (healthbuff[1] > 200.0f || healthbuff[1] < 50.0f || healthbuff[0]>healthbuff[1] || healthbuff[0] < 0.0f)
					continue;
				PlayerData* data = &response.Players[response.PlayerCount];
				data->Health = healthbuff[0] / healthbuff[1] * 100;

				data->TeamID = getI(pBase + 0x828);
				
				
				uintptr_t me = getI(pBase + 0x128);
				if (me == 258) {
					if (isBeta)
						printf("\nMe(%d): %lX ", data->TeamID, pBase);
					myteamID = data->TeamID;
					continue;
				}
				else if (me != 257)
					continue;
				

				if (data->TeamID == myteamID && myteamID<=100)
					continue;
				

				pvm(getA(pBase + 0x2ff0) + 0x160, &exyz, sizeof(exyz));
				
				data->HeadLocation = World2Screen(vMat, exyz);
				
				data->Distance = getDistance(xyz, exyz);
				if (data->Distance > 600.0f)
					continue;
				pvm(pBase + 0x8b8, &data->isBot, sizeof(data->isBot));
				
				strcpy(data->PlayerNameByte, "66:111:116:");

				if (data->HeadLocation.Z != 1.0f && data->HeadLocation.X < width + 100 && data->HeadLocation.X > -50) {
					if (isPremium && !data->isBot) {
						data->Bone = getPlayerBone(pBase, vMat);
						strcpy(data->PlayerNameByte, getNameByte(getA(pBase + 0x7e0)));
						if (strlen(data->PlayerNameByte) < 4)
							continue;
					}

					data->Weapon = getPlayerWeapon(pBase);
				}
				
			
				if (response.PlayerCount >= maxplayerCount) {
					continue;
				}
				
				response.PlayerCount++;
		
				if (isBeta)
					printf("\nE | %lX > TI:%d | H:%0.1f | XY: %0.1f %0.1f | %d", pBase, data->TeamID, data->Health, data->HeadLocation.X, data->HeadLocation.Y, data->isBot);

			}
			else if (strstr(name, "VH") || (strstr(name, "PickUp_") && !strstr(name, "BP")) || strstr(name, "Rony") || strstr(name, "Mirado") || strstr(name, "LadaNiva") || strstr(name, "AquaRail")) {//Vehicle
				if (!isPremium)
					continue;
				VehicleData* data = &response.Vehicles[response.VehicleCount];
				pvm(getA(pBase + 0x188) + 0x1b0, &exyz, sizeof(exyz));

				data->Location = World2Screen(vMat, exyz);
				if (data->Location.Z == 1.0f || data->Location.X > width + 200 || data->Location.X < -200)
					continue;
				data->Distance = getDistance(xyz, exyz);
				strcpy(data->VehicleName, name);
				if (response.VehicleCount >= maxvehicleCount) {
					continue;
				}
				response.VehicleCount++;

				if (isBeta)
					printf("\nV | %lX > XY: %0.1f %0.1f | N: %s", pBase, data->Location.X, data->Location.Y, name);
			}
			else if (strstr(name, "Pickup_C") || strstr(name, "PickUp") || strstr(name, "BP_Ammo") || strstr(name, "BP_QK") || strstr(name, "Wrapper")) {//Items
				if (!isPremium)
					continue;
				ItemData* data = &response.Items[response.ItemsCount];
				pvm(getA(pBase + 0x188) + 0x1b0, &exyz, sizeof(exyz));
				data->Location = World2Screen(vMat, exyz);
				if (data->Location.Z == 1.0f || data->Location.X > width + 100 || data->Location.X < -50)
					continue;
				data->Distance = getDistance(xyz, exyz);
				if (data->Distance > 200.0f)
					continue;
				strcpy(data->ItemName, name);
				if (response.ItemsCount >= maxitemsCount) {
					continue;
				}
				response.ItemsCount++;

				if (isBeta)
					printf("\nI | %lX > XY: %0.1f %0.1f | D:%0.1fm %s", pBase, data->Location.X, data->Location.Y, data->Distance, name);
			}
			else if (strstr(name, "BP_AirDropPlane_C") || strstr(name, "PlayerDeadInventoryBox_C") || strstr(name, "BP_AirDropBox_C")) {//Items
				if (!isPremium)
					continue;
				
				ItemData* data = &response.Items[response.ItemsCount];
				pvm(getA(pBase + 0x188) + 0x1b0, &exyz, sizeof(exyz));
				data->Location = World2Screen(vMat, exyz);
				if (data->Location.Z == 1.0f || data->Location.X > width + 100 || data->Location.X < -50)
					continue;
				data->Distance = getDistance(xyz, exyz);
				strcpy(data->ItemName, name);
				if (response.ItemsCount >= maxitemsCount) {
					continue;
				}
				response.ItemsCount++;

				if (isBeta)
					printf("\nSp | %lX > XY: %0.1f %0.1f | D:%0.1fm %s", pBase, data->Location.X, data->Location.Y, data->Distance, name);
			}
			else if (strstr(name, "BP_Grenade_Shoulei_C") || strstr(name, "BP_Grenade_Burn_C")) {//Grenade Warning
			if (!isPremium)
				continue;
			GrenadeData* data = &response.Grenade[response.GrenadeCount];
			pvm(getA(pBase + 0x188) + 0x1b0, &exyz, sizeof(exyz));
			data->Location = World2Screen(vMat, exyz);
			
			data->Distance = getDistance(xyz, exyz);
			if (data->Distance > 150.0f)
				continue;
			if (strstr(name, "Shoulei"))
				data->type = 1;
			else
				data->type = 2;
			if (response.GrenadeCount >= maxgrenadeCount) {
				continue;
			}
			response.GrenadeCount++;


			if (isBeta)
				printf("\nGW | %lX > XY: %0.1f %0.1f | D:%0.1fm %d", pBase, data->Location.X, data->Location.Y, data->Distance, name);
			}


		}

		if(response.PlayerCount+response.ItemsCount+response.VehicleCount+response.GrenadeCount + response.GrenadeCount >0)
			response.Success = true;

		if (isBeta) {
			printf("\nPlayers: %d | Vehicle: %d | Items: %d ", response.PlayerCount,response.VehicleCount,response.ItemsCount);
			break;
		}
		
		send((void*)&response, sizeof(response));
		
	}


	
	if (isBeta)
		puts("\n\nScript Completed ");
}

uintptr_t getMatrix(uintptr_t base)
{
	                        //ViewWorld
	return getA(getA(base + 0x98E74B0) + 0x98);
}

uintptr_t getEntityAddr(uintptr_t base)
{                           //GWorld
	return getA(getA(base + 0x9907940) + 0x30) + 0xb0;
}

char* getNameByte(uintptr_t address)
{
	char static lj[64];
	memset(lj, 0, 64);
	unsigned short int nameI[32];
	pvm(address, nameI, sizeof(nameI));
	char s[10];
	int i;
	for (i = 0; i < 32; i++)
	{
		if (nameI[i] == 0)
			break;
		sprintf(s, "%d:", nameI[i]);
		strcat(lj, s);
	}
	lj[63] = '\0';

	return lj;
}


PlayerBone getPlayerBone(uintptr_t pBase, struct D3DMatrix viewMatrix)
{
	PlayerBone b;
	b.isBone = true;
	struct D3DMatrix oMatrix;
	uintptr_t boneAddr = getA(pBase + 0x3f8);
	struct D3DMatrix baseMatrix = getOMatrix(boneAddr + 0x1a0);
	int bones[] = { 4, 3, 0,11, 32, 12, 33, 63, 62, 52, 56, 53, 57, 54, 58 };
	boneAddr = getA(boneAddr + 0x6c0);


	oMatrix = getOMatrix(boneAddr + (bones[0] + 1) * 48);
	b.neck = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));

	oMatrix = getOMatrix(boneAddr + (bones[1] + 1) * 48);
	b.cheast = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));

	oMatrix = getOMatrix(boneAddr + (bones[2] + 1) * 48);
	b.pelvis = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lSh 3
	oMatrix = getOMatrix(boneAddr + (bones[3] + 1) * 48);
	b.lSh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rSh 4
	oMatrix = getOMatrix(boneAddr + (bones[4] + 1) * 48);
	b.rSh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lElb 5
	oMatrix = getOMatrix(boneAddr + (bones[5] + 1) * 48);
	b.lElb = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rElb 6
	oMatrix = getOMatrix(boneAddr + (bones[6] + 1) * 48);
	b.rElb = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lWr 7
	oMatrix = getOMatrix(boneAddr + (bones[7] + 1) * 48);
	b.lWr = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rWr 8
	oMatrix = getOMatrix(boneAddr + (bones[8] + 1) * 48);
	b.rWr = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lTh 9
	oMatrix = getOMatrix(boneAddr + (bones[9] + 1) * 48);
	b.lTh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rTh 10
	oMatrix = getOMatrix(boneAddr + (bones[10] + 1) * 48);
	b.rTh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lKn 11
	oMatrix = getOMatrix(boneAddr + (bones[11] + 1) * 48);
	b.lKn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rKn 12
	oMatrix = getOMatrix(boneAddr + (bones[12] + 1) * 48);
	b.rKn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//lAn 13 
	oMatrix = getOMatrix(boneAddr + (bones[13] + 1) * 48);
	b.lAn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
	//rAn 14
	oMatrix = getOMatrix(boneAddr + (bones[14] + 1) * 48);
	b.rAn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));

	return b;
}
PlayerWeapon getPlayerWeapon(uintptr_t base) {
	PlayerWeapon p;
	uintptr_t addr[3];
	pvm(getA(base + 0x178), addr, sizeof(addr));

	if (isValid64(addr[0]) && getI(addr[0] + 0x11c) == 2) {
		p.isWeapon = true;
		p.id = getI(getA(addr[0] + 0x5f8) + 0x130);
		p.ammo = getI(addr[0] + 0xae8);
	}
	else if (isValid64(addr[1]) && getI(addr[1] + 0x11c) == 2) {
		p.isWeapon = true;
		p.id = getI(getA(addr[1] + 0x5f8) + 0x130);
		p.ammo = getI(addr[1] + 0xae8);
	}
	else if (isValid64(addr[2]) && getI(addr[2] + 0x11c) == 2) {
		p.isWeapon = true;
		p.id = getI(getA(addr[2] + 0x5f8) + 0x130);
		p.ammo = getI(addr[2] + 0xae8);
	}

	return p;
}