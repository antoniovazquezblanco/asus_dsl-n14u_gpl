<%
/*add a rule*/
if request_Form("ACLActionFlag") = "0" then

	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")
	
	if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_set("WebCurSet_Entry","acl_id","RuleIndexSEL")
		/*remove original data*/
		TCWebAPI_unset("ACL_Entry")
		
		TCWebAPI_set("ACL_Entry","Activate","RuleActiveRDO")
		TCWebAPI_set("ACL_Entry","ScrIPAddrBegin","ScrIPaddrBeginTXT")
		TCWebAPI_set("ACL_Entry","ScrIPAddrEnd","ScrIPaddrEndTXT")
		TCWebAPI_set("ACL_Entry","Application","ApplicationSEL")
		TCWebAPI_set("ACL_Entry","Interface","InterfaceSEL")
	end if
	
	/*commit changes*/
	TCWebAPI_commit("ACL_Entry")

/*select a rule*/	
elseif Request_Form("ACLActionFlag") = "1" then
	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")
	TCWebAPI_set("WebCurSet_Entry","acl_id","RuleIndexSEL")

/*remove a rule*/
elseif Request_Form("ACLActionFlag") = "2" then

	TCWebAPI_set("ACL_Common","Activate","ActivateRDO")

	if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then
		TCWebAPI_unset("ACL_Entry")
	end if

	TCWebAPI_commit("ACL_Entry")
end if
%>

<html>
<head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=<%tcWebApi_get("String_Entry","textTTNETCharSet","s")%>">
<link rel="stylesheet" type="text/css" href="/style.css">
</head>

<!--<script language="javascript" src="/tc2func.js"></script>-->
<script language="JavaScript">
var str_langacl = [
				["yes", "<%tcWebApi_get("String_Entry", "ACLYesText", "s")%>"],
				["no", "<%tcWebApi_get("String_Entry", "ACLNoText", "s")%>"],
				["Web", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText0", "s")%>"],
				["FTP", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText1", "s")%>"],
				["Telnet", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText2", "s")%>"],
				["SSH", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText3", "s")%>"],
				["TFTP", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText4", "s")%>"],
				["SNMP", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText5", "s")%>"],
				["Ping", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText6", "s")%>"],
				["ALL", "<%tcWebApi_get("String_Entry", "ACLApplicationSelectText7", "s")%>"],
				["WAN", "<%tcWebApi_get("String_Entry", "ACLInterfaceSelectText0", "s")%>"],
				["LAN", "<%tcWebApi_get("String_Entry", "ACLInterfaceSelectText1", "s")%>"],
				["Both", "<%tcWebApi_get("String_Entry", "ACLInterfaceSelectText2", "s")%>"]
			];
var strobj = {};
for (var i=0; i<str_langacl.length; i++)
	strobj[str_langacl[i][0]] = str_langacl[i][1];
var acl_info = [
				["<%tcWebApi_get("ACL_Entry0", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry0", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry0", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry0", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry0", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry1", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry1", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry1", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry1", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry1", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry2", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry2", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry2", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry2", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry2", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry3", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry3", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry3", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry3", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry3", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry4", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry4", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry4", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry4", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry4", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry5", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry5", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry5", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry5", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry5", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry6", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry6", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry6", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry6", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry6", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry7", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry7", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry7", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry7", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry7", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry8", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry8", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry8", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry8", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry8", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry9", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry9", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry9", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry9", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry9", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry10", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry10", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry10", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry10", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry10", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry11", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry11", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry11", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry11", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry11", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry12", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry12", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry12", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry12", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry12", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry13", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry13", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry13", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry13", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry13", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry14", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry14", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry14", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry14", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry14", "Interface", "s")%>"],
				["<%tcWebApi_get("ACL_Entry15", "Activate", "s")%>", "<%tcWebApi_get("ACL_Entry15", "ScrIPAddrBegin", "s")%>", "<%tcWebApi_get("ACL_Entry15", "ScrIPAddrEnd", "s")%>", "<%tcWebApi_get("ACL_Entry15", "Application", "s")%>", "<%tcWebApi_get("ACL_Entry15", "Interface", "s")%>"]
			];
function stAcllist(active, srcipst, srciped, app, interface)
{
	var i=2;
	this.active = active;
	if ("Yes" == active) 
		this.active = strobj["yes"];
	else if("No" == active) 
		this.active = strobj["no"];
	this.srcipst = srcipst;
	this.srciped = srciped;
	this.app = app;
	this.interface = interface;
	//app
	for (; i<str_langacl.length; i++)
	{
		if (app == str_langacl[i][0]) {
			app = str_langacl[i][1];
			break;
		}
	}
	for (; i<str_langacl.length; i++)
	{
		if (interface == str_langacl[i][0]) {
			interface = str_langacl[i][1];
			break;
		}
	}
	//interface	
}
var ary_acl = new Array(8);
for (var i=0; i<acl_info.length; i++)
{
	ary_acl[i] = new stAcllist(acl_info[i][0], acl_info[i][1], acl_info[i][2], acl_info[i][3], acl_info[i][4]);
}
function writeInfototable()
{
	var strtemp = "";
	for (var i=0; i<ary_acl.length; i++) {
		var n = i+1;
		if ("N/A" == ary_acl[i].active)
			continue;
		strtemp += '<tr><td align="center" class="tabdata">' + n + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_acl[i].active + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_acl[i].srcipst + '~' + ary_acl[i].srciped + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_acl[i].app + '</td>\n';
		strtemp += '<td align="center" class="tabdata">' + ary_acl[i].interface + '</td></tr>\n';
	}
	document.write(strtemp);
}


function init(){
	if(document.ACL_form.ActivateRDO[1].checked){
		aclSwitch(0);
		return;
	}
}

function doAdd() {
	document.forms[0].ACLActionFlag.value = 0;
}

function doIndexChange() {
	document.forms[0].ACLActionFlag.value = 1;
	document.forms[0].submit();
	return;
}

function doDel(){
	document.forms[0].ACLActionFlag.value = 2;
}              

function aclSwitch(on_off){
	var value;
	form = document.ACL_form;
	
	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.RuleIndexSEL.disabled = value;
	form.RuleActiveRDO[0].disabled = value;
	form.RuleActiveRDO[1].disabled = value;
	form.ScrIPaddrBeginTXT.disabled = value;
	form.ScrIPaddrEndTXT.disabled = value;
	form.ApplicationSEL.disabled = value;
	form.InterfaceSEL.disabled = value;
	
}

function aclRuleSwitch(on_off){
	var value;
	form = document.ACL_form;

	if(on_off == 0){
		value = true;
	}else{
		value = false;
	}
	form.ScrIPaddrBeginTXT.disabled = value;
	form.ScrIPaddrEndTXT.disabled = value;
	form.ApplicationSEL.disabled = value;
	form.InterfaceSEL.disabled = value;
}

function verifyForm(){
	form = document.ACL_form;
	var addrBegin = form.ScrIPaddrBeginTXT.value;
	var addrEnd = form.ScrIPaddrEndTXT.value;
	var hidden = form.ACLActionFlag.value;
	var ACL_active = form.ActivateRDO[1];
	
	if(hidden == 0 && ACL_active.checked == false)
	{
		if((addrBegin == "0.0.0.0")&&(addrEnd=="0.0.0.0")){
			return true;
		}else{
			if((!doValidateIP(addrBegin,0,1,0))||(!doValidateIP(addrEnd,0,1,0))){
				return false;
			}else{
				return doValidateRange(addrBegin, addrEnd);
			}
		}
	}
}

function doValidateIP(Address, option1, option2, Where) {

	var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	var digits;
	var i;

	if(((option1 == 1 || option1 == 4) && Address == "0.0.0.0") || (option1 == 2 && Address == "255.255.255.255"))
		return true;
		
	if(address == null)
	{
		if(option1 == 4 && Address == "")
			return true;
		if(Where == 1){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
			<!-- RpEnd -->   
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
			<!-- RpEnd -->
		}else {
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg0" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg0" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS2Text","s")%>");
		<!-- RpEnd -->
		}	    	        				         
		return false; 
	}
	
	digits = address[0].split(".");
	
	for(i=0; i < 4; i++)
	{
		if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (option1 != 4 && Number(digits[0]) > 223))
		{ 
			if(Where == 1){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
				alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
				<!-- RpEnd --> 
			}else if(Where == 2){
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
				alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
				<!-- RpEnd -->
			}else{	  
				<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
				alert("<%tcWebApi_get("String_Entry","ACLJS3Text","s")%>" + Address);
				<!-- RpEnd --> 
			}     	
			return false; 
		}
	}

	if((!isValidIpAddr(digits[0],digits[1],digits[2],digits[3])) || (option1 == 3 && Address == "1.0.0.0") || (option2 == 1 && digits[3] == 0))
	{
		if(Where == 1)
		{
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg4" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg4" RpTextType="ASCII" -->  	                     
			alert("<%tcWebApi_get("String_Entry","ACLJS0Text","s")%>"+Address);
			<!-- RpEnd --> 
		}else if(Where == 2){
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg76" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg76" RpTextType="ASCII" -->	  	  	         		       
			alert("<%tcWebApi_get("String_Entry","ACLJS1Text","s")%>");
			<!-- RpEnd -->
		}else{	    
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg1" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg1" RpTextType="ASCII" -->  	               
			alert("<%tcWebApi_get("String_Entry","ACLJS3Text","s")%>" + Address);
			<!-- RpEnd --> 
		}     	
		return false;
	}

	return true;
}

function doValidateRange(startIP, endIP) {
	var staddress;
	var edaddress;
	var cnt;

	staddress=startIP.split(".");
	edaddress=endIP.split(".");
	
	for(cnt=0; cnt < 4; cnt++)
	{
		if((cnt<3)&&( Number(edaddress[cnt])!= Number(staddress[cnt])))
		{
		<!-- RpNamedDisplayText Name="JavaScriptErrMsg88" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg88" RpTextType="ASCII" -->  	                       
		alert("<%tcWebApi_get("String_Entry","ACLJS4Text","s")%>");
		<!-- RpEnd -->
		return false;
		}
	
		if((cnt==3)&&( Number(edaddress[cnt]) < Number(staddress[cnt])))
		{
			<!-- RpNamedDisplayText Name="JavaScriptErrMsg7" RpGetType=Function RpGetPtr="RpGet_JavaScriptErrMsg7" RpTextType="ASCII" -->  	                       
			alert("<%tcWebApi_get("String_Entry","ACLJS5Text","s")%>");
			<!-- RpEnd -->      	            
			return false;
		}
	}

	return true;
}

function isValidIpAddr(ip1,ip2,ip3,ip4) {

	if(ip1==0 || ip4==255 || ip1==127 || ip4==0)
	return false;

	return true;
}

function doAclSummary()
{
	window.open("/cgi-bin/access_acllist.asp","ACL_List","toolbar=no,menubar=no,scrollbars=yes,height=300, width=520,location=0,left=100,top=100");
	return false;
}
</script>

<body onLoad="init()" >
<form METHOD="POST" action="/cgi-bin/access_acl.asp" name="ACL_form" onSubmit="return verifyForm();">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td height="5" class="light-orange" colspan="5">&nbsp;</td>
		</tr>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF"><%tcWebApi_get("String_Entry","ACLText","s")%></font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"></td>
			<td width="10" ></td>
			<td width="440"></td>
		</tr>
		<tr>
			<td height="25" class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLACLText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="ActivateRDO" VALUE="Yes"
				<%if TCWebAPI_get("ACL_Common", "Activate", "h") = "Yes" then asp_write("checked") end if %> onClick="aclSwitch(1);init()"><font color="#000000"><%tcWebApi_get("String_Entry","ACLActivatedText","s")%></font>
				<INPUT TYPE="RADIO" NAME="ActivateRDO" VALUE="No"
				<%if TCWebAPI_get("ACL_Common", "Activate", "h") = "No" then asp_write("checked") end if %> onClick="aclSwitch(0)"><font color="#000000"><%tcWebApi_get("String_Entry","ACLDeactivatedText","s")%></font>
			</td>
		</tr>
		<tr>
			<td height="25" class="title-sub"><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingText","s")%></font></td>
			<td class="light-orange"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
			<td class="tabdata"><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td width="150" height="25" class="light-orange">&nbsp;</td>
			<td width="10" class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingRuleIndexText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="RuleIndexSEL" SIZE="1" onChange="doIndexChange()">
					<OPTION VALUE="0" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "0" then asp_Write("selected") end if %>>1
					<OPTION VALUE="1" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "1" then asp_Write("selected") end if %>>2
					<OPTION VALUE="2" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "2" then asp_Write("selected") end if %>>3
					<OPTION VALUE="3" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "3" then asp_Write("selected") end if %>>4
					<OPTION VALUE="4" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "4" then asp_Write("selected") end if %>>5
					<OPTION VALUE="5" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "5" then asp_Write("selected") end if %>>6
					<OPTION VALUE="6" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "6" then asp_Write("selected") end if %>>7
					<OPTION VALUE="7" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "7" then asp_Write("selected") end if %>>8
					<OPTION VALUE="8" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "8" then asp_Write("selected") end if %>>9
					<OPTION VALUE="9" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "9" then asp_Write("selected") end if %>>10
					<OPTION VALUE="10" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "10" then asp_Write("selected") end if %>>11
					<OPTION VALUE="11" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "11" then asp_Write("selected") end if %>>12
					<OPTION VALUE="12" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "12" then asp_Write("selected") end if %>>13
					<OPTION VALUE="13" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "13" then asp_Write("selected") end if %>>14
					<OPTION VALUE="14" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "14" then asp_Write("selected") end if %>>15
					<OPTION VALUE="15" <%if TCWebAPI_get("WebCurSet_Entry", "acl_id", "h") = "15" then asp_Write("selected") end if %>>16
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingActiveText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="RADIO" NAME="RuleActiveRDO" VALUE="Yes" 
				<%if TCWebAPI_get("ACL_Entry", "Activate", "h") = "Yes" then asp_write("checked") end if %>><font color="#000000"><%tcWebApi_get("String_Entry","ACLYesText","s")%></font>
				<INPUT TYPE="RADIO" NAME="RuleActiveRDO" VALUE="No" 
				<%if TCWebAPI_get("ACL_Entry", "Activate", "h") = "No" then asp_write("checked") end if
				  if TCWebAPI_get("ACL_Entry", "Activate", "h") = "N/A" then asp_write("checked") end if  %>><font color="#000000"><%tcWebApi_get("String_Entry","ACLNoText","s")%></font>
			</td>
			</td>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLListSecureIPText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<INPUT TYPE="TEXT" NAME="ScrIPaddrBeginTXT" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("ACL_Entry", "ScrIPAddrBegin", "h") <> "N/A" then TCWebAPI_get("ACL_Entry", "ScrIPAddrBegin", "s") else asp_Write("0.0.0.0") end if %>" >~
				<INPUT TYPE="TEXT" NAME="ScrIPaddrEndTXT" SIZE="15" MAXLENGTH="15" VALUE=
				"<%if TCWebAPI_get("ACL_Entry", "ScrIPAddrEnd", "h") <> "N/A" then TCWebAPI_get("ACL_Entry", "ScrIPAddrEnd", "s") else asp_Write("0.0.0.0") end if %>" ><font color="#000000"><%tcWebApi_get("String_Entry","ACLEditingSrcIPDescripText","s")%></font>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLListAppText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="ApplicationSEL" SIZE="1">
					<OPTION value="Web" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Web" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText0","s")%>
					<OPTION value="FTP" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "FTP" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText1","s")%>
					<OPTION value="Telnet" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Telnet" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText2","s")%>
                                        <% if TCWebAPI_get("ACL_Common", "sshOption", "h") = "support" then %>
                                        <OPTION value="SSH" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "SSH" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText3","s")%>
                                        <% end if %>
					<OPTION value="TFTP" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "TFTP" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText4","s")%>
                                        <OPTION value="SNMP" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "SNMP" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText5","s")%>
					<OPTION value="Ping" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "Ping" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText6","s")%>
					<OPTION value="ALL" <%if TCWebAPI_get("ACL_Entry", "Application", "h") = "ALL" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLApplicationSelectText7","s")%>
				</SELECT>
			</td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000"><%tcWebApi_get("String_Entry","ACLListInterfaceText","s")%></font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
				<SELECT NAME="InterfaceSEL" SIZE="1">
					<OPTION value="WAN" <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "WAN" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLInterfaceSelectText0","s")%>
					<OPTION value="LAN" <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "LAN" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLInterfaceSelectText1","s")%>
					<OPTION value="Both" <%if TCWebAPI_get("ACL_Entry", "Interface", "h") = "Both" then asp_write("selected") end if %>><%tcWebApi_get("String_Entry","ACLInterfaceSelectText2","s")%>
				</SELECT>
			</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="150" class="title-sub"><font color="#000000"><%tcWebApi_get("String_Entry","ACLListText","s")%></font></td>
			<td width="10" class="light-orange"></td>
			<td width="10" ><hr noshade class="light-orange-line"></td>
			<td width="580" ><hr noshade class="light-orange-line"></td>
			<td width="10" ><hr noshade class="light-orange-line"></td>
		</tr>
		<tr>
			<td class="light-orange"></td>
			<td class="light-orange"></td>
			<td class="tabdata">&nbsp;</td>
      		<td class="tabdata">
				<table width="480" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC" bgcolor="#FFFFFF">
				<tr height="30">
					<td width="60"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ACLCgiIndexText", "s")%></strong></div></td>
					<td width="60"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ACLCgiActiveText", "s")%></strong></td>
					<td width="180"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ACLCgiSeIPAddrText", "s")%></strong></td>
					<td width="90"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ACLCgiApplicationText", "s")%></strong></td>
					<td width="80"  align="center" class="tabdata"><strong><%tcWebApi_get("String_Entry", "ACLCgiInterfaceText", "s")%></strong></td>
				</tr>
				<script language="JavaScript" type="text/JavaScript">
					writeInfototable();
				</script>
				</table>
			</td>
			<td width="10" ></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td class="tabdata">&nbsp;</td>
		</tr>
	</table>
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
		<tr>
			<td width="160" class="black" height="42" >&nbsp;</td>
			<td width="160" class="orange">&nbsp;</td>
			<td width="440" class="orange">
				<INPUT TYPE="HIDDEN" NAME="ACLActionFlag" VALUE="0">
				<INPUT TYPE="SUBMIT" NAME="ACLAddBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonSetText","s")%>" onClick="doAdd();">
				<INPUT TYPE="SUBMIT" NAME="ACLDelBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonDeleteText","s")%>" onClick="doDel();">
				<INPUT TYPE="RESET"  NAME="ACLCancelBTN" VALUE="<%tcWebApi_get("String_Entry","ButtonCancelText","s")%>" onClick="document.forms[0].ACLActionFlag.value = 3; javascript:window.location='access_acl.asp'">
			</td>
		</tr>
	</table>
</form>
</body>
</html>
