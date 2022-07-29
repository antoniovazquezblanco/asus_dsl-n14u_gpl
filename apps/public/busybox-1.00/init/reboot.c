/* vi: set sw=4 ts=4: */
/*
 * Mini reboot implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/reboot.h>
#include "busybox.h"
#include "init_shared.h"

#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
#define		SIGNAL_PATH		"/var/tmp/signal_reboot"
#define		SIGNAL_PATH_SYSLOG		"/tmp/syslogd_reboot"
static int quit_signal(void)
{
	int ret;
	FILE *fp = NULL;
	char buf[8];

	memset(buf, 0, sizeof(buf));
	
	fp = fopen(SIGNAL_PATH, "r");

	if (fp == NULL)
		ret = 0;
	else {
		fgets(buf, 8, fp);
		
		if (buf[0] == '1')
			ret = 1;
		else 
			ret = 0;

		fclose(fp);
		unlink(SIGNAL_PATH);
	}

	return ret;
}

static int syslogd_quit_signal(void)
{
	int ret;
	FILE *fp = NULL;

	fp = fopen(SIGNAL_PATH_SYSLOG, "r");
	if (fp == NULL)
		ret = 0;
	else {
		ret = 1;
		fclose(fp);
		unlink(SIGNAL_PATH_SYSLOG);
		system("echo the syslogd is terminated by signal > /dev/console");
	}

	return ret;
}
#endif

extern int reboot_main(int argc, char **argv)
{
	char *delay; /* delay in seconds before rebooting */

#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
	int count = 0;
#endif
	if(bb_getopt_ulflags(argc, argv, "d:", &delay)) {
		sleep(atoi(delay));
	}
	
#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)	
	system("killall -SIGUSR1 cfg_manager");
	/* wait cfg_manager done */
	while (!quit_signal() && count++ < 10)
		sleep(1);
	
#endif
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	count = 0;
	system("killall syslogd");
	/* wait syslogd storing system log */
	while (!syslogd_quit_signal() && count++ < 10)
		sleep(1);
#endif

#ifndef CONFIG_INIT
#ifndef RB_AUTOBOOT
#define RB_AUTOBOOT		0x01234567
#endif
	return(bb_shutdown_system(RB_AUTOBOOT));
#else
	return kill_init(SIGTERM);
#endif
}

/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
