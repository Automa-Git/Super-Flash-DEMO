// Note: Export xml recipes

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define 	ERR_RW_XMLFILE			-1

#define	REC_TITLE				1300
#define	MAX_INGREDIENT			10
#define	INGREDIENTX_QTY			1301		//first
#define	INGREDIENTX_KCAL			1311		//first

#define 	XML_ERROR				1322

text fileNameXML<128> ;
static text ingredient<50>[10] ;
static text attNameQnty<50>, attNameKcal<50>  ;

/* BINDING:
	- The recipes xml file must always exist and  must have the following basic formatting:
	
	<?xml version="1.0"?>
	<recipes xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="recipes.xsd">
	</recipes>
	
	otherwise the library gives an error.
	
	- Only existing ingredients are modified, if they don't exist they aren't added
*/

main()
{
	int handle, error ;
	unsigned long rootnode ;
	text recipeName<128> ;

	initXMLstring() ;
	recipeName  =  tt_remove_trailing_spaces(vardb[REC_TITLE]) ;
	error = -1 ;
	vardb[XML_ERROR]  = error ; 

	if (handle = xml_load_file(fileNameXML)) {
		if (rootnode = xml_get_root_node(handle)) {
			if (rootnode = xml_find_node_by_xpath(handle, rootnode, "//recipe[@title='"+ recipeName +"']/ingredients")) //the new root is the node with the recipe title selected
				error = modifyRecipe(fileNameXML, recipeName, handle, rootnode) ; //Change the recipe values in the xml file
			else {
				error = xml_status() ;
				if (error == XML_MGM_ERR_NODE_FIND) 
					error =  addNewRecipe(fileNameXML, recipeName) ; //Add new recipe in the xml file
				}
			}
		else error = xml_status() ;
		xml_terminate_file(handle) ;
		}
	else error = xml_status() ;

	vardb[XML_ERROR]  = error ; 
}
int modifyRecipe(text fileName, text recipeName, int handle, unsigned long rootnode) 
{
	int mod, err, i, count, posIngredient ;
	unsigned long selecthandle, nodeadd ;
	text ingredientValue<50> = "", qtyValue, kcalValue;

	mod = FALSE ;
	err = FALSE ;

	if (selecthandle = xml_select_nodes_by_xpath(handle, rootnode, "ingredient")) {
		if (count = xml_get_select_count(handle, selecthandle)) {
			i = 0 ;
			while (i < count && !err) {
				if (nodeadd = xml_get_node_by_select(handle, selecthandle, i)) {
					if (xml_get_value(handle, nodeadd, ingredientValue)) {
						if ((posIngredient = getIndexBaseDesc(ingredientValue)) != -1) { 	//Search if it is a managed ingredient
							//Set Ingredients Values
							dc_int_to_text(qtyValue,vardb[INGREDIENTX_QTY + posIngredient],4,0,0,0,0,0);
							dc_int_to_text(kcalValue,vardb[INGREDIENTX_KCAL + posIngredient],4,1,0,0,0,0);
							if (xml_set_attribute(handle, nodeadd, attNameQnty,qtyValue)) {
								if (xml_set_attribute(handle, nodeadd, attNameKcal, kcalValue))
									mod = TRUE ;
								else 	err = xml_status() ;
								}
							else 	err = xml_status() ;
							}
						}
					else 	err = xml_status() ;
					}
				else 	err = xml_status() ;
				i++ ;
				}
			if (mod)
				xml_save_file(handle, fileName) ;
			}
		else 	err = xml_status() ;
		}
	else 	err = xml_status() ;
	
	return (err) ;
}
int addNewRecipe(text fileName, text recipeName) 
{
	int handleFR = 0, handleFW = 0, error = FALSE, endFile = FALSE ;
	text endTagXML  =  "</recipes>" ;
	text fileNameTmp = "XML\\tmp.xml" ;
	text rowText<500> = "" ;
	
	if((handleFR = fm_open_file(fileName, FM_ASCII, FM_READING, FM_FREE_ACCESS) ) && (handleFW = fm_open_file(fileNameTmp, FM_ASCII, FM_WRITING, FM_FREE_ACCESS))) {
		while(!endFile && !error) {
			if (!(error = readRowXML(handleFR, rowText))) {
				if(tt_indexed_compare(rowText, 0, endTagXML, 0, 10) == TT_EQUAL) {
					if (!writeNewRecipeInXML(handleFW, recipeName))
						error = ERR_RW_XMLFILE ;
					endFile = TRUE ;
					}
				else 	{
					rowText += "\n" ;
					if (!fm_write_text(handleFW, rowText))
						error = ERR_RW_XMLFILE ;
					}
				}
			}
		fm_close_file(handleFR) ;
		fm_close_file(handleFW) ;
	
		error = adjustFilenames(fileName,fileNameTmp);
		}
	else error = XML_PARSE_FILE_NOT_FOUND;

	return (error) ;
}
int writeNewRecipeInXML(int handle, text recipeName) 
{
	int ok, indIngredient ;

	ok = fm_write_text(handle, "\t<recipe title=\""+ recipeName + "\">\n") ;
	if (ok)
		ok = fm_write_text(handle, "\t\t<ingredients>\n") ;
	if (ok) {
		indIngredient = 0 ;
		while (indIngredient < MAX_INGREDIENT && ok) {
			ok = writeIngredientRow(handle, indIngredient) ;
			indIngredient++ ;
			}
		}
	if (ok)
		ok = fm_write_text(handle, "\t\t</ingredients>\n") ;
	if (ok)
		ok = fm_write_text(handle, "\t</recipe>\n") ;
	if (ok)
		ok = fm_write_text(handle, "</recipes>\n") ;
	
	return (ok) ;
}
int writeIngredientRow(int handle, int indIngredient)
{
	int ok ;
	text rowIngredient<500>, qtyValue, kcalValue;

	dc_int_to_text(qtyValue,vardb[INGREDIENTX_QTY + indIngredient],4,0,0,0,0,0);
	dc_int_to_text(kcalValue,vardb[INGREDIENTX_KCAL + indIngredient],4,1,0,0,0,0);

	rowIngredient = "\t\t\t<ingredient um=\"g\" " +  attNameQnty + "=\"" + qtyValue + "\" " +  attNameKcal + "=\"" +  kcalValue + "\">" + ingredient[indIngredient] + "</ingredient>\n";
	
	ok = fm_write_text(handle, rowIngredient) ;
	
	return (ok) ;
}
initXMLstring()
{
	fileNameXML = "XML\\recipes.xml";

	attNameQnty  =  "qta" ;
	attNameKcal  =  "kcal" ;

	ingredient[0] = "Flour" ;
	ingredient[1] = "Sugar" ;
	ingredient[2] = "Eggs" ;
	ingredient[3] = "Yeast" ;
	ingredient[4] = "Butter" ;
	ingredient[5] = "Milk" ;
	ingredient[6] = "Jam" ;
	ingredient[7] = "Chocolate" ;
	ingredient[8] = "Fruits" ;
	ingredient[9] = "Liquor" ;
}
int getIndexBaseDesc(text desc)
{
	int found = FALSE, i = 0, index = -1 ;

	while (i < MAX_INGREDIENT && !found) {
		if (ingredient[i] == desc) {
			index = i ;
			found = TRUE ;
			}
		i++ ;
		}
	
	return (index) ;
}
