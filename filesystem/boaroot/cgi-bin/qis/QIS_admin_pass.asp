<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!--QIS_admin_pass.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<link href="/form_style.css"  rel="stylesheet" type="text/css">
<link href="/qis/qis_style.css"  rel="stylesheet" type="text/css">
<link href="/usp_style.css"  rel="stylesheet" type="text/css">
<link href="/pwdmeter.css" rel="stylesheet" type="text/css">
<style type="text/css">
.QISTutorFont{
	font-family:Segoe UI, Arial, sans-serif;
	width:280px;
	line-height: 20px;
}
</style>
<script type="text/javascript" language="JavaScript" src="/state.js"></script>
<script type="text/javascript" language="JavaScript" src="/general.js"></script>
<script type="text/javascript" language="JavaScript" src="/help.js"></script>
<script type="text/JavaScript" src="/jquery.js"></script>
<script type="text/javascript">
<% auto_detection() %>

var $j = jQuery.noConflict();
function QKinternettype_load_body(){
	parent.document.title = "ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> -  Welcome!";
	//parent.set_step("t0");
	document.form.uiViewPassword.focus();	

	if (dsl_support != -1) {
		var detect_status = '<% tcWebApi_get("AutoPVC_Common","dsltmp_autodet_state","s") %>';
		var wan_type = "<%tcWebApi_get("AutoPVC_Common","Detect_XDSL","s")%>";

		if ((detect_status == "initializing") || (detect_status == "wait_for_init") ||
			(detect_status == "up") || (detect_status == "down") || (detect_status == "")) {
			if(detect_status == "up" && wan_type == "PTM"){
				document.form.next_page.value = "QIS_PTM_manual_setting.asp";
			}
			else{
				document.form.next_page.value = "QIS_detect.asp";
			}
		}
		else if ((detect_status == "pppoe") || (detect_status == "pppoa")) { //1:PPPoE, 2:PPPoA
			document.form.next_page.value = "QIS_ppp_cfg.asp";
		}
		else if (detect_status == "dhcp") { //2:MER/Automatic IP
			document.form.next_page.value = "QIS_mer_cfg.asp";
		}
		else if (detect_status == "Fail") {
			if(wan_type == "ATM")
				document.form.next_page.value = "QIS_manual_setting.asp";
			else
				document.form.next_page.value = "QIS_PTM_manual_setting.asp";
		}
		else
			document.form.next_page.value = "QIS_detect.asp";
	}	
}

function submitForm(){

       if(document.form.uiViewUsername.value.length == 0){
                showtext($("alert_msg1"), "<% tcWebApi_get("String_Entry","JS_Shareblanktest","s") %>");
		$("alert_msg1").style.display = "";
                document.form.uiViewUsername.focus();
                document.form.uiViewUsername.select();
                return false;
        }
        else{
                var alert_str = validate_hostname(document.form.uiViewUsername);
                if(alert_str != ""){
                        showtext($("alert_msg1"), alert_str);
                        $("alert_msg1").style.display = "";
                        document.form.uiViewUsername.focus();
                        document.form.uiViewUsername.select();                        
                        return false;
                }else{
                        $("alert_msg1").style.display = "none";
                }

                document.form.uiViewUsername.value = trim(document.form.uiViewUsername.value);

		if(document.form.uiViewUsername.value == "root"
                                || document.form.uiViewUsername.value == "guest"
                                || document.form.uiViewUsername.value == "anonymous"
                ){
                                showtext($("alert_msg1"), "<% tcWebApi_get("String_Entry","USB_App_account_alert","s") %>");
                                $("alert_msg1").style.display = "";
                                document.form.uiViewUsername.focus();
                                document.form.uiViewUsername.select();
                                return false;
                }
                else{
                                $("alert_msg1").style.display = "none";
                }
	}


	if(document.form.uiViewPassword.value == "" || document.form.uiViewPasswordConfirm.value == ""){
		alert("<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s") %>");
		document.form.uiViewPassword.focus();
		document.form.uiViewPassword.select();
		return false;
	}

	if(document.form.uiViewPassword.value != document.form.uiViewPasswordConfirm.value){
		alert("<% tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s") %>");
		document.form.uiViewPassword.focus();
		document.form.uiViewPassword.select();
		return false;
	}

	if(!validate_string_webpassword(document.form.uiViewPassword)){
		document.form.uiViewPassword.focus();
		document.form.uiViewPassword.select();
		return false;
	}

	document.form.submit();
}

function gotoPrev(){
	document.location.href = "/cgi-bin/qis/QIS_detect.asp";
}

/* password item show or not */
function pass_checked(obj){
	switchType(obj, document.form.show_pass_1.checked, true);
}

function clean_scorebar(obj){
	if(obj.value == "")
		document.getElementById("scorebarBorder").style.display = "none";
}
</script>
</head>
<body onLoad="QKinternettype_load_body();" onunload="">
<form method="post" name="form" action="QIS_process.asp">
<input type="hidden" name="current_page" value="/cgi-bin/qis/QIS_admin_pass.asp">
<input type="hidden" name="next_page" value="QIS_detect.asp">
<div class="QISmain">
	<div>
		<table width="730px">
			<tr>
				<td align="left">
					<span class="description_down"><%tcWebApi_get("String_Entry","QIS_pass_title","s")%></span>
				</td>
				<td align="right">&nbsp;
				</td>
			</tr>
		</table>
	</div>
	<div style="margin:5px;"><img style="width: 720px; *width: 710px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
<br/>

<div style="margin-left:30px;">
	<span><%tcWebApi_get("String_Entry","QIS_pass_desc1","s")%></span><br><br>
	<table id="tblsetting_1" class="QISform" width="400" border=0 align="center" cellpadding="5" cellspacing="0">
		<tr>
		  <th width="200"><%tcWebApi_get("String_Entry","Router_Login_Name","s")%></th>
		  <td class="QISformtd">
		  		<input type="text" name="uiViewUsername" maxlength="20"  tabindex="1" value="<% tcWebApi_Get("Account_Entry0","username","s") %>" onKeyPress="return is_string(this, event);"" class="input_15_table" autocapitalization="off" autocomplete="off">
				<br/><span id="alert_msg1" style="color:#FFCC00;"></span>
		  </td>
		</tr>
		<tr>
		  <th width="200"><a class="hintstyle" href="javascript:void(0);" onClick="openHint(11,4)"><%tcWebApi_get("String_Entry","PASS_new","s")%></a></th>
		  <td class="QISformtd">
			<input id="uiViewPassword" type="password"  tabindex="2" autocapitalization="off" autocomplete="off" value="" name="uiViewPassword" style="height:25px;" class="input_15_table" maxlength="16" onpaste="return false;" onkeyup="chkPass(this.value, 'http_passwd');" onBlur="clean_scorebar(this);"/>
				&nbsp;&nbsp;
        <div id="scorebarBorder" style="margin-left:140px; margin-top:-26px; display:none;" title="<%tcWebApi_get("String_Entry","LHC_x_Password_itemSecur","s")%>">
        	<div id="score" name="score" style="margin-top:3px;"></div>
        	<div id="scorebar" name="scorebar">&nbsp;</div>        	
        </div>
		  </td>
		</tr>
		<tr>
		  <th width="200"><span class="hintstyle"><%tcWebApi_get("String_Entry","PASS_retype","s")%></span></th>
		  <td class="QISformtd">
			<input id="uiViewPasswordConfirm" type="password"  tabindex="3" autocapitalization="off" autocomplete="off" value="" name="uiViewPasswordConfirm" style="height:25px;" class="input_15_table" maxlength="16" onpaste="return false;"/>
		  	<div style="margin-top:1px;"><input type="checkbox" name="show_pass_1"  tabindex="4" onclick="pass_checked(document.form.uiViewPassword);pass_checked(document.form.uiViewPasswordConfirm);"><%tcWebApi_get("String_Entry","QIS_show_pass","s")%></div>
		  </td>
		</tr>
	</table>
	<br>
	<div class="QISTutorFont" style="margin-top:5px;"><%tcWebApi_get("String_Entry","QIS_pass_desc3","s")%><br><%tcWebApi_get("String_Entry","QIS_pass_desc2","s")%></div>
	<div style="margin-left:300px;margin-top:-120px;"><img width="300px" src="/images/qis/router_login.jpg"></span></div>
</div>
	<br/>
<div class="apply_gen">
	<!--<input type="button" value="<% tcWebApi_get("String_Entry","btn_pre","s") %>" onclick="gotoPrev();" class="button_gen">-->
	<input type="button" value="<% tcWebApi_get("String_Entry","btn_next","s") %>" tabindex="5" onClick="submitForm();" class="button_gen">
</div>
</div>
</form>
</body>
<!--QIS_admin_pass.asp-->
</html>
