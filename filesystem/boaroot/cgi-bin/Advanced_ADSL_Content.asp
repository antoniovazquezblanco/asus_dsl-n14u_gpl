<%
If Request_Form("saveFlag") = "1" Then
	if tcWebApi_get("WebCustom_Entry", "isDSLPowerSavingSupport", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","dslx_power_saving","dslx_power_saving")
	end if
tcWebApi_set("Adsl_Entry","dslx_snrm_offset","dslx_snrm_offset")
tcWebApi_set("Adsl_Entry","vdsl_snrm_offset","vdsl_snrm_offset")
tcWebApi_set("Adsl_Entry","vdsl_tx_gain_off","vdsl_tx_gain_off")
tcWebApi_set("Adsl_Entry","vdsl_rx_agc","vdsl_rx_agc")
tcWebApi_set("Adsl_Entry","vdsl_upbo","vdsl_upbo")
tcWebApi_set("Adsl_Entry","dslx_sra","dslx_sra")
tcWebApi_set("Adsl_Entry","dslx_bitswap","dslx_bitswap")
tcWebApi_set("Adsl_Entry","MODULATIONTYPE","ModulationType")
tcWebApi_set("Adsl_Entry","ANNEXTYPEA","AnnexTypeA")
tcWebApi_set("Wan_Common","UniqueMac","UniqueMac")
tcWebApi_set("Wan_Common","sharepvc","sharepvc")
	if tcWebApi_get("WebCustom_Entry", "havePtm", "h") = "Yes" then
		tcWebApi_set("Adsl_Entry","vdsl_profile","vdsl_profile")
	end if
tcWebApi_commit("Adsl_Entry")
end if
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_ADSL_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu_dsl_setting","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<link rel="stylesheet" type="text/css" href="/other.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
function initial(){
	show_menu();
}

function redirect(){
	document.location.href = "/cgi-bin/Advanced_ADSL_Content.asp";
}

function applyRule(){
	document.adv_adsl.saveFlag.value = 1;
	showLoading(8);
	setTimeout("redirect();", 8000);
	document.adv_adsl.submit();
}
function doSharePVCChange(){
	with (document.adv_adsl){
		if(sharepvc.value == 0)
			UniqueMac.value = "0";
		else
			UniqueMac.value = "1";
	}
	return;
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<div class="drword" id="drword" style="height:110px;"><%tcWebApi_get("String_Entry","Main_alert_proceeding_desc4","s")%> <%tcWebApi_get("String_Entry","Main_alert_proceeding_desc1","s")%>...
<br/>
<br/>
</div>
<div class="drImg"><img src="/images/alertImg.png"></div>
<div style="height:70px;"></div>
</td>
</tr>
</table>
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="" target="hidden_frame">
<input type="hidden" name="action_mode" value="">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value=""></form>
<FORM METHOD="POST" ACTION="/cgi-bin/Advanced_ADSL_Content.asp" name="adv_adsl" target="hidden_frame">
<input type="hidden" name="UniqueMac" value="<%tcWebApi_get("Wan_Common", "UniqueMac", "s")%>"/>
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

<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top" >
<div>&nbsp;</div>
<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_6","s")%> - <%tcWebApi_get("String_Entry","menu_dsl_setting","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","dslsetting_disc0","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th width="30%"><%tcWebApi_get("String_Entry","dslsetting_disc1","s")%></th>
<td>
	<SELECT NAME="ModulationType" class="input_option">
	<%if tcWebApi_get("Adsl_Entry","ANNEXTYPE","h") = "B" then %>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>>Auto Sync-Up
		<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>>ADSL Auto Sync-Up
		<%end if%>
		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>>VDSL2
		<%end if%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>>ADSL2+
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>>ADSL2
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>>G.DMT
	<%else%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "Auto Sync-Up" then asp_Write("selected") end if %>>Auto Sync-Up
		<%if tcWebApi_get("WebCustom_Entry","isCwmpVDSLSupport","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL Auto Sync-Up" then asp_Write("selected") end if %>>ADSL Auto Sync-Up
		<%end if%>
		<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "VDSL2" then asp_Write("selected") end if %>>VDSL2
		<%end if%>
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2+" then asp_Write("selected") end if %>>ADSL2+
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "ADSL2" then asp_Write("selected") end if %>>ADSL2
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.DMT" then asp_Write("selected") end if %>>G.DMT
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "T1.413" then asp_Write("selected") end if %>>T1.413
		<option <% if tcWebApi_get("Adsl_Entry","MODULATIONTYPE","h") = "G.lite" then asp_Write("selected") end if %>>G.lite
	<%end if%>
	</SELECT>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","dslsetting_disc2","s")%></th>
<td>
	<SELECT NAME="AnnexTypeA" class="input_option">
		<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A" then asp_Write("selected") end if %>>ANNEX A
		<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX I" then asp_Write("selected") end if %>>ANNEX I
		<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/L" then asp_Write("selected") end if %>>ANNEX A/L
		<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX M" then asp_Write("selected") end if %>>ANNEX M
		<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX A/I/J/L/M" then asp_Write("selected") end if %>>ANNEX A/I/J/L/M
		<%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "DSL-N12U-C1" then%>
			<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B" then asp_Write("selected") end if %>>ANNEX B
			<option <% if tcWebApi_get("Adsl_Entry","ANNEXTYPEA","h") = "ANNEX B/J/M" then asp_Write("selected") end if %>>ANNEX B/J/M
		<%end if%>
	</SELECT>
</td>
</tr>
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,1);"><%tcWebApi_get("String_Entry","dslsetting_Stability_Adj","s")%> (ADSL)</a>
</th>
<td>
	<select id="" class="input_option" name="dslx_snrm_offset">
		<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		<option value="2560" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "2560" then asp_Write("selected") end if %>>5 dB</option>
		<option value="2048" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "2048" then asp_Write("selected") end if %>>4 dB</option>
		<option value="1536" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "1536" then asp_Write("selected") end if %>>3 dB</option>
		<option value="1024" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "1024" then asp_Write("selected") end if %>>2 dB</option>
		<option value="512" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "512" then asp_Write("selected") end if %>>1 dB</option>
		<option value="-512" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-512" then asp_Write("selected") end if %>>-1 dB</option>
		<option value="-1024" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-1024" then asp_Write("selected") end if %>>-2 dB</option>
		<option value="-1536" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-1536" then asp_Write("selected") end if %>>-3 dB</option>
		<option value="-2048" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-2048" then asp_Write("selected") end if %>>-4 dB</option>
		<option value="-2560" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-2560" then asp_Write("selected") end if %>>-5 dB</option>
		<option value="-3072" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-3072" then asp_Write("selected") end if %>>-6 dB</option> 
		<option value="-3584" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-3584" then asp_Write("selected") end if %>>-7 dB</option> 
		<option value="-4096" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-4096" then asp_Write("selected") end if %>>-8 dB</option> 
		<option value="-4608" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-4608" then asp_Write("selected") end if %>>-9 dB</option> 
		<option value="-5120" <% if tcWebApi_get("Adsl_Entry","dslx_snrm_offset","h") = "-5120" then asp_Write("selected") end if %>>-10 dB</option>
	</select>
</td>
</tr>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,6);"><%tcWebApi_get("String_Entry","dslsetting_Stability_Adj","s")%> (VDSL)</a>
</th>
<td>
	<select id="" class="input_option" name="vdsl_snrm_offset">
		<option value="32767" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "32767" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		<option value="2560" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "2560" then asp_Write("selected") end if %>>5 dB</option>
		<option value="3072" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "3072" then asp_Write("selected") end if %>>6 dB</option>
		<option value="3584" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "3584" then asp_Write("selected") end if %>>7 dB</option>
		<option value="4096" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "4096" then asp_Write("selected") end if %>>8 dB</option>
		<option value="4608" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "4608" then asp_Write("selected") end if %>>9 dB</option>
		<option value="5120" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "5120" then asp_Write("selected") end if %>>10 dB</option>
		<option value="5632" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "5632" then asp_Write("selected") end if %>>11 dB</option>
		<option value="6144" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "6144" then asp_Write("selected") end if %>>12 dB</option>
		<option value="6656" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "6656" then asp_Write("selected") end if %>>13 dB</option>
		<option value="7168" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "7168" then asp_Write("selected") end if %>>14 dB</option>
		<option value="7680" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "7680" then asp_Write("selected") end if %>>15 dB</option>
		<option value="8192" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "8192" then asp_Write("selected") end if %>>16 dB</option>
		<option value="8704" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "8704" then asp_Write("selected") end if %>>17 dB</option>
		<option value="9216" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "9216" then asp_Write("selected") end if %>>18 dB</option>
		<option value="9728" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "9728" then asp_Write("selected") end if %>>19 dB</option>
		<option value="10240" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "10240" then asp_Write("selected") end if %>>20 dB</option>
		<option value="10752" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "10752" then asp_Write("selected") end if %>>21 dB</option>
		<option value="11264" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "11264" then asp_Write("selected") end if %>>22 dB</option>
		<option value="11776" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "11776" then asp_Write("selected") end if %>>23 dB</option>
		<option value="12288" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "12288" then asp_Write("selected") end if %>>24 dB</option>
		<option value="12800" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "12800" then asp_Write("selected") end if %>>25 dB</option>
		<option value="13312" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "13312" then asp_Write("selected") end if %>>26 dB</option>
		<option value="13824" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "13824" then asp_Write("selected") end if %>>27 dB</option>
		<option value="14336" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "14336" then asp_Write("selected") end if %>>28 dB</option>
		<option value="14848" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "14848" then asp_Write("selected") end if %>>29 dB</option>
		<option value="15360" <% if tcWebApi_get("Adsl_Entry","vdsl_snrm_offset","h") = "15360" then asp_Write("selected") end if %>>30 dB</option>
	</select>
</td>
</tr>

<!--vdsl_tx_gain_off-->
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,8);">Tx Power Control (VDSL)</a>
</th>
<td>
	<select id="" class="input_option" name="vdsl_tx_gain_off">
		<option value="32767" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "32767" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		<option value="30" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "30" then asp_Write("selected") end if %>>3 dB</option>
		<option value="20" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "20" then asp_Write("selected") end if %>>2 dB</option>
		<option value="10" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "10" then asp_Write("selected") end if %>>1 dB</option>
		<option value="0" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "0" then asp_Write("selected") end if %>>0 dB</option>
		<option value="-10" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-10" then asp_Write("selected") end if %>>-1 dB</option>
		<option value="-20" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-20" then asp_Write("selected") end if %>>-2 dB</option>
		<option value="-30" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-30" then asp_Write("selected") end if %>>-3 dB</option>
		<option value="-40" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-40" then asp_Write("selected") end if %>>-4 dB</option>
		<option value="-50" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-50" then asp_Write("selected") end if %>>-5 dB</option>
		<option value="-60" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-60" then asp_Write("selected") end if %>>-6 dB</option>
		<option value="-70" <% if tcWebApi_get("Adsl_Entry","vdsl_tx_gain_off","h") = "-70" then asp_Write("selected") end if %>>-7 dB</option>
	</select>
</td>
</tr>

<!--vdsl_rx_agc-->
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,9);">Rx AGC GAIN Adjustment (VDSL)</a>
</th>
<td>
	<select id="" class="input_option" name="vdsl_rx_agc">
		<option value="65535" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "65535" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		<option value="394" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "394" then asp_Write("selected") end if %>>Stable</option>
		<option value="476" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "476" then asp_Write("selected") end if %>>Balance</option>
		<option value="550" <% if tcWebApi_get("Adsl_Entry","vdsl_rx_agc","h") = "550" then asp_Write("selected") end if %>>High Performance</option>
	</select>
</td>
</tr>

<!--upbo stands for upstream power back off-->
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,10);">UPBO - Upstream Power Back Off (VDSL)</a>
</th>
<td>
	<select id="" class="input_option" name="vdsl_upbo">
		<option value="auto" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "auto" then asp_Write("selected") end if %>><% tcWebApi_Get("String_Entry", "Auto", "s") %></option>
		<option value="on" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "on" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
		<option value="off" <% if tcWebApi_get("Adsl_Entry","vdsl_upbo","h") = "off" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
	</select>
</td>
</tr>
<%end if%>

<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,2);"><%tcWebApi_get("String_Entry","dslsetting_SRA","s")%></a>
</th>
<td>
	<select id="" class="input_option" name="dslx_sra">
		<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_sra","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
		<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_sra","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
	</select>
</td>
</tr>
<tr>
<th>
<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,4);">Bitswap</a>
</th>
<td>
	<select id="" class="input_option" name="dslx_bitswap">
		<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_bitswap","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
		<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_bitswap","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
	</select>
</td>
</tr>
<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,3);"><%tcWebApi_get("String_Entry","dslsetting_sharePVC","s")%></a> 				
	</th>
	<td>
		<select class="input_option" name="sharepvc" onchange="doSharePVCChange()">
			<option value="1" <% if tcWebApi_get("Wan_Common","sharepvc","h") = "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Enabled","s")%></option>
			<option value="0" <% if tcWebApi_get("Wan_Common","sharepvc","h") = "0" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		</select>
	</td>
</tr>

<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
<tr>
<th>
	<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,7);">VDSL Profile</a>
</th>
<td>
<input type="radio" name="vdsl_profile" class="input" value="0" <% If TCWebApi_get("Adsl_Entry","vdsl_profile","h") <> "1" then asp_Write("checked") end if%> >30a multi mode
<input type="radio" name="vdsl_profile" class="input" value="1" <% If TCWebApi_get("Adsl_Entry","vdsl_profile","h") = "1" then asp_Write("checked") end if%> >17a multi mode
</td>
</tr>
<%end if%>

<%if tcWebApi_get("WebCustom_Entry", "isDSLPowerSavingSupport", "h") = "Yes" then%>
<tr>
	<th>
		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(25,5);"><%tcWebApi_get("String_Entry","DSL_pwr_saving_in","s")%></a>
	</th>
	<td>
		<select class="input_option" name="dslx_power_saving">
			<option value="1" <% if tcWebApi_get("Adsl_Entry","dslx_power_saving","h") = "1" then asp_Write("selected") end if %>>Auto Adjust</option>
			<option value="0" <% if tcWebApi_get("Adsl_Entry","dslx_power_saving","h") <> "1" then asp_Write("selected") end if %>><%tcWebApi_get("String_Entry","btn_Disabled","s")%></option>
		</select>
	</td>
</tr>
<%end if%>
</table>
<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_ADSL_Content.asp-->
</html>
