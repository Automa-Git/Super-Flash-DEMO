// Note: XML-SVG display image

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

main()
{
	text htmlPath = "SVG\\HTML" ;
	text htmlFilePath = "displaySVG.html" ;

	displayOnIE(htmlPath, htmlFilePath) ;
}


