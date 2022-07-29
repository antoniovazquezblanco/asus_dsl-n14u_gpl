#include <time.h>
#include <curl/curl.h>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlreader.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <fcntl.h>
#include <dirent.h>
//#include <sha.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
//#include <sys/time.h>
//#include <sys/socket.h>
#include "api.h"
#include "data.h"
#include "function.h"

#define BUFSIZE	1024*16
#define UP_QUEUE 0
#define STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES         6000
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3

typedef enum time_status
{
    older,
    newer,
    same,
    new_file
} time_status;


typedef enum rename_type_t
{
    createfolder_action,
    createfile_action,
    rename_action,
    move_action
} rename_type ;

typedef struct server_capacity_tag
{
    int total;
    int used;
} server_capacity;

server_capacity pre_cap;

char *sid = "50001";
Servicegateway sergate;
extern Aaa aaa;
extern char username[256];
extern char password[256];
extern queue_t queue_download;
extern queue_t queue_upload;
extern struct sync_item *down_head;
//extern sync_item_t down_head;
extern struct sync_item *up_head;
//extern int sync_up;
//extern int sync_down;
extern int MySyncFolder;
extern int exit_loop;
//extern int uploading;
//extern int downloading;
extern struct sync_item *from_server_sync_head;
extern sync_item_t up_excep_fail;
//extern sync_item_t dragfolder_recursion_head;
extern sync_item_t download_only_socket_head;
extern int upload_only;
double start_time;
//extern char uploadfile_path[NORMALSIZE];
//extern char downloadfile_path[NORMALSIZE];
extern int server_space_full;
extern int local_space_full;
//extern int init_fail_item;
extern int max_upload_filesize;
extern int copying_file_number;
//extern char otp_key[8];
extern struct asus_config cfg;
extern int IsAccountFrozen;
int MyRecycleID;
extern int IsSyncError;

typedef struct cmp_item
{
    time_status status;
    int id;
    long long size;
}cmp_item_t ;

/*
int my_setsockopt_func(void *clientp, curl_socket_t curlfd, curlsocktype purpose)
{
    printf("entern my_setsockopt_func\n");
    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

     if(setsockopt(curlfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(struct timeval)))
    {
         perror("*********setsockopt send is error:*******");
     }
     else
     {
         printf("*********setsockopt send is ok:*******\n");
     }
     setsockopt(curlfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
}
*/

size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{
   if(exit_loop)
        return -1;

  int len ;
  len = fwrite(ptr, size, nmemb, stream);
  //printf("write len is %d\n",len);
  return len;

}


size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)

{

   if(exit_loop)
       return -1;

  int len;
  len = fread(ptr, size, nmemb, stream);
  //printf("read len is %d\n",len);
  return len;

}

int my_progress_func(char *progress_data,

                     double t, /* dltotal */

                     double d, /* dlnow */

                     double ultotal,

                     double ulnow)

{

  //printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0/t);

#if 0
    int sec;
    double  elapsed = 0;
    elapsed = time(NULL) - start_time;
    sec = (int)elapsed;
    if( sec > 0 )
    {
    //double progress = ulnow*100.0/ultoal;
        if(sec % 5 == 0)
            printf("%s %g / %g (%g %%)\n", progress_data, ulnow, ultotal, ulnow*100.0/ultotal);
    }
#endif

#if 1
    if(exit_loop)
        return -1;



    if(t > 1 && d > 10 && ultotal > 0) // download
        printf("%s %10.0f / %10.0f (%g %%)\n", progress_data, d, t, d*100.0/t);
    else
        printf("%s %10.0f / %10.0f (%g %%)\n", progress_data, ulnow, ultotal, ulnow*100.0/ultotal);



#endif

  return 0;

}

#if 0
struct myprogress {
  double lastruntime;
  CURL *curl;
};

static int progress(void *p,
                    double dltotal, double dlnow,
                    double ultotal, double ulnow)
{
  struct myprogress *myp = (struct myprogress *)p;
  CURL *curl = myp->curl;
  double curtime = 0;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);

  /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
    myp->lastruntime = curtime;
    fprintf(stderr, "TOTAL TIME: %f \r\n", curtime);
  }

  //fprintf(stderr, "UP: %g of %g  DOWN: %g of %g\r\n",
          //ulnow, ultotal, dlnow, dltotal);

  if(dltotal < dlnow){
          fprintf(stderr, "read : %f bytes\n", dlnow);
  }else{
          fprintf(stderr, "%% %.2f processed\n", (dlnow/dltotal)*100);
  }

  if(dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
    return 1;
  return 0;
}
#endif

/*
int if_file_exist(char *filename)
{
    FILE *fp;
    fp = fopen(filename,"r");
    if( NULL == fp )
        return 0;
    fclose(fp);
    return 1;

}*/

size_t write_data(void *ptr,size_t size,size_t nmemb,void *stream)
{
    if( exit_loop == 1)
        return -1;
    int len = fwrite(ptr,size,nmemb,(FILE *)stream);
    return len;
}

size_t read_data(void *ptr,size_t size,size_t nmemb,void *stream)
{
    return fread(ptr,size,nmemb,stream);
}

void
        parseGateway (xmlDocPtr doc, xmlNodePtr cur,Servicegateway *sg)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                sg->status = atoi(key);
                if(sg->status != 0)
                    return;
            }

            if( !(xmlStrcmp(cur->name, (const xmlChar *)"servicegateway")))
            {
                strcpy(sg->gateway,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"liveupdateuri")))
            {
                strcpy(sg->liveupdateuri,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"time")))
            {
                strcpy(sg->time,key);
            }

            xmlFree(key);
            cur = cur->next;
        }
}

int
        parseGetmysyncfolder(xmlDocPtr doc, xmlNodePtr cur,Getmysyncfolder *gf)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gf->status = atoi(key);
            if(gf->status != 0)
                return -1;
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
          gf->id = atoi(key);
        }


        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseGetPersonalSystemFolder(xmlDocPtr doc, xmlNodePtr cur,Getpersonalsystemfolder *gp)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gp->status = atoi(key);
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"script")))
        {
            strcpy(gp->script,key);
        }
        else if (!(xmlStrcmp(cur->name, (const xmlChar *)"folderid")))
        {
             gp->folderid = atoi(key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseGetinfo (xmlDocPtr doc, xmlNodePtr cur,Getinfo *gi)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        //printf("name is %s,key is %s,type is %d\n",cur->name,key,cur->type);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            //xmlFree(key);
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            gi->status = atoi(key);
            if(gi->status != 0)
                return -1;
        }
        if( !(xmlStrcmp(cur->name, (const xmlChar *)"account")))
        {
            gi->account = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"email")))
        {
            strcpy(gi->email,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"regyear")))
        {
            strcpy(gi->regyear,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"language")))
        {
            strcpy(gi->language,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"activateddate")))
        {
            strcpy(gi->activateddate,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"credentialstate")))
        {
            gi->credentialstate = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"usedbackuppc")))
        {
            gi->usedbackuppc = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"backuppc")))
        {
            parseGetinfo(doc,cur,gi);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
            strcpy(gi->backuppc.name,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"createdtime")))
        {
            strcpy(gi->backuppc.createdtime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"package")))
        {
            parseGetinfo(doc,cur,gi);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
            gi->package.id = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
            strcpy(gi->package.display,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"capacity")))
        {
            gi->package.capacity = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"uploadbandwidth")))
        {
            gi->package.uploadbandwidth = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"downloadbandwidth")))
        {
            gi->package.downloadbandwidth = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"upload")))
        {
            gi->package.upload = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"download")))
        {
            gi->package.download = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"concurrentsession")))
        {
            gi->package.concurrentsession = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"maxfilesize")))
        {
            gi->package.maxfilesize = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"sharegroup")))
        {
            gi->package.sharegroup = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"hasencrption")))
        {
            gi->package.hasencrption = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"expire")))
        {
            strcpy(gi->package.expire,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"maxbackuppc")))
        {
            gi->package.maxbackuppc = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"usedcapacity")))
        {
            gi->usedcappacity = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"freecapacity")))
        {
            gi->freecapacity = atoi(key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

static int
        parseAaa (xmlDocPtr doc, xmlNodePtr cur,Aaa *aaa)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        //printf("name=%s,key=%s\n",cur->name,key);

        /* if value is null and node is no children node,not parse */
        if(key == NULL && cur->children == NULL)
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            aaa->status = atoi(key);

            /*if(aaa->status != 0)
            {
            	xmlFree(key);
            	return -1;
            }*/
        }

        if( !(xmlStrcmp(cur->name, (const xmlChar *)"token")))
        {
            strcpy(aaa->token,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"contentrelay")))
        {
            strcpy(aaa->contentrelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"filerelay")))
        {
            strcpy(aaa->filerelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"inforelay")))
        {
            strcpy(aaa->inforelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"jobrelay")))
        {
            strcpy(aaa->jobrelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"rssrelay")))
        {
            strcpy(aaa->rssrelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"searchrelay")))
        {
            strcpy(aaa->searchrelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"webrelay")))
        {
            strcpy(aaa->webrelay,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"package")))
        {
           parseAaa(doc,cur,aaa);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"auxpasswordurl")))
        {
            char *p = oauth_url_unescape(key,NULL);
            strcpy(aaa->auxpasswordurl,p);
            //printf("auxpasswordurl=%s\n",aaa->auxpasswordurl);
            free(p);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseInitbinaryupload (xmlDocPtr doc, xmlNodePtr cur,Initbinaryupload *ibu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            ibu->status = atoi(key);
            //if(ibu->status != 0)
                //return -1;
        }

        if(ibu->status == 0)
        {
            if( !(xmlStrcmp(cur->name, (const xmlChar *)"transid")))
            {
                strcpy(ibu->transid,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"offset")))
            {
                ibu->offset = atoi(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"fileid")))
            {
                ibu->fileid = atoi(key);
            }
        }
        else
        {
            if( !(xmlStrcmp(cur->name, (const xmlChar *)"logmessage")))
            {
                strcpy(ibu->logmessage,key);
            }
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseResumebinaryupload (xmlDocPtr doc, xmlNodePtr cur,Resumebinaryupload *rbu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            rbu->status = atoi(key);
            if(rbu->status != 0)
            {
            	xmlFree(key);
            	return -1;
            }
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int
        parseFinishbinaryupload (xmlDocPtr doc, xmlNodePtr cur,Finishbinaryupload *fbu)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            fbu->status = atoi(key);
            if(fbu->status != 0)
            {
            	xmlFree(key);
            	return -1;
            }
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"fileid")))
        {
            fbu->fileid = atoi(key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

int get_server_item_size(xmlDocPtr doc, xmlNodePtr cur,Browse *browse)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    int folder_size = 0;
    int file_size = 0;
    //browse->filelist = (File **)malloc
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        //printf("name is %s,key is %s,type is %d\n",cur->name,key,cur->type);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            //xmlFree(key);
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            browse->status = atoi(key);
            if(browse->status != 0)
            {
                xmlFree(key);
                return -1;
            }
        }

        if( !(xmlStrcmp(cur->name, (const xmlChar *)"folder")))
        {
            folder_size += 1 ;
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"file")))
        {
            file_size += 1 ;
        }

        xmlFree(key);
        cur = cur->next;
    }

    //printf("folder size is %d,file size is %d\n",folder_size,file_size);

    if(file_size > 0)
    {
        browse->filelist = (File **)malloc(sizeof(File *)*file_size);

        if(browse->filelist == NULL)
        {
            handle_error(S_MEMORY_FAIL,"get_server_item_size");
            return -1;
        }
    }

    if(folder_size > 0)
    {
        browse->folderlist = (Folder **)malloc(sizeof(Folder *)*folder_size);

        if(browse->folderlist == NULL)
        {
            handle_error(S_MEMORY_FAIL,"get_server_item_size");
            my_free(browse->filelist);
            return -1;
        }
    }

    //size[0] = file_size;
    //size[1] = folder_size;
    return 0;
}


static int
        parseBrowse (xmlDocPtr doc, xmlNodePtr cur,Browse *browse)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    int foldernum = browse->foldernumber-1 ;
    int filenum = browse->filenumber-1;
    //get_server_item_size(doc,cur,browse);
    //browse->filelist = (File **)malloc
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        //printf("name is %s,key is %s,type is %d\n",cur->name,key,cur->type);

        if(key == NULL && cur->children == NULL) //if value is null,not parse
        {
            //xmlFree(key);
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            browse->status = atoi(key);
            if(browse->status != 0)
            {
            	xmlFree(key);
                free_server_list(browse);
                return -1;
            }
        }
        if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
        {
            strcpy(browse->scrip,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"parentfolder")))
        {
            //get_server_item_size(doc,cur,browse);
            parseBrowse(doc,cur,browse);

        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
                strcpy(browse->parentfolder.name,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
        {
            //printf("parent folder is %s\n",cur->parent->name);
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"parentfolder")))
                browse->parentfolder.id = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                //(browse->folderlist)[foldernum] = atoi(key);
                (browse->folderlist)[foldernum]->id = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->id = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"folder")))
        {
            browse->foldernumber += 1 ;
            int num = browse->foldernumber -1;

            (browse->folderlist)[num] = (Folder *)malloc(sizeof(Folder));
            if((browse->folderlist)[num] == NULL)
            {
                handle_error(S_MEMORY_FAIL,"parse browse");
                free_server_list(browse);
                return -1;
            }
            memset(browse->folderlist[num],0,sizeof(Folder));
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"treesize")))
        {
            (browse->folderlist)[foldernum]->treesize = atoll(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
                if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->display,key);
                else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->display,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"attribute")))
        {
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"creationtime")))
        {
                if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                {
                    strcpy((browse->folderlist)[foldernum]->attribute.creationtime,key);
                    //printf("creationtime type is folder\n");
                }
                else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                {
                    strcpy((browse->filelist)[filenum]->attribute.creationtime,key);
                    //printf("creationtime type is file\n");
                }
                else
                    printf("creationtime not find folder or file\n");
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastaccesstime")))
        {
                if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->attribute.lastaccesstime,key);
                else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->attribute.lastaccesstime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastwritetime")))
        {
                if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->attribute.lastwritetime,key);
                else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->attribute.lastwritetime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"finfo")))
        {
                if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->attribute.finfo,key);
                else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->attribute.finfo,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"x-timeforsynccheck")))
        {
            if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                strcpy((browse->folderlist)[foldernum]->attribute.xtimeforsynccheck,key);
             else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                 strcpy((browse->filelist)[filenum]->attribute.xtimeforsynccheck,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"x-machinename")))
        {
             if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"folder")))
                strcpy((browse->folderlist)[foldernum]->attribute.xmachinename,key);
             else if( !(xmlStrcmp(cur->parent->parent->name, (const xmlChar *)"file")))
                 strcpy((browse->filelist)[filenum]->attribute.xmachinename,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isencrypted")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->isencrypted = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->isencrypted = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"issharing")))
        {
            (browse->folderlist)[foldernum]->issharing = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isowner")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->isowner = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->isowner = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isbackup")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->isbackup = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->isbackup = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isorigdeleted")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->isorigdeleted = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->isorigdeleted = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"ispublic")))
        {
            if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                (browse->folderlist)[foldernum]->ispublic = atoi(key);
            else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                (browse->filelist)[filenum]->ispublic = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"createdtime")))
        {
                if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->createdtime,key);
                else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->createdtime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"markid")))
        {
                if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                    (browse->folderlist)[foldernum]->markid = atoi(key);
                else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                    (browse->filelist)[filenum]->markid = atoi(key);
        }

        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"metadata")))
        {

                if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"folder")))
                    strcpy((browse->folderlist)[foldernum]->metadata,key);
                else if( !(xmlStrcmp(cur->parent->name, (const xmlChar *)"file")))
                    strcpy((browse->filelist)[filenum]->metadata,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"file")))
        {
            browse->filenumber += 1 ;
            int num = browse->filenumber -1;
            (browse->filelist)[num] = (File *)malloc(sizeof(File));
            if((browse->filelist)[num] == NULL)
            {
                handle_error(S_MEMORY_FAIL,"parse browse");
                free_server_list(browse);
                return -1;
            }
            memset(browse->filelist[num],0,sizeof(File));
            parseBrowse(doc,cur,browse);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"size")))
        {
                (browse->filelist)[filenum]->size = atoll(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"headversion")))
        {
                (browse->filelist)[filenum]->headversion= atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"page")))
        {
            parseBrowse(doc,cur,browse);
        }
        if( !(xmlStrcmp(cur->name, (const xmlChar *)"pageno")))
        {
            browse->page.pageno = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"pagesize")))
        {
            browse->page.pagesize = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"totalcount")))
        {
            browse->page.totalcount= atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"hasnextpage")))
        {
            browse->page.hasnextpage = atoi(key);
        }
        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

void
        parsePropfind (xmlDocPtr doc, xmlNodePtr cur,Propfind *pfind)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                pfind->status = atoi(key);
                if(pfind->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }

            if( !(xmlStrcmp(cur->name, (const xmlChar *)"isencrypt")))
            {
                pfind->isencrypt = atoi(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"size")))
            {
                pfind->size = atoll(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
                pfind->script = atoi(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"type")))
            {
                strcpy(pfind->type,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
            {
                pfind->id = atoi(key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void
        parseCreatefolder (xmlDocPtr doc, xmlNodePtr cur,Createfolder *createfolder)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                createfolder->status = atoi(key);
                if(createfolder->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }


            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               strcpy(createfolder->scrip,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"id")))
            {
                createfolder->id = atoi(key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void
        parseChangeseq (xmlDocPtr doc, xmlNodePtr cur,Changeseq *changeseq)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                changeseq->status = atoi(key);
                if(changeseq->status != 0)
                {
                	xmlFree(key);
                    return;
                }
            }


            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               strcpy(changeseq->scrip,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"changeseq")))
            {
                changeseq->changeseq = atoi(key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}


static int
        parseGetEntryinfo (xmlDocPtr doc, xmlNodePtr cur,Getentryinfo *ge)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        /* if value is null and node is no children node,not parse */
        if(key == NULL && cur->children == NULL)
        {
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
           ge->status = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
        {
           strcpy(ge->scrip,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isfolder")))
        {
           ge->isfolder = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"display")))
        {
           strcpy(ge->display,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"parent")))
        {
           ge->parent = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"headversion")))
        {
           ge->headversion = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"attribute")))
        {
            parseGetEntryinfo(doc,cur,ge);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"creationtime")))
        {
               strcpy(ge->attr.creationtime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastaccesstime")))
        {
             strcpy(ge->attr.lastaccesstime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"lastwritetime")))
        {
            strcpy(ge->attr.lastwritetime,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"finfo")))
        {
            strcpy(ge->attr.finfo,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"x-timeforsynccheck")))
        {
               strcpy(ge->attr.xtimeforsynccheck,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"x-machinename")))
        {
            strcpy(ge->attr.xmachinename,key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"filesize")))
        {
            ge->filesize = atoll(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"treesize")))
        {
            ge->treesize = atoll(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"ishidden")))
        {
            ge->ishidden = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isinfected")))
        {
            ge->isinfected = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isbackup")))
        {
            ge->isbackup = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"isorigdeleted")))
        {
            ge->isorigdeleted = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"ispublic")))
        {
            ge->ispublic = atoi(key);
        }
        else if( !(xmlStrcmp(cur->name, (const xmlChar *)"createdtime")))
        {
            strcpy(ge->createdtime,key);
        }

        xmlFree(key);
        cur = cur->next;
    }
    return 0;
}

void parseOperateEntry(xmlDocPtr doc, xmlNodePtr cur,Operateentry *oe)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                oe->status = atoi(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               strcpy(oe->script,key);
            }


            xmlFree(key);
            cur = cur->next;
        }
}

void parseMoveEntry(xmlDocPtr doc, xmlNodePtr cur,Moveentry *me)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }

            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                me->status = atoi(key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"scrip")))
            {
               strcpy(me->script,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"logmessage")))
            {
               strcpy(me->logmessage,key);
            }

            xmlFree(key);
            cur = cur->next;
        }
}

int StrToHex(char *src,int len)
{
    int h,l,sum;
    int i;
    char str[4] = {0};
    if(len == 1)
    {
       strcpy(str,src);
       sum = atoi(str);
    }
    else if(len == 2)
    {
       strncpy(str,src,1);
       sum = atoi(str)*16;

       memset(str,0,sizeof(str));
       strcpy(str,src+1);
       sum += atoi(str);
   }

    return sum;
}

void parseGetUserState(xmlDocPtr doc, xmlNodePtr cur,Getuserstate *gu)
{
        xmlChar *key;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            //printf("name=%s,key=%s\n",cur->name,key);

            if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
            {
                //xmlFree(key);
                cur = cur->next;
                continue;
            }


            if (!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
            {
                gu->status = atoi(key);
            }
            else if (!(xmlStrcmp(cur->name, (const xmlChar *)"servicestate")))
            {
               parseGetUserState(doc,cur,gu);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"userstate")))
            {    
                gu->userstate = StrToHex(key,strlen(key));
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"extensionstarttime")))
            {
               strcpy(gu->extensionstarttime,key);
            }
            else if( !(xmlStrcmp(cur->name, (const xmlChar *)"extensionendtime")))
            {
               strcpy(gu->extensionendtime,key);
            }

            xmlFree(key);
            cur = cur->next;
        }
        //printf("parse end\n");
}

int parseHTMLTag(htmlDocPtr doc,htmlNodePtr cur)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(key == NULL && cur->children == NULL) //if value is null and no children ,not parse
        {
            //xmlFree(key);
            cur = cur->next;
            continue;
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"body")))
        {
           parseHTMLTag(doc,cur);
        }

        if (!(xmlStrcmp(cur->name, (const xmlChar *)"h1")))
        {
           handle_error(S_HTTP_ERROR,key);
           xmlFree(key);
           break;
        }

        //printf("cur->name is %s,key is %s\n",cur->name,key);

        xmlFree(key);
        cur = cur->next;
    }
}

int myParseHTML(char *name)
{
        htmlNodePtr cur;
        htmlDocPtr          ptr = htmlParseFile( name, "utf-8" );

        if( !ptr ) {
            fprintf( stderr, "unable to parse doc %s\n", name );
        }
        else {
            fprintf( stderr, "got parsed document: %s\n", name );
        }
        xmlCleanupParser();
        xmlMemoryDump();

        cur = xmlDocGetRootElement(ptr);

        if (cur == NULL) {
            printf("%s empty document\n",name);

            xmlFreeDoc(ptr);
            return -1;
        }

        while (cur != NULL) {

            //printf("name is %s\n",cur->name);

            if ((!xmlStrcmp(cur->name, (const xmlChar *)"html")))
            {
                parseHTMLTag(ptr,cur);
            }

            cur = cur->next;
        }
        xmlFreeDoc(ptr);

        return( 0 );

}

int
        parseDoc1(char *docname,void *obj)
{

    xmlDocPtr doc;
    xmlNodePtr cur;

    //if(if_file_exist(docname) == 1)
        doc = xmlParseFile(docname);
    //else
        //doc = xmlParseMemory(docname,strlen(docname));

    if (doc == NULL ) {
        //fprintf(stderr,"%s not parsed successfully. \n",docname);
        printf("%s not parsed successfully. \n",docname);
        return -1;
    }


    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        //fprintf(stderr,"%s empty document\n",docname);
        printf("%s empty document\n",docname);

        xmlFreeDoc(doc);
        return -1;
    }

    //print_element_names(doc,cur);

    //cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"requestservicegateway")))
        {
            parseGateway(doc, cur,(Servicegateway *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"aaa")))
        {
            parseAaa(doc,cur,(Aaa *)obj);
        } 
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"browse")))
        {
            if(get_server_item_size(doc,cur,(Browse *)obj) == -1)
                return -1;
            parseBrowse(doc,cur,(Browse *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"propfind")))
        {
            parsePropfind(doc,cur,(Propfind *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"create")))
        {
            parseCreatefolder(doc,cur,(Createfolder *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getchangeseq")))
        {
            parseChangeseq(doc,cur,(Changeseq *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"initbinaryupload")))
        {
            parseInitbinaryupload(doc,cur,(Initbinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"resumebinaryupload")))
        {
            parseResumebinaryupload(doc,cur,(Resumebinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"finishbinaryupload")))
        {
            parseFinishbinaryupload(doc,cur,(Finishbinaryupload *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getinfo")))
        {
            parseGetinfo(doc,cur,(Getinfo *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getmysyncfolder")))
        {
            parseGetmysyncfolder(doc,cur,(Getmysyncfolder *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getentryinfo")))
        {
            parseGetEntryinfo(doc,cur,(Getentryinfo *)obj);
        }
        else  if( (!xmlStrcmp(cur->name, (const xmlChar *)"remove") ) ||
              !xmlStrcmp(cur->name, (const xmlChar *)"rename") )
        {
            parseOperateEntry(doc,cur,(Operateentry *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"move")))
        {
            parseMoveEntry(doc,cur,(Moveentry *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getuserstate")))
        {
            parseGetUserState(doc,cur,(Getuserstate *)obj);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"getpersonalsystemfolder")))
        {
            parseGetPersonalSystemFolder(doc,cur,(Getpersonalsystemfolder *)obj);
        }

        //else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"asusconfig")))
        //{
          //  parseAsusconfig(doc,cur,(Asusconfig *)obj);
        //}

        cur = cur->next;
    }
    xmlFreeDoc(doc);

    return 0;
}

void
        parseDoc(char *docname)
{

    xmlDocPtr doc;
    xmlNodePtr cur;
    int status;

    doc = xmlParseFile(docname);

    if (doc == NULL ) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return;
    }


    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    //cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"requestservicegateway")))
        {
            //parseGateway(doc, cur);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"aaa")))
        {
            //parseAaa(doc,cur);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"initbinaryupload")))
        {
            //parseInitbinaryupload(doc,cur);
        }
        else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"browse")))
        {
            //parseBrowse(doc,cur,(Browse *)obj);
        }

        cur = cur->next;
    }
    xmlFreeDoc(doc);

    //return status;
}


int sendRequest(char *filename,char *url,char *postdata,char *cookie,char *header)
{
    //printf("send request postdata=%s \n",postdata);

    FILE *fd;
    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
    char cookies[NORMALSIZE];
    char err_message[NORMALSIZE];
    int status;
    struct curl_slist *headers = NULL;

    //printf("open %s file \n",filename);

    fd = fopen(filename,"w");
    if(NULL == fd)
    {
        printf("open %s file fail\n",filename);
        return -1;
    }

    //printf("curl  11 \n");
    curl = curl_easy_init();
    //printf("curl  22 \n");

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,err_message);
    curl_easy_setopt(curl,CURLOPT_FAILONERROR,1);
    //printf("curl  aa \n");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    //curl_easy_setopt(curl,CURLOPT_TIMEOUT,15);


    if(cookie != NULL)
    {
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookie);
    }
    else
    {
        snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
        curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    }

    if(header != NULL)
    {
        headers = curl_slist_append(headers,header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
    }

    //printf("curl  bb \n");
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    if(postdata != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    }

    curl_easy_setopt(curl,CURLOPT_TIMEOUT,90);

    //printf("curl perform start \n");
    if(!exit_loop)
        res = curl_easy_perform(curl);
    //printf("curl perform end \n");

    if( res != CURLE_OK )
    {
        printf("error message is %s \n",err_message);
        //printf("curl error is %s \n",curl_easy_strerror(res));
        //curl_easy_cleanup(curl);
        //return -1;
    }

    if(header)
        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);

    fclose(fd);

      return res;
}

int getServiceGateway(char *username, char *password,Servicegateway *sg)
{
    //printf("enter getservergateway function\n");

    //Servicegateway *sg;
    int status;
    //sg = getb(Servicegateway);

    //if(NULL == sg)
    //{
        //printf("create dynamic memory fail\n");
        //return NULL;
    //}

    memset(sg,0,sizeof(Servicegateway));

    char *url = "https://sp.yostore.net/member/requestservicegateway/";
    char postdata[512];

    snprintf(postdata,512,"<requestservicegateway><userid>%s</userid><password>%s</password><language>zh_TW</language><service>1</service><time>2008/1/1</time></requestservicegateway>",username,password);

    status = sendRequest(gateway_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        //my_free(sg);
        return S_NETWORK_FAIL;
    }

    if( parseDoc1(gateway_xml,sg) == -1)
    {
        //my_free(sg);
        return S_NETWORK_FAIL;
    }
    //else
        //return sg;

#if 0

    printf("service status is %d \n",sg.status);
    printf("service gateway is %s \n",sg.gateway);
    printf("service liveupdateurl is %s \n",sg.liveupdateuri);
    printf("service gateway time %s \n",sg.time);
#endif
   return 0;
}

char *makeAuthorize()
{
    char *header = NULL;
    header = (char *)malloc(sizeof(char*)*1024);
    char header_signature_method[64];
    char header_timestamp[64];
    char header_nonce[64];
    char *header_signature = NULL;
    char prekey[128];
    unsigned long int sec;

    char query_string[1024];
    char *incode_string = NULL;
    char *sha1_string = NULL;

    snprintf(header_signature_method,64,"%s","HMAC-SHA1");
    snprintf(prekey,128,"%s","111C243AC3224439A5C619423B39F7AF");

    sec = time((time_t *)NULL);
    snprintf(header_timestamp,64,"%u",sec);
    snprintf(header_nonce,64,"%u",sec);

    snprintf( query_string,1024,"nonce=%s&signature_method=%s&timestamp=%s",header_nonce,header_signature_method,header_timestamp);
    incode_string = oauth_url_escape(query_string);
    if(NULL == incode_string)
    {
        handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
        return NULL;
    }

    sha1_string = oauth_sign_hmac_sha1(incode_string,prekey);

    if(NULL == sha1_string)
    {
        handle_error(S_SHA1_FAIL,"makeAuthorize");
        my_free(incode_string);
        return NULL;
    }

    header_signature = oauth_url_escape(sha1_string);
    if(NULL == header_signature)
    {
        handle_error(S_URL_ESCAPE_FAIL,"makeAuthorize");
        my_free(incode_string);
        my_free(sha1_string);
        return NULL;
    }

    snprintf(header,1024,"Authorization:signature_method=\"%s\",timestamp=\"%s\",nonce=%s,signature=\"%s\"",header_signature_method,header_timestamp,header_nonce,header_signature);

    my_free(incode_string);
    my_free(sha1_string);
    my_free(header_signature);

    return header;
}



int getToken(char *username, char *password,char *key,int first)
{
    int status;
    char url[256];
    char postdata[512];
    char *header;
    char cookie[512];
    int error_type;
    char msg[512] = {0};
    char filename[512] = {0};
    //struct curl_slist *headers = NULL;
    //Servicegateway *sg;
    memset(&aaa,0,sizeof(Aaa));

    header = makeAuthorize();
    //headers = curl_slist_append(headers,header);

    snprintf(url,256,"https://%s/member/acquiretoken/",sergate.gateway);
    snprintf(cookie,512,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
    snprintf(postdata,512,"<aaa><userid>%s</userid><password>%s</password><time>2008/1/1</time><auxpassword>%s</auxpassword></aaa>",username,password,key);

    //printf("postdata=%s\n",postdata);
    status = sendRequest(token_xml,url,postdata,cookie,header);
    my_free(header);
    //my_free(sg);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return S_NETWORK_FAIL;
    }


#if 1
    if( parseDoc1(token_xml,&aaa) == -1)
        return S_NETWORK_FAIL;

    if(aaa.status != 0 )
    {
       handle_error(aaa.status,"token");

       status = aaa.status;
       error_type = S_ERROR;

       if(status == S_AUTH_FAIL)
       {
           //error_type = S_ERROR;
           strcpy(msg,"Authentication Failed");
       }
       else if(status == S_OTP_AUTH_FAIL)
       {
           //error_type = S_ERROR;
           if(first)
           {
               if(strlen(cfg.otp_key)>0)
               {
                   if(strlen(cfg.captcha)>0)
                       return status;            // when need otp and captcha no write log 13/11/1
                   else
                        strncpy(msg,"Security Code error.WARNING:If the OTP authentication "
                              "Failures reaches 10 times,OTP will be locked,you must visit the "
                              "official website ,OTP can be used again after unlock it",512);
               }
               else
                   strcpy(msg,"OTP athentication failed.Please input Security Code");
           }
           else
               strcpy(msg,"Security Code has expired, please re-enter");

       }
       else if(status == S_OTP_ID_LOCKED)
       {
           //error_type = S_ERROR;
           strncpy(msg,"OTP Services Credential ID is LOCKED state,you must visit "
                  "the official website can be used again after unlock OTP services",512);
       }
       else if(status == S_CAPTCHA_FAIL)
       {
           error_type = S_NEEDCAPTCHA;
           strcpy(msg,"Need to enter the CAPTCHA");
           strcpy(filename,aaa.auxpasswordurl);
       }


       write_log(error_type,msg,filename);

       return aaa.status;
    }
#endif

    return 0;

}

Getinfo *getInfo(char *username,char *server)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char timestamp[MINSIZE];
    int status;
    int sec;
    //Servicegateway *sg;
    Getinfo *gi;

    //sg = getServiceGateway(username,password);

    //if(NULL == sg)
       // return NULL;

    //if( sg->status != 0 )
    //{
        //handle_error(sg->status,"gateway");
        //my_free(sg);
        //return NULL;
    //}

    gi = getb(Getinfo);

    if(NULL == gi)
    {
        //my_free(sg);
        printf("create dynamic memory fail\n");
        return NULL;
    }

    memset(gi,0,sizeof(Getinfo));

    sec = time((time_t *)NULL);
    //sec = time(NULL);

    snprintf(timestamp,MINSIZE,"%d",sec);
    snprintf(url,NORMALSIZE,"https://%s/member/getinfo/",server);
    snprintf(postdata,MAXSIZE,"<getinfo><token>%s</token><userid>%s</userid><time>2008/1/1</time></getinfo>"
            ,aaa.token,username);

    //printf("url is %s,postdata is %s\n",url,postdata);

    status = sendRequest(get_info_xml,url,postdata,NULL,NULL);
    //my_free(sg);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gi);
        return NULL;
    }

    if( parseDoc1(get_info_xml,gi) == -1)
    {
        my_free(gi);
        return NULL;
    }
    else
    {
        /*if(gi->status == 2)
        {
            my_free(gi);
            if(strlen(cfg.otp_key)>0) //support key
            {
                write_log(S_ERROR,"Security Code has expired, please re-enter",NULL);
                return NULL;
            }

            if( getToken(username,password) != -1)
                getInfo(username,server);
            else
                return NULL;
        }*/
        return gi;
    }
}

Getmysyncfolder *getMySyncFolder(char *username)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    int status;
    Getmysyncfolder *gf;
    gf = getb(Getmysyncfolder);

    if(NULL == gf)
        return NULL;

    memset(gf,0,sizeof(Getmysyncfolder));

    snprintf(url,NORMALSIZE,"https://%s/folder/getmysyncfolder/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getmysyncfolder><token>%s</token><userid>%s</userid></getmysyncfolder>"
            ,aaa.token,username);

    status = sendRequest(get_sync_folder_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gf);
        return NULL;
    }

    if( parseDoc1(get_sync_folder_xml,gf) == -1)
    {
        my_free(gf);
        return NULL;
    }
    //else
    //{
        /*
        if(gf->status != 0)
        {
            free(gf);
            gf = NULL;
            handle_error(gf->status,"getmysyncfolder");
            return NULL;
        }
        else
        {
            return gf;
        }
        */
        return gf;
    //}
}

Getpersonalsystemfolder *getPersonalSystemFolder(char *username,char *filename)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    int status;
    Getpersonalsystemfolder *gp;
    gp = getb(Getpersonalsystemfolder);

    if(NULL == gp)
        return NULL;

    memset(gp,0,sizeof(Getpersonalsystemfolder));

    snprintf(url,NORMALSIZE,"https://%s/folder/getpersonalsystemfolder/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getpersonalsystemfolder><token>%s</token><userid>%s</userid><rawfoldername>%s</rawfoldername></getpersonalsystemfolder>"
            ,aaa.token,username,filename);

    status = sendRequest(get_personal_system_folder_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gp);
        return NULL;
    }

    if( parseDoc1(get_personal_system_folder_xml,gp) == -1)
    {
        my_free(gp);
        return NULL;
    }

    return gp;

}

int GetMyRecycleID(char *username,char *filename)
{
    Getpersonalsystemfolder *gp;
    int id = -10;

    while(!exit_loop)
    {
        gp = getPersonalSystemFolder(username,filename);

        if(NULL == gp)
        {
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }
        if(gp->status != 0)
        {
            handle_error(gp->status,"getpersonalsystemfolder");
            my_free(gp);
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        id = gp->folderid;
        my_free(gp);

        break;
    }

    return id;
}

Getuserstate *getUserState(char *user,char *server)
{
    Getuserstate *gu;
    int status;
    char url[256] = {0};
    char postdata[256] = {0};
    char cookie[32] = {0};

    gu = getb(Getuserstate);

    if(NULL == gu)
    {
        printf("create dynamic memory fail\n");
        return NULL;
    }

    memset(gu,0,sizeof(Getuserstate));

    snprintf(cookie,32,"OMNISTORE_VER=1_0; path=/;");
    snprintf(url,256,"https://%s/member/getuserstate/",server);
    snprintf(postdata,256,"<getuserstate><userid>%s</userid><serviceid>1</serviceid></getuserstate>",user);

    status = sendRequest(get_user_state_xml,url,postdata,cookie,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(gu);
        return NULL;
    }

    if( parseDoc1(get_user_state_xml,gu) == -1)
    {
        my_free(gu);
        return NULL;
    }

    return gu;
}

int CheckUserState(char *user,char *server)
{
    int status;
    int ok = 0;
    Getuserstate *gu;
    int result = 0;

    while(!ok && !exit_loop)
    {
      gu = getUserState(user,server);
      if(gu == NULL)
      {
          //usleep(1000*500);
          enter_sleep_time(1000*500,NULL);
          check_network_state();
          continue;
          //break;
      }

      if(gu->status != 0)
      {
          status = gu->status;
          handle_error(status,"getUserState");
          my_free(gu);
          return status;
      }

      ok = 1;

      //gu->userstate = 8;
#ifdef DEBUG
      printf("userstate=%d\n",gu->userstate);
#endif

      if(gu->userstate & FROZEN)   //accout state is FROZEN;
      {
          write_log(S_ERROR,"Your accout is frozen,you can't upload file","");
          result = S_ACCOUNT_FROZEN;
      }
      else if(gu->userstate & CLOSE)
      {
          write_log(S_ERROR,"Your accout is close,please active it","");
          result = S_ACCOUNT_CLOSE;
      }

    }

    return result;

}

Browse *browseFolder(char *username,int id,int issibiling)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char itemofpage[NORMALSIZE];   
    Browse *browse = getb(Browse);

    if( NULL == browse )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(browse,0,sizeof(Browse));

    snprintf(url,NORMALSIZE,"https://%s/folder/browse/",aaa.inforelay);
    snprintf(itemofpage,NORMALSIZE,"%s","<page><pagesize>0</pagesize><enable>0</enable></page>");
    //sprintf(itemoffilter,"%s","<filter><starttime></starttime><endtime></endtime></filter>");
    //sprintf(postdata,"<browse><token>%s</token><language>zh_TW</language><userid>%s</userid><folderid>%d</folderid><computerseq></computerseq><fileext></fileext>%s%s<sortby>0</sortby><sortdirection></sortdirection><issibiling>%d</issibiling></browse>"
    snprintf(postdata,MAXSIZE,"<browse><token>%s</token><language>zh_TW</language><userid>%s</userid><folderid>%d</folderid>%s<issibiling>%d</issibiling></browse>"
            ,aaa.token,username,id,itemofpage,issibiling);

    status = sendRequest(browse_folder_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(browse);
        return NULL;
    }


    /*
    printf("browse status is %d \n",browse.status);
    printf("browse parentfolder name is %s \n",browse.parentfolder.name);
    printf("browse parentfolder id is %d \n",browse.parentfolder.id);
    int k;
    for(k=0;k<browse.foldernumber;k++)
    {
        printf("browse folder[%d] id is %d \n",browse.foldernumber,browse.folderlist[k].id);
    }
    printf("browse page size is %d \n",browse.page.pagesize);
    */
    if(parseDoc1(browse_folder_xml,browse) == -1)
    {
        my_free(browse);
        return NULL;

    }
    else
    {
        return browse;
    }
}

Propfind *checkEntryExisted(char *userid,int parentID,char *filename,char *type)
{
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char entryName[NORMALSIZE];
    int status;
    char *encode = NULL;

    memset(url,0,sizeof(url));
    memset(postdata,0,sizeof(postdata));
    memset(entryName,0,sizeof(entryName));

    Propfind *find = getb(Propfind);

    if( NULL == find )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(find,0,sizeof(Propfind));

    strcpy(entryName,filename);
    encode = oauth_encode_base64(0,entryName);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"checkEntryExisted");
        return NULL;
    }

    snprintf(url,NORMALSIZE,"https://%s/find/propfind/",aaa.inforelay);

    snprintf(postdata,MAXSIZE,"<propfind><token>%s</token><scrip></scrip><userid>%s</userid><parent>%d</parent><find>%s</find><type>%s</type></propfind>"
            ,aaa.token,userid,parentID,encode,type);

    status = sendRequest(propfind_xml,url,postdata,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(find);
        return NULL;
    }


    if(parseDoc1(propfind_xml,find) == -1)
    {
        my_free(find);
        return NULL;

    }
    else
        return find;
}

Createfolder *createFolder(char *username,int parentID,int isencrpted,char *name)
{
    add_sync_item("create_folder_fail",name,up_excep_fail);

    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char at[MAXSIZE];
    unsigned long int sec;
    char timestamp[MINSIZE];
    char *entryName =  NULL;
    char *encode = NULL;
    Createfolder *cf = getb(Createfolder);

    if( NULL == cf )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(cf,0,sizeof(Createfolder));

    //char *p = strchr()
    entryName = parse_name_from_path(name);

    if(entryName == NULL)
    {
    	my_free(cf);
        printf("obtain name fail by parse path");
        return NULL;
    }



    //strcpy(entryName,name);
    //sprintf(entryName,"%s",oauth_url_escape(oauth_encode_base64(0,entryName)));
    //sprintf(entryName,"%s",oauth_encode_base64(0,entryName));
    encode = oauth_encode_base64(0,entryName);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"createFolder");
        return NULL;
    }

    //printf("name base64 is %s \n",entryName);

    sec = time((time_t *)NULL);
    snprintf(timestamp,MINSIZE,"%u",sec);
    snprintf(at,MAXSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,timestamp,timestamp,timestamp);
    snprintf(url,NORMALSIZE,"https://%s/folder/create/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<create><token>%s</token><userid>%s</userid><parent>%d</parent><isencrypted>%d</isencrypted><display>%s</display><attribute>%s</attribute></create>"
            ,aaa.token,username,parentID,isencrpted,encode,at);

    status = sendRequest(create_folder_xml,url,postdata,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(cf);
        my_free(entryName);
        return NULL;
    }


    if(parseDoc1(create_folder_xml,cf) == -1)
    {
        my_free(cf);
        my_free(entryName);
        return NULL;

    }
    else
    {
        del_sync_item("create_folder_fail",name,up_excep_fail);
        my_free(entryName);
        return cf;
    }

    //return 0;

}

Operateentry *renameEntry(char *username,int id,int isencrpted,char *newname,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    char *entryName = NULL;
    Operateentry *oe = getb(Operateentry);

    if( NULL == oe )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(oe,0,sizeof(Operateentry));

    if(isfolder)
    {
        strcpy(type,"folder");
    }
    else
    {
        strcpy(type,"file");
    }

    //sprintf(rename,"../../asuswebstorage/xml/rename_%s.xml",type);
    //strcpy(entryName,newname);
    //sprintf(entryName,"%s",oauth_encode_base64(0,entryName));
    entryName = oauth_encode_base64(0,newname);

    if(NULL == entryName)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"renameEntry");
        return NULL;
    }

    snprintf(url,NORMALSIZE,"https://%s/%s/rename/",aaa.inforelay,type);
    snprintf(postdata,MAXSIZE,"<rename><token>%s</token><userid>%s</userid><id>%d</id><isencrypted>%d</isencrypted><display>%s</display></rename>"
            ,aaa.token,username,id,isencrpted,entryName);

    my_free(entryName);

    //printf("rename_xml is %s\n",rename_xml);
    status = sendRequest(rename_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(oe);
        return NULL;
    }


    if(parseDoc1(rename_xml,oe) == -1)
    {
        my_free(oe);
        return NULL;
    }
    else
        return oe;
}

Moveentry *moveEntry(char *username,int id,char *name,int parentID,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    char entryName[NORMALSIZE];
    char *encode = NULL;
    Moveentry *me = getb(Moveentry);

    if( NULL == me )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(me,0,sizeof(Moveentry));

    if(isfolder)
    {
        strcpy(type,"folder");
    }
    else
    {
        strcpy(type,"file");
    }

    if( NULL == name)
    {
        strcpy(entryName,name);
        encode = oauth_encode_base64(0,entryName);
        if(NULL == encode)
        {
            handle_error(S_ENCODE_BASE64_FAIL,"moveEntry");
            return NULL;
        }
        snprintf(postdata,MAXSIZE,"<move><token>%s</token><userid>%s</userid><id>%d</id><display>%s</display><parent>%d</parent></move>"
                ,aaa.token,username,id,encode,parentID);
    }
    else
    {
        strcpy(entryName,"");
        snprintf(postdata,MAXSIZE,"<move><token>%s</token><userid>%s</userid><id>%d</id><display>%s</display><parent>%d</parent></move>"
                ,aaa.token,username,id,entryName,parentID);
    }


    snprintf(url,NORMALSIZE,"https://%s/%s/move/",aaa.inforelay,type);


    status = sendRequest(move_xml,url,postdata,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(me);
        return NULL;
    }


    if(parseDoc1(move_xml,me) == -1)
    {
        my_free(me);
        return NULL;
    }
    else
        return me;
}

Operateentry *removeEntry(char *username,int id,int ischildonly,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char type[MINSIZE];
    Operateentry *oe = getb(Operateentry);

    if( NULL == oe )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(oe,0,sizeof(Operateentry));

    if(isfolder)
    {
        strcpy(type,"folder");
        snprintf(postdata,MAXSIZE,"<remove><token>%s</token><userid>%s</userid><id>%d</id><ischildonly>%d</ischildonly></remove>"
                ,aaa.token,username,id,ischildonly);

    }
    else
    {
        strcpy(type,"file");
        snprintf(postdata,MAXSIZE,"<remove><token>%s</token><userid>%s</userid><id>%d</id></remove>"
                ,aaa.token,username,id);
    }

    snprintf(url,NORMALSIZE,"https://%s/%s/remove/",aaa.inforelay,type);

    status = sendRequest(remove_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(oe);
        return NULL;
    }


    if(parseDoc1(remove_xml,oe) == -1)
    {
        my_free(oe);
        return NULL;

    }
    else
        return oe;
}

int updateEntryAttribute(char *username,int id,int parentID,int isencrpted,int isfolder)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    char at[MAXSIZE];
    unsigned long int sec;
    char timestamp[MINSIZE];
    char type[MINSIZE];

    if(isfolder)
    {
        strcpy(type,"folder");
        snprintf(postdata,MAXSIZE,"<updateattribute><token>%s</token><userid>%s</userid><folder>%d</folder><parent>%d</parent><isencrypted>%d</isencrypted><attribute>%s</attribute></updateattribute>"
                ,aaa.token,username,id,parentID,isencrpted,at);
    }
    else
    {
        strcpy(type,"file");
        snprintf(postdata,MAXSIZE,"<updateattribute><token>%s</token><userid>%s</userid><folder>%d</folder><attribute>%s</attribute></updateattribute>"
                ,aaa.token,username,id,at);
    }

    sec = time((time_t *)NULL);
    snprintf(timestamp,MINSIZE,"%u",sec);
    snprintf(at,MAXSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,timestamp,timestamp,timestamp);
    snprintf(url,NORMALSIZE,"https://%s/%s/updateattribute/",aaa.inforelay,type);

    status = sendRequest(update_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

Getentryinfo *getEntryInfo(int isfolder,int entryid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    Getentryinfo *ge = getb(Getentryinfo);

    if( NULL == ge )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(ge,0,sizeof(Getentryinfo));

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getentryinfo/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getentryinfo><token>%s</token><isfolder>%d</isfolder><entryid>%d</entryid></getentryinfo>"
            ,aaa.token,isfolder,entryid);

    status = sendRequest(get_entry_info_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(ge);
        return NULL;
    }

    if(parseDoc1(get_entry_info_xml,ge) == -1)
    {
        my_free(ge);
        return NULL;

    }
    else
        return ge;

    //return 0;
}

int getLatestChangeFiles(char *username,int top,int targetroot,int sortdirection)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/file/getlatestchangefiles/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getlatestchangefiles><userid>%s</userid><token>%s</token><top>%d</top><targetroot>%d</targetroot><sortdirection>%d</sortdirection></getlatestchangefiles>"
            ,username,aaa.token,top,targetroot,sortdirection);

    status = sendRequest(get_change_files_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getLatestUploads(char *username,int top,int targetroot,int sortdirection)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/file/getlatestuploads/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getlatestuploads><userid>%s</userid><token>%s</token><top>%d</top><targetroot>%d</targetroot><sortdirection>%d</sortdirection></getlatestuploads>"
            ,username,aaa.token,top,targetroot,sortdirection);

    status = sendRequest(get_uploads_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int setEntryMark(int isfolder,int entryid,int markid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/setentrymark/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<setentrymark><token>%s</token><isfolder>%d</isfolder><entryid>%d</entryid><markid>%d</markid></setentrymark>"
            ,aaa.token,isfolder,entryid,markid);

    status = sendRequest(set_mark_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

int getShareCode(char *username,int entryType,int entryID,char *password,int actionType)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getsharecode/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getsharecode><token>%s</token><script></script><userid>%s</userid><entrytype>%d</entrytype><entryid>%d</entryid><password>%s</password><actiontype>%d</actiontype></getsharecode>"
            ,aaa.token,username,entryType,entryID,password,actionType);

    status = sendRequest(get_share_code_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int deleteShareCode(char *username,int entryType,int entryID,char *password)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/deletesharecode/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<deletesharecode><token>%s</token><script></script><userid>%s</userid><entrytype>%d</entrytype><entryid>%d</entryid><password>%s</password></deletesharecode>"
            ,aaa.token,username,entryType,entryID,password);

    status = sendRequest(del_share_code_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

int getSharedEntries(char *username,int kind,int pagesize,int sortby,int sortdirection,char *firstentrybound)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/getsharedentries/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getsharedentries><token>%s</token><userid>%s</userid><kind>%d</kind><pagesize>%d</pagesize><sortby>%d</sortby><sortdirection>%d</sortdirection><firstentrybound>%s</firstentrybound></getsharedentries>"
            ,aaa.token,username,kind,pagesize,sortby,sortdirection,firstentrybound);

    status = sendRequest(get_share_entry_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }

    return 0;
}

/*getSharedFiles API is nousing*/
#if 0
int getSharedFiles(char *username,int count)
{
    char *infofilename = "../../asuswebstorage/xml/aaa.xml";
    char *getsharedfiles_filename = "../../asuswebstorage/xml/getsharedfiles.xml";

    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    /*obtain token and inforelay*/
    parseDoc(infofilename);

    sprintf(url,"https://%s/fsentry/getsharedfiles/",aaa.inforelay);
    sprintf(postdata,"<getsharedfiles><token>%s</token><userid>%s</userid><count>%d</count></getsharedfiles>"
            ,aaa.token,username,count);

    sendRequest(getsharedfiles_filename,url,postdata,NULL,NULL);

    return 0;
}
#endif

int checkPassword(char *username,char *suri)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/checkpassword/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<checkpassword><token>%s</token><userid>%s</userid><suri>%s</suri></checkpassword>"
            ,aaa.token,username,suri);

    status = sendRequest(check_pwd_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int comparePassword(char *username,int isfolder,int ffid,char *password)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];

    snprintf(url,NORMALSIZE,"https://%s/fsentry/comparepassword/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<comparepassword><token>%s</token><userid>%s</userid><isfolder>%d</isfolder><ffid>%d</ffid><passwd>%s</passwd></comparepassword>"
            ,aaa.token,username,isfolder,ffid,password);

    status = sendRequest(cmp_pwd_xml,url,postdata,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

Changeseq *getChangeSeq(int folderid)
{
    int status;
    char url[NORMALSIZE];
    char postdata[MAXSIZE];
    Changeseq *cs = getb(Changeseq);

    if( NULL == cs )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(cs,0,sizeof(Changeseq));

    snprintf(url,NORMALSIZE,"https://%s/folder/getchangeseq/",aaa.inforelay);
    snprintf(postdata,MAXSIZE,"<getchangeseq><token>%s</token><scrip></scrip><folderid>%d</folderid></getchangeseq>"
            ,aaa.token,folderid);

    //printf("url is %s,postdat is %s\n",url,postdata);
    status = sendRequest(get_change_seq_xml,url,postdata,NULL,NULL);
    //printf("status is %d\n",status);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(cs);
        return NULL;
    }

    if(parseDoc1(get_change_seq_xml,cs) == -1)
    {
        my_free(cs);
        return NULL;
    }
    else
        return cs;
}

/*encode file by sha512*/
int do_fp(FILE *f,char *checksum);
void pt(unsigned char *md,char*checksum);
int read(int, void *, unsigned int);
int do_fp(FILE *f,char *checksum)
{
    SHA512_CTX c;
    unsigned char md[SHA512_DIGEST_LENGTH];
    int fd;
    int i;
    unsigned char buf[BUFSIZE];

    fd=fileno(f);
    SHA512_Init(&c);
    for (;;)
    {
        if(exit_loop)
            return -1;
        i=read(fd,buf,BUFSIZE);
        if (i <= 0) break;
        SHA512_Update(&c,buf,(unsigned long)i);
    }
    SHA512_Final(&(md[0]),&c);
    pt(md,checksum);

    return 0;
}

void pt(unsigned char *md, char *checksum)
{
    int i;
    char temp[3];

    for (i=0; i<SHA512_DIGEST_LENGTH; i++)
    {
        //printf("%02x",md[i]);
        memset(temp,0,sizeof(temp));
        sprintf(temp,"%02x",md[i]);
        strcat(checksum,temp);
        //sprintf(checksum,"%s%02x",checksum,md[i]);
    }
    //printf("\n");
}

int sha512(char *filename,char *checksum)
//char *sha512(char *filename)
{
    //int i,err=0;
    FILE *IN;

    IN=fopen(filename,"r");
    if (IN == NULL)
    {
       printf("%s can't open \n",filename);
       //return NULL;
       return -1;
    }
    //printf("SHA(%s)= ",filename);
    if(do_fp(IN,checksum) == -1)
    {
        fclose(IN);
        return -1;
    }
    fclose(IN);

    //return checksum;
    return 0;
}

int if_server_space_full(char *filename)
{
    struct stat filestat;
    long long int filesize;
    long long int server_free_capacity;

    if( stat(filename,&filestat) == -1)
    {
        //perror("stat:");
        printf("servr sapce full stat error:%s file not exist\n",filename);
        return -1;
    }

    filesize = filestat.st_size;

    //filesize = filesize / 1024 / 1024;

    /*check server space is full*/
    //server_free_capacity = check_server_space(username);

    while( (server_free_capacity = check_server_space(username)) == -1)
    {
        if(exit_loop)
            return -1;
        //usleep(1000*100);
        enter_sleep_time(1000*500,NULL);
        check_network_state();
    }

    if(server_free_capacity > 0)
    {
        //printf("filesize=%lld,server_free_capacity=%lld\n",filesize,server_free_capacity * 1024 *1024);
        if( filesize > server_free_capacity * 1024 *1024)
        {
            //printf("file size is %lld,server free capacity is %lld \n",filesize,server_free_capacity);
            handle_error(S_SERVER_SPACE_FULL,"upload");
            return 1;
        }
    }

    return 0;
}

Initbinaryupload  *initBinaryUpload(char *filename,int parentID,char *transid)
{


    int status;
    char url[MAXSIZE];
    char checksum[NORMALSIZE];
    char at[NORMALSIZE];
    char *at_encode = NULL;
    char fileCtime[MINSIZE];
    char fileAtime[MINSIZE];
    char fileMtime[MINSIZE];
    char name_raw[NORMALSIZE];
    char *finalname = NULL;
    Initbinaryupload *ibu;
    //long long int server_free_capacity;
    char *encode = NULL;

    memset(checksum,0,sizeof(checksum));

    ibu = getb(Initbinaryupload);

    if( NULL == ibu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(ibu,0,sizeof(Initbinaryupload));

    struct stat filestat;
    long long int filesize;

    if( stat(filename,&filestat) == -1)
    {
        printf("InitBinaryUpload stat error:%s file not exist\n",filename);
        my_free(ibu);
        return NULL;
    }

    filesize = filestat.st_size;

    snprintf(fileCtime,MINSIZE,"%ld",filestat.st_ctime);
    snprintf(fileAtime,MINSIZE,"%ld",filestat.st_atime);
    snprintf(fileMtime,MINSIZE,"%ld",filestat.st_mtime);

    char *p = strrchr(filename,'/');

    if(NULL == p)
    {
        return NULL;
    }

    p++;
    strcpy(name_raw,p);

    encode = oauth_encode_base64(0,name_raw);

    if(NULL == encode)
    {
        handle_error(S_ENCODE_BASE64_FAIL,"initBinaryUpload");
        return NULL;
    }

    finalname = oauth_url_escape(encode);

    if(NULL == finalname)
    {
        handle_error(S_URL_ESCAPE_FAIL,"initBinaryUpload");
        my_free(encode);
        return NULL;
    }

    //printf("final file name is %s ,filesize is %lld\n",finalname,filesize);

    snprintf(at,NORMALSIZE,"<creationtime>%s</creationtime><lastaccesstime>%s</lastaccesstime><lastwritetime>%s</lastwritetime>"
            ,fileCtime,fileAtime,fileMtime);

    at_encode = oauth_url_escape(at);

    if(NULL == at_encode)
    {
        handle_error(S_URL_ESCAPE_FAIL,"initBinaryUpload");
        my_free(encode);
        my_free(finalname);
        return NULL;
    }

    //strcpy(checksum,sha512(filename));
    //sprintf(checksum,"%s",sha512(filename));
    sha512(filename,checksum);
    //printf("checksum is %s\n",sha512(filename));
    //printf("checksum is %s\n",checksum);
    /*obtain token and inforelay*/
    //parseDoc(TOKEN_XML);

    if(NULL == transid)
    {

        snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%d&na=%s&at=%s&fs=%lld&sg=%s&sc=&dis=%s"
            ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,filesize,checksum,sid);
    }
    else
    {
        snprintf(url,MAXSIZE,"https://%s/webrelay/initbinaryupload/?tk=%s&pa=%d&na=%s&at=%s&fs=%lld&sg=%s&sc=&dis=%s&tx=%s"
            ,aaa.webrelay,aaa.token,parentID,finalname,at_encode,filesize,checksum,sid,transid);
    }

    //printf("url: %s,len is %d\n",url,strlen(url));

    status = sendRequest(init_upload_xml,url,NULL,NULL,NULL);

    my_free(encode);
    my_free(finalname);
    my_free(at_encode);

    if( status != 0 )
    {
        my_free(ibu);
        return NULL;
    }


    if(parseDoc1(init_upload_xml,ibu) == -1)
    {
        my_free(ibu);
        return NULL;
    }
    else
    {   //printf("enter finish init binary uplaod, trans id is %s\n",ibu->transid);
        return ibu;
    }

}


Resumebinaryupload *resumeBinaryUpload(char *filename, Initbinaryupload *ibu)
{
    //printf("enter resum upload\n");
    FILE *fd;
    FILE *output;
    CURL *curl;
    CURLcode res;
    char cookies[NORMALSIZE];
    char url[NORMALSIZE];
    struct stat filestat;
    int filesize;
    //size_t filesize;
    double speed_upload, total_time;
    Resumebinaryupload *rbu;
    char *progress_data = "*";
    char range[128];
    int offset;

    offset = ibu->offset;

    memset(range,0,sizeof(range));

    rbu = getb(Resumebinaryupload);

    if( NULL == rbu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(rbu,0,sizeof(Resumebinaryupload));


    if( stat(filename,&filestat) == -1)
    {
        //perror("stat:");
        printf("Resumebinaryupload stat error:%s file not exist\n",filename);
        my_free(rbu);
        return NULL;
    }

    filesize = filestat.st_size;

    //if(offset > 0)
    //{
        //sprintf(range,"%d-%d",offset,filesize);
        //printf(" range is %s \n",range);
    //}
    //else
        printf("######filesize is %d,offset is %d #####\n",filesize,offset);




#if 1
    //parseDoc(TOKEN_XML);

    snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/resumebinaryupload/?tk=%s&tx=%s&dis=%s"
            ,aaa.webrelay,aaa.token,ibu->transid,sid);


    if( NULL==(fd= fopen(resume_upload_xml,"w")) )
    {
        my_free(rbu);
        return NULL;
    }


    if( NULL==(output= fopen(filename,"rb")) )
    {
        my_free(rbu);
        fclose(fd);
        return NULL;
    }

    if(offset > 0)
        fseek(output,offset,0);

    curl = curl_easy_init();

    /*
    res = curl_easy_setopt(curl,CURLOPT_SOCKOPTFUNCTION,my_setsockopt_func);

    if(res != CURLE_OK)
    {
        printf("curl set sockoptfunction option fail\n");
        return NULL;
    }
    else
    {
        printf("curl set sockoptfunction option ok\n");
    }
    */

#if 0
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
    curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
#endif
    curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    //CURLOPT_SSL_VERIFYHOST
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    //curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0);
    //curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl,CURLOPT_READFUNCTION,my_read_func);
    curl_easy_setopt(curl, CURLOPT_READDATA, output);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filesize);
    //curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);
    //curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,30);

    if( offset > 0)
        //curl_easy_setopt(curl, CURLOPT_RANGE,range);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)offset);
#if 1


    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);

    start_time = time(NULL);
#endif
    res = curl_easy_perform(curl);

    //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
    //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);

    if( res != 0 )
    {
        handle_error(res,"curl");
        my_free(rbu);
        fclose(fd);
        fclose(output);
        curl_easy_cleanup(curl);
        return NULL;
    }

    curl_easy_cleanup(curl);

    fclose(fd);
    fclose(output);

    if(parseDoc1(resume_upload_xml,rbu) == -1)
    {
        my_free(rbu);
        return NULL;
    }
    else
        return rbu;


#endif

    //return 0;

}

Finishbinaryupload *finishBinaryUpload(Initbinaryupload *ibu)
{
    int status;
    char url[NORMALSIZE];
    Finishbinaryupload *fbu;

    fbu = getb(Finishbinaryupload);

    if( NULL == fbu )
    {
        printf("create memery error\n");
        return NULL;
    }

    memset(fbu,0,sizeof(Finishbinaryupload));

    snprintf(url,NORMALSIZE,"https://%s/webrelay/finishbinaryupload/?tk=%s&tx=%s&dis=%s"
            ,aaa.webrelay,aaa.token,ibu->transid,sid);

    status = sendRequest(finish_upload_xml,url,NULL,NULL,NULL);

    if( status != 0 )
    {
        handle_error(status,"curl");
        my_free(fbu);
        return NULL;
    }

    if(parseDoc1(finish_upload_xml,fbu) == -1)
    {
        my_free(fbu);
        return NULL;
    }
    else
        return fbu;
}

/*
int check_exist_on_server(char *username,char *filename,int parentID)
{
    Propfind *find;
    char finalname[NORMALSIZE];

    memset(finalname,0,sizeof(finalname));

    char *p = strrchr(filename,'/');

    if(p)
    {
        p++;
        strcpy(finalname,p);
    }

    find = checkEntryExisted(username,parentID,finalname,"system.file");

    if(NULL == find)
    {
        printf("find prop failed\n");
        return -1;
    }
    else if( find->status != 0 )
    {
        handle_error(find->status,"propfind");
        my_free(find);
        return -1;
    }
    else if( !strcmp(find->type,"system.notfound") )
    {
        my_free(find);
        return -1;
    }
    else if( !strcmp(find->type,"system.file") )
    {
        my_free(find);
        return 0;
    }
}*/

int is_local_file_newer(char *filename,int parentID,cmp_item_t *cmp)
{
    /*check file is exist on server*/
    Propfind *find;
    Getentryinfo *getinfo;
    int server_mtime = 0 ;
    int local_mtime = 0;
    int local_atime = 0;
    int local_ctime = 0;
    struct stat filestat;
    char finalname[NORMALSIZE];
    long long int max_filesize;

    if( stat(filename,&filestat) == -1)
    {
        //perror("stat:");
        printf("is_local_file_newer stat error:%s file not exist\n",filename);
        return S_UPLOAD_DELETED;
    }

    max_filesize = (long long int)max_upload_filesize * 1024 *1024;

    if(filestat.st_size > max_filesize)
    {
        printf("file size is %lld\n",filestat.st_size);
        return S_FILE_TOO_LARGE;
    }

    local_mtime = (int)filestat.st_mtime;
    local_atime = (int)filestat.st_atime;
    local_ctime = (int)filestat.st_ctime;

    //printf("local_mtime is %d,atime is %d,ctime is %d\n",local_mtime,local_atime,local_ctime);
    //printf("local_mtime is %s\n",ctime(&filestat.st_mtime));

    cmp->size = filestat.st_size;

    char *p = strrchr(filename,'/');

    if(p)
    {
        p++;
        strcpy(finalname,p);
    }


    find = checkEntryExisted(username,parentID,finalname,"system.file");

    if(NULL == find)
    {
        printf("find prop failed\n");
        return -1;
    }
    else if( find->status != 0 )
    {
        handle_error(find->status,"propfind");
        my_free(find);
        return -1;
    }
    else if( !strcmp(find->type,"system.file") )
    {
        cmp->id = find->id;
        my_free(find);

        getinfo = getEntryInfo(0,cmp->id);

        if(NULL == getinfo)
        {
            printf("get entry info failed\n");
            return -1;
        }
        else if( getinfo->status != 0 )
        {
            handle_error(getinfo->status,"getinfo");
            my_free(getinfo);
            return -1;
        }
        else
        {
             server_mtime = atoi(getinfo->attr.lastwritetime);
             //printf("local_mtime is %d,server_mtime is %d\n",local_mtime,server_mtime);
             my_free(getinfo);
              if(local_mtime > server_mtime)
                  cmp->status = newer;
              else if(local_mtime < server_mtime)
                  cmp->status = older;
              else
                   cmp->status = same;
              //return 0;
         }
    }

    if(server_mtime == 0)  //server not existed this file
        cmp->status = new_file;

    return 0;
}

int uploadFile(char *filename,int parentID,char *transid)
{
#ifdef DEBUG
    printf("#####upload %s is start,parentID is %d#####\n",filename,parentID);
#endif

    char *con_name;
    char action[128];
    char excep_action[128];
    char error_message[512];
    int res;
    int type;
    int error_code = -10;
    server_space_full = 0;
    memset(action,0,sizeof(action));
    cmp_item_t c_item = {-2,-1,0};
    int check_res;
    int IsExistList = 0;

    snprintf(excep_action,128,"up_excep_fail");
    snprintf(action,128,"uploadfile,%d,%s",parentID,transid);
    
    if(IsAccountFrozen)
    {
        check_res = CheckUserState(username,sergate.gateway);

        if(check_res == S_ACCOUNT_FROZEN)
        {
            add_sync_item(excep_action,filename,up_excep_fail);
            return 0;
        }            

        if(check_res == S_ACCOUNT_CLOSE)
        {
           exit_loop = 1;
           return 0;
        }

        if(check_res == S_AUTH_FAIL)
                return 0;

        IsAccountFrozen = 0;
    }

    check_res = is_local_file_newer(filename,parentID,&c_item);

    if(check_res != 0)
    {
        if(check_res == S_FILE_TOO_LARGE)
        {
           add_sync_item(excep_action,filename,up_excep_fail);
           snprintf(error_message,512,"%s filesize too large,max upload file size is %dMB",
                    filename,max_upload_filesize);
#ifdef DEBUG
           printf("%s\n",error_message);
#endif
           write_log(S_ERROR,error_message,"");
        }

        return check_res;
    }

    //if(is_local_file_newer(filename,parentID,&c_item) == -1)
        //return -1;



    printf("compare status is %d\n",c_item.status);

    switch (c_item.status)
    {
    case older: // server file newer
        /*
        if(upload_only)
        {
            return S_
        }
    	con_name = get_confilicted_name(filename,0);
        if(con_name == NULL)
            return -1;
        if(rename(filename,con_name) == -1)
            return -1;
        add_sync_item("rename",con_name,from_server_sync_head);
        res = uploadFile(con_name,parentID,NULL);
        //if(res != 0)
            //handle_upload_fail_code(res,parentID,con_name,path)
        downloadFile(c_item.id,filename,c_item.size,0);
        add_sync_item("createfile",filename,from_server_sync_head);
        my_free(con_name);
        return 0;
        */
        res = handle_rename(parentID,filename,createfile_action,NULL);
        return res;
        break;
    case newer: //local file newer
         removeEntry(username,c_item.id,0,0);
         break;
    case same: //server not exist this file
         return 0;
         break;
    case new_file:
         break;
    default:
         break;
    }




    res = if_server_space_full(filename);

    //printf("get server space end\n");

    if(res == 1)
    {
        write_log(S_ERROR,"server space is not enough","");
        //uploading = 0;
        server_space_full = 1;
        return S_SERVER_SPACE_FULL;
    }
    else if(res == -1)
    {
        snprintf(error_message,512,"upload %s fail,file is not exist",filename);
        write_log(S_ERROR,error_message,"");
        //uploading = 0;
        return S_UPLOAD_DELETED;
    }

#if UP_QUEUE
    queue_entry_t  entry,entry2;

    entry = (queue_entry_t)malloc(sizeof(struct queue_entry));

    if(NULL == entry)
       return -1;
    entry->type = 2;
    entry->id = parentID;
    strcpy(entry->filename,filename);

    queue_enqueue(entry,queue_upload);
#endif

    //type = (downloading == 1) ?  S_DOWNUP : S_UPLOAD;
    //write_log(type,"",filename);
    write_log(S_UPLOAD,"",filename);
    //uploading = 1;
    //memset(uploadfile_path,0,sizeof(uploadfile_path));
    //strcpy(uploadfile_path,filename);

    //print_sync_item(up_head,UPLOAD);

    snprintf(error_message,512,"upload %s fail\n",filename);
    Initbinaryupload *ibu;
    Resumebinaryupload *rbu;
    Finishbinaryupload *fbu;
    char *conflict_name;

    //memset(conflict_name,0,sizeof(conflict_name));



    add_sync_item(excep_action,filename,up_excep_fail);
    res = add_sync_item(action,filename,up_head);
    if(res == -1)
        IsExistList = 1;

    ibu = initBinaryUpload(filename,parentID,transid);

    //printf("initBinaryUpload end\n");

    if( NULL == ibu)
    {
        //uploading = 0;
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)     //add for upload only mySync
                del_sync_item(excep_action,filename,up_excep_fail);
        return -1;
    }
    else if( ibu->status != 0  )
    {
        error_code = ibu->status;
        handle_error(ibu->status,"initbinaryupload");
        //uploading = 0;
        write_log(S_ERROR,error_message,"");

        switch(error_code)
        {
        case S_FILE_TOO_LARGE:
             del_sync_item(action,filename,up_head);
             break;
        case S_NAME_REPEAT:
             break;
        default:
             break;
        }
        
        my_free(ibu);

        return error_code;
    }

    if(ibu->fileid > 0)
    {
    	del_sync_item(excep_action,filename,up_excep_fail);
    	del_sync_item(action,filename,up_head);
        printf("upload %s end!!!!\n",filename);
        my_free(ibu);
        //uploading = 0;
        return 0;
    }


    del_sync_item(action,filename,up_head);
    memset(action,0,sizeof(action));
    snprintf(action,128,"uploadfile,%d,%s",parentID,ibu->transid);
    add_sync_item(action,filename,up_head);

    rbu = resumeBinaryUpload(filename,ibu);

    //printf("resumeBinaryUpload end\n");

    if( NULL == rbu)
    {
    	my_free(ibu);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)   //add for upload only mySync
        	del_sync_item(excep_action,filename,up_excep_fail);
        //uploading = 0;
    	return -1;
    }

    else if( rbu->status !=0 )
    {
        error_code = rbu->status;
        handle_error(rbu->status,"resumebinaryupload");
        my_free(ibu);
        my_free(rbu);
        //uploading = 0;
        write_log(S_ERROR,error_message,"");
        return error_code;
    }



    fbu = finishBinaryUpload(ibu);

    //printf("finishBinaryUpload end\n");

    if(NULL == fbu)
    {
    	my_free(ibu);
    	my_free(rbu);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Upload Fail");
        IsSyncError = 1;
        if(upload_only == 1)    //add for upload only mySync
        	del_sync_item(excep_action,filename,up_excep_fail);
        //uploading = 0;
    	return -1;
    }

    else if(  fbu->status != 0 )
    {
        int res_value = fbu->status;
        handle_error(fbu->status,"finishbinaryupload");
        my_free(ibu);
        my_free(rbu);
        my_free(fbu);
        //uploading = 0;
        write_log(S_ERROR,error_message,"");
        return res_value;
    }

#if UP_QUEUE
    if(!queue_empty(queue_upload))
    {
        entry2 = queue_dequeue(queue_upload);
        free(entry2);
    }
#endif

    del_sync_item(action,filename,up_head);
    del_sync_item(excep_action,filename,up_excep_fail);

    my_free(ibu);
    my_free(rbu);
    my_free(fbu);

#ifdef DEBUG
    printf("upload %s end!!!!\n",filename);
#endif

    //if(downloading)
        //write_log(S_DOWNLOAD,"",downloadfile_path);

    //printf("copying_file_number=%d\n",copying_file_number);

    //if(copying_file_number > 0)
        //write_log(S_SYNC,"","");


    //else
        //write_log(S_SYNC,"","");

    //uploading = 0;

    /*check upload file is ok*/

    /*
    while(check_exist_on_server(username,filename,parentID) == -1)
    {
        sleep(1);
    }
    */

    return 0;
}


int update_local_file_attr(Fileattribute *attr,char *filename)
{
   //Getentryinfo *getinfo;
   struct utimbuf tbuf;
   int server_mtime;
   int server_atime;

   /* change local file time according to server file */
   //getinfo = getEntryInfo(0,id);

   //if(NULL == getinfo)
   //{
       //printf("get entry info failed\n");
       //return -1;
  // }
   //else if( getinfo->status != 0 )
   //{
       //handle_error(getinfo->status,"getinfo");
      // my_free(getinfo);
       //return -1;
   //}
  // else
   //{
       //server_mtime = atoi(getinfo->attr.lastwritetime);
       server_mtime = atoi(attr->lastwritetime);
       //server_atime = atoi(getinfo->attr.lastaccesstime);
       server_atime = atoi(attr->lastaccesstime);
       tbuf.actime = (time_t)server_atime;
       tbuf.modtime = (time_t)server_mtime;
       if(utime(filename,&tbuf) == -1)
       {
           printf("utime %s fail\n",filename);
                //my_free(getinfo);
           return -1;
       }
       //my_free(getinfo);
    //}

   return 0;
}

#if WRITE_DOWNLOAD_TEMP_FILE
int write_download_temp_file(const char *action,const char *name)
{
    FILE *fp;
    fp = fopen(down_item_temp_file,"w");
    if(NULL == fp)
    {
        printf("open %s fail",down_item_temp_file);
        return -1;
    }

    fprintf(fp,"%s,%s\n",action,name);
    fclose(fp);

    return 0;
}
#endif

int check_download_path_exist(const char *filename)
{
    char file_path[512];
    char *p = NULL;
    DIR *pDir = NULL;

    memset(file_path,0,sizeof(file_path));

    p = strrchr(filename,'/');

    if(NULL == p )
    {
        printf(" %s path is not exist\n",filename);
        return -1;
    }

    //p++; // pass '/'

    strncpy(file_path,filename,strlen(filename)-strlen(p));

    //printf("file path is %s\n",file_path);

    pDir = opendir(file_path);

    //printf("pdir end\n");

    if(NULL == pDir)
    {
        //printf("opendir fail\n");
        return -1;
    }
    //else
        //printf("opendir ok\n");

    closedir(pDir);
    return 1;
}

char *get_temp_name(char *fullname)
{
    char *temp_name = NULL;
    char *temp_suffix = ".asus.td";
    int len = 0;
    char path[NORMALSIZE];
    char newfilename[NORMALSIZE];

    memset(path,0,sizeof(path));
    memset(newfilename,0,sizeof(newfilename));
    char *filename = NULL;
    filename =  parse_name_from_path(fullname);
    len = strlen(filename);
    //printf("filename len is %d\n",len);
    //my_free(filename);
    if(len > 247)
    {
        strncpy(path,fullname,strlen(fullname)-len-1);
        strncpy(newfilename,filename,247);
        //printf("newfilename len is %d\n",strlen(newfilename));
        temp_name = (char *)malloc(sizeof(char)*(strlen(path)+strlen("/")+
                     strlen(newfilename)+strlen(temp_suffix)+1));
        memset(temp_name,0,sizeof(temp_name));
        sprintf(temp_name,"%s/%s%s",path,newfilename,temp_suffix);
    }
    else
    {
        temp_name = (char *)malloc(sizeof(char)*(strlen(fullname)+strlen(temp_suffix)+1));
        memset(temp_name,0,sizeof(temp_name));
        sprintf(temp_name,"%s%s",fullname,temp_suffix);
    }

    my_free(filename);

    return temp_name;
}

int IsEntryDeletedFromServer(int fileID,int isfolder)
{
    Getentryinfo *ginfo = NULL;
    ginfo = getEntryInfo(isfolder,fileID);
    if(ginfo == NULL)
    {
        printf("getEntryInfo fail\n");
        return -1;
    }

    if(ginfo->status != 0)
    {

        if(ginfo->status == S_FILE_NOT_EXIST)
        {
#ifdef DEBUG
            printf("file has del from server\n");
#endif
            my_free(ginfo);
            return 1;
        }
        else
        {
            handle_error(ginfo->status,"getEntryInfo before download");
            my_free(ginfo);
            return -1;
        }
    }
    else
    {
        //printf("getEntryInfo ok,parentID=%d\n",ginfo->parent);
        if(ginfo->parent == MyRecycleID)
        {
#ifdef DEBUG
            printf("file has put to Recycle\n");
#endif
            my_free(ginfo);
            return 1;
        }
    }

    return 0;
}

int downloadFile(int fileID,char *filename,long long int size,int ismodify,Fileattribute *attr)
{
#if DOWN_QUEUE
    queue_entry_t  entry,entry2;

    entry = (queue_entry_t)malloc(sizeof(struct queue_entry));

    if(NULL == entry)
        return  -1;
    entry->type = 1;
    entry->id = fileID;
    strcpy(entry->filename,filename);
    entry->size = size;

    queue_enqueue(entry,queue_download);
#endif

    //struct myprogress prog;

    /*
    if(init_fail_item)
        if(check_download_path_exist(filename) == -1)
        {
            printf(" %s path is not exist\n",filename);
            return -1;
        }
    */


    //file_path = parse_path_from_name()
    char *progress_data = "*";
    local_space_full = 0;
    //int if_filename_too_long = 0;
    char *temp_name = NULL;

#ifdef DEBUG
    printf("download %s is start,fildID is %d,size is %lld\n",filename,fileID,size);
#endif

    char action[256];
    int type;
    long http_code = -10;

    memset(action,0,sizeof(action));
    snprintf(action,256,"downloadfile,%d,%lld",fileID,size);

    //printf("action is %s \n",action);



    //printf("bb is start\n");


    /*
    if(NULL == down_head)
    {
        printf("down head is null \n");
    }
    else
        print_sync_item(down_head,DOWNLOAD);
     */
    //print_all_sync_item()



    //print_sync_item(down_head,DOWNLOAD);

    //printf("aa is start\n");

    char url[NORMALSIZE];
    CURL *curl;
    CURLcode res;
    FILE *fd;
    char cookies[NORMALSIZE];
    long long int disk_free_size;
    struct stat filestat;
    int file_exist =  -1;
    int local_file_len = -1;
    char error_message[NORMALSIZE];
    int status;
    char curl_err_msg[NORMALSIZE] = {0};
    int IsExistList = 0 ;

    //printf("stat is end\n");
    status = IsEntryDeletedFromServer(fileID,0);

    if(status == 1)
        return 0;

    if(status == -1)
        return -1;


    disk_free_size = check_disk_space(sync_path);

    if( disk_free_size <= size )
    {
        //if(size > disk_free_size)
        //{
            handle_error(S_LOCAL_SPACE_FULL,"download");
            write_log(S_ERROR,"usb disk space is not enough","");
            //downloading = 0;
            local_space_full = 1;
            return -1;
        //}
    }

    //if_filename_too_long = test_if_filename_too_long(filename);

    temp_name = get_temp_name(filename);

    if(NULL == temp_name)
    {
        handle_error(S_MEMORY_FAIL,"get temp name");
        return -1;
    }

    status = add_sync_item(action,temp_name,down_head);
    if(status == -1)
        IsExistList = 1;

    //printf("temp name is %s \n",temp_name);

    /*
    if(if_filename_too_long)
    {
        temp_name = (char *)malloc(sizeof(char)*(strlen(filename)+1));
        memset(temp_name,0,sizeof(temp_name));
        sprintf(temp_name,"%s",filename);
    }
    else
    {
        temp_name = (char *)malloc(sizeof(char)*(strlen(filename)+strlen(temp_suffix)+1));
        memset(temp_name,0,sizeof(temp_name));
        sprintf(temp_name,"%s%s",filename,temp_suffix);
    }
    */
    //if(if_filename_too_long)




    //printf("stat is start\n");

    if( stat(temp_name,&filestat) == -1)
    {
        //printf("filename %s is not exist\n",filename);
        file_exist = 0 ;
        local_file_len = 0;
    }
    else
    {
        //printf("filename %s is exist\n",filename);
        file_exist = 1;
        local_file_len = filestat.st_size;
    }

    //type = (uploading == 1) ?  S_DOWNUP : S_DOWNLOAD;
    //downloading = 1;
    //memset(downloadfile_path,0,sizeof(downloadfile_path));
    //strcpy(downloadfile_path,filename);

#if WRITE_DOWNLOAD_TEMP_FILE
    write_download_temp_file(action,filename);
#endif

    //write_log(type,"",filename);
    write_log(S_DOWNLOAD,"",filename);

    if(file_exist && ismodify != 1 )
    {
       fd = fopen(temp_name,"ab");
    }
    else
    {
        /*
       if(ismodify == 1)
        {
           if(remove(filename) == -1)
           {
               snprintf(error_message,NORMALSIZE,"remove %s fail \n",filename);
               handle_error(S_REMOVE_LOCAL_FAIL,error_message);
           }
           else
           {
               add_server_action_list("remove",filename,from_server_sync_head);

   #if TREE_NODE_ENABLE
               modify_tree_node(fullname,DirRootNode,DEL_TREE_NODE)
   #endif
           }
         }
         */
       //printf("fopen wb\n");
       fd = fopen(temp_name,"wb");
    }


    if(NULL == fd)
    {
        printf("fopen %s fail,dir is not exist??\n",filename);
        my_free(temp_name);
        return -1;
    }
    //else
        //printf("fopen ok\n");


    //add by alan.No matter file download success or fail,it will be recored into from_server_sync_head
    //use download temp file name can disable
    //add_server_action_list("createfile",filename,from_server_sync_head);

    snprintf(cookies,NORMALSIZE,"OMNISTORE_VER=1_0; path=/;sid=%s;",sid);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/directdownload/?tk=%s&fi=%d&pv=0&u=&of=&rn=&dis=%s"
            ,aaa.webrelay,aaa.token,fileID,sid);
    //sprintf(url,"%s","www.baidu.com");

    //sendRequest(finishbinaryupload_filename,url,NULL,NULL,NULL);
    curl = curl_easy_init();

    //prog.lastruntime = 0;
    //prog.curl = curl;


#if 0
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);
    curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
#endif
    /* resume download file*/
    if(file_exist && ismodify != 1)
    	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE,(curl_off_t)local_file_len);
    curl_easy_setopt(curl,CURLOPT_COOKIE,cookies);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);	
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_write_func);
    curl_easy_setopt(curl,CURLOPT_READFUNCTION,my_read_func);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,1);
    curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,30);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fd);
    //curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,curl_err_msg);

    res = curl_easy_perform(curl);

    if( res != 0 )
    {
        snprintf(error_message,NORMALSIZE,"download %s fail",filename);
        write_log(S_ERROR,error_message,"");
        if(!IsExistList)
            write_trans_excep_log(filename,1,"Download Fail");
        IsSyncError = 1;
        handle_error(res,"curl");
        //downloading = 0 ;
        fclose(fd);
        curl_easy_cleanup(curl);
        my_free(temp_name);
        return -1;
    }


    fclose(fd);

    //add_server_action_list("createfile",filename,from_server_sync_head);

    //curl_easy_getinfo(curl,CURLINFO_HTTP_CONNECTCODE,&http_code);
#if 1
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);

#endif
    curl_easy_cleanup(curl);


#if DOWN_QUEUE
    if(!queue_empty(queue_download))
    {
        entry2 = queue_dequeue(queue_download);
        free(entry2);
    }
#endif

#if WRITE_DOWN_TEMP_FILE
    remove(down_item_temp_file);
#endif
    del_sync_item(action,temp_name,down_head);

#ifdef DEBUG
    //print_all_sync_item(down_head);
#endif

    //if(uploading)
        //write_log(S_UPLOAD,"",uploadfile_path);
    //else
        //write_log(S_SYNC,"","");

    //downloading = 0;

#ifdef DEBUG
    printf("download %s is end\n",filename);
#endif

    if(http_code > 300 && http_code < 600)
    {
        printf("download file http code is %ld\n",http_code);
        if(remove(temp_name) == -1)
        {
           printf("remove downlaod excep %s fail \n",temp_name);
        }
    }
    else
    {
        //if(!if_filename_too_long)
        //{
            if(rename(temp_name,filename) == -1)
            {
                printf("rename %s to %s fail",temp_name,filename);
                my_free(temp_name);
                return -1;
            }
        //}
        //else
            //printf("rename %s to %s ok",temp_name,filename);

        if( update_local_file_attr(attr,filename) == -1)
        {
            //printf("update local file attr fail");
            handle_error(S_UPDATE_ATTR_FAIL,"update file attr");
        }
    }

    my_free(temp_name);
    return 0;
}

int getResizedPhoto(int pfd,int st,int pv)
{
    int status;
    char url[NORMALSIZE];
    char *encode = NULL;
    char query_string[MAXSIZE];
    char query_raw[NORMALSIZE];
    //struct curl_slist *headers = NULL;
    char header[NORMALSIZE];

    snprintf(header,NORMALSIZE,"Last-Modified:Tue, 17 Nov 2009 07:13:19 GMT,ETag:\"1258441999687\"");
    snprintf(query_raw,NORMALSIZE,"pfd=%d,st=%d,pv=%d",pfd,st,pv);
    //printf("######## query string is %s #########\n",query_string);
    //sprintf(query_string,"%s",oauth_encode_base64(0,query_string));
    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getResizePhoto");
       return -1;
   }

    snprintf(query_string,MAXSIZE,"%s.jpg",encode);

    //sprintf(url,"https://%s/webrelay/getresizedphoto/%s?tk=%s&dis=%s&ecd=1"
            //,aaa.webrelay,query_string,aaa.token,sid);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/getresizedphoto/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);

    status = sendRequest(get_resize_photo_xml,url,NULL,NULL,header);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getFullTextCompanion(int fi,int pv,int k)
{
    int status;
    char url[NORMALSIZE];
    char query_raw[NORMALSIZE];
    char *encode = NULL;
    char query_string[MAXSIZE];
    //struct curl_slist *headers = NULL;
    char header[NORMALSIZE];

    /*obtain token and inforelay*/
    //parseDoc(TOKEN_XML);

    snprintf(header,NORMALSIZE,"Last-Modified:Tue, 17 Nov 2009 07:13:19 GMT,ETag:\"1258441999687\"");
    snprintf(query_raw,NORMALSIZE,"fi=%d,pv=%d,k=%d",fi,pv,k);
    //sprintf(query_string,"%s",oauth_encode_base64(0,query_string));
    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getFullTextCompanion");
       return -1;
   }

   snprintf(query_string,MAXSIZE,"%s.txt",encode);

    //sprintf(url,"https://%s/webrelay/getfulltextcompanion/%s?tk=%s&dis=%s&ecd=1"
            //,aaa.webrelay,query_string,aaa.token,sid);
    snprintf(url,MAXSIZE,"https://%s/webrelay/getfulltextcompanion/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);

    status = sendRequest(get_full_txt_xml,url,NULL,NULL,header);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int getVideoSnapshot(int fi,int pv)
{
    int status;
    char url[NORMALSIZE];
    char query_raw[NORMALSIZE];
    char query_string[MAXSIZE];
    char *encode = NULL;

    snprintf(query_raw,NORMALSIZE,"fi=%d,pv=%d",fi,pv);
    //sprintf(query_string,"%s",oauth_encode_base64(0,query_string));

    encode = oauth_encode_base64(0,query_raw);

    if(encode == NULL)
   {
       handle_error(S_ENCODE_BASE64_FAIL,"getVideoSnapshot");
       return -1;
   }
    snprintf(query_string,MAXSIZE,"%s.jpg",encode);

    //sprintf(url,"https://%s/webrelay/getvideosnapshot/%s?tk=%s&dis=%s&ecd=1"
            //,aaa.webrelay,query_string,aaa.token,sid);
    snprintf(url,NORMALSIZE,"https://%s/webrelay/getvideosnapshot/%s/%s?dis=%s&ecd=1"
            ,aaa.webrelay,aaa.token,query_string,sid);


    status = sendRequest(get_video_snapshot_xml,url,NULL,NULL,NULL);

    my_free(encode);

    if( status != 0 )
    {
        handle_error(status,"curl");
        return -1;
    }


    return 0;
}

int handle_error(int code,char *type)
{
    printf("code is %d,type is %s\n",code,type);
    char error_message[NORMALSIZE];

    memset(error_message,0,sizeof(error_message));


    switch (code)
    {
    case S_AUTH_FAIL:

        if( strcmp(type,"gateway") == 0)
        {
#ifdef DEBUG
            printf("username is error \n");
            strcpy(error_message,"username is error");
            //write_system_log("error","username is error");
#endif
        }
        else
        {
#ifdef DEBUG
            printf("auth failed ,please check username and password\n");
            strcpy(error_message,"auth failed ,please check username and password");
            //write_system_log("error","auth failed ,please check username and password");
#endif
        }
        break;

    case CURLE_COULDNT_RESOLVE_HOST:
#ifdef DEBUG
        printf("can't resolve host,please check connection \n");
        strcpy(error_message,"can't resolve host,please check connection");
        //write_system_log("error","can't resolve host,please check connection");
#endif
        break;
    case CURLE_COULDNT_CONNECT:
#ifdef DEBUG
        printf("can't connect to host,please check connection \n");
        strcpy(error_message,"can't connect to host,please check connection");
        //write_system_log("error","can't connect to host,please check connection");
#endif
        break;
    case CURLE_PARTIAL_FILE:
        printf("partial file ,please check transe file size \n");
        break;
    case CURLE_QUOTE_ERROR:
        printf("quote error  \n");
        break;
    case CURLE_HTTP_RETURNED_ERROR:
        printf("http return error code is %d  \n",code);
        break;
    case CURLE_OPERATION_TIMEDOUT:
        printf("connect time out  \n",code);
        break;
    case S_LOCAL_SPACE_FULL:
#ifdef DEBUG
        printf("local space is not enough \n",code);
        strcpy(error_message,"local space is not enough");
#endif
        break;
    case S_SERVER_SPACE_FULL:
#ifdef DEBUG
        printf("server space is not enough \n",code);
        strcpy(error_message,"server space is not enough");
#endif
        break;
    case S_MEMORY_FAIL:
#ifdef DEBUG
        printf("create dynamic memory fail \n",code);
#endif
        strcpy(error_message,"create dynamic memory fail");
        break;

    case S_MKDIR_FAIL:
#ifdef DEBUG
        printf("create folder error,please check disk can write or dir has exist???");
#endif
        //strcpy(error_message,"create folder error,please check disk can write???");
        break;

    case S_UPDATE_ATTR_FAIL:
#ifdef DEBUG
        printf("update attr fail\n");
#endif
        strcpy(error_message,"update file attr fail");
        break;

    case S_OPENDIR_FAIL:
#ifdef DEBUG
        printf("open dir fail\n");
#endif
        strcpy(error_message,"open dir fail");
        break;

    default:
#ifdef DEBUG
        snprintf(error_message,NORMALSIZE,"code is %d,type is %s",code,type);
#endif
        break;
    }
#if SYSTEM_LOG
    write_system_log("error",error_message);
#endif
    //write_log(S_ERROR,error_message,"");

    return 0;
}

int getParentID(char *path)
{
    if(NULL == path)
        return -1;

    char *p = NULL,*p1= NULL;
    char folder_name[512];
    char *cut_path;
    char parse_path[512];
    int parentID = -5;
    Propfind *pfind;
    int sync_path_len;
    const char *split = "/";
    char *p2;

    memset(parse_path,0,sizeof(parse_path));
    strcpy(parse_path,path);

    if( !strcmp(parse_path,sync_path) ) // path is sync root path
    {
        return MySyncFolder;
    }

    sync_path_len = strlen(sync_path);
    cut_path = parse_path;
    cut_path = cut_path + sync_path_len;
    cut_path++ ; // pass first '/'
    //p = cut_path;


    p2 = strtok(cut_path,split);
    int j=0;
    while(p2!=NULL)
    {
        if(j == 0)
            parentID = MySyncFolder;

#ifdef DEBUG
        //printf("check path is %s\n",p2);
#endif

        pfind = checkEntryExisted(username,parentID,p2,"system.folder");

        if(NULL == pfind)
        {
            return -1;
        }

        if(pfind->status != 0)
        {
            my_free(pfind);
            return -1;
        }

        if(strcmp(pfind->type,"system.notfound") == 0)
        {
        	my_free(pfind);
        	return -2;
        }
        /*else if(pfind->status == 2)
        {
            my_free(pfind);
            if(strlen(cfg.otp_key)>0) //support key
            {
                write_log(S_ERROR,"Security Code has expired, please re-enter",NULL);
                return -1;
            }

            getToken(username,password);
            getParentID(path);
        }*/

        parentID = pfind->id;

        //printf("@@@@@ parentID is %d\n",parentID);
        my_free(pfind);
        j++;
        p2 = strtok(NULL,split);
    }

   return parentID;
}

long int check_server_space(char *username)
{
    Getinfo *gi = NULL;
    long long int size;

    gi = getInfo(username,sergate.gateway);

    if(NULL == gi)
        return -1;
    if(gi->status != 0)
    {
        handle_error(gi->status,"getinfo");
        my_free(gi);
        return -1;
    }

    size = gi->freecapacity;
    //size = size * 1024 *1024; // transe MB to byte

    my_free(gi);
    return size;

}

int get_server_space(char *username,server_capacity *cap)
{
    //printf("enter get_server_space\n");
    Getinfo *gi = NULL;
    //long long int size;

    gi = getInfo(username,sergate.gateway);


    if(NULL == gi)
        return -1;
    if(gi->status != 0)
    {
        handle_error(gi->status,"getinfo");
        my_free(gi);
        return -1;
    }

    cap->total = gi->usedcappacity + gi->freecapacity;
    cap->used = gi->usedcappacity;
    //size = size * 1024 *1024; // transe MB to byte

    //printf("totao=%d,used=%d\n",cap->total,cap->used);

    my_free(gi);
    //return size;
    return 0;
}

/*
int write_log(Browse *bs)
{
    char log_name[256];
    int fd;
    int len;

    sprintf(log_name,"%s/%s",log_path,"asuswebstorage");

    if((fd = open(log_name,O_RDWR|O_CREAT|O_TRUNC, 0777))<0)
    {
        perror("Open log error\n");
        return -1;
    }
    else
    {
        len = write(fd,bs,sizeof(Browse));
        close(fd);
        return len;
    }
}
*/

int write_log(int status, char *message, char *filename)
{
    Log_struc log_s;
    FILE *fp;
    int mount_path_length;
    server_capacity cap;

    if(status == S_SYNC || status == S_UPLOAD)
    {
        if(get_server_space(username,&cap) != -1)
        {
            pre_cap.total = cap.total;
            pre_cap.used = cap.used;
        }
        else
        {
            cap.total = pre_cap.total;
            cap.used = pre_cap.used;
        }
    }

    mount_path_length = strlen(mount_path);

    memset(&log_s,0,LOG_SIZE);

    log_s.status = status;

    fp = fopen(general_log,"w");

    if(fp == NULL)
    {
        printf("open %s error\n",general_log);
        return -1;
    }

    if(log_s.status == S_ERROR)
    {
        //printf("******** status is ERROR *******\n");
        strcpy(log_s.error,message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%u\nUSED_SPACE:%u\n",log_s.status,log_s.error,pre_cap.total,pre_cap.used);

    }
    else if(log_s.status == S_NEEDCAPTCHA)
    {
        //strcpy(log_s.error,message);
        fprintf(fp,"STATUS:%d\nERR_MSG:%s\nTOTAL_SPACE:%u\nUSED_SPACE:%u\nCAPTCHA_URL:%s\n",
                status,message,pre_cap.total,pre_cap.used,filename);
    }
    else if(log_s.status == S_DOWNLOAD)
    {
        //printf("******** status is DOWNLOAD *******\n",log_s.status);
    	strcpy(log_s.path,filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%u\nUSED_SPACE:%u\n",
                log_s.status,mount_path,log_s.path+mount_path_length,pre_cap.total,pre_cap.used);
    }
    else if(log_s.status == S_UPLOAD)
    {
        //printf("******** upload status is UPLOAD *******\n",log_s.status);
        strcpy(log_s.path,filename);
        fprintf(fp,"STATUS:%d\nMOUNT_PATH:%s\nFILENAME:%s\nTOTAL_SPACE:%u\nUSED_SPACE:%u\n",log_s.status,mount_path,log_s.path+mount_path_length,cap.total,cap.used);
    }
    else
    {
        /*
        if (log_s.status == S_INITIAL)
            printf("******** other status is INIT *******\n",log_s.status);
        else
            printf("******** other status is SYNC *******\n",log_s.status);
        */
        if (log_s.status == S_SYNC)
            fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%u\nUSED_SPACE:%u\n",log_s.status,cap.total,cap.used);
        else
            fprintf(fp,"STATUS:%d\nTOTAL_SPACE:%u\nUSED_SPACE:%u\n",log_s.status,pre_cap.total,pre_cap.used);

    }

    fclose(fp);
}

int write_system_log(char *action,char *message)
{
    //printf("wirte system log start\n");
    struct stat buf;
    FILE *fp;
    //char mode[4];
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    char *ctime = asctime(timeinfo);

    if( ctime[ strlen(ctime)-1 ] == '\n')
        ctime[ strlen(ctime)-1 ] = '\0';

    //memset(mode,0,sizeof(mode));
    //printf("stat before\n");

    if( stat(system_log,&buf) == -1)
    {
        //strcpy(mode,"w");
        //printf("file %s not exist\n",system_log);
        fp = fopen(system_log,"w");
    }
    else
    {
        //printf("file %s exist\n",system_log);
        fp = fopen(system_log,"a");
    }

    //printf("oepn fp\n");

    //fp = fopen(system_log,mode);

     if(NULL == fp)
     {
         printf("open %s failed\n",system_log);
         return -1;
     }

     fprintf(fp,"%s asuswebstorage [action]:%s [message]:%s\n",ctime,action,message);
     fclose(fp);

     /*wiret temp file*/
#if 0
     fp = fopen(temp_file,"w");

     if(NULL == fp)
     {
         printf("open %s failed\n",temp_file);
         return -1;
     }

     fprintf(fp,"%s\n",filename);

     fclose(fp);
#endif

     //printf("wirte system log end\n");

     return 0;

}

int write_confilicted_log(char *prename, char *confilicted_name)
{
    struct stat buf;
    FILE *fp;

    if( stat(confilicted_log,&buf) == -1)
    {
        fp = fopen(confilicted_log,"w");
    }
    else
    {
        fp = fopen(confilicted_log,"a");
    }

     if(NULL == fp)
     {
         printf("open %s failed\n",system_log);
         return -1;
     }

     fprintf(fp,"%s is download from server,%s is local file and rename from %s\n",prename,confilicted_name,prename);
     fclose(fp);

     return 0;
}

int write_trans_excep_log(char *fullname,int type,char *msg)
{
    FILE *fp = 0;
    //int len;
    char ctype[16] = {0};

    if(type == 1)
        strcpy(ctype,"Error");
    else if(type == 2)
        strcpy(ctype,"Info");
    else if(type == 3)
        strcpy(ctype,"Warning");


    //printf("trans_excep_file=%s\n",trans_excep_file);

    if(access(trans_excep_file,0) == 0)
        fp = fopen(trans_excep_file,"a");
    else
        fp = fopen(trans_excep_file,"w");


    if(fp == NULL)
    {
        printf("open %s fail\n",trans_excep_file);
        return -1;
    }

    //len = strlen(mount_path);
    fprintf(fp,"TYPE:%s\nUSERNAME:%s\nFILENAME:%s\nMESSAGE:%s\n",ctype,username,fullname,msg);
    //fprintf(fp,"ERR_CODE:%d\nMOUNT_PATH:%s\nFILENAME:%s\nRULENUM:%d\n",
                //err_code,mount_path,fullname+len,0);
    fclose(fp);
    return 0;
}

/*
int is_file_copying(char *filename)
{
    unsigned long size1,size2;
    size1 = stat_file(filename);
    usleep(1000*100);
    size2 = stat_file(filename);

    return (size2 - size1);
}*/


#if 1
int sync_all_item(char *dir,int parentID)
{
    struct dirent* ent = NULL;
    Createfolder *cf;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];
    int res_value = -10;

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return S_UPLOAD_DELETED;
    }

    while (NULL != (ent=readdir(dp)))
    {
        int id;
        int status = -10;

        //if(ent->d_name[0] == '.')
            //continue;

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));
        //memset(&createfolder,0,sizeof(Createfolder));

        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if(upload_only && receve_socket)
            return -1;

        if( test_if_dir(fullname) == 1)
        {
            cf = createFolder(username,parentID,0,fullname);
            if(NULL == cf)
            {
                printf("sync_all_item function create folder fail\n");
                fail_flag = 1;
                continue;
            }
            else if(cf->status != 0)
            {
                 fail_flag = 1;
                 snprintf(error_message,NORMALSIZE,"createfolder %s fail on server",ent->d_name);
                 handle_error(cf->status,error_message);
                 res_value = handle_createfolder_fail_code(cf->status,parentID,dir,fullname);
                 if(res_value != 0)
                      fail_flag = 1;
                 my_free(cf);
                 continue;
            }
            else if(cf->status == 0)
            {
                //add_sync_item("createfolder",fullname,dragfolder_recursion_head);
                id = cf->id;
                my_free(cf);
                sync_all_item(fullname,id);
            }
        }
        else
        {
            //if( is_file_copying(fullname) == 0)
            //{
                status = uploadFile(fullname,parentID,NULL);

                if(status != 0)
                {
                   printf("upload %s failed\n",fullname);
                   res_value = handle_upload_fail_code(status,parentID,fullname,dir);
                   if(res_value != 0)
                        fail_flag = 1;
                }
                //add_sync_item("createfile",fullname,dragfolder_recursion_head);
           //}
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}


#endif
int add_all_download_only_socket_list(char *cmd,const char *dir)
{   
    struct dirent* ent = NULL;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return -1;
    }

    add_sync_item(cmd,dir,download_only_socket_head);

    while (NULL != (ent=readdir(dp)))
    {

        //if(ent->d_name[0] == '.')
            //continue;

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));
        //memset(&createfolder,0,sizeof(Createfolder));

        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            add_all_download_only_socket_list("createfolder",fullname);
        }
        else
        {
            add_sync_item("createfile",fullname,download_only_socket_head);
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

int add_all_download_only_dragfolder_socket_list(const char *dir)
{
    struct dirent* ent = NULL;
    char fullname[NORMALSIZE];
    int fail_flag = 0;
    char error_message[NORMALSIZE];

    DIR *dp = opendir(dir);

    if(dp == NULL)
    {
       snprintf(error_message,NORMALSIZE,"opendir %s fail",dir);
       handle_error(S_OPENDIR_FAIL,error_message);
       fail_flag = 1;
       return -1;
    }

    while (NULL != (ent=readdir(dp)))
    {

        //if(ent->d_name[0] == '.')
            //continue;

        if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,".."))
            continue;

        memset(fullname,0,sizeof(fullname));
        memset(error_message,0,sizeof(error_message));
        //memset(&createfolder,0,sizeof(Createfolder));

        snprintf(fullname,NORMALSIZE,"%s/%s",dir,ent->d_name);

        if( test_if_dir(fullname) == 1)
        {
            //add_sync_item("createfolder",fullname,dragfolder_recursion_head);
            add_sync_item("createfolder",fullname,download_only_socket_head);
            add_all_download_only_dragfolder_socket_list(fullname);
        }
        else
        {
            //add_sync_item("createfile",fullname,dragfolder_recursion_head);
            add_sync_item("createfile",fullname,download_only_socket_head);
        }
    }

    closedir(dp);

    return (fail_flag == 1) ? -1 : 0;
}

int handle_rename(int parentID,char *fullname,int type,char *prepath)
{
    Propfind *find = NULL;
    int entryID = -10;
    char *confilicted_name = NULL;
    char *filename = NULL;
    //char *path = NULL;
    char path[512];
    int isfolder = 0;
    char *newfilename = NULL;
    Operateentry *oe = NULL;
    int res_value = -10;
    char newfullname[512];
    //char move_fullname[NORMALSIZE];
    int status = -10;

    //memset(move_fullname,0,sizeof(move_fullname));
    filename = parse_name_from_path(fullname);


    if(NULL == filename)
    {
        handle_error(S_MEMORY_FAIL,"handle_rename parse filename");
        return -1;
    }

    /*
    path = (char *)malloc(sizeof(char)*(strlen(fullname)-strlen(filename)));
    if(NULL == path)
    {
        handle_error(S_MEMORY_FAIL,"handle_rename parse filename");
        free(filename);
        return -1;
    }
    */
    memset(path,0,sizeof(path));
    strncpy(path,fullname,strlen(fullname)-strlen(filename)-1);

    if(test_if_dir(fullname))
        isfolder = 1;
    else
        isfolder = 0;

    /*
    if(type == move_action)
    {
        snprintf(move_fullname,NORMALSIZE,"%s/%s",prepath,filename);
        confilicted_name = get_confilicted_name(move_fullname,isfolder);
    }
    else
    */
   confilicted_name = get_confilicted_name(fullname,isfolder);

    if(NULL == confilicted_name)
    {
        printf("get %s confilicted name fail\n",fullname);
        my_free(filename);
        //my_free(path);
        return -1;
    }
    //printf(" %s reanme to %s \n",fullname,confilicted_name);
    //strcpy(cur_name,confilicted_name);
    if(upload_only)
    {  
        find = checkEntryExisted(username,parentID,filename,"system.unknown");

        if(NULL == find)
        {
            printf("find prop failed\n");
            my_free(filename);
            //my_free(path);
            my_free(confilicted_name);
            return -1;
        }

        if( find->status != 0 )
        {
            handle_error(find->status,"propfind");
            my_free(filename);
            //my_free(path);
            my_free(confilicted_name);
            my_free(find);
            return -1;
        }

        entryID = find->id;

        if(strcmp(find->type,"system.folder") == 0)
        {
            isfolder = 1;
        }
        else if(strcmp(find->type,"system.file") == 0)
        {
            isfolder = 0;
        }

       newfilename = parse_name_from_path(confilicted_name);
       if(NULL == newfilename)
       {
           handle_error(find->status,"propfind");
           my_free(filename);
           //my_free(path);
           my_free(confilicted_name);
           my_free(find);
           return -1;
       }

       oe = renameEntry(username,entryID,0,newfilename,isfolder) ;
       if(NULL == oe)
       {
           printf("operate rename failed\n");
           handle_error(find->status,"propfind");
           my_free(filename);
           //my_free(path);
           my_free(confilicted_name);
           my_free(find);
           my_free(newfilename);
           return -1;
       }

       if( oe->status != 0 )
       {
           handle_error(oe->status,"renameEntry");
           /*
           newfullname = (char *)malloc(sizeof(char)*(strlen(path)+strlen("/")+strlen(newfilename)));
           if(NULL == newfullname)
           {
               printf("operate rename failed\n");
               handle_error(find->status,"propfind");
               my_free(filename);
               my_free(path);
               my_free(confilicted_name);
               my_free(find);
               my_free(newfilename);
               my_free(oe);
               return -1;
           }
           */
           sprintf(newfullname,"%s/%s",path,newfilename);
           res_value = handle_rename_fail_code(oe->status,parentID,newfullname,path,isfolder);
           if(res_value != 0)
           {
               my_free(filename);
               my_free(confilicted_name);
               my_free(find);
               my_free(newfilename);
               my_free(oe);
               return res_value;
           }
       }
       res_value = upload_entry(fullname,parentID,path);

       my_free(find);
       my_free(newfilename);
       my_free(oe);
    }
    else  //sync
    {
        /*
        if(type == move_action)
        {
            printf("rename %s to %s \n",move_fullname,confilicted_name);
            status = rename(move_fullname,confilicted_name);
        }
        else
        */
        //printf("rename %s to %s \n",fullname,confilicted_name);
        status = rename(fullname,confilicted_name);

        if(status == -1)
        {
            handle_error(S_RENAME_FAIL," rename name");
            my_free(filename);
            //my_free(path);
            my_free(confilicted_name);
            return -1;
        }
        add_sync_item("rename",confilicted_name,from_server_sync_head);
        res_value = upload_entry(confilicted_name,parentID,path);

        switch (type)
        {
        case createfolder_action:
            my_mkdir(fullname);
            add_server_action_list("createfolder",fullname,from_server_sync_head);
            break;
        default:
            break;
        }

    }
    my_free(filename);
    //my_free(path);
    my_free(confilicted_name);

    return res_value;
}

int handle_createfolder_fail_code(int status,int parent_ID,char *path,char* fullname)
{
    int res_value = -10;
    int ID = parent_ID ;
    Createfolder *cf = NULL;
    char *confilicted_name = NULL;
    char error_message[512];
    //rename_type type;
    //char cur_name[512];

    memset(error_message,0,sizeof(error_message));
    //memset(cur_name,0,sizeof(cur_name));

    switch(status)
    {
    case S_DIR_NOT_EXIST: //parent dir not exist
        ID = create_server_folder_r(path);
        if(ID<0)
            return -1;
        res_value = upload_entry(fullname,ID,path);
        //strcpy(cur_name,fullname);
        break;
    case S_NAME_REPEAT:
        //type =  createfolder_action;
        //res_value = handle_rename(parent_ID,fullname,createfolder_action,NULL);
        return 0;
        /*
        confilicted_name = get_confilicted_name(fullname,1);
        if(NULL == confilicted_name)
        {
            printf("get %s confilicted name fail\n",fullname);
            return -1;
        }
        //printf(" %s reanme to %s \n",fullname,confilicted_name);
        //strcpy(cur_name,confilicted_name);
        if(upload_only)
        {
           //renameEntry(username,entry_ID,0,newname,isfolder) ;
        }
        else
        {
            if(rename(fullname,confilicted_name) == -1)
            {
                handle_error(S_RENAME_FAIL," rename name");
                return -1;
            }
            add_sync_item("rename",confilicted_name,from_server_sync_head);
            res_value = upload_entry(confilicted_name,parent_ID,path);
            my_mkdir(fullname);
            add_server_action_list("createfolder",fullname,from_server_sync_head);
        }
        */

        break;
    default:
        break;
    }

    /*
    if(ID > 0)
    {

        cf = createFolder(username,ID,0,cur_name);
        if(cf == NULL)
        {
            res_value = -1;
        }
        else if(cf->status == 0)
        {
            res_value = 0 ;
            my_free(cf);
        }
        else
        {
            res_value = handle_createfolder_fail_code(cf->status,ID,path,cur_name);
            my_free(cf);
        }

        if(status == S_NAME_REPEAT )
        {
            if(mkdir(cur_name,0777) == -1)
            {
                snprintf(error_message,NORMALSIZE,"mkdir %s fail \n",cur_name);
                handle_error(S_MKDIR_FAIL,error_message);
                return -1;
            }
            else
            {
                add_server_action_list("createfolder",cur_name,from_server_sync_head);
#if SYSTEM_LOG
                write_system_log("createfolder",cur_name);
#endif
#if TREE_NODE_ENABLE
                modify_tree_node(cur_name,DirRootNode,ADD_TREE_NODE)
#endif
                res_value = 0;
            }

        }
    }
    else
    {
        res_value = -1;
    }
    */


    return res_value;
}

int write_up_limit_file_log(const char *fullname,const char *msg)
{
    struct stat buf;
    FILE *fp = NULL;

    if( stat(up_limit_file,&buf) == -1)
    {
        fp = fopen(system_log,"w");
    }
    else
    {
        fp = fopen(up_limit_file,"a");
    }

    if(NULL == fp)
    {
        printf("open %s fail\n",up_limit_file);
        return -1;
    }

    fprintf(fp,"%s\n",fullname);
    fclose(fp);

    return 0;
}

int handle_upload_fail_code(int status,int parent_ID,char* fullname,const char *path)
{
    int res_value = 0;
    char *up_action = NULL;
    int ID = -10 ;
    int res_upload = -10;
    //char *confilicted_name = NULL;
    //rename_type type;

    switch(status)
    {
    case S_USER_NOSPACE:
        write_log(S_ERROR,"server space is not enough","");
        //uploading = 0;
        server_space_full = 1;
        res_value = 0;
        break;
    case S_SERVER_SPACE_FULL:
        up_action = (char *)malloc(sizeof(char)*256);
        memset(up_action,0,256);
        snprintf(up_action,256,"uploadfile,%d,%s",parent_ID,"none");
        add_sync_item(up_action,fullname,up_head);
        add_sync_item("up_excep_fail",fullname,up_excep_fail);
        my_free(up_action);
        res_value = 0;
        break;
    case S_UPLOAD_DELETED:
    case S_FILE_TOO_LARGE:
         //write_up_limit_file_log(fullname);
         write_trans_excep_log(fullname,1,"Filesize Too Large");
         res_value = 0;
         break;
    case S_TRANS_ID_NOT_EXIST:
        res_value = 0 ;
        break;
    case S_DIR_NOT_EXIST:
        ID = create_server_folder_r(path);
        if( -1 == ID )
        {
            res_value = -1;
        }
        else if(ID > 0)
        {
            res_upload = uploadFile(fullname,ID,NULL);
            if(res_upload != 0)
            {
                res_value = handle_upload_fail_code(res_upload,ID,fullname,path);
            }
        }
        break;
 case S_NAME_REPEAT:
        //type = createfile_action;
        res_value = handle_rename(parent_ID,fullname,createfile_action,NULL);
        /*
        confilicted_name = get_confilicted_name(fullname,0);
        if(NULL == confilicted_name)
        {
            printf("get %s confilicted name fail\n",fullname);
            return -1;
        }
        //strcpy(cur_name,confilicted_name);
        //printf(" %s reanme to %s \n",fullname,cur_name);
        if( rename(fullname,confilicted_name) == -1)
        {
            handle_error(S_RENAME_FAIL,"renmae");
            return -1;
        }
        add_sync_item("rename",confilicted_name,from_server_sync_head);
        res_upload = uploadFile(confilicted_name,ID,NULL);
        if(res_upload != 0)
        {
            res_value = handle_upload_fail_code(res_upload,ID,confilicted_name,path);
        }
        */
        break;
   case -1:
        //write_trans_excep_log(fullname,1,"Upload Fail");
        res_value = -1;
        break;
    default:
        break;
    }
    return res_value;
}

int handle_delete_fail_code(int status)
{
    int res_value = 0;

    switch(status)
    {
    case S_FILE_NOT_EXIST:
    case S_DIR_NOT_EXIST:
        res_value = 0;
        break;
    default:
        break;
    }

    return res_value;
}

int handle_rename_fail_code(int status,int parentID,char *fullname,char *path,int isfolder)
{
    int res_value = 0;
    char *confilicted_name = NULL;
    int res_upload = -10;
    int ID = parentID ;
    char error_message[512];
    //char cur_name[512];

    memset(error_message,0,sizeof(error_message));
    //memset(cur_name,0,sizeof(cur_name));

    switch(status)
    {
    case S_FILE_NOT_EXIST:
        //strcpy(cur_name,fullname);
        res_value = upload_entry(fullname,parentID,path);
        break;
    case S_DIR_NOT_EXIST:
        ID = create_server_folder_r(path);
        if(ID < 0)
            return -1;
        //strcpy(cur_name,fullname);
        res_value = upload_entry(fullname,ID,path);
        break;
    case S_NAME_REPEAT:
        res_value = handle_rename(parentID,fullname,rename_action,NULL);
        /*
        confilicted_name = get_confilicted_name(fullname,isfolder);
        if(NULL == confilicted_name)
        {
            printf("get %s confilicted name fail\n",fullname);
            return -1;
        }
        //strcpy(cur_name,confilicted_name);
        //printf(" %s reanme to %s \n",fullname,cur_name);
        if( rename(fullname,confilicted_name) == -1)
        {
            handle_error(S_RENAME_FAIL,"renmae");
            return -1;
        }
        add_sync_item("rename",confilicted_name,from_server_sync_head);
        res_value = upload_entry(confilicted_name,parentID,path);
        */
        break;
    default:
        break;
    }
  return res_value;
}

int handle_move_fail_code(int status,char *path,char *fullname,int parentID,char *prepath,int entryID,int isfolder)
{
    int res_value = 0;
    Moveentry *me = NULL;
    char *confilicted_name = NULL;
    int res_upload = -10;
    int ID = parentID ;
    //char error_message[512];
    //char cur_name[512];

    switch(status)
    {
    case S_FILE_NOT_EXIST:
        res_value = upload_entry(fullname,parentID,path);
        break;
    case S_NAME_REPEAT:
        //printf("move fullname is %s\n",fullname);
        removeEntry(username,entryID,0,isfolder);
        res_value = handle_rename(parentID,fullname,move_action,prepath);
        /*
        confilicted_name = get_confilicted_name(fullname,isfolder);
        if(NULL == confilicted_name)
        {
            printf("get %s confilicted name fail\n",fullname);
            return -1;
        }
        //strcpy(cur_name,confilicted_name);
        //printf(" %s reanme to %s \n",fullname,cur_name);
        if(rename(fullname,confilicted_name) == -1)
        {
            handle_error(S_RENAME_FAIL,"renmae");
            return -1;
        }
        add_sync_item("rename",confilicted_name,from_server_sync_head);
        res_value = upload_entry(confilicted_name,parentID,path);
        */
        break;
    case S_DIR_NOT_EXIST:
    case S_ILLEGAL_OPERATION:   //parent dir delete
        ID = create_server_folder_r(path);
        if(ID<0)
            return -1;
        res_value = upload_entry(fullname,ID,path);
        break;
    default:
        break;

    }

    return res_value;

}

int create_server_folder_r(const char *path)
{
    if(NULL == path)
        return -1;

    char *p = NULL,*p1= NULL;
    char folder_name[512];
    char *cut_path;
    char parse_path[512];
    int parentID = -5;
    Propfind *pfind;
    int sync_path_len;
    const char *split = "/";
    char *p2;
    Createfolder *cf = NULL;
    char fullname[512];

    memset(parse_path,0,sizeof(parse_path));
    memset(fullname,0,sizeof(fullname));
    strcpy(parse_path,path);
    strcpy(fullname,sync_path);

    if( !strcmp(parse_path,sync_path) ) // path is sync root path
    {
        return MySyncFolder;
    }

    sync_path_len = strlen(sync_path);
    cut_path = parse_path;
    cut_path = cut_path + sync_path_len;
    cut_path++ ; // pass first '/'
    //p = cut_path;


    p2 = strtok(cut_path,split);
    int j=0;
    while(p2!=NULL)
    {
        strcat(fullname,"/");
        strcat(fullname,p2);

        if(j == 0)
            parentID = MySyncFolder;

#ifdef DEBUG
        //printf("check path is %s\n",p2);
#endif

        pfind = checkEntryExisted(username,parentID,p2,"system.folder");

        if(NULL == pfind)
        {
            return -1;
        }
        else if(pfind->status == 0)
        {
            if(strcmp(pfind->type,"system.notfound") == 0)
            {
                    my_free(pfind);
                    //printf("fullname is %s\n",fullname);
                    cf = createFolder(username,parentID,0,fullname);
                    if(NULL == cf)
                    {
                        printf("cf is null\n");
                        return -1;
                    }
                    else if(cf->status == 0)
                    {
                        parentID = cf->id;
                        my_free(cf);
                    }
                    else
                    {
                        handle_error(cf->status,"createfolder");
                        my_free(cf);
                        return -1;
                    }
            }
            else
            {
                parentID = pfind->id;
                my_free(pfind);
            }
        }
        else
        {
            handle_error(pfind->status,"propfind");
            my_free(pfind);
            return -1;
        }

        /*
        else if(pfind->status == 2)
        {
            my_free(pfind);
            getToken(username,password);
            getParentID(path);
        }
        */


        j++;
        p2 = strtok(NULL,split);
    }

   return parentID;
}

int upload_entry(char *fullname,int parent_ID,char *path)
{
    int status = -10;
    int entry_ID = -10;
    Createfolder *cf = NULL;
    int res_value = 0;
    //sprintf(fullname,"%s/%s",path,newname);
#if TREE_NODE_ENABLE
    modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif
    if(test_if_dir(fullname))
    {
        cf = createFolder(username,parent_ID,0,fullname);
        if(NULL == cf)
            return -1;
        else if( cf->status != 0 )
        {
            handle_error(cf->status,"propfind");
            res_value = handle_createfolder_fail_code(cf->status,parent_ID,path,fullname);
            my_free(cf);
            return res_value;
        }
        else
        {
            entry_ID = cf->id;
            my_free(cf);
            res_value = sync_all_item(fullname,entry_ID);
            return res_value;
        }
    }
    else //entry is file
    {
        status = uploadFile(fullname,parent_ID,NULL);
        
        if(status != 0)
        {
            printf("upload %s failed\n",fullname);
            res_value = handle_upload_fail_code(status,parent_ID,fullname,path);
            return res_value;
        }
    }

    return res_value;
}
