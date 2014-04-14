/**
 * Configur file filled by compier tool.
 */

#define AH5_MPI_ENABME_ @AMELETHDF_ENABLE_MPI@

#if @AMELETHDF_DEBUG@
# define AH5__DEBUG 1
#else
# define AH5_NDEBUG 1
#endif
