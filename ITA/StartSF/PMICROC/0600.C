//Alarm Page

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define 	CODEPAGE				437

#define	IS_SPIKING				1709
#define	RADIOBTN_SET_ALM_INDEX	1710
#define 	SPEAK_ALM_STATE			1712

#define 	ALARM_DESCRIPT			1713
#define	ALARM_DATEHOUR			1714
#define	ALARM_GROUP				1715

#define	SPEAK_HELPFILE_STATE		1718
#define	ALARM_HELPFILE			1719
#define 	CMD_SPEAK_HELP			1721
#define	IS_SPIKING_HELP			1722
#define	IS_IN_PAUSE				1723


int getHelpFile(int  codeAlrm) ;

main(int ident, int channel, int write, long job, int numval, int arrval[])
{
	int err = FALSE, nall ;
	text filename<256>;

	if (write) 
	{
		if (job == 0) // init
		{
			vardb[SPEAK_ALM_STATE] = 1 ;
		
			vardb[IS_SPIKING] = FALSE;
			vardb[RADIOBTN_SET_ALM_INDEX] = - 1 ;
			uvardb[ALARM_DESCRIPT] = uvardb[ALARM_GROUP] = uvardb[ALARM_DATEHOUR] = U"";
			vardb[ALARM_HELPFILE] = "" ;
			vardb[SPEAK_HELPFILE_STATE] = 0;
		
			vardb[IS_SPIKING_HELP] = FALSE;
			vardb[CMD_SPEAK_HELP] = 2 ;
			vardb[IS_IN_PAUSE] = FALSE;
		}
		else if (job == 1) //command active alarm and speak
		{
			if (vardb[SPEAK_ALM_STATE] && (vardb[IS_SPIKING] || vardb[IS_SPIKING_HELP])) {
				stopSpeakAlarm();
				vardb[IS_SPIKING] = FALSE ;
				vardb[IS_SPIKING_HELP]  = FALSE;
				vardb[IS_IN_PAUSE] = FALSE;
				vardb[CMD_SPEAK_HELP] = 2 ;
				stopSpeakAlarm();
				}
			if (vardb[RADIOBTN_SET_ALM_INDEX] == 0) {
				vardb[1705] = 1 ;
				nall = 33 ;
				read_information_alarm(nall) ;
				}
			else if (vardb[RADIOBTN_SET_ALM_INDEX] == 1) {
				vardb[1705] = 2 ;
				nall = 34 ;
				read_information_alarm(nall) ;
				}
			else if (vardb[RADIOBTN_SET_ALM_INDEX] == 2) {
				vardb[1705] = 4 ;
				nall = 35 ;
				read_information_alarm(nall) ;
				}
		}
		else if (job == 2) //command speak/mute
		{
			if(vardb[SPEAK_ALM_STATE]) //stop speaking
			{
				stopSpeakAlarm();
				vardb[IS_SPIKING] = FALSE;
				vardb[IS_SPIKING_HELP] = FALSE;
				vardb[IS_IN_PAUSE] = FALSE;
				vardb[CMD_SPEAK_HELP] = 2 ;
				vardb[SPEAK_ALM_STATE] = 0 ;
			}
			else
				vardb[SPEAK_ALM_STATE] = 1 ;
		}
		else if (job == 3) //command alarm help
		{
			if(vardb[SPEAK_ALM_STATE])
			{
				if(vardb[CMD_SPEAK_HELP] == 0) //play speak help file
				{
					if(vardb[IS_SPIKING])
					{
						stopSpeakAlarm();
						vardb[IS_SPIKING] = FALSE;
						stopSpeakAlarm();
					}
					if(vardb[IS_IN_PAUSE])
					{
						playBalcon(PAUSE, "");
						vardb[IS_IN_PAUSE] = FALSE;
					}
					else
					{	//start to read help alarm file
						filename = vardb[ALARM_HELPFILE];
						tt_remove_trailing_spaces(filename);
						playBalcon(PLAY_FILE,"\\" + filename);
						vardb[IS_SPIKING_HELP]  = TRUE;
					}
				}
				else if(vardb[CMD_SPEAK_HELP] == 1) //pause speak help file
				{
					if(vardb[IS_SPIKING_HELP])
					{
						playBalcon(PAUSE, "");
						vardb[IS_IN_PAUSE] = TRUE;
					}
				}
				else 	if(vardb[CMD_SPEAK_HELP] == 2) //stop speak
				{
					if(vardb[IS_SPIKING_HELP])
					{
						stopSpeakAlarm() ;
						vardb[IS_SPIKING_HELP]  = FALSE;
						vardb[IS_IN_PAUSE] = FALSE;
					}
				}
			}
		}
		else 
			err = TRUE ;
	}
	else {
		if (job == 2) //state speak/mute alarm
			arrval[0] = vardb[SPEAK_ALM_STATE] ;
		else 
			err = TRUE ;
		}

	return (err);
}
read_information_alarm(int nall)
{
	int presentFile ;
	unsigned long int dateHourSF ;
	utext title<100>, strGroupName, strDateEng, strHour, strDateHour ;

	am_alarm_utitle(nall, title) ;
	am_alarm_ugroup(nall, strGroupName) ;
	dateHourSF = tm_local_time() ;
	//set in english date
	strDateEng = UDataEnG(strDateEng,dateHourSF) ;
	tm_to_utext_hour(strHour,dateHourSF) ;
	strDateHour = strDateEng + U" " + strHour ;

	uvardb[ALARM_DESCRIPT] = title ;
	uvardb[ALARM_GROUP] = strGroupName ;
	uvardb[ALARM_DATEHOUR] = strDateHour ;

	presentFile = getHelpFile(nall);
	vardb[SPEAK_HELPFILE_STATE] = presentFile;
		
	if(vardb[SPEAK_ALM_STATE])
		playAlarm(title,strGroupName,strDateHour);
}
int getHelpFile(int  codeAlrm)
{
	int fileHelpPresent = TRUE ;
	text filename<256> ;

	am_alarm_detail(codeAlrm,filename,"","") ;

	if(tt_len(filename) == 0)
		fileHelpPresent = FALSE ;
	vardb[ALARM_HELPFILE] = filename ;

	return (fileHelpPresent) ;		
}
playAlarm(utext utitle, utext ugroup, utext udate)
{
	text title<100>, strGroupName, strDateEng;
	text strAlarmInfo <256>;

	dc_utext_to_text(title,utitle,CODEPAGE);
	dc_utext_to_text(strGroupName, ugroup,CODEPAGE);
	dc_utext_to_text(strDateEng,udate,CODEPAGE);
	strAlarmInfo = "Alarm " + title + " of the group " + strGroupName + " in date " + strDateEng;

	playBalcon(PLAY_TEXT, strAlarmInfo);
	vardb[IS_SPIKING] = TRUE;
}
stopSpeakAlarm()
{
	playBalcon(STOP_ACTIVE,"");
}
