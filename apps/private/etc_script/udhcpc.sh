#!/bin/sh

# udhcpc script edited by Tim Riker <Tim@Rikers.org>

[ -z "$1" ] && echo "Error: should be called from udhcpc" && exit 1
PROFILE_CFG=/userfs/profile.cfg
#TYPE=`tcapi get Dproxy_Entry type`
#echo $TYPE
if [ -f $PROFILE_CFG ] ; then
	. $PROFILE_CFG
fi
#if [ "$TCSUPPORT_DNSEACHPVC" != "" ] ;then
	RESOLV_CONF="/etc/resolv_"$interface".conf"
#else
#	RESOLV_CONF="/etc/resolv.conf"
#fi
GATEWAY_FILE="/etc/"$interface"_gateway.conf"
IFNAME=`expr substr $interface 1 3`
if [ "$IFNAME" = "eth" -o "$IFNAME" = "usb" ]; then
WAN_NUM=11
else
WAN_NUM=`expr substr $interface 4 2`
fi

if [ "`expr index $WAN_NUM _`" != "0" ]; then
WAN_NUM=`expr substr $WAN_NUM 1 1`
fi
LEASE_FILE="/tmp/udhcpc"$WAN_NUM".lease"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"

if [ "$WAN_NUM" = "11" ]; then
ISPCONF="/etc/isp""$WAN_NUM"".conf"
elif [ "$TCSUPPORT_MULTISERVICE_ON_WAN" != "" -a $WAN_NUM -gt 7 ]; then
ISPCONF="/etc/isp""$WAN_NUM""_0.conf"
else
ISPCONF=/etc/isp$WAN_NUM.conf
fi
if [ -f $ISPCONF ] ; then
	chmod 777 $ISPCONF
	. $ISPCONF
else
	exit 0
fi

VPNC_CFG=/etc/vpnc.conf
if [ -f $VPNC_CFG ] ; then
    . $VPNC_CFG
fi

case "$1" in
	deconfig)
		#/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		rm -f $GATEWAY_FILE
		rm -f /tmp/udhcpc*
		;;

	renew|bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK
		/sbin/rcset System_Entry CurrentWANIP $ip
		/sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_ipaddr" "$ip"
		/sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_netmask" "$subnet"

		if [ -n "$router" ] ; then
			echo "deleting routers"
			while /sbin/route del default gw 0.0.0.0 dev $interface ; do
				:
			done

			echo "adding default routers"
			for i in $router ; do
				echo "route add default gw $i dev $interface"
				/sbin/route add default gw $i dev $interface
				echo "gateway=$i" > $GATEWAY_FILE
			done
		fi

		#if [ $TYPE != "1" ] ; then
			echo -n > $RESOLV_CONF
			for i in $dns ; do
				echo adding dns $i
				echo nameserver $i >> $RESOLV_CONF
			done
			[ -n "$domain" ] && echo search $domain >> $RESOLV_CONF
		#fi

		# Save the lease time
		echo -n $lease > $LEASE_FILE
		/sbin/udhcpc_expires $interface $lease

		# bound, check network whether the same with lan
		/sbin/udhcpc_bound $ip $subnet $WAN_NUM
		if [ "$?" -ne "0" ] ; then
			exit 1
		fi

		# Set gateway info
		[ -n "$router" ] && /sbin/rcset "Wan_PVC"$WAN_NUM "gateway_x" "$router"
		[ -n "$router" ] && /sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_gateway" "$router"

		# Set dns info
		if [ "$DNS_type" != "1" ] ; then
			[ -n "$dns" ] && /sbin/rcset "Wan_PVC"$WAN_NUM "dns_x" "$dns"
			[ -n "$dns" ] && /sbin/rcset "Wanduck_Common" "wan"$WAN_NUM"_dns" "$dns"
		fi

		echo "[udhcpc.sh]DNS=$dns" > /dev/console
		/sbin/wan_up $interface

		# WAN_STATE_CONNECTED
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_state_t" "2" &
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_sbstate_t" "0" &
		#/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_auxstate_t" "0" &

		# restart firewall
		#/userfs/bin/tcapi commit "Firewall" &

		# restart QoS
		#/userfs/bin/tcapi commit "QoS" &

		# re-write routing table
		#/userfs/bin/tcapi commit "Route" &

		# restart ddns
		#/userfs/bin/tcapi commit "Ddns" &

		# check and start vpnc
		#if [ "$auto_conn" = "1" ] ; then
			#/userfs/bin/tcapi commit "VPNC" &
			#Stop vpn client
			#killall run_vpnc
			#if [ -f /var/run/l2tpd-vpnc.pid ] ; then
				#kill -15 `cat /var/run/l2tpd-vpnc.pid`
				#sleep 1
			#fi
			#if [ -f /var/run/ppp20.pid ] ; then
				#kill -15 `cat /var/run/ppp20.pid`
				#sleep 2 
				#if [ -f /var/run/ppp20.pid ] ; then
					#kill -9 `cat /var/run/ppp20.pid`
					#rm -f /var/run/ppp20.pid
				#fi
			#fi
			#/sbin/run_vpnc &
		#fi

		;;
esac

exit 0
