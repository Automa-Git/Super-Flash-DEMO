// Note: Display XML Recipes

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

main()
{
	text path = "XML";
	text fileName1 = "output1.xml";
	text fileName2 = "output2.xml";

	displayOnIE(path, fileName1) ;
	displayOnIE(path, fileName2) ;
}
