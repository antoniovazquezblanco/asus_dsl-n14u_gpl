
<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=

iso-8859-1

">
<link rel="stylesheet" type="text/css" href="/style.css">

<style type="text/css">
</style>
<script type='text/javascript'>
function onClickRunWizard() {
	window.open("/cgi-bin/wizardset.asp","OpenClose","toolbar=no,menubar=no,height=400,width=520,location=0,left=100,top=100");
    return false;
}
</script>
</head>

<body>


<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main"><% tcWebApi_get("String_Entry","HelpIndexQuickText","s") %></td>
    <td width="10" class="black">&nbsp;</td>
    <td width="50" ><hr noshade></td>
    <td width="500"><hr noshade></td>
    <td width="50" ><hr noshade></td>
  </tr>
  <tr>
    <td width="150" class="light-orange">&nbsp;</td>
    <td width="10" class="light-orange"></td>
    <td width="50">&nbsp;</td>
    <td width="500" class="databold" height="150"><span class="Model">
    
     <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
     <% tcWebApi_get("String_Entry","HelpHomeVDSLWizardText","s") %>
     <%else%>
     <% tcWebApi_get("String_Entry","HelpHomeADSLWizardText","s") %>
     <%end if%>
		
        </span></td>
    <td width="50" class="content" >&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="20" class="light-orange">&nbsp;</td>
    <td width="10"  class="light-orange"></td>
    <td width="50"  height="30">&nbsp;</td>
    <td width="500" height="50">&nbsp;</td>
    <td width="50"  class="tabdata">&nbsp;</td>
  </tr>
</table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
    	<INPUT type=button 
    	
    	value="<% tcWebApi_get("String_Entry","ButtonRunWizardText","s") %>"
    	
		 name=wizard onclick="onClickRunWizard()">
    </td>
  </tr>
</table>
</div>
</body>
</html>        
