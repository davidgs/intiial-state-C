##[Initial State](http://initialsate.com) C-Library

This is a small C-Library to allow for bucket creation and data streaming to [Initial State](http://initialstate.com)  

Designed to run on the Intel Edison, it uses Libcurl rather than openssl directly as Intel Edison's openssl implementation is insufficient.

It consist of only 2 functions defined as follows:

    int create_bucket(char *access_key, char *bucket_key, char *bucket_name);
    
and 

    int stream_event(char *access_key, char *bucket_key, char *json);
    
Both functions return 0 on success or 1 on failure. Libcurl will print out error messages for you.