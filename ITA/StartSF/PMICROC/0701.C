// Note: XML-SVG manipulation - Unicode

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define SVG_FILEPATH					1050
#define XML_NODE_VALUE					1052
#define XML_ATTRIBUTE_VALUE				1053
#define XML_ERROR						1055

#define ALARM_STATE   					1051
#define ALARM_OFF						0
#define ALARM_WARNING					1	
#define ALARM_ON						2

extern text nameSVG[2] ;
extern int toggle ;

main()
{
	int handle, next ;
	int stateAlarm ;
	int okatt = FALSE, okval = FALSE ;
	unsigned long nodeadd, rootnode ;

	text fAttName  	=  "id" ;
	text sAttName  =  "style" ;
	text sAttValColor<100>, sAttValTitle<100>;
	text errorXML<100> = "" ;

	stateAlarm = vardb[ALARM_STATE] ;

	if (toggle == 0)
		next = 1 ;
	else 
		next = 0 ;
	
	nameSVG[0] = "SVG\\alarm.svg" ;
	nameSVG[1] = "SVG\\alarm1.svg" ;
	

	if ( stateAlarm == ALARM_OFF) 	{
		sAttValColor = "fill:SpringGreen;" ; 
		sAttValTitle = "ALARM OFF" ;
		}
	else  if (stateAlarm == ALARM_WARNING)	{
		sAttValColor = "fill:yellow;" ;
		sAttValTitle = "WARNING!" ;
		}
	else 	{
		sAttValColor = "fill:Crimson;" ; 
		sAttValTitle = "ALARM ON" ;
		}

	handle = xml_load_file(nameSVG[toggle]) ;
	if (handle) {
		rootnode = xml_get_root_node(handle) ;
	
		if (nodeadd = xml_find_node_by_attribute(handle, rootnode, fAttName,  "alarmColor")) {
			okatt = xml_set_attribute(handle, nodeadd, sAttName, sAttValColor) ;
			vardb[XML_ATTRIBUTE_VALUE]  =  sAttValColor ;
			}
		
		if (nodeadd = xml_find_node_by_attribute(handle, rootnode, fAttName, "title")) {
			okval = xml_set_value(handle, nodeadd, sAttValTitle) ;
			vardb[XML_NODE_VALUE] = sAttValTitle ;
			}
		
		if (okatt && okval) {
			xml_save_file(handle, nameSVG[next]) ;
			vardb[SVG_FILEPATH] = nameSVG[next] ;
			saveSVGFforHTMLDisplay(nameSVG[next]);
			}
		else {
			errorXML +=  "impossible to change " ;
			if (!okatt) {
				errorXML +=  "attribute node" ;
				if(!okval)
					errorXML +=  " and " ;
				}
			if (!okval)
				errorXML +=  "node value" ;
			vardb[SVG_FILEPATH] = "ICONS\\Layout\\error.svg" ;
			}
	
		xml_terminate_file(handle) ;
		}
	else
		errorXML +=  "impossible to load file XML " ;
	
	vardb[XML_ERROR]  = errorXML ; 
	
	toggle = next ;	
}

saveSVGFforHTMLDisplay(text fileNameSVG)
{
	text svgNameHtml = "SVG\\HTML\\alarmHTML.svg" ;
	dm_copy_files(fileNameSVG, svgNameHtml) ;
}
