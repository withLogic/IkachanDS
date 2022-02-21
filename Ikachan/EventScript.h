#pragma once

#include "Map.h"
#include "NpChar.h"
#include "Item.h"
#include "PiyoPiyo.h"
#include "Effect.h"
//#include <windows.h>
#include "WindowsWrapper.h"

enum FADE_MODE
{
	FADE_MODE_NONE,
	FADE_MODE_QUAKE,
	FADE_MODE_FADEOUT,
	FADE_MODE_FADEIN,
	FADE_MODE_QUAKE2,
};

struct FADE
{
	bool mask;
	BYTE mode;
	int wait;
};

struct EVENT_SCR
{
	char mode; //x0
	//alignment x1
	//alignment x2
	//alignment x3
	char* data; //x4
	bool msg_box; //x8
	//alignment x9
	//alignment xA
	//alignment xB
	DWORD size; //xC
	int event_no; //x10
	DWORD p_read; //x14
	BYTE ani_cursor; //x18
	//alignment x19
	short wait; //x1A
	short x1C; //x1C
	char line; //x1E
	char ypos_line[2]; //x1F, x20
	char p_write; //x21
	bool select; //x22
};

BOOL ProcFade(FADE *fade, FRAME *frame, CARET_SPAWNER *caret_spawner);
void PutNumber(int x, int y, int no);
void PutNumber2(int x, int y, int no);
void DebugPutText(const char* text);
BOOL ReadEventScript(const char* path, EVENT_SCR *ptx);
BOOL JumpEventScript(EVENT_SCR *ptx);
short GetEventScriptNo(EVENT_SCR *ptx);
void PutEventScriptCursor(EVENT_SCR *ptx);
char EventScriptProc(EVENT_SCR *ptx, ITEMS *items, NPCHAR *npc, MAP *map, PIYOPIYO_CONTROL *piyocont, FADE *fade, FRAME *frame);
void PutMsgBox(EVENT_SCR *ptx);
void InitEventScript(EVENT_SCR *ptx);
