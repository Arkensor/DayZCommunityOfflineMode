class AbstractSoundScene
{
	proto native AbstractWave Play2D(SoundObject soundObject);
	proto native AbstractWave Play3D(SoundObject soundObject);
	proto native SoundObject BuildSoundObject(SoundObjectBuilder soundObjectbuilder);

	proto native float GetRadioVolume();
	proto native void SetRadioVolume(float vol, float time);

	proto native float GetSpeechExVolume();
	proto native void SetSpeechExVolume(float vol, float time);

	proto native float GetMusicVolume();
	proto native void SetMusicVolume(float vol, float time);

	proto native float GetSoundVolume();
	proto native void SetSoundVolume(float vol, float time);

	proto native float GetVOIPVolume();
	proto native void SetVOIPVolume(float vol, float time);
}


class SoundObjectBuilder
{
	void SoundObjectBuilder(SoundParams soundParams);	
	
	SoundObject BuildSoundObject()
	{
		return GetGame().GetSoundScene().BuildSoundObject(this);
	}

	proto native void Initialize(SoundParams soundParams);
	proto native void UpdateEnvSoundControllers(vector position);
	proto native void SetVariable(string name, float value);
}


class SoundObject
{
	void SoundObject(SoundParams soundParams);

	AbstractWave Play2D()
	{
		return GetGame().GetSoundScene().Play2D(this);
	}
	
	AbstractWave Play3D()
	{
		return GetGame().GetSoundScene().Play3D(this);
	}
	
	proto native void SetPosition(vector position);
	proto native void Initialize(SoundParams soundParams);
}


//soundsys.hpp
class SoundParams
{
	void SoundParams(string name);
		
	proto native bool Load(string name);
	proto native bool IsValid();
}

class AbstractWave
{
	proto native void Play();
				 void PlayWithOffset(float offset) { Play(); SetStartOffset(offset); }
	//proto native void Mute();
	proto native void Stop();
	proto native void SetStartOffset(float offset);
	proto native float GetLength();
	proto native void Repeat(int count);
	proto native float GetCurrPosition();
	
				void OnPlay()
				{
					//Print("[lukasikjak] AbstractWave::OnPlay");
				}
				
				void OnStop()
				{
					//Print("[lukasikjak] AbstractWave::OnStop");
				}
				
				void OnLoad()
				{
					//Print("[lukasikjak] AbstractWave::OnLoad");
				}
				
				void OnEnd()
				{
					//Print("[lukasikjak] AbstractWave::OnEnd");
				}
}