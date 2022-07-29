#include "rc.h"
#include <arpa/inet.h>

static int
expires(int unit, unsigned int in)
{
	char tmp[32];
	time_t now;
	FILE *fp;

	// nvram_set_int(strcat_r(prefix, "expires", tmp), (unsigned int) uptime() + in);
	sprintf(tmp, "/tmp/udhcpc%d.expires.uptime", unit);
	if ((fp = fopen(tmp, "w")) == NULL) {
		perror(tmp);
		return errno;
	}
	fprintf(fp, "%d", (unsigned int) uptime() + in);
	fclose(fp);

	snprintf(tmp, sizeof(tmp), "/tmp/udhcpc%d.expires", unit); 
	if ((fp = fopen(tmp, "w")) == NULL) {
		perror(tmp);
		return errno;
	}
	time(&now);
	fprintf(fp, "%d", (unsigned int) now + in);
	fclose(fp);

	return 0;
}

int
udhcpc_expires(int argc, char **argv)
{
	if (argc < 3)
		return EINVAL;
	else {
		return expires(atoi(argv[1]), strtoul(argv[2], NULL, 10));
	}
}

int
udhcpc_bound(int argc, char **argv)
{
	char lan_ip[16] = {0};
	char lan_subnet[16] = {0};
	int wan_unit;
	char prefix[8] = {0};

	if (argc < 4)
		return EINVAL;

	tcapi_get("Lan_Entry0", "IP", lan_ip);
	tcapi_get("Lan_Entry0", "netmask", lan_subnet);

	if ((inet_network(argv[1]) & inet_network(argv[2])) ==
		(inet_network(lan_ip) & inet_network(lan_subnet))) {
dbg("wan ip = %x!\n", inet_network(argv[1]));
dbg("wan subnet = %x!\n", inet_network(argv[2]));
dbg("lan ip = %x!\n", inet_network(lan_ip));
dbg("lan subnet = %x!\n", inet_network(lan_subnet));
		wan_unit = atoi(argv[3]);
		sprintf(prefix, "wan%d_", wan_unit);
		update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_INVALID_IPADDR);
		return 200;
	}

	return 0;
}
