/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1997,99 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/boa-asp/src/read.c#2 $*/

#include "boa.h"
#include <stdio.h>
#include <string.h>
#include "../../lib/libtcapi.h"

/*
 * Name: read_header
 * Description: Reads data from a request socket.  Manages the current
 * status via a state machine.  Changes status from READ_HEADER to
 * READ_BODY or WRITE as necessary.
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 */

int read_header(request * req)
{
    int bytes, buf_bytes_left;
    char *check, *buffer;
		char *pstr;
		char str_lang[6];
		char str_type[4] = {0};
		int	nIndex = 1;

    check = req->client_stream + req->parse_pos;
    buffer = req->client_stream;
    bytes = req->client_stream_pos;

#ifdef VERY_FASCIST_LOGGING
    if (check < (buffer + bytes)) {
        buffer[bytes] = '\0';
        log_error_time();
        fprintf(stderr, "1) %s:%d - Parsing headers (\"%s\")\n",
                __FILE__, __LINE__, check);
    }
#endif

	/* Paul add 2013/3/7, for retrieve Language type from HTTP header */
	tcapi_get("LanguageSwitch_Entry", "Type", str_type);
	if(strlen(str_type))
		nIndex = atoi(str_type);

	if(nIndex == 0) /* Language never been set before, start checking */
	{
		tcapi_get("WebCurSet_Entry", "detected_lang_type", str_type);
		if(strlen(str_type))
			nIndex = atoi(str_type);

		if(nIndex == 0) { // Language never been detect before, start checking
			memset(str_lang, 0, sizeof(str_lang));
			if(pstr = strstr(check,"Accept-Language:"))
			{
				//fprintf(stderr, "2) %s:%d - ********************** (Accept-Language: found at [%d])\n",
	      //          __FILE__, __LINE__, pstr-check+1);

				strncpy (str_lang, &check[pstr-check+17], 6);
				str_lang[5] = '\0';

				int i, len = strlen(str_lang);

				for(i=0; i<len; ++i)
				{
					if(isupper(str_lang[i])){
						str_lang[i] = tolower(str_lang[i]);
					}
				}

				//fprintf(stderr, "6) %s:%d - ********************** (Accept-Language: converted to lower [%s])\n",
	      //          __FILE__, __LINE__, str_lang);

				if(!strcmp(str_lang, "en-us"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "1");
				}
				else if(!strcmp(str_lang, "ru-ru"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "14");
				}
				else if(!strcmp(str_lang, "fr-fr"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "9");
				}
				else if(!strcmp(str_lang, "de-at") || !strcmp(str_lang, "de-li") || !strcmp(str_lang, "de-lu") || !strcmp(str_lang, "de-de") || !strcmp(str_lang, "de-ch"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "6");
				}
				else if(!strcmp(str_lang, "cs-cz"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "4");
				}
				else if(!strcmp(str_lang, "pl-pl"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "13");
				}
				else if(!strcmp(str_lang, "zh-tw") || !strcmp(str_lang, "zh-hk") || !strcmp(str_lang, "zh-mo")) //Modify for Macau
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "18");
				}
				else if(!strcmp(str_lang, "zh-cn") || !strcmp(str_lang, "zh-sg")) //Modify for Singapore
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "3");
				}
				else if(!strcmp(str_lang, "ms-my") || !strcmp(str_lang, "ms-bn"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "11");
				}
				else if(!strcmp(str_lang, "th-th"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "16");
				}
				else if(!strcmp(str_lang, "tr-tr"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "17");
				}
				else if(!strcmp(str_lang, "da-dk"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "5");
				}
				else if(!strcmp(str_lang, "fi-fi"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "8");
				}
				else if(!strcmp(str_lang, "nb-no") || !strcmp(str_lang, "nn-no"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "12");
				}
				else if(!strcmp(str_lang, "sv-fi") || !strcmp(str_lang, "sv-se"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "15");
				}
				else if(!strcmp(str_lang, "pt-br"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "2");
				}
				else if(!strcmp(str_lang, "es-ec") || !strcmp(str_lang, "es-py") || !strcmp(str_lang, "es-pa") || !strcmp(str_lang, "es-ni") || !strcmp(str_lang, "es-gt"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "7");
				}
				else if(!strcmp(str_lang, "es-do") || !strcmp(str_lang, "es-es") || !strcmp(str_lang, "es-hn") || !strcmp(str_lang, "es-ve") || !strcmp(str_lang, "es-pr"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "7");
				}
				else if(!strcmp(str_lang, "es-ar") || !strcmp(str_lang, "es-bo") || !strcmp(str_lang, "es-us") || !strcmp(str_lang, "es-co") || !strcmp(str_lang, "es-cr"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "7");
				}
				else if(!strcmp(str_lang, "es-uy") || !strcmp(str_lang, "es-pe") || !strcmp(str_lang, "es-cl") || !strcmp(str_lang, "es-mx") || !strcmp(str_lang, "es-sv"))	
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "7");
				}
				else if(!strcmp(str_lang, "it-it") || !strcmp(str_lang, "it-ch"))
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "10");
				}
				else if(!strcmp(str_lang, "uk-ua")) /* Paul add 2013/12/4, for Ukrainian support*/
				{
					tcapi_set("WebCurSet_Entry", "detected_lang_type", "19");
				}
				else
				{
					/* Paul add 2013/12/10, for IE11 support */
					str_lang[2] = '\0';

					if(!strcmp(str_lang, "en"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "1");
					}
					else if(!strcmp(str_lang, "ru"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "14");
					}
					else if(!strcmp(str_lang, "fr"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "9");
					}
					else if(!strcmp(str_lang, "de"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "6");
					}
					else if(!strcmp(str_lang, "cs"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "4");
					}
					else if(!strcmp(str_lang, "pl"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "13");
					}
					else if(!strcmp(str_lang, "ms"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "11");
					}
					else if(!strcmp(str_lang, "th"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "16");
					}
					else if(!strcmp(str_lang, "tr"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "17");
					}
					else if(!strcmp(str_lang, "da"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "5");
					}
					else if(!strcmp(str_lang, "fi"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "8");
					}
					else if(!strcmp(str_lang, "nb") || !strcmp(str_lang, "nn"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "12");
					}
					else if(!strcmp(str_lang, "sv"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "15");
					}
					else if(!strcmp(str_lang, "pt"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "2");
					}
					else if(!strcmp(str_lang, "es"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "7");
					}
					else if(!strcmp(str_lang, "it"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "10");
					}
					else if(!strcmp(str_lang, "uk"))
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "19");
					}
					else
					{
						tcapi_set("WebCurSet_Entry", "detected_lang_type", "1");
					}
				}
				initandparserfile();
			}
			/*else
			{
				fprintf(stderr, "2) %s:%d - ********************** (Accept-Language: NOT found)\n",
	                __FILE__, __LINE__);
			}*/
		}
	}

    while (check < (buffer + bytes)) {
        switch (req->status) {
        case READ_HEADER:
            if (*check == '\r') {
                req->status = ONE_CR;
                req->header_end = check;
            } else if (*check == '\n') {
                req->status = ONE_LF;
                req->header_end = check;
            }
            break;

        case ONE_CR:
            if (*check == '\n')
                req->status = ONE_LF;
            else if (*check != '\r')
                req->status = READ_HEADER;
            break;

        case ONE_LF:
            /* if here, we've found the end (for sure) of a header */
            if (*check == '\r') /* could be end o headers */
                req->status = TWO_CR;
            else if (*check == '\n')
                req->status = BODY_READ;
            else
                req->status = READ_HEADER;
            break;

        case TWO_CR:
            if (*check == '\n')
                req->status = BODY_READ;
            else if (*check != '\r')
                req->status = READ_HEADER;
            break;

        default:
            break;
        }
#if 0
#ifdef VERY_FASCIST_LOGGING
        log_error_time();
        fprintf(stderr, "status, check: %d, %d\n", req->status, *check);
#endif
#endif
        req->parse_pos++;       /* update parse position */
        check++;

        if (req->status == ONE_LF) {
            *req->header_end = '\0';

            /* terminate string that begins at req->header_line */

            if (req->logline) {
                if (process_option_line(req) == 0) {
                    return 0;
                }
            } else {
                if (process_logline(req) == 0)
                    return 0;
                if (req->simple)
                    return process_header_end(req);
            }
            /* set header_line to point to beginning of new header */
            req->header_line = check;
        } else if (req->status == BODY_READ) {
#ifdef VERY_FASCIST_LOGGING
            int retval;
            log_error_time();
            fprintf(stderr, "%s:%d -- got to body read.\n",
                    __FILE__, __LINE__);
            retval = process_header_end(req);
#else
            int retval = process_header_end(req);
#endif
            /* process_header_end inits non-POST cgi's */

            if (retval && req->method == M_POST) {
                /* for body_{read,write}, set header_line to start of data,
                   and header_end to end of data */
                req->header_line = check;
                req->header_end =
                    req->client_stream + req->client_stream_pos;

                req->status = BODY_WRITE;
                /* so write it */
                /* have to write first, or read will be confused
                 * because of the special case where the
                 * filesize is less than we have already read.
                 */

                /*

                   As quoted from RFC1945:

                   A valid Content-Length is required on all HTTP/1.0 POST requests. An
                   HTTP/1.0 server should respond with a 400 (bad request) message if it
                   cannot determine the length of the request message's content.

                 */

                if (req->content_length) {
                    int content_length;

                    content_length = boa_atoi(req->content_length);
#if 0	//Content-Length is required but don't limit to larger then zero. Sam, 2013/7/3
                    /* Is a content-length of 0 legal? */
                    if (content_length <= 0) {
                        log_error_time();
                        fprintf(stderr, "Invalid Content-Length [%s] on POST!\n",
                                req->content_length);
                        send_r_bad_request(req);
                        return 0;
                    }
#endif
                    if (single_post_limit && content_length > single_post_limit) {
                        log_error_time();
                        fprintf(stderr, "Content-Length [%d] > SinglePostLimit [%d] on POST!\n",
                                content_length, single_post_limit);
                        send_r_bad_request(req);
                        return 0;
                    }
                    req->filesize = content_length;
                    req->filepos = 0;
                    if (req->header_end - req->header_line > req->filesize) {
                        req->header_end = req->header_line + req->filesize;
                    }
                } else {
                    log_error_time();
                    fprintf(stderr, "Unknown Content-Length POST!\n");
                    send_r_bad_request(req);
                    return 0;
                }
            }                   /* either process_header_end failed or req->method != POST */
            return retval;      /* 0 - close it done, 1 - keep on ready */
        }                       /* req->status == BODY_READ */
    }                           /* done processing available buffer */

#ifdef VERY_FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - Done processing buffer.  Status: %d\n",
            __FILE__, __LINE__, req->status);
#endif

    if (req->status < BODY_READ) {
        /* only reached if request is split across more than one packet */

        buf_bytes_left = CLIENT_STREAM_SIZE - req->client_stream_pos;
        if (buf_bytes_left < 1) {
            log_error_time();
            fputs("buffer overrun - read.c, read_header - closing\n",
                  stderr);
            req->status = DEAD;
            return 0;
        }

#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(req->ssl == NULL)
#endif
	{
        bytes = read(req->fd, buffer + req->client_stream_pos, buf_bytes_left);
	}
#if defined(TCSUPPORT_WEBSERVER_SSL)
	else{
		bytes = boa_sslRead(req->ssl, buffer + req->client_stream_pos, buf_bytes_left);
	}
#endif 
        if (bytes < 0) {
            if (errno == EINTR)
                return 1;
            if (errno == EAGAIN || errno == EWOULDBLOCK) /* request blocked */
                return -1;
            /*
               else if (errno == EBADF || errno == EPIPE) {

               req->status = DEAD;
               return 0;
            */
               
#if 0		//lee 9-25
            log_error_doc(req);
            perror("header read");            /* don't need to save errno because log_error_doc does */
#endif
			return 0;
        } else if (bytes == 0) {
            /*
               log_error_time();
               fputs("unexpected end of headers\n", stderr);
             */
            return 0;
        }

        /* bytes is positive */
        req->client_stream_pos += bytes;

#ifdef FASCIST_LOGGING1
        log_error_time();
        req->client_stream[req->client_stream_pos] = '\0';
        fprintf(stderr, "%s:%d -- We read %d bytes: \"%s\"\n",
                __FILE__, __LINE__, bytes,
#ifdef VERY_FASCIST_LOGGING2
                req->client_stream + req->client_stream_pos - bytes);
#else
                "");
#endif
#endif

        return 1;
    }
    return 1;
}

/*
 * Name: read_body
 * Description: Reads body from a request socket for POST CGI
 *
 * Return values:
 *
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 *

 As quoted from RFC1945:

 A valid Content-Length is required on all HTTP/1.0 POST requests. An
 HTTP/1.0 server should respond with a 400 (bad request) message if it
 cannot determine the length of the request message's content.

 */

int read_body(request * req)
{
    int bytes_read, bytes_to_read, bytes_free;

    bytes_free = BUFFER_SIZE - (req->header_end - req->header_line);
    bytes_to_read = req->filesize - req->filepos;

    if (bytes_to_read > bytes_free)
        bytes_to_read = bytes_free;

    if (bytes_to_read <= 0) {
        req->status = BODY_WRITE; /* go write it */
        return 1;
    }

#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(req->ssl == NULL)
#endif
	{ 
	bytes_read = read(req->fd, req->header_end, bytes_to_read);
	}
#if defined(TCSUPPORT_WEBSERVER_SSL)
	else{
		bytes_read = boa_sslRead(req->ssl,req->header_end ,bytes_to_read);
	}
#endif

    if (bytes_read == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            /*
               req->status = BODY_WRITE;
               return 1;
             */
            return -1;
        } else {
            boa_perror(req, "read body");
            return 0;
        }
    } else if (bytes_read == 0) {
        /* this is an error.  premature end of body! */
        log_error_time();
        fprintf(stderr, "%s:%d - Premature end of body!!\n",
                __FILE__, __LINE__);
        send_r_bad_request(req);
        return 0;
    }

    req->status = BODY_WRITE;

#ifdef FASCIST_LOGGING1
    log_error_time();
    fprintf(stderr, "%s:%d - read %d bytes.\n",
            __FILE__, __LINE__, bytes_to_read);
#endif

    req->header_end += bytes_read;

    return 1;
}

/*
 * Name: write_body
 * Description: Writes a chunk of data to a file
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: EOF or error, close it down
 *   1: successful write, recycle in ready queue
 */

int write_body(request * req)
{
    int bytes_written, bytes_to_write = req->header_end - req->header_line;
    if (req->filepos + bytes_to_write > req->filesize)
        bytes_to_write = req->filesize - req->filepos;

    if (bytes_to_write == 0) {  /* nothing left in buffer to write */
        req->header_line = req->header_end = req->buffer;
        if (req->filepos >= req->filesize)
        {
            return init_cgi(req);
        }
        
        /* if here, we can safely assume that there is more to read */
        req->status = BODY_READ;
        return 1;
    }

    bytes_written = write(req->post_data_fd,
                          req->header_line, bytes_to_write);

    if (bytes_written == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return -1;          /* request blocked at the pipe level, but keep going */
        else if (errno == EINTR)
            return 1;
        else if (errno == ENOSPC) {
            /* 20010520 - Alfred Fluckiger */
            /* No test was originally done in this case, which might  */
            /* lead to a "no space left on device" error.             */
            boa_perror(req, "write body"); /* OK to disable if your logs get too big */
            return 0;
        } else {
            boa_perror(req, "write body"); /* OK to disable if your logs get too big */
            return 0;
        }
    }
#ifdef FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - wrote %d bytes. %ld of %ld\n",
            __FILE__, __LINE__,
            bytes_written, req->filepos, req->filesize);
#endif

    req->filepos += bytes_written;
    req->header_line += bytes_written;

    return 1;                   /* more to do */
}
