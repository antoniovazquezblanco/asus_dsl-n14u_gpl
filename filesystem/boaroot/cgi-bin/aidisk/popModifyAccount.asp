<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<!--aidisk/popModifyAccount.asp-->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>Mod New Account</title>
<link rel="stylesheet" href="/form_style.css" type="text/css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>

<script type="text/javascript">
var selectedAccount = parent.getSelectedAccount();
function initial(){
	$("new_account").value = selectedAccount;
	showtext($("selected_account"), selectedAccount);
	clickevent();
}
function clickevent(){
	$("Submit").onclick = function(){
		if(validForm()){
			$("account").value = selectedAccount;
			parent.showLoading();
			document.modifyAccountForm.submit();
			parent.hidePop("apply");
		}
	};
}
function validForm(){

  showtext($("alert_msg2"), "");

	//account
	if($("new_account").value.length == 0){
			alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc1","s")%>");
			$("new_account").focus();
			return false;
	}
	else{
		var alert_str = validate_hostname($("new_account"));
                if(alert_str != ""){
                	alert(alert_str);
									$("new_account").focus();
									return false;
                }

               	$("new_account").value = trim($("new_account").value);
                                
                if($("new_account").value == "root"
                	|| $("new_account").value == "guest"
                        || $("new_account").value == "anonymous"
                ){
                		alert("<%tcWebApi_get("String_Entry","USB_App_account_alert","s")%>");
										$("new_account").focus();
										return false;
                }
                else if(checkDuplicateName($("new_account").value, parent.get_accounts()) &&
         					$("new_account").value != decodeURIComponent(selectedAccount)){                 
                        alert("<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc5","s")%>");
                        $("new_account").focus();
                        return false;
               	}


	}

        // password
        if($("password").value.length <= 0 || $("confirm_password").value.length <= 0){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc6","s")%>");
                if($("password").value.length <= 0){
                                $("password").focus();
                                $("password").select();
                }else{
                                $("confirm_password").focus();
                                $("confirm_password").select();
                }
                return false;
        }

        if($("password").value != $("confirm_password").value){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc7","s")%>");
                $("confirm_password").focus();
                return false;
        }

        if(!validate_string(document.createAccountForm.password)){
                $("password").focus();
                $("password").select();
                return false;
        }

        if($("password").value.length > 16){
                showtext($("alert_msg2"),"*<%tcWebApi_get("String_Entry","LHC_x_Password_id","s")%>");
                $("password").focus();
                $("password").select();
                return false;
        }

        var re = new RegExp("[^a-zA-Z0-9]+","gi");
        if(re.test($("password").value)){
                showtext($("alert_msg2"),"<%tcWebApi_get("String_Entry","File_Pop_content_alert_desc9","s")%>");
                $("password").focus();
                return false;
        }

	return true;
}
function checkDuplicateName(newname, teststr){
	var existing_string = teststr.join(',');
	existing_string = "," + existing_string + ",";
	var newstr = "," + trim(newname) + ",";
	var re = new RegExp(newstr,"gi")
	var matchArray = existing_string.match(re);
	if (matchArray != null)
		return true;
	else
		return false;
}
</script>
</head>
<body onLoad="initial();">
<form method="post" name="modifyAccountForm" action="modify_account.asp" target="hidden_frame">
<input name="account" id="account" type="hidden" value="">
<table width="90%" class="popTable" border="0" align="center" cellpadding="0" cellspacing="0">
	<thead>
		<tr>
		<td colspan="2"><span style="color:#FFF"><%tcWebApi_get("String_Entry","ModAccountTitle","s")%>: </span><span style="color:#FFF" id="selected_account"></span><img src="/images/button-close.gif" onClick="parent.hidePop('OverlayMask');"></td>
		</tr>
	</thead>
	<tbody>
		<tr valign="middle">
			<td height="30" colspan="2" class="hint_word"><%tcWebApi_get("String_Entry","ModAccountAlert","s")%></td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","AiDisk_Account","s")%>: </th>
			<td>
				<input class="input_15_table" name="new_account" id="new_account" type="text" maxlength="20">
			</td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","ModAccountPassword","s")%>: </th>
			<td><input class="input_15_table" name="new_password" id="new_password" type="password" maxlength="17"></td>
		</tr>
		<tr>
			<th><%tcWebApi_get("String_Entry","Confirmpassword","s")%>: </th>
			<td><input class="input_15_table" id="confirm_password" type="password" maxlength="17">
				<br/><span id="alert_msg2" style="color:#FC0;margin-left:8px;"></span>
			</td>
		</tr>
	</tbody>
	<tr bgcolor="#E6E6E6">
		<td colspan="2" align="right"><input id="Submit" type="button" class="button_gen" value="<%tcWebApi_get("String_Entry","CTL_modify","s")%>"></td>
	</tr>
</table>
</form>
</body>

<!--aidisk/popModifyAccount.asp-->
</html>

