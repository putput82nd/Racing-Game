#include "..\Headers\Sound.h"


//This error check will show me the FMOD specific error if there is one, helps to track down who the culprit is!
//incredibly useful!			list of errors in fmod_errors.hpp
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
       // exit(-1);
    }
}

Sound* Sound::Instance()
{
	static Sound* m_pSoundInstance = new Sound();

	return m_pSoundInstance;

	//static Sound temp;
	//return &temp;
}

//Sound Init - in private
Sound::Sound()
{
	Initialize();
}

Sound::~Sound()
{
	Release();
}

void Sound::Update()
{
	system->update();
}

//Main initialize, where everything gets done and set!
void Sound::Initialize()
{
//create system
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
    ERRCHECK(result);

    if(version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    }

	////init system
	//PLUS ONE FOR THE WHITE NOISE CHANNEL
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	// Set stream size higher than the default (16384) to try to help reduce stuttering
	system->setStreamBufferSize(65536, FMOD_TIMEUNIT_RAWBYTES);
		   
    //Create an oscillator DSP units for the tone.   
    result = system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp[0]);
    ERRCHECK(result);
    result = dsp[0]->setParameter(FMOD_DSP_OSCILLATOR_RATE, 440.0f);       /* musical note 'A' */
    ERRCHECK(result);

	////Load all the sounds and set the fmod mode, such as 3D or not
	LoadAllSongs();

	InitChannel();

	//initialize the sounds and set them to stop/pause
	InitSounds();

	WhiteNoise(true);

	Init3DSound();

	InitDSPEffects();

	//InitReverb();		
}

void Sound::Release()
{
	//Release all the sounds that have been created
	for(int i = 0; i < Max_Sounds; i++)
	{
		result = sound[i]->release();
		ERRCHECK(result);
	}

	for(int i = 0; i < Max_3D_Sounds; i++)
	{
		result = sound3D[i]->release();
		ERRCHECK(result);
	}

	for(int i = 0; i < Max_SoundEff; i++)
	{
		result = soundEffect[i]->release();
		ERRCHECK(result);
	}

	for(int i = 0; i < Max_DSP_EFFECTS; i++)
	{
		result = DSP_SoundEffects[i]->release();
		ERRCHECK(result);
	}
	
	result = dsp[0]->release();
    ERRCHECK(result);

	//sound effect dsp do not have to be released, just the sounds
		
	//channels technically don't even have to be released, just the sounds. You can release almost everything just to be cautious if you want
	result = ChannelMusic->release();
    ERRCHECK(result);
    result = ChannelEffects->release();
    ERRCHECK(result);	
	result = Channel3DSound->release();
    ERRCHECK(result);
	result = ChannelSound->release();
    ERRCHECK(result);

	result = MasterGroup->release();
    ERRCHECK(result);		

	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This is where you should define the songs to be loaded
void Sound::LoadAllSongs()
{
	//Create the sounds
	CreateStream("..\\Assets//Sounds//Music//Week4Demo//DDR-FastTechnoFromHell.mp3",	FMOD_SOFTWARE | FMOD_LOOP_NORMAL, Song1);
	CreateStream("..\\Assets//Sounds//Music//Week4Demo//Professional_Griefers.wma", FMOD_SOFTWARE | FMOD_LOOP_NORMAL, Song2);
	//CreateStream("Sounds//Music//Week4Demo//Imaginary_Space.mp3",	FMOD_SOFTWARE | FMOD_LOOP_NORMAL, Song3);

	CreateSound("..\\Assets//Sounds//Effects//PortalGunWave.wav",	FMOD_SOFTWARE | FMOD_2D, SoundEffect1);

	//Load in the 3D sounds, this can also be called with the rest of Load Sounds, but in order to show a difference, called in 3D init. 
	//Create3DSound("Sounds//Music//Week4Demo//DDR-FastTechnoFromHell.mp3" , FMOD_3D | FMOD_LOOP_NORMAL, Song3D1);
	//Create3DSound("Sounds//Music//Week4Demo//DDR-FastTechnoFromHell.mp3" , FMOD_3D | FMOD_LOOP_NORMAL, Song3D2);
	//Create3DSound("Sounds//Music//Week4Demo//DDR-FastTechnoFromHell.mp3" , FMOD_3D | FMOD_LOOP_NORMAL, Song3D3);
}

void Sound::CreateStream(const char* fileName, FMOD_MODE Mode, Songs songs)
{
	//Initialize the sounds, sound groups, channels
	result = system->createStream(fileName,	Mode, NULL,	&sound[songs]);
    ERRCHECK(result);
}

void Sound::CreateSound(const char* fileName, FMOD_MODE Mode, SoundEffects sound_effects)
{
	//Initialize the sounds, sound groups, channels
	result = system->createSound(fileName,	Mode, NULL,	&soundEffect[sound_effects]);
    ERRCHECK(result);
}

void Sound::Create3DSound(const char* fileName, FMOD_MODE Mode, Songs3D songs_3D)
{
	//Initialize the sounds, sound groups, channels
	result = system->createSound(fileName,	Mode, NULL,	&sound3D[songs_3D]);
    ERRCHECK(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sound::InitSounds()
{
	bool paused = true;

/////////////////////////////////////////////////////////
//For Music
	for(int i = 0; i < Max_Sounds; i++)
	{
		result = system->playSound(/*FMOD_CHANNEL_FREE*/FMOD_CHANNEL_REUSE, sound[i], paused, &channel[i]);
		ERRCHECK(result);

		//sets the channel group
		if(ChannelMusic)
		{
			result = channel[i]->setChannelGroup(ChannelMusic);
			ERRCHECK(result);
		}

		// Songs repeat forever by default
		result = channel[i]->setLoopCount(-1);
		ERRCHECK(result);

		// Flush buffer to ensure loop logic is executed
		result = channel[i]->setPosition(0, FMOD_TIMEUNIT_MS);
		ERRCHECK(result);

		// Set paused or not as applicable
		if (!paused)
		{
			result = channel[i]->setPaused(paused);
			ERRCHECK(result);
		}

	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For sound effects
	result = system->playSound(/*FMOD_CHANNEL_FREE*/FMOD_CHANNEL_REUSE, soundEffect[SoundEffect1], paused, &channelSound[SoundEffect1]);
	ERRCHECK(result);

	//sets the channel group for sounds
	if(ChannelSound)
	{
		result = channelSound[SoundEffect1]->setChannelGroup(ChannelSound);
		ERRCHECK(result);
	}

	// Set paused or not as applicable
	if (!paused)
	{
		result = channelSound[SoundEffect1]->setPaused(paused);
		ERRCHECK(result);
	}
	// Cancel any fade that was previously applied
	//FadeBool = false;
}

void Sound::InitChannel()
{
	//// Create two channel groups to allow master volume control
	//// One for music, one for effects
	result = system->createChannelGroup(NULL, &ChannelMusic);
	ERRCHECK(result);

	//ERRCHECK(result);
	result = system->createChannelGroup(NULL, &ChannelEffects);
	ERRCHECK(result);

	result = system->createChannelGroup(NULL, &Channel3DSound);
	ERRCHECK(result);

	result = system->createChannelGroup(NULL, &ChannelSound);
	ERRCHECK(result);

	////Master group holds all groups
	//with a master group, you will be able to control all the sounds added to it, and pan them all at once, or volume, etc. 
	result = system->getMasterChannelGroup(&MasterGroup);
    ERRCHECK(result);


	//Instead of being independent, set the group A and B to be children of the master group.    
    result = MasterGroup->addGroup(ChannelMusic);
    ERRCHECK(result);

    result = MasterGroup->addGroup(ChannelEffects);
	ERRCHECK(result);

	result = MasterGroup->addGroup(Channel3DSound);
	ERRCHECK(result);

	result = MasterGroup->addGroup(ChannelSound);
	ERRCHECK(result);	

	//Initialize all the channels and set then all to paused, there may be a better way to do this then all these for loops
	for(int i = 0; i < Max_Channels; i ++)
	{
		channel[i] = 0;
	}

	for(int i = 0; i < Max_3D_Channels; i ++)
	{
		channel3D[i] = 0;
	}

	//will have to do the same for effects later
	DSP[0] = 0;

	for(int i = 0; i < Max_DSP_EFFECTS; i++)
	{
		DSP_SoundEffects[i] = 0;
	
	}

	for(int i = 0; i < Max_SoundEff; i++)
	{
		channelSound[i] = 0;
	}
}

void Sound::InitDSPEffects()
{
	result = system->createDSPByType(FMOD_DSP_TYPE_ECHO, &DSP_SoundEffects[Echo]);
	ERRCHECK(result);

	result = system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &DSP_SoundEffects[Flange]);
	ERRCHECK(result);

	result = system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &DSP_SoundEffects[Distortion]);
	ERRCHECK(result);

	result = system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &DSP_SoundEffects[Pitch]);
	ERRCHECK(result);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sound::Init3DSound()
{
	//this is for distance sound
	///*const int    */INTERFACE_UPDATETIME	= 50;			// 50ms update for interface
	/*const float  */DISTANCE_FACTOR		= 1.0f;         // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
	/*const float  */DOPPLER_SCALE			= 1.0f;
	/*const float  */ROLLOFF_SCALE			= 1.0f;

	Max_Distance_Listener = 36;
	Min_Distance_Listener = -35;

	result = system->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);
 
	//Load in the 3D sounds, this can also be called with the rest of Load Sounds, but in order to show a difference, called here. 
	Create3DSound("..\\Assets//Sounds//Music//Week4Demo//DMC_Cerberus.mp3" , FMOD_SOFTWARE | FMOD_3D | FMOD_LOOP_NORMAL, Song3D1);
	Create3DSound("..\\Assets//Sounds//Music//Week4Demo//Imaginary_Space.mp3" , FMOD_SOFTWARE | FMOD_3D | FMOD_LOOP_NORMAL, Song3D2);
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Set this sound as a distance point sound
	for(int i = 0; i < 2; i++)
	{
		//may want to set a function that will take parameters to initialize this
		result = sound3D[i]->set3DMinMaxDistance(10.5f * DISTANCE_FACTOR, 5000.0f * DISTANCE_FACTOR);
		ERRCHECK(result);
	}

	FMOD_VECTOR pos1 = { -10.0f * DISTANCE_FACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel1 = {  0.0f, 0.0f, 0.0f };
	Set3DSoundPosition(Song3D1, vel1, pos1);
	FMOD_VECTOR pos2 = { 15.0f * DISTANCE_FACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel2 = { 0.0f, 0.0f, 0.0f };
	Set3DSoundPosition(Song3D2, vel2, pos2);
	//FMOD_VECTOR pos3 = { 15.0f * DISTANCE_FACTOR, 0.0f, 0.0f };
 //   FMOD_VECTOR vel3 = { 0.0f, 0.0f, 0.0f };
	//Set3DSoundPosition(Song3D3, vel3, pos3);

	Set3DListener();

	CreateConeSound();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3D DISTANCE SOUND
void Sound::Set3DSoundPosition(Songs3D songs_3D, FMOD_VECTOR velocity, FMOD_VECTOR position)
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound3D[songs_3D], true, &channel3D[songs_3D]);
	ERRCHECK(result);

	result = channel3D[songs_3D]->set3DAttributes(&position, &velocity);
	ERRCHECK(result);

	result = channel3D[songs_3D]->setPaused(true);
	ERRCHECK(result);
}

void Sound::Set3DListener()
{
	//initialize the full position of the listener object
	//it may be a good idea to make this it's own function but it takes in the data from the game play object
	lastPostition.x = 0.0f;		lastPostition.y = 0.0f;		lastPostition.z = 0.0f;
	forward.x  = 0.0f;			forward.y  = 0.0f;			forward.z  = 1.0f;
	up.x	   = 0.0f;			up.y	   = 1.0f;			up.z	   = 0.0f;
	velocity.x = 0.0f;			velocity.y = 0.0f;			velocity.z = 0.0f;
	listenerPosition.x = 0.0f;	listenerPosition.y = 0.0f;	listenerPosition.z = -1.0f * DISTANCE_FACTOR;
	timer = 0;

	//listener ID, pos, vel, forward, up
	result = system->set3DListenerAttributes(0, &listenerPosition, &velocity, &forward, &up);
	ERRCHECK(result);
}

void Sound::UpdateListener()
{
	velocity.x = (listenerPosition.x - lastPostition.x) * (1000 / INTERFACE_UPDATETIME);
	velocity.y = (listenerPosition.y - lastPostition.y) * (1000 / INTERFACE_UPDATETIME);
	velocity.z = (listenerPosition.z - lastPostition.z) * (1000 / INTERFACE_UPDATETIME);

    // store pos for next time
    lastPostition = listenerPosition;

	//listener ID, pos, vel, forward, up
	result = system->set3DListenerAttributes(0, &listenerPosition, &velocity, &forward, &up);
	ERRCHECK(result);

	timer += (30 * (1.0f / (float)INTERFACE_UPDATETIME));    // t is just a time value .. it increments in 30m/s steps in this example
}

//The listener can be moved in all 3 planes, for this first demo we will move it in just the X direction, but you can move it the same way as below
//I will add a function that will change all 3 at once depending on how the ship is changing
void Sound::MoveListenerXL()
{
	listenerPosition.x -= 1.0f * DISTANCE_FACTOR;
	//cout << " Listener Pos " << listenerPosition.x << endl;

	//ensure that the listener doens't go out of bounds
	if(listenerPosition.x < Min_Distance_Listener * DISTANCE_FACTOR)
	{
		listenerPosition.x = Min_Distance_Listener * DISTANCE_FACTOR;
	}

	UpdateListener();
}

void Sound::MoveListenerXR()
{
	listenerPosition.x += 1.0f * DISTANCE_FACTOR;
	//cout << " Listener Pos " << listenerPosition.x << endl;

	//ensure that the listener doens't go out of bounds
	if(listenerPosition.x > Max_Distance_Listener * DISTANCE_FACTOR)
	{
		listenerPosition.x = Max_Distance_Listener * DISTANCE_FACTOR;
	}

	UpdateListener();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADDITIONAL PREFERENCES
void Sound::CreateConeSound()
{
	Create3DSound("..\\Assets//Sounds//Music//Week4Demo//portal_still_alive.mp3" , FMOD_SOFTWARE | FMOD_3D | FMOD_LOOP_NORMAL, Song3D3);

	Orientation.x = 0.5f;	Orientation.y = 0.0f;	Orientation.z = 0.0f;

	
	result = system->playSound(FMOD_CHANNEL_FREE, sound3D[Song3D3], true, &channel3D[Song3D3]);
	ERRCHECK(result);

	result = channel3D[Song3D3]->set3DConeOrientation(&Orientation);
	ERRCHECK(result);

							//	Parameters
							//
							//Inside cone angle: in degrees, from 0 to 360. 
							//This is the angle within which the sound is at its normal volume. 
							//Must not be greater than outsideconeangle. Default = 360. 
							//
							//Outside cone angle: in degrees, from 0 to 360.
							//This is the angle outside of which the sound is at its outside volume. 
							//Must not be less than insideconeangle. Default = 360. 
							//
							//Cone outside volume, from 0 to 1.0. Default = 1.0. 
							
	result = channel3D[Song3D3]->set3DConeSettings(40.0f, 120.0f, 80.0f);
	ERRCHECK(result);

	//you can change the cone and have it move by using get, and re-setting it's orientation, position, or settings
	//Almost everything is customizable and instantaneous


	result = channel3D[Song3D3]->setPaused(true);
	ERRCHECK(result);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//Single Sound preferences
void Sound::StopSound(Songs songs)
{
	channel[songs]->stop();
	FadeBoolIn = false;
	FadeBoolOut = false;
}

void Sound::PauseSound(Songs songs)
{
	channel[songs]->setPaused(true);
	//FadeBoolIn = false;
	//FadeBoolOut = false;
}

void Sound::UnPauseSound(Songs songs)
{
	channel[songs]->setPaused(false);
}


void Sound::StopAllSongs()
{
	for(int i = 0; i <= Max_Channels; i++)
	{
		channel[i]->stop();
		//different loop for this if different
		channel3D[i]->stop();		
	}

	for(int i = 0; i < Max_DSP_EFFECTS; i++)
	{
		DSP_SoundEffects[Echo]->remove();
	}

	for(int i = 0; i < Max_SoundEff; i ++)
	{
		channelSound[i]->stop();
	}
	
	DSP[0]->stop();

	FadeBoolIn  = false;
	FadeBoolOut = false;
}

void Sound::ResetSound(Songs songs)
{
	StopSound(songs);

	result = system->playSound(/*FMOD_CHANNEL_FREE*/FMOD_CHANNEL_REUSE, sound[songs], true, &channel[songs]);
	ERRCHECK(result);
	
}

//Everything can be done either single sound or through using channel groups, or the master group. The setting just needs to be added as below
//Most are already available, but a few still need to be added. Thinking of making a new .cpp specifically for each feature
//This takes up a lot of space.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Pause or Stop Song, individual
void Sound::PlaySoundEffect(SoundEffects sound_effects)
{
	result = system->playSound(/*FMOD_CHANNEL_FREE*/FMOD_CHANNEL_REUSE, soundEffect[SoundEffect1], false, &channelSound[SoundEffect1]);
	ERRCHECK(result);

}

bool Sound::TogglePause(Songs songs)
{
	bool	isPaused;
	channel[songs]->getPaused(&isPaused);
	channel[songs]->setPaused(!isPaused);
	channel[songs]->setVolume(1.0f);
	return	!isPaused;
}

//for white noise sound
bool Sound::TogglePauseEffect()
{
	bool	isPaused;
	DSP[0]->getPaused(&isPaused);
	DSP[0]->setPaused(!isPaused);
	return	!isPaused;
}

bool Sound::TogglePause3D(Songs3D songs_3D)
{
	bool	isPaused;
	channel3D[songs_3D]->getPaused(&isPaused);
	channel3D[songs_3D]->setPaused(!isPaused);
	return	!isPaused;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VOLUME CONTROLS
//individual
void Sound::SetSongVolume(Songs songs, float volume)
{
	channel[songs]->setVolume(volume);
}

float Sound::GetSongVolume(Songs songs)
{
	float currentVol;
	channel[songs]->getVolume(&currentVol);
	return currentVol;
}

//Master, so for all sounds in this channel group
void Sound::SetMasterVolume(float vol)
{
	vol = max(min(vol, 1.0f), 0.0f);
	ChannelMusic->setVolume(vol);
}

//Master, so for all sounds in this channel group
float Sound::GetMasterVolume()
{
	float vol;
	ChannelMusic->getVolume(&vol);
	return vol;
}

////////////////////////////////////////////////////////////////////////////////////////////
//Individual
void Sound::SongVolumeUp(Songs songs)
{
	float volume = GetSongVolume(songs);
	volume += 0.1f;
	SetSongVolume(songs, volume);
}
void Sound::SongVolumeDown(Songs  songs)
{
	float volume = GetSongVolume(songs);
	volume -= 0.1f;
	SetSongVolume(songs, volume);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Pan music on speakers

void Sound::SetPan(Songs songs, float Pan)
{
	channel[songs]->setPan(Pan);
}

float Sound::GetPan(Songs songs)
{
	float Pan;
	channel[songs]->getPan(&Pan);
	return Pan;
}

void Sound::PanForwards(Songs songs)
{
	float pan = GetPan(songs);
    pan -= 0.1f;

	if (pan < -1)
    {
        pan = -1;
    }
	//for testing purposes
	//cout << endl << pan << "Back" << endl;

    SetPan(songs, pan);
}

void Sound::PanBackwards(Songs songs)
{
	float pan = GetPan(songs);
    pan += 0.1f;

	if (pan > 1)
    {
        pan = 1;
    }
	//for testing purposes
	//cout << endl << pan << "Forward" << endl;

	SetPan(songs, pan);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Set FMOD DSP Effects
//White Noise, sin, cosine, etc

void Sound::WhiteNoise(bool paused)
{
	//initialize whitenoise
	result = system->playDSP(FMOD_CHANNEL_REUSE, dsp[0], paused, &DSP[0]);
	ERRCHECK(result);

	DSP[0]->setVolume(0.25f);
	

	result = dsp[0]->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 5);
	ERRCHECK(result);

	DSP[0]->setPaused(paused);	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EFFECTS SETTINGS
void Sound::EchoToggle()
{
	bool Activate;

	result = DSP_SoundEffects[Echo]->getActive(&Activate);
	ERRCHECK(result);

	if(Activate)
	{
		result = DSP_SoundEffects[Echo]->remove();
		ERRCHECK(result);
	}
	else
	{
		result = system->addDSP(DSP_SoundEffects[Echo], 0);
		ERRCHECK(result);

		//FMOD_DSP_ECHO, FMOD_DSP_ECHO_DELAY, FMOD_DSP_ECHO_DECAYRATIO, FMOD_DSP_ECHO_DRYMIX/WETMIX, FMOD_DSP_ECHO_MAXCHANNELS		See fmod_dsp.hpp
		result = DSP_SoundEffects[Echo]->setParameter(FMOD_DSP_ECHO_DELAY, 500.0f);		/* Echo delay in ms.  10  to 5000.  Default = 500. */
		ERRCHECK(result);
		//result = DSP_SoundEffects[Echo]->setParameter(FMOD_DSP_ECHO_DECAYRATIO, 0.0f);		
																								/* Echo decay per delay.  
																								0 to 1.  1.0 = No decay, 
																								0.0 = total decay (ie simple 1 line delay).  
																								Default = 0.5. */
		ERRCHECK(result);
	}	
}

void Sound::FlangeToggle()
{
	bool Activate;

	result = DSP_SoundEffects[Flange]->getActive(&Activate);
	ERRCHECK(result);

	if(Activate)
	{
		result = DSP_SoundEffects[Flange]->remove();
		ERRCHECK(result);
	}
	else
	{
		result = system->addDSP(DSP_SoundEffects[Flange], 0);
		ERRCHECK(result);

		//FMOD_DSP_FLANGE, FMOD_DSP_FLANGE_DRYMIX/WETMIX, FMOD_DSP_FLANGE_DEPTH, FMOD_DSP_FLANGE_RATE		See fmod_dsp.hpp
		//Similar to how it was done with echo, if you want to mess with settings
	}
}

void Sound::DistortionToggle()
{
	bool Activate;

	result = DSP_SoundEffects[Distortion]->getActive(&Activate);
	ERRCHECK(result);

	if(Activate)
	{
		result = DSP_SoundEffects[Distortion]->remove();
		ERRCHECK(result);
	}
	else
	{
		result = system->addDSP(DSP_SoundEffects[Distortion], 0);
		ERRCHECK(result);
		
		//FMOD_DSP_DISTORTION_LEVEL													See fmod_dsp.hpp
		//Similar to how it was done with echo, if you want to mess with settings
	}
		//FMOD_DSP_DISTORTION_LEVEL													See fmod_dsp.hpp
		//Similar to how it was done with echo, if you want to mess with settings
}

void Sound::PitchToggle()
{
	bool Activate;

	result = DSP_SoundEffects[Pitch]->getActive(&Activate);
	ERRCHECK(result);

	if(Activate)
	{
		result = DSP_SoundEffects[Pitch]->remove();
		ERRCHECK(result);
	}
	else
	{
		result = system->addDSP(DSP_SoundEffects[Pitch], 0);
		ERRCHECK(result);

		//FMOD_DSP_PITCHSHIFT_PITCH, FMOD_DSP_PITCHSHIFT_FFTSIZE, FMOD_DSP_PITCHSHIFT_MAXCHANNELS			See fmod_dsp.hpp
		//Similar to how it was done with echo, if you want to mess with settings
		result = DSP_SoundEffects[Pitch]->setParameter(FMOD_DSP_PITCHSHIFT_PITCH, 0.5f);		/* Pitch value.  0.5 to 2.0.  
																							Default = 1.0. 0.5 = one octave down, 
																							2.0 = one octave up. 
																							1.0 does not change the pitch. */
		ERRCHECK(result);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FADE
void Sound::TransitionSongs(Songs songs1, Songs songs2)
{
	FadeOutSong(songs1);
	FadeInSong(songs2);
}

void Sound::FadeInSong(Songs songs)
{
	channel[songs]->setVolume(0.0f);

	channel[songs]->setPaused(false);

	FadeLength2 = 3000;	

	FadeStartTick2 = GetTickCount();

	channel[songs]->getVolume(&CurrentVolume2);

	FadeStartVol2	=		CurrentVolume2;

	FadeTargetVol2	=		1.0f;		//song2 target is to play, so target is 1.0

	FadeBoolIn		=		true;
	FadePauseAfter	=		false;

	//Update the transition data
	//FadeUpdate(songs, FadeLength2, FadeStartTick2, FadeStartVol2, FadeTargetVol2, FadePauseAfter);

	while(FadeBoolIn == true)
	{
		float Progress = min(static_cast<float>(GetTickCount() - FadeStartTick2) / FadeLength2, 1.0f);

		if(Progress == 1.0f)
		{
			FadeBoolIn = false;
		}

		// Translate linear progress into a smooth sine-squared fade effect
		float PI	 =		3.14159265358979323846f;
		float Volume =		static_cast<float>(sin( (Progress * PI) / 2));

		Volume		*=		Volume;
		// Scale volume between start and target volumes
		Volume		 =		Volume * (FadeTargetVol2 - FadeStartVol2) + FadeStartVol2;

		// Alter song volume
		channel[songs]->setVolume(Volume);

		// Post-fade processing
		if (!FadeBoolIn)
		{
			channel[songs]->setPaused(FadePauseAfter);
		}		
	}
}
void Sound::FadeOutSong(Songs songs)
{
	channel[songs]->setVolume(1.0f);

	channel[songs]->setPaused(false);

	FadeLength1 = 3000;		//miliseconds to fade

	FadeStartTick1 = GetTickCount();

	channel[songs]->getVolume(&CurrentVolume1);

	FadeStartVol1	=		CurrentVolume1;

	FadeTargetVol1	=		0.0f;

	FadeBoolOut		=		true;
	FadePauseAfter	=		true;

	//Update the transition data
	//FadeUpdate(songs, FadeLength1, FadeStartTick1, FadeStartVol1, FadeTargetVol1, FadePauseAfter);

	while(FadeBoolOut == true)
	{
		float Progress = min(static_cast<float>(GetTickCount() - FadeStartTick1) / FadeLength1, 1.0f);

		if(Progress == 1.0f)
		{
			FadeBoolOut = false;
		}

		// Translate linear progress into a smooth sine-squared fade effect
		float PI	 =		3.14159265358979323846f;
		float Volume =		static_cast<float>(sin( (Progress * PI) / 2));

		Volume		*=		Volume;
		// Scale volume between start and target volumes
		Volume		 =		Volume * (FadeTargetVol1 - FadeStartVol1) + FadeStartVol1;

		// Alter song volume
		channel[songs]->setVolume(Volume);

		// Post-fade processing
		if (!FadeBoolOut)
		{
			channel[songs]->setPaused(FadePauseAfter);
		}		
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Reverb Preferences

void Sound::InitReverb()
{
	for(int i = 0; i < Max_Reverbs; i++)
	{
		result = system->createReverb(&reverb[i]);
		ERRCHECK(result);
	}

	//FMOD_REVERB_PROPERTIES	prop = FMOD_PRESET_OFF;
	FMOD_REVERB_PROPERTIES	prop = FMOD_PRESET_CAVE;
	system->setReverbAmbientProperties(&prop);

	FMOD_REVERB_PROPERTIES	prop1 = FMOD_PRESET_CONCERTHALL;
	reverb[ConcertHall]->setProperties(&prop1);
	FMOD_REVERB_PROPERTIES	prop2 = FMOD_PRESET_ALLEY;
	reverb[Alley]->setProperties(&prop2);
	FMOD_REVERB_PROPERTIES	prop3 = FMOD_PRESET_UNDERWATER;
	reverb[UnderWater]->setProperties(&prop3);
	FMOD_REVERB_PROPERTIES	prop4 = FMOD_PRESET_HANGAR;
	reverb[Hangar]->setProperties(&prop4);
	FMOD_REVERB_PROPERTIES	prop5 = FMOD_PRESET_FOREST;
	reverb[Forest]->setProperties(&prop5);
	FMOD_REVERB_PROPERTIES	prop6 = FMOD_PRESET_BATHROOM;
	reverb[BathRoom]->setProperties(&prop6);

	FMOD_VECTOR Posit1 = {0.0f, 0.0f, 0.0f};
	float min_dist1 = 5.0f;
	float max_dist1 = 10.0f;
	reverb[ConcertHall]->set3DAttributes(&Posit1, min_dist1, max_dist1);

	FMOD_VECTOR Posit2 = {-10.0f, 0.0f, 0.0f};
	float min_dist2 = 5.0f;
	float max_dist2 = 10.0f;
	reverb[Alley]->set3DAttributes(&Posit2, min_dist2, max_dist2);

	FMOD_VECTOR Posit3 = {-25.0f, 0.0f, 0.0f};
	float min_dist3 = 5.0f;
	float max_dist3 = 10.0f;
	reverb[UnderWater]->set3DAttributes(&Posit3, min_dist3, max_dist3);

	FMOD_VECTOR Posit4 = {15.0f, 0.0f, 0.0f};
	float min_dist4 = 5.0f;
	float max_dist4 = 10.0f;
	reverb[Hangar]->set3DAttributes(&Posit4, min_dist4, max_dist4);

	FMOD_VECTOR Posit5 = {20.0f, 0.0f, 0.0f};
	float min_dist5 = 5.0f;
	float max_dist5 = 10.0f;
	reverb[Forest]->set3DAttributes(&Posit5, min_dist5, max_dist5);

	FMOD_VECTOR Posit6 = {-35.0f, 0.0f, 0.0f};
	float min_dist6 = 5.0f;
	float max_dist6 = 10.0f;
	reverb[BathRoom]->set3DAttributes(&Posit6, min_dist6, max_dist6);

}