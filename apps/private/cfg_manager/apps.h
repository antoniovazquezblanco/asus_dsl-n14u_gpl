#ifndef _APPS_H
#define _APPS_H

#include "cfg_manager.h"

int
usb_init(void);
int
usb_boot(mxml_node_t *top);

int
apps_init(void);

int
apps_boot(mxml_node_t *top);

int
aicloud_init(void);
int
aicloud_boot(mxml_node_t *top);
int
aicloud_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif