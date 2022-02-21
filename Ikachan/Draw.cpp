#include "Draw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vita2d.h>
#include <psp2/ctrl.h>

#define MAX_SURFACE 512

//Global cliprect
RECT grcFull = {0, 0, SURFACE_WIDTH, SURFACE_HEIGHT};
SURFACE surf[MAX_SURFACE];

//Draw state
int mag;
BOOL fullscreen;

SURFACE frontBuffer;
vita2d_texture *backBuffer;

//Client
int client_x;
int client_y;

// vita stuff
vita2d_pgf *pgf;
vita2d_pvf *pvf;
SceCtrlData pad;

void SetClientOffset(int width, int height){
    client_x = width;
    client_y = height;
};

void UpdateInput(){
    sceCtrlPeekBufferPositive(0, &pad, 1);

    gKey = 0;

    if (pad.lx > 170){ printf("Right."); }
    if (pad.lx < 85) { printf("Left."); };
    if (pad.ly > 170){ printf("Down."); }
    if (pad.ly < 85) { printf("Up."); }

	gKey |= (pad.buttons & SCE_CTRL_UP) ? CEY_UP : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_RIGHT) ? CEY_RIGHT : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_DOWN) ? CEY_DOWN : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_LEFT) ? CEY_LEFT : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_CROSS) ? CEY_Z : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_CIRCLE) ? CEY_X : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_START) ? CEY_S : 0 ;
	gKey |= (pad.buttons & SCE_CTRL_TRIANGLE) ? CEY_SPACE : 0 ;

	gMouse = 0;
}

BOOL Flip_SystemTask(void){
    // update our inputs before drawing
    UpdateInput();

    vita2d_wait_rendering_done();
    vita2d_end_drawing();

	vita2d_swap_buffers();	
    vita2d_start_drawing();
	vita2d_clear_screen();
    return true;
};

BOOL StartDirectDraw(){
    vita2d_init();
    vita2d_set_clear_color(RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();

    memset(&pad, 0, sizeof(pad));

    return true;
};

void EndDirectDraw() {
    vita2d_fini();
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);
};

BOOL MakeSurface_File(const char *name, int surf_no){
    char path[MAX_PATH];
    sprintf(path, "%s//%s", gModulePath, name);
    
	//Make sure a surface can be made here
	if (surf_no > MAX_SURFACE) { return FALSE; };
    if ( surf[surf_no].texture != NULL){
        return false;
    }

    printf("LoadPng_File %s\n", path);
    surf[surf_no].texture = vita2d_load_PNG_file(path);
    surf[surf_no].w = 320;
    surf[surf_no].h = 128;

    return true;
};

BOOL MakeSurface_Generic(int bxsize, int bysize, int surf_no){
    // Nothing to do
    return true;
};

void BackupSurface(int surf_no, const RECT *rect){
    // Nothing to do
};

void PutBitmap3(const RECT *rcView, int x, int y, const RECT *rect, int surf_no){

    // exit if we do not have the surface loaded
    if ( surf[surf_no].texture == NULL){
        printf("PutBitmap3 SURFACE NOT LOADED: %i \n", surf_no);
        return;
    }
    
    vita2d_draw_texture_part_scale(surf[surf_no].texture, x * SCALE_X, y * SCALE_Y, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SCALE_X, SCALE_Y);
};

void CortBox(const RECT *rect, DWORD col){
	//const unsigned char col_red = col & 0x0000FF;
	//const unsigned char col_green = (col & 0x00FF00) >> 8;
	//const unsigned char col_blue = (col & 0xFF0000) >> 16;

    //vita2d_draw_rectangle(rect->left, rect->top, rect->right * SCALE_X, rect->bottom * SCALE_Y, RGBA8(col_red, col_green, col_blue, 255));
    //vita2d_draw_rectangle(680, 350, 100, 150, RGBA8(0, 0, 255, 255));
};

void CortBox2(const RECT *rect, DWORD col, int surf_no){
    // Nothing to do
};

void InitTextObject(){
    MakeSurface_File("Pbm/smalfont.png", SURFACE_ID_FONT);
};

void PutText(RECT* rcView, int x, int y, const char *text, unsigned long color){
    char v;

    const unsigned char col_red = color & 0x0000FF;
	const unsigned char col_green = (color & 0x00FF00) >> 8;
	const unsigned char col_blue = (color & 0xFF0000) >> 16;
    color = RGBA8(col_red, col_green, col_blue, 255);

    while (v = *text++)
    {
        if ((v -= 0x20) >= 0x00 && v <= 0x60)
        {
            RECT rect = {(v & 0x1F) << 3, (v / 32) * 12, ((v & 0x1F) + 1) << 3, ((v / 32) + 1) * 12};
            vita2d_draw_texture_tint_part_scale(surf[SURFACE_ID_FONT].texture, x * SCALE_X, y * SCALE_Y, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SCALE_X, SCALE_Y, color);

            x += font_space[v];
        }
    }
}

void PutText2(int x, int y, const char *text, unsigned long color, int surf_no){
    // Nothing to do
};

void EndTextObject(){
    // Nothing to do
};