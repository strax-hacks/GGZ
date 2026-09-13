#include "globals.h"
#include "SYSTEM_Paths.h"

Mix_Chunk* AUDIO_CHUNK_Disk;
Mix_Chunk* AUDIO_CHUNK_Jump;
Mix_Chunk* AUDIO_CHUNK_Stomp;
Mix_Chunk* AUDIO_CHUNK_Killed;
Mix_Chunk* AUDIO_CHUNK_Ding;
Mix_Chunk* AUDIO_CHUNK_Click;
Mix_Chunk* AUDIO_CHUNK_GotPowerUp;
Mix_Chunk* AUDIO_CHUNK_Shoot;
Mix_Chunk* AUDIO_CHUNK_Crack;

Mix_Music* BGM_Title;
Mix_Music* BGM_Outdoors;
Mix_Music* BGM_Indoors;
Mix_Music* BGM_Highscore;
Mix_Music* BGM_MultiSong;

int VolumePercentage_Music;
int VolumePercentage_Sound;
int ShowVolume_Music;
int ShowVolume_Sound;

// ##############################################
// ##############################################
// ##############################################

void AUDIO_Define()
{
  //*********************************
  // initialize sound for Cardputer ZERO / ES8389 audio codec
  // 44100Hz 16-bit stereo with 1024 chunk size for low-latency glitch-free playback
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    // Fallback to 22050Hz if 44.1kHz is unavailable
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
  }
  AUDIO_Volume_Change_Music(50, false);  // Default music volume 50%
  AUDIO_Volume_Change_Sound(50, false);  // Default SFX volume 50%
  Mix_AllocateChannels(16);  // Allocate 16 channels for concurrent sounds

  //*********************************

  Mix_FreeChunk(AUDIO_CHUNK_Disk); AUDIO_CHUNK_Disk = NULL;
  AUDIO_CHUNK_Disk  = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_DISK]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Jump); AUDIO_CHUNK_Jump = NULL;
  AUDIO_CHUNK_Jump  = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_JUMP]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Stomp); AUDIO_CHUNK_Stomp = NULL;
  AUDIO_CHUNK_Stomp = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_STOMP]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Killed); AUDIO_CHUNK_Killed = NULL;
  AUDIO_CHUNK_Killed  = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_KILLED]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Ding); AUDIO_CHUNK_Ding = NULL;
  AUDIO_CHUNK_Ding  = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_DING]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Click); AUDIO_CHUNK_Click = NULL;
  AUDIO_CHUNK_Click  = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_CLICK]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_GotPowerUp); AUDIO_CHUNK_GotPowerUp = NULL;
  AUDIO_CHUNK_GotPowerUp = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_GOTPOWERUP]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Shoot); AUDIO_CHUNK_Shoot = NULL;
  AUDIO_CHUNK_Shoot = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_SHOOT]).c_str());

  Mix_FreeChunk(AUDIO_CHUNK_Crack); AUDIO_CHUNK_Crack = NULL;
  AUDIO_CHUNK_Crack = Mix_LoadWAV(GetAssetPath(FileName.WavChunk[AUDIO_CRACK]).c_str());

  Mix_FreeMusic(BGM_Title); BGM_Title = NULL;
  BGM_Title = Mix_LoadMUS(GetAssetPath(FileName.BGM_Title).c_str());

  Mix_FreeMusic(BGM_Outdoors); BGM_Outdoors = NULL;
  BGM_Outdoors = Mix_LoadMUS(GetAssetPath(FileName.BGM_Outdoors).c_str());

  Mix_FreeMusic(BGM_Indoors); BGM_Indoors = NULL;
  BGM_Indoors = Mix_LoadMUS(GetAssetPath(FileName.BGM_Indoors).c_str());

  Mix_FreeMusic(BGM_Highscore); BGM_Highscore = NULL;
  BGM_Highscore = Mix_LoadMUS(GetAssetPath(FileName.BGM_Highscore).c_str());

  Mix_FreeMusic(BGM_MultiSong); BGM_MultiSong = NULL;
  BGM_MultiSong = Mix_LoadMUS(GetAssetPath(FileName.BGM_MultiSong).c_str());

}

// ##############################################
// ##############################################
// ##############################################

void AUDIO_Volume_Change_Music(int Percentage, bool ShowVolumeOverlay)
{
  int Value128 = 0;
  // VOLUME OF SAMPLES
  if(Percentage < 1){Value128 = 0; Percentage = 0;}

  if(Percentage > 99){Value128 = 128; Percentage = 100;}

  if(Percentage > 0 && Percentage < 100)
  {
    Value128 = (int)(128*Percentage/100);
  }

  VolumePercentage_Music = Percentage;
  GV.VolumeMusic = Value128;
  Mix_VolumeMusic(GV.VolumeMusic);

  if(ShowVolumeOverlay){ShowVolume_Music = 3;}
}

// ##############################################
// ##############################################
// ##############################################

void AUDIO_Volume_Change_Sound(int Percentage, bool ShowVolumeOverlay)
{
  int Value128 = 0;
  // VOLUME OF SAMPLES
  if(Percentage < 1){Value128 = 0; Percentage = 0;}

  if(Percentage > 99){Value128 = 128; Percentage = 100;}

  if(Percentage > 0 && Percentage < 100)
  {
    Value128 = (int)(128*Percentage/100);
  }

  VolumePercentage_Sound = Percentage;
  GV.VolumeSounds = Value128;
  Mix_Volume(-1, GV.VolumeSounds);

  if(ShowVolumeOverlay){ShowVolume_Sound = 3;}
}

// ##############################################
// ##############################################
// ##############################################

void AUDIO_Sound_Play(int SoundNumber)
{
  //Mix_PlayChannel(-1,WavChunk[soundNumber],0);
  if(GV.VolumeSounds > 0)
  {
    if(SoundNumber == AUDIO_DISK)       {Mix_PlayChannel(-1,AUDIO_CHUNK_Disk,0);}
    if(SoundNumber == AUDIO_JUMP)       {Mix_PlayChannel(-1,AUDIO_CHUNK_Jump,0);}
    if(SoundNumber == AUDIO_STOMP)      {Mix_PlayChannel(-1,AUDIO_CHUNK_Stomp,0);}
    if(SoundNumber == AUDIO_KILLED)     {Mix_PlayChannel(-1,AUDIO_CHUNK_Killed,0);}
    if(SoundNumber == AUDIO_DING)       {Mix_PlayChannel(-1,AUDIO_CHUNK_Ding,0);}
    if(SoundNumber == AUDIO_CLICK)      {Mix_PlayChannel(-1,AUDIO_CHUNK_Click,0);}
    if(SoundNumber == AUDIO_GOTPOWERUP) {Mix_PlayChannel(-1,AUDIO_CHUNK_GotPowerUp,0);}
    if(SoundNumber == AUDIO_SHOOT)      {Mix_PlayChannel(-1,AUDIO_CHUNK_Shoot,0);}
    if(SoundNumber == AUDIO_CRACK)      {Mix_PlayChannel(-1,AUDIO_CHUNK_Crack,0);}
  }

  if(SoundNumber == AUDIO_GOTPOWERUP)
  {
    GV.TempMusicVolume = GV.VolumeMusic;
    GV.VolumeMusic = 0;
    GV.TempMusicOffCounter = 1000;
    Mix_VolumeMusic(GV.VolumeMusic);
  }
}

// ##############################################
// ##############################################
// ##############################################

static int CurrentMusicNumber = -1;

void AUDIO_Music_Halt()
{
  Mix_HaltMusic();
  CurrentMusicNumber = -1;
}

void AUDIO_Music_Play(int MusicNumber)
{
  if(GV.VolumeMusic > 0 && GV.MusicEnabled)
  {
    if (CurrentMusicNumber == MusicNumber && Mix_PlayingMusic())
    {
      return;
    }

    // PLAY MUSIC
    if(MusicNumber == MUSIC_MENU && BGM_Title)           {Mix_PlayMusic(BGM_Title,-1); CurrentMusicNumber = MUSIC_MENU;}
    else if(MusicNumber == MUSIC_OUTDOORS && BGM_Outdoors)   {Mix_PlayMusic(BGM_Outdoors,-1); CurrentMusicNumber = MUSIC_OUTDOORS;}
    else if(MusicNumber == MUSIC_INDOORS && BGM_Indoors)     {Mix_PlayMusic(BGM_Indoors,-1); CurrentMusicNumber = MUSIC_INDOORS;}
    else if(MusicNumber == MUSIC_HIGHSCORE && BGM_Highscore) {Mix_PlayMusic(BGM_Highscore,-1); CurrentMusicNumber = MUSIC_HIGHSCORE;}
    else if(BGM_MultiSong)
    {
      // Fallback if individual track not loaded: play multi-song at specific song position
      Mix_PlayMusic(BGM_MultiSong, -1);
      if(MusicNumber == MUSIC_MENU)           {Mix_SetMusicPosition(0.0); CurrentMusicNumber = MUSIC_MENU;}
      else if(MusicNumber == MUSIC_OUTDOORS)   {Mix_SetMusicPosition(25.0); CurrentMusicNumber = MUSIC_OUTDOORS;}
      else if(MusicNumber == MUSIC_INDOORS)    {Mix_SetMusicPosition(50.0); CurrentMusicNumber = MUSIC_INDOORS;}
      else if(MusicNumber == MUSIC_HIGHSCORE)  {Mix_SetMusicPosition(67.0); CurrentMusicNumber = MUSIC_HIGHSCORE;}
    }
  }
}
