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
WAN_NUM=`expr substr $interface 4 2`
LEASE_FILE="/tmp/udhcpc"$WAN_NUM".lease"

[ -n "$broadcast" ] && BROADCAST="broadcast $broadcast"
[ -n "$subnet" ] && NETMASK="netmask $subnet"


case "$1" in
	deconfig)
		#/sbin/ifconfig $interface down
		/sbin/ifconfig $interface 0.0.0.0
		rm $GATEWAY_FILE
		rm /tmp/udhcpc*
		;;

	renew|bound)
		/sbin/ifconfig $interface $ip $BROADCAST $NETMASK
		/userfs/bin/tcapi set System_Entry CurrentWANIP $ip &

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
		/sbin/udhcpc_expires $WAN_NUM $lease

		# bound, check network whether the same with lan
		/sbin/udhcpc_bound $ip $subnet $WAN_NUM
		if [ "$?" -ne "0" ] ; then
			exit 1
		fi

		# WAN_STATE_CONNECTED
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_state_t" "2" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_sbstate_t" "0" &
		/userfs/bin/tcapi set "Wanduck_Common" "wan"$WAN_NUM"_auxstate_t" "0" &

		# restart firewall
		/userfs/bin/tcapi commit "Firewall" &

		# restart QoS
		/userfs/bin/tcapi commit "QoS" &

		# re-write routing table
		/userfs/bin/tcapi commit "Route" &

		# restart ddns
		/userfs/bin/tcapi commit "Ddns" &
		;;
esac

exit 0
