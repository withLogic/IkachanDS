#include "Sound.h"
#include "System.h"
#include <stdint.h>
#include <stdio.h>

extern "C" {
    #include "vitaAudio/vitaAudio.h"
}

//DirectSound objects
vaudio lpPRIMARYBUFFER;

//DirectSound buffers
#define SE_MAX 512
vaudio lpSECONDARYBUFFER[SE_MAX];

//Sound buffer code
SOUNDBUFFER::SOUNDBUFFER(size_t bufSize)
{
	// Do nothing
}

SOUNDBUFFER::~SOUNDBUFFER()
{
	// Do nothing
}

void SOUNDBUFFER::Release()
{
	// Do nothing
}

void SOUNDBUFFER::Lock(s8 **outBuffer, size_t *outSize)
{
	// Do nothing
}

void SOUNDBUFFER::Unlock()
{
	//Nothing
}

void SOUNDBUFFER::SetCurrentPosition(uint32_t dwNewPosition)
{
	// Do nothing
}

void SOUNDBUFFER::SetFrequency(uint32_t dwFrequency)
{
	// Do nothing
}

void SOUNDBUFFER::SetVolume(int32_t lVolume)
{
	// Do nothing
}

void SOUNDBUFFER::SetPan(int32_t lPan)
{
	// Do nothing
}

void SOUNDBUFFER::Play(bool bLooping)
{
	// Do nothing
}

void SOUNDBUFFER::Stop()
{
	// Do nothing
}

//DirectSound interface
BOOL InitDirectSound()
{
	return TRUE;
}

void EndDirectSound()
{
	// Do nothing
}

//Sound object creation and release
void ReleaseSoundObject(int no)
{
	// Do nothing
}

BOOL InitSoundObject(const char* resname, int no)
{
	return TRUE;
}

void PlaySoundObject(int no, int mode)
{
	// Do nothing
}

void ChangeSoundFrequency(int no, DWORD rate) //100 is MIN, 9999 is MAX, and 2195 is normal
{
	// Do nothing
}

void ChangeSoundVolume(int no, long volume) //300 is MAX and 300 is normal
{
	// Do nothing
}

void ChangeSoundPan(int no, long pan) //512 is MAX and 256 is normal
{
	// Do nothing
}

//PiyoPiyo sound
int freq_tbl[12] = { 1551, 1652, 1747, 1848, 1955, 2074, 2205, 2324, 2461, 2616, 2770, 2938 };

BOOL MakePiyoPiyoSoundObject(CHAR *wave, BYTE *envelope, int octave, int data_size, int no)
{
	return true;
}