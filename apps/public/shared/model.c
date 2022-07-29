#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #include <bcmnvram.h>
// #include <bcmdevs.h>
#include "libtcapi.h"
#include "shared.h"

int get_model(void)
{
	char ProductName[32] = {0};
	tcapi_get("SysInfo_Entry", "ProductName", ProductName);

	if(!strcmp(ProductName, "DSL-N16U"))	return MODEL_DSLN16U;
	else if(!strcmp(ProductName, "DSL-N66U")) return MODEL_DSLN66U;
	else if(!strcmp(ProductName, "DSL-N55U-C1")) return MODEL_DSLN55UC1;
	else if(!strcmp(ProductName, "DSL-N10-C1")) return MODEL_DSLN10C1;
	else if(!strcmp(ProductName, "DSL-N12E-C1")) return MODEL_DSLN12EC1;
	else if(!strcmp(ProductName, "DSL-N12U-C1")) return MODEL_DSLN12UC1;
	else if(!strcmp(ProductName, "DSL-N14U")) return MODEL_DSLN14U;
	else return MODEL_UNKNOWN;
}
