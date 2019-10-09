#ifndef SOUND_H
#define SOUND_H

//basic includes
#include <iostream>
#include <string>
#include <windows.h>
#include <map>
//fmod api
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>

using namespace std;


//The array of sounds/channels/anything fmod needs to be known before hand, which isn't a problem,
//when You have designed the Sound manager, sounds are pre-defined per level. No Instantaneous sounds created. 
#define Max_Sounds		2		//songs
#define Max_SoundEff	1		//sound effects
#define Max_Channels	2		//channels for songs
#define Max_3D_Sounds	3		//3D song/sounds
#define Max_3D_Channels	3		//3D song/sound channels
#define Max_DSP_EFFECTS	4		//DSP Effects such as ECHO
#define Max_Reverbs		6		//3D Reverb effects

//Singleton Class, to make sure only one instance of Fmod API Sound System is created
class Sound
{
public:
//Singleton Design Pattern	
////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Sound* Instance(); //function to get single instance of class
	//public destructor
	~Sound();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//enum data to hold the IDs of each sound, easier to use and keep track of
	//struct AllSoundID
	//{

		enum SoundEffects
		{
			//Placeholders until sounds decided
			SoundEffect1 = 0, 
			SoundEffect2, 
			SoundEffect3, 
			SoundEffect4,
			SoundEffect5
		} /*sound_effects*/;

		enum Songs
		{
			//Placeholders until songs decided
			Song1 = 0, 
			Song2, 
			Song3, 
			Song4, 
			Song5
		} /*songs*/;

		enum Songs3D
		{
			//Placeholders until songs decided
			Song3D1 = 0, 
			Song3D2, 
			Song3D3, 
			Song3D4, 
			Song3D5
		} /*songs_3D*/;

		enum DSP_Effects
		{
			Echo = 0,
			Flange, 
			Distortion,
			Pitch
		} /*dsp_effects*/;

		enum Reverb_Effects
		{
			ConcertHall = 0,
			Alley, 
			UnderWater,
			Hangar,
			Forest,
			BathRoom
		} /*dsp_effects*/;
		//The one for white noise, sin waves and such is not necessary, since that is an Internal FMOD sound and needs to be set specifically
	/*};*/

//////////////////////////////////////////////////////////////////////////////////////////////////////	
//Initial Functions
	void Update();
	void Initialize();
	void Release();

	//you will need the file name and the mode it will be in such as FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D
	void LoadAllSongs();
	void CreateStream(const char* fileName, FMOD_MODE Mode, Songs );
	void CreateSound(const char* fileName, FMOD_MODE Mode, SoundEffects );
	void Create3DSound(const char* fileName, FMOD_MODE Mode, Songs3D );
	void InitSounds();
	void InitChannel();
	void InitDSPEffects();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//3D Sound Settings

	void Init3DSound();

	void Set3DSoundPosition(Songs3D songs_3D, FMOD_VECTOR velocity, FMOD_VECTOR position);
	void Set3DListener();
	//should take vector of forward, up, velocity, and position, mainly position
	void Set3DListPosBasedOnGame();
	void UpdateListener();

	//individual plane movements///////////////////////////////////////////////////////////////
	void MoveListenerXL();
	void MoveListenerXR();
	void MoveListenerYL();
	void MoveListenerYR();
	void MoveListenerZL();
	void MoveListenerZR();
	//this one will take the movement data from the player object and move the listender accordingly
	void MoveListenerXYZPos(float X, float Y, float Z);


//ADDITIONAL PREFERENCES SETTINGS for 3D sound
	void CreateConeSound();
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Pause/unpause/stop songs,sounds, etc

	void StopSound(Songs songs);
	void PauseSound(Songs songs);
	void UnPauseSound(Songs songs);	
	void StopAllSongs();
	void ResetSound(Songs songs);
	/////////////////////////////////////////////////////////////
	void PlaySoundEffect(SoundEffects sound_effects);
	bool TogglePause(Songs songs);
	bool TogglePauseEffect();
	bool TogglePause3D(Songs3D songs_3D);

//VOLUME FUNCTIONS

	void	SetSongVolume(Songs songs, float volume);
	float	GetSongVolume(Songs songs);
	void	Set3DSongVolume(Songs3D songs_3D, float volume);	//not complete
	float	Get3DSongVolume(Songs3D songs_3D);					//not complete
	void	SetMasterVolume(float volume);
	float	GetMasterVolume();	
	//////////////////////////////////////////////////////////////////////
	void SongVolumeUp(Songs songs);
	void SongVolumeDown(Songs songs);
	void Song3DVolumeUp(Songs3D songs_3D);		//not complete
	void Song3DVolumeDown(Songs3D songs_3D);	//not complete

//PAN SPEAKER FUNCTION
	//in order to avoid making doubles for 2d and 3d sound, I may need to pass in the type of channel
	void	SetPan(Songs songs, float Pan);
	float	GetPan(Songs songs);
	void	PanForwards(Songs songs);
	void	PanBackwards(Songs songs);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EFFECTS FUNCTIONS
	void GetEffect();				//not complete
	void WhiteNoise(bool paused);

//DSP EFFECTS SETTINGS	
	void EchoToggle();
	void FlangeToggle();
	void DistortionToggle();
	void PitchToggle();
	
//FADE FUNCTIONS
	void TransitionSongs(Songs songs1, Songs songs2/*, int milsec, float target, bool pauseWhenDone*/);
	void FadeInSong(Songs songs);
	void FadeOutSong(Songs songs);
	//virtual void UpdateFade(int ID);

	//REVERB FUNCTIONS
	void InitReverb();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//private initializer
	Sound();
	Sound(Sound const&) {};
	Sound& operator=(Sound const&){};
	static Sound* m_pSoundInstance; //static pointer to single instance of class

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//FMOD API

	FMOD::System		*system;

	FMOD::DSP			*dsp[1];

	FMOD::Sound			*soundEffect[Max_SoundEff];
	FMOD::Sound			*sound[Max_Sounds];						//array of sounds, with an ID
	FMOD::Sound			*sound3D[Max_3D_Sounds];
	
	FMOD::Channel		*channel[Max_Channels];					//array of channels with an ID to determine which
	FMOD::Channel		*channelSound[Max_SoundEff];
	FMOD::Channel		*channel3D[Max_3D_Channels];
	
	FMOD::Channel		*DSP[1];								//This one is for white noise and such, creates new sound
	
	FMOD::DSP			*DSP_SoundEffects[Max_DSP_EFFECTS];		//This one is to change a sounds effect, cause echo etc
	
	FMOD::SoundGroup	*songGroup; 
	
	FMOD::ChannelGroup	*ChannelMusic, *ChannelEffects, *Channel3DSound, *ChannelSound;
	FMOD::ChannelGroup	*MasterGroup;
	
	FMOD::Reverb		*reverb[Max_Reverbs];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//VARIABLES
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//FMOD TESTING Variables
	FMOD_RESULT			result;
	unsigned int		version;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//3D Parameters
	int			Max_Distance_Listener;
	int			Min_Distance_Listener;
	int			INTERFACE_UPDATETIME		/*const= 50*/;			// 50ms update for interface
	float		DISTANCE_FACTOR			/*const= 1.0f*/;         // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
	float		DOPPLER_SCALE			/*const= 1.0f*/;
	float		ROLLOFF_SCALE			/*const= 1.0f*/;
	float		timer;
	//bool ListenerFlag;
	FMOD_VECTOR		forward;			
	FMOD_VECTOR		up;					
	FMOD_VECTOR		velocity;		
	FMOD_VECTOR		listenerPosition;
	FMOD_VECTOR		lastPostition;
	//for cone sounds
	FMOD_VECTOR		Orientation;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FADE	/ TRANSITIONS parameters, for music to fade in and out when beginning and ending
	
	int		FadeLength1;
	int		FadeLength2;
	int		FadeStartTick1;
	int		FadeStartTick2;
	float	FadeStartVol1;
	float	FadeStartVol2;
	float	FadeTargetVol1;
	float	FadeTargetVol2;
	float	CurrentVolume1;
	float	CurrentVolume2;

	bool	FadeBoolIn;
	bool	FadeBoolOut;
	bool	FadePauseAfter;
};





#endif