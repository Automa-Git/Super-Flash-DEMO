<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
	<xsl:template match="/">
		<HTML>
		<HEAD>
			<link rel="stylesheet" type="text/css" href="styleOut2.css" />
			<title>Super-Flash Recipes</title>
			<script src="jquery-3.4.1.min.js"></script>
			<script>
				$(document).ready(function() { setInterval("refreshPage()", 10000); });
	 
				function refreshPage() { location.reload(); }
			</script>
		</HEAD>
		<BODY>
			<div class="row">
				<xsl:apply-templates select="document('recipes.xml')//recipe"/>
			</div> 
		</BODY>
		</HTML>
	</xsl:template>

	<xsl:template match="//recipe">
		<!-- recipe element -->
		<div class="column">
			<p class="title"><xsl:value-of select="@title"/></p>
			<TABLE width="90%" >
			<THEAD>
				<tr>
					<th>Ingredient</th>
					<th>Quantity</th> 
					<th>Kcal</th>
				</tr>
			</THEAD>
			<TBODY>
				<xsl:apply-templates select="ingredients/ingredient"/>
			</TBODY>
			<TFOOT>
				<tr>
					<td colspan="2" class="total">Total Amount Kcal</td>
					<td><xsl:value-of select="sum(ingredients/ingredient/@kcal)"/></td>
				</tr>    
			</TFOOT>
			</TABLE>
		</div>
	</xsl:template>
	
	<xsl:template match="ingredients/ingredient">	
	<!-- list of ingredients used  -->
		<xsl:if test="@qta > 0"> 
			<tr>
				<!-- column Ingredient -->
				<td class="firstCln" width="15%"><xsl:value-of select="."/>    [<xsl:value-of select="@um"/>]</td>
				<!--  - column Quantity -->
				<td width="20%"><xsl:value-of select="@qta"/></td>
				<!--  - column Kcal -->
				<td width="20%"><xsl:value-of select="@kcal"/></td>
			</tr>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>