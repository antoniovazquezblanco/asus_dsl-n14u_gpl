#include <stdint.h>
#include <pthread.h>

#ifndef DATA_H
#define DATA_H

#define MINSIZE 64
#define NORMALSIZE 512
#define MAXSIZE 1024
#define BUFSIZE	1024*16

#define ASUSWEBSTORAGE_SYNCFOLDER "MySyncFolder"
#define ASUSWEBSTORAGE 10

#define S_INITIAL		70
#define S_SYNC			71
#define S_DOWNUP		72
#define S_UPLOAD		73
#define S_DOWNLOAD		74
#define S_STOP			75
#define S_ERROR			76
#define S_NEEDCAPTCHA           77
#define S_AUTHFAIL              78
#define S_OTPFAIL               79

#define LOG_SIZE                sizeof(struct LOG_STRUCT)

//#ifndef IPKG
#define CONFIG_PATH "/tmp/smartsync/asuswebstorage/config/Cloud.conf"
//#else
//#define CONFIG_PATH "/opt/etc/Cloud.conf"
//#endif

#ifdef IPKG
#define NOTIFY_PATH "/tmp/notify/usb"

#define GET_NVRAM_SCRIPT_1 "/tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram"
#define SH_GET_NVRAM_SCRIPT_1 "sh /tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram"
#define NVRAM_PATH_1 "/tmp/smartsync/asuswebstorage/config/asuswebstorage_tmpconfig"

#define GET_NVRAM_SCRIPT_2 "/tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram_link"
#define SH_GET_NVRAM_SCRIPT_2 "sh /tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram_link"
#define NVRAM_PATH_2 "/tmp/smartsync/asuswebstorage/config/link_internet"
#endif


//#ifndef IPKG
//#define SHELL_FILE  "/tmp/asuswebstorage_write_nvram"
//#define NVRAM_TOKENFILE "asuswebstorage_tokenfile"
//#endif


/*servergetway struct*/
typedef struct SERVICEGATEWAY
{
    int status;
    char gateway[MINSIZE];
    char liveupdateuri[NORMALSIZE];
    char time[MINSIZE];
}Servicegateway;

/*get tokey struct*/
typedef struct PACKAGE
{
    int id;
    char display[NORMALSIZE];
    int capacity;
    int uploadbandwidth;
}Package;

typedef struct AAA
{
    int status;
    char token[MINSIZE];
    char contentrelay[MINSIZE];
    char filerelay[MINSIZE];
    char inforelay[MINSIZE];
    char jobrelay[MINSIZE];
    char rssrelay[MINSIZE];
    char searchrelay[MINSIZE];
    char webrelay[MINSIZE];
    Package package;
    char auxpasswordurl[256];
}Aaa;

/*initbinaryupload struct*/
typedef struct INITBINARYUPLOAD
{
    int status;
    char transid[NORMALSIZE];
    int offset;
    char logmessage[NORMALSIZE];
    int fileid;
}Initbinaryupload;

/* resume binary upload*/
typedef struct RESUMEBINARYUPLOAD
{
    int status;
}Resumebinaryupload;

/* finish binary upload */
typedef struct FINISHBINARYUPLOAD
{
    int status;
    int fileid;
}Finishbinaryupload;

/*browse folder struct*/

typedef struct PARENTFOLDER
{
    char name[NORMALSIZE];
    int id;
}Parentfolder;

typedef struct PAGE
{
   int pageno;
   int pagesize;
   int totalcount;
   int hasnextpage;
}Page;

typedef struct ATTRIBUTE
{
    char creationtime[MINSIZE];
    char lastaccesstime[MINSIZE];
    char lastwritetime[MINSIZE];
    char finfo[MINSIZE];
    char xtimeforsynccheck[NORMALSIZE];
    char xmachinename[NORMALSIZE];
}Attribute;

typedef struct FOLDER
{
    int id;
    long long treesize;
    char display[NORMALSIZE];
    Attribute attribute;
    int isencrypted;
    int issharing;
    int isowner;
    int isbackup;
    int isorigdeleted;
    int ispublic;
    char createdtime[MINSIZE];
    int markid;
    char metadata[NORMALSIZE];
    int isdeleted;
}Folder;

typedef struct FILEATTRIBUTE
{
    char creationtime[MINSIZE];
    char lastaccesstime[MINSIZE];
    char lastwritetime[MINSIZE];
    char finfo[MINSIZE];
    char xtimeforsynccheck[NORMALSIZE];
    char xmachinename[NORMALSIZE];
}Fileattribute;

typedef struct FILE
{
    int id;
    int status;
    char display[NORMALSIZE];
    Fileattribute attribute;
    long long size;
    int isencrypted;
    int isowner;
    int isbackup;
    int isorigdeleted;
    int isinfected;
    int ispublic;
    int headversion;
    char createdtime[MINSIZE];
    int markid;
    char metadata[NORMALSIZE];
}File;

typedef struct BROWSE
{
    int status;
    char scrip[MINSIZE];
    Parentfolder parentfolder;
    int foldernumber;
    int filenumber;
    Folder **folderlist;
    File   **filelist;
    Page page;
}Browse;

/*struct local folder struct*/
typedef struct LOCALFOLDER
{
  char name[NORMALSIZE];
  //struct LOCALFOLDER *next;
  //int isfolder;
}Localfolder;

typedef struct LOCALFILE
{
    char name[NORMALSIZE];
    Attribute attribute;
    long long size;
    //struct LOCALFILE *next;
}Localfile;

typedef struct LOCAL
{
 int foldernum;
 int filenum;
 Localfolder **folderlist;
 Localfile   **filelist;
 //Localfolder *folder_head;
 //Localfolder *file_head;
}Local;

//typedef struct
typedef struct LOCALS
{
    Parentfolder parent;
    Local local;
    struct LOCALS *next;
}Locals;

/* find/propfind data struct*/
typedef struct PROPFIND
{
    int status;
    int isencrypt;
    long long size;
    unsigned long script;
    char type[MINSIZE];
    int id;
}Propfind;

/*get change seq*/
typedef struct CHANGESEQ
{
  int status;
  char scrip[MINSIZE];
  unsigned int changeseq;

}Changeseq;

/*createfolder data struct*/
typedef struct CREATEFOLDER
{
    int status;
    char scrip[MINSIZE];
    int id;

}Createfolder;

/* get all items struct*/
typedef struct FOLDERNAME
{
    char name[512];
}Foldername;

typedef struct FOLDERS
{
  int number;
  Foldername folderlist[512];
}Folders;

/* get file and folderid*/
typedef struct ITEMID
{
    int fileID;
    int parentID;
}ItemID;

/*rename struct*/
typedef struct OPERATEENTRY
{
    int status;
    char script[MINSIZE];
}Operateentry;

/*move etnry*/
typedef struct MOVEENTRY
{
    int status;
    char script[MINSIZE];
    char logmessage[NORMALSIZE];
}Moveentry;

/*get info*/
typedef struct BACKUPPC
{
    char name[NORMALSIZE];
    char createdtime[NORMALSIZE];
}Backuppc;

typedef struct FEATURELIST
{

}Featurelist;

typedef struct INFOPACKAGE
{
    int id;
    char display[NORMALSIZE];
    int capacity;
    int uploadbandwidth;
    int downloadbandwidth;
    int upload;
    int download;
    int concurrentsession;
    int maxfilesize;
    int sharegroup;
    int hasencrption;
    char expire[MINSIZE];
    int maxbackuppc;

}Infopackage;



typedef struct GETINFO
{
   int status;
   int account;
   char email[NORMALSIZE];
   char regyear[MINSIZE];
   char language[MINSIZE];
   char activateddate[MINSIZE];
   int credentialstate;
   int usedbackuppc;
   Backuppc backuppc;
   Infopackage package;
   int usedcappacity;
   int freecapacity;
}Getinfo;

/*get MySyncFolder data struct*/
typedef struct GETMYSYNCFOLDER
{
    int status;
    int id;
}Getmysyncfolder;

/*get PersonalSystemFolder data struct*/
typedef struct GETPERSONALSYSTEMFOLDER
{
    int status;
    char script[16];
    int  folderid;
}Getpersonalsystemfolder;

/*upload and download item struct*/
typedef struct TRANSITEM
{
    int id;
    char name[NORMALSIZE];
    long long int size;
    char transid[64];

}Transitem;

/* log struct */
typedef struct LOG_STRUCT{
        int  status;
        char  error[512];
        float  progress;
        char path[512];
}Log_struc;

/*get entry info*/
typedef struct GETENTRYINFO
{
    int status;
    char scrip[MINSIZE];
    int isfolder;
    char display[NORMALSIZE];
    int parent;
    int headversion;
    Fileattribute attr;
    long long filesize;     // for file
    long long treesize; //for folder
    int ishidden;       //for folder
    int isinfected;
    int isbackup;
    int isorigdeleted;
    int ispublic;
    char createdtime[MINSIZE];
}Getentryinfo;

typedef struct my_mutex_tag
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int ready;
}my_mutex_t;

/*add by alan*/
/*typedef struct socket_action{
	char buf[1024];
	struct socket_action *next;
}Socket_Action;
Socket_Action *SocketActionList;
Socket_Action *SocketActionTail;
Socket_Action *SocketActionTmp;*/

typedef struct SOCKET_CMD
{
    char cmd_name[32];
    char filename[256];
    char path[256];
    char oldname[256]; //rename or move prename
    char newname[256]; //rename new name
    char oldpath[256]; //move old path
}Socket_cmd;

/*muti dir read config*/
struct asus_rule
{
    int rule;
    char path[256];
};

struct asus_config
{
    int type;
    char user[256];
    char pwd[256];
    char url[32];
    int  enable;
    char sync_path[256];
    int rule;
    char dir_name[256];
    int sync_disk_exist;
    char captcha[8];
    char otp_key[8];
};

typedef struct USERSTATE
{
    int status;
    int userstate;
    char extensionstarttime[32];
    char extensionendtime[32];
}Getuserstate;

#endif
