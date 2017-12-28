#ifndef GETDATA_H
#define GETDATA_H

int http_get(const char *url, const char *filename);
json_t *http_get_json_data(const char *url);

#endif
