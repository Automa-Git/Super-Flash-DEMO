<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>

	<xsl:template match="/">
		<HTML>
		<HEAD>
			<link rel="stylesheet" type="text/css" href="styleOut1.css" />
			<title>Super-Flash Recipes</title>
			<script src="jquery-3.4.1.min.js"></script>
			<script>
				$(document).ready(function() { setInterval("refreshPage()", 10000); });
	 
				function refreshPage() { location.reload(); }
			</script>
		</HEAD>
		<BODY>
			<xsl:apply-templates select="document('recipes.xml')//recipe"/>
		</BODY>
		</HTML>
	</xsl:template>

	<xsl:template match="//recipe">
		<!-- recipe element -->
		<p class="title"><xsl:value-of select="@title"/></p>
		<TABLE class="table-ext">
			<THEAD>
				<tr>
					<th width="20%">Ingredient</th>
					<th width="20%">Quantity</th> 
					<th width="20%">Kcal</th>
				</tr>
			</THEAD>
			<TBODY>
				<tr>	
					<td colspan="3">
						<div class="divinterno">		
							<table class="table-int">
								<xsl:apply-templates select="ingredients/ingredient"/>
							</table>
						</div>
					</td>
				</tr>
			</TBODY>
		</TABLE>
	</xsl:template>
	
	<xsl:template match="ingredients/ingredient">	
		<!-- list of ingredients used  -->
		<tr>
			<!-- column Ingredient -->
			<td width="20%"><xsl:value-of select="."/>    [<xsl:value-of select="@um"/>]</td>
			<!--  - column Quantity -->
			<td width="20%"><xsl:value-of select="@qta"/></td>
			<!--  - column Kcal -->
			<td width="20%"><xsl:value-of select="@kcal"/></td>
		</tr>
	</xsl:template>
</xsl:stylesheet>
