/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Network services
 *
 * Copyright 2004, ASUSTeK Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include "libtcapi.h"
#include "rtstate.h"
#include "rc.h"

void update_wan_state(char *prefix, int state, int reason)
{
	char attr[32], value[4];

	// nvram_set_int(strcat_r(prefix, "state_t", tmp), state);
	strcat_r(prefix, "state_t", attr);
	sprintf(value, "%d", state);
	tcapi_set("Wanduck_Common", attr, value);
	// nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), 0);
	strcat_r(prefix, "sbstate_t", attr);
	tcapi_set("Wanduck_Common", attr, "0");
	// nvram_set_int(strcat_r(prefix, "auxstate_t", tmp), 0);
	strcat_r(prefix, "auxstate_t", attr);
	tcapi_set("Wanduck_Common", attr, "0");

	if (state == WAN_STATE_STOPPED) {
		// Save Stopped Reason
		// keep ip info if it is stopped from connected
		// nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), reason);
		strcat_r(prefix, "sbstate_t", attr);
		sprintf(value, "%d", reason);
		tcapi_set("Wanduck_Common", attr, value);
	}
	else if(state == WAN_STATE_STOPPING){
		unlink("/tmp/wanstatus.log");
	}
}

int
found_default_route(int wan_unit)
{
	int i, n, found;
	FILE *f;
	unsigned int dest, mask;
	char buf[256], device[256];
	char *wanif;

	n = 0;
	found = 0;
	mask = 0;
	device[0] = '\0';

	if (f = fopen("/proc/net/route", "r"))
	{
		while (fgets(buf, sizeof(buf), f) != NULL)
		{
			if (++n == 1 && strncmp(buf, "Iface", 5) == 0)
				continue;

			i = sscanf(buf, "%255s %x %*s %*s %*s %*s %*s %x",
						device, &dest, &mask);

			if (i != 3)
			{
//				fprintf(stderr, "junk in buffer");
				break;
			}

			if (device[0] != '\0' && dest == 0 && mask == 0)
			{
//				fprintf(stderr, "default route dev: %s\n", device);
				found = 1;
				break;
			}
		}

		fclose(f);

		wanif = get_wan_ifname(wan_unit);

		if (found && !strcmp(wanif, device))
		{
//			fprintf(stderr, "got default route!\n");
			return 1;
		}
		else
		{
			// fprintf(stderr, "no default route!\n");
			return 0;
		}
	}

	fprintf(stderr, "no default route!!!\n");
	return 0;
}
