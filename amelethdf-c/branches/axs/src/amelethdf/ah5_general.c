#include "ah5_general.h"

#include <ctype.h>

hid_t AH5_create(const char *name, unsigned flags, const char *entry_point)
{
  hid_t file_id;
  file_id = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  AH5_write_str_attr(file_id, ".", AH5_A_FORMAT, AH5_FORMAT);
  AH5_write_str_attr(file_id, ".", AH5_A_VERSION, AH5_DEFAULT_VERSION);

  if (entry_point)
    AH5_write_str_attr(file_id, ".", AH5_A_ENTRY_POINT, entry_point);
  
  return file_id;
}

// Set complex number
AH5_complex_t AH5_set_complex(float real, float imag)
{
    AH5_complex_t rdata;

#ifdef AH5_SDT_CCOMPLEX
    rdata =  real + imag * _Complex_I;
#else
    rdata.re = real;
    rdata.im = imag;
#endif /*AH5_SDT_CCOMPLEX*/

    return rdata;
}

hid_t AH5_H5Tcreate_cpx_memtype(void)
{
    hid_t cpx_memtype;

    cpx_memtype = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
    H5Tinsert(cpx_memtype, "r", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(cpx_memtype, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);

    return cpx_memtype;
}

hid_t AH5_H5Tcreate_cpx_filetype(void)
{
    hid_t cpx_filetype;

    cpx_filetype = H5Tcreate(H5T_COMPOUND, H5Tget_size(AH5_NATIVE_FLOAT) * 2);
    H5Tinsert(cpx_filetype, "r", 0, AH5_NATIVE_FLOAT);
    H5Tinsert(cpx_filetype, "i", H5Tget_size(AH5_NATIVE_FLOAT), AH5_NATIVE_FLOAT);

    return cpx_filetype;
}



/*
    AH5_FALSE = AH5_version_minimum("0.5", "0.1");
    AH5_FALSE = AH5_version_minimum("3.5", "2");
    AH5_FALSE = AH5_version_minimum("3.1", "2.186");

    AH5_TRUE = AH5_version_minimum("1.2.3.4", "1.2.3.4");
    AH5_TRUE = AH5_version_minimum("0.5", "0.6");
    AH5_TRUE = AH5_version_minimum("0.5", "1.8.6");
    AH5_TRUE = AH5_version_minimum("0.5.5", "1");
    AH5_TRUE = AH5_version_minimum("2", "2.5");
    AH5_TRUE = AH5_version_minimum("2.0.5.0", "2.1");
*/
char AH5_version_minimum(const char *required_version, const char *sim_version)
{
    char rdata = AH5_FALSE;
    char *a, *b, *atemp, *btemp, *temp;
    int ai = 0, bi = 0;  // storage for version numbers

    // cannot use 2x strtok
    a = AH5_trim_zeros(required_version);  // original pointers
    b = AH5_trim_zeros(sim_version);  //  to be used in free()
    atemp = a;  // creatin working pointers
    btemp = b;  //  (that will be destroyed)

    while (atemp != NULL && btemp != NULL && bi == ai)
    {
        temp = strstr(atemp, ".");  // pointer to dot in solver_version
        if (temp != NULL)
        {
            *temp = '\0';
            ai = atoi(atemp);
            atemp = temp + 1;  // pointer to the rest of the string
        }
        else
        {
            // no dot is present -> make last check
            ai = atoi(atemp);
            atemp = NULL;
        }

        temp = strstr(btemp, ".");  // pointer to dot in sim_version
        if (temp != NULL)
        {
            *temp = '\0';
            bi = atoi(btemp);
            btemp = temp + 1;  // pointer to the rest of the string
        }
        else
        {
            // no dot is present -> make last check
            bi = atoi(btemp);
            btemp = NULL;
        }

        if (bi > ai)
            rdata = AH5_TRUE;
    }
    if (atemp == NULL && bi == ai)
        rdata = AH5_TRUE;

    free(b);
    free(a);
    return rdata;
}


// Remove ".0" from the end; allocates new memory!!!
char *AH5_trim_zeros(const char *version)
{
    int i, number = 0;
    char *rdata;

    rdata = strdup(version);

    while (1)
    {
        for (i = (int) strlen(rdata); i > -1; i--)
            if (rdata[i] == '.')
                break;
        number = atoi(rdata + i + 1);
        if (number == 0 && i > -1)
            rdata[i] = '\0';
        else
            break;
    }
    return rdata;
}



/** Join path. Append to 'base' the new node 'name' and return 'base'
 *
 * Parameters:
 *  - base: Pointer to the destination path, which should contain a C string,
 *    and be large enough to contain the concatenated resulting path.
 *  - head: C string to be appended. This should not overlap destination.
 *
 * Return:
 *  base is returned.
 *
 * Example:
 *
 *  "/base" + "name" -> "/base/name"
 *  "base/" + "name" -> "base/name"
 *  "./base" + "/name" -> "./base/name"
 *  "" + "name" -> "name"
 *  "base" + "" -> "base"
 *
 */
char *AH5_join_path(char *base, const char *head)
{
    char *endbase = NULL;

    if (head[0] == '\0')
        return base;
    if (base[0] == '\0')
        return strcpy(base, head);

    endbase = base + strlen(base);
    while (isspace(*(--endbase)) && endbase != (base-1));

    if ((*endbase == '/' && head[0] != '/')
            || (*endbase != '/' && head[0] == '/'))
    {
        strcat(AH5_trim_path(base), head);
    }
    else if ((*endbase == '/' && head[0] == '/'))
    {
        strcat(AH5_trim_path(base), head + 1);
    }
    else
    {
        strcat(AH5_trim_path(base), "/");
        strcat(base, head);
    }
    return base;
}


// Remove white char and return; does not allocate new memory
char *AH5_trim_path(char *path)
{
    char *frontp = path - 1;
    char *endp = NULL;
    size_t len = 0;

    if (path == NULL)
        return NULL;

    if (path[0] == '\0')
        return path;

    len = strlen(path);
    endp = path + len;

    // Move the front and back pointers to address the first non-whitespace
    // characters from each end.

    while (isspace(*(++frontp)));
    while (isspace(*(--endp)) && endp != frontp);

    if (path + len - 1 != endp)
        *(endp + 1) = '\0';
    else if (frontp != path &&  endp == frontp)
        *path = '\0';

    // Shift the string so that it starts at path so that if it's dynamically
    // allocated, we can still free it on the returned pointer.  Note the reuse
    // of endp to mean the front of the string buffer now.

    endp = path;
    if (frontp != path)
    {
        while (*frontp) *endp++ = *frontp++;
        *endp = '\0';
    }
    return path;
}


// Check for path validity
char AH5_path_valid(hid_t loc_id, const char *path)
{
    char *temp;
    int i, len, slashes = 0;

    len = (int) strlen(path);
    if (len >= AH5_ABSOLUTE_PATH_LENGTH)
        return AH5_FALSE;

    temp = strdup(path);
    for (i = (int) strlen(path); i > 0; i--)
    {
        if (temp[i] == '/')
        {
            if ((len - i) > AH5_ELEMENT_NAME_LENGTH)
            {
                free(temp);
                return AH5_FALSE;
            }
            len = i;
            temp[i] = '\0';
            slashes++;  /* count number of slashes excluding the first one */
        }
    }

    if ((len + 1) > AH5_ELEMENT_NAME_LENGTH)
    {
        free(temp);
        return AH5_FALSE;
    }

    if (strcmp(path, ".") == 0)
    {
        if (!H5Iis_valid(loc_id))
        {
            free(temp);
            return AH5_FALSE;
        }
    }
    else
    {
        if(H5Lexists(loc_id, temp, H5P_DEFAULT) != AH5_TRUE)
        {
            free(temp);
            return AH5_FALSE;
        }
    }

    i = 1;
    while (slashes > 0)
    {
        while (temp[i] != '\0')
            i++;
        temp[i] = '/';
        slashes--;
        if(H5Lexists(loc_id, temp, H5P_DEFAULT) != AH5_TRUE)
        {
            free(temp);
            return AH5_FALSE;
        }
    }
    free(temp);
    return AH5_TRUE;
}


// Add element to aset; allocates new memory!!!
AH5_set_t AH5_add_to_set(AH5_set_t aset, char *aelement)
{
    hsize_t id = 0;

    if (!AH5_index_in_set(aset, aelement, &id))
    {
        aset.values[aset.nb_values] = strdup(aelement);  // malloc!!!
        aset.nb_values++;
    }
    return aset;
}


// Return the index of first occurrence of an element
int AH5_index_in_set(AH5_set_t aset, char *aelement, hsize_t *index)
{
    int present = AH5_FALSE;
    hsize_t i;

    for (i = 0; i < aset.nb_values; i++)
    {
        if (strcmp(aelement, aset.values[i]) == 0)
        {
            *index = i;
            present = AH5_TRUE;
            break;
        }
    }
    return present;
}


// Read children names of an object
AH5_children_t AH5_read_children_name(hid_t file_id, const char* path)
{
    H5G_info_t ginfo;
    AH5_children_t children;
    hsize_t i, j = 0;
    ssize_t size;
    hid_t group_id;
    char temp[AH5_ELEMENT_NAME_LENGTH], *temp2;

    children.childnames = NULL;
    /*
        - path must exist
        - number of children must be greater than zero
        - element name
            - must be readable
            - must be shorter than AH5_ELEMENT_NAME_LENGTH
            - must NOT be same as "_param"
    */
    if (AH5_path_valid(file_id, path) || strcmp(path, "/") == 0)
    {
        group_id = H5Gopen(file_id, path, H5P_DEFAULT);
        H5Gget_info(group_id, &ginfo);
        if (ginfo.nlinks > 0)
        {
            children.childnames = (char **) malloc((size_t) ginfo.nlinks * sizeof(char *));
            for (i = 0; i < ginfo.nlinks; i++)
            {
                size = H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
                if (size < 0)
                    printf("***** ERROR: Cannot read all children of \"%s\". *****\n\n", path);
                else if (size >= AH5_ELEMENT_NAME_LENGTH)
                {
                    temp2 = (char *) malloc ((size + 1) * sizeof(char));
                    H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp2, size + 1, H5P_DEFAULT);
                    printf("***** ERROR: Maximum name length (%i) exceeded in \"%s/%s\". *****\n\n", AH5_ELEMENT_NAME_LENGTH - 1, path, temp2);
                    free(temp2);
                }
                else
                {
                    H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp, size + 1, H5P_DEFAULT);
                    if (strcmp(temp, "_param") != 0)  // exclude parameterized attributes
                    {
                        children.childnames[j] = (char *) malloc((size + 2) * sizeof(char));
                        strcpy(children.childnames[j], "/");
                        strcat(children.childnames[j++], temp);
                    }
                }
            }
            if (j == 0)
                free(children.childnames);
        }
        H5Gclose(group_id);
    }
    children.nb_children = j;
    return children;
}


// Get last part of a path; does not allocate new memory
char *AH5_get_name_from_path(const char *path)
{
    int i;

    for (i = (int) strlen(path); i > -1; i--)
        if (path[i] == '/')
            break;
    return (char *) path + i + 1;
}

// Get base part of a path; allocates new memory!
char *AH5_get_base_from_path(const char *path)
{
    int i;
    char *rdata, *temp;

    temp = strdup(path);
    for (i = (int) strlen(temp); i > 0; i--)
        if (temp[i] == '/')
            break;
    temp[i] = '\0';
    rdata = strdup(temp);  // strndup wasn't available
    free(temp);
    return rdata;
}

void AH5_print_err_dset(const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", category, path);
}

void AH5_print_err_tble(const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read table \"%s\". *****\n\n", category, path);
}

void AH5_print_err_attr(const char *category, const char *path, const char *attr_name)
{
    printf("\n***** ERROR(%s): Cannot read mandatory attribute \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

void AH5_print_err_path(const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read path \"%s\". *****\n\n", category, path);
}

void AH5_print_err_inv_attr(const char *category, const char *path, const char *attr_name)
{
    printf("\n***** ERROR(%s): Invalid attribute value in \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

void AH5_print_err_func_not_implemented(const char *category, const char *path, const char *func_name)
{
    printf("\n***** ERROR(%s): Problem in %s... function '%s' not implemented yet!\n\n *****\n\n", category, path, func_name);
}

void AH5_print_wrn_attr(const char *category, const char *path, const char *attr_name)
{
    printf("\n***** WARNING(%s): Invalid attribute value in \"%s[@%s]\". *****\n\n", category, path, attr_name);
}


