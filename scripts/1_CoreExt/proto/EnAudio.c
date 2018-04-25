typedef int[] AudioHandle;

class AudioSystem
{
	proto native static AudioHandle BankLoad(string path);
	proto native static AudioHandle ShaderLoad(string path);
	proto native static AudioHandle SoundPlay(AudioHandle bank, AudioHandle shader);
	proto native static void SoundDestroy(AudioHandle handle);
};
