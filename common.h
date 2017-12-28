#ifndef COMMON_H
#define COMMON_H

char *json_string_to_ptr(json_t *jcomp, char *field);
json_t *get_jdata_from(const char *u, const char *u_prefix, int input_value, const char *u_suffix);
int find_jdata_size(json_t *jroot, json_t *jdata);

#endif
