#define 	TRUE						1
#define 	FALSE					0

#define	PLAY_TEXT					0
#define	PLAY_FILE					1
#define	PAUSE					2
#define	STOP_ACTIVE				3
#define	STOP_OTHER				4

extern debug ;

text DataEnG(text strDate, unsigned long  int dt);
utext UDataEnG(utext strDate, unsigned long  int dt);
int playBalcon(int command, text strText) ;

text DataEnG(text strDate, unsigned long  int dt)
{
	text tmp ;

	strDate =  dc_int_to_text(tmp, tm_month(dt), 2, 0, 0, 1, 0, 0) + "-" +   dc_int_to_text(tmp, tm_day(dt), 2, 0, 0, 1, 0, 0) + "-" +  dc_int_to_text(tmp, tm_year(dt), 4, 0, 0, 1, 0, 0) ;

	return (strDate) ;
}

utext UDataEnG(utext strDate, unsigned long  int dt)
{
	utext tmp ;

	strDate =  dc_int_to_utext(tmp, tm_month(dt), 2, 0, 0, 1, 0, 0) + U"-" +   dc_int_to_utext(tmp, tm_day(dt), 2, 0, 0, 1, 0, 0) + U"-" +  dc_int_to_utext(tmp, tm_year(dt), 4, 0, 0, 1, 0, 0) ;

	return (strDate) ;
}

wait_please( unsigned long tick)
{
	unsigned long int start = start_tick() ;
	while (read_tick(start) < tick);
}


playBalcon(int command, text strText)
{
	text curdir<400> ;

	dm_current_directory(curdir) ;
	tt_remove_trailing_spaces(curdir);
	if(command == PLAY_TEXT)
		system_command("",curdir + "\\balcon\\balcon.exe"," -tray -id 1033 -t \" " + strText + "\" -s -3 -p 7 ","balcon",SYSTEM_COMMAND_HIDE);
	else if(command == PLAY_FILE)
		system_command("",curdir + "\\balcon\\balcon.exe","-tray -id 1033 -f \"" + curdir + strText + "\"  -s -3 -p 7 ","balcon",SYSTEM_COMMAND_HIDE);
	else if(command == PAUSE)
		system_command("",curdir + "\\balcon\\balcon.exe","-pr","balcon",SYSTEM_COMMAND_HIDE);
	else if(command == STOP_ACTIVE)
		system_command("",curdir + "\\balcon\\balcon.exe"," -ka","balcon",SYSTEM_COMMAND_HIDE);
	else
		system_command("",curdir + "\\balcon\\balcon.exe"," -k","balcon",SYSTEM_COMMAND_HIDE);
}


displayOnIE(text pathFileName, text fileName)
{
	text path<256> ;
	text fullPath<512>;
	
	dm_current_directory(path);
	fullPath = path + "\\" + pathFileName ;
	system_command("", fileName, "", fullPath, SYSTEM_COMMAND_SHOW);
}


int readRowXML(int handle, text rowText)
{
	int endRow = FALSE, errorRead = FALSE;
	text tmp<2>;

	rowText = "" ;
	tmp = "" ;

	while(!endRow) {
		fm_read_characters(handle, tmp, 1) ;
		if (fm_ok_reading(handle)) {
			tt_set_char(tmp,1,0);
			if(tt_char(tmp, 0) == 0xA) 
				endRow = TRUE;
			else if(tt_char(tmp, 0) == 0xD)
				;
			else rowText += tmp;
			}
		else {	
			endRow = TRUE ;
			errorRead = XML_PARSE_FILE_READ_ERROR ;
			}
		}

	return (errorRead) ;
}

int adjustFilenames(text fileName, text fileNameTmp)
{
	int error = XML_MGM_ERR_SAVE_FILE;
	
	if(dm_delete_files(fileName))
	{
		if(dm_rename_files(fileNameTmp,fileName))
			error = FALSE;
	}

	return error;
}
