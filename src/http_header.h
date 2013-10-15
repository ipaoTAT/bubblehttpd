#ifndef HTTP_HEADER_H
#define	HTTP_HEADER_H

#include "response.h"

#define	FILE_OK			200
#define	BAD_REQUEST		400
#define	FILE_FORBIDEN		403			/* there are no access permission*/
#define FILE_NO_FOUND		404			/* file not found on server */
#define	UNALLOW_METHOD		405			/* un allow http request method*/
#define FILE_TOO_LARGE		413			/* file is too large */
#define	URI_TOO_LONG		414			/*  */
#define	UNSUPPORT_CONTENT_TYPE	415
#define	SERVER_ERROR		500
#define	UNSUPPORT_HTTP_VERSION	505



#define ALLOW				"Allow:GET\r\n"	/* the server allow GET request method*/
#define	SERVER				"Server:MiniHttpd/Linux Alpha\r\n"

int set_send_data(http_res_t *res);

#endif

