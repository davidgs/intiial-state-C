#include <curl/curl.h>

int create_bucket(char *access_key, char *bucket_key, char *bucket_name);
int stream_event(char *access_key, char *bucket_key, char *json);