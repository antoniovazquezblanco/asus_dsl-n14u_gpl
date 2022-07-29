<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_WAN_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - <%tcWebApi_get("String_Entry","menu5_3_1","s")%>n</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<style>
.apply_gen_wan{
text-align:center;
background-color:#4D595D;
width:99%;
margin-top:10px;
border-radius: 0 0 3px 3px;
-moz-border-radius-bottomright: 3px;
-moz-border-radius-bottomleft: 3px;
behavior: url(PIE.htc);
border-radius: 0 0 3px 3px;
}
</style>
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/detect.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
wan_proto = 'pppoe';
var wans_dualwan = 'dsl usb';
var nowWAN = '';
var wan_type_name = wans_dualwan.split(" ")[0].toUpperCase();
if(dualWAN_support != -1){
switch(wan_type_name){
case "DSL":
location.href = "Advanced_DSL_Content.asp";
break;
case "USB":
location.href = "Advanced_Modem_Content.asp";
break;
default:
break;
}
}


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }


var wireless = []; // [[MAC, associated, authorized], ...]
var original_wan_type = wan_proto;
var original_wan_dhcpenable = parseInt('');
var original_dnsenable = parseInt('');
function initial(){
show_menu();
change_wan_type(document.form.wan_proto.value, 0);
fixed_change_wan_type(document.form.wan_proto.value);
genWANSoption();
change_wan_type(document.form.wan_proto.value, 0);
if(document.form.wan_proto.value == "pppoe"
|| document.form.wan_proto.value == "pptp"
|| document.form.wan_proto.value == "l2tp"
){
document.form.wan_pppoe_username.value = decodeURIComponent(document.form.wan_pppoe_username_org.value);
document.form.wan_pppoe_passwd.value = decodeURIComponent(document.form.wan_pppoe_passwd_org.value);
}
}
function change_wan_unit(){
if(document.form.wan_unit.options[document.form.wan_unit.selectedIndex].text == "DSL")
document.form.current_page.value = "Advanced_DSL_Content.asp";
else if(document.form.wan_unit.options[document.form.wan_unit.selectedIndex].text == "USB")
document.form.current_page.value = "Advanced_Modem_Content.asp";
else
return false;
FormActions("apply.asp", "change_wan_unit", "", "");
document.form.target = "";
document.form.submit();
}
function genWANSoption(){
for(i=0; i<wans_dualwan.split(" ").length; i++)
document.form.wan_unit.options[i] = new Option(wans_dualwan.split(" ")[i].toUpperCase(), i);
document.form.wan_unit.selectedIndex = '0';
if(wans_dualwan.search(" ") < 0 || wans_dualwan.split(" ")[1] == 'none' || dualWAN_support == -1)
$("WANscap").style.display = "none";
}
function applyRule(){
if(document.form.wan_dnsenable_x[1].checked == true && document.form.wan_proto.value != "dhcp" && document.form.wan_dns1_x.value == "" && document.form.wan_dns1_x.value == "")
alert("DNS server not set! Please setup the DNS server on the client device.");
if(validForm()){
showLoading();
inputCtrl(document.form.wan_dhcpenable_x[0], 1);
inputCtrl(document.form.wan_dhcpenable_x[1], 1);
if(!document.form.wan_dhcpenable_x[0].checked){
inputCtrl(document.form.wan_ipaddr_x, 1);
inputCtrl(document.form.wan_netmask_x, 1);
inputCtrl(document.form.wan_gateway_x, 1);
}
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
if(!document.form.wan_dnsenable_x[0].checked){
inputCtrl(document.form.wan_dns1_x, 1);
inputCtrl(document.form.wan_dns2_x, 1);
}
document.form.submit();
}
}
function valid_IP(obj_name, obj_flag){
var A_class_start = inet_network("1.0.0.0");
var A_class_end = inet_network("126.255.255.255");
var B_class_start = inet_network("127.0.0.0");
var B_class_end = inet_network("127.255.255.255");
var C_class_start = inet_network("128.0.0.0");
var C_class_end = inet_network("255.255.255.255");
var ip_obj = obj_name;
var ip_num = inet_network(ip_obj.value);
if(obj_flag == "DNS" && ip_num == -1){ //DNS allows to input nothing
return true;
}
if(obj_flag == "GW" && ip_num == -1){ //GW allows to input nothing
return true;
}
if(ip_num > A_class_start && ip_num < A_class_end)
return true;
else if(ip_num > B_class_start && ip_num < B_class_end){
			alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
ip_obj.focus();
ip_obj.select();
return false;
}
else if(ip_num > C_class_start && ip_num < C_class_end)
return true;
else{
			alert(ip_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
ip_obj.focus();
ip_obj.select();
return false;
}
}
function validForm(){
if(!document.form.wan_dhcpenable_x[0].checked){// Set IP address by userself
if(!valid_IP(document.form.wan_ipaddr_x, "")) return false; //WAN IP
if(!valid_IP(document.form.wan_gateway_x, "GW"))return false; //Gateway IP
if(document.form.wan_gateway_x.value == document.form.wan_ipaddr_x.value){
			alert("<%tcWebApi_get("String_Entry","IPC_warning_WANIPEQUALGatewayIP","s")%>");
return false;
}
var default_netmask = "";
var wrong_netmask = 0;
var netmask_obj = document.form.wan_netmask_x;
var netmask_num = inet_network(netmask_obj.value);
if(netmask_num==0){
var netmask_reverse_num = 0; //Viz 2011.07 : Let netmask 0.0.0.0 pass
}else{
var netmask_reverse_num = ~netmask_num;
}
if(netmask_num < 0) wrong_netmask = 1;
var test_num = netmask_reverse_num;
while(test_num != 0){
if((test_num+1)%2 == 0)
test_num = (test_num+1)/2-1;
else{
wrong_netmask = 1;
break;
}
}
if(wrong_netmask == 1){
			alert(netmask_obj.value+" <%tcWebApi_get("String_Entry","JS_validip","s")%>");
netmask_obj.value = default_netmask;
netmask_obj.focus();
netmask_obj.select();
return false;
}
}
if(!document.form.wan_dnsenable_x[0].checked){
if(!valid_IP(document.form.wan_dns1_x, "DNS")) return false; //DNS1
if(!valid_IP(document.form.wan_dns2_x, "DNS")) return false; //DNS2
}
if(document.form.wan_proto.value == "pppoe"
|| document.form.wan_proto.value == "pptp"
|| document.form.wan_proto.value == "l2tp"
){
if(!validate_string(document.form.wan_pppoe_username)
|| !validate_string(document.form.wan_pppoe_passwd)
)
return false;
if(!validate_number_range(document.form.wan_pppoe_idletime, 0, 4294967295))
return false;
}
if(document.form.wan_proto.value == "pppoe"){
if(!validate_number_range(document.form.wan_pppoe_mtu, 576, 1492)
|| !validate_number_range(document.form.wan_pppoe_mru, 576, 1492))
return false;
if(!validate_string(document.form.wan_pppoe_service)
|| !validate_string(document.form.wan_pppoe_ac))
return false;
}
if(document.form.wan_hostname.value.length > 0)
if(!validate_string(document.form.wan_hostname))
return false;
if(document.form.wan_hwaddr_x.value.length > 0)
if(!check_hwaddr(document.form.wan_hwaddr_x))
return false;
if(document.form.wan_heartbeat_x.value.length > 0)
if(!validate_string(document.form.wan_heartbeat_x))
return false;
return true;
}
function done_validating(action){
refreshpage();
}
function change_wan_type(wan_type, flag){
if(typeof(flag) != "undefined")
change_wan_dhcp_enable(flag);
else
change_wan_dhcp_enable(1);
if(wan_type == "pppoe"){
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
inputCtrl(document.form.wan_auth_x, 0);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 1);
inputCtrl(document.form.wan_pppoe_idletime_check, 1);
inputCtrl(document.form.wan_pppoe_mtu, 1);
inputCtrl(document.form.wan_pppoe_mru, 1);
inputCtrl(document.form.wan_pppoe_service, 1);
inputCtrl(document.form.wan_pppoe_ac, 1);
inputCtrl(document.form.wan_pppoe_options_x, 1);
inputCtrl(document.form.wan_pptp_options_x, 0);
$("vpn_server").style.display = "none";
}
else if(wan_type == "pptp"){
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
inputCtrl(document.form.wan_auth_x, 0);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
inputCtrl(document.form.wan_pppoe_options_x, 1);
inputCtrl(document.form.wan_pptp_options_x, 1);
$("vpn_server").style.display = "";
}
else if(wan_type == "l2tp"){
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
inputCtrl(document.form.wan_auth_x, 0);
inputCtrl(document.form.wan_pppoe_username, 1);
inputCtrl(document.form.wan_pppoe_passwd, 1);
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
inputCtrl(document.form.wan_pppoe_options_x, 1);
inputCtrl(document.form.wan_pptp_options_x, 0);
$("vpn_server").style.display = "";
}
else if(wan_type == "static"){
inputCtrl(document.form.wan_dnsenable_x[0], 0);
inputCtrl(document.form.wan_dnsenable_x[1], 0);
inputCtrl(document.form.wan_auth_x, 1);
inputCtrl(document.form.wan_pppoe_username, (document.form.wan_auth_x.value != ""));
inputCtrl(document.form.wan_pppoe_passwd, (document.form.wan_auth_x.value != ""));
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
inputCtrl(document.form.wan_pppoe_options_x, 0);
inputCtrl(document.form.wan_pptp_options_x, 0);
$("vpn_server").style.display = "none";
}
else{ // Automatic IP
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
inputCtrl(document.form.wan_auth_x, 1);
inputCtrl(document.form.wan_pppoe_username, (document.form.wan_auth_x.value != ""));
inputCtrl(document.form.wan_pppoe_passwd, (document.form.wan_auth_x.value != ""));
inputCtrl(document.form.wan_pppoe_idletime, 0);
inputCtrl(document.form.wan_pppoe_idletime_check, 0);
inputCtrl(document.form.wan_pppoe_mtu, 0);
inputCtrl(document.form.wan_pppoe_mru, 0);
inputCtrl(document.form.wan_pppoe_service, 0);
inputCtrl(document.form.wan_pppoe_ac, 0);
inputCtrl(document.form.wan_pppoe_options_x, 0);
inputCtrl(document.form.wan_pptp_options_x, 0);
$("vpn_server").style.display = "none";
}
}
function fixed_change_wan_type(wan_type){
var flag = false;
if(!document.form.wan_dhcpenable_x[0].checked){
if(document.form.wan_ipaddr_x.value.length == 0)
document.form.wan_ipaddr_x.focus();
else if(document.form.wan_netmask_x.value.length == 0)
document.form.wan_netmask_x.focus();
else if(document.form.wan_gateway_x.value.length == 0)
document.form.wan_gateway_x.focus();
else
flag = true;
}else
flag = true;
if(wan_type == "pppoe"){
if(wan_type == original_wan_type){
document.form.wan_dnsenable_x[0].checked = original_dnsenable;
document.form.wan_dnsenable_x[1].checked = !original_dnsenable;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', original_dnsenable);
/* Viz 2011.11 if(flag == true && document.form.wan_dns1_x.value.length == 0){
document.form.wan_dns1_x.focus();
} */
}
else{
document.form.wan_dnsenable_x[0].checked = 1;
document.form.wan_dnsenable_x[1].checked = 0;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', 0);
inputCtrl(document.form.wan_dns1_x, 0);
inputCtrl(document.form.wan_dns2_x, 0);
}
}else if(wan_type == "pptp" || wan_type == "l2tp"){
if(wan_type == original_wan_type){
document.form.wan_dnsenable_x[0].checked = original_dnsenable;
document.form.wan_dnsenable_x[1].checked = !original_dnsenable;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', original_dnsenable);
/* Viz 2011.11 if(flag == true && document.form.wan_dns1_x.value.length == 0)
document.form.wan_dns1_x.focus();*/
}
else{
document.form.wan_dnsenable_x[0].checked = 0;
document.form.wan_dnsenable_x[1].checked = 1;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', 0);
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
}
}
else if(wan_type == "static"){
document.form.wan_dnsenable_x[0].checked = 0;
document.form.wan_dnsenable_x[1].checked = 1;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', 0);
/* Viz 2011.11 if(flag == true && document.form.wan_dns1_x.value.length == 0)
document.form.wan_dns1_x.focus();
*/
}
else{ // wan_type == "dhcp"
if(wan_type == original_wan_type){
document.form.wan_dnsenable_x[0].checked = original_dnsenable;
document.form.wan_dnsenable_x[1].checked = !original_dnsenable;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', original_dnsenable);
}
else{
document.form.wan_dnsenable_x[0].checked = 1;
document.form.wan_dnsenable_x[1].checked = 0;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', 0);
inputCtrl(document.form.wan_dns1_x, 0);
inputCtrl(document.form.wan_dns2_x, 0);
}
}
}
function change_wan_dhcp_enable(flag){
var wan_type = document.form.wan_proto.value;
if(wan_type == "pppoe"){
if(flag == 1){
if(wan_type == original_wan_type){
document.form.wan_dhcpenable_x[0].checked = original_wan_dhcpenable;
document.form.wan_dhcpenable_x[1].checked = !original_wan_dhcpenable;
}
else{
document.form.wan_dhcpenable_x[0].checked = 1;
document.form.wan_dhcpenable_x[1].checked = 0;
}
}
inputCtrl(document.form.wan_dhcpenable_x[0], 1);
inputCtrl(document.form.wan_dhcpenable_x[1], 1);
var wan_dhcpenable = document.form.wan_dhcpenable_x[0].checked;
inputCtrl(document.form.wan_ipaddr_x, !wan_dhcpenable);
inputCtrl(document.form.wan_netmask_x, !wan_dhcpenable);
inputCtrl(document.form.wan_gateway_x, !wan_dhcpenable);
}
else if(wan_type == "pptp"
|| wan_type == "l2tp"
){
if(flag == 1){
if(wan_type == original_wan_type){
document.form.wan_dhcpenable_x[0].checked = original_wan_dhcpenable;
document.form.wan_dhcpenable_x[1].checked = !original_wan_dhcpenable;
}
else{
document.form.wan_dhcpenable_x[0].checked = 0;
document.form.wan_dhcpenable_x[1].checked = 1;
}
}
inputCtrl(document.form.wan_dhcpenable_x[0], 1);
inputCtrl(document.form.wan_dhcpenable_x[1], 1);
var wan_dhcpenable = document.form.wan_dhcpenable_x[0].checked;
inputCtrl(document.form.wan_ipaddr_x, !wan_dhcpenable);
inputCtrl(document.form.wan_netmask_x, !wan_dhcpenable);
inputCtrl(document.form.wan_gateway_x, !wan_dhcpenable);
}
else if(wan_type == "static"){
document.form.wan_dhcpenable_x[0].checked = 0;
document.form.wan_dhcpenable_x[1].checked = 1;
inputCtrl(document.form.wan_dhcpenable_x[0], 0);
inputCtrl(document.form.wan_dhcpenable_x[1], 0);
inputCtrl(document.form.wan_ipaddr_x, 1);
inputCtrl(document.form.wan_netmask_x, 1);
inputCtrl(document.form.wan_gateway_x, 1);
}
else{ // wan_type == "dhcp"
document.form.wan_dhcpenable_x[0].checked = 1;
document.form.wan_dhcpenable_x[1].checked = 0;
inputCtrl(document.form.wan_dhcpenable_x[0], 0);
inputCtrl(document.form.wan_dhcpenable_x[1], 0);
inputCtrl(document.form.wan_ipaddr_x, 0);
inputCtrl(document.form.wan_netmask_x, 0);
inputCtrl(document.form.wan_gateway_x, 0);
}
if(document.form.wan_dhcpenable_x[0].checked){
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
}
else{ //wan_dhcpenable_x NO
document.form.wan_dnsenable_x[0].checked = 0;
document.form.wan_dnsenable_x[1].checked = 1;
change_common_radio(document.form.wan_dnsenable_x, 'IPConnection', 'wan_dnsenable_x', 0);
inputCtrl(document.form.wan_dnsenable_x[0], 1);
inputCtrl(document.form.wan_dnsenable_x[1], 1);
}
}
function showMAC(){
var tempMAC = "";
document.form.wan_hwaddr_x.value = login_mac_str();
}
function check_macaddr(obj,flag){ //control hint of input mac address
if (flag){
$("check_mac") ? $("check_mac").style.display="none" : true;
}
else{
var childsel=document.createElement("div");
childsel.setAttribute("id","check_mac");
childsel.style.color="#FFCC00";
obj.parentNode.appendChild(childsel);
$("check_mac").innerHTML="The format for the MAC address is six groups of two hexadecimal digits, separated by colons (:), in transmission order (e.g. 12:34:56:aa:bc:ef)";
}
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<script>
if(sw_mode == 3){
alert("<%tcWebApi_get("String_Entry","page_not_support_mode_hint","s")%>");
location.href = "index.asp";
}
</script>
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="productid" value="<% tcWebApi_staticGet("SysInfo_Entry","ProductName","s") %>">
<input type="hidden" name="support_cdma" value="">
<input type="hidden" name="current_page" value="Advanced_WAN_Content.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="group_id" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="restart_wan_if">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="lan_ipaddr" value="<%If tcWebApi_get("Lan_Entry","IP","h") <> "" then tcWebApi_get("Lan_Entry","IP","s") end if%>" />
<input type="hidden" name="lan_netmask" value="<%If tcWebApi_get("Lan_Entry","netmask","h") <> "" then tcWebApi_get("Lan_Entry","netmask","s") end if%>" />
<input type="hidden" name="wan_pppoe_username_org" value="aaaaaaaa" />
<input type="hidden" name="wan_pppoe_passwd_org" value="bbbbbbbb" />
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td height="430" valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td align="left" valign="top">
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D">
<div>&nbsp;</div>
		  			<div class="formfonttitle"><%tcWebApi_get("String_Entry","menu5_3","s")%> - <%tcWebApi_get("String_Entry","menu5_3_1","s")%></div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc" style="margin-bottom:0px;"><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_sd","s")%></div>
</td>
</tr>
<tr id="WANscap">
<td bgcolor="#4D595D" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
<td colspan="2"><%tcWebApi_get("String_Entry","wan_index","s")%></td>
</tr>
</thead>
<tr>
<th><%tcWebApi_get("String_Entry","wan_type","s")%></th>
<td align="left">
<select id="" class="input_option" name="wan_unit" onchange="change_wan_unit();"></select>
</td>
</tr>
</table>
</td>
</tr>
<tr id="t2BC">
<td bgcolor="#4D595D" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
							<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
								<th><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_in","s")%></th>
<td align="left">
<select id="wan_proto_menu" class="input_option" name="wan_proto" onchange="change_wan_type(this.value);fixed_change_wan_type(this.value);">
<option value="dhcp" >Automatic IP</option>
<option value="pppoe" selected>PPPoE</option>
<option value="pptp" >PPTP</option>
<option value="l2tp" >L2TP</option>
<option value="static" >Static IP</option>
</select>
</td>
</tr>
<tr>
								<th><%tcWebApi_get("String_Entry","Enable_WAN","s")%></th>                 
<td>
<input type="radio" name="wan_enable" class="input" value="1" checked><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_enable" class="input" value="0" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,22);"><%tcWebApi_get("String_Entry","Enable_NAT","s")%></a></th>
<td>
<input type="radio" name="wan_nat_x" class="input" value="1" checked><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_nat_x" class="input" value="0" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
								<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,23);"><%tcWebApi_get("String_Entry","BC_EnableMediaServer_in","s")%></a></th>                 
<td>
<input type="radio" name="wan_upnp_enable" class="input" value="1" onclick="return change_common_radio(this, 'LANHostConfig', 'wan_upnp_enable', '1')" checked><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_upnp_enable" class="input" value="0" onclick="return change_common_radio(this, 'LANHostConfig', 'wan_upnp_enable', '0')" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
</table>
</td>
</tr>
<tr id="IPsetting">
<td bgcolor="#4D595D" id="ip_sect" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
								<td colspan="2"><%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_sn","s")%></td>
</tr>
</thead>
<tr>
								<th><%tcWebApi_get("String_Entry","L3F_x_DHCPClient_in","s")%></th>
<td>
<input type="radio" name="wan_dhcpenable_x" class="input" value="1" onclick="change_wan_dhcp_enable(0);" ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_dhcpenable_x" class="input" value="0" onclick="change_wan_dhcp_enable(0);" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,1);"><%tcWebApi_get("String_Entry","IPC_ExternalIPAddress_in","s")%></a></th>
<td><input type="text" name="wan_ipaddr_x" maxlength="15" class="input_15_table" value="" onKeyPress="return is_ipaddr(this, event);" ></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,2);"><%tcWebApi_get("String_Entry","IPC_x_ExternalSubnetMask_in","s")%></a></th>
<td><input type="text" name="wan_netmask_x" maxlength="15" class="input_15_table" value="" onKeyPress="return is_ipaddr(this, event);" ></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,3);"><%tcWebApi_get("String_Entry","IPC_x_ExternalGateway_in","s")%></a></th>
<td><input type="text" name="wan_gateway_x" maxlength="15" class="input_15_table" value="" onKeyPress="return is_ipaddr(this, event);" ></td>
</tr>
</table>
</td>
</tr>
<tr id="DNSsetting">
<td bgcolor="#4D595D" id="dns_sect" valign="top">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
              <td colspan="2"><%tcWebApi_get("String_Entry","IPC_x_DNSServerEnable_sn","s")%></td>
</tr>
</thead>
<tr>
            		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,12);"><%tcWebApi_get("String_Entry","IPC_x_DNSServerEnable_in","s")%></a></th>
<td>
<input type="radio" name="wan_dnsenable_x" class="input" value="1" onclick="return change_common_radio(this, 'IPConnection', 'wan_dnsenable_x', 1)"  /><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_dnsenable_x" class="input" value="0" onclick="return change_common_radio(this, 'IPConnection', 'wan_dnsenable_x', 0)"  /><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,13);"><%tcWebApi_get("String_Entry","IPC_x_DNSServer1_in","s")%></a></th>
<td><input type="text" maxlength="15" class="input_15_table" name="wan_dns1_x" value="" onkeypress="return is_ipaddr(this, event)" ></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,14);"><%tcWebApi_get("String_Entry","IPC_x_DNSServer2_in","s")%></a></th>
<td><input type="text" maxlength="15" class="input_15_table" name="wan_dns2_x" value="" onkeypress="return is_ipaddr(this, event)" ></td>
</tr>
</table>
</td>
</tr>
<tr id="PPPsetting">
<td bgcolor="#4D595D" id="account_sect">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
              	<td colspan="2"><%tcWebApi_get("String_Entry","PPPC_UserName_sn","s")%></td>
</tr>
</thead>
<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,29);"><%tcWebApi_get("String_Entry","PPPC_Authentication_in","s")%></a></th>
<td align="left">
<select class="input_option" name="wan_auth_x" onChange="change_wan_type(document.form.wan_proto.value);">
<option value="" selected><%tcWebApi_get("String_Entry","checkbox_No","s")%>ne</option>
<option value="8021x-md5" >802.1x MD5</option>
</select></td>
</tr>
<tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,4);"><%tcWebApi_get("String_Entry","PPPC_UserName_in","s")%></a></th>
<td><input type="text" maxlength="64" class="input_32_table" name="wan_pppoe_username" value="aaaaaaaa" onkeypress="return is_string(this, event)" onblur=""></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,5);"><%tcWebApi_get("String_Entry","PPPC_Password_in","s")%></a></th>
<td><input type="password" maxlength="64" class="input_32_table" name="wan_pppoe_passwd" value="bbbbbbbb"></td>
</tr>
<tr style="display:none">
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,6);"><%tcWebApi_get("String_Entry","PPPC_IdleDisconnectTime_in","s")%></a></th>
<td>
<input type="text" maxlength="10" class="input_12_table" name="wan_pppoe_idletime" value="0" onkeypress="return is_number(this,event)" />
<input type="checkbox" style="margin-left:30;display:none;" name="wan_pppoe_idletime_check" value="" onclick="return change_common_radio(this, 'PPPConnection', 'wan_pppoe_idletime', '1')" />
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,7);"><%tcWebApi_get("String_Entry","PPPC_x_PPPoEMTU_in","s")%></a></th>
<td><input type="text" maxlength="5" name="wan_pppoe_mtu" class="input_6_table" value="1492" onKeyPress="return is_number(this,event);"/></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,8);"><%tcWebApi_get("String_Entry","PPPC_x_PPPoEMRU_in","s")%></a></th>
<td><input type="text" maxlength="5" name="wan_pppoe_mru" class="input_6_table" value="1492" onKeyPress="return is_number(this,event);"/></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,9);"><%tcWebApi_get("String_Entry","PPPC_x_ServiceName_in","s")%></a></th>
<td><input type="text" maxlength="32" class="input_32_table" name="wan_pppoe_service" value="" onkeypress="return is_string(this, event)" onblur=""/></td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,10);"><%tcWebApi_get("String_Entry","PPPC_x_AccessConcentrator_in","s")%></a></th>
<td><input type="text" maxlength="32" class="input_32_table" name="wan_pppoe_ac" value="" onkeypress="return is_string(this, event)" onblur=""/></td>
</tr>
<tr>
		<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,17);"><%tcWebApi_get("String_Entry","PPPC_x_PPTPOptions_in","s")%></a></th>
<td>
<select name="wan_pptp_options_x" class="input_option">
<option value="" selected>auto</option>
<option value="-mppc" ><%tcWebApi_get("String_Entry","checkbox_No","s")%> Encryption</option>
<option value="+mppe-40" >MPPE 40</option>
<option value="+mppe-128" >MPPE 128</option>
</select>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,18);"><%tcWebApi_get("String_Entry","PPPC_x_AdditionalOptions_in","s")%></a></th>
<td><input type="text" name="wan_pppoe_options_x" value="" class="input_32_table" maxlength="255" onKeyPress="return is_string(this, event)" onBlur="validate_string(this)"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td bgcolor="#4D595D">
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
            	<td colspan="2"><%tcWebApi_get("String_Entry","PPPC_x_HostNameForISP_sn","s")%></td>
</tr>
</thead>
<tr id="vpn_server">
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,19);"><%tcWebApi_get("String_Entry","BOP_isp_heart_item","s")%></a></th>
<td>
<input type="text" name="wan_heartbeat_x" class="input_32_table" maxlength="256" value="" onKeyPress="return is_string(this, event)"></td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> VPN+DHCP Connection?</th>
<td>
<input type="radio" name="wan_vpndhcp" class="input" value="1" onclick="return change_common_radio(this, 'IPConnection', 'wan_vpndhcp', 1)" checked /><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" name="wan_vpndhcp" class="input" value="0" onclick="return change_common_radio(this, 'IPConnection', 'wan_vpndhcp', 0)"  /><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,15);"><%tcWebApi_get("String_Entry","PPPC_x_HostNameForISP_in","s")%></a></th>
<td><input type="text" name="wan_hostname" class="input_32_table" maxlength="32" value="" onkeypress="return is_string(this, event)"></td>
</tr>
<tr>
<th ><a class="hintstyle" href="javascript:void(0);" onClick="openHint(7,16);"><%tcWebApi_get("String_Entry","PPPC_x_MacAddressForISP_in","s")%></a></th>
<td>
<input type="text" name="wan_hwaddr_x" class="input_20_table" maxlength="17" value="" onKeyPress="return is_hwaddr(this,event)" onblur="check_macaddr(this,check_hwaddr_temp(this))">
<input type="button" class="button_gen" onclick="showMAC();" value="MAC Clone">
</td>
</tr>
</table>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule();" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</td>
</tr>
</tbody>
</table>
</td>
</form>
</tr>
</table>
<!--div class="apply_gen_wan">
<input class="button_gen" onclick="applyRule();" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div-->
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</body>

<!--Advanced_WAN_Content.asp-->
</html>

