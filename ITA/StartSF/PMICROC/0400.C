// Media Player: play song and video

#include <microc.h>
#include <sf.h>

#define SONG_NAME		1600

main()
{
	text path<256>, songName, song<512>;

	dm_current_directory(path);
	songName = tt_remove_spaces(vardb[SONG_NAME]);
	song = path + songName;
	system_command("","wmplayer","\""+ song +"\"","",SYSTEM_COMMAND_SHOWMAXIMIZED);
}
