#!/bin/sh

/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 0 &# INITIALIZING
/userfs/bin/tcapi set WebCustom_Entry webs_state_error 0 &
/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "" &

forsq=`/userfs/bin/tcapi get Apps_Entry apps_sq`
url_path=`/userfs/bin/tcapi get WebCustom_Entry webs_state_url`
model=`/userfs/bin/tcapi get SysInfo_Entry ProductName`
firmware_file=`/userfs/bin/tcapi get SysInfo_Entry ProductName`_`/userfs/bin/tcapi get WebCustom_Entry webs_state_info`.zip

touch /tmp/update_url
update_url=`cat /tmp/update_url`

#testing purpose, must be marked before checkin.
#update_url="http://192.168.123.198

# get firmware information
echo 3 > /proc/sys/vm/drop_caches
if [ "$update_url" != "" ]; then
	echo "---- update_url exist upgrade ----" >> /tmp/webs_upgrade.log
        wget -q ${update_url}/$firmware_file -O /var/tmp/tclinux.bin
elif [ "$forsq" = "1" ]; then
	echo "---- sq path upgrade ----" >> /tmp/webs_upgrade.log
        wget -q http://dlcdnet.asus.com/pub/ASUS/LiveUpdate/Release/Wireless_SQ/$firmware_file -O /var/tmp/tclinux.bin
elif [ "$url_path" = "" ]; then
	echo "---- Official path upgrade ----" >> /tmp/webs_upgrade.log
	wget -q http://dlcdnet.asus.com/pub/ASUS/wireless/ASUSWRT/$firmware_file -O /var/tmp/tclinux.bin
else
	echo "---- External URL path upgrade ----" >> /tmp/webs_upgrade.log
	wget -q $url_path/$firmware_file -O /var/tmp/tclinux.bin
fi

if [ "$?" != "0" ]; then
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error 1 &
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware fail" &
else
	echo 3 > /proc/sys/vm/drop_caches
	/userfs/bin/tcapi set WebCustom_Entry webs_state_error_msg "download firmware successfully" &
fi
/userfs/bin/tcapi set WebCustom_Entry webs_state_upgrade 1 &
