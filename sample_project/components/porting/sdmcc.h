#ifndef SDMCC_H
#define SDMCC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_vfs_fat.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

    void sdmcc_initt(void);

#ifdef __cplusplus
}
#endif

#endif
