#include "Generic.h"
#include "Draw.h"
#include "Sound.h"
#include "PixelScript.h"
#include <stdio.h>
#include <cstring>
#include "fopen.h"

long GetFileSizeLong(const char *path)
{
	/*
	long len;
	FILE_e *fp;

	len = 0;

	fp = fopen_embed(path, "rb");
	if (fp == NULL)
		return -1;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fclose_embed(fp);
	return len;*/
	return 0;
}


void GetCompileDate(int *year, int *month, int *day)
{
	int i;
	char strMonth[0x10];

	const char *table[13] = {
		"XXX",
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	sscanf("Mar  8 2006", "%s %d %d", strMonth, day, year);	//The expansion of __DATE__ is not reproductible. (TODO: Think about changing this to be reproductible)
	
	for (i = 0; i < 12; ++i)	//This being 12 instead of 13 might be a bug, but it works anyway by accident
		if (!memcmp(&strMonth, table[i], 3))
			break;
	
	*month = i;
}

BOOL GetCompileVersion(int *v1, int *v2, int *v3, int *v4)
{
	*v1 = (unsigned short)1;
	*v2 = (unsigned short)5;
	*v3 = (unsigned short)2;
	*v4 = (unsigned short)0;
	return TRUE;
	/*
	unsigned int puLen;
	VS_FIXEDFILEINFO *lpBuffer;
	DWORD dwHandle;
	DWORD dwLen;
	char path[MAX_PATH];
	LPVOID lpData;
	BOOL bResult;

	lpData = NULL;
	bResult = FALSE;

	GetModuleFileName(NULL, path, sizeof(path));
	dwLen = GetFileVersionInfoSize(path, &dwHandle);

	if (dwLen == 0)
		goto fail;

	lpData = malloc(dwLen);

	if (lpData == NULL)
		goto fail;

	if (!GetFileVersionInfo(path, 0, dwLen, lpData))
		goto fail;

	if (!VerQueryValueA(lpData, "/", (LPVOID*)&lpBuffer, &puLen))
		goto fail;

	*v1 = (unsigned short)(lpBuffer->dwFileVersionMS >> 16);
	*v2 = (unsigned short)(lpBuffer->dwFileVersionMS & 0xFFFF);
	*v3 = (unsigned short)(lpBuffer->dwFileVersionLS >> 16);
	*v4 = (unsigned short)(lpBuffer->dwFileVersionLS & 0xFFFF);
	bResult = TRUE;

fail:

	if (lpData != NULL)
		free(lpData);
	
	return bResult;
	*/
}

BOOL OpenSoundVolume(void)
{
	return TRUE;
	/*
	//Get system directory...
	char path[MAX_PATH];
	GetSystemDirectory(path, sizeof(path));

	//Get first Sndvol32 path
	char path2[MAX_PATH];
	sprintf(path2, "%s/Sndvol32.exe", path);

	//Cut off the file path...
	size_t i = strlen(path);
	while (path[i] != '/')
		--i;
	path[i] = '\0';

	//And... uhhh... get another path to try to open from?
	char path3[MAX_PATH];
	sprintf(path3, "%s/Sndvol32.exe", path);

	//Open Sndvol32 I guess?
	INT_PTR error1 = (INT_PTR)ShellExecute(hWnd, "open", path2, NULL, NULL, SW_SHOW);
	INT_PTR error2 = (INT_PTR)ShellExecute(hWnd, "open", path3, NULL, NULL, SW_SHOW);

	if (error1 <= 32 && error2 <= 32)
		return FALSE;
	else
		return TRUE;
	*/
}

void LoadGenericData()
{
	//Load surfaces
	MakeSurface_File("Pbm/MyChar.png", SURFACE_ID_MYCHAR);
	MakeSurface_File("Pbm/MyChar2.png", SURFACE_ID_MYCHAR2);
	MakeSurface_File("Pbm/MyChar3.png", SURFACE_ID_MYCHAR3);
	MakeSurface_File("Pbm/Msgbox.png", SURFACE_ID_MSGBOX);
	MakeSurface_File("Pbm/Cursor.png", SURFACE_ID_CURSOR);
	MakeSurface_File("Pbm/YesNo.png", SURFACE_ID_YESNO);
	MakeSurface_File("Pbm/Status.png", SURFACE_ID_STATUS);
	MakeSurface_File("Pbm/Figure.png", SURFACE_ID_FIGURE);
	MakeSurface_File("Pbm/Item.png", SURFACE_ID_ITEM);
	MakeSurface_File("Pbm/ItemBox.png", SURFACE_ID_ITEMBOX);
	MakeSurface_File("Pbm/Loading.png", SURFACE_ID_LOADING);
	MakeSurface_File("Pbm/PrtBack.png", SURFACE_ID_PRTBACK);
	MakeSurface_File("Pbm/PrtFilt.png", SURFACE_ID_PRTFILT);
	MakeSurface_File("Pbm/PrtDir.png", SURFACE_ID_PRTDIR);
	MakeSurface_File("Pbm/PrtBlock.png", SURFACE_ID_PRTBLOCK);
	MakeSurface_File("Pbm/PrtDmg.png", SURFACE_ID_PRTDMG);
	MakeSurface_File("Pbm/PrtItem.png", SURFACE_ID_PRTITEM);
	MakeSurface_File("Pbm/PrtSnack.png", SURFACE_ID_PRTSNACK);
	MakeSurface_File("Pbm/Back.png", SURFACE_ID_BACK);
	MakeSurface_File("Pbm/Hari.png", SURFACE_ID_HARI);
	MakeSurface_File("Pbm/Isogin.png", SURFACE_ID_ISOGIN);
	MakeSurface_File("Pbm/Kani.png", SURFACE_ID_KANI);
	MakeSurface_File("Pbm/Sleep.png", SURFACE_ID_SLEEP);
	MakeSurface_File("Pbm/Chibi.png", SURFACE_ID_CHIBI);
	MakeSurface_File("Pbm/Hoshi.png", SURFACE_ID_HOSHI);
	MakeSurface_File("Pbm/Dum.png", SURFACE_ID_DUM);
	MakeSurface_File("Pbm/Carry.png", SURFACE_ID_CARRY);
	MakeSurface_File("Pbm/Juel.png", SURFACE_ID_JUEL);
	MakeSurface_File("Pbm/Ufo.png", SURFACE_ID_UFO);
	MakeSurface_File("Pbm/Ironhead.png", SURFACE_ID_IRONHEAD);
	MakeSurface_File("Pbm/Star.png", SURFACE_ID_STAR);
	MakeSurface_File("Pbm/Bubble.png", SURFACE_ID_BUBBLE);
	MakeSurface_File("Pbm/Damage.png", SURFACE_ID_DAMAGE);
	MakeSurface_File("Pbm/LevelUp.png", SURFACE_ID_LEVELUP);
	MakeSurface_File("Pbm/Editor.png", SURFACE_ID_EDITOR);
	MakeSurface_File("Pbm/NpcType.png", SURFACE_ID_NPCTYPE);
	MakeSurface_File("Pbm/Opening.png", SURFACE_ID_OPENING);
	MakeSurface_File("Pbm/MaruAme.png", SURFACE_ID_MARUAME);
	MakeSurface_File("Pbm/Staff.png", SURFACE_ID_STAFF);
	MakeSurface_File("Pbm/End.png", SURFACE_ID_END);

	//Make generic surfaces
	MakeSurface_Generic(SURFACE_WIDTH, SURFACE_HEIGHT, SURFACE_ID_BACKUP);
	MakeSurface_Generic(272, 16, SURFACE_ID_TEXT0);
	MakeSurface_Generic(272, 16, SURFACE_ID_TEXT1);
	for (int i = 0; i < MAX_PSLINES; i++)
		MakeSurface_Generic(SURFACE_WIDTH, 16, i + SURFACE_ID_WORDS0);

	//Load sounds
	InitSoundObject("DASH", SOUND_ID_DASH);
	InitSoundObject("OUCH", SOUND_ID_OUCH);
	InitSoundObject("ITEM", SOUND_ID_ITEM);
	InitSoundObject("HITHEAD", SOUND_ID_HITHEAD);
	InitSoundObject("CRASH", SOUND_ID_CRASH);
	InitSoundObject("READY", SOUND_ID_READY);
	InitSoundObject("GO", SOUND_ID_GO);
	InitSoundObject("MESSAGE", SOUND_ID_MESSAGE);
	InitSoundObject("DEAD", SOUND_ID_DEAD);
	InitSoundObject("WIN", SOUND_ID_WIN);
	InitSoundObject("LEVELUP", SOUND_ID_LEVELUP);
	InitSoundObject("LIFEUP", SOUND_ID_LIFEUP);
	InitSoundObject("SAVE", SOUND_ID_SAVE);
	InitSoundObject("YESNO", SOUND_ID_YESNO);
	InitSoundObject("NODMG", SOUND_ID_NODMG);
	InitSoundObject("QUAKE", SOUND_ID_QUAKE);
	InitSoundObject("BOSSOUCH", SOUND_ID_BOSSOUCH);
	ChangeSoundFrequency(SOUND_ID_NODMG, 1200);
	ChangeSoundFrequency(SOUND_ID_QUAKE, 700);
	ChangeSoundFrequency(SOUND_ID_BOSSOUCH, 1200);
}