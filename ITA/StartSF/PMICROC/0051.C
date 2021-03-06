// Note: Delete xml recipes

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define 	ERR_RW_XMLFILE			-1

#define	REC_TITLE				1300
#define 	XML_ERROR				1322

main()
{
	int handle, error ;
	unsigned long rootnode ;
	text fileNameXML = "XML\\recipes.xml";
	text recipeName<128>, endTagXML<256> ;

	recipeName  =  tt_remove_trailing_spaces(vardb[REC_TITLE]) ;
	error = -1 ;
	vardb[XML_ERROR]  = error ; 

	if (handle = xml_load_file(fileNameXML)) {
		if (rootnode = xml_get_root_node(handle)) {
			if (rootnode = xml_find_node_by_xpath(handle, rootnode, "//recipe[@title='"+ recipeName +"']/ingredients")) { //the new root is the node with the recipe title selected
				endTagXML  =  "\t<recipe title=\"" + recipeName +"\">" ;
				error = deleteRecipe(fileNameXML, recipeName, endTagXML) ;
				}
			else 
				error = xml_status() ;
			}
		else error = xml_status() ;
		xml_terminate_file(handle) ;
		}
	else error = xml_status() ;

	vardb[XML_ERROR]  = error ; 
}
int deleteRecipe(text fileName, text recipeName, text endTagXML1) 
{
	int handleFR = 0, handleFW = 0, error = FALSE, endFile = FALSE ;
	int startDel, stopDel ;
	text endTagXML2 = "\t</recipe>" ;
	text fileNameTmp = "XML\\tmp.xml" ;
	text rowText<500> = "" ;
	
	startDel = stopDel = FALSE ;
	if((handleFR = fm_open_file(fileName, FM_ASCII, FM_READING, FM_FREE_ACCESS) ) && (handleFW = fm_open_file(fileNameTmp, FM_ASCII, FM_WRITING, FM_FREE_ACCESS))) {
		while(!endFile && !error) {
			if (!(error = readRowXML(handleFR, rowText))) {
				if(tt_indexed_compare(rowText, 0, endTagXML1, 0, tt_len(endTagXML1)) == TT_EQUAL)
					startDel = TRUE ;
				if(tt_indexed_compare(rowText, 0, endTagXML2, 0, tt_len(endTagXML2)) == TT_EQUAL) 
					stopDel = TRUE ;
				if (!startDel) {
					rowText += "\n" ;
					if (!fm_write_text(handleFW, rowText))
						error = ERR_RW_XMLFILE ;
					}
				else 	if (stopDel)
					startDel = stopDel = FALSE ;
				stopDel = FALSE ;
				}
			}
		fm_close_file(handleFR) ;
		fm_close_file(handleFW) ;
	
		error = adjustFilenames(fileName,fileNameTmp);
		}
	else error = XML_PARSE_FILE_NOT_FOUND;

	return (error) ;
}

