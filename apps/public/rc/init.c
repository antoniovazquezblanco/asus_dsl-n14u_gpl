#include "rc.h"
#include <sys/types.h>
#include <linux/fs.h>
#include "shutils.h"
#ifdef RTCONFIG_USB
#include "usb_info.h"
#endif
#include "tcutils.h"

#define SYSINFO		"SysInfo_Entry"
#define APPS_DATA	"Apps_Entry"

int init_nvram(void)
{
	int model = get_model();

	tcapi_set(SYSINFO, "rc_support", "");

	/* Paul modify 2013/6/5 */
	switch (model) {
	case MODEL_DSLN16U:
		add_rc_support("2.4G update usbX2 rawifi dsl dualwan wifi_hw_sw ipv6 PARENTAL2 printer mssid no5gmssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN55UC1:
		add_rc_support("2.4G 5G update usbX2 rawifi dsl dualwan wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN66U:
		add_rc_support("2.4G 5G update usbX2 rawifi dsl dualwan wifi_hw_sw ipv6 PARENTAL2 printer mssid appnet pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN10C1:
		add_rc_support("2.4G update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid pwrctrl wds HTTPS");
		break;
	case MODEL_DSLN12EC1:
		add_rc_support("2.4G update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid pwrctrl wds HTTPS");
		break;
	case MODEL_DSLN12UC1:
		add_rc_support("2.4G update usbX1 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid appnet pwrctrl wds HTTPS");
		break;
	case MODEL_DSLN14U:
		add_rc_support("2.4G update usbX1 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid appnet pwrctrl wds HTTPS");
		break;
	}

#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	add_rc_support("pptpd");
#endif
#ifdef MTK_CRYPTO_DRIVER
	add_rc_support("hwcrypto");
#endif
#ifdef RTCONFIG_SPECTRUM
add_rc_support("spectrum");
#endif /* RTCONFIG_SPECTRUM */

#ifdef ASUSSUPPORT_EMAIL
	add_rc_support("email");
#endif

#ifdef CWMP
	add_rc_support("cwmp");
#endif

	tcapi_set(APPS_DATA, "apps_ipkg_server", "http://nw-dlcdnet.asus.com/asusware/mipsbig/stable");
	tcapi_set(APPS_DATA, "apps_dev", "");
	tcapi_set(APPS_DATA, "apps_mounted_path", "");

	return 0;
}

static void sysinit(void)
{
#ifdef TCSUPPORT_USBHOST
	 // LPRng support
        mkdir("/var/state", 0777);
        mkdir("/var/state/parport", 0777);
        mkdir("/var/state/parport/svr_statue", 0777);
#endif
#ifdef TCSUPPORT_USB_PRINTER_SERVER
	eval("hotplug2", "--coldplug");
	start_hotplug2();
#endif
	init_nvram();  // for system indepent part after getting model	

#ifdef RTCONFIG_SHP
	// if(nvram_get_int("qos_enable") || nvram_get_int("macfilter_enable_x") || nvram_get_int("lfp_disable_force")) {
	if(tcapi_match("QoS_Entry0", "qos_enable", "1")) {
		tcapi_set("GUITemp_Entry1", "lfp_disable", "1");
	}
	else {
		tcapi_set("GUITemp_Entry1", "lfp_disable", "0");
	}

	if(tcapi_get_int("GUITemp_Entry1", "lfp_disable")==0) {
		restart_lfp();
	}
#endif

	setup_conntrack();
}

int init_main(int argc, char *argv[])
{

	sysinit();
	
#ifdef TCSUPPORT_USBHOST
	int fd = -1;
	fd = file_lock("usb");	// hold off automount processing
	start_usb();
	file_unlock(fd);	// allow to process usb hotplug events
#endif

	start_services();
	tcapi_set("Apps_Entry", "success_start_service", "1");

#ifdef TCSUPPORT_USB_PRINTER_SERVER
	start_usblpsrv();
#endif

	return 0;
}
