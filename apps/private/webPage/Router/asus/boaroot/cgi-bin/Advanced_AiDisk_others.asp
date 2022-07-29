<%
If Request_Form("Sambaflag")="1" then 
	/*TCWebAPI_set("Samba_Entry","WorkGroup","WorkGroup")*/
	tcWebApi_Set("Samba_Entry","st_samba_workgroup","st_samba_workgroup")
	tcWebApi_Set("Samba_Entry","NetBiosName","computer_name")
	tcWebApi_Set("Samba_Entry","st_max_user","st_max_user")
	tcWebApi_Set("Samba_Entry","ftp_lang", "ftp_lang")
	tcWebApi_Set("GUITemp_Entry0","action_script", "action_script")
	tcWebApi_Commit("Samba_Entry")
End If
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_AiDisk_others.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_4_3","s")%></title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>

function initial(){
	show_menu();
	$("option5").innerHTML = '<table><tbody><tr><td><div id="index_img5"></div></td><td><div style="width:120px;"><%tcWebApi_get("String_Entry","Menu_usb_application","s")%></div></td></tr></tbody></table>';
	$("option5").className = "m5_r";
}

function applyRule(){
	if(validForm()){
		document.form.current_page.value = "Advanced_AiDisk_others.asp";
		document.form.next_page.value = "";
		document.form.Sambaflag.value = "1";
		showLoading(3);
		setTimeout('location = "'+ location.pathname +'";', 3000);
		document.form.submit();
	}
}
function trim(str){
	return str.replace(/(^s*)|(s*$)/g, "");
}
function validForm(){
        
        if(document.form.computer_name.value.length == 0){
                showtext($("alert_msg1"), "<% tcWebApi_get("String_Entry","JS_Shareblanktest","s") %>");
                document.form.computer_name.focus();
                document.form.computer_name.select();
                return false;
        }
        else{
                
                var alert_str = validate_hostname(document.form.computer_name);
                if(alert_str != ""){
                        showtext($("alert_msg1"), alert_str);
                        $("alert_msg1").style.display = "";
                        document.form.computer_name.focus();
                        document.form.computer_name.select();
                        return false;
                }else{
                        $("alert_msg1").style.display = "none";
								}

                document.form.computer_name.value = trim(document.form.computer_name.value);
        }

	if(!validate_range(document.form.st_max_user, 1, 5)){
		document.form.st_max_user.focus();
		document.form.st_max_user.select();
		return false;
	}
	
	var workgroup_check = new RegExp('^[a-zA-Z0-9]([-a-zA-Z0-9.]*[a-zA-Z0-9]|)$','gi');
	if(!workgroup_check.test(document.form.st_samba_workgroup.value)){
        alert("<% tcWebApi_get("String_Entry","JS_validchar","s") %>");                
        document.form.st_samba_workgroup.focus();
        document.form.st_samba_workgroup.select();
        return false;
  }
	
	return true;
}
function done_validating(action){
	refreshpage();
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="Advanced_AiDisk_others.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="current_page" value="Advanced_AiDisk_others.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_ftpsamba">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<INPUT TYPE="HIDDEN" NAME="Sambaflag" VALUE="0">
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
				<td align="left" valign="top" >
				<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
				<tbody>
					<tr>
					<td bgcolor="#4D595D" valign="top" >
						<div>&nbsp;</div>
						<div style="width:730px">
							<table width="730px">
							<tr>
								<td align="left">
							<span class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_4","s")%> - <%tcWebApi_get("String_Entry","menu5_4_3","s")%></span>
								</td>
								<td align="right">
									<img onclick="go_setting('APP_Installation.asp')" align="right" style="cursor:pointer;position:absolute;margin-left:-20px;margin-top:-30px;" title="Back to USB Extension" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'">
								</td>
							</tr>
							</table>
						</div>
						<div style="margin:5px;"><img src="/images/New_ui/export/line_export.png"></div>
		  <div class="formfontdesc"><%tcWebApi_get("String_Entry","USB_App_disk_miscellaneous_desc","s")%></div>
						<table width="98%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
							<tr>
								<th width="40%">
                                                <a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,1);"><%tcWebApi_get("String_Entry","ShareNode_MaximumLoginUser_in","s")%></a>
								</th>
								<td>
									<input type="text" name="st_max_user" class="input_3_table" maxlength="1" value="<% tcWebApi_Get("Samba_Entry", "st_max_user", "s") %>" onKeyPress="return is_number(this, event);" onblur="validate_number_range(this, 1, 5)">
								</td>
							</tr>
							<tr>
								<th>
                                            <a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,2);"><%tcWebApi_get("String_Entry","ShareNode_DeviceName_in","s")%></a>
								</th>
								<td>
									<input type="text" name="computer_name" id="computer_name" class="input_15_table" maxlength="15" value="<% tcWebApi_Get("Samba_Entry", "NetBiosName", "s") %>">
									<br/><span id="alert_msg1" style="color:#FC0;"></span>
								</td>
							</tr>
							<tr>
								<th>
                                            <a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,3);"><%tcWebApi_get("String_Entry","ShareNode_WorkGroup_in","s")%></a>
								</th>
								<td>
									<input type="text" name="st_samba_workgroup" class="input_32_table" maxlength="32" value="<% tcWebApi_Get("Samba_Entry", "st_samba_workgroup", "s") %>">
								</td>
							</tr>
							<tr>
								<th>
                                        		<a class="hintstyle" href="javascript:void(0);" onClick="openHint(17,9);"><%tcWebApi_get("String_Entry","ShareNode_FTPLANG_in","s")%></a>
								</th>
								<td>
									<select name="ftp_lang" class="input_option" onChange="return change_common(this, 'Storage', 'ftp_lang');">
										<option value="CN" <%if tcWebApi_Get("Samba_Entry", "ftp_lang", "h") = "CN" then asp_write("selected") end if%>><%tcWebApi_get("String_Entry","ShareNode_FTPLANG_optionname3","s")%></option>
										<option value="TW" <%if tcWebApi_Get("Samba_Entry", "ftp_lang", "h") = "TW" then asp_write("selected") end if%>><%tcWebApi_get("String_Entry","ShareNode_FTPLANG_optionname2","s")%></option>
										<option value="EN" <%if tcWebApi_Get("Samba_Entry", "ftp_lang", "h") = "EN" then asp_write("selected") end if%>><%tcWebApi_get("String_Entry","ShareNode_FTPLANG_optionname1","s")%></option>
									</select>
								</td>
							</tr>
							<!-- Viz 2011.04 tr>
								<th>
									<a class="hintstyle" href="javascript:openHint(17, 4);"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> Download Master?</a>
								</th>
								<td>
									<input type="radio" name="apps_dl" value="1" ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="apps_dl" value="0" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr-->
							<!--tr>
								<th>
									<a class="hintstyle" href="javascript:openHint(17, 5);"><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> Download Share?</a>
								</th>
								<td>
									<input type="radio" name="apps_dl_share" value="1" ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
									<input type="radio" name="apps_dl_share" value="0" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
								</td>
							</tr-->
							<!--
							<tr>
								<th>Seeding</th>
								<td>
									<select name="apps_seed" class="input">
										<option value="1" ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_buttonname","s")%></option>
										<option value="0" ><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%></option>
									</select>
								</td>
							</tr>
							<tr>
								<th>Maximum Upload Rate</th>
								<td>
									<input type="text" name="apps_upload_max" class="input" maxlength="3" size="3" value="" /> Kb/s
								</td>
							</tr>
							-->
						</table>
						<div class="apply_gen">
            	<input type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>" onclick="applyRule();">
						</div>
					</td>
					</tr>
				</tbody>
				</table>
				</td>
			</tr>
			</table>
		</td>
		<td width="10" align="center" valign="top">&nbsp;</td>
	</tr>
</table>
</form>
<div id="footer"></div>
</body>

<!--Advanced_AiDisk_others.asp-->
</html>

