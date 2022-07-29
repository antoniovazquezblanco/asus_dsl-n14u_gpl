<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Main_DHCPStatus_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_7_3","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';

	var tableData = [
		//["1", "BVA-NB","192.168.177.168","00:22:15:A5:03:68"],
		["1", "<%tcWebApi_staticGet("DhcpLease_Entry0", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry0", "MAC","s")%>"],
		["2", "<%tcWebApi_staticGet("DhcpLease_Entry1", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry1", "MAC","s")%>"],
		["3", "<%tcWebApi_staticGet("DhcpLease_Entry2", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry2", "MAC","s")%>"],
		["4", "<%tcWebApi_staticGet("DhcpLease_Entry3", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry3", "MAC","s")%>"],
		["5", "<%tcWebApi_staticGet("DhcpLease_Entry4", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry4", "MAC","s")%>"],
		["6", "<%tcWebApi_staticGet("DhcpLease_Entry5", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry5", "MAC","s")%>"],
		["7", "<%tcWebApi_staticGet("DhcpLease_Entry6", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry6", "MAC","s")%>"],
		["8", "<%tcWebApi_staticGet("DhcpLease_Entry7", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry7", "MAC","s")%>"],
		["9", "<%tcWebApi_staticGet("DhcpLease_Entry8", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry8", "MAC","s")%>"],
		["10", "<%tcWebApi_staticGet("DhcpLease_Entry9", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry9", "MAC","s")%>"],
		["11", "<%tcWebApi_staticGet("DhcpLease_Entry10", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry10", "MAC","s")%>"],
		["12", "<%tcWebApi_staticGet("DhcpLease_Entry11", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry11", "MAC","s")%>"],
		["13", "<%tcWebApi_staticGet("DhcpLease_Entry12", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry12", "MAC","s")%>"],
		["14", "<%tcWebApi_staticGet("DhcpLease_Entry13", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry13", "MAC","s")%>"],
		["15", "<%tcWebApi_staticGet("DhcpLease_Entry14", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry14", "MAC","s")%>"],
		["16", "<%tcWebApi_staticGet("DhcpLease_Entry15", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry15", "MAC","s")%>"],
		["17", "<%tcWebApi_staticGet("DhcpLease_Entry16", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry16", "MAC","s")%>"],
		["18", "<%tcWebApi_staticGet("DhcpLease_Entry17", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry17", "MAC","s")%>"],
		["19", "<%tcWebApi_staticGet("DhcpLease_Entry18", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry18", "MAC","s")%>"],
		["20", "<%tcWebApi_staticGet("DhcpLease_Entry19", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry19", "MAC","s")%>"],
		["21", "<%tcWebApi_staticGet("DhcpLease_Entry20", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry20", "MAC","s")%>"],
		["22", "<%tcWebApi_staticGet("DhcpLease_Entry21", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry21", "MAC","s")%>"],
		["23", "<%tcWebApi_staticGet("DhcpLease_Entry22", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry22", "MAC","s")%>"],
		["24", "<%tcWebApi_staticGet("DhcpLease_Entry23", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry23", "MAC","s")%>"],
		["25", "<%tcWebApi_staticGet("DhcpLease_Entry24", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry24", "MAC","s")%>"],
		["26", "<%tcWebApi_staticGet("DhcpLease_Entry25", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry25", "MAC","s")%>"],
		["27", "<%tcWebApi_staticGet("DhcpLease_Entry26", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry26", "MAC","s")%>"],
		["28", "<%tcWebApi_staticGet("DhcpLease_Entry27", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry27", "MAC","s")%>"],
		["29", "<%tcWebApi_staticGet("DhcpLease_Entry28", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry28", "MAC","s")%>"],
		["30", "<%tcWebApi_staticGet("DhcpLease_Entry29", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry29", "MAC","s")%>"],
		["31", "<%tcWebApi_staticGet("DhcpLease_Entry30", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry30", "MAC","s")%>"],
		["32", "<%tcWebApi_staticGet("DhcpLease_Entry31", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry31", "MAC","s")%>"],
		["33", "<%tcWebApi_staticGet("DhcpLease_Entry32", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry32", "MAC","s")%>"],
		["34", "<%tcWebApi_staticGet("DhcpLease_Entry33", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry33", "MAC","s")%>"],
		["35", "<%tcWebApi_staticGet("DhcpLease_Entry34", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry34", "MAC","s")%>"],
		["36", "<%tcWebApi_staticGet("DhcpLease_Entry35", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry35", "MAC","s")%>"],
		["37", "<%tcWebApi_staticGet("DhcpLease_Entry36", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry36", "MAC","s")%>"],
		["38", "<%tcWebApi_staticGet("DhcpLease_Entry37", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry37", "MAC","s")%>"],
		["39", "<%tcWebApi_staticGet("DhcpLease_Entry38", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry38", "MAC","s")%>"],
		["40", "<%tcWebApi_staticGet("DhcpLease_Entry39", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry39", "MAC","s")%>"],
		["41", "<%tcWebApi_staticGet("DhcpLease_Entry40", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry40", "MAC","s")%>"],
		["42", "<%tcWebApi_staticGet("DhcpLease_Entry41", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry41", "MAC","s")%>"],
		["43", "<%tcWebApi_staticGet("DhcpLease_Entry42", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry42", "MAC","s")%>"],
		["44", "<%tcWebApi_staticGet("DhcpLease_Entry43", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry43", "MAC","s")%>"],
		["45", "<%tcWebApi_staticGet("DhcpLease_Entry44", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry44", "MAC","s")%>"],
		["46", "<%tcWebApi_staticGet("DhcpLease_Entry45", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry45", "MAC","s")%>"],
		["47", "<%tcWebApi_staticGet("DhcpLease_Entry46", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry46", "MAC","s")%>"],
		["48", "<%tcWebApi_staticGet("DhcpLease_Entry47", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry47", "MAC","s")%>"],
		["49", "<%tcWebApi_staticGet("DhcpLease_Entry48", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry48", "MAC","s")%>"],
		["50", "<%tcWebApi_staticGet("DhcpLease_Entry49", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry49", "MAC","s")%>"],
		["51", "<%tcWebApi_staticGet("DhcpLease_Entry50", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry50", "MAC","s")%>"],
		["52", "<%tcWebApi_staticGet("DhcpLease_Entry51", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry51", "MAC","s")%>"],
		["53", "<%tcWebApi_staticGet("DhcpLease_Entry52", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry52", "MAC","s")%>"],
		["54", "<%tcWebApi_staticGet("DhcpLease_Entry53", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry53", "MAC","s")%>"],
		["55", "<%tcWebApi_staticGet("DhcpLease_Entry54", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry54", "MAC","s")%>"],
		["56", "<%tcWebApi_staticGet("DhcpLease_Entry55", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry55", "MAC","s")%>"],
		["57", "<%tcWebApi_staticGet("DhcpLease_Entry56", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry56", "MAC","s")%>"],
		["58", "<%tcWebApi_staticGet("DhcpLease_Entry57", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry57", "MAC","s")%>"],
		["59", "<%tcWebApi_staticGet("DhcpLease_Entry58", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry58", "MAC","s")%>"],
		["60", "<%tcWebApi_staticGet("DhcpLease_Entry59", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry59", "MAC","s")%>"],
		["61", "<%tcWebApi_staticGet("DhcpLease_Entry60", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry60", "MAC","s")%>"],
		["62", "<%tcWebApi_staticGet("DhcpLease_Entry61", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry61", "MAC","s")%>"],
		["63", "<%tcWebApi_staticGet("DhcpLease_Entry62", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry62", "MAC","s")%>"],
		["64", "<%tcWebApi_staticGet("DhcpLease_Entry63", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry63", "MAC","s")%>"],
		["65", "<%tcWebApi_staticGet("DhcpLease_Entry64", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry64", "MAC","s")%>"],
		["66", "<%tcWebApi_staticGet("DhcpLease_Entry65", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry65", "MAC","s")%>"],
		["67", "<%tcWebApi_staticGet("DhcpLease_Entry66", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry66", "MAC","s")%>"],
		["68", "<%tcWebApi_staticGet("DhcpLease_Entry67", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry67", "MAC","s")%>"],
		["69", "<%tcWebApi_staticGet("DhcpLease_Entry68", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry68", "MAC","s")%>"],
		["70", "<%tcWebApi_staticGet("DhcpLease_Entry69", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry69", "MAC","s")%>"],
		["71", "<%tcWebApi_staticGet("DhcpLease_Entry70", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry70", "MAC","s")%>"],
		["72", "<%tcWebApi_staticGet("DhcpLease_Entry71", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry71", "MAC","s")%>"],
		["73", "<%tcWebApi_staticGet("DhcpLease_Entry72", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry72", "MAC","s")%>"],
		["74", "<%tcWebApi_staticGet("DhcpLease_Entry73", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry73", "MAC","s")%>"],
		["75", "<%tcWebApi_staticGet("DhcpLease_Entry74", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry74", "MAC","s")%>"],
		["76", "<%tcWebApi_staticGet("DhcpLease_Entry75", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry75", "MAC","s")%>"],
		["77", "<%tcWebApi_staticGet("DhcpLease_Entry76", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry76", "MAC","s")%>"],
		["78", "<%tcWebApi_staticGet("DhcpLease_Entry77", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry77", "MAC","s")%>"],
		["79", "<%tcWebApi_staticGet("DhcpLease_Entry78", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry78", "MAC","s")%>"],
		["80", "<%tcWebApi_staticGet("DhcpLease_Entry79", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry79", "MAC","s")%>"],
		["81", "<%tcWebApi_staticGet("DhcpLease_Entry80", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry80", "MAC","s")%>"],
		["82", "<%tcWebApi_staticGet("DhcpLease_Entry81", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry81", "MAC","s")%>"],
		["83", "<%tcWebApi_staticGet("DhcpLease_Entry82", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry82", "MAC","s")%>"],
		["84", "<%tcWebApi_staticGet("DhcpLease_Entry83", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry83", "MAC","s")%>"],
		["85", "<%tcWebApi_staticGet("DhcpLease_Entry84", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry84", "MAC","s")%>"],
		["86", "<%tcWebApi_staticGet("DhcpLease_Entry85", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry85", "MAC","s")%>"],
		["87", "<%tcWebApi_staticGet("DhcpLease_Entry86", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry86", "MAC","s")%>"],
		["88", "<%tcWebApi_staticGet("DhcpLease_Entry87", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry87", "MAC","s")%>"],
		["89", "<%tcWebApi_staticGet("DhcpLease_Entry88", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry88", "MAC","s")%>"],
		["90", "<%tcWebApi_staticGet("DhcpLease_Entry89", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry89", "MAC","s")%>"],
		["91", "<%tcWebApi_staticGet("DhcpLease_Entry90", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry90", "MAC","s")%>"],
		["92", "<%tcWebApi_staticGet("DhcpLease_Entry91", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry91", "MAC","s")%>"],
		["93", "<%tcWebApi_staticGet("DhcpLease_Entry92", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry92", "MAC","s")%>"],
		["94", "<%tcWebApi_staticGet("DhcpLease_Entry93", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry93", "MAC","s")%>"],
		["95", "<%tcWebApi_staticGet("DhcpLease_Entry94", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry94", "MAC","s")%>"],
		["96", "<%tcWebApi_staticGet("DhcpLease_Entry95", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry95", "MAC","s")%>"],
		["97", "<%tcWebApi_staticGet("DhcpLease_Entry96", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry96", "MAC","s")%>"],
		["98", "<%tcWebApi_staticGet("DhcpLease_Entry97", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry97", "MAC","s")%>"],
		["99", "<%tcWebApi_staticGet("DhcpLease_Entry98", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry98", "MAC","s")%>"],
		["100", "<%tcWebApi_staticGet("DhcpLease_Entry99", "HostName","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "IP","s")%>","<%tcWebApi_staticGet("DhcpLease_Entry99", "MAC","s")%>"]
	];

function showTable(id,data,keyIndex){
	var content = "Mac Address            IP Address         Host name\n";

	//tableData = [["1", "BVA-NB","192.168.177.168","00:22:15:A5:03:68"], [xx], ..]
	for(var i =0; i<data.length; i++){
		if(data[i][keyIndex] != ""){
			if(data[i][1] == "N/A")
				content +=data[i][3] + "      " + data[i][2] + "        " + data[i][3] + "\n";
			else
				content +=data[i][3] + "      " + data[i][2] + "        " + data[i][1] + "\n";	
		}
	}

	document.getElementById('tarea').value = content;
}

function initial()
{
	showTable('dhcpclientList',tableData,2);
}
</script>
</head>
<body onload="initial(); show_menu();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="apply.cgi" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_DHCPStatus_Content.asp">
<input type="hidden" name="next_page" value="Main_DHCPStatus_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div></td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td valign="top">
<table width="760px" border="0" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
<tbody>
<tr bgcolor="#4D595D">
<td valign="top">
<div>&nbsp;</div>
		  <div class="formfonttitle"><%tcWebApi_get("String_Entry","System_Log","s")%> - <%tcWebApi_get("String_Entry","menu5_7_3","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","DHCPlease_title","s")%></div>
<div style="margin-top:8px">
				<textarea id="tarea" cols="63" rows="25" readonly="readonly" wrap=off style="font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;"></textarea>
</div>
			<div class="apply_gen">
			  		<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
			</div>				
</td>
</tr>
</tbody>
</table>
</form>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Main_DHCPStatus_Content.asp-->
</html>

