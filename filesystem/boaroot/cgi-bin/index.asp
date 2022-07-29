<%
if tcWebApi_Get("WebCurSet_Entry","lang_detected","h") = "1" then
	tcWebApi_commit("LanguageSwitch_Entry")
end if
%>
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<script>
	if("<% tcWebApi_staticGet("SysInfo_Entry","x_Setting","s") %>" == "0")
		document.location.href = "/cgi-bin/QIS_wizard.asp";
	else
		document.location.href = "/cgi-bin/index2.asp";
</script>
</html>
