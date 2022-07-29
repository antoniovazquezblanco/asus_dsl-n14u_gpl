#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#include "libtcapi.h"
#include "shutils.h"
#include "rc.h"

int
ppp_linkunit(char *linkname)
{
	int unit = -1;
	if (strncmp(linkname, "ppp", 3))
		return -1;
	if (!isdigit(linkname[3]))
		return -1;
	unit = atoi(&linkname[3]);
	if(unit >= 80)
		return unit/10;
	else
		return unit;
}

/*
 * Called when link comes up
 */
int
ipup_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char prefix[] = "wanXXXXXXXXXX_";
	int unit;

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;
	
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	update_wan_state(prefix, WAN_STATE_CONNECTED, 0);

	//restart firewall
	tcapi_commit("Firewall");

	//restart QoS
	tcapi_commit("QoS");

	//re-write routing table
	tcapi_commit("Route");

	//restart ddns for ppp interface
	tcapi_commit("Ddns");

	return 0;
}

/*
 * Called when link goes down
 */
int
ipdown_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char prefix[] = "wanXXXXXXXXXX_";
	int unit;

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	update_wan_state(prefix, WAN_STATE_STOPPED, pppstatus());

	return 0;
}


/*
 * Called when link closing with auth fail
 */
int
authfail_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char prefix[] = "wanXXXXXXXXXX_";
	int unit;

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	// override wan_state
	update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PPP_AUTH_FAIL);

	return 0;
}
