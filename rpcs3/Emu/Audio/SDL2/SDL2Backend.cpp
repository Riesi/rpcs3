#include "Emu/System.h"
#include "SDL2Backend.h"

#ifdef HAVE_SDL2

#include <SDL2/SDL.h>

SDL2Backend::SDL2Backend()
{
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
		fprintf(stderr, "SDL2: Failed to initialize: %s\n", SDL_GetError());
}
SDL2Backend::~SDL2Backend()
{
	this->Close();
}

void SDL2Backend::Close()
{
	if(this->connection) {
		SDL_CloseAudioDevice(this->connection);
		SDL_Quit();
		this->connection = 0;
	}
}

void SDL2Backend::Open(u32 /* num_buffers */)
{
	SDL_AudioSpec audioSpec= {0};
	audioSpec.format = (get_sample_size() == 2) ? AUDIO_S16LSB : AUDIO_F32LSB;
	audioSpec.freq =  get_sampling_rate();
	audioSpec.samples = 4096;
	// default channel mapping of SDL2 is the same as the one used in PulseBackend
	audioSpec.channels = get_channels();
	
	this->connection = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
	if(!this->connection) {
		fprintf(stderr, "SDL2: Failed to initialize audio: %s\n", SDL_GetError());
	}
}

bool SDL2Backend::AddData(const void* src, u32 num_samples)
{
	AUDIT(this->connection);

	if(SDL_QueueAudio(this->connection, src, num_samples * get_sample_size()) < 0) {
		fprintf(stderr, "SDL2: Failed to queue audio data: %s\n", SDL_GetError());
		return false;
	}
	
	return true;
}

void SDL2Backend::Play(){
	SDL_PauseAudioDevice(this->connection, 0);
}
void SDL2Backend::Pause(){
	SDL_PauseAudioDevice(this->connection, 1);
}
void SDL2Backend::Flush(){
	SDL_ClearQueuedAudio(this->connection);
}

bool SDL2Backend::IsPlaying(){
	switch (SDL_GetAudioDeviceStatus(this->connection)) {
		case SDL_AUDIO_PLAYING:
			return true;
		default: break;
	}
	return false;
}
	
u64 SDL2Backend::GetNumEnqueuedSamples(){
	return SDL_GetQueuedAudioSize(this->connection) / get_sample_size();
}

#endif
