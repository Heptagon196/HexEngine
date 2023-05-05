#pragma once
#include "SDL2/SDL_mixer.h"
#include "BaseComponent.h"

class Audio : BaseComponent {
    private:
        static std::unordered_map<std::string, std::pair<Mix_Music*, char*>> bgm;
        static std::unordered_map<std::string, Mix_Chunk*> sfx;
        static Mix_Music* GetMusic(const std::string& music);
        static Mix_Chunk* GetChunk(const std::string& chunk);
        static Audio* current;
    public:
        static void refl();
        static Audio& main();
        void Awake();
        void Start();
        void OnDestroy();
        bool playOnStart = false;
        bool loop = false;
        std::string music;
        void SetMusic(const std::string& music);
        void Play();
        void Pause();
        void Resume();
        void Stop();
        static void ClearBuffers();
        static void PlayClip(const std::string& clip, int volume);
        static void PauseAll();
        static void ResumeAll();
        static void StopAll();
        static int Volume(int volume);
        static int MusicVolume(int volume);
};
