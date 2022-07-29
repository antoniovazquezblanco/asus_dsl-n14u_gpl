#!/bin/sh

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
echo "forsq = ${forsq}" >> /tmp/webs_upgrade.log #1 for sq
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
echo "model = ${model}" >> /tmp/webs_upgrade.log #DSL-N66U

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#must be marked before checkin.
#update_url="http://192.168.123.198"

/userfs/bin/tcapi set WebCustom_Entry webs_state_update 0 &# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_url "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_info "" &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &

if [ "$update_url" != "" ]; then
	echo "---- update_url exist ----" >> /tmp/webs_upgrade.log
        wget -q ${update_url}/wlan_update_v2.zip -O /tmp/wlan_update.txt
elif [ "$forsq" = "1" ]; then
	echo "---- sq pqth ----" >> /tmp/webs_upgrade.log
        wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/wlan_update_v2.zip -O /tmp/wlan_update.txt
else
	echo "---- Official path ----" >> /tmp/webs_upgrade.log
	wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless/wlan_update_v2.zip -O /tmp/wlan_update.txt
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download wlan_update.zip fail" &
else
	buildno=`grep $model /tmp/wlan_update.txt | sed 's/.*#FW//;' | sed 's/#.*//;'`
	url_path=`grep $model /tmp/wlan_update.txt | sed 's/.*#URL//;' | sed 's/#.*//;'`
	if [ "$buildno" = "" ]; then
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
		/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "parse wlan_update.txt fail" &
	else
		/userfs/bin/tcapi set WebCustom_Entry webs_state_info $buildno &
		/userfs/bin/tcapi set WebCustom_Entry webs_state_url $url_path &
	fi
fi
rm /tmp/wlan_update.txt
/userfs/bin/tcapi set WebCustom_Entry webs_state_update 1 &
