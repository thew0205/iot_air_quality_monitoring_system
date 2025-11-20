#include "ff_sddisk.h"
#include "ff_stdio.h"

static FF_Disk_t *pxDisk = nullptr;

bool fat_sd_card_init()
{
    bool noError = true;
    pxDisk = FF_SDDiskInit("sd0");
    configASSERT(pxDisk);
    noError &= pxDisk != nullptr;
    if (noError)
    {
        BaseType_t xError = FF_SDDiskMount(pxDisk);
        configASSERT(!FF_isERR(xError));
        noError &= !FF_isERR(xError);
    }
    if (noError)
    {
        noError &= FF_FS_Add("/sd0", pxDisk);
    }

    return noError;
}

bool fat_sd_card_deinit()
{
    bool noError = true;

    FF_FS_Remove("/sd0");
    noError &= (FF_Unmount(pxDisk) == FF_ERR_NONE);
    FF_SDDiskDelete(pxDisk);
    return noError;
}

FF_FILE *fat_sd_card_open(const char *pcFile, const char *pcMode)
{
    return ff_fopen(pcFile, pcMode);
}

bool fat_sd_card_close(FF_FILE *pxStream)
{
    return !FF_isERR(ff_fclose(pxStream));
}

size_t fat_sd_card_write(const void *pvBuffer, size_t xItems, FF_FILE *pxStream)
{
    return ff_fwrite(pvBuffer,
                     1,
                     xItems,
                     pxStream);
}
