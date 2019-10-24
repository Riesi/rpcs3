#pragma once

#ifdef HAVE_SDL2

#include "Emu/Audio/AudioBackend.h"
#include <SDL2/SDL.h>

class SDL2Backend : public AudioBackend
{
public:
	SDL2Backend();
	virtual ~SDL2Backend() override;

	virtual const char* GetName() const override { return "SDL2"; }

	static const u32 capabilities = PLAY_PAUSE_FLUSH | IS_PLAYING | GET_NUM_ENQUEUED_SAMPLES;
	virtual u32 GetCapabilities() const override { return capabilities; }

	virtual void Open(u32) override;
	virtual void Close() override;

	virtual bool AddData(const void* src, u32 num_samples) override;

	virtual void Play()  override;
	virtual void Pause() override;
	virtual void Flush() override;

	virtual bool IsPlaying() override;
	
	virtual u64 GetNumEnqueuedSamples() override;

private:
	SDL_AudioDeviceID connection = 0;
};

#endif
