<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>

<!--Advanced_WANPort_Content.asp-->
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="/images/favicon.png">
<link rel="icon" href="/images/favicon.png">
<title>ASUS <%tcWebApi_get("String_Entry","Web_Title2","s")%> <% tcWebApi_staticGet("SysInfo_Entry","ProductTitle","s") %> - Dual WAN</title>
<link rel="stylesheet" type="text/css" href="/index_style.css">
<link rel="stylesheet" type="text/css" href="/form_style.css">
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/detect.js"></script>
<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>
wan_route_x = '';
wan_nat_x = '1';
var wans_caps = 'dsl usb lan';
var wans_dualwan_orig = 'dsl usb';
var wans_mode_orig = 'fo';
var wans_routing_isp_enable_orig = parseInt('');
var wans_routing_rulelist_array = '';
var wans_flag;
var switch_stb_x = '0';
var wans_caps_primary;
var wans_caps_secondary;
var $j = jQuery.noConflict();


function login_ip_str() { return '<% tcWebApi_get("WebCurSet_Entry","login_ip_tmp","s"); %>'; }

function login_mac_str() { return ''; }

var wireless = []; // [[MAC, associated, authorized], ...]
function initial(){
show_menu();
wans_flag = (wans_dualwan_orig.search("none") == -1) ? 1:0;
wans_caps_primary = wans_caps;
addWANOption(document.form.wans_primary, wans_caps_primary.split(" "));
document.form.wans_primary.value = wans_dualwan_orig.split(" ")[0];
appendLANoption1(document.form.wans_primary);
if(wans_dualwan_orig.search(" ") < 0 || wans_dualwan_orig.split(" ")[1] == "none"){ // single wan
inputCtrl(document.form.wans_second, 0);
inputCtrl(document.form.wans_mode, 0);
inputCtrl(document.form.wans_lb_ratio_0, 0);
inputCtrl(document.form.wans_lb_ratio_1, 0);
inputCtrl(document.form.wans_routing_isp_enable[1], 0);
inputCtrl(document.form.wans_routing_isp_enable[0], 0);
inputCtrl(document.form.wans_isp_unit, 0);
inputCtrl(document.form.isp_country, 0);
inputCtrl(document.form.isp_list, 0);
inputCtrl(document.form.wans_routing_enable[1], 0);
inputCtrl(document.form.wans_routing_enable[0], 0);
}
else{ // init dual WAN
wans_caps_secondary = wans_caps.split(" ").splice(1);
addWANOption(document.form.wans_second, wans_caps_secondary);
document.form.wans_second.value = wans_dualwan_orig.split(" ")[1];
appendLANoption2(document.form.wans_second);
}
appendModeOption(wans_mode_orig);
show_wans_rules();
}
function removeOptions(obj) {
if (obj == null) return;
if (obj.options == null) return;
obj.options.length = 0;
}
function applyRule(){
if(document.form.wans_primary.value == document.form.wans_second.value){
alert("It is not allowed to setup dual WAN with the same interface!");
document.form.wans_second.focus();
return;
}
if(wans_flag == 1){
document.form.wans_dualwan.value = document.form.wans_primary.value +" "+ document.form.wans_second.value;
}else{
document.form.wans_dualwan.value = document.form.wans_primary.value + " none";
}
if(document.form.wans_lanport1 && !document.form.wans_lanport2)
document.form.wans_lanport.value = document.form.wans_lanport1.value;
else if(!document.form.wans_lanport1 && document.form.wans_lanport2)
document.form.wans_lanport.value = document.form.wans_lanport2.value;
var tmp_pri_if = wans_dualwan_orig.split(" ")[0].toUpperCase();
var tmp_sec_if = wans_dualwan_orig.split(" ")[1].toUpperCase();
if (tmp_pri_if != 'LAN' || tmp_sec_if != 'LAN') {
var port_conflict = false;
var lan_port_num = document.form.wans_lanport.value;
if (switch_stb_x == '1') {
if (lan_port_num == '1') {
port_conflict = true;
}
}
else if (switch_stb_x == '2') {
if (lan_port_num == '2') {
port_conflict = true;
}
}
else if (switch_stb_x == '3') {
if (lan_port_num == '3') {
port_conflict = true;
}
}
else if (switch_stb_x == '4') {
if (lan_port_num == '4') {
port_conflict = true;
}
}
else if (switch_stb_x == '5') {
if (lan_port_num == '1' || lan_port_num == '2') {
port_conflict = true;
}
}
else if (switch_stb_x == '6') {
if (lan_port_num == '3' || lan_port_num == '4') {
port_conflict = true;
}
}
if (port_conflict) {
alert("IPTV port number is same as dual wan LAN port number");
return;
}
}
document.form.wans_lb_ratio.value = document.form.wans_lb_ratio_0.value + ":" + document.form.wans_lb_ratio_1.value;
if (document.form.wans_primary.value == "dsl") document.form.next_page = "Advanced_DSL_Content.asp";
if (document.form.wans_primary.value == "lan") document.form.next_page = "Advanced_WAN_Content.asp";
if (document.form.wans_primary.value == "usb") document.form.next_page = "Advanced_Modem_Content.asp";
if(document.form.wans_mode.value != "fo" && document.form.wans_routing_isp_enable[0].checked){
if(document.form.wans_isp_unit.value == 0){
document.form.wan0_routing_isp.value = country[document.form.isp_country.value]+"_"+country_n_isp[document.form.isp_country.value][document.form.isp_list.value];
}
if(document.form.wans_isp_unit.value == 1){
document.form.wan1_routing_isp.value = country[document.form.isp_country.value]+"_"+country_n_isp[document.form.isp_country.value][document.form.isp_list.value];
}
}
save_table();
showLoading();
document.form.submit();
}
function addWANOption(obj, wanscapItem){
free_options(obj);
for(i=0; i<wanscapItem.length; i++){
if(wanscapItem[i].length > 0){
obj.options[i] = new Option(wanscapItem[i].toUpperCase(), wanscapItem[i]);
}
}
if(wans_dualwan_orig.search(" ") < 0 || wans_dualwan_orig.split(" ")[1] == "none"){
appendLANoption1(document.form.wans_primary);
}else{
appendLANoption1(document.form.wans_primary);
appendLANoption2(document.form.wans_second);
}
}
function appendLANoption1(obj){
if(obj.value == "lan"){
if(!document.form.wans_lanport1){
var childsel=document.createElement("select");
childsel.setAttribute("id","wans_lanport1");
childsel.setAttribute("name","wans_lanport1");
childsel.setAttribute("class","input_option");
obj.parentNode.appendChild(childsel);
document.form.wans_lanport1.options[0] = new Option("LAN Port 1", "1");
document.form.wans_lanport1.options[1] = new Option("LAN Port 2", "2");
document.form.wans_lanport1.options[2] = new Option("LAN Port 3", "3");
document.form.wans_lanport1.options[3] = new Option("LAN Port 4", "4");
}else{
document.form.wans_lanport1.style.display = "";
}
if('1' == 0)
document.form.wans_lanport1.selectedIndex = 0;
else
document.form.wans_lanport1.selectedIndex = '1' - 1;
}
else if(document.form.wans_lanport1){
document.form.wans_lanport1.style.display = "none";
}
}
function appendLANoption2(obj){
if(obj.value == "lan"){
if(!document.form.wans_lanport2){
var childsel=document.createElement("select");
childsel.setAttribute("id","wans_lanport2");
childsel.setAttribute("name","wans_lanport2");
childsel.setAttribute("class","input_option");
obj.parentNode.appendChild(childsel);
document.form.wans_lanport2.options[0] = new Option("LAN Port 1", "1");
document.form.wans_lanport2.options[1] = new Option("LAN Port 2", "2");
document.form.wans_lanport2.options[2] = new Option("LAN Port 3", "3");
document.form.wans_lanport2.options[3] = new Option("LAN Port 4", "4");
}else{
document.form.wans_lanport2.style.display = "";
}
if('1' == 0)
document.form.wans_lanport2.selectedIndex = 0;
else
document.form.wans_lanport2.selectedIndex = '1' - 1;
}
else if(document.form.wans_lanport2){
document.form.wans_lanport2.style.display = "none";
}
}
function appendModeOption(v){
if(v == "fo"){
inputCtrl(document.form.wans_lb_ratio_0, 0);
inputCtrl(document.form.wans_lb_ratio_1, 0);
inputCtrl(document.form.wans_routing_isp_enable[1], 0);
inputCtrl(document.form.wans_routing_isp_enable[0], 0);
inputCtrl(document.form.wans_isp_unit, 0);
inputCtrl(document.form.isp_country, 0);
inputCtrl(document.form.wans_routing_enable[1], 0);
inputCtrl(document.form.wans_routing_enable[0], 0);
}else{
inputCtrl(document.form.wans_lb_ratio_0, 1);
inputCtrl(document.form.wans_lb_ratio_1, 1);
inputCtrl(document.form.wans_routing_isp_enable[1], 1);
inputCtrl(document.form.wans_routing_isp_enable[0], 1);
change_isp_radio(wans_routing_isp_enable_orig);
inputCtrl(document.form.wans_routing_enable[1], 1);
inputCtrl(document.form.wans_routing_enable[0], 1);
document.form.wans_lb_ratio_0.value = '3:1'.split(':')[0];
document.form.wans_lb_ratio_1.value = '3:1'.split(':')[1];
}
}
function change_isp_radio(v){
if(v == 1){
inputCtrl(document.form.wans_isp_unit, 1);
inputCtrl(document.form.isp_country, 1);
inputCtrl(document.form.isp_list, 1);
Load_ISP_country();
appendcoutry(document.form.isp_country.value);
}else{
inputCtrl(document.form.wans_isp_unit, 0);
inputCtrl(document.form.isp_country, 0);
inputCtrl(document.form.isp_list, 0);
}
}
function addRow_Group(upper){
var rule_num = $('wans_RoutingRules_table').rows.length;
var item_num = $('wans_RoutingRules_table').rows[0].cells.length;
if(rule_num >= upper){
alert("<%tcWebApi_get("String_Entry","JS_itemlimit1","s")%> " + upper + " <%tcWebApi_get("String_Entry","JS_itemlimit2","s")%>");
return false;
}
if(document.form.wans_FromIP_x_0.value==""){
alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
document.form.wans_FromIP_x_0.focus();
document.form.wans_FromIP_x_0.select();
return false;
}else if(document.form.wans_ToIP_x_0.value==""){
alert("<%tcWebApi_get("String_Entry","JS_fieldblank","s")%>");
document.form.wans_ToIP_x_0.focus();
document.form.wans_ToIP_x_0.select();
return false;
}else if(valid_IP_form(document.form.wans_FromIP_x_0,0)==true && valid_IP_form(document.form.wans_ToIP_x_0,0)==true){
if(item_num >=2){
for(i=0; i<rule_num; i++){
if(document.form.wans_FromIP_x_0.value == $('wans_RoutingRules_table').rows[i].cells[0].innerHTML
&& document.form.wans_ToIP_x_0.value == $('wans_RoutingRules_table').rows[i].cells[1].innerHTML){
alert("This entry has been in list.");
document.form.wans_FromIP_x_0.focus();
document.form.wans_FromIP_x_0.select();
return false;
}
}
}
addRow(document.form.wans_FromIP_x_0 ,1);
addRow(document.form.wans_ToIP_x_0, 0);
addRow(document.form.wans_unit_x_0, 0);
document.form.wans_unit_x_0.value = 0;
show_wans_rules();
}else{
return false;
}
}
function addRow(obj, head){
if(head == 1)
wans_routing_rulelist_array += "&#60"
else
wans_routing_rulelist_array += "&#62"
wans_routing_rulelist_array += obj.value;
obj.value = "";
}
function show_wans_rules(){
var wans_rules_row = wans_routing_rulelist_array.split('&#60');
var code = "";
code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="wans_RoutingRules_table">';
if(wans_rules_row.length == 1)
code +='<tr><td style="color:#FFCC00;" colspan="4"><%tcWebApi_get("String_Entry","checkbox_No","s")%> data in table.</td></tr>';
else{
for(var i = 1; i < wans_rules_row.length; i++){
code +='<tr id="row'+i+'">';
var routing_rules_col = wans_rules_row[i].split('&#62');
for(var j = 0; j < routing_rules_col.length; j++){
if(j != 2){
code +='<td width="30%">'+ routing_rules_col[j] +'</td>'; //IP width="98"
}else{
code += '<td width="25%"><select class="input_option">';
if(routing_rules_col[2] =="0")
code += '<option value="0" selected>Primary WAN</option>';
else
code += '<option value="0">Primary WAN</option>';
if(routing_rules_col[2] =="1")
code += '<option value="1" selected>Secondary WAN</option>';
else
code += '<option value="1">Secondary WAN</option>';
code += '</select></td>';
}
}
code +='<td width="15%"><!--input class="edit_btn" onclick="edit_Row(this);" value=""/-->';
code +='<input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
}
}
code +='</table>';
$("wans_RoutingRules_Block").innerHTML = code;
}
function save_table(){
var rule_num = $('wans_RoutingRules_table').rows.length;
var item_num = $('wans_RoutingRules_table').rows[0].cells.length;
var tmp_value = "";
var comp_tmp = "";
for(i=0; i<rule_num; i++){
tmp_value += "<"
for(j=0; j<item_num-1; j++){
if(j==2){
tmp_value += $('wans_RoutingRules_table').rows[i].cells[2].firstChild.value;
}else{
tmp_value += $('wans_RoutingRules_table').rows[i].cells[j].innerHTML;
}
if(j != item_num-2)
tmp_value += ">";
}
}
if(tmp_value == "<"+"No data in table." || tmp_value == "<")
tmp_value = "";
document.form.wans_routing_rulelist.value = tmp_value;
}
function change_isp_unit(v){
if(v == 0){
document.form.wan0_routing_isp_enable.value = 1;
document.form.wan1_routing_isp_enable.value = 0;
}else{
document.form.wan0_routing_isp_enable.value = 0;
document.form.wan1_routing_isp_enable.value = 1;
}
}
var country = new Array("china");
var country_n_isp = new Array();
for(i=0; i < country.length; i++){
country_n_isp[i]=new Array("edu","telecom","mobile","unicom");
}
function Load_ISP_country(){
document.form.isp_country.options[0] = new Option("China", 0);
}
function appendcoutry(v){
var country_isp_num = country_n_isp[v].length;
for(j = 0; j < country_isp_num; j++){
document.form.isp_list.style.display = "";
document.form.isp_list.options[j] = new Option(country_n_isp[v][j], j);
}
}
</script>
</head>
<body onload="initial();">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="start_apply.asp" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_WANPort_Content.asp">
<input type="hidden" name="next_page" value="Advanced_WANPort_Content.asp">
<input type="hidden" name="next_host" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_wait" value="30">
<input type="hidden" name="action_script" value="reboot">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="EN">
<input type="hidden" name="firmver" value="<% tcWebApi_staticGet("DeviceInfo","FwVer","s") %>">
<input type="hidden" name="wl_ssid" value="IBIZA_2.4G">
<input type="hidden" name="wans_dualwan" value="dsl usb">
<input type="hidden" name="wans_lanport" value="1">
<input type="hidden" name="wans_lb_ratio" value="3:1">
<input type="hidden" name="wan0_routing_isp_enable" value="0">
<input type="hidden" name="wan0_routing_isp" value="china_mobile">
<input type="hidden" name="wan1_routing_isp_enable" value="0">
<input type="hidden" name="wan1_routing_isp" value="china_mobile">
<input type="hidden" name="wans_routing_rulelist" value=''>
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
<td valign="top">
<table width="760px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
<div class="formfonttitle">Wan Port Setup</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<div class="formfontdesc"><%tcWebApi_get("String_Entry","L3F_x_ConnectionType_sd","s")%></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead>
<tr>
<td colspan="2"><%tcWebApi_get("String_Entry","t2BC","s")%></td>
</tr>
</thead>
<tr>
<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> Dual Wan</th>
<td>
<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_dualwan_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$j('#radio_dualwan_enable').iphoneSwitch(wans_dualwan_orig.split(' ')[1] != 'none',
function() {
wans_flag = 1;
inputCtrl(document.form.wans_second, 1);
wans_caps_secondary = wans_caps.split(" ").splice(1);
addWANOption(document.form.wans_second, wans_caps_secondary);
document.form.wans_second.value = wans_dualwan_orig.split(" ")[1];
inputCtrl(document.form.wans_mode, 1);
appendModeOption(wans_mode_orig);
},
function() {
wans_flag = 0;
document.form.wans_dualwan.value = document.form.wans_primary.value + " none";
inputCtrl(document.form.wans_second, 0);
inputCtrl(document.form.wans_mode, 0);
inputCtrl(document.form.wans_lb_ratio_0, 0);
inputCtrl(document.form.wans_lb_ratio_1, 0);
inputCtrl(document.form.wans_routing_isp_enable[1], 0);
inputCtrl(document.form.wans_routing_isp_enable[0], 0);
inputCtrl(document.form.wans_isp_unit, 0);
inputCtrl(document.form.isp_country, 0);
inputCtrl(document.form.isp_list, 0);
inputCtrl(document.form.wans_routing_enable[1], 0);
inputCtrl(document.form.wans_routing_enable[0], 0);
},
{
switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
}
);
</script>
</div>
</td>
</tr>
<tr>
<th>Primary WAN</th>
<td>
<select name="wans_primary" class="input_option" onchange="appendLANoption1(this);"></select>
</td>
</tr>
<tr>
<th>Secondary WAN</th>
<td>
<select name="wans_second" class="input_option" onchange="appendLANoption2(this);"></select>
</td>
</tr>
<tr>
<th>Multi WAN Mode</th>
<td>
<select name="wans_mode" class="input_option" onchange="appendModeOption(this.value);">
<option value="fo" selected>Fail Over</option>
<option value="lb" >LoadBalance</option>
</select>
</td>
</tr>
<tr>
<th>Load Balance Configuration</th>
<td>
<input type="text" maxlength="1" class="input_3_table" name="wans_lb_ratio_0" value="" onkeypress="" onkeyup="" onBlur=""/>&nbsp;:
<input type="text" maxlength="1" class="input_3_table" name="wans_lb_ratio_1" value="" onkeypress="" onkeyup="" onBlur=""/>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> ISP profile rules?</th>
<td>
<input type="radio" value="1" name="wans_routing_isp_enable" class="content_input_fd" onClick="change_isp_radio(this.value);" ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="wans_routing_isp_enable" class="content_input_fd" onClick="change_isp_radio(this.value);" ><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
<tr>
<th>WAN to apply ISP profile rules</th>
<td>
<select name="wans_isp_unit" class="input_option" onchange="change_isp_unit(this.value)">
<option class="content_input_fd" value="0" >Primary WAN</option>
<option class="content_input_fd" value="1">Secondary WAN</option>
</select>
</td>
</tr>
<tr>
<th>Country / ISP </th>
<td>
<select name="isp_country" class="input_option" onchange="appendcoutry(this.value);" value=""></select>
<select name="isp_list" class="input_option" style="display:none;"value=""></select>
</td>
</tr>
<tr>
<th><%tcWebApi_get("String_Entry","WC11b_WirelessCtrl_button1name","s")%> the Routing Tables?</th>
<td>
<input type="radio" value="1" name="wans_routing_enable" class="content_input_fd" onClick="" ><%tcWebApi_get("String_Entry","checkbox_Yes","s")%>
<input type="radio" value="0" name="wans_routing_enable" class="content_input_fd" onClick="" checked><%tcWebApi_get("String_Entry","checkbox_No","s")%>
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;" >
<thead>
<tr><td colspan="4" id="Routing_table">Routing rules table for dual WAN</td></tr>
</thead>
<tr>
<th>Source IP</th>
<th>Destination IP</th>
<th>WAN Unit</th>
<th><% tcWebApi_Get("String_Entry", "list_add_delete", "s") %></th>
</tr>
<tr>
<div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>
<td width="30%">
<input type="text" class="input_15_table" maxlength="15" name="wans_FromIP_x_0" style="" onKeyPress="return is_ipaddr(this,event)">
</td>
<td width="30%">
<input type="text" class="input_15_table" maxlength="15" name="wans_ToIP_x_0" onkeypress="return is_ipaddr(this,event)">
</td>
<td width="25%">
<select name="wans_unit_x_0" class="input_option">
<option value="0">Primary WAN</option>
<option value="1">Secondary WAN</option>
</select>
</td>
<td width="15%">
<div>
<input type="button" class="add_btn" onClick="addRow_Group(32);" value="">
</div>
</td>
</tr>
</table>
<div id="wans_RoutingRules_Block"></div>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="<%tcWebApi_get("String_Entry","CTL_apply","s")%>"/>
</div>
</td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
</form>
<div id="footer"></div>
</body>

<!--Advanced_WANPort_Content.asp-->
</html>

