<?xml version="1.0" ?>
<devicemap>
  <% ajax_wanstate(); %>
  <wan>usb=<% tcWebApi_Get("USB_Entry", "usb_path1", "s") %></wan>
  <wan>usb=<% tcWebApi_Get("USB_Entry", "usb_path2", "s") %></wan>
  <wan>monoClient=<% tcWebApi_Get("USB_Entry", "mfp_ip_monopoly", "s") %></wan>
  <wan>cooler=""</wan>
  <wan>wlc_state=""</wan>
  <wan>wlc_sbstate=""</wan>
  <wan>wifi_hw_switch=<% tcWebApi_Get("WLan_Common", "APOn", "s") %></wan>
  <wan>psta:""</wan>
  <wan>umount=<% tcWebApi_Get("USB_Entry", "usb_path1_removed", "s"); %></wan>
  <wan>umount=<% tcWebApi_Get("USB_Entry", "usb_path2_removed", "s"); %></wan>
  <wan>ddnsRet=<% tcWebApi_Get("GUITemp_Entry2", "ddns_return_code_chk", "s"); %></wan>
  <wan>ddnsUpdate=<% tcWebApi_Get("GUITemp_Entry2", "ddns_updated", "s"); %></wan>
  <wan>wan_line_state=<% tcWebApi_staticGet("Info_Adsl", "lineState", "s") %></wan>
  <wan>wlan0_radio_flag=<% tcWebApi_staticGet("WLan_Entry", "wl0_radio_flag", "s") %></wan>
  <wan>wlan1_radio_flag=<% tcWebApi_staticGet("WLan_Entry", "wl1_radio_flag", "s") %></wan>
</devicemap>
