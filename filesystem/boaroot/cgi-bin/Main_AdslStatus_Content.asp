<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--Main_AdslStatus_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<meta HTTP-EQUIV="refresh" CONTENT="10">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu_dsl_log","s")%></title>
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
var sync_status = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
var adsl_timestamp = "<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>";
var adsl_boottime = boottime - parseInt(adsl_timestamp);

function showadslbootTime(){
	if((adsl_timestamp != "") && (sync_status == "up"))
	{
		Days = Math.floor(adsl_boottime / (60*60*24));
		Hours = Math.floor((adsl_boottime / 3600) % 24);
		Minutes = Math.floor(adsl_boottime % 3600 / 60);
		Seconds = Math.floor(adsl_boottime % 60);

		$("boot_days").innerHTML = Days;
		$("boot_hours").innerHTML = Hours;
		$("boot_minutes").innerHTML = Minutes;
		$("boot_seconds").innerHTML = Seconds;
		adsl_boottime += 1;
		setTimeout("showadslbootTime()", 1000);
	}
	else
	{
		$("boot_days").innerHTML = "0";
		$("boot_hours").innerHTML = "0";
		$("boot_minutes").innerHTML = "0";
		$("boot_seconds").innerHTML = "0";
	}
}
</script>
</head>
<body onload="show_menu();load_body();showadslbootTime();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="apply.cgi" target="hidden_frame">
<input type="hidden" name="current_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="next_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
</form>
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td align="left" valign="top">
<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
<tr>
<td bgcolor="#4D595D" colspan="3" valign="top">
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","System_Log","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_log","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><% tcWebApi_Get("String_Entry", "GeneralLog_title", "s") %></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","adsl_fw_ver_in","s")%></th>
<td>
	<%If tcWebApi_get("Info_Adsl","fwVer","h") <> "" then tcWebApi_staticGet("Info_Adsl","fwVer","s") end if%>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","adsl_link_sts_in","s")%></th>
<td>
	<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>
</td>
</tr>
<tr>
<th width="20%">DSL <%tcWebApi_get("String_Entry","General_x_SystemUpTime_in","s")%>
<td>
	<span id="boot_days"></span> <%tcWebApi_get("String_Entry","Day","s")%> <span id="boot_hours"></span> <%tcWebApi_get("String_Entry","Hour","s")%> <span id="boot_minutes"></span> <%tcWebApi_get("String_Entry","Minute","s")%> <span id="boot_seconds"></span> <%tcWebApi_get("String_Entry","Second","s")%>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></th>
<td>
	<% if tcWebApi_get("Info_Adsl","Opmode","h") <> "" then tcWebApi_get("Info_Adsl","Opmode","s") end if%>
</td>
</tr>
<tr>
<th width="20%"><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></th>
<td>
	<% if tcWebApi_get("Info_Adsl","AdslType","h") <> "" then tcWebApi_get("Info_Adsl","AdslType","s") end if%>
</td>
</tr>
<tr>
<th width="20%">SNR Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","SNRMarginDown","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginDown","s") end if%>
</td>
</tr>
<tr>
<th width="20%">SNR Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","SNRMarginUp","h") <> "" then tcWebApi_get("Info_Adsl","SNRMarginUp","s") end if%>
</td>
</tr>
<tr>
<th width="20%">Line Attenuation Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","AttenDown","h") <> "" then tcWebApi_get("Info_Adsl","AttenDown","s") end if%>
</td>
</tr>
<tr>
<th width="20%">Line Attenuation Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","AttenUp","h") <> "" then tcWebApi_get("Info_Adsl","AttenUp","s") end if%>
</td>
</tr>
<tr>
<th width="20%">Data Rate Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","DataRateDown","h") <> "" then tcWebApi_get("Info_Adsl","DataRateDown","s") end if%>
</td>
</tr>
<tr>
<th width="20%">Data Rate Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","DataRateUp","h") <> "" then tcWebApi_get("Info_Adsl","DataRateUp","s") end if%>
</td>
</tr>
<tr>
<th width="20%">MAX Rate Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","AttainDown","h") <> "" then tcWebApi_get("Info_Adsl","AttainDown","s") end if%>&nbsp;kbps
</td>
</tr>
<tr>
<th width="20%">MAX Rate Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","AttainUp","h") <> "" then tcWebApi_get("Info_Adsl","AttainUp","s") end if%>&nbsp;kbps
</td>
</tr>
<tr>
<th width="20%">POWER Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","PowerDown","h") <> "" then tcWebApi_get("Info_Adsl","PowerDown","s") end if%>
</td>
</tr>
<tr>
<th width="20%">POWER Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","PowerUp","h") <> "" then tcWebApi_get("Info_Adsl","PowerUp","s") end if%>
</td>
</tr>
<tr>
<th width="20%">CRC Down</th>
<td>
	<% if tcWebApi_get("Info_Adsl","CRCDown","h") <> "" then tcWebApi_get("Info_Adsl","CRCDown","s") end if%>
</td>
</tr>
<tr>
<th width="20%">CRC Up</th>
<td>
	<% if tcWebApi_get("Info_Adsl","CRCUp","h") <> "" then tcWebApi_get("Info_Adsl","CRCUp","s") end if%>
</td>
</tr>
</table>
</td>
</tr>
<tr class="apply_gen" valign="top">
<td width="100%" align="center" >
<form method="post" name="form3" action="apply.cgi">
<input type="hidden" name="current_page" value="Main_LogStatus_Content.asp">
<input type="hidden" name="action_mode" value=" Refresh ">
<input type="hidden" name="next_host" value="">
<input type="button" onClick="location.href=location.href" value="<%tcWebApi_get("String_Entry","CTL_refresh","s")%>" class="button_gen">
</form>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top"></td>
</tr>
</table>
<div id="footer"></div>
</form>
</body>

<!--Main_AdslStatus_Content.asp-->
</html>

