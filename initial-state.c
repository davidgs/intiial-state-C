#include <string.h>
#include <stdlib.h>
#include "initial-state.h"
#define BUCKET_API "https://groker.initialstate.com/api/buckets"
#define EVENT_API "https://groker.initialstate.com/api/events"

// return 1 on failure, 0 on success
int create_bucket(char *access_key, char *bucket_key, char *bucket_name) {
  CURL *curl;
  CURLcode res;
  struct curl_slist *chunk = NULL;
  char json[1024];
  char tmp[256];
  curl_global_init(CURL_GLOBAL_DEFAULT);

  if (access_key == NULL || bucket_key == NULL) {
    return 1; // MUST have an access key and bucket key
  }
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, BUCKET_API);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    /* Add a custom header */
    sprintf(tmp, "X-IS-AccessKey: %s", access_key);
    chunk = curl_slist_append(chunk, tmp);
    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    sprintf(json, "{\"bucketKey\" : \"%s\", \"bucketName\" : \"%s\"}",
            bucket_key, bucket_name);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return 0;
}

int stream_event(char *access_key, char *bucket_key, char *json) {

  CURL *curl;
  CURLcode res;
  struct curl_slist *chunk = NULL;
  char tmp[1024];
  if (access_key == NULL || bucket_key == NULL) {
    return 1; // MUST have an access key and bucket key
  }

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, EVENT_API);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    /* Add a custom header */
    sprintf(tmp, "X-IS-AccessKey: %s", access_key);
    chunk = curl_slist_append(chunk, tmp);
    sprintf(tmp, "X-IS-BucketKey: %s", bucket_key);
    chunk = curl_slist_append(chunk, tmp);
    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Perform the request, res will get the return code */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* Check for errors */

    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return 0;
}