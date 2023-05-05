#include "Audio.h"
#include "../HexEngine.h"

std::unordered_map<std::string, std::pair<Mix_Music*, char*>> Audio::bgm;
std::unordered_map<std::string, Mix_Chunk*> Audio::sfx;
Audio* Audio::current = nullptr;

void Audio::refl() {
    REG_COMP(Audio);
    REG_STATIC_METHOD(main);
    REG_FIELD(playOnStart);
    REG_FIELD(loop);
    REG_FIELD(music);
    REG_METHOD(SetMusic);
    REG_METHOD(Play);
    REG_METHOD(Pause);
    REG_METHOD(Resume);
    REG_METHOD(Stop);
    REG_STATIC_METHOD(ClearBuffers);
    REG_STATIC_METHOD(PauseAll);
    REG_STATIC_METHOD(ResumeAll);
    REG_STATIC_METHOD(StopAll);
    REG_STATIC_METHOD(PlayClip);
    REG_STATIC_METHOD(Volume);
    REG_STATIC_METHOD(MusicVolume);
}

Mix_Music* Audio::GetMusic(const std::string& music) {
    if (bgm.contains(music)) {
        return bgm[music].first;
    }

    auto&& data = HexEngine::FileSystem::Read(music);
    data.seekg(0, std::ios::end);
    int fileSize = data.tellg();
    data.seekg(0, std::ios::beg);
    char* cont = new char[fileSize];
    data.read(cont, fileSize);
    SDL_RWops* io = SDL_RWFromConstMem(cont, fileSize);

    Mix_Music* newMusic = Mix_LoadMUS_RW(io, 1);

    bgm[music] = {newMusic, cont};
    return newMusic;
}

Mix_Chunk* Audio::GetChunk(const std::string& chunk) {
    if (sfx.contains(chunk)) {
        return sfx[chunk];
    }

    auto&& data = HexEngine::FileSystem::Read(chunk);
    data.seekg(0, std::ios::end);
    int fileSize = data.tellg();
    data.seekg(0, std::ios::beg);
    char* cont = new char[fileSize];
    data.read(cont, fileSize);
    SDL_RWops* io = SDL_RWFromConstMem(cont, fileSize);

    Mix_Chunk* newChunk = Mix_LoadWAV_RW(io, 1);

    delete[] cont;

    sfx[chunk] = newChunk;
    return newChunk;
}

Audio& Audio::main() {
    return *current;
}

void Audio::Awake() {
    if (current == nullptr) {
        current = this;
    }
    HexEngine::HexEngine::Instance().DontDestroyOnLoad(*gameObject);
}

void Audio::Start() {
    if (playOnStart) {
        Play();
    }
}

void Audio::OnDestroy() {
    ClearBuffers();
}

void Audio::ClearBuffers() {
    StopAll();
    for (auto& music : bgm) {
        Mix_FreeMusic(music.second.first);
        delete[] music.second.second;
    }
    for (auto& chunk : sfx) {
        Mix_FreeChunk(chunk.second);
    }
    bgm.clear();
    sfx.clear();
}

void Audio::SetMusic(const std::string& newMusic) {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    music = newMusic;
}

void Audio::Play() {
    Mix_PlayMusic(GetMusic(music), loop ? -1 : 0);
}

void Audio::Pause() {
    Mix_PauseMusic();
}

void Audio::Resume() {
    Mix_ResumeMusic();
}

void Audio::Stop() {
    Mix_HaltMusic();
}

void Audio::PlayClip(const std::string& clip, int volume) {
    auto* chunk = GetChunk(clip);
    Mix_VolumeChunk(chunk, volume);
    Mix_PlayChannel(-1, chunk, 0);
}

void Audio::PauseAll() {
    Mix_Pause(-1);
}

void Audio::ResumeAll() {
    Mix_Resume(-1);
}

void Audio::StopAll() {
    Mix_HaltChannel(-1);
}

int Audio::Volume(int volume) {
    return Mix_MasterVolume(volume);
}

int Audio::MusicVolume(int volume) {
    return Mix_VolumeMusic(volume);
}
