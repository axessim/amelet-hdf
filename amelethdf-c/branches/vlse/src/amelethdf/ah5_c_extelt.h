#ifndef AH5_C_EXTELT_H
#define AH5_C_EXTELT_H

#include "ah5_config.h"
#include "ah5_general.h"
#ifndef _MSC_VER
#include <unistd.h>
#else
#define R_OK 4
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _AH5_eet_dataset_t
	{
		char            *path;
		hid_t           *file_id;
		hsize_t         nb_eed_items;
		char            **eed_items;
	} AH5_eet_dataset_t;

	typedef struct _AH5_external_element_t
	{
		hsize_t         nb_datasets;
		AH5_eet_dataset_t *datasets;
	} AH5_external_element_t;

    AH5_PUBLIC char AH5_read_eet_dataset (hid_t file_id, const char *path, AH5_eet_dataset_t *eet_dataset);
    AH5_PUBLIC char AH5_read_external_element (hid_t file_id, AH5_external_element_t *external_element);

    AH5_PUBLIC void AH5_print_eet_dataset (const AH5_eet_dataset_t *eet_dataset, int space);
    AH5_PUBLIC void AH5_print_external_element (const AH5_external_element_t *external_element);

    AH5_PUBLIC void AH5_free_eet_dataset (AH5_eet_dataset_t *eet_dataset);
    AH5_PUBLIC void AH5_free_external_element (AH5_external_element_t *external_element);

    AH5_PUBLIC char AH5_open_external_files (AH5_eet_dataset_t *eet_dataset);
    AH5_PUBLIC char AH5_close_external_files (AH5_eet_dataset_t *eet_dataset);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_EXTELT_H
