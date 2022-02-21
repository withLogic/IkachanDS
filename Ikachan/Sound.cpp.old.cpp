#include "Sound.h"
#include <stdlib.h>
#include <cstring>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/audioout.h>

//DirectSound objects
int backgroundMusicPort;
SOUNDBUFFER* lpPRIMARYBUFFERBGM[16];

//DirectSound buffers
#define SE_MAX 512
SOUNDBUFFER* lpSECONDARYBUFFER[SE_MAX];

//Keep track of all existing sound buffers
SOUNDBUFFER *soundBuffers;

//Vita hardware
int freqs[] = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};
int audioSize = 16384;
int audioSampleRate = 24000;
int audioVolume = SCE_AUDIO_VOLUME_0DB;
bool start_audio = false;
bool stop_audio = false;

#define NUM_CHANNELS 16

char channelStates[NUM_CHANNELS] = {0};

int8_t getFreeChannel(void)
{
	for(uint8_t i=0; i<16; i++)
	{	
		if(!channelStates[i])
		{
			return i;
		}
			
	}
	return -1;
}

//Sound buffer code
SOUNDBUFFER::SOUNDBUFFER(size_t bufSize)
{
	//Set parameters
	size = bufSize;
	
	playing = false;
	looping = false;
	looped = false;

	timer = 0;
	
	frequency = 44100;
	volume = 127;
	pan = 63;
	samplePosition = 0;
	channelId = -1;
	
	//Create waveform buffer
	data = new s8[bufSize];
	memset(data, 0x80, bufSize);
	
	//Add to buffer list
	this->next = soundBuffers;
	soundBuffers = this;
}

void SOUNDBUFFER::Lock(s8 **outBuffer, size_t *outSize)
{
	if (outBuffer != NULL)
		*outBuffer = data;

	if (outSize != NULL)
		*outSize = size;
}

void SOUNDBUFFER::Unlock()
{
	//Nothing
}

void SOUNDBUFFER::SetCurrentPosition(uint32_t dwNewPosition)
{
	samplePosition = dwNewPosition;
}

void SOUNDBUFFER::SetFrequency(uint32_t dwFrequency)
{
	frequency = dwFrequency;
	if (channelId == -1) return;
	sceAudioOutSetConfig(channelId, -1, frequency, (SceAudioOutMode)-1);
}

void SOUNDBUFFER::SetVolume(int32_t lVolume)
{
	//float vol = MillibelToVolume(lVolume);
	//volume = (vol * 127);

	//if (channelId == -1) return;
	volume = lVolume;
}

void SOUNDBUFFER::Play(bool bLooping)
{	
	if (!data) return;

	playing = true;
	looping = bLooping;

	if (channelId != -1 && size < 257 && !looping){
		playing = false;
		channelId = -1;
		return;
	}

	if (channelId == -1)
	{
		channelId = getFreeChannel();
	}

	if(channelId == -1)
	{
		return;
	}

	channelStates[channelId] = 1;
	lpPRIMARYBUFFERBGM[channelId]->data = data;
}

void SOUNDBUFFER::Stop()
{
	playing = false;
	if(channelId != -1)
	{
		lpPRIMARYBUFFERBGM[channelId]->data = NULL;
	}
	channelId = -1;
}

int backgroundAudioThreadMethod(int args, void *argp)
{
	backgroundMusicPort = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, 1024, 44100, SCE_AUDIO_OUT_MODE_MONO);
	sceAudioOutSetConfig(backgroundMusicPort, -1, -1, SCE_AUDIO_OUT_MODE_MONO);
	int volume[] = {audioVolume, audioVolume};
    sceAudioOutSetVolume(backgroundMusicPort, (SceAudioOutChannelFlag)(SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH), volume);
	
	while (!stop_audio)
    {
		for (int i = 0; i < 16; i++)
		{
			if(sceAudioOutOutput(backgroundMusicPort, lpPRIMARYBUFFERBGM[i]->data) == 0)
			{
				channelStates[i] = 0;
			}
		}		
    }
	 
	sceAudioOutReleasePort(backgroundMusicPort);
    free(lpPRIMARYBUFFERBGM[0]->data);
	
    sceKernelExitDeleteThread(0);
    return 0;
}

BOOL InitDirectSound(){

	// clear out the secondary buffer
	for (int i = 0; i < SE_MAX; i++){
		lpSECONDARYBUFFER[i] = NULL;
	}

	// create audio buffers?
	for (int i = 0; i < 16; i++)
	{
		lpPRIMARYBUFFERBGM[i] = new SOUNDBUFFER(audioSize);
	}

	SceUID backgroundAudioThread = sceKernelCreateThread("Audio Thread", (SceKernelThreadEntry)backgroundAudioThreadMethod, 0x10000100, 0x10000, 0, 0, NULL);
	int res1 = sceKernelStartThread(backgroundAudioThread, sizeof(backgroundAudioThread), &backgroundAudioThread);
	if (res1 != 0){
		printf("Failed to init background audio thread (0x%x)", res1);
		return false;
	}

	return true; 
};

void EndDirectSound() {

	for (int i = 0; i < SE_MAX; i++) 
	{
		if (lpSECONDARYBUFFER[i] != NULL)
		{
			//lpSECONDARYBUFFER[i]->Release();
			lpSECONDARYBUFFER[i] = NULL;
		}
	}

	if(start_audio){
		stop_audio = true;
		backgroundMusicPort = -1;
	}
};

void ReleaseSoundObject(int no){
	if (lpSECONDARYBUFFER[no] != NULL)
	{
		//lpSECONDARYBUFFER[no]->Release();
		lpSECONDARYBUFFER[no] = NULL;
	}
};

BOOL InitSoundObject(const char* resname, int no) {
    //Get file path
    char path[MAX_PATH];
    sprintf(path, "%s//Wave//%s.raw", gModulePath, resname);
	
    //Open file
    FILE *fp = fopen(path, "rb");
    if (fp == NULL)
        return FALSE;
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

    //Read file
    signed char *data = (signed char *)malloc(size);
    if (data == NULL)
    {
        fclose(fp);
        return FALSE;
    }

    fread(data, size, 1, fp);
    fclose(fp);

    //Create buffer
    lpSECONDARYBUFFER[no] = new SOUNDBUFFER(size);
    if (lpSECONDARYBUFFER[no] == NULL)
        return FALSE;

    //Upload data to buffer
    s8 *buf;
    lpSECONDARYBUFFER[no]->Lock(&buf, NULL);
    
	memcpy(buf, data, size);
    
	lpSECONDARYBUFFER[no]->Unlock();

	free(data);

	return true;
};

void PlaySoundObject(int no, int mode) {
	if (lpSECONDARYBUFFER[no] != NULL)
	{
		switch (mode)
		{
			case SOUND_MODE_STOP: //Stop
				lpSECONDARYBUFFER[no]->Stop();
				break;

			case SOUND_MODE_PLAY: //Play
				lpSECONDARYBUFFER[no]->Stop();
				lpSECONDARYBUFFER[no]->SetCurrentPosition(0);
				lpSECONDARYBUFFER[no]->Play(false);
				break;

			case SOUND_MODE_PLAY_LOOP: //Play looped
				lpSECONDARYBUFFER[no]->Play(true);
				break;
		}
	}
};

void ChangeSoundFrequency(int no, DWORD rate) {
	if (lpSECONDARYBUFFER[no] != NULL)
		lpSECONDARYBUFFER[no]->SetFrequency((rate * 10) + 100);
};

void ChangeSoundVolume(int no, long volume) {
	if (lpSECONDARYBUFFER[no] != NULL)
		lpSECONDARYBUFFER[no]->SetVolume((volume - 300) * 8);
};

void ChangeSoundPan(int no, long pan) {

};

//PiyoPiyo sound
int freq_tbl[12] = { 1551, 1652, 1747, 1848, 1955, 2074, 2205, 2324, 2461, 2616, 2770, 2938 };

/*
BOOL MakePiyoPiyoSoundObject(CHAR *wave, BYTE *envelope, int octave, int data_size, int no) {
	BOOL result;
	int i;

	//Construct sound buffers
	for (i = 0; i < 24; i++) {
		lpSECONDARYBUFFER[no + i] = new SOUNDBUFFER(data_size);
	}

	//Precalculate envelope
    WORD *pre_env = (WORD*)malloc(data_size * sizeof(WORD));
    WORD *pre_env_p = pre_env;
    for (int j = 0; j < data_size; j++)
        *pre_env_p++ = ((WORD)envelope[(j << 6) / data_size]) << 1;
    
    //Write sound data
    WORD *wp = (WORD*)malloc(data_size * sizeof(WORD));
    WORD *wpp;
    
    for (i = 0; i < 24; i++)
    {
        //Get frequency
        int freq;
        if (i < 12)
            freq = octave * freq_tbl[i] / 16;
        else
            freq = octave * freq_tbl[i - 12] / 8;
        
        //Construct waveform
        union
        {
            WORD word;
            struct
            {
                BYTE lower;
                BYTE upper;
            } fixed;
        } wp_sub = { 0 };
        pre_env_p = pre_env;
        wpp = wp;
        
        for (int j = 0; j < data_size; j++)
        {
            //Set sample and increment sub-pos
            *wpp++ = (wave[wp_sub.fixed.upper]) * (*pre_env_p++);
            wp_sub.word += freq;
        }
        unsigned char *buf;
   		lpSECONDARYBUFFER[no + i]->Lock(&buf, NULL);
		
        //Copy waveform
        wpp = wp;
        for (int v = 0; v < data_size; v++)
            *buf++ = ((*wpp++ >> 8));
        
		lpSECONDARYBUFFER[no + i]->Unlock();
    	lpSECONDARYBUFFER[no + i]->SetFrequency(22050);
        
    }
    
    //Check if there was an error and free wave buffer
    if (i == 24)
        result = TRUE;

    free(wp);
    free(pre_env);
    return result;
}; 
*/

BOOL MakePiyoPiyoSoundObject(CHAR *wave, BYTE *envelope, int octave, int data_size, int no)
{
	return true;
}