
#ifndef _SOUND_APP_H_
#define _SOUND_APP_H_

#include "../Common/tutorial.h"

class SoundApp : public HoeTutorial
{
public:
	SoundApp(HOE_INSTANCE instance, HoeGame::Console * con);
	bool LoadScene();
};

#endif // _SOUND_APP_H_

