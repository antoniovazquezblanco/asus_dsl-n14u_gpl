#include <stdio.h>
#include <malloc.h>
#include "base.h"

#if 0
#include <mcheck.h>
#endif

//#define CONFIG_PATH "/opt/etc/Cloud.conf"      //for N16 test
#define MYPORT 3569
#define BACKLOG 100 /* max listen num*/

#define OTRHRS 0
#define HREF 1
#define CREATIONDATE 2
#define GETCONTENTLANGUAGE 3
#define GETCONTENTLENGTH 4
#define GETCONTENTTYPE 5
#define GETLASTMODIFIED 6
#define STATUS 7

#ifndef NVRAM_
#define NOTIFY_PATH "/tmp/notify/usb"
#else
#define NOTIFY_PATH "/tmp/notify/usb"
#endif
#define SHELL_FILE  "/tmp/smartsync/script/write_nvram"

int infoType;
int stop_up = 0;
int stop_down = 0 ;
//int exit_proc = 0;
int no_config = 0;

int disk_change;
int sync_disk_removed;
int sighandler_finished;
int mountflag;

int add_port_for_host(int index,struct asus_config *config){

    char *temp;
    const char chr = ':';

    temp = my_nstrchr(chr,config->prule[index]->host,2);
    if(temp != NULL)      //HOST already has port
    {
        return 0;
    }
    else                //HOST has no port
    {
        temp = strstr(config->prule[index]->host,"https");
        if(temp == NULL)     //HOST start with http
        {
            sprintf(config->prule[index]->host,"%s:8082",config->prule[index]->host);
        }
        else                 //HOST start with https
        {
            sprintf(config->prule[index]->host,"%s:443",config->prule[index]->host);
        }
    }

    return 0;
}

void parse_server_url(char *pstr,int i,struct asus_config *config){

    char *temp;
    char *p;
    int n;
    char *to_serverpath = NULL;
    const char chr = '/';
    const char *split = "/";

    to_serverpath = (char *)malloc(sizeof(char));
    memset(to_serverpath,'\0',sizeof(to_serverpath));

    DEBUG("pstr = %s\n",pstr);

    temp = my_nstrchr(chr,pstr,3);

    DEBUG("temp = %s\n",temp);

    strncpy(config->prule[i]->host,pstr,strlen(pstr)-strlen(temp));

    add_port_for_host(i,config);

    DEBUG("config->prule[%d]->host = %s\n",i,config->prule[i]->host);

    temp = temp + 1;

    DEBUG("temp1 = %s\n",temp);

    p=strtok(temp,split);
    while(p!=NULL)
    {
        p = oauth_url_escape(p);
        n = strlen(p);
        //printf("p = %s, n = %d\n",p,n);
        to_serverpath = realloc(to_serverpath, strlen(to_serverpath)+n+2);
        //printf("to_serverpath = %s\n",to_serverpath);
        sprintf(to_serverpath,"%s/%s",to_serverpath,p);

        free(p);
        p=strtok(NULL,split);
    }


    sprintf(config->prule[i]->rooturl,"%s%s",config->prule[i]->host,to_serverpath);
    sprintf(config->prule[i]->rootfolder,"%s",to_serverpath);
    config->prule[i]->rootfolder_length = strlen(config->prule[i]->rootfolder);

    free(to_serverpath);

    DEBUG("config->prule[%d]->rooturl = %s\nconfig->prule[%d]->rootfolder = %s\nconfig->prule[%d]->host = %s\n",
           i,config->prule[i]->rooturl,i,config->prule[i]->rootfolder,i,config->prule[i]->host);

}

void parse_mount_path(char *p,int i,struct asus_config *config){

    char *temp;
    const char chr = '/';

    if(!strncmp(p,"/tmp",4))
    {
        temp = my_nstrchr(chr,p,4);
        if(temp == NULL)
        {
            strncpy(config->prule[i]->mount_path,p+4,strlen(p)-4);
            sprintf(config->prule[i]->base_folder,"/");
        }
        else
        {
            strncpy(config->prule[i]->mount_path,p+4,strlen(p)-strlen(temp)-4);
            sprintf(config->prule[i]->base_folder,"%s",temp);
        }

    }
    else
    {
        temp = my_nstrchr(chr,p,3);
        if(temp == NULL)
        {
            strncpy(config->prule[i]->mount_path,p,strlen(p));
            sprintf(config->prule[i]->base_folder,"/");
        }
        else
        {
            strncpy(config->prule[i]->mount_path,p,strlen(p)-strlen(temp));
            sprintf(config->prule[i]->base_folder,"%s",temp);
        }
    }

    DEBUG("config->prule[%d]->mount_path = %s\n",i,config->prule[i]->mount_path);
    DEBUG("config->prule[%d]->base_folder = %s\n",i,config->prule[i]->base_folder);

}

//new parse_config_mutidir.Used for several Server folders and several Local folders
int parse_config_mutidir(char *path,struct asus_config *config)
{
    DEBUG("#####parse_config####\n");

    FILE *fp;
    //DIR *dir;

    char buffer[256];
    char *p;

    int i = 0;
    //int k = 0;
    int len = 0;
    int j = 0;

    memset(buffer,'\0',256);

    if (access(path,0) == 0)
    {
        if(( fp = fopen(path,"rb"))==NULL)
        {
            fprintf(stderr,"read Cloud error");
            exit(-1);
        }
        config->prule = (struct asus_rule **)malloc(sizeof(struct asus_rule*)*config->dir_number);
        while(fgets(buffer,256,fp)!=NULL)
        {
            if( buffer[ strlen(buffer)-1 ] == '\n' )
                buffer[ strlen(buffer)-1 ] = '\0';
            p=buffer;
            if(i == (j*6))
            {
                config->type = atoi(p);

                DEBUG("config->type = %d\n",config->type);

                if( config->type != 1)
                    return 1;
            }
            else if(i == (j*6+1))
            {
                config->prule[j] = (struct asus_rule*)malloc(sizeof(struct asus_rule));
                memset(config->prule[j],0,sizeof(struct asus_rule));
                sprintf(config->prule[j]->desc,"%s",p);
            }
            else if(i == (j*6+2))
            {
                //config->prule[j] = (struct asus_rule*)malloc(sizeof(struct asus_rule));
                parse_server_url(p,j,config);
            }
            else if(i == (j*6+3))
            {
                //config->prule[j] = (struct asus_rule*)malloc(sizeof(struct asus_rule));
                config->prule[j]->rule = atoi(p);

                DEBUG("config->dir_number = %d\n",config->dir_number);

            }
            else if(i == (j*6+4))
            {
                sprintf(config->prule[j]->capacha,"%s",p);
            }
            else if(i == (j*6+5))
            {
                len = strlen(p);
                if(p[len-1] == '\n')
                    p[len-1] = '\0';

                strcpy(config->prule[j]->base_path,p);
                /* trim path last char '/' */
                len = strlen(config->prule[j]->base_path);
                if(config->prule[j]->base_path[len-1] == '/')
                    config->prule[j]->base_path[len-1] = '\0';

                DEBUG("config->prule[%d]->base_path = %s\n",j,config->prule[j]->base_path);

                config->prule[j]->base_path_len = strlen(config->prule[j]->base_path);
                parse_mount_path(config->prule[j]->base_path,j,config);
                j++;
            }
            i++;
        }
        fclose(fp);

        //parse_mount_path(config->prule[0]->base_path);

        /*if(flag)
        {
            for( i = 0 ; i < config->dir_number;i++)
                my_mkdir_r(config->prule[i]->base_path,i);  //have mountpath
        }*/
    }
    //printf("type is %d,status is %d,username is %s,password is %s,rule is %d,base_path is %s\n",
    //type,status,username,password,rule,base_path);
    return 0;
}


void clear_global_var(){

    prog_total = -1;
    prog_time = 0;

    del_rule_num = -1;

    download_only = 0;
    upload_only = 0;

    //local_sync = 0;
    //server_sync = 1;

    first_sync = 1;

    finished_initial = 0;

    stop_progress = 0;
    exit_loop = 0;
    //receve_socket = 0;

    DiskAvailable = 0;
    DiskAvailableShow = 0;
    DiskUsedShow = 0;
    PreDiskAvailableShow = 0;
    PreDiskUsedShow = 0;

    disk_change = 0;
    sync_disk_removed = 0;

    mountflag = 1;

    //dounfinish = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_socket, NULL);
    pthread_mutex_init(&mutex_checkdisk, NULL);
    pthread_mutex_init(&mutex_receve_socket, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    //pthread_mutex_init(&mutex_copyfilelist, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond_socket, NULL);
    pthread_cond_init(&cond_log, NULL);

    memset(log_path,0,sizeof(log_path));
    memset(general_log,0,sizeof(general_log));
    memset(base64_auth,'\0',sizeof(base64_auth));
    //memset(up_item_file, 0, sizeof(up_item_file));
//    memset(down_item_file, 0, sizeof(down_item_file));
    memset(&asus_cfg,0,sizeof(struct asus_config));
    memset(&asus_cfg_stop,0,sizeof(struct asus_config));
    if(initial_tokenfile_info_data(&tokenfile_info_start) == NULL)
    {
        return;
    }


}

void replace_char_in_str(char *str,char newchar,char oldchar){

    int i;
    int len;
    len = strlen(str);
    for(i=0;i<len;i++)
    {
        if(str[i] == oldchar)
        {
            //printf("str[i] = %c\n",str[i]);
            str[i] = newchar;
        }
    }

}

struct disk_info *initial_disk_data(struct disk_info **disk)
{
    struct disk_info *follow_disk;

    if(disk == NULL)
        return NULL;

    *disk = (struct disk_info *)malloc(sizeof(struct disk_info));
    if(*disk == NULL)
        return NULL;

    follow_disk = *disk;

    follow_disk->diskname = NULL;
    follow_disk->diskpartition = NULL;
    follow_disk->mountpath = NULL;
    follow_disk->serialnum = NULL;
    follow_disk->product = NULL;
    follow_disk->vendor = NULL;
    follow_disk->next = NULL;
    follow_disk->port = (unsigned int)0;
    follow_disk->partitionport = (unsigned int )0;

    return follow_disk;
}

int init_diskinfo_struct()
{
    int len = 0;
    FILE *fp;
    if(access(USBINFO_PATH,0)==0)
    {
        fp =fopen(USBINFO_PATH,"r");
        if(fp)
        {
            fseek(fp,0,SEEK_END);
            len = ftell(fp);
            fseek(fp,0,SEEK_SET);
        }
    }
    else
        return -1;

    char buf[len+1];
    memset(buf,'\0',sizeof(buf));
    fread(buf,1,len,fp);
    fclose(fp);

    if(initial_disk_data(&follow_disk_info_start) == NULL){
        return -1;
    }

    follow_disk_info = follow_disk_info_start;
    //get diskname and mountpath
    char a[1024];
    char *p,*q;
    fp = fopen("/proc/mounts","r");
    if(fp)
    {
        while(!feof(fp))
        {
            memset(a,'\0',sizeof(a));
            fscanf(fp,"%[^\n]%*c",a);
            if((strlen(a) != 0)&&((p=strstr(a,"/dev/sd")) != NULL))
            {
                if(initial_disk_data(&follow_disk_tmp) == NULL)
                {
                    return -1;
                }
                p = p + 5;
                follow_disk_tmp->diskname=(char *)malloc(5);
                memset(follow_disk_tmp->diskname,'\0',5);
                strncpy(follow_disk_tmp->diskname,p,4);

                p = p + 3;
                follow_disk_tmp->partitionport=atoi(p);
                if((q=strstr(p,"/tmp")) != NULL)
                {
                    if((p=strstr(q," ")) != NULL)
                    {
                        follow_disk_tmp->mountpath=(char *)malloc(strlen(q)-strlen(p)+1);
                        memset(follow_disk_tmp->mountpath,'\0',strlen(q)-strlen(p)+1);
                        strncpy(follow_disk_tmp->mountpath,q,strlen(q)-strlen(p));
                    }
                }
                char diskname_tmp[4];
                memset(diskname_tmp,'\0',sizeof(diskname_tmp));
                strncpy(diskname_tmp,follow_disk_tmp->diskname,3);
                if((p=strstr(buf,diskname_tmp)) != NULL)
                {
                    p = p - 6;
                    follow_disk_tmp->port=atoi(p);
                    follow_disk_tmp->diskpartition=(char *)malloc(4);
                    memset(follow_disk_tmp->diskpartition,'\0',4);
                    strncpy(follow_disk_tmp->diskpartition,diskname_tmp,3);
                    q=strstr(p,"_serial");
                    q = q + 8;
                    p=strstr(q,"_pid");
                    follow_disk_tmp->serialnum=(char *)malloc(strlen(q)-strlen(p)-4);
                    memset(follow_disk_tmp->serialnum,'\0',strlen(q)-strlen(p)-4);
                    strncpy(follow_disk_tmp->serialnum,q,strlen(q)-strlen(p)-5);
                    p = p + 5;
                    q=strstr(p,"_vid");
                    follow_disk_tmp->product=(char *)malloc(strlen(p)-strlen(q)-4);
                    memset(follow_disk_tmp->product,'\0',5);
                    strncpy(follow_disk_tmp->product,p,strlen(p)-strlen(q)-5);
                    q = q + 5;
                    follow_disk_tmp->vendor=(char *)malloc(5);
                    memset(follow_disk_tmp->vendor,'\0',5);
                    strncpy(follow_disk_tmp->vendor,q,4);
                }

                follow_disk_info->next = follow_disk_tmp;
                follow_disk_info = follow_disk_tmp;
            }
        }
    }
    fclose(fp);
}

int get_mounts_info(struct mounts_info_tag *info)
{
    int len = 0;
    FILE *fp;
    int i = 0;
    int num = 0;
    //char *mount_path;
    char **tmp_mount_list=NULL;
    char **tmp_mount=NULL;

    char buf[len+1];
    memset(buf,'\0',sizeof(buf));
    char a[1024];
    //char *temp;
    char *p,*q;
    fp = fopen("/proc/mounts","r");
    if(fp)
    {
        while(!feof(fp))
        {
            memset(a,'\0',sizeof(a));
            fscanf(fp,"%[^\n]%*c",a);
            if((strlen(a) != 0)&&((p=strstr(a,"/dev/sd")) != NULL))
            {
                if((q=strstr(p,"/mnt")) != NULL)
                {
                    if((p=strstr(q," ")) != NULL)
                    {
                        len = strlen(q) - strlen(p)+1;

                        tmp_mount = (char **)malloc(sizeof(char *)*(num+1));
                        if(tmp_mount == NULL)
                        {
                            fclose(fp);
                            return -1;
                        }


                        tmp_mount[num] = my_str_malloc(len+1);
                        if(tmp_mount[num] == NULL)
                        {
                            free(tmp_mount);
                            fclose(fp);
                            return -1;
                        }
                        snprintf(tmp_mount[num],len,"%s",q);

                        if(num != 0)
                        {
                            for(i = 0; i < num; ++i)
                                tmp_mount[i] = tmp_mount_list[i];

                            free(tmp_mount_list);
                            tmp_mount_list = tmp_mount;
                        }
                        else
                            tmp_mount_list = tmp_mount;

                        ++num;

                        //info->paths =
                    }
                }
            }
        }
    }
    fclose(fp);

    info->paths = tmp_mount_list;
    info->num = num;
    return 0;
}

int get_tokenfile_info()
{
    int i;
    int j = 0;
    struct mounts_info_tag *info = NULL;
    char *tokenfile;
    FILE *fp;
    char buffer[1024];
    char *p;

    tokenfile_info = tokenfile_info_start;

    info = (struct mounts_info_tag *)malloc(sizeof(struct mounts_info_tag));
    if(info == NULL)
    {
        printf("obtain memory space fail\n");
        return -1;
    }

    memset(info,0,sizeof(struct mounts_info_tag));
    memset(buffer,0,1024);

    if(get_mounts_info(info) == -1)
    {
        printf("get mounts info fail\n");
        return -1;
    }

    for(i=0;i<info->num;i++)
    {
        //printf("info->paths[%d] = %s\n",i,info->paths[i]);
        tokenfile = my_str_malloc(strlen(info->paths[i])+18+1);
#ifndef PC
        sprintf(tokenfile,"%s/.webdav_tokenfile",info->paths[i]);
#else
        sprintf(tokenfile,".webdav_tokenfile");
#endif
        //printf("tokenfile = %s\n",tokenfile);
        if(!access(tokenfile,F_OK))
        {
            //printf("tokenfile is exist!\n");
            if((fp = fopen(tokenfile,"r"))==NULL)
            {
                fprintf(stderr,"read tokenfile error\n");
                exit(-1);
            }
            while(fgets(buffer,1024,fp)!=NULL)
            {
                if( buffer[ strlen(buffer)-1 ] == '\n' )
                    buffer[ strlen(buffer)-1 ] = '\0';
                p = buffer;
                if(j == 0)
                {
                    if(initial_tokenfile_info_data(&tokenfile_info_tmp) == NULL)
                    {
                        return -1;
                    }
                    tokenfile_info_tmp->url = my_str_malloc(strlen(p)+1);
                    sprintf(tokenfile_info_tmp->url,"%s",p);
                    tokenfile_info_tmp->mountpath = my_str_malloc(strlen(info->paths[i])+1);
                    sprintf(tokenfile_info_tmp->mountpath,"%s",info->paths[i]);
                    j++;
                }
                else
                {
                    tokenfile_info_tmp->folder = my_str_malloc(strlen(p)+1);
                    sprintf(tokenfile_info_tmp->folder,"%s",p);
                    tokenfile_info->next = tokenfile_info_tmp;
                    tokenfile_info = tokenfile_info_tmp;
                    j = 0;
                }
            }
            fclose(fp);
        }
        free(tokenfile);
    }


    for(i=0;i<info->num;++i)
    {
        free(info->paths[i]);
    }
    if(info->paths != NULL)
        free(info->paths);
    free(info);

    return 0;
}

void free_disk_struc(struct disk_info **disk)
{
    struct disk_info *follow_disk, *old_disk;

    if(disk == NULL)
        return;

    follow_disk = *disk;
    while(follow_disk != NULL)
    {
        if(follow_disk->diskname != NULL)
            free(follow_disk->diskname);
        if(follow_disk->diskpartition != NULL)
            free(follow_disk->diskpartition);
        if(follow_disk->mountpath != NULL)
            free(follow_disk->mountpath);
        if(follow_disk->serialnum != NULL)
            free(follow_disk->serialnum);
        if(follow_disk->product != NULL)
            free(follow_disk->product);
        if(follow_disk->vendor != NULL)
            free(follow_disk->vendor);

        old_disk = follow_disk;
        follow_disk = follow_disk->next;
        free(old_disk);
    }
}

int check_sync_disk_removed()
{
    DEBUG("check_sync_disk_removed start! \n");

    int ret;

    free_tokenfile_info(tokenfile_info_start);

    if(get_tokenfile_info()==-1)
    {
        printf("\nget_tokenfile_info failed\n");
        exit(-1);
    }

    ret = check_config_path(0);
    return ret;

}

int check_disk_change()
{
    int status = -1;
    pthread_mutex_lock(&mutex_checkdisk);
    disk_change = 0;
    pthread_mutex_unlock(&mutex_checkdisk);
    status = check_sync_disk_removed();

    if(status == 2 || status == 1)
    {
        exit_loop = 1;
        sync_up = 0;
        sync_down = 0;
        sync_disk_removed = 1;
    }

    return 0;
}

//new parse_config_mutidir.Used for several Server folders and several Local folders
void init_global_var()
{
    DEBUG("go into init_global_var \n");
#if 1
    int ret;
    int i;
    int num = asus_cfg.dir_number;

    char *temp = NULL;
    const char chr = '/';

    local_sync = 0;
    server_sync = 1;

    g_pSyncList = (sync_list **)malloc(sizeof(sync_list *)*num);
    memset(g_pSyncList,0,sizeof(g_pSyncList));
    //printf("log_path = %s\ntemp_path = %s\ngeneral_log = %s\n",log_path,temp_path,general_log);
    //printf("log_path = %s\ngeneral_log = %s\n",log_path,general_log);

    //printf("before for \n");
    sighandler_finished = 0;

    for(i=0;i<num;i++)
    {
        g_pSyncList[i] = (sync_list *)malloc(sizeof(sync_list));
        memset(g_pSyncList[i],0,sizeof(sync_list));

        g_pSyncList[i]->index = i;
        g_pSyncList[i]->receve_socket = 0;
        g_pSyncList[i]->have_local_socket = 0;
        g_pSyncList[i]->first_sync = 1;
        g_pSyncList[i]->no_local_root = 0;
        g_pSyncList[i]->init_completed = 0;
        g_pSyncList[i]->ServerRootNode = NULL;
        g_pSyncList[i]->OldServerRootNode = NULL;
        g_pSyncList[i]->server_sync_running = 0;

        g_pSyncList[i]->sync_disk_exist = 0;

        sprintf(g_pSyncList[i]->temp_path,"%s/.smartsync/webdav",asus_cfg.prule[i]->mount_path);
        my_mkdir_r(g_pSyncList[i]->temp_path,i);    //have mountpath

        DEBUG("temp_path = %s\n",g_pSyncList[i]->temp_path);

        DEBUG("g_pSyncList[%d]->index = %d\n",i,g_pSyncList[i]->index);

        char *base_path_tmp,*base_folder_tmp;

        base_path_tmp = my_str_malloc(sizeof(asus_cfg.prule[i]->base_path));
        base_folder_tmp = my_str_malloc(sizeof(asus_cfg.prule[i]->base_folder));

        sprintf(base_path_tmp,"%s",asus_cfg.prule[i]->base_path);
        sprintf(base_folder_tmp,"%s",asus_cfg.prule[i]->base_folder);

        replace_char_in_str(base_path_tmp,'_','/');
        replace_char_in_str(base_folder_tmp,'_','/');

        snprintf(g_pSyncList[i]->up_item_file,255,"%s/%s_up_item",
                 g_pSyncList[i]->temp_path,base_path_tmp);
//        snprintf(g_pSyncList[i]->down_item_file,255,"%s/%s_down_item",
//                 g_pSyncList[i]->temp_path,base_path_tmp);
        snprintf(g_pSyncList[i]->conflict_file,255,"%s/conflict_%s",
                 g_pSyncList[i]->temp_path,base_folder_tmp);

        free(base_path_tmp);
        free(base_folder_tmp);

        g_pSyncList[i]->server_action_list = create_action_item_head();
        g_pSyncList[i]->dragfolder_action_list = create_action_item_head();
        g_pSyncList[i]->unfinished_list = create_action_item_head();
        g_pSyncList[i]->up_space_not_enough_list = create_action_item_head();
        g_pSyncList[i]->copy_file_list = create_action_item_head();
        if(asus_cfg.prule[i]->rule == 1)
        {
            g_pSyncList[i]->download_only_socket_head = create_action_item_head();
        }
        else
        {
            g_pSyncList[i]->download_only_socket_head = NULL;
        }
        g_pSyncList[i]->SocketActionList = queue_create();
        g_pSyncList[i]->SocketActionList_Priority = queue_create();

//        DEBUG("g_pSyncList[%d]->down_item_file = %s\n",i,g_pSyncList[i]->down_item_file);
        DEBUG("g_pSyncList[%d]->up_item_file = %s\n",i,g_pSyncList[i]->up_item_file);

        tokenfile_info_tmp = tokenfile_info_start->next;
        while(tokenfile_info_tmp != NULL)
        {
            //printf("tokenfile_info_tmp->folder = %s\n",tokenfile_info_tmp->folder);
            if(!strcmp(tokenfile_info_tmp->folder,asus_cfg.prule[i]->base_folder)
                && !strcmp(tokenfile_info_tmp->url,asus_cfg.prule[i]->rooturl))
                {
                g_pSyncList[i]->sync_disk_exist = 1;
                if(access(asus_cfg.prule[i]->base_path,F_OK))
                {
                    my_mkdir_r(asus_cfg.prule[i]->base_path,i);
                }
                break;
            }
            tokenfile_info_tmp = tokenfile_info_tmp->next;
        }
        DEBUG("g_pSyncList[i]->sync_disk_exist = %d\n",g_pSyncList[i]->sync_disk_exist);

    }
#endif
}

/*
int get_path_to_index(char *path)
{
    int i;
    char *root_path = NULL;
    char *temp = NULL;
    root_path = my_str_malloc(512);

    if(strncmp(path,"/tmp",4))
    {
        temp = my_nstrchr('/',path,4);
    }
    else
    {
        temp = my_nstrchr('/',path,5);
    }
    if(temp == NULL)
    {
        sprintf(root_path,"%s",path);
    }
    else
    {
        snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
    }

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(!strcmp(root_path,asus_cfg.prule[i]->base_path))
            break;
    }

    DEBUG("get_path_to_index root_path = %s\n",root_path);
    free(root_path);

    return i;
}*/
int get_path_to_index(char *path)
{
    DEBUG("get_path_to_index path = %s\n",path);
    int i;
    char root_path[512];
    char cmp_path[1024];
    memset(cmp_path,0,1024);
    sprintf(cmp_path,"%s/",path);
    //char *temp = NULL;
    //root_path = my_str_malloc(512);

    /*if(strncmp(path,"/tmp",4))
    {
        temp = my_nstrchr('/',path,4);
    }
    else
    {
        temp = my_nstrchr('/',path,5);
    }
    if(temp == NULL)
    {
        sprintf(root_path,"%s",path);
    }
    else
    {
        snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
    }*/

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        memset(root_path,0,512);
        sprintf(root_path,"%s/",asus_cfg.prule[i]->base_path);
        if(!strncmp(cmp_path,root_path,strlen(root_path)))
            break;
    }

    //DEBUG("get_path_to_index root_path = %s\n",root_path);
    //DEBUG("get_path_to_index i = %d\n",i);
    //free(root_path);

    return i;
}


int cmd_parser(char *cmd,int index)
{
    DEBUG("socket command is %s \n",cmd);
    //printf("socket command is %s \n",cmd);

    /*if( !strncmp(cmd,"exit",4))
    {
        DEBUG("exit socket\n");
    	return 0;
    }*/

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }

    const char split = '\n';
    char cmd_name[64];
    //char cmd_param[512];
    char *path;
    char *temp;
    char filename[256];
    char *fullname;
    char oldname[256],newname[256];
    char *oldpath;
    char action[64];
    char *cmp_name;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;
    int status;
    //Asusconfig asusconfig;

    memset(cmd_name, 0, sizeof(cmd_name));
    memset(oldname,'\0',sizeof(oldname));
    memset(newname,'\0',sizeof(newname));
    memset(action,0,sizeof(action));

    ch = cmd;
    int i = 0;
    while(*ch != split)
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    strcpy(temp,ch);
    p = strchr(temp,split);

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        strcpy(oldname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);

        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }
    else
    {
        strcpy(filename,p);
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));

        DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);

        if(filename[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }

    free(temp);


    if( !strncmp(cmd_name,"rename",6) )
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        sprintf(cmp_name,"%s/%s",path,newname);
    }
    else
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        sprintf(cmp_name,"%s/%s",path,filename);
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        strcpy(action,"createfile");
        action_item *item;

        //pthread_mutex_lock(&mutex_copyfilelist);
        item = get_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {
            DEBUG("##### delete copyfile %s ######\n",cmp_name);
            //pthread_mutex_lock(&mutex);
            del_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list);
        }
        //pthread_mutex_unlock(&mutex_copyfilelist);
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        strcpy(action,"remove");
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        strcpy(action,"createfolder");
    }
    else if( strncmp(cmd_name, "rename",6) == 0 )
    {
        strcpy(action,"rename");
    }
    else if( strncmp(cmd_name, "move",4) == 0 )
    {
        strcpy(action,"move");
    }

    if(g_pSyncList[index]->server_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,cmp_name,g_pSyncList[index]->server_action_list,index);

        if(item != NULL)
        {
            DEBUG("##### %s %s by WebDAV Server self ######\n",action,cmp_name);
            //pthread_mutex_lock(&mutex);
            del_action_item(action,cmp_name,g_pSyncList[index]->server_action_list);

            DEBUG("#### del action item success!\n");
            //pthread_mutex_unlock(&mutex);
            //local_sync = 0;
            free(path);
            free(fullname);
            free(cmp_name);
            return 0;
        }
    }

    if(g_pSyncList[index]->dragfolder_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,cmp_name,g_pSyncList[index]->dragfolder_action_list,index);

        if(item != NULL)
        {
            DEBUG("##### %s %s by dragfolder recursion self ######\n",action,cmp_name);
            //pthread_mutex_lock(&mutex);
            del_action_item(action,cmp_name,g_pSyncList[index]->dragfolder_action_list);

            //pthread_mutex_unlock(&mutex);
            //local_sync = 0;
            free(path);
            free(fullname);
            free(cmp_name);
            return 0;
        }
    }
    free(cmp_name);

    DEBUG("###### %s is start ######\n",cmd_name);
    
    if( strcmp(cmd_name, "copyfile") != 0 && g_pSyncList[index]->server_sync_running)
    {
        g_pSyncList[index]->have_local_socket = 1;
    }

    if( strcmp(cmd_name, "createfile") == 0 || strcmp(cmd_name, "dragfile") == 0 )
    {
        sprintf(fullname,"%s/%s",path,filename);
        int exist;
        exist = is_server_exist(path,fullname,index);
        if(!exist)
        {
            status = Upload(fullname,index);

            if(status != 0)
            {
                DEBUG("upload %s failed\n",fullname);
                free(path);
                free(fullname);
                //return status;
                if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                    return status;
                return 0;
            }
            else
            {
                char *serverpath;
                serverpath = localpath_to_serverpath(fullname,index);
                //printf("serverpath = %s\n",serverpath);
                time_t modtime;
                modtime = Getmodtime(serverpath,index);
                if(modtime != -1)
                    ChangeFile_modtime(fullname,modtime,index);
                else
                {
                    DEBUG("ChangeFile_modtime failed!\n");
                }
                free(fullname);
                free(serverpath);
            }
        }
        else
        {
            if((newer_file(fullname,index)) != 2 && (newer_file(fullname,index)) != -1)
            {
                if(1)
                {
                    char *newname;
                    newname = change_server_same_name(fullname,index);
                    //Move(fullname,newname,index);
                    DEBUG("newname = %s\n",newname);
                    write_conflict_log(fullname,newname,index);
                    status = Move(fullname,newname,index);
                    free(newname);
                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        //free(serverpath);
                        return status;
                    }
                    //free(newname);

                    status = Upload(fullname,index);

                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        //return status;
                        if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                            return status;
                        return 0;
                    }
                    else
                    {
                        char *serverpath;
                        serverpath = localpath_to_serverpath(fullname,index);
                        //printf("serverpath = %s\n",serverpath);
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                        {
                            char *urlpath;
                            char *m_temp;
                            const char m_chr = ':';

                            m_temp = my_nstrchr(m_chr,asus_cfg.prule[index]->host,2);

                            urlpath = my_str_malloc(strlen(serverpath)+strlen(asus_cfg.prule[index]->host)+1);
                            snprintf(urlpath,strlen(asus_cfg.prule[index]->host)-strlen(m_temp)+1,"%s",
                                     asus_cfg.prule[index]->host);

                            sprintf(urlpath,"%s%s",urlpath,serverpath);

                            CloudFile *filetmp = NULL;
                            if(g_pSyncList[index]->init_completed)
                            {
                                filetmp = get_CloudFile_node(g_pSyncList[index]->OldServerRootNode,urlpath,0x2);
                            }
                            else
                            {
                                filetmp = get_CloudFile_node(g_pSyncList[index]->ServerRootNode,urlpath,0x2);
                            }
                            free(urlpath);
                            if(filetmp != NULL)
                            {
                                filetmp->modtime = modtime;
                            }
                            ChangeFile_modtime(fullname,modtime,index);
                        }

                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(fullname);
                        free(serverpath);
                    }

                }
            }
        }

    }

    else if( strcmp(cmd_name, "copyfile") == 0 )
    {
        sprintf(fullname,"%s/%s",path,filename);
        /*while(is_copying_finished(fullname) !=0 ){
            sleep(1);
        }*/
        //pthread_mutex_lock(&mutex_copyfilelist);
        add_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        //pthread_mutex_unlock(&mutex_copyfilelist);

        free(fullname);
    }

    else if( strcmp(cmd_name, "modify") == 0 )
    {
        sprintf(fullname,"%s/%s",path,filename);
        char *serverpath;
        serverpath = localpath_to_serverpath(fullname,index);
        int exist;
        exist = is_server_exist(path,fullname,index);

        if(!exist)       //Server has no the same file
        {
            status = Upload(fullname,index);

            if(status != 0)
            {
                DEBUG("upload %s failed\n",fullname);
                free(path);
                free(fullname);
                //return status;
                if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                    return status;
                return 0;
            }
            else
            {
                //char *serverpath;
                //serverpath = localpath_to_serverpath(fullname,index);
                //printf("serverpath = %s\n",serverpath);
                time_t modtime;
                modtime = Getmodtime(serverpath,index);
                if(modtime != -1)
                    ChangeFile_modtime(fullname,modtime,index);
                else
                {
                    DEBUG("ChangeFile_modtime failed!\n");
                }
                free(fullname);
                free(serverpath);
            }
        }
        else
        {
            //char *serverpath;
            //serverpath = localpath_to_serverpath(fullname,index);
            char *urlpath;
            char *m_temp;
            const char m_chr = ':';

            m_temp = my_nstrchr(m_chr,asus_cfg.prule[index]->host,2);

            urlpath = my_str_malloc(strlen(serverpath)+strlen(asus_cfg.prule[index]->host)+1);
            snprintf(urlpath,strlen(asus_cfg.prule[index]->host)-strlen(m_temp)+1,"%s",
                     asus_cfg.prule[index]->host);

            sprintf(urlpath,"%s%s",urlpath,serverpath);

            //DEBUG("urlpath = %s\n",urlpath);

            CloudFile *filetmp = NULL;
            if(g_pSyncList[index]->init_completed)
            {
                filetmp = get_CloudFile_node(g_pSyncList[index]->OldServerRootNode,urlpath,0x2);
            }
            else
            {
                filetmp = get_CloudFile_node(g_pSyncList[index]->ServerRootNode,urlpath,0x2);
            }
            //CloudFile *realfiletmp = NULL;
            //realfiletmp = get_CloudFile_node(g_pSyncList[index]->RealOldServerRootNode,urlpath,0x2);
            free(urlpath);

            if(asus_cfg.prule[index]->rule == 2)
            {
                if(filetmp == NULL)
                {
                    char *newname;
                    newname = change_server_same_name(fullname,index);
                    DEBUG("newname = %s\n",newname);
                    write_conflict_log(fullname,newname,index);
                    status = Move(fullname,newname,index);
                    free(newname);
                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        free(serverpath);
                        return status;
                    }
                    status = Upload(fullname,index);

                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        free(serverpath);
                        //return status;
                        if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                            return status;
                        return 0;
                    }
                    else
                    {
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                        {
                            //filetmp->modtime = modtime;
                            //filetmp->ismodify = 0;
                            ChangeFile_modtime(fullname,modtime,index);
                        }
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(fullname);
                        free(serverpath);
                    }
                }
                else
                {
                    if(filetmp->ismodify)
                    {
                        char *newname;
                        newname = change_server_same_name(fullname,index);
                        DEBUG("newname = %s\n",newname);
                        write_conflict_log(fullname,newname,index);
                        status = Move(fullname,newname,index);
                        free(newname);
                        if(status != 0)
                        {
                            DEBUG("upload %s failed\n",fullname);
                            free(path);
                            free(fullname);
                            free(serverpath);
                            return status;
                        }
                        status = Upload(fullname,index);
                        filetmp->ismodify = 0;

                        if(status != 0)
                        {
                            DEBUG("upload %s failed\n",fullname);
                            free(path);
                            free(fullname);
                            free(serverpath);
                            //return status;
                            if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                                return status;
                            return 0;
                        }
                        else
                        {
                            time_t modtime;
                            modtime = Getmodtime(serverpath,index);
                            if(modtime != -1)
                            {
                                filetmp->modtime = modtime;
                                //filetmp->ismodify = 0;
                                ChangeFile_modtime(fullname,modtime,index);
                            }
                            else
                            {
                                DEBUG("ChangeFile_modtime failed!\n");
                            }
                            free(fullname);
                            free(serverpath);
                        }
                    }
                    else
                    {
                        time_t modtime1,modtime2;
                        modtime1 = Getmodtime(serverpath,index);
                        modtime2 = filetmp->modtime;
                        if(modtime1 != modtime2)
                        {
                            char *newname;
                            newname = change_server_same_name(fullname,index);
                            DEBUG("newname = %s\n",newname);
                            write_conflict_log(fullname,newname,index);
                            status = Move(fullname,newname,index);
                            free(newname);
                            if(status != 0)
                            {
                                DEBUG("upload %s failed\n",fullname);
                                free(path);
                                free(fullname);
                                free(serverpath);
                                return status;
                            }
                            status = Upload(fullname,index);

                            if(status != 0)
                            {
                                DEBUG("upload %s failed\n",fullname);
                                free(path);
                                free(fullname);
                                free(serverpath);
                                if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                                    return status;
                                //return status;
                                return 0;
                            }
                            else
                            {
                                time_t modtime;
                                modtime = Getmodtime(serverpath,index);
                                if(modtime != -1)
                                {
                                    filetmp->modtime = modtime;
                                    ChangeFile_modtime(fullname,modtime,index);
                                }
                                else
                                {
                                    DEBUG("ChangeFile_modtime failed!\n");
                                }
                                free(fullname);
                                free(serverpath);
                            }
                        }
                        else
                        {
                            status = Upload(fullname,index);

                            if(status != 0)
                            {
                                DEBUG("upload %s failed\n",fullname);
                                free(path);
                                free(fullname);
                                //return status;
                                if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                                    return status;
                                return 0;
                            }
                            else
                            {
                                //char *serverpath;
                                //serverpath = localpath_to_serverpath(fullname,index);
                                //printf("serverpath = %s\n",serverpath);
                                time_t modtime;
                                modtime = Getmodtime(serverpath,index);
                                if(modtime != -1)
                                {
                                    filetmp->modtime = modtime;
                                    ChangeFile_modtime(fullname,modtime,index);
                                }
                                else
                                {
                                    DEBUG("ChangeFile_modtime failed!\n");
                                }
                                free(fullname);
                                free(serverpath);
                            }
                        }
                    }
                }
            }
            else
            {
                time_t modtime1,modtime2;
                modtime1 = Getmodtime(serverpath,index);
                if(filetmp == NULL)
                {
                    modtime2 = (time_t)-1;
                }
                else
                {
                    modtime2 = filetmp->modtime;
                }
                DEBUG("modtime1 = %lu,modtime2 = %lu\n",modtime1,modtime2);
                if(modtime1 == modtime2)
                {
                    status = Upload(fullname,index);

                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        //return status;
                        if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                            return status;
                        return 0;
                    }
                    else
                    {
                        //char *serverpath;
                        //serverpath = localpath_to_serverpath(fullname,index);
                        //printf("serverpath = %s\n",serverpath);
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                        {
                            filetmp->modtime = modtime;
                            //filetmp->ismodify = 0;
                            ChangeFile_modtime(fullname,modtime,index);
                        }
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(fullname);
                        free(serverpath);
                    }
                }
                else
                {
                    char *newname;
                    newname = change_server_same_name(fullname,index);
                    DEBUG("newname = %s\n",newname);
                    write_conflict_log(fullname,newname,index);
                    status = Move(fullname,newname,index);
                    free(newname);
                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        free(serverpath);
                        return status;
                    }
                    status = Upload(fullname,index);

                    if(status != 0)
                    {
                        DEBUG("upload %s failed\n",fullname);
                        free(path);
                        free(fullname);
                        free(serverpath);
                        //return status;
                        if(status == LOCAL_FILE_LOST || status == NO_PARENT)
                            return status;
                        return 0;
                    }
                    else
                    {
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                        {
                            filetmp->modtime = modtime;
                            //filetmp->ismodify = 0;
                            ChangeFile_modtime(fullname,modtime,index);
                        }
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(fullname);
                        free(serverpath);
                    }
                }
            }
        }
    }

    else if(strcmp(cmd_name, "delete") == 0  || strcmp(cmd_name, "remove") == 0){
        sprintf(fullname,"%s/%s",path,filename);
        status = Delete(fullname,index);

        free(fullname);

    	if(status != 0)
        {
            DEBUG("delete failed\n");
            free(path);
            //free(fullname);
            return status;
    	}
    }
    else if(strncmp(cmd_name, "move",4) == 0 || strncmp(cmd_name, "rename",6) == 0)
    {
        if(strncmp(cmd_name, "move",4) == 0)
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
            sprintf(mv_newpath,"%s/%s",path,oldname);
            sprintf(mv_oldpath,"%s/%s",oldpath,oldname);
            free(oldpath);
    	}
        else
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            sprintf(mv_newpath,"%s/%s",path,newname);
            sprintf(mv_oldpath,"%s/%s",path,oldname);
        }
        if(strncmp(cmd_name,"rename",6) == 0)
        {
            int exist = 0;
            //if(test_if_dir(mv_newpath))   //del by zero check
            if(1)
            {
                exist = is_server_exist(path,mv_newpath,index);
                if(exist)
                {
                    char *newname;
                    newname = change_server_same_name(mv_newpath,index);
                    DEBUG("newname = %s\n",newname);
                    write_conflict_log(mv_newpath,newname,index);
                    status = Move(mv_newpath,newname,index);

                    free(newname);
                    if(status == 0)
                    {
                        status = Move(mv_oldpath,mv_newpath,index);
                    }
                }
                else
                {
                    status = Move(mv_oldpath,mv_newpath,index);
                }
            }
            /*else
            {
                exist = is_server_exist(path,mv_newpath,index);
                if(exist)
                {
                    DEBUG("have the same name\n");
                    char *newname;
                    newname = change_server_same_name(mv_newpath,index);
                    DEBUG("newname = %s\n",newname);
                    status = Move(mv_newpath,newname,index);

                    free(newname);
                    if(status == 0)
                    {
                        //sleep(2);
                        status = Move(mv_oldpath,mv_newpath,index);
                    }

                }
                else
                {
                    DEBUG("have no the same name\n");
                    status = Move(mv_oldpath,mv_newpath,index);
                }

            }*/
        }
        else    //move
        {
            int exist = 0;
            int old_index;
            old_index = get_path_to_index(mv_oldpath);
            if(index == old_index)
            {
                if(test_if_dir(mv_newpath))
                {
                    exist = is_server_exist(path,mv_newpath,index);

                    if(exist)
                    {
                        char *newname;
                        newname = change_server_same_name(mv_newpath,index);
                        DEBUG("newname = %s\n",newname);
                        write_conflict_log(mv_newpath,newname,index);
                        status = Move(mv_newpath,newname,index);

                        free(newname);
                        if(status == 0)
                        {
                            status = moveFolder(mv_oldpath,mv_newpath,index);
                            if(status == 0)
                            {
                                Delete(mv_oldpath,index);
                            }
                        }
                    }
                    else
                    {
                        status = moveFolder(mv_oldpath,mv_newpath,index);
                        if(status == 0)
                        {
                            Delete(mv_oldpath,index);
                        }
                    }
                }
                else
                {
                    exist = is_server_exist(path,mv_newpath,index);

                    if(exist)
                    {
                        char *newname;
                        newname = change_server_same_name(mv_newpath,index);
                        DEBUG("newname = %s\n",newname);
                        write_conflict_log(mv_newpath,newname,index);
                        status = Move(mv_newpath,newname,index);

                        free(newname);
                        if(status == 0)
                        {
                            //sleep(2);
                            status = Move(mv_oldpath,mv_newpath,index);
                        }

                    }
                    else
                    {
                        status = Move(mv_oldpath,mv_newpath,index);
                    }

                }
            }
            else
            {
                if(asus_cfg.prule[old_index]->rule == 1)
                {
                    del_download_only_action_item("move",mv_oldpath,g_pSyncList[old_index]->download_only_socket_head);
                }
                else
                {
                    Delete(mv_oldpath,old_index);
                }

                if(test_if_dir(mv_newpath))
                {
                    status = createFolder(mv_newpath,index);
                    //printf("create_folder_cmp = %d\n",create_folder_cmp);
                    //free(fullname);
                    if(status != 0)
                    {
                        DEBUG("createFolder failed status = %d\n",status);
                        free(path);
                        free(mv_oldpath);
                        free(mv_newpath);
                        return status;
                    }

                }
                else
                {
                    status = Upload(mv_newpath,index);

                    if(status != 0)
                    {
                        DEBUG("move %s to %s failed\n",mv_oldpath,mv_newpath);
                        free(path);
                        free(mv_oldpath);
                        free(mv_newpath);
                        return status;
                    }
                    else
                    {
                        char *serverpath;
                        serverpath = localpath_to_serverpath(mv_newpath,index);
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                            ChangeFile_modtime(mv_newpath,modtime,index);
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(serverpath);
                    }
                }
            }

        }


        free(mv_oldpath);
        free(mv_newpath);

        if(status != 0)
        {
            DEBUG("move/rename failed\n");
            free(path);
            return status;
        }
    }
    else if(strcmp(cmd_name, "dragfolder") == 0)
    {
        sprintf(fullname,"%s/%s",path,filename);
        if(access(fullname,0) != 0)
        {
            DEBUG("Local has no %s\n",fullname);
            free(fullname);
            free(path);
            return LOCAL_FILE_LOST;
        }
        char info[512];
        memset(info,0,sizeof(info));
        sprintf(info,"createfolder\n%s\n%s",path,filename);
        pthread_mutex_lock(&mutex_socket);
        add_socket_item(info,0,index);
        pthread_mutex_unlock(&mutex_socket);
        deal_dragfolder_to_socketlist(fullname,index);
        free(fullname);

    }
    else if(strcmp(cmd_name, "createfolder") == 0)
    {
        sprintf(fullname,"%s/%s",path,filename);
        status = Mkcol(fullname,index);
        //printf("create_folder_cmp = %d\n",create_folder_cmp);
        free(fullname);
        if(status != 0)
        {
            DEBUG("createFolder failed status = %d\n",status);
            free(path);
            return status;
        }
    }

    free(path);
    return 0;
}

int download_only_add_socket_item(char *cmd,int index){

    DEBUG("download_only_add_socket_item receive socket : %s\n",cmd);

    if( strstr(cmd,"(conflict)") != NULL )
        return 0;

    DEBUG("socket command is %s \n",cmd);

    if( !strncmp(cmd,"exit",4))
    {
        DEBUG("exit socket\n");
        return 0;
    }

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }

    const char split = '\n';
    char cmd_name[64];
    char *path = NULL;
    char *temp = NULL;
    char filename[256];
    char *fullname = NULL;
    char oldname[256],newname[256];
    char *oldpath = NULL;
    char action[64];
    char *ch = NULL;
    char *old_fullname = NULL;

    memset(cmd_name,'\0',sizeof(cmd_name));
    memset(oldname,'\0',sizeof(oldname));
    memset(newname,'\0',sizeof(newname));
    memset(action,'\0',sizeof(action));

    ch = cmd;
    int i = 0;
    while(*ch != split)
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    strcpy(temp,ch);
    p = strchr(temp,split);

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));

    //printf("path = %s\n",path);

    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);
    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        strcpy(oldname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);
    }
    else
    {
        strcpy(filename,p);
        DEBUG("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);
    }

    free(temp);

    if( !strncmp(cmd_name,"rename",strlen("rename")) )
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        old_fullname = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        sprintf(fullname,"%s/%s",path,newname);
        sprintf(old_fullname,"%s/%s",path,oldname);
        free(path);
    }
    else if( !strncmp(cmd_name,"move",strlen("move")) )
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
        old_fullname = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
        sprintf(fullname,"%s/%s",path,oldname);
        sprintf(old_fullname,"%s/%s",oldpath,oldname);
        free(oldpath);
        free(path);
    }
    else
    {
        fullname = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        sprintf(fullname,"%s/%s",path,filename);
        free(path);
    }

    if( !strncmp(cmd_name,"copyfile",strlen("copyfile")) )
    {
        //pthread_mutex_lock(&mutex_copyfilelist);
        add_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        //pthread_mutex_unlock(&mutex_copyfilelist);
        return 0;
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        strcpy(action,"createfile");
        action_item *item;

        //pthread_mutex_lock(&mutex_copyfilelist);
        item = get_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {
            DEBUG("##### delete copyfile %s ######\n",fullname);
            //pthread_mutex_lock(&mutex);
            del_action_item("copyfile",fullname,g_pSyncList[index]->copy_file_list);
        }
        //pthread_mutex_unlock(&mutex_copyfilelist);
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        strcpy(action,"remove");
        del_download_only_action_item(action,fullname,g_pSyncList[index]->download_only_socket_head);
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        strcpy(action,"createfolder");
    }
    else if( strncmp(cmd_name, "rename",6) == 0 )
    {
        strcpy(action,"rename");
        del_download_only_action_item(action,old_fullname,g_pSyncList[index]->download_only_socket_head);
        free(old_fullname);
    }
    else if( strncmp(cmd_name, "move",4) == 0 )
    {
        strcpy(action,"move");
        del_download_only_action_item(action,old_fullname,g_pSyncList[index]->download_only_socket_head);
        //free(old_fullname);
    }

    if(g_pSyncList[index]->server_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,fullname,g_pSyncList[index]->server_action_list,index);

        if(item != NULL)
        {
            DEBUG("##### %s %s by WebDAV Server self ######\n",action,fullname);
            //pthread_mutex_lock(&mutex);
            del_action_item(action,fullname,g_pSyncList[index]->server_action_list);

            //pthread_mutex_unlock(&mutex);
            //local_sync = 0;
            free(fullname);
            return 0;
        }
    }

    if(g_pSyncList[index]->dragfolder_action_list->next != NULL)
    {
        action_item *item;

        item = get_action_item(action,fullname,g_pSyncList[index]->dragfolder_action_list,index);

        if(item != NULL)
        {
            DEBUG("##### %s %s by dragfolder recursion self ######\n",action,fullname);
            //pthread_mutex_lock(&mutex);
            del_action_item(action,fullname,g_pSyncList[index]->dragfolder_action_list);

            //pthread_mutex_unlock(&mutex);
            //local_sync = 0;
            free(fullname);
            return 0;
        }
    }

    if( strcmp(cmd_name, "copyfile") != 0 && g_pSyncList[index]->server_sync_running)
    {
        g_pSyncList[index]->have_local_socket = 1;
    }


    //}
    //printf("add download_only_socket_head fullname = %s\n",fullname);
    if(strncmp(cmd_name, "rename",6) == 0)
    {
        if(test_if_dir(fullname))
        {
            add_all_download_only_socket_list(cmd_name,fullname,index);
        }
        else
        {
            add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
        }
    }
    else if(strncmp(cmd_name, "move",4) == 0)
    {
        int old_index;
        old_index = get_path_to_index(old_fullname);
        if(old_index == index)
        {
            if(test_if_dir(fullname))
            {
                add_all_download_only_socket_list(cmd_name,fullname,index);
            }
            else
            {
                add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
            }
        }
        else
        {
            if(asus_cfg.prule[old_index]->rule == 1)
            {
                del_download_only_action_item("",old_fullname,g_pSyncList[old_index]->download_only_socket_head);
            }
            else
            {
                Delete(old_fullname,old_index);
            }
            if(test_if_dir(fullname))
            {
                add_all_download_only_socket_list(cmd_name,fullname,index);
            }
            else
            {
                add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
            }
        }
        free(old_fullname);

    }
    else if(strcmp(cmd_name, "createfolder") == 0 || strcmp(cmd_name, "dragfolder") == 0)
    {
        add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
        add_all_download_only_dragfolder_socket_list(fullname,index);
    }
    else if( strcmp(cmd_name, "createfile") == 0  || strcmp(cmd_name, "dragfile") == 0 || strcmp(cmd_name, "modify") == 0)
    {
        add_action_item(cmd_name,fullname,g_pSyncList[index]->download_only_socket_head);
    }

    free(fullname);
    return 0;
}

static int startelm(void *userdata, int state,
                    const char *nspace, const char *name,
                    const char **atts)
{
    //printf("startelm\n");
    struct context *ctx = userdata;
    ne_buffer *buf = ctx->buf;
    //printf("buf->data = %s\n",buf->data);
    //printf("nspace = %s\n",nspace);
    //printf("name = %s\n",name);
    int n;

    if (strcmp(name, "decline") == 0)
        return NE_XML_DECLINE;

    if (strcmp(name, EVAL_DEFAULT) == 0) {
        //printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
        const char *val = ne_xml_resolve_nspace(ctx->parser, NULL, 0);
        //printf("val = %s\n",val);

        ne_buffer_concat(ctx->buf, EVAL_DEFAULT "=[", val, "]", NULL);
        return NE_XML_DECLINE;
    }
    else if (strncmp(name, EVAL_SPECIFIC, strlen(EVAL_SPECIFIC)) == 0) {
        //printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
        const char *which = name + strlen(EVAL_SPECIFIC);
        const char *r = ne_xml_resolve_nspace(ctx->parser, which, strlen(which));

        ne_buffer_concat(ctx->buf, name, "=[", r, "]", NULL);
        return NE_XML_DECLINE;
    }

    ne_buffer_concat(buf, "<", name, NULL);
    for (n = 0; atts && atts[n] != NULL; n+=2) {
        ne_buffer_concat(buf, " ", atts[n], "='", atts[n+1], "'", NULL);
    }
    ne_buffer_zappend(buf, ">");

    //printf("buf->data = %s\n",buf->data);
    //printf("startelm finished!\n");

    return state + 1;
}

static int chardata(void *userdata, int state, const char *cdata, size_t len)
{
    struct context *ctx = userdata;
    ne_buffer_append(ctx->buf, cdata, len);
    return strncmp(cdata, "!ABORT!", len) == 0 ? ABORT : NE_XML_DECLINE;
}

static int endelm(void *userdata, int state,
                  const char *nspace, const char *name)
{
    //printf("endelm\n");
    struct context *ctx = userdata;
    ne_buffer_concat(ctx->buf, "</", name, ">", NULL);
    return 0;
}

/*parse CloudeInfo to tree_ServerList*/
void parseCloudInfo_tree(xmlDocPtr doc, xmlNodePtr cur)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(!(xmlStrcmp(cur->name, (const xmlChar *)"response")))
        {
            parseCloudInfo_tree(doc,cur);
            if(FolderTmp->isFolder == 3)
            {
                free_CloudFile_item(FolderTmp);
            }
            /*else
            {
                printf("get the info right!\n");
            }*/
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"propstat")))
            parseCloudInfo_tree(doc,cur);
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"prop")))
            parseCloudInfo_tree(doc,cur);
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"href")))
        {
            FolderTmp = (CloudFile *)malloc(sizeof(CloudFile));
            memset(FolderTmp,0,sizeof(CloudFile));
            FolderTmp->next = NULL;
            FolderTmp->href = my_str_malloc(strlen(key)+1);
            //memset(FolderTmp->href,'\0',sizeof(FolderTmp->href));
            strcpy(FolderTmp->href,key);
            FolderTmp->isFolder = 3;
            FolderTmp->ismodify = 0;
            //printf("href = %s\n",FolderTmp->href);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"creationdate")))
        {
            strcpy(FolderTmp->creationdate,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontentlanguage")))
        {
            strcpy(FolderTmp->getcontentlanguage,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontentlength")))
        {
            strcpy(FolderTmp->getcontentlength,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontenttype")))
        {
            strcpy(FolderTmp->getcontenttype,key);
            if(!(strcmp(FolderTmp->getcontenttype,"httpd/unix-directory")))
                FolderTmp->isFolder = 1;
            else
                FolderTmp->isFolder = 0;
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getlastmodified")))
        {
            strcpy(FolderTmp->getlastmodified,key);
            //DEBUG("(FolderTmp->getlastmodified = %s\n",FolderTmp->getlastmodified);
            FolderTmp->modtime = ne_httpdate_parse(FolderTmp->getlastmodified);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            strcpy(FolderTmp->status,key);
            if(FolderTmp->isFolder == 1)
            {
                TreeFolderTail->next = FolderTmp;
                TreeFolderTail = FolderTmp;
                TreeFolderTail->next = NULL;
                //printf("status = %s\n",FolderTail->status);
            }
            else if(FolderTmp->isFolder == 0)
            {
                TreeFileTail->next = FolderTmp;
                TreeFileTail = FolderTmp;
                TreeFileTail->next = NULL;
                //printf("status = %s\n",FileTail->status);
            }
        }
        //else if(!(xmlStrcmp(cur->name, (const xmlChar *)"/response")))
        //printf("RRRRRRRRRRRRRRRRRRRRRRRRresponse over!\n");

        //printf("cur->name = %s\n",cur->name);
        //printf("key = %s\n",key);

        xmlFree(key);
        cur = cur->next;
    }
}

/*parse CloudeInfo to one_ServerList*/
void parseCloudInfo_one(xmlDocPtr doc, xmlNodePtr cur)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        //printf("cur->name = %s\n",cur->name);
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if(!(xmlStrcmp(cur->name, (const xmlChar *)"response")))
        {
            parseCloudInfo_one(doc,cur);
            if(FileTmp_one->isFolder == 3)
            {
                free_CloudFile_item(FileTmp_one);
            }
            /*else
            {
                printf("get the info right!\n");
            }*/
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"propstat")))
            parseCloudInfo_one(doc,cur);
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"prop")))
            parseCloudInfo_one(doc,cur);
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"href")))
        {
            FileTmp_one = (CloudFile *)malloc(sizeof(CloudFile));
            memset(FileTmp_one,0,sizeof(CloudFile));
            FileTmp_one->next = NULL;
            FileTmp_one->href = my_str_malloc(strlen(key)+1);
            //memset(FileTmp_one->href,'\0',sizeof(FileTmp_one->href));
            strcpy(FileTmp_one->href,key);
            FileTmp_one->isFolder = 3;
            FileTmp_one->ismodify = 0;
            //printf("href = %s\n",FileTmp_one->href);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"creationdate")))
        {
            strcpy(FileTmp_one->creationdate,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontentlanguage")))
        {
            strcpy(FileTmp_one->getcontentlanguage,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontentlength")))
        {
            strcpy(FileTmp_one->getcontentlength,key);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getcontenttype")))
        {
            strcpy(FileTmp_one->getcontenttype,key);
            if(!(strcmp(FileTmp_one->getcontenttype,"httpd/unix-directory")))
                FileTmp_one->isFolder = 1;
            else
                FileTmp_one->isFolder = 0;
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"getlastmodified")))
        {
            strcpy(FileTmp_one->getlastmodified,key);
            FileTmp_one->modtime = ne_httpdate_parse(FileTmp_one->getlastmodified);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"status")))
        {
            strcpy(FileTmp_one->status,key);
            FileTail_one->next = FileTmp_one;
            FileTail_one = FileTmp_one;
            FileTail_one->next = NULL;
        }

        //printf("cur->name = %s\n",cur->name);
        //printf("key = %s\n",key);

        xmlFree(key);
        cur = cur->next;
    }
}

void parseRouterInfo(xmlDocPtr doc, xmlNodePtr cur)
{
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        //printf("key = %s\n",key);

        if(!(xmlStrcmp(cur->name, (const xmlChar *)"result")))
        {
            parseRouterInfo(doc,cur);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"disk_space")))
        {
            parseRouterInfo(doc,cur);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"item")))
        {
            parseRouterInfo(doc,cur);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"DiskAvailable")))
        {
            //printf("key = %s\n",key);
            DiskAvailable = 1024*(atoll(key));
            //printf("DiskAvailable = %lld\n",DiskAvailable);
            DiskAvailableShow = (atoll(key))/1024;
            //printf("DiskAvailableShow = %lld\n",DiskAvailableShow);
        }
        else if(!(xmlStrcmp(cur->name, (const xmlChar *)"DiskUsed")))
        {
            //printf("key = %s\n",key);
            DiskUsedShow = (atoll(key))/1024;
            //printf("DiskUsedShow = %lld\n",DiskUsedShow);
        }

        xmlFree(key);
        cur = cur->next;
    }
}

/*used for get XML root node*/
int my_parseMemory(char *parseBuf,int parseBufLength,void (*cmd_data)(xmlDocPtr, xmlNodePtr)){
    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseMemory(parseBuf,parseBufLength);

    if (doc == NULL ) {
        DEBUG("parsed failed. \n");
        return -1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        DEBUG("empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }

    while(cur != NULL)
    {
        //DEBUG("cur->name = %s\n",cur->name);
        cmd_data(doc, cur);
        cur = cur->next;
    }

    xmlFreeDoc(doc);


    return 0;
}

int ne_getrouterinfo(void (*cmd_data)(xmlDocPtr, xmlNodePtr),int index)
{
    struct context ctx;
    ne_uri uri = {0};
    DEBUG("asus_cfg.prule[index]->rooturl = %s\n",asus_cfg.prule[index]->rooturl);
    if (ne_uri_parse(asus_cfg.prule[index]->rooturl, &uri) || uri.host==NULL || uri.path==NULL)
    {
        printf("Could not parse url %s\n", asus_cfg.prule[index]->rooturl);
        return -1;
    }

    ne_xml_parser *p;
    ne_buffer *buf1 = ne_buffer_create();
    p = ne_xml_create();

    ctx.buf = buf1;
    ctx.parser = p;
    //printf("uri.path = %s\n",uri.path);
    ne_request *req = ne_request_create(asus_cfg.prule[index]->sess,"GETROUTERINFO",uri.path);

    int ret;

    ne_xml_push_handler(p, startelm, chardata, endelm, &ctx);
    //printf("uri.path = %s\n",uri.path);
    ret = ne_xml_dispatch_request(req, p);
    //printf("ne_getrouterinfo ret = %d\n",ret);

    if(ret == NE_OK && ne_get_status(req)->klass != 2)
    {
        ret = NE_ERROR;
    }

    if(ret != NE_OK)
    {
        ne_xml_destroy(p);
        ne_buffer_destroy(buf1);
        ne_request_destroy(req);
        ne_uri_free(&uri);
        char error_info[200];
        memset(error_info,0,sizeof(error_info));
        sprintf(error_info,"%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("ERROR info :%s\n ret = %d\n",error_info,ret);

        if(ret == 3)
        {
            //write_log(S_ERROR,"Validation Failed!","",index);
            return INVALID_ARGUMENT;
        }
        if(strstr(error_info,"Could not connect to server") != NULL)
        {
            //write_log(S_ERROR,"Could not connect to server!","",index);
            return COULD_NOT_CONNECNT_TO_SERVER;
        }
        return ret;
    }
    //printf("ret = %d\n",ret);

    //printf("ctx->buf->data1 = %s\n",ctx.buf->data);
    ret = my_parseMemory(ctx.buf->data,(int)ctx.buf->length,cmd_data);
    if(ret != 0)
    {
        DiskAvailable = -1;
        DiskAvailableShow = -1;
        DiskUsedShow = -1;
        ne_xml_destroy(p);
        ne_buffer_destroy(buf1);
        ne_uri_free(&uri);
        ne_request_destroy(req);
        return PARSE_XML_FAILED;
    }
    else
    {
        PreDiskAvailableShow = DiskAvailableShow;
        PreDiskUsedShow = DiskUsedShow;
    }

    ne_xml_destroy(p);
    ne_buffer_destroy(buf1);
    ne_uri_free(&uri);
    ne_request_destroy(req);
    //printf("getCloudInfo finished!\n");
    return ret;
}

int getCloudInfo(char *URL,void (*cmd_data)(xmlDocPtr, xmlNodePtr),int index){
    //printf("****************getCloudInfo****************\n");
    //printf("URL = %s\n",URL);
    struct context ctx;
    //printf("%s\n",URL);
    ne_uri uri = {0};
    //printf("URL = %s\n",URL);
    if (ne_uri_parse(URL, &uri) || uri.host==NULL || uri.path==NULL)
    {
        DEBUG("Could not parse url %s\n", URL);
        return -1;
    }

    //printf("uri.scheme = %s\n",uri.scheme);
    ne_xml_parser *p;
    ne_buffer *buf1 = ne_buffer_create();
    p = ne_xml_create();
    //struct context ctx;

    ctx.buf = buf1;
    ctx.parser = p;

    ne_request *req = ne_request_create(asus_cfg.prule[index]->sess,"PROPFIND",uri.path);

    int ret;

    char buf[128];
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<propfind xmlns=\"DAV:\"><allprop/></propfind>\n");

    ne_add_request_header(req,"Depth","1");
    ne_add_request_header(req,"Content-Type","application/xml");
    //ne_add_request_header(req,"User-Agent","webdav_client1.0.0");
    ne_set_request_body_buffer(req,buf,84);

    ne_xml_push_handler(p, startelm, chardata, endelm, &ctx);
    ret = ne_xml_dispatch_request(req, p);

    if(ret == NE_OK && ne_get_status(req)->klass != 2)
    {
        ret = NE_ERROR;
    }

    //ret = ne_xml_parse_response(req, p);
    //printf("ERROR info :%s\n",ne_get_error(sess));
    //printf("ret = %d\n",ret);
    if(ret != NE_OK){
        ne_xml_destroy(p);
        ne_buffer_destroy(buf1);
        ne_request_destroy(req);
        ne_uri_free(&uri);
        //printf("getCloudInfo finished!\n");
        char error_info[200];
        memset(error_info,0,sizeof(error_info));
        sprintf(error_info,"%s",ne_get_error(asus_cfg.prule[index]->sess));

        DEBUG("ERROR info :%s\n ret = %d\n",error_info,ret);

        //write_log(S_ERROR,error_info,"",index);
        if(ret == 3)
        {
            //write_log(S_ERROR,"Validation Failed!","",index);
            return INVALID_ARGUMENT;
        }
        if(strstr(error_info,"Could not connect to server") != NULL)
        {
            //write_log(S_ERROR,"Could not connect to server!","",index);
            return COULD_NOT_CONNECNT_TO_SERVER;
        }
        if(strstr(error_info,"404") != NULL)
        {
            write_log(S_ERROR,"Server Deleted Sync Folder!","",index);
            return SERVER_ROOT_DELETED;
        }
        return ret;
    }

    ret = my_parseMemory(ctx.buf->data,(int)ctx.buf->length,cmd_data);

    ne_xml_destroy(p);
    ne_buffer_destroy(buf1);
    ne_uri_free(&uri);
    ne_request_destroy(req);
    //printf("getCloudInfo finished!\n");
    return ret;
}

Browse *browseFolder(char *URL,int index){
    //printf("browseFolder URL = %s\n",URL);
    int status;
    int i=0;

    Browse *browse = getb(Browse);
    if( NULL == browse )
    {
        printf("create memery error\n");
        exit(-1);
    }
    memset(browse,0,sizeof(Browse));

    TreeFolderList = (CloudFile *)malloc(sizeof(CloudFile));
    memset(TreeFolderList,0,sizeof(CloudFile));
    TreeFileList = (CloudFile *)malloc(sizeof(CloudFile));
    memset(TreeFileList,0,sizeof(CloudFile));

    TreeFolderList->href = NULL;
    TreeFileList->href = NULL;

    TreeFolderTail = TreeFolderList;
    TreeFileTail = TreeFileList;
    TreeFolderTail->next = NULL;
    TreeFileTail->next = NULL;

    status = getCloudInfo(URL,parseCloudInfo_tree,index);

    if(status != 0)
    {
        free_CloudFile_item(TreeFolderList);
        free_CloudFile_item(TreeFileList);
        TreeFolderList = NULL;
        TreeFileList = NULL;
        free(browse);
        //printf("get Cloud Info ERROR! \n");
        return NULL;
    }

    browse->filelist = TreeFileList;
    browse->folderlist = TreeFolderList;

    CloudFile *de_foldercurrent,*de_filecurrent;
    de_foldercurrent = TreeFolderList->next;
    de_filecurrent = TreeFileList->next;
    while(de_foldercurrent != NULL){
        ++i;
        de_foldercurrent = de_foldercurrent->next;
    }
    browse->foldernumber = i;
    i = 0;
    while(de_filecurrent != NULL){
        ++i;
        de_filecurrent = de_filecurrent->next;
    }
    browse->filenumber = i;
    return browse;
}

#if 1
int send_action(int type, char *content)
{
    if(exit_loop)
    {
        return 0;
    }
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    char str[1024];
    int port;

    //if(type == 1)
    port = INOTIFY_PORT;

    struct sockaddr_in their_addr; /* connector's address information */


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        //exit(1);
        return -1;
    }

    bzero(&(their_addr), sizeof(their_addr)); /* zero the rest of the struct */
    their_addr.sin_family = AF_INET; /* host byte order */
    their_addr.sin_port = htons(port); /* short, network byte order */
    their_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //their_addr.sin_addr.s_addr = ((struct in_addr *)(he->h_addr))->s_addr;
    bzero(&(their_addr.sin_zero), sizeof(their_addr.sin_zero)); /* zero the rest of the struct */
    if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct
                                                              sockaddr)) == -1) {
        perror("connect");
        //exit(1);
        return -1;
    }

    sprintf(str,"%d@%s",type,content);

    //printf("send content is %s \n",str);

    if (send(sockfd, str, strlen(str), 0) == -1) {
        perror("send");
        //exit(1);
        return -1;
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        //exit(1);
        return -1;
    }

    buf[numbytes] = '\0';
    close(sockfd);

    DEBUG("send_action finished!\n");
    return 0;
}
#endif

/*char *get_socket_base_path(char *cmd){

    //printf("get_socket_base_path cmd : %s\n",cmd);

    char *temp = NULL;
    char *temp1 = NULL;
    char path[1024];
    char *root_path = NULL;
    const char split = '\n';

    if(!strncmp(cmd,"rmroot",6))
    {
        temp = strchr(cmd,'/');
        root_path = my_str_malloc(512);
        sprintf(root_path,"%s",temp);
    }
    else
    {
        temp = strchr(cmd,'/');
        temp1 = strchr(temp,split);
        memset(path,0,sizeof(path));
        strncpy(path,temp,strlen(temp)-strlen(temp1));

        //printf("get_socket_base_path path = %s\n",path);

        root_path = my_str_malloc(512);

        if(strncmp(path,"/tmp",4))
        {
            temp = my_nstrchr('/',path,4);
        }
        else
        {
            temp = my_nstrchr('/',path,5);
        }

        if(temp == NULL)
        {
            sprintf(root_path,"%s",path);
        }
        else
        {
            snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
        }
    }
    //printf("get_socket_base_path root_path = %s\n",root_path);
    return root_path;
}*/

int get_socket_base_path(char *cmd){

    DEBUG("get_socket_base_path cmd : %s\n",cmd);

    char *temp = NULL;
    char *temp1 = NULL;
    char path[1024];
    //char *root_path = NULL;
    const char split = '\n';
    int ret;

    memset(path,0,1024);

    if(!strncmp(cmd,"rmroot",6))
    {
        temp = strchr(cmd,'/');
        //root_path = my_str_malloc(512);
        sprintf(path,"%s",temp);
        ret = get_path_to_index(path);
    }
    else
    {
        temp = strchr(cmd,'/');
        temp1 = strchr(temp,split);
        memset(path,0,sizeof(path));
        strncpy(path,temp,strlen(temp)-strlen(temp1));

        //printf("get_socket_base_path path = %s\n",path);

        /*root_path = my_str_malloc(512);

        if(strncmp(path,"/tmp",4))
        {
            temp = my_nstrchr('/',path,4);
        }
        else
        {
            temp = my_nstrchr('/',path,5);
        }

        if(temp == NULL)
        {
            sprintf(root_path,"%s",path);
        }
        else
        {
            snprintf(root_path,strlen(path)-strlen(temp)+1,"%s",path);
        }*/
        ret = get_path_to_index(path);
    }
    //DEBUG("get_socket_base_path ret = %d\n",ret);
    return ret;
}

/*
 *pri = 0,add to SocketActionList
 *pri = 1,add to SocketActionList_Priority
*/
int add_socket_item(char *buf,int pri,int i){

    pthread_mutex_lock(&mutex_receve_socket);
    //receve_socket = 1;
    g_pSyncList[i]->receve_socket = 1;
    pthread_mutex_unlock(&mutex_receve_socket);

    SocketActionTmp = malloc (sizeof (struct queue_entry));
    memset(SocketActionTmp,0,sizeof(struct queue_entry));
    sprintf(SocketActionTmp->cmd_name,"%s",buf);
    if(pri)
    {
        queue_enqueue(SocketActionTmp,g_pSyncList[i]->SocketActionList_Priority);
    }
    else
    {
        queue_enqueue(SocketActionTmp,g_pSyncList[i]->SocketActionList);
    }
    //queue_enqueue(SocketActionTmp,g_pSyncList[i]->SocketActionList);

    DEBUG("SocketActionTmp->cmd_name = %s\n",SocketActionTmp->cmd_name);

    //free(r_path);
    return 0;
}

int reset_socket(char *newpath_full,char *newpath_part,char *oldpath_full,char *oldpath_part,
                 int index,char *newpath_normal,char *oldpath_normal)
{
    //char *oldpath_tmp;
    //oldpath_tmp = my_str_malloc(strlen(oldpath)+2);
    //sprintf(oldpath_tmp,"%s/",oldpath);
    int i;
    char *change_start;
    char *change_stop;
    char new_cmd_name[1024];
    char tmp_cmd_name[1024];
    const char *split = "\n";
    char *p;

    queue_entry_t socket_execute;
    socket_execute = g_pSyncList[index]->SocketActionList->head;
    while(socket_execute != NULL)
    {
        change_start = NULL;
        change_stop = NULL;
        if((change_start = strstr(socket_execute->cmd_name,oldpath_full)) != NULL)
        {

            i = 0;
            memset(new_cmd_name,0,1024);
            memset(tmp_cmd_name,0,1024);
            //tmp_cmd_name = my_str_malloc(strlen(socket_execute->cmd_name)+1);
            //sprintf(tmp_cmd_name,"%s",socket_execute->cmd_name);
            strcpy(tmp_cmd_name,socket_execute->cmd_name);
            //printf("!!!!!!!!!!!!!!!!!!!!reset_socket tmp_cmd_name\n%s\n",tmp_cmd_name);
            p = strtok(tmp_cmd_name,split);

            while(p!=NULL)
            {
                //printf("reset_socket strtok\n%s\n",p);
                if(strcmp(p,oldpath_normal))   //have no oldpath_full
                {
                    if(i == 0)
                    {
                        i++;
                        sprintf(new_cmd_name,"%s",p);
                    }
                    else
                    {
                        sprintf(new_cmd_name,"%s\n%s",new_cmd_name,p);
                    }
                }
                else    //have oldpath_full
                {
                    if(i == 0)
                    {
                        i++;
                        sprintf(new_cmd_name,"%s",newpath_normal);
                    }
                    else
                    {
                        sprintf(new_cmd_name,"%s\n%s",new_cmd_name,newpath_normal);
                    }
                }

                p=strtok(NULL,split);
            }
            //free(tmp_cmd_name);
//            change_stop = change_start + strlen(oldpath_full);
//            snprintf(new_cmd_name,strlen(socket_execute->cmd_name)-strlen(change_start)+1,"%s",socket_execute->cmd_name);
//            strcat(new_cmd_name,newpath_full);
//            strcat(new_cmd_name,change_stop);
            memset(socket_execute->cmd_name,0,1024);
            sprintf(socket_execute->cmd_name,"%s",new_cmd_name);
            printf("!!!!!!!!!!!!!!!!!!!!!!!new socket_execute->cmd_name = %s\n",socket_execute->cmd_name);
        }

        else if((change_start = strstr(socket_execute->cmd_name,oldpath_part)) != NULL)
        {
            i = 0;
            memset(new_cmd_name,0,1024);
            memset(tmp_cmd_name,0,1024);
            //tmp_cmd_name = my_str_malloc(strlen(socket_execute->cmd_name)+1);
            sprintf(tmp_cmd_name,"%s",socket_execute->cmd_name);
            p = strtok(tmp_cmd_name,split);
            while(p!=NULL)
            {
                //printf("reset_socket strtok\n%s\n",p);
                if(strncmp(p,oldpath_part,strlen(oldpath_part)))   //have no oldpath_part
                {
                    if(i == 0)
                    {
                        i++;
                        sprintf(new_cmd_name,"%s",p);
                    }
                    else
                    {
                        sprintf(new_cmd_name,"%s\n%s",new_cmd_name,p);
                    }
                }
                else    //have oldpath_part
                {
                    change_stop = p + strlen(oldpath_part);
                    if(i == 0)
                    {
                        i++;
                        sprintf(new_cmd_name,"%s%s",newpath_part,change_stop);
                    }
                    else
                    {
                        sprintf(new_cmd_name,"%s\n%s%s",new_cmd_name,newpath_part,change_stop);
                    }
                }

                //free(p);
                p=strtok(NULL,split);
            }
            //free(tmp_cmd_name);
            memset(socket_execute->cmd_name,0,1024);
            sprintf(socket_execute->cmd_name,"%s",new_cmd_name);
//            change_stop = change_start + strlen(oldpath_part);
//            snprintf(new_cmd_name,strlen(socket_execute->cmd_name)-strlen(change_start)+1,"%s",socket_execute->cmd_name);
//            strcat(new_cmd_name,newpath_part);
//            strcat(new_cmd_name,change_stop);
//            memset(socket_execute->cmd_name,0,1024);
//            sprintf(socket_execute->cmd_name,"%s",new_cmd_name);
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!new cme name\n%s",socket_execute->cmd_name);
        }
        socket_execute = socket_execute->next_ptr;
    }

    action_item *item;
    //pthread_mutex_lock(&mutex_copyfilelist);
    item = g_pSyncList[index]->copy_file_list->next;
    while(item != NULL)
    {
        printf("\nitem->path = %s\n",item->path);
        printf("\noldpath_part = %s\n",oldpath_part);
        //if((change_start = strstr(item->path,oldpath_part)) != NULL)
        if(strncmp(item->path,oldpath_part,strlen(oldpath_part)) == 0)
        {
            memset(new_cmd_name,0,1024);
            change_stop = item->path + strlen(oldpath_part);
            sprintf(new_cmd_name,"%s%s",newpath_part,change_stop);

            free(item->path);
            item->path = NULL;
            item->path = my_str_malloc(strlen(new_cmd_name)+1);
            sprintf(item->path,"%s",new_cmd_name);

//            change_stop = change_start + strlen(oldpath_part);
//            snprintf(new_cmd_name,strlen(item->path)-strlen(change_start)+1,"%s",item->path);
//            strcat(new_cmd_name,newpath_part);
//            strcat(new_cmd_name,change_stop);
//            free(item->path);
//            item->path = NULL;
//            item->path = my_str_malloc(strlen(new_cmd_name)+1);
//            sprintf(item->path,"%s",new_cmd_name);
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!new item path = %s",item->path);
        }
        item = item->next;
    }
    //pthread_mutex_unlock(&mutex_copyfilelist);
}

/*
 *pri = 0,rename0
 *pri = 1,move0
*/
int handle_pri_socket(char *s_buf,int pri ,int index)
{
    char *p,*q;
    char split = '\n';
    char path0[512];
    char path1[512];
    char path2[256];

    char *newpath_full,*newpath_part;
    char *oldpath_full,*oldpath_part;
    char *newpath_normal,*oldpath_normal;

    bzero(path0,512);
    bzero(path1,512);
    bzero(path2,256);

    p = strchr(s_buf,split);
    p++;

    q = strchr(p,split);

    if(p!=NULL)
    {
        strncpy(path0,p,strlen(p)-strlen(q));
    }
    else
    {
        return -1;
    }

    q++;
    p = strchr(q,split);

    if(q != NULL)
    {
        strncpy(path1,q,strlen(q)-strlen(p));
    }
    else
    {
        return -1;
    }

    p++;
    if(p != NULL)
    {
        strncpy(path2,p,strlen(p));
    }
    else
    {
        return -1;
    }

    DEBUG("path0 = %s\n",path0);
    DEBUG("path1 = %s\n",path1);
    DEBUG("path2 = %s\n",path2);

    if(pri)    //move0
    {
        newpath_full = my_str_malloc(strlen(path0)+strlen(path2)+3);
        newpath_part = my_str_malloc(strlen(path0)+strlen(path2)+3);
        oldpath_full = my_str_malloc(strlen(path1)+strlen(path2)+3);
        oldpath_part = my_str_malloc(strlen(path1)+strlen(path2)+3);
        newpath_normal = my_str_malloc(strlen(path0)+strlen(path2)+2);
        oldpath_normal = my_str_malloc(strlen(path1)+strlen(path2)+2);
        sprintf(newpath_full,"%s/%s\n",path0,path2);
        sprintf(newpath_part,"%s/%s/",path0,path2);
        sprintf(oldpath_full,"%s/%s\n",path1,path2);
        sprintf(oldpath_part,"%s/%s/",path1,path2);
        sprintf(newpath_normal,"%s/%s",path0,path2);
        sprintf(oldpath_normal,"%s/%s",path1,path2);
    }
    else       //rename0
    {
        newpath_full = my_str_malloc(strlen(path0)+strlen(path2)+3);
        newpath_part = my_str_malloc(strlen(path0)+strlen(path2)+3);
        oldpath_full = my_str_malloc(strlen(path0)+strlen(path1)+3);
        oldpath_part = my_str_malloc(strlen(path0)+strlen(path1)+3);
        newpath_normal = my_str_malloc(strlen(path0)+strlen(path2)+2);
        oldpath_normal = my_str_malloc(strlen(path0)+strlen(path1)+2);
        sprintf(newpath_full,"%s/%s\n",path0,path2);
        sprintf(newpath_part,"%s/%s/",path0,path2);
        sprintf(oldpath_full,"%s/%s\n",path0,path1);
        sprintf(oldpath_part,"%s/%s/",path0,path1);
        sprintf(newpath_normal,"%s/%s",path0,path2);
        sprintf(oldpath_normal,"%s/%s",path0,path1);
    }

    printf("newpath_full = %s\n",newpath_full);
    printf("newpath_part = %s\n",newpath_part);
    printf("oldpath_full = %s\n",oldpath_full);
    printf("oldpath_part = %s\n",oldpath_part);
    printf("newpath_normal = %s\n",newpath_normal);
    printf("oldpath_normal = %s\n",oldpath_normal);

    pthread_mutex_lock(&mutex_socket);
    reset_socket(newpath_full,newpath_part,oldpath_full,oldpath_part,index,newpath_normal,oldpath_normal);
    pthread_mutex_unlock(&mutex_socket);


    free(newpath_full);
    free(oldpath_full);
    free(newpath_part);
    free(oldpath_part);
    free(newpath_normal);
    free(oldpath_normal);
    return 0;
}

#if 1
void *SyncLocal()
{

    int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd*/
    int numbytes;
    char buf[MAXDATASIZE];
    int yes = 1;
    int ret;

    fd_set read_fds;
    fd_set master;
    int fdmax;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_ZERO(&master);

    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    int sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
    	perror("Server-setsockopt() error lol!");
    	exit(1);
    }

    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), sizeof(my_addr.sin_zero)); /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct
                                                         sockaddr))== -1) {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sin_size = sizeof(struct sockaddr_in);

    FD_SET(sockfd,&master);
    fdmax = sockfd;

    while(!exit_loop)
    { /* main accept() loop */

    	//printf("it is go to SyncLocal's while\n");

        //if(exit_proc == 1)
        //exit(1);
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;

        read_fds = master;

        ret = select(fdmax+1,&read_fds,NULL,NULL,&timeout);

        switch (ret)
        {
        case 0:
            //printf("No data in ten seconds\n");
            continue;
            break;
        case -1:
            perror("select");
            continue;
            break;
        default:
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
                                 &sin_size)) == -1) {
                perror("accept");
                continue;
            }
            memset(buf, 0, sizeof(buf));

            if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
                perror("recv");
                exit(1);
            }

            if(buf[strlen(buf)] == '\n')
            {
                buf[strlen(buf)] = '\0';
            }

            close(new_fd);
            
            //if(sync_down == 1)
            //{
            int i;
            //local_sync = 1;
            //char *r_path;
            i = get_socket_base_path(buf);

            //DEBUG("add_socket_item r_path = %s\n",r_path);

            /*for(i=0;i<asus_cfg.dir_number;i++)
            {
                if(!strcmp((asus_cfg.prule[i]->base_path),r_path))
                    break;
            }
            free(r_path);*/

            if(!strncmp(buf,"rename0",7))
            {
                handle_pri_socket(buf,0,i);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(buf,1,i);
                pthread_mutex_unlock(&mutex_socket);
            }
            else if(!strncmp(buf,"move0",5))
            {
                handle_pri_socket(buf,1,i);
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(buf,1,i);
                pthread_mutex_unlock(&mutex_socket);
            }
            else
            {
                pthread_mutex_lock(&mutex_socket);
                add_socket_item(buf,0,i);
                pthread_mutex_unlock(&mutex_socket);
            }
            DEBUG("add %s OK \n",buf);
            //}
        }


        //printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));

    }
    close(sockfd);
    DEBUG("stop WebDAV local sync\n");
    stop_down = 1;
}
#endif

/*
int is_rename_folder(char *cmd)
{
    DEBUG("socket command is %s \n",cmd);

    if( !strncmp(cmd,"exit",4))
    {
        DEBUG("exit socket\n");
        return 0;
    }

    if(!strncmp(cmd,"rmroot",6))
    {
        g_pSyncList[index]->no_local_root = 1;
        return 0;
    }

    const char split = '\n';
    char cmd_name[64];
    //char cmd_param[512];
    char *path;
    char *temp;
    char filename[256];
    char *fullname;
    char oldname[256],newname[256];
    char *oldpath;
    char action[64];
    char *cmp_name;
    char *mv_newpath;
    char *mv_oldpath;
    char *ch;
    int status;
    //Asusconfig asusconfig;

    memset(cmd_name, 0, sizeof(cmd_name));
    memset(oldname,'\0',sizeof(oldname));
    memset(newname,'\0',sizeof(newname));
    memset(action,0,sizeof(action));

    ch = cmd;
    int i = 0;
    while(*ch != split)
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    temp = my_str_malloc((size_t)(strlen(ch)+1));

    strcpy(temp,ch);
    p = strchr(temp,split);

    path = my_str_malloc((size_t)(strlen(temp)- strlen(p)+1));


    if(p!=NULL)
        snprintf(path,strlen(temp)- strlen(p)+1,"%s",temp);

    p++;
    if(strcmp(cmd_name, "rename") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);

        if(newname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            return 0;
        }
    }
    else if(strcmp(cmd_name, "move") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        oldpath = my_str_malloc((size_t)(strlen(p)- strlen(p1)+1));

        if(p1 != NULL)
            snprintf(oldpath,strlen(p)- strlen(p1)+1,"%s",p);

        p1++;

        strcpy(oldname,p1);

        DEBUG("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);

        if(oldname[0] == '.' || (strstr(path,"/.")) != NULL)
        {
            free(temp);
            free(path);
            free(oldpath);
            return 0;
        }
    }
    else
    {
        return 0;
    }

    free(temp);


    if( !strcmp(cmd_name,"rename") )
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
        sprintf(cmp_name,"%s/%s",path,newname);
    }
    else
    {
        cmp_name = my_str_malloc((size_t)(strlen(path)+strlen(filename)+2));
        sprintf(cmp_name,"%s/%s",path,filename);
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        strcpy(action,"createfile");
        action_item *item;

        item = get_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list,index);

        if(item != NULL)
        {
            DEBUG("##### delete copyfile %s ######\n",cmp_name);
            //pthread_mutex_lock(&mutex);
            del_action_item("copyfile",cmp_name,g_pSyncList[index]->copy_file_list);
        }
    }

    if(strcmp(cmd_name, "move") == 0 || strcmp(cmd_name, "rename") == 0)
    {
        if(strcmp(cmd_name, "move") == 0)
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(oldpath)+strlen(oldname)+2));
            sprintf(mv_newpath,"%s/%s",path,oldname);
            sprintf(mv_oldpath,"%s/%s",oldpath,oldname);
            free(oldpath);
        }
        else
        {
            mv_newpath = my_str_malloc((size_t)(strlen(path)+strlen(newname)+2));
            mv_oldpath = my_str_malloc((size_t)(strlen(path)+strlen(oldname)+2));
            sprintf(mv_newpath,"%s/%s",path,newname);
            sprintf(mv_oldpath,"%s/%s",path,oldname);
        }
    }




    free(cmp_name);
    return 0;
}
*/

void *Socket_Parser(){

    DEBUG("*******Socket_Parser start********\n");

    queue_entry_t socket_execute;
    int status = 0;
    //int mysync = 1;
    int has_socket = 0;
    int i;
    struct timeval now;
    struct timespec outtime;
    int fail_flag;
    char *cmd = NULL;

    while(!exit_loop)
    {
        //local_sync = 1;

        //printf("*******Socket_Parser start loop********\n");
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            fail_flag = 0;
            //pthread_mutex_lock(&mutex_sync);
            while (server_sync == 1 && exit_loop ==0)
            {
                //printf("server sync = %d\n",server_sync);
                usleep(1000*10);
            }
            local_sync = 1;
            //pthread_mutex_unlock(&mutex_sync);
            if(disk_change)
            {
                //disk_change = 0;
                check_disk_change();
            }

            if(exit_loop)
                break;

            if(g_pSyncList[i]->sync_disk_exist == 0)
                continue;

            /*if(g_pSyncList[i]->no_local_root)               //move from syncserver
            {
                printf("@@@@@@@@@@@@@@@make root dir \n");
                my_mkdir_r(asus_cfg.prule[i]->base_path,i);   //have mountpath
                send_action(1,asus_cfg.prule[i]->base_path);
                usleep(1000*10);
                g_pSyncList[i]->no_local_root = 0;
                g_pSyncList[i]->init_completed = 0;
            }*/

            if(asus_cfg.prule[i]->rule == 1)    //Dwonload Only
            {

                while(exit_loop ==0)
                {
                    while((g_pSyncList[i]->SocketActionList->head != NULL
                           || g_pSyncList[i]->SocketActionList_Priority->head != NULL)
                          && exit_loop == 0 && server_sync == 0)
                    {
                        while(g_pSyncList[i]->SocketActionList_Priority->head != NULL
                              && exit_loop == 0 && server_sync == 0)
                        {
                            has_socket = 1;
                            socket_execute = g_pSyncList[i]->SocketActionList_Priority->head;
                            cmd = my_str_malloc(strlen(socket_execute->cmd_name)+1);
                            sprintf(cmd,"%s",socket_execute->cmd_name);
                            status = download_only_add_socket_item(cmd,i);
                            free(cmd);
                            cmd = NULL;
                            if(status == 0 || status == SERVER_SPACE_NOT_ENOUGH
                               || status == LOCAL_FILE_LOST || status == SERVER_FILE_DELETED
                               || status == SERVER_ROOT_DELETED)
                            {
                                //printf("########will del socket item##########\n");
                                pthread_mutex_lock(&mutex_socket);
                                socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList_Priority);
                                free(socket_execute);
                                //printf("del socket item ok\n");
                                pthread_mutex_unlock(&mutex_socket);
                            }
                            else
                            {
                                fail_flag = 1;
                                DEBUG("######## socket item fail########\n");
                                break;
                            }
                            //sleep(2);
                            usleep(1000*20);
                        }

                        while(g_pSyncList[i]->SocketActionList->head != NULL
                              && exit_loop == 0 && server_sync == 0)
                        {
                            has_socket = 1;
                            socket_execute = g_pSyncList[i]->SocketActionList->head;
                            cmd = my_str_malloc(strlen(socket_execute->cmd_name)+1);
                            sprintf(cmd,"%s",socket_execute->cmd_name);
                            status = download_only_add_socket_item(cmd,i);
                            free(cmd);
                            cmd = NULL;
                            if(status == 0 || status == SERVER_SPACE_NOT_ENOUGH
                               || status == LOCAL_FILE_LOST || status == SERVER_FILE_DELETED
                               || status == SERVER_ROOT_DELETED)
                            {
                                //printf("########will del socket item##########\n");
                                pthread_mutex_lock(&mutex_socket);
                                socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList);
                                free(socket_execute);
                                //printf("del socket item ok\n");
                                pthread_mutex_unlock(&mutex_socket);
                            }
                            else
                            {
                                fail_flag = 1;
                                DEBUG("######## socket item fail########\n");
                                break;
                            }
                            //sleep(2);
                            usleep(1000*20);
                        }
                    }

                    if(fail_flag)
                        break;

                    //pthread_mutex_lock(&mutex_copyfilelist);
                    if(g_pSyncList[i]->copy_file_list->next == NULL)
                    {
                        break;
                    }
                    else
                    {
                        usleep(1000*100);
                        //sleep(1);
                    }
                    //pthread_mutex_unlock(&mutex_copyfilelist);
                }


                if(g_pSyncList[i]->dragfolder_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    free_action_item(g_pSyncList[i]->dragfolder_action_list);
                    g_pSyncList[i]->dragfolder_action_list = create_action_item_head();
                }
                if(g_pSyncList[i]->server_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    free_action_item(g_pSyncList[i]->server_action_list);
                    g_pSyncList[i]->server_action_list = create_action_item_head();
                }
                pthread_mutex_lock(&mutex_receve_socket);
                //receve_socket = 0;
                if(g_pSyncList[i]->SocketActionList->head == NULL)
                    g_pSyncList[i]->receve_socket = 0;
                pthread_mutex_unlock(&mutex_receve_socket);


            }
            else   //Upload Only and Sync
            {
                if(asus_cfg.prule[i]->rule == 2)     //upload only规则时，处理未完成的上传动作
                {

                    if(g_pSyncList[i]->SocketActionList->head != NULL)
                    {
                        g_pSyncList[i]->ServerRootNode = create_server_treeroot();
                        status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode,i);
                        if (status != NE_OK)
                        {
                            DEBUG("uploadonly get ServerList ERROR! \n");
                            usleep(1000*20);
                            continue;
                        }
                        status = compareServerList(i);
                        if(status == 0)
                        {
                            DEBUG("server list is modified \n");
                            check_serverlist_modify(i,g_pSyncList[i]->ServerRootNode->Child);
                            free_server_tree(g_pSyncList[i]->OldServerRootNode);
                            g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;

                        }
                        else
                        {
                            if(g_pSyncList[i]->init_completed)
                            {
                                free_server_tree(g_pSyncList[i]->ServerRootNode);
                            }
                            DEBUG("server list is no modify \n");
                        }
                    }

                    action_item *p,*p1;
                    p = g_pSyncList[i]->unfinished_list->next;
                    p1 = g_pSyncList[i]->up_space_not_enough_list->next;
                    if(g_pSyncList[i]->init_completed == 0 && p == NULL && p1 == NULL)
                    {
                        status = wd_initial();
                        if(status != 0)
                            continue;
                    }
                    if((p != NULL || p1 != NULL) && exit_loop == 0)
                    {
                        do_unfinished(i);
                    }



                }

                while(exit_loop ==0)
                {
                    while((g_pSyncList[i]->SocketActionList->head != NULL
                           || g_pSyncList[i]->SocketActionList_Priority->head != NULL)
                          && exit_loop == 0 && server_sync == 0)
                    {
                        while(g_pSyncList[i]->SocketActionList_Priority->head != NULL
                              && exit_loop == 0 && server_sync == 0)
                        {
                            has_socket = 1;
                            socket_execute = g_pSyncList[i]->SocketActionList_Priority->head;
                            cmd = my_str_malloc(strlen(socket_execute->cmd_name)+1);
                            sprintf(cmd,"%s",socket_execute->cmd_name);
                            status = cmd_parser(cmd,i);
                            free(cmd);
                            cmd = NULL;
                            if(status == 0 || status == SERVER_SPACE_NOT_ENOUGH
                               || status == LOCAL_FILE_LOST || status == SERVER_FILE_DELETED
                               || status == SERVER_ROOT_DELETED)
                            {
                                //printf("########will del socket item##########\n");
                                pthread_mutex_lock(&mutex_socket);
                                socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList_Priority);
                                free(socket_execute);
                                //DEBUG("del socket item ok\n");
                                pthread_mutex_unlock(&mutex_socket);
                            }
                            else
                            {
                                fail_flag = 1;
                                DEBUG("######## socket item fail########\n");
                                break;
                            }
                            //sleep(2);
                            usleep(1000*20);
                        }

                        while(g_pSyncList[i]->SocketActionList->head != NULL
                              && exit_loop == 0 && server_sync == 0)
                        {
                            has_socket = 1;
                            socket_execute = g_pSyncList[i]->SocketActionList->head;
                            cmd = my_str_malloc(strlen(socket_execute->cmd_name)+1);
                            sprintf(cmd,"%s",socket_execute->cmd_name);
                            status = cmd_parser(cmd,i);
                            free(cmd);
                            cmd = NULL;
                            if(status == 0 || status == SERVER_SPACE_NOT_ENOUGH
                               || status == LOCAL_FILE_LOST || status == SERVER_FILE_DELETED
                               || status == SERVER_ROOT_DELETED)
                            {
                                if(status == LOCAL_FILE_LOST && g_pSyncList[i]->SocketActionList_Priority->head != NULL)
                                    break;
                                //printf("########will del socket item##########\n");
                                pthread_mutex_lock(&mutex_socket);
                                socket_execute = queue_dequeue(g_pSyncList[i]->SocketActionList);
                                free(socket_execute);
                                //DEBUG("del socket item ok\n");
                                pthread_mutex_unlock(&mutex_socket);
                            }
                            else
                            {
                                fail_flag = 1;
                                DEBUG("######## socket item fail########\n");
                                break;
                            }
                            //sleep(2);
                            usleep(1000*20);
                            if(g_pSyncList[i]->SocketActionList_Priority->head != NULL)
                                break;
                        }
                    }

                    if(fail_flag)
                        break;

                    //pthread_mutex_lock(&mutex_copyfilelist);
                    if(g_pSyncList[i]->copy_file_list->next == NULL)
                    {
                        break;
                    }
                    else
                    {
                        //sleep(1);
                        usleep(1000*100);
                    }
                    //pthread_mutex_unlock(&mutex_copyfilelist);
                }


                if(g_pSyncList[i]->dragfolder_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    //printf("#### clear dragfolder_action_list!\n");
                    free_action_item(g_pSyncList[i]->dragfolder_action_list);
                    g_pSyncList[i]->dragfolder_action_list = create_action_item_head();
                }
                //printf("#### clear dragfolder_action_list success!\n");
                if(g_pSyncList[i]->server_action_list->next != NULL && g_pSyncList[i]->SocketActionList->head == NULL)
                {
                    //printf("#### clear server_action_list!\n");
                    free_action_item(g_pSyncList[i]->server_action_list);
                    g_pSyncList[i]->server_action_list = create_action_item_head();
                }
                //printf("#### clear server_action_list success!\n");
                pthread_mutex_lock(&mutex_receve_socket);
                //receve_socket = 0;
                if(g_pSyncList[i]->SocketActionList->head == NULL)
                    g_pSyncList[i]->receve_socket = 0;
                pthread_mutex_unlock(&mutex_receve_socket);

                if(asus_cfg.prule[i]->rule == 2 && has_socket == 1 && fail_flag == 0)     //upload only规则时，处理未完成的上传动作
                {
                    g_pSyncList[i]->ServerRootNode = create_server_treeroot();
                    status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode,i);
                    if (status != NE_OK)
                    {
                        DEBUG("uploadonly get ServerList ERROR! \n");
                        usleep(1000*20);
                        continue;
                    }
                    status = compareServerList(i);
                    if(status == 0)
                    {
                        DEBUG("server list is modified \n");
                        check_serverlist_modify(i,g_pSyncList[i]->ServerRootNode->Child);
                        free_server_tree(g_pSyncList[i]->OldServerRootNode);
                        g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;

                    }
                    else
                    {
                        if(g_pSyncList[i]->init_completed)
                        {
                            free_server_tree(g_pSyncList[i]->ServerRootNode);
                        }
                        DEBUG("server list is no modify \n");
                    }
                    //check_serverlist_modify(i,g_pSyncList[i]->ServerRootNode->Child);
                    //free_server_tree(g_pSyncList[i]->OldServerRootNode);
                    //g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                }
            }
            DEBUG("#### SocketActionList is NULL\n");
            if(has_socket == 1)
            {
                DEBUG("write log!\n");                
                has_socket = 0;
            }
            if(fail_flag)
            {
                write_log(S_ERROR,"Local sync hronization is not entirely successful,failue inofrmation please refer error log!","",i);
            }

        }

        //printf("write log success!\n");

        pthread_mutex_lock(&mutex_socket);
        local_sync = 0;
        if(!exit_loop)
        {
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + 2;
            outtime.tv_nsec = now.tv_usec * 1000;
            pthread_cond_timedwait(&cond_socket, &mutex_socket, &outtime);
        }
        pthread_mutex_unlock(&mutex_socket);
    }

    DEBUG("stop WebDAV Socket_Parser\n");
    stop_down = 1;
}

/*
 *judge is server changed
 *0:server changed
 *1:server is not changed
*/
int isServerChanged(Server_TreeNode *newNode,Server_TreeNode *oldNode)
{
    //printf("isServerChanged start!\n");
    int res = 1;
    int serverchanged = 0;
    if(newNode->browse == NULL && oldNode->browse == NULL)
    {
        DEBUG("########Server is not change\n");
        return 1;
    }
    else if(newNode->browse == NULL && oldNode->browse != NULL)
    {
        DEBUG("########Server changed1\n");
        return 0;
    }
    else if(newNode->browse != NULL && oldNode->browse == NULL)
    {
        DEBUG("########Server changed2\n");
        return 0;
    }
    else
    {
        if(newNode->browse->filenumber != oldNode->browse->filenumber || newNode->browse->foldernumber != oldNode->browse->foldernumber)
        {
            DEBUG("########Server changed3\n");
            return 0;
        }
        else
        {
            int cmp;
            CloudFile *newfoldertmp = NULL;
            CloudFile *oldfoldertmp = NULL;
            CloudFile *newfiletmp = NULL;
            CloudFile *oldfiletmp = NULL;
            if(newNode->browse != NULL)
            {
                if(newNode->browse->foldernumber > 0)
                    newfoldertmp = newNode->browse->folderlist->next;
                if(newNode->browse->filenumber > 0)
                    newfiletmp = newNode->browse->filelist->next;
            }
            if(oldNode->browse != NULL)
            {
                if(oldNode->browse->foldernumber > 0)
                    oldfoldertmp = oldNode->browse->folderlist->next;
                if(oldNode->browse->filenumber > 0)
                    oldfiletmp = oldNode->browse->filelist->next;
            }

            while (newfoldertmp != NULL || oldfoldertmp != NULL)
            {
                if ((cmp = strcmp(newfoldertmp->href,oldfoldertmp->href)) != 0){

                    DEBUG("########Server changed4\n");
                    return 0;
                }
                newfoldertmp = newfoldertmp->next;
                oldfoldertmp = oldfoldertmp->next;
            }
            while (newfiletmp != NULL || oldfiletmp != NULL)
            {
                if ((cmp = strcmp(newfiletmp->href,oldfiletmp->href)) != 0)
                {
                    DEBUG("########Server changed5\n");
                    return 0;
                }
                if (newfiletmp->modtime != oldfiletmp->modtime)
                {
                    //printf("newpath=%s,newtime=%lu\n",newfiletmp->href,newfiletmp->modtime);
                    //printf("oldpath=%s,oldtime=%lu\n",oldfiletmp->href,oldfiletmp->modtime);
                    DEBUG("########Server changed6\n");
                    return 0;
                }
                newfiletmp = newfiletmp->next;
                oldfiletmp = oldfiletmp->next;
            }
        }

        if((newNode->Child == NULL && oldNode->Child != NULL) || (newNode->Child != NULL && oldNode->Child == NULL))
        {
            DEBUG("########Server changed7\n");
            return 0;
        }
        if((newNode->NextBrother == NULL && oldNode->NextBrother != NULL) || (newNode->NextBrother!= NULL && oldNode->NextBrother == NULL))
        {
            DEBUG("########Server changed8\n");
            return 0;
        }

        if(newNode->Child != NULL && oldNode->Child != NULL)
        {
            res = isServerChanged(newNode->Child,oldNode->Child);
            if(res == 0)
            {
                serverchanged = 1;
            }
        }
        if(newNode->NextBrother != NULL && oldNode->NextBrother != NULL)
        {
            res = isServerChanged(newNode->NextBrother,oldNode->NextBrother);
            if(res == 0)
            {
                serverchanged = 1;
            }
        }
    }
    if(serverchanged == 1)
    {
        DEBUG("########Server changed9\n");
        return 0;
    }
    else
    {
        //printf("########Server is not change\n");
        return 1;
    }
}
/*ret = 0,server changed
 *ret = 1,server is no changed
*/
int compareServerList(int index)
{
    int ret;

    DEBUG("#########compareServerList\n");

    if(g_pSyncList[index]->OldServerRootNode == NULL)
    {
        ret = 1;
        return ret;
    }
    else
    {
        if(g_pSyncList[index]->ServerRootNode->Child != NULL && g_pSyncList[index]->OldServerRootNode->Child != NULL)
        {
            ret = isServerChanged(g_pSyncList[index]->ServerRootNode->Child,g_pSyncList[index]->OldServerRootNode->Child);
            return ret;
        }
        else if(g_pSyncList[index]->ServerRootNode->Child == NULL && g_pSyncList[index]->OldServerRootNode->Child == NULL)
        {
            ret = 1;
            return ret;
        }
        else
        {
            ret = 0;
            return ret;
        }
    }
}

int is_server_modify(Server_TreeNode *newNode,Server_TreeNode *oldNode)
{

    //DEBUG("########is_server_modify\n");
    if(newNode->browse == NULL && oldNode->browse == NULL)
    {
        DEBUG("########Server is no modify1\n");
        return 1;
    }
    else if(newNode->browse == NULL && oldNode->browse != NULL)
    {
        DEBUG("########Server is no modify2\n");
        return 1;
    }
    else if(newNode->browse != NULL && oldNode->browse == NULL)
    {
        DEBUG("########Server is no modify3\n");
        return 1;
    }
    else
    {
        int cmp;
        CloudFile *newfoldertmp = NULL;
        CloudFile *oldfoldertmp = NULL;
        CloudFile *newfiletmp = NULL;
        CloudFile *oldfiletmp = NULL;
        if(newNode->browse != NULL)
        {
            if(newNode->browse->foldernumber > 0)
                newfoldertmp = newNode->browse->folderlist->next;
            if(newNode->browse->filenumber > 0)
            {
                newfiletmp = newNode->browse->filelist->next;
            }
            else
            {
                DEBUG("########Server is no modify4\n");
                return 1;
            }
        }
        if(oldNode->browse != NULL)
        {
            if(oldNode->browse->foldernumber > 0)
                oldfoldertmp = oldNode->browse->folderlist->next;
            if(oldNode->browse->filenumber <= 0)
            {
                DEBUG("########Server is no modify5\n");
                return 1;
            }
        }

        while(newfiletmp != NULL)
        {
            oldfiletmp = oldNode->browse->filelist->next;
            while (oldfiletmp != NULL)
            {
                if ((cmp = strcmp(newfiletmp->href,oldfiletmp->href)) == 0)
                {
                    if((newfiletmp->modtime != oldfiletmp->modtime) || (oldfiletmp->ismodify == 1))
                    {
                        DEBUG("########Server %s is modified\n",newfiletmp->href);
                        newfiletmp->ismodify = 1;
                    }
                    break;
                }

                oldfiletmp = oldfiletmp->next;
            }

            newfiletmp = newfiletmp->next;
        }

    }
    //DEBUG("########is_server_modify over\n");
    return 1;
}

Server_TreeNode * getoldnode(Server_TreeNode *tempoldnode,char *href)
{
    //Server_TreeNode *tempoldnode;
    Server_TreeNode *retnode;
    //tempoldnode = NULL;
    retnode = NULL;

    //DEBUG("getoldnode tempoldnode->parenthref -> %s   %d\n",tempoldnode->parenthref,strlen(tempoldnode->parenthref));
    //DEBUG("                              href -> %s   %d\n",href,strlen(href));

    if(strcmp(href,tempoldnode->parenthref) == 0)
    {
        //DEBUG("*****get the retnode\n");
        retnode = tempoldnode;
        return retnode;
    }

    if(tempoldnode->Child != NULL)
    {
        retnode = getoldnode(tempoldnode->Child,href);
        if(retnode != NULL)
        {
            return retnode;
        }
    }
    if(tempoldnode->NextBrother != NULL)
    {
        retnode = getoldnode(tempoldnode->NextBrother,href);
        if(retnode != NULL)
        {
            return retnode;
        }
    }
    return retnode;
}

int check_serverlist_modify(int index,Server_TreeNode *newnode)
{
    int ret;
    //DEBUG("############check_serverlist_modify %s\n",newnode->parenthref);
    Server_TreeNode *oldnode;
    oldnode = getoldnode(g_pSyncList[index]->OldServerRootNode->Child,newnode->parenthref);

    /*if(newnode == NULL)
    {
        DEBUG("newnode is NULL\n");
    }

    if(oldnode == NULL)
    {
        DEBUG("oldnode is NULL\n");
    }*/

    if(newnode != NULL && oldnode != NULL)
    {

        ret = is_server_modify(newnode,oldnode);
        //return ret;
    }
    else if(newnode == NULL)
    {
        return 0;
    }

    if(newnode->Child != NULL)
    {
        ret = check_serverlist_modify(index,newnode->Child);
    }

    if(newnode->NextBrother != NULL)
    {
        ret = check_serverlist_modify(index,newnode->NextBrother);
    }

    return ret;
}

int the_same_name_compare(LocalFile *localfiletmp,CloudFile *filetmp,int index){

    //DEBUG("the_same_name_compare start!\n");

    int ret = 0;
    //int newer_file_ret = 0;
    int time_ret = 0;

    if(g_pSyncList[index]->init_completed)      //sync phase
    {
        CloudFile *oldfiletmp = NULL;
        oldfiletmp = get_CloudFile_node(g_pSyncList[index]->RealOldServerRootNode,filetmp->href,0x2);

        time_ret = sync_newer_file(localfiletmp->path,index,oldfiletmp);

        //DEBUG("time_ret = %d\n",time_ret);

        if(time_ret == -1)
        {
            return time_ret;
        }
        else if(time_ret == 0)
        {
            return ret;
        }
        else if(time_ret == 1)
        {
            if(asus_cfg.prule[index]->rule ==0)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);  //??
                ret = Upload(localfiletmp->path,index);
                if(ret == NE_OK || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                {
                    if(ret == NE_OK)
                    {
                        //char *serverpath;
                        //serverpath = localpath_to_serverpath(localfiletmp->path,index);
                        //printf("serverpath = %s\n",serverpath);
                        time_t modtime;
                        modtime = Getmodtime(filetmp->href,index);
                        if(modtime != -1)
                        {
                            ChangeFile_modtime(localfiletmp->path,modtime,index);
                            oldfiletmp->modtime = modtime;
                            oldfiletmp->ismodify = 0;
                        }
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        //free(serverpath);
                    }
                }
                else
                {
                    return ret;
                }
            }
            else
            {
                char *newname;
                newname = change_local_same_name(localfiletmp->path);
                rename(localfiletmp->path,newname);
                //add_action_item("rename",newname,server_action_list);

                DEBUG("newname = %s\n",newname);
                write_conflict_log(localfiletmp->path,newname,index);
                free(newname);

                //char *localpath;
                //localpath = serverpath_to_localpath(filetmp->href,index);

                action_item *item;
                item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

                if(is_local_space_enough(filetmp,index))
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                    ret = Download(filetmp->href,index);
                    if (ret == NE_OK)
                    {
                        time_t modtime;
                        modtime = Getmodtime(filetmp->href,index);
                        if(modtime != -1)
                        {
                            ChangeFile_modtime(localfiletmp->path,modtime,index);
                            oldfiletmp->modtime = modtime;
                            oldfiletmp->ismodify = 0;
                        }
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        if(item != NULL)
                        {
                            del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                        }
                    }
                    else
                    {
                        //free(localpath);
                        //free(serverpathtmp);
                        return ret;
                    }
                }
                else
                {
                    write_log(S_ERROR,"local space is not enough!","",index);
                    if(item == NULL)
                    {
                        add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                    return ret;
                }
            }
        }
        else if(time_ret == 2)
        {
            //char *localpath;
            //localpath = serverpath_to_localpath(filetmp->href,index);

            action_item *item;
            item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

            if(is_local_space_enough(filetmp,index))
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                ret = Download(filetmp->href,index);
                if (ret == NE_OK)
                {
                    time_t modtime;
                    modtime = Getmodtime(filetmp->href,index);
                    if(modtime != -1)
                        ChangeFile_modtime(localfiletmp->path,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
                else
                {
                    //free(localpath);
                    //free(serverpathtmp);
                    return ret;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                if(item == NULL)
                {
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }
                return ret;
            }
            //free(localpath);
        }
        else
        {
            char *newname;
            newname = change_local_same_name(localfiletmp->path);
            rename(localfiletmp->path,newname);
            //add_action_item("rename",newname,server_action_list);

            DEBUG("newname = %s\n",newname);
            write_conflict_log(localfiletmp->path,newname,index);
            free(newname);

            //char *localpath;
            //localpath = serverpath_to_localpath(filetmp->href,index);

            action_item *item;
            item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

            if(is_local_space_enough(filetmp,index))
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                ret = Download(filetmp->href,index);
                if (ret == NE_OK)
                {
                    time_t modtime;
                    modtime = Getmodtime(filetmp->href,index);
                    if(modtime != -1)
                    {
                        ChangeFile_modtime(localfiletmp->path,modtime,index);
                        oldfiletmp->modtime = modtime;
                        oldfiletmp->ismodify = 0;
                    }
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
                else
                {
                    //free(localpath);
                    //free(serverpathtmp);
                    return ret;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                if(item == NULL)
                {
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }
                return ret;
            }
        }

    }
    else                                    //init phase
    {
        time_ret = init_newer_file(localfiletmp->path,index);
        if(time_ret == 0)
        {
            if(asus_cfg.prule[index]->rule != 1)
            {
                char *newname;
                newname = change_server_same_name(localfiletmp->path,index);
                Move(localfiletmp->path,newname,index);
                DEBUG("newname = %s\n",newname);
                write_conflict_log(localfiletmp->path,newname,index);
                free(newname);

                ret = Upload(localfiletmp->path,index);

                if(ret != 0)
                {
                    DEBUG("upload %s failed\n",localfiletmp->path);
                    //free(path);
                    //free(fullname);
                    return ret;
                }
                else
                {
                    //char *serverpath;
                    //serverpath = localpath_to_serverpath(fullname,index);
                    //printf("serverpath = %s\n",serverpath);
                    time_t modtime;
                    modtime = Getmodtime(filetmp->href,index);
                    if(modtime != -1)
                        ChangeFile_modtime(localfiletmp->path,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                        return -1;
                    }
                    //free(fullname);
                    //free(serverpath);
                }
            }
            else
            {
                char *newname;
                newname = change_local_same_name(localfiletmp->path);
                rename(localfiletmp->path,newname);
                //add_action_item("rename",newname,server_action_list);

                DEBUG("newname = %s\n",newname);
                free(newname);

                //char *localpath;
                //localpath = serverpath_to_localpath(filetmp->href,index);

                action_item *item;
                item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

                if(is_local_space_enough(filetmp,index))
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                    ret = Download(filetmp->href,index);
                    if (ret == NE_OK)
                    {
                        time_t modtime;
                        modtime = Getmodtime(filetmp->href,index);
                        if(modtime != -1)
                            ChangeFile_modtime(localfiletmp->path,modtime,index);
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        if(item != NULL)
                        {
                            del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                        }
                    }
                    else
                    {
                        //free(localpath);
                        //free(serverpathtmp);
                        return ret;
                    }
                }
                else
                {
                    write_log(S_ERROR,"local space is not enough!","",index);
                    if(item == NULL)
                    {
                        add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                    return ret;
                }
            }
        }
        else if(time_ret == 1)
        {
            return ret;
        }
        else if(time_ret == 2)
        {
            return ret;
        }
        else
        {
            return time_ret;
        }
    }

    return ret;
}

int sync_server_to_local_init(Browse *perform_br,Local *perform_lo,int index){

    if(perform_br == NULL || perform_lo == NULL)
    {
        return 0;
    }

    CloudFile *foldertmp = NULL;
    CloudFile *filetmp = NULL;
    LocalFolder *localfoldertmp;
    LocalFile *localfiletmp;
    int ret = 0;

    if(perform_br->foldernumber > 0)
        foldertmp = perform_br->folderlist->next;
    if(perform_br->filenumber > 0)
        filetmp = perform_br->filelist->next;

    localfoldertmp = perform_lo->folderlist->next;
    localfiletmp = perform_lo->filelist->next;

    /****************handle files****************/
    if(perform_br->filenumber == 0 && perform_lo->filenumber != 0)
    {
        while(localfiletmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule != 1)
            {
                /*while(is_file_copying(localfiletmp->path))
                {
                    usleep(1000*100);
                }*/

                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }

                //add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);   //del by zero check
                ret = Upload(localfiletmp->path,index);
                if(ret == NE_OK || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                {
                    if(ret == NE_OK)
                    {
                        char *serverpath;
                        serverpath = localpath_to_serverpath(localfiletmp->path,index);
                        //printf("serverpath = %s\n",serverpath);
                        time_t modtime;
                        modtime = Getmodtime(serverpath,index);
                        if(modtime != -1)
                            ChangeFile_modtime(localfiletmp->path,modtime,index);
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(serverpath);
                    }
                }
                else
                {
                    return ret;
                }
            }
            else
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->download_only_socket_head);
            }

            localfiletmp = localfiletmp->next;
        }

    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber == 0)
    {
        if(asus_cfg.prule[index]->rule != 2)
        {
            while(filetmp != NULL && !exit_loop)
            {
                if(is_local_space_enough(filetmp,index))
                {
                    char *localpath;
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    localpath = serverpath_to_localpath(filetmp->href,index);
                    add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);

                    //wait_handle_socket(index);

                    ret = Download(filetmp->href,index);
                    if (ret == NE_OK)
                    {
                        //ChangeFile_modtime(localpath,filetmp->modtime);
                        time_t modtime;
                        modtime = Getmodtime(filetmp->href,index);
                        if(modtime != -1)
                            ChangeFile_modtime(localpath,modtime,index);
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        free(localpath);
                    }
                    else
                    {
                        free(localpath);
                        return ret;
                    }
                }
                else
                {
                    write_log(S_ERROR,"local space is not enough!","",index);
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }

                filetmp = filetmp->next;
            }
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber != 0)
    {
        while(localfiletmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *localpathtmp;
            localpathtmp = strstr(localfiletmp->path,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
            while(filetmp != NULL)
            {
                char *serverpathtmp;
                serverpathtmp = strstr(filetmp->href,asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
                serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    free(serverpathtmp);
                    break;
                }
                else
                {
                    filetmp = filetmp->next;
                }
                free(serverpathtmp);
            }
            if (cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 1)
                {
                    /*while(is_file_copying(localfiletmp->path))
                    {
                        usleep(1000*100);
                    }*/

                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }

                    add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
                    ret = Upload(localfiletmp->path,index);
                    if(ret == NE_OK || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
                    {
                        if(ret == NE_OK)
                        {
                            char *serverpath;
                            serverpath = localpath_to_serverpath(localfiletmp->path,index);
                            //printf("serverpath = %s\n",serverpath);
                            time_t modtime;
                            modtime = Getmodtime(serverpath,index);
                            if(modtime != -1)
                                ChangeFile_modtime(localfiletmp->path,modtime,index);
                            else
                            {
                                DEBUG("ChangeFile_modtime failed!\n");
                            }
                            free(serverpath);
                        }
                    }
                    else
                    {
                        return ret;
                    }
                }
                else
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->download_only_socket_head);
                }
            }
            else
            {
                if((ret = the_same_name_compare(localfiletmp,filetmp,index)) != 0)
                {
                    return ret;
                }
            }
            filetmp = perform_br->filelist->next;
            localfiletmp = localfiletmp->next;
        }


        filetmp = perform_br->filelist->next;
        localfiletmp = perform_lo->filelist->next;
        while(filetmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *serverpathtmp;
            serverpathtmp = strstr(filetmp->href,asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
            serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
            while(localfiletmp != NULL)
            {
                char *localpathtmp;
                localpathtmp = strstr(localfiletmp->path,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    break;
                }
                else
                {
                    localfiletmp = localfiletmp->next;
                }
            }
            if (cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 2)
                {
                    if(is_local_space_enough(filetmp,index))
                    {
                        char *localpath;
                        localpath = serverpath_to_localpath(filetmp->href,index);

                        if(wait_handle_socket(index))
                        {
                            return HAVE_LOCAL_SOCKET;
                        }

                        add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                        ret = Download(filetmp->href,index);
                        //printf("XXXXXXXXXXXXXret3 = %d\n",ret);
                        if (ret == NE_OK)
                        {
                            //ChangeFile_modtime(localpath,filetmp->modtime);
                            time_t modtime;
                            modtime = Getmodtime(filetmp->href,index);
                            if(modtime != -1)
                                ChangeFile_modtime(localpath,modtime,index);
                            else
                            {
                                DEBUG("ChangeFile_modtime failed!\n");
                            }
                            free(localpath);
                        }
                        else
                        {
                            free(localpath);
                            free(serverpathtmp);
                            return ret;
                        }
                    }
                    else
                    {
                        write_log(S_ERROR,"local space is not enough!","",index);
                        add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
            }
            free(serverpathtmp);
            filetmp = filetmp->next;
            localfiletmp = perform_lo->filelist->next;
        }
    }



    /*************handle folders**************/
    if(perform_br->foldernumber == 0 && perform_lo->foldernumber != 0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            if(asus_cfg.prule[index]->rule != 1)
            {
                add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->server_action_list);
                ret = Mkcol(localfoldertmp->path,index);
                if(ret != NE_OK && ret != -1)
                {
                    return ret;
                }
            }
            else
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->download_only_socket_head);
            }
            localfoldertmp = localfoldertmp->next;
        }
    }
    else if(perform_br->foldernumber != 0 && perform_lo->foldernumber == 0)
    {
        if(asus_cfg.prule[index]->rule != 2)
        {
            while(foldertmp != NULL && !exit_loop)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                char *localpath;
                localpath = serverpath_to_localpath(foldertmp->href,index);
                if(NULL == opendir(localpath))
                {
                    add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                    mkdir(localpath,0777);
                }
                free(localpath);
                foldertmp = foldertmp->next;
            }
        }
    }
    else if(perform_br->foldernumber != 0 && perform_lo->foldernumber != 0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *localpathtmp;
            localpathtmp = strstr(localfoldertmp->path,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
            while(foldertmp != NULL)
            {
                char *serverpathtmp;
                serverpathtmp = strstr(foldertmp->href,asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
                serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    free(serverpathtmp);
                    break;
                }
                else
                {
                    foldertmp = foldertmp->next;
                }
                free(serverpathtmp);
            }
            if (cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 1)
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->server_action_list);
                    ret = Mkcol(localfoldertmp->path,index);
                    if(ret != NE_OK && ret != -1)
                    {
                        return ret;
                    }
                }
                else
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->download_only_socket_head);
                }
            }
            foldertmp = perform_br->folderlist->next;
            localfoldertmp = localfoldertmp->next;
        }

        foldertmp = perform_br->folderlist->next;
        localfoldertmp = perform_lo->folderlist->next;
        while(foldertmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *serverpathtmp;
            serverpathtmp = strstr(foldertmp->href,asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
            serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
            while(localfoldertmp != NULL)
            {
                char *localpathtmp;
                localpathtmp = strstr(localfoldertmp->path,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    break;
                }
                else
                {
                    localfoldertmp = localfoldertmp->next;
                }
            }
            if (cmp != 0)
            {
                if(asus_cfg.prule[index]->rule != 2)
                {
                    if(wait_handle_socket(index))
                    {
                        return HAVE_LOCAL_SOCKET;
                    }
                    char *localpath;
                    localpath = serverpath_to_localpath(foldertmp->href,index);
                    if(NULL == opendir(localpath))
                    {
                        add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                        mkdir(localpath,0777);
                    }
                    free(localpath);
                }
            }
            free(serverpathtmp);
            foldertmp = foldertmp->next;
            localfoldertmp = perform_lo->folderlist->next;
        }
    }

    if(exit_loop)
        ret = NE_WEBDAV_QUIT;

    return ret;
}

int sync_server_to_local_perform(Browse *perform_br,Local *perform_lo,int index){

    if(perform_br == NULL || perform_lo == NULL)
    {
        return 0;
    }

    CloudFile *foldertmp = NULL;
    CloudFile *filetmp = NULL;
    LocalFolder *localfoldertmp;
    LocalFile *localfiletmp;
    int ret = 0;
    int wait_ret = 0;

    if(perform_br->foldernumber > 0)
        foldertmp = perform_br->folderlist->next;
    if(perform_br->filenumber > 0)
        filetmp = perform_br->filelist->next;

    localfoldertmp = perform_lo->folderlist->next; 
    localfiletmp = perform_lo->filelist->next;

    /****************handle files****************/
    //printf("##########handle files\n");
    if(perform_br->filenumber == 0 && perform_lo->filenumber != 0)
    {
        while(localfiletmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule == 1)
            {
                action_item *item;
                item = get_action_item("download_only",localfiletmp->path,
                                       g_pSyncList[index]->download_only_socket_head,index);
                if(item != NULL)
                {
                    localfiletmp = localfiletmp->next;
                    continue;
                }
            }
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("remove",localfiletmp->path,g_pSyncList[index]->server_action_list);

            action_item *pp;
            pp = get_action_item("upload",localfiletmp->path,
                                 g_pSyncList[index]->up_space_not_enough_list,index);
            if(pp == NULL)
            {
                unlink(localfiletmp->path);
            }
            localfiletmp = localfiletmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber == 0)
    {
        while(filetmp != NULL && !exit_loop)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            char *localpath;
            localpath = serverpath_to_localpath(filetmp->href,index);

            action_item *item;
            item = get_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list,index);

            if(is_local_space_enough(filetmp,index))
            {
                add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                ret = Download(filetmp->href,index);
                if (ret == NE_OK)
                {
                    //ChangeFile_modtime(localpath,filetmp->modtime);
                    time_t modtime;
                    modtime = Getmodtime(filetmp->href,index);
                    if(modtime != -1)
                        ChangeFile_modtime(localpath,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    if(item != NULL)
                    {
                        del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }
                }
                else
                {
                    free(localpath);
                    return ret;
                }
            }
            else
            {
                write_log(S_ERROR,"local space is not enough!","",index);
                if(item == NULL)
                {
                    add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                }
            }
            free(localpath);
            filetmp = filetmp->next;
        }
    }
    else if(perform_br->filenumber != 0 && perform_lo->filenumber != 0)
    {
        //printf("remove local file while\n");
        while(localfiletmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *localpathtmp;
            localpathtmp = strstr(localfiletmp->path,
                                  asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
            //printf("localpathtmp = %s\n",localpathtmp);
            while(filetmp != NULL)
            {
                char *serverpathtmp;
                serverpathtmp = strstr(filetmp->href,
                                       asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
                serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
                //printf("serverpathtmp = %s\n",serverpathtmp);
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    free(serverpathtmp);
                    break;
                }
                else
                {
                    filetmp = filetmp->next;
                }
                free(serverpathtmp);
            }
            if (cmp != 0)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                if(asus_cfg.prule[index]->rule == 1)
                {
                    action_item *item;
                    item = get_action_item("download_only",localfiletmp->path,
                                           g_pSyncList[index]->download_only_socket_head,index);
                    if(item != NULL)
                    {
                        filetmp = perform_br->filelist->next;
                        localfiletmp = localfiletmp->next;
                        continue;
                    }
                }
                action_item *pp;
                pp = get_action_item("upload",localfiletmp->path,
                                     g_pSyncList[index]->up_space_not_enough_list,index);
                if(pp == NULL)
                {
                    unlink(localfiletmp->path);
                    add_action_item("remove",localfiletmp->path,g_pSyncList[index]->server_action_list);
                }
                //add_action_item("remove",localfiletmp->path,g_pSyncList[index]->server_action_list);

                //unlink(localfiletmp->path);
            }
            else
            {
                if((ret = the_same_name_compare(localfiletmp,filetmp,index)) != 0)
                {
                    return ret;
                }
            }
            filetmp = perform_br->filelist->next;
            localfiletmp = localfiletmp->next;
        }


        filetmp = perform_br->filelist->next;
        localfiletmp = perform_lo->filelist->next;

        //printf("download server file while\n");
        while(filetmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *serverpathtmp;
            serverpathtmp = strstr(filetmp->href,
                                   asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
            serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
            while(localfiletmp != NULL)
            {
                char *localpathtmp;
                localpathtmp = strstr(localfiletmp->path,
                                      asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    break;
                }
                else
                {
                    localfiletmp = localfiletmp->next;
                }
            }
            free(serverpathtmp);
            if (cmp != 0)
            {
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                char *localpath;
                localpath = serverpath_to_localpath(filetmp->href,index);

                action_item *item;
                item = get_action_item("download",
                                       filetmp->href,g_pSyncList[index]->unfinished_list,index);

                if(is_local_space_enough(filetmp,index))
                {
                    add_action_item("createfile",localpath,g_pSyncList[index]->server_action_list);
                    ret = Download(filetmp->href,index);
                    if (ret == NE_OK)
                    {
                        //char *localpath;
                        //localpath = serverpath_to_localpath(filetmp->href);
                        //ChangeFile_modtime(localpath,filetmp->modtime);
                        time_t modtime;
                        modtime = Getmodtime(filetmp->href,index);
                        if(modtime != -1)
                            ChangeFile_modtime(localpath,modtime,index);
                        else
                        {
                            DEBUG("ChangeFile_modtime failed!\n");
                        }
                        if(item != NULL)
                        {
                            del_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                        }
                    }
                    else
                    {
                        free(localpath);
                        //free(serverpathtmp);
                        return ret;
                    }
                }
                else
                {
                    write_log(S_ERROR,"local space is not enough!","",index);
                    if(item == NULL)
                    {
                        add_action_item("download",filetmp->href,g_pSyncList[index]->unfinished_list);
                    }

                }
                free(localpath);
            }
            //free(serverpathtmp);
            filetmp = filetmp->next;
            localfiletmp = perform_lo->filelist->next;
        }
    }



    /*************handle folders**************/
    //printf("##########handle folders\n");
    if(perform_br->foldernumber == 0 && perform_lo->foldernumber != 0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            if(asus_cfg.prule[index]->rule == 1)
            {
                action_item *item;
                item = get_action_item("download_only",
                                       localfoldertmp->path,g_pSyncList[index]->download_only_socket_head,index);
                if(item != NULL)
                {
                    localfoldertmp = localfoldertmp->next;
                    continue;
                }
            }
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            del_all_items(localfoldertmp->path,index);
            localfoldertmp = localfoldertmp->next;
        }
    }
    else if(perform_br->foldernumber != 0 && perform_lo->foldernumber == 0)
    {
        while(foldertmp != NULL && !exit_loop)
        {
            char *localpath;
            char *parentpath;
            localpath = serverpath_to_localpath(foldertmp->href,index);
            parentpath = parse_parentfolder_from_path(localpath);

            if(is_server_exist(parentpath,localpath,index))
            {
                if(wait_handle_socket(index))
                {
                    free(parentpath);
                    free(localpath);
                    return HAVE_LOCAL_SOCKET;
                }

                if(NULL == opendir(localpath))
                {
                    add_action_item("createfolder",localpath,g_pSyncList[index]->server_action_list);
                    mkdir(localpath,0777);
                }
                free(parentpath);
                free(localpath);
            }

            foldertmp = foldertmp->next;
        }

    }
    else if(perform_br->foldernumber != 0 && perform_lo->foldernumber != 0)
    {
        while(localfoldertmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *localpathtmp;
            localpathtmp = strstr(localfoldertmp->path,
                                  asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
            while(foldertmp != NULL)
            {
                char *serverpathtmp;
                serverpathtmp = strstr(foldertmp->href,
                                       asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
                serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    free(serverpathtmp);
                    break;
                }
                else
                {
                    foldertmp = foldertmp->next;
                }
                free(serverpathtmp);
            }
            if (cmp != 0)
            {
                if(asus_cfg.prule[index]->rule == 1)
                {
                    action_item *item;
                    item = get_action_item("download_only",
                                           localfoldertmp->path,g_pSyncList[index]->download_only_socket_head,index);
                    if(item != NULL)
                    {
                        foldertmp = perform_br->folderlist->next;
                        localfoldertmp = localfoldertmp->next;
                        continue;
                    }
                }
                if(wait_handle_socket(index))
                {
                    return HAVE_LOCAL_SOCKET;
                }
                del_all_items(localfoldertmp->path,index);
            }
            foldertmp = perform_br->folderlist->next;
            localfoldertmp = localfoldertmp->next;
        }

        foldertmp = perform_br->folderlist->next;
        localfoldertmp = perform_lo->folderlist->next;
        while(foldertmp != NULL && !exit_loop)
        {
            int cmp = 1;
            char *serverpathtmp;
            serverpathtmp = strstr(foldertmp->href,
                                   asus_cfg.prule[index]->rootfolder) + asus_cfg.prule[index]->rootfolder_length;
            serverpathtmp = oauth_url_unescape(serverpathtmp,NULL);
            while(localfoldertmp != NULL)
            {
                char *localpathtmp;
                localpathtmp = strstr(localfoldertmp->path,
                                      asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
                if ((cmp = strcmp(localpathtmp,serverpathtmp)) == 0)
                {
                    //filetmp = OldFileList->next;
                    break;
                }
                else
                {
                    localfoldertmp = localfoldertmp->next;
                }
            }
            free(serverpathtmp);
            if (cmp != 0)
            {
                char *localpath;
                char *parentpath;
                localpath = serverpath_to_localpath(foldertmp->href,index);
                parentpath = parse_parentfolder_from_path(localpath);

                if(is_server_exist(parentpath,localpath,index))
                {
                    if(wait_handle_socket(index))
                    {
                        free(parentpath);
                        free(localpath);
                        return HAVE_LOCAL_SOCKET;
                    }

                    if(NULL == opendir(localpath))
                    {
                        add_action_item("createfolder",localpath,
                                        g_pSyncList[index]->server_action_list);
                        mkdir(localpath,0777);
                    }
                    free(parentpath);
                    free(localpath);
                }

            }
            //free(serverpathtmp);
            foldertmp = foldertmp->next;
            localfoldertmp = perform_lo->folderlist->next;
        }
    }

    if(exit_loop)
        ret = NE_WEBDAV_QUIT;

    return ret;

}

int sync_server_to_local(Server_TreeNode *treenode,int (*sync_fuc)(Browse*,Local*,int),int index){

    //DEBUG("sync_server_to_local start!\n");
    if(treenode->parenthref == NULL)
    {
        return 0;
    }

    Local *localnode;
    char *hreftmp;
    char *localpath;
    int ret = 0;


    hreftmp = strstr(treenode->parenthref,asus_cfg.prule[index]->rootfolder)+asus_cfg.prule[index]->rootfolder_length;
    //DEBUG("hreftmp = %s,hreftmplen = %d\n",hreftmp,strlen(hreftmp));
    if(strlen(hreftmp) != 0)
    {
        hreftmp = oauth_url_unescape(hreftmp,NULL);
        localpath = my_str_malloc(asus_cfg.prule[index]->base_path_len+strlen(hreftmp)+1);
        //memset(localpath,'\0',sizeof(localpath));
        sprintf(localpath,"%s%s",asus_cfg.prule[index]->base_path,hreftmp);
        free(hreftmp);
    }
    else
    {
        localpath = my_str_malloc(asus_cfg.prule[index]->base_path_len+1);
        //memset(localpath,'\0',sizeof(localpath));
        sprintf(localpath,"%s",asus_cfg.prule[index]->base_path);
    }


    //printf("sync_server_to_local hreftmp = %s\n",hreftmp);
    //printf("sync_server_to_local localpath = %s\n",localpath);

    localnode = Find_Floor_Dir(localpath);
    free(localpath);

    if(NULL != localnode)
    {
        ret = sync_fuc(treenode->browse,localnode,index);
        //DEBUG("**********ret2 = %d\n",ret);
        if(ret == COULD_NOT_CONNECNT_TO_SERVER || ret == CONNECNTION_TIMED_OUT
           || ret == COULD_NOT_READ_RESPONSE_BODY || ret == HAVE_LOCAL_SOCKET
           || ret == NE_WEBDAV_QUIT)
        {
            DEBUG("###############free localnode\n");
            free_localfloor_node(localnode);
            return ret;
        }


        //DEBUG("###############free localnode\n");
        free_localfloor_node(localnode);
        //printf("###############localnode->filenumber = %d\n",localnode->filenumber);
    }

    if(treenode->Child != NULL && !exit_loop)
    {
        ret = sync_server_to_local(treenode->Child,sync_fuc,index);
        if(ret != 0 && ret != SERVER_SPACE_NOT_ENOUGH
           && ret != LOCAL_FILE_LOST && ret != SERVER_FILE_DELETED)
        {
            return ret;
        }
    }
    if(treenode->NextBrother != NULL && !exit_loop)
    {
        ret = sync_server_to_local(treenode->NextBrother,sync_fuc,index);
        if(ret != 0 && ret != SERVER_SPACE_NOT_ENOUGH
           && ret != LOCAL_FILE_LOST && ret != SERVER_FILE_DELETED)
        {
            return ret;
        }
    }

    if(exit_loop)
        ret = NE_WEBDAV_QUIT;

    return ret;
}

int is_server_have_localpath(char *path,Server_TreeNode *treenode,int index){

    if(treenode == NULL)
        return 0;

    char *hreftmp;
    char *localpath;
    int ret = 0;
    int cmp = 1;

    hreftmp = strstr(treenode->parenthref,asus_cfg.prule[index]->rootfolder)+asus_cfg.prule[index]->rootfolder_length;
    hreftmp = oauth_url_unescape(hreftmp,NULL);
    if(hreftmp == NULL)
    {
        localpath = my_str_malloc(asus_cfg.prule[index]->base_path_len+1);
        //memset(localpath,'\0',sizeof(localpath));
        sprintf(localpath,"%s",asus_cfg.prule[index]->base_path);
    }
    else
    {
        localpath = my_str_malloc(asus_cfg.prule[index]->base_path_len+strlen(hreftmp)+1);
        //memset(localpath,'\0',sizeof(localpath));
        sprintf(localpath,"%s%s",asus_cfg.prule[index]->base_path,hreftmp);
        free(hreftmp);
    }

    if((cmp = strcmp(localpath,path)) == 0)
    {
        ret = 1;
        free(localpath);
        return ret;
    }
    else
    {
        free(localpath);
    }

    if(treenode->Child != NULL)
    {
        ret = is_server_have_localpath(path,treenode->Child,index);
        if(ret == 1)
        {
            return ret;
        }
    }
    if(treenode->NextBrother != NULL)
    {
        ret = is_server_have_localpath(path,treenode->NextBrother,index);
        if(ret == 1)
        {
            return ret;
        }
    }

    return ret;
}

int sync_local_to_server_init(Local *perform_lo,int index){

    LocalFolder *localfoldertmp;
    LocalFile *localfiletmp;
    int ret = 0;

    localfoldertmp = perform_lo->folderlist->next;
    localfiletmp = perform_lo->filelist->next;

    /*handle files*/
    while(localfiletmp != NULL && !exit_loop)
    {
        if(asus_cfg.prule[index]->rule != 1)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->server_action_list);
            ret = Upload(localfiletmp->path,index);
            if(ret == NE_OK || ret == SERVER_SPACE_NOT_ENOUGH || ret == LOCAL_FILE_LOST)
            {
                if(ret == NE_OK)
                {
                    char *serverpath;
                    serverpath = localpath_to_serverpath(localfiletmp->path,index);
                    //printf("serverpath = %s\n",serverpath);
                    time_t modtime;
                    modtime = Getmodtime(serverpath,index);
                    if(modtime != -1)
                        ChangeFile_modtime(localfiletmp->path,modtime,index);
                    else
                    {
                        DEBUG("ChangeFile_modtime failed!\n");
                    }
                    free(serverpath);
                }
            }
            else
            {
                return ret;
            }
        }
        else
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("createfile",localfiletmp->path,g_pSyncList[index]->download_only_socket_head);
        }

        localfiletmp = localfiletmp->next;
    }

    /*handle folders*/
    while(localfoldertmp != NULL && !exit_loop)
    {
        if(asus_cfg.prule[index]->rule != 1)
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->server_action_list);
            ret = Mkcol(localfoldertmp->path,index);
            if(ret != NE_OK && ret != LOCAL_FILE_LOST)
            {
                return ret;
            }
        }
        else
        {
            if(wait_handle_socket(index))
            {
                return HAVE_LOCAL_SOCKET;
            }
            add_action_item("createfolder",localfoldertmp->path,g_pSyncList[index]->download_only_socket_head);
        }

        localfoldertmp = localfoldertmp->next;
    }

    if(exit_loop)
        ret = NE_WEBDAV_QUIT;

    return ret;
}

int sync_local_to_server(char *path,int index){

    //DEBUG("sync_local_to_server path = %s\n",path);

    Local *localnode;
    int ret = 0;
    int res = 0;
    LocalFolder *localfoldertmp;

    localnode = Find_Floor_Dir(path);

    if(localnode == NULL)
    {
        return 0;
    }

    ret = is_server_have_localpath(path,g_pSyncList[index]->ServerRootNode->Child,index);

    if(ret == 0)
    {
        res = sync_local_to_server_init(localnode,index);
        if(res != 0 && res != SERVER_SPACE_NOT_ENOUGH && res != LOCAL_FILE_LOST)
        {
            DEBUG("##########res = %d\n",res);
            free_localfloor_node(localnode);
            return res;
        }
    }

    localfoldertmp = localnode->folderlist->next;
    while(localfoldertmp != NULL && !exit_loop)
    {
        res = sync_local_to_server(localfoldertmp->path,index);
        if(res != 0 && res != SERVER_SPACE_NOT_ENOUGH && res != LOCAL_FILE_LOST)
        {
            DEBUG("##########res = %d\n",res);
            free_localfloor_node(localnode);
            return res;
        }

        localfoldertmp = localfoldertmp->next;
    }

    free_localfloor_node(localnode);

    if(exit_loop)
        res = NE_WEBDAV_QUIT;

    return res;
}

int compareLocalList(int index){

    DEBUG("compareLocalList start!\n");
    int ret = 0;

    if(g_pSyncList[index]->ServerRootNode->Child != NULL)
    {
        DEBUG("ServerRootNode->Child != NULL\n");
        ret = sync_server_to_local(g_pSyncList[index]->ServerRootNode->Child,sync_server_to_local_perform,index);
    }
    else
    {
        DEBUG("ServerRootNode->Child == NULL\n");
    }

    return ret;
}

#if 1
/*
 *used for initial
 *if the local root folder is null,
 *it will download all the files from Server
*/
int wd_initial(){
    int ret;
    int status;
    int i;
    //struct timeval now;
    //struct timespec outtime;
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        if(g_pSyncList[i]->init_completed)
            continue;

        g_pSyncList[i]->server_sync_running = 1;
        if(disk_change)
        {
            //disk_change = 0;
            check_disk_change();
        }
        if(exit_loop)
            break;
        if(g_pSyncList[i]->sync_disk_exist == 0)
        {
            write_log(S_ERROR,"Sync disk unplug!","",i);
            continue;
        }

        write_log(S_INITIAL,"","",i);
        ret = 1;

        DEBUG("it is %d init \n",i);

        //while(ret != 0 && ret != NE_WEBDAV_QUIT && exit_loop == 0)
        if(exit_loop == 0)
        {
            DEBUG("wd_initial ret = %d\n",ret);

            free_server_tree(g_pSyncList[i]->ServerRootNode);
            g_pSyncList[i]->ServerRootNode = NULL;

            DEBUG("wd_initial free_server_tree end\n");

#if LIST_TEST
            //do{
            g_pSyncList[i]->ServerRootNode = create_server_treeroot();
            status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode,i);
            //SearchServerTree(ServerRootNode);

            DEBUG("wd_initial browse_to_tree status = %d\n",status);
            //sleep(2);
            usleep(1000*200);
            if(status != NE_OK)
                continue;
            //usleep(1000*200);
            //}while(status != NE_OK && exit_loop == 0);
#endif

            if(exit_loop == 0)
            {
                if(test_if_dir_empty(asus_cfg.prule[i]->base_path))
                {
                    DEBUG("base_path is blank\n");

                    if(asus_cfg.prule[i]->rule != 2)
                    {
                        if(g_pSyncList[i]->ServerRootNode->Child != NULL)
                        {
                            DEBUG("######## init sync folder,please wait......#######\n");

                            ret = initMyLocalFolder(g_pSyncList[i]->ServerRootNode->Child,i);
                            if(ret != 0 && ret != NE_WEBDAV_QUIT)
                                continue;
                            else if(ret == NE_WEBDAV_QUIT)
                                break;
                            g_pSyncList[i]->init_completed = 1;
                            g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;

                            DEBUG("######## init sync folder end#######\n");
                        }
                    }
                    else
                    {
                        g_pSyncList[i]->init_completed = 1;
                        g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                        ret = 0;
                    }
                }
                else
                {
                    //if(sync_up == 1)
                    //{
                    DEBUG("######## init sync folder(have files),please wait......#######\n");

                    if(g_pSyncList[i]->ServerRootNode->Child != NULL)
                        ret = sync_server_to_local(g_pSyncList[i]->ServerRootNode->Child,sync_server_to_local_init,i);
                    //printf("**********ret1 = %d\n",ret);
                    if(ret != 0 && ret != NE_WEBDAV_QUIT)
                        continue;
                    else if(ret == NE_WEBDAV_QUIT)
                        break;
                    //if(sync_down == 1)
                    //{
                    ret = sync_local_to_server(asus_cfg.prule[i]->base_path,i);

                    DEBUG("#########ret = %d\n",ret);
                    //}
                    if(ret != 0 && ret != NE_WEBDAV_QUIT)
                        continue;
                    else if(ret == NE_WEBDAV_QUIT)
                        break;

                    DEBUG("######## init sync folder end#######\n");
                    //}
                    g_pSyncList[i]->init_completed = 1;
                    g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
                }
            }
        }
        if(ret == 0)
            write_log(S_SYNC,"","",i);
        g_pSyncList[i]->server_sync_running = 0;
    }


    return ret;
}
#endif

void *SyncServer(){
    struct timeval now;
    struct timespec outtime;

    int status;
    int i;
    int complete_sync = 0;

    while (!exit_loop)
    {
        DEBUG("***************SyncServer start**************\n");
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            g_pSyncList[i]->server_sync_running = 1;
            status = 0;
            DEBUG("the %d SyncServer\n",i);

            while (local_sync == 1 && exit_loop == 0){
                //printf("local_sync = %d\n",local_sync);
                //sleep(2);
                usleep(1000*10);
                //server_sync = 0;
            }
            server_sync = 1;      //server sync starting
            //pthread_mutex_unlock(&mutex_sync);
            if(disk_change)
            {
                //disk_change = 0;
                check_disk_change();
            }

            if(exit_loop)
                break;

            if(g_pSyncList[i]->sync_disk_exist == 0)
            {
                write_log(S_ERROR,"Sync disk unplug!","",i);
                continue;
            }

            if(g_pSyncList[i]->no_local_root)
            {
                my_mkdir_r(asus_cfg.prule[i]->base_path,i);   //have mountpath
                send_action(1,asus_cfg.prule[i]->base_path);
                usleep(1000*10);
                g_pSyncList[i]->no_local_root = 0;
                g_pSyncList[i]->init_completed = 0;
            }

            status = do_unfinished(i);

            DEBUG("do_unfinished completed!\n");

            //dounfinish = 0;
            if(status != 0 && status != -1)
            {
                server_sync = 0;      //server sync finished
                //sleep(2);
                usleep(1000*200);
                break;
            }

            if(g_pSyncList[i]->init_completed == 0)
                status = wd_initial();

            if(status != 0)
                continue;

            if(asus_cfg.prule[i]->rule == 2)
            {
                //write_log(S_SYNC,"","",i);
                continue;
            }
            
            if(exit_loop == 0)
            {
                //server_sync = 1;      //server sync starting
#if LIST_TEST
                DEBUG("get ServerRootNode\n");

                g_pSyncList[i]->ServerRootNode = create_server_treeroot();
                status = browse_to_tree(asus_cfg.prule[i]->rooturl,g_pSyncList[i]->ServerRootNode,i);
                //SearchServerTree(ServerRootNode);
                if (status != NE_OK)
                {
                    DEBUG("get ServerList ERROR! \n");
                    //server_sync = 0;      //server sync finished

                    if(exit_loop == 0)
                        check_link_internet(i);

                    g_pSyncList[i]->first_sync = 1;
                    //sleep(2);
                    usleep(1000*20);
                    continue;
                    //break;
                }
                //sleep(2);
                //usleep(1000*200);
#endif
                if(g_pSyncList[i]->unfinished_list->next != NULL)
                {
                    continue;
                }

                //printf("status = %d\n",status);
                if(asus_cfg.prule[i]->rule == 0)
                {
                    status = compareServerList(i);
                }

                if(g_pSyncList[i]->first_sync)
                {
                    DEBUG("first sync!\n");
#if LIST_TEST
                    free_server_tree(g_pSyncList[i]->RealOldServerRootNode);
                    g_pSyncList[i]->RealOldServerRootNode = g_pSyncList[i]->OldServerRootNode;
                    //free_server_tree(g_pSyncList[i]->OldServerRootNode);
                    g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
#endif
                    //getLocalList();
                    status = compareLocalList(i);
                    if(status == 0)
                    {
                        complete_sync = 1;
                        g_pSyncList[i]->first_sync = 0;
                    }
                    else
                    {
                        complete_sync = 0;
                    }
                }
                else
                {
                    //serverList different or download only
                    if (status == 0 || asus_cfg.prule[i]->rule == 1)
                    {
#if LIST_TEST
                        free_server_tree(g_pSyncList[i]->RealOldServerRootNode);
                        g_pSyncList[i]->RealOldServerRootNode = g_pSyncList[i]->OldServerRootNode;
                        //free_server_tree(g_pSyncList[i]->OldServerRootNode);
                        g_pSyncList[i]->OldServerRootNode = g_pSyncList[i]->ServerRootNode;
#endif
                        //getLocalList();
                        status = compareLocalList(i);
                        if(status != 0)
                        {
                            complete_sync = 0;
                            g_pSyncList[i]->first_sync = 1;
                        }
                        else
                        {
                            complete_sync = 1;
                        }
                    }
                    else
                    {
#if LIST_TEST
                        free_server_tree(g_pSyncList[i]->ServerRootNode);
                        g_pSyncList[i]->ServerRootNode = NULL;
#endif
                    }
                }


            }
            if(complete_sync)
            {
                write_log(S_SYNC,"","",i);
            }
            else
            {
                write_log(S_ERROR,"Local sync hronization is not entirely successful,failue inofrmation please refer error log!","",i);
            }
            g_pSyncList[i]->server_sync_running = 0;
        }
        //write_log(S_SYNC,"","");
        server_sync = 0;      //server sync finished
        pthread_mutex_lock(&mutex);
        if(!exit_loop)
        {
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + 10;
            outtime.tv_nsec = now.tv_usec * 1000;
            pthread_cond_timedwait(&cond, &mutex, &outtime);
        }
        pthread_mutex_unlock(&mutex);
    }

    DEBUG("stop WebDAV server sync\n");
    stop_up = 1;

}

void* sigmgr_thread(){
    sigset_t   waitset;
    //siginfo_t  info;
    int        sig;
    int        rc;
    pthread_t  ppid = pthread_self();

    pthread_detach(ppid);

    sigemptyset(&waitset);
    sigaddset(&waitset,SIGUSR1);
    sigaddset(&waitset,SIGUSR2);
    sigaddset(&waitset,SIGTERM);

    while (1)  {
        rc = sigwait(&waitset, &sig);
        if (rc != -1) {
            DEBUG("sigwait() fetch the signal - %d\n", sig);
            sig_handler(sig);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}

void sig_handler (int signum)
{
    switch (signum)
    {
    case SIGTERM:case SIGUSR2:
        {

            if(signum == SIGUSR2)
            {
                DEBUG("signal is SIGUSR2\n");
                mountflag = 0;
                //sleep(20);
                FILE *fp;
                fp = fopen("/proc/mounts","r");
                if(fp == NULL)
                {
                    printf("open /proc/mounts fail\n");
                }
                char a[1024];
                while(!feof(fp))
                {
                    memset(a,'\0',1024);
                    fscanf(fp,"%[^\n]%*c",a);
                    //printf("\nmount = %s\n",a);
                    if(strstr(a,"/dev/sd"))
                    {
                        mountflag = 1;
                        break;
                    }
                }
                fclose(fp);
            }
            //printf("mountflag = %d\n",mountflag);
            if(signum == SIGTERM || mountflag == 0)
            {
                sync_up = 0;
                sync_down = 0;
                stop_progress = 1;
                exit_loop = 1;
                //pthread_mutex_lock(&mutex);

#ifndef NVRAM_
#ifndef PC
                system(SH_GET_NVRAM_SCRIPT_1);
                sleep(2);
                if(create_webdav_conf_file(&asus_cfg_stop) == -1)
                {
                    printf("create_webdav_conf_file fail\n");
                    return;
                }
#endif
#else
                if(convert_nvram_to_file_mutidir(CONFIG_PATH,&asus_cfg_stop) == -1)
                {
                    printf("convert_nvram_to_file fail\n");
                    //nvram_set(NVRAM_USBINFO,"");
                    //nvram_commit();
                    write_to_nvram("","wd_tokenfile");   //the "" maybe cause errors
                    return;
                }
#endif
                if(asus_cfg_stop.dir_number == 0)
                {
                    //nvram_set(NVRAM_USBINFO,"");
                    //nvram_commit();
                    del_rule_num = 0;
                    char *filename;
                    filename = my_str_malloc(strlen(asus_cfg.prule[0]->mount_path)+18+1);
#ifndef PC
                    sprintf(filename,"%s/.webdav_tokenfile",asus_cfg.prule[0]->mount_path);
#else
                    sprintf(filename,".webdav_tokenfile");
#endif
                    remove(filename);
                    free(filename);
                    remove(g_pSyncList[0]->conflict_file);
#ifdef NVRAM_
                    write_to_nvram("","wd_tokenfile");   //the "" maybe cause errors
#else
                    write_to_wd_tokenfile("");
#endif
                }
                else
                {
                    if(asus_cfg_stop.dir_number != asus_cfg.dir_number)
                    {
                        parse_config_mutidir(CONFIG_PATH,&asus_cfg_stop);
                        //reget_usbinfo_config();
                        //write_usbinfo_config();
                        rewrite_tokenfile_and_nv();
                    }
                }
                //pthread_mutex_unlock(&mutex);
                sighandler_finished = 1;
                pthread_cond_signal(&cond);
                pthread_cond_signal(&cond_socket);
                pthread_cond_signal(&cond_log);
            }
            else
            {
                pthread_mutex_lock(&mutex_checkdisk);
                disk_change = 1;
                pthread_mutex_unlock(&mutex_checkdisk);
                sighandler_finished = 1;
            }
            break;
        }

    case SIGUSR1:  // add user
        DEBUG("signal is SIGUSER1\n");
        //signal(SIGINT, SIG_DFL);
        //no_config = 0;
        exit_loop = 1;
        read_config();
        sighandler_finished = 1;
        //restart_run = 1;
        //run();
        break;
#if 0
    case SIGUSR2:  // Plug the hard disk
        DEBUG("signal is SIGUSR2\n");
        //signal(SIGINT, SIG_DFL);
        //no_config = 0;
        //exit_loop = 1;
        //sync_up = 0;
        //sync_down = 0;
        disk_change = 1;
        sighandler_finished = 1;
        break;
#endif
#if 0
    case SIGPIPE:  // delete user
        printf("signal is SIGPIPE\n");

        signal(SIGPIPE, SIG_IGN);
        //no_config = 0;
        //stop_progress = 1;
        //exit_loop = 1;
        //sync_up = 0;
        //sync_down = 0;
        //restart_run = 1;
        //run();

        break;
#endif
    default:
        break;
    }
}

//just add unfinished upload files
void add_unfinished_list_to_file(int index){

    //printf("enter add_unfinished_list_to_file\n");

    if(g_pSyncList[index]->unfinished_list->next != NULL)
    {
        action_item *p;
        char *localpath;
        p = g_pSyncList[index]->unfinished_list->next;

        DEBUG("p->path = %s\n",p->path);
        while(p != NULL)
        {
            if(!(strcmp(p->action,"upload")))
            {
                localpath = strstr(p->path,asus_cfg.prule[index]->base_path) + asus_cfg.prule[index]->base_path_len;
                FILE *fp;
                //printf("localpath = %s\n",localpath);
                //printf("g_pSyncList[index]->up_item_file = %s\n",g_pSyncList[index]->up_item_file);
                fp = fopen(g_pSyncList[index]->up_item_file,"w");
                //printf("open %s OK!\n",g_pSyncList[index]->up_item_file);
                if(fp == NULL)
                {
                    DEBUG("open %s error\n",g_pSyncList[index]->up_item_file);
                    //return NULL;
                }
                fprintf(fp,"%s",localpath);
                fclose(fp);
                //printf("close fp!\n");
                return ;
            }
            p = p->next;
        }
    }
}

void clean_up()
{
    DEBUG("enter clean up\n");

    while(!sighandler_finished)
    {
        sleep(1);
    }
    int i;


    for(i=0;i<asus_cfg.dir_number;i++)
    {
        add_unfinished_list_to_file(i);

        queue_destroy(g_pSyncList[i]->SocketActionList);
        queue_destroy(g_pSyncList[i]->SocketActionList_Priority);
        //#ifndef NVRAM_
        if(g_pSyncList[i]->sync_disk_exist)
        {
            ne_session_destroy(asus_cfg.prule[i]->sess);
        }
        DEBUG("clean %d sess success!\n",i);

        //printf("the pointer g_pSyncList[i]->ServerRootNode = %p\n",g_pSyncList[i]->ServerRootNode);
        if(g_pSyncList[i]->ServerRootNode == g_pSyncList[i]->OldServerRootNode)
        {
            DEBUG("the same Pointer!\n");

            if(g_pSyncList[i]->ServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->ServerRootNode);
        }
        else
        {
            if(g_pSyncList[i]->ServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->ServerRootNode);

            DEBUG("clean %d ServerRootNode success!\n",i);

            if(g_pSyncList[i]->OldServerRootNode != NULL)
                free_server_tree(g_pSyncList[i]->OldServerRootNode);

            DEBUG("clean %d OldServerRootNode success!\n",i);
        }

        free_action_item(g_pSyncList[i]->server_action_list);
        free_action_item(g_pSyncList[i]->dragfolder_action_list);
        free_action_item(g_pSyncList[i]->unfinished_list);
        free_action_item(g_pSyncList[i]->copy_file_list);
        free_action_item(g_pSyncList[i]->up_space_not_enough_list);
        if(asus_cfg.prule[i]->rule == 1)
        {
            free_action_item(g_pSyncList[i]->download_only_socket_head);
        }
        free(g_pSyncList[i]);
        //printf("clean %d up_space_not_enough_list success!\n",i);

    }
    free(g_pSyncList);


#if 0
    if(LocalRootNode != NULL)
        free_tree_node(LocalRootNode);
    else
        printf("LocalRootNode is NULL!\n");
#endif


    DEBUG("clean up end !!!\n");
}

void stop_process_clean_up(){

//    unlink("/tmp/smartsync_app/webdav_client_start");

    unlink("/tmp/notify/usb/webdav_client");
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond_socket);
    pthread_cond_destroy(&cond_log);

    unlink(general_log);
    //remove(NOTIFY_PATH);

}

#ifdef NVRAM_
/*Type>Desc>URL>Rule>capacha>LocalFolder*/
int convert_nvram_to_file_mutidir(char *file,struct asus_config *config)
{
    DEBUG("enter convert_nvram_to_file_mutidir function\n");

    FILE *fp;
    char *nv, *nvp, *b;
    //struct asus_config config;
    int i;
    int j = 0;
    //int status;
    char *p;
    char *buffer;
    char *buf;

    fp=fopen(file, "w");

    if (fp==NULL) return -1;

    //nv = nvp = strdup(nvram_safe_get("cloud_sync"));
#ifndef USE_TCAPI
    nv = nvp = strdup(nvram_safe_get("cloud_sync"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "cloud_sync", tmp);
    nvp = nv = my_str_malloc(strlen(tmp)+1);
    sprintf(nv,"%s",tmp);
#endif

    //printf("otain nvram end\n");

    if(nv)
    {
        while ((b = strsep(&nvp, "<")) != NULL)
        {
            i = 0;
            buf = buffer = strdup(b);

            DEBUG("buffer = %s\n",buffer);

            while((p = strsep(&buffer,">")) != NULL)
            {
                DEBUG("p = %s\n",p);

                if (*p == 0)
                {
                    fprintf(fp,"\n");
                    i++;
                    continue;
                }
                if(i == 0)
                {
                    if(atoi(p) != 1)
                        break;
                    if(j > 0)
                    {
                        fprintf(fp,"\n%s",p);
                    }
                    else
                    {
                        fprintf(fp,"%s",p);
                    }
                    j++;
                }
                else
                {
                    fprintf(fp,"\n%s",p);
                }
                i++;
            }
            free(buf);
            //status = parse_nvram(b,&config);
            /*if(status == -1)
            {
                printf("%s:%d parse nvram fail\n",__FILE__,__LINE__);
                return -1;
            }
            else if(config.type != 0)
                continue;*/
            /*else
            {
                fprintf(fp, "%d,%s,%s,%s,%d,%d,%d,",
                        config.type,config.user,config.pwd,config.url,config.enable,
                        config.ismuti,config.dir_number);
                for(i=0;i<config.dir_number;i++)
                {
                    if( i == config.dir_number - 1)
                       fprintf(fp,"%d,%s",config.prule[i]->rule,config.prule[i]->path);
                    else
                       fprintf(fp,"%d,%s,",config.prule[i]->rule,config.prule[i]->path);
                }

                fprintf(fp,"\n");

                for(i=0;i<config.dir_number;i++)
                {
                    my_free(config.prule[i]);
                }
                my_free(config.prule);

                break;
            }*/

        }
        DEBUG("j = %d\n",j);
        free(nv);
        config->dir_number = j;
    }
    else
        DEBUG("get nvram fail\n");
    fclose(fp);
    DEBUG("end convert_nvram_to_file_mutidir function\n");
    return 0;
}
#else
int create_webdav_conf_file(struct asus_config *config){

    DEBUG("enter create_webdav_conf_file function\n");

    FILE *fp;
    char *nv, *nvp, *b;
    int i;
    int j = 0;
    char *p;
    char *buffer;
    char *buf;

    fp = fopen("/tmp/smartsync/webdav/config/webdav_tmpconfig","r");
    if (fp==NULL)
    {
        nvp = my_str_malloc(2);
        sprintf(nvp,"");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        nvp =  (char *)malloc( file_size * sizeof( char ) );
        fread(nvp , file_size , sizeof(char) , fp);
        fclose(fp);
    }

    nv = nvp;

    replace_char_in_str(nvp,'\0','\n');

    DEBUG("**********nv = %s\n",nv);

    fp=fopen(CONFIG_PATH, "w");

    if (fp==NULL) return -1;

    //nv = nvp = strdup(nvram_safe_get("cloud_sync"));

    //printf("otain nvram end\n");

    if(nv)
    {
        while ((b = strsep(&nvp, "<")) != NULL)
        {
            i = 0;
            buf = buffer = strdup(b);

            DEBUG("buffer = %s\n",buffer);

            while((p = strsep(&buffer,">")) != NULL)
            {
                DEBUG("p = %s\n",p);

                if (*p == 0)
                {
                    fprintf(fp,"\n");
                    i++;
                    continue;
                }
                if(i == 0)
                {
                    if(atoi(p) != 1)
                        break;
                    if(j > 0)
                    {
                        fprintf(fp,"\n%s",p);
                    }
                    else
                    {
                        fprintf(fp,"%s",p);
                    }
                    j++;
                }
                else
                {
                    fprintf(fp,"\n%s",p);
                }
                i++;
            }
            free(buf);

        }

        free(nv);
        config->dir_number = j;
    }
    else
        DEBUG("get nvram fail\n");
    fclose(fp);
    return 0;

}


int get_dir_number(char *file,struct asus_config *config)
{
    int i = 0;
    FILE *fp;
    char buffer[256];

    if(access(file,F_OK) == 0)
    {
        if((fp = fopen(file,"rb")) == NULL)
        {
            fprintf(stderr,"read Cloud error");
            return -2;
        }
        while(fgets(buffer,255,fp) != NULL)
        {
            i++;
        }
        fclose(fp);
        config->dir_number = i/6;
        return 0;
    }
    return -1;
}
#endif


#if 0
int get_usbinfo_config(struct asus_config *config){

    DEBUG("get_usbinfo_config");

    int i;
    if(initial_disk_data(&config_disk_info_start) == NULL){
        return -1;
    }

    config_disk_info = config_disk_info_start;
    
    for(i=0;i<config->dir_number;i++)
    {
        DEBUG("for i = %d\n",i);

        if(initial_disk_data(&config_disk_tmp) == NULL)
        {
            return -1;
        }
        follow_disk_tmp = follow_disk_info_start->next;
        while(follow_disk_tmp != NULL)
        {
            DEBUG("while follow_disk_tmp->mountpath : %s\n",follow_disk_tmp->mountpath);

            char *disk_mount_path_tmp;
            disk_mount_path_tmp = follow_disk_tmp->mountpath+4;

            DEBUG("disk_mount_path_tmp = %s\n",disk_mount_path_tmp);

            //printf("asus_cfg.prule[i]->mount_path = %s\n",asus_cfg.prule[i]->mount_path);
            if( !strcmp(disk_mount_path_tmp,config->prule[i]->mount_path) )
            {
                config->prule[i]->partitionport = follow_disk_tmp->partitionport;
                sprintf(config->prule[i]->product,"%s",follow_disk_tmp->product);
                sprintf(config->prule[i]->vendor,"%s",follow_disk_tmp->vendor);
                sprintf(config->prule[i]->serialnum,"%s",follow_disk_tmp->serialnum);

                config_disk_tmp->partitionport = follow_disk_tmp->partitionport;

                config_disk_tmp->vendor = my_str_malloc(strlen(follow_disk_tmp->vendor)+1);
                sprintf(config_disk_tmp->vendor,"%s",follow_disk_tmp->vendor);

                config_disk_tmp->product = my_str_malloc(strlen(follow_disk_tmp->product)+1);
                sprintf(config_disk_tmp->product,"%s",follow_disk_tmp->product);

                config_disk_tmp->serialnum = my_str_malloc(strlen(follow_disk_tmp->serialnum)+1);
                sprintf(config_disk_tmp->serialnum,"%s",follow_disk_tmp->serialnum);

                config_disk_tmp->url = my_str_malloc(strlen(config->prule[i]->rooturl)+1);
                sprintf(config_disk_tmp->url,"%s",config->prule[i]->rooturl);

                config_disk_info->next = config_disk_tmp;
                config_disk_info = config_disk_tmp;

                DEBUG("config->prule[i]->product:%s\n",config->prule[i]->product);
                DEBUG("config->prule[i]->serialnum:%s\n",config->prule[i]->serialnum);
                DEBUG("config->prule[i]->vendor:%s\n",config->prule[i]->vendor);
                DEBUG("config->prule[i]->partitionport:%d\n",config->prule[i]->partitionport);

                break;
            }
            follow_disk_tmp = follow_disk_tmp->next;
        }
    }

    return 0;
}
#endif

int renew_config_path()
{
    int i;
    int removed = 0;
    char cmp_mount_path[256];

    int has_tmp = 0;
    char new_mount_path[256];
    char tmp[256];


    for(i=0;i<asus_cfg.dir_number;i++)
    {
        config_disk_tmp = config_disk_info_start->next;
        while(config_disk_tmp != NULL)
        {
            if(!strcmp(config_disk_tmp->url,asus_cfg.prule[i]->rooturl))
            {
                asus_cfg.prule[i]->partitionport = config_disk_tmp->partitionport;
                sprintf(asus_cfg.prule[i]->product,"%s",config_disk_tmp->product);
                sprintf(asus_cfg.prule[i]->vendor,"%s",config_disk_tmp->vendor);
                sprintf(asus_cfg.prule[i]->serialnum,"%s",config_disk_tmp->serialnum);
                break;
            }
            config_disk_tmp=config_disk_tmp->next;
        }
    }

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        follow_disk_tmp = follow_disk_info_start->next;
        while(follow_disk_tmp != NULL)
        {
            if( !strcmp(asus_cfg.prule[i]->product,follow_disk_tmp->product) &&
                !strcmp(asus_cfg.prule[i]->serialnum,follow_disk_tmp->serialnum) &&
                !strcmp(asus_cfg.prule[i]->vendor,follow_disk_tmp->vendor) &&
                asus_cfg.prule[i]->partitionport == follow_disk_tmp->partitionport)
            {
                memset(new_mount_path,0,sizeof(new_mount_path));
                memset(cmp_mount_path,0,sizeof(cmp_mount_path));
                if(!strncmp(asus_cfg.prule[i]->mount_path,"/tmp",4))
                {
                    strcpy(cmp_mount_path,asus_cfg.prule[i]->mount_path);
                    strcpy(new_mount_path,follow_disk_tmp->mountpath);
                    has_tmp = 1;
                }
                else
                {
                    sprintf(cmp_mount_path,"/tmp%s",asus_cfg.prule[i]->mount_path);
                    strncpy(new_mount_path,follow_disk_tmp->mountpath+4,strlen(follow_disk_tmp->mountpath)-4);
                }

                DEBUG("cmp_mount_path=%s,new_mount_path=%s\n",cmp_mount_path,new_mount_path);

                if(strcmp(follow_disk_tmp->mountpath,cmp_mount_path)) // mount_path change;
                {
                    //需要修改Config档中的SyncPath
                    removed = 2;
                    memset(tmp,0,sizeof(tmp));
                    sprintf(tmp,"%s",asus_cfg.prule[i]->base_path+(strlen(asus_cfg.prule[i]->mount_path)));

                    memset(asus_cfg.prule[i]->mount_path,0,sizeof(asus_cfg.prule[i]->mount_path));
                    strcpy(asus_cfg.prule[i]->mount_path,new_mount_path);

                    memset(asus_cfg.prule[i]->base_path,0,sizeof(asus_cfg.prule[i]->base_path));
                    sprintf(asus_cfg.prule[i]->base_path,"%s%s",asus_cfg.prule[i]->mount_path,tmp);

                    asus_cfg.prule[i]->base_path_len = strlen(asus_cfg.prule[i]->base_path);

                    DEBUG("asus_cfg.prule[%d]->base_path = %s\n",i,asus_cfg.prule[i]->base_path);
                }
                break;
            }
            follow_disk_tmp=follow_disk_tmp->next;
        }
    }

    /*if(removed == 2)
    {
        rewrite_config();
    }*/

    return 0;
}

#ifdef NVRAM_
int write_to_nvram(char *contents,char *nv_name)
{
    char *command;
    command = my_str_malloc(strlen(contents)+strlen(SHELL_FILE)+strlen(nv_name)+8);
    sprintf(command,"sh %s \"%s\" %s",SHELL_FILE,contents,nv_name);

    DEBUG("command : [%s]\n",command);

    system(command);
    free(command);

    return 0;
}
#else
int write_to_wd_tokenfile(char *contents)
{
    if(contents == NULL || contents == "")
    {
        unlink(TOKENFILE_RECORD);
        return 0;
    }
    FILE *fp;
    if( ( fp = fopen(TOKENFILE_RECORD,"w") ) == NULL )
    {
        printf("open wd_tokenfile failed!\n");
        return -1;
    }
    fprintf(fp,"%s",contents);
    fclose(fp);
    //printf("write_to_wd_tokenfile end\n");
    return 0;
}
#endif

int write_to_tokenfile(char *mpath)
{
    //write_debug_log("write_to_tokenfile");
    printf("write_to_tokenfile start\n");
    FILE *fp;
    //int flag=0;

    char *filename;
    filename = my_str_malloc(strlen(mpath)+18+1);
#ifndef PC
    sprintf(filename,"%s/.webdav_tokenfile",mpath);
#else
    sprintf(filename,".webdav_tokenfile");
#endif
    printf("filename = %s\n",filename);

    int i = 0;
    if( ( fp = fopen(filename,"w") ) == NULL )
    {
        printf("open tokenfile failed!\n");
        return -1;
    }

    tokenfile_info_tmp = tokenfile_info_start->next;
    while(tokenfile_info_tmp != NULL)
    {
        printf("tokenfile_info_tmp->mountpath = %s\n",tokenfile_info_tmp->mountpath);
        if(!strcmp(tokenfile_info_tmp->mountpath,mpath))
        {
            //write_debug_log(tokenfile_info_tmp->folder);
            printf("tokenfile_info_tmp->url = %s\n",tokenfile_info_tmp->url);
            if(i == 0)
            {
                fprintf(fp,"%s\n%s",tokenfile_info_tmp->url,tokenfile_info_tmp->folder);
                i=1;
            }
            else
            {
                fprintf(fp,"\n%s\n%s",tokenfile_info_tmp->url,tokenfile_info_tmp->folder);
            }
            //flag = 1;
        }

        tokenfile_info_tmp = tokenfile_info_tmp->next;
    }

    fclose(fp);
    if(!i)
        remove(filename);
    free(filename);

    printf("write_to_tokenfile end\n");
    return 0;
}

int check_config_path(int is_read_config)
{
    printf("check_config_path start\n");
    int i;
    int flag;
    char *nv;
    char *nvp;
    char *new_nv;
    int nv_len;
    int is_path_change = 0;

#ifdef NVRAM_
    //nv = strdup(nvram_safe_get("wd_tokenfile"));
#ifndef USE_TCAPI
    nv = strdup(nvram_safe_get("wd_tokenfile"));
#else
    char tmp[MAXLEN_TCAPI_MSG] = {0};
    tcapi_get(AICLOUD, "wd_tokenfile", tmp);
    nv = my_str_malloc(strlen(tmp)+1);
    sprintf(nv,"%s",tmp);
#endif
#else
    FILE *fp;
    fp = fopen(TOKENFILE_RECORD,"r");
    if(fp==NULL)
    {
        nv = my_str_malloc(2);
        sprintf(nv,"");
    }
    else
    {
        fseek( fp , 0 , SEEK_END );
        int file_size;
        file_size = ftell( fp );
        fseek(fp , 0 , SEEK_SET);
        //nv =  (char *)malloc( file_size * sizeof( char ) );
        nv = my_str_malloc(file_size+2);
        //fread(nv , file_size , sizeof(char) , fp);
        fscanf(fp,"%[^\n]%*c",nv);
        fclose(fp);
    }


#endif
    nv_len = strlen(nv);

    //write_debug_log("check_config_path");
    printf("nv_len = %d\n",nv_len);
    printf("nv = %s\n",nv);

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        flag = 0;
        tokenfile_info_tmp = tokenfile_info_start->next;
        while(tokenfile_info_tmp != NULL)
        {
            if( !strcmp(tokenfile_info_tmp->url,asus_cfg.prule[i]->rooturl) &&
                !strcmp(tokenfile_info_tmp->folder,asus_cfg.prule[i]->base_folder))
            {
                if(strcmp(tokenfile_info_tmp->mountpath,asus_cfg.prule[i]->mount_path))
                {
                    memset(asus_cfg.prule[i]->mount_path,0,sizeof(asus_cfg.prule[i]->mount_path));
                    sprintf(asus_cfg.prule[i]->mount_path,"%s",tokenfile_info_tmp->mountpath);
                    memset(asus_cfg.prule[i]->base_path,0,sizeof(asus_cfg.prule[i]->base_path));
                    sprintf(asus_cfg.prule[i]->base_path,"%s%s",tokenfile_info_tmp->mountpath,tokenfile_info_tmp->folder);
                    asus_cfg.prule[i]->base_path_len = strlen(asus_cfg.prule[i]->base_path);
                    is_path_change = 1;
                }
                if(!is_read_config)
                {
                    if(g_pSyncList[i]->sync_disk_exist == 0)
                        is_path_change = 1;   //plug the disk and the mout_path not change
                }
                flag = 1;
                break;
            }
            tokenfile_info_tmp = tokenfile_info_tmp->next;
        }
        if(!flag)
        {
            nvp = my_str_malloc(strlen(asus_cfg.prule[i]->rooturl)+strlen(asus_cfg.prule[i]->base_folder)+2);
            sprintf(nvp,"%s>%s",asus_cfg.prule[i]->rooturl,asus_cfg.prule[i]->base_folder);

            //write_debug_log(nv);
            //write_debug_log(nvp);
            printf("nvp = %s\n",nvp);

            if(!is_read_config)
            {
                if(g_pSyncList[i]->sync_disk_exist == 1)
                    is_path_change = 2;   //remove the disk and the mout_path not change
            }

            //printf("write nvram and tokenfile if before\n");

            if(strstr(nv,nvp) == NULL)
            {
                //printf("write nvram and tokenfile if behind");

                if(initial_tokenfile_info_data(&tokenfile_info_tmp) == NULL)
                {
                    return -1;
                }
                tokenfile_info_tmp->url = my_str_malloc(strlen(asus_cfg.prule[i]->rooturl)+1);
                sprintf(tokenfile_info_tmp->url,"%s",asus_cfg.prule[i]->rooturl);

                tokenfile_info_tmp->mountpath = my_str_malloc(strlen(asus_cfg.prule[i]->mount_path)+1);
                sprintf(tokenfile_info_tmp->mountpath,"%s",asus_cfg.prule[i]->mount_path);

                tokenfile_info_tmp->folder = my_str_malloc(strlen(asus_cfg.prule[i]->base_folder)+1);
                sprintf(tokenfile_info_tmp->folder,"%s",asus_cfg.prule[i]->base_folder);

                tokenfile_info->next = tokenfile_info_tmp;
                tokenfile_info = tokenfile_info_tmp;

                write_to_tokenfile(asus_cfg.prule[i]->mount_path);

                if(nv_len)
                {
                    new_nv = my_str_malloc(strlen(nv)+strlen(nvp)+2);
                    sprintf(new_nv,"%s<%s",nv,nvp);

                }
                else
                {
                    new_nv = my_str_malloc(strlen(nvp)+1);
                    sprintf(new_nv,"%s",nvp);
                }
                printf("new_nv = %s\n",new_nv);
#ifdef NVRAM_
                write_to_nvram(new_nv,"wd_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif
                free(new_nv);
            }
            free(nvp);
        }
    }
    free(nv);
    return is_path_change;
}

void read_config()
{
    int i = 0;

    DEBUG("####read_config####\n");
#ifdef NVRAM_
    /*#ifndef MUTI_DIR
    if(convert_nvram_to_file(CONFIG_PATH) == -1)
    {
        printf("convert_nvram_to_file fail\n");
        return;
    }

#else*/
    if(convert_nvram_to_file_mutidir(CONFIG_PATH,&asus_cfg) == -1)
    {
        printf("convert_nvram_to_file fail\n");
        return;
    }
#else
    if(create_webdav_conf_file(&asus_cfg) == -1)
    {
        printf("create_webdav_conf_file fail\n");
        return;
    }
#endif
    if(asus_cfg.dir_number == 0)
    {
        no_config = 1;
        return;
    }

    parse_config_mutidir(CONFIG_PATH,&asus_cfg);
    while(i < asus_cfg.dir_number)
    {
        if( strlen(asus_cfg.prule[i]->rooturl) == 0 )
        {
            //printf("username is blank ,please input your username and passwrod\n");
            DEBUG("Read config error!\n");
            //write_log(S_ERROR,"Read config error!","",i);
            no_config = 1;
            break;
        }
        else
        {
            no_config = 0;
        }
        i++;
    }


    while(no_config == 1 && exit_loop != 1 )
    {
        //sleep(1);
        usleep(1000*100);
    }

    if(!no_config)
    {
        //no_config = 0 ;
        exit_loop = 0;
        if(get_tokenfile_info()==-1)
        {
            printf("\nget_tokenfile_info failed\n");
            exit(-1);
        }
        check_config_path(1);
    }

}

void send_to_inotify(){

    int i;

    for(i=0;i<asus_cfg.dir_number;i++)
    {
        DEBUG("send_action base_path = %s\n",asus_cfg.prule[i]->base_path);
        //write_debug_log(asus_cfg.prule[i]->base_path);
        //#ifndef NVRAM_
        if(g_pSyncList[i]->sync_disk_exist)
        {
            send_action(1,asus_cfg.prule[i]->base_path);
            usleep(1000*10);
        }

    }
}

void run(){
    int status;
    int i;
    int need_server_thid = 0;
    int create_thid1 = 0;
    int create_thid2 = 0;
    int create_thid3 = 0;

    init_global_var();
    for(i=0;i<asus_cfg.dir_number;i++)
    {
        //#ifndef NVRAM_
        if(g_pSyncList[i]->sync_disk_exist)
        {
            status = wd_create_session(i);
        }
    }

    handle_quit_upload();

    if(exit_loop == 0)
    {
#if 0
        LocalRootNode = create_tree_rootnode(base_path);
        FindDir(LocalRootNode,base_path);
        SearchTree(LocalRootNode);

        if(status != 0)
        {
            printf("get Local List ERROR! \n");
        }
#endif
        send_to_inotify();

        if(exit_loop == 0)
        {
            DEBUG("create newthid2\n");
            //write_debug_log("create newthid2");
            if( pthread_create(&newthid2,NULL,(void *)SyncLocal,NULL) != 0)
            {
                printf("thread creation failder\n");
                exit(1);
            }
            usleep(1000*500);
            create_thid2 = 1;
        }

        if(exit_loop == 0)
        {
            DEBUG("create newthid3\n");
            //write_debug_log("create newthid3");
            if( pthread_create(&newthid3,NULL,(void *)Socket_Parser,NULL) != 0)
            {
                printf("thread creation failder\n");
                exit(-1);
            }
            usleep(1000*500);
            create_thid3 = 1;
        }

        status = wd_initial();          //used for init
#if 0
        free_tree_node(LocalRootNode);
        LocalRootNode = NULL;
#endif
        //getLocalList();
#if 0
        LocalRootNode = create_tree_rootnode(base_path);
        FindDir(LocalRootNode,base_path);
        SearchTree(LocalRootNode);
#endif
        finished_initial = 1;


        need_server_thid = get_create_threads_state();

        if(need_server_thid && exit_loop == 0)
        {
            DEBUG("create newthid1\n");
            if( pthread_create(&newthid1,NULL,(void *)SyncServer,NULL) != 0)
            {
                printf("thread creation failder\n");
                exit(1);
            }
            create_thid1 = 1;
        }
        else
        {
            server_sync = 0;
        }

        if(create_thid1)
        {

            pthread_join(newthid1,NULL);
            DEBUG("newthid1 has stoped!\n");
        }

        //pthread_cond_destroy(&cond);
        if(create_thid3)
        {
            pthread_join(newthid3,NULL);
            DEBUG("newthid3 has stoped!\n");
        }

        if(create_thid2)
        {
            pthread_join(newthid2,NULL);
            DEBUG("newthid2 has stoped!\n");
        }

    }

    //sleep(2);
    usleep(1000);
    clean_up();

    if(stop_progress != 1)
    {
        printf("run again!\n");

        //#ifndef NVRAM_
        while(disk_change)
        {
            //write_debug_log("while disk_change");
            pthread_mutex_lock(&mutex_checkdisk);
            disk_change = 0;
            pthread_mutex_unlock(&mutex_checkdisk);
            sync_disk_removed = check_sync_disk_removed();

            if(sync_disk_removed == 2)
            {
                printf("sync path is change\n");
            }
            else if(sync_disk_removed == 1)
            {
                printf("sync disk is unmount\n");
            }
            else if(sync_disk_removed == 0)
            {
                printf("sync disk exists\n");
            }
        }
        //#endif

        exit_loop = 0;
        //read_config();
        run();
    }
}

int write_notify_file(char *path,int signal_num)
{
    FILE *fp;
    char fullname[64];
    memset(fullname,0,sizeof(fullname));

    //my_mkdir_r(path);
    my_mkdir("/tmp/notify");
    my_mkdir("/tmp/notify/usb");
    sprintf(fullname,"%s/webdav_client",path);
    fp = fopen(fullname,"w");
    if(NULL == fp)
    {
        printf("open notify %s file fail\n",fullname);
        return -1;
    }
    fprintf(fp,"%d",signal_num);
    fclose(fp);
    return 0;
}

int reget_usbinfo_config(){
    int i,j;
    int exist;
    char *disk_mount_path_tmp;

    if(asus_cfg.dir_number > asus_cfg_stop.dir_number)
    {
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg_stop.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[j]->rooturl,asus_cfg.prule[i]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                config_disk_tmp = config_disk_info_start->next;
                while(config_disk_tmp != NULL)
                {
                    if(!strcmp(config_disk_tmp->url,asus_cfg.prule[i]->rooturl))
                    {
                        free(config_disk_tmp->url);
                        config_disk_tmp->url = NULL;
                        break;
                    }
                    config_disk_tmp = config_disk_tmp->next;
                }
            }
        }
    }
    else
    {
        for(i=0;i<asus_cfg_stop.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[i]->rooturl,asus_cfg.prule[j]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                follow_disk_tmp = follow_disk_info_start->next;
                while(follow_disk_tmp != NULL)
                {
                    disk_mount_path_tmp = follow_disk_tmp->mountpath+4;
                    if( !strcmp(disk_mount_path_tmp,asus_cfg_stop.prule[i]->mount_path) )
                    {
                        if(initial_disk_data(&config_disk_tmp) == NULL)
                        {
                            return -1;
                        }
                        config_disk_tmp->partitionport = follow_disk_tmp->partitionport;

                        config_disk_tmp->vendor = my_str_malloc(strlen(follow_disk_tmp->vendor)+1);
                        sprintf(config_disk_tmp->vendor,"%s",follow_disk_tmp->vendor);

                        config_disk_tmp->product = my_str_malloc(strlen(follow_disk_tmp->product)+1);
                        sprintf(config_disk_tmp->product,"%s",follow_disk_tmp->product);

                        config_disk_tmp->serialnum = my_str_malloc(strlen(follow_disk_tmp->serialnum)+1);
                        sprintf(config_disk_tmp->serialnum,"%s",follow_disk_tmp->serialnum);

                        config_disk_tmp->url = my_str_malloc(strlen(asus_cfg_stop.prule[i]->rooturl)+1);
                        sprintf(config_disk_tmp->url,"%s",asus_cfg_stop.prule[i]->rooturl);

                        config_disk_info->next = config_disk_tmp;
                        config_disk_info = config_disk_tmp;
                        break;
                    }
                    follow_disk_tmp = follow_disk_tmp->next;
                }
            }
        }
    }
    return 0;
}

int rewrite_tokenfile_and_nv(){

    int i,j;
    int exist;
    //write_debug_log("rewrite_tokenfile_and_nv start");
    printf("rewrite_tokenfile_and_nv start\n");
    if(asus_cfg.dir_number > asus_cfg_stop.dir_number)
    {
        for(i=0;i<asus_cfg.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg_stop.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[j]->rooturl,asus_cfg.prule[i]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                //write_debug_log("del form nv");
                printf("del form nv\n");
                del_rule_num = i;
                char *new_nv;
                delete_tokenfile_info(asus_cfg.prule[i]->rooturl,asus_cfg.prule[i]->base_folder);
                new_nv = delete_nvram_contents(asus_cfg.prule[i]->rooturl,asus_cfg.prule[i]->base_folder);                
                printf("new_nv = %s\n",new_nv);
                remove(g_pSyncList[i]->conflict_file);
                write_to_tokenfile(asus_cfg.prule[i]->mount_path);
#ifdef NVRAM_
                write_to_nvram(new_nv,"wd_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif
                free(new_nv);
            }
        }
    }
    else
    {
        for(i=0;i<asus_cfg_stop.dir_number;i++)
        {
            exist = 0;
            for(j=0;j<asus_cfg.dir_number;j++)
            {
                if(!strcmp(asus_cfg_stop.prule[i]->rooturl,asus_cfg.prule[j]->rooturl))
                {
                    exist = 1;
                    break;
                }
            }
            if(!exist)
            {
                //write_debug_log("add to nv");
                //printf("add to nv\n");
                char *new_nv;
                add_tokenfile_info(asus_cfg_stop.prule[i]->rooturl,asus_cfg_stop.prule[i]->base_folder,asus_cfg_stop.prule[i]->mount_path);
                new_nv = add_nvram_contents(asus_cfg_stop.prule[i]->rooturl,asus_cfg_stop.prule[i]->base_folder);
                //printf("new_nv = %s\n",new_nv);
                write_to_tokenfile(asus_cfg_stop.prule[i]->mount_path);
#ifdef NVRAM_
                write_to_nvram(new_nv,"wd_tokenfile");
#else
                write_to_wd_tokenfile(new_nv);
#endif
                free(new_nv);
            }
        }
    }
    return 0;
}

/*int create_shell_file(){

    FILE *fp;
    char contents[256];
    memset(contents,0,256);
    strcpy(contents,"#! /bin/sh\nnvram set $2=\"$1\"\nnvram commit");

    if(( fp = fopen(SHELL_FILE,"w"))==NULL)
    {
        fprintf(stderr,"create shell file error");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);
    return 0;

}*/

#ifndef NVRAM_
/*int write_get_nvram_script(){

    FILE *fp;
    char contents[512];
    memset(contents,0,512);
    strcpy(contents,"#! /bin/sh\nNV=`nvram get cloud_sync`\nif [ ! -f \"/tmp/smartsync/webdav/config/webdav_tmpconfig\" ]; then\n   touch /tmp/smartsync/webdav/config/webdav_tmpconfig\nfi\necho \"$NV\" >/tmp/smartsync/webdav/config/webdav_tmpconfig");

    if(( fp = fopen(GET_NVRAM_SCRIPT_1,"w"))==NULL)
    {
        fprintf(stderr,"create webdav_get_nvram file error\n");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);

    return 0;
}*/
int write_get_nvram_script(char *nvram_name,char *nvram_path,char *script_path)
{
    FILE *fp;
    char contents[512];
    memset(contents,0,512);
    sprintf(contents,"#! /bin/sh\nNV=`nvram get %s`\nif [ ! -f \"%s\" ]; then\n   touch %s\nfi\necho \"$NV\" >%s",nvram_name,nvram_path,nvram_path,nvram_path);

    if(( fp = fopen(script_path,"w"))==NULL)
    {
        fprintf(stderr,"create webdav_get_nvram file error\n");
        return -1;
    }

    fprintf(fp,"%s",contents);
    fclose(fp);

    return 0;
}
#endif

int main(int argc, char *argv[]){
#if 0
    setenv("MALLOC_TRACE","memlog",1);
    mtrace();
#endif
    //create_start_file();

    exit_loop = 0;
    stop_progress = 0;

    sigset_t bset,oset;
    pthread_t sig_thread;

    sigemptyset(&bset);
    sigaddset(&bset,SIGUSR1);
    sigaddset(&bset,SIGUSR2);
    sigaddset(&bset,SIGTERM);

    if( pthread_sigmask(SIG_BLOCK,&bset,&oset) == -1)
        printf("!! Set pthread mask failed\n");

    if( pthread_create(&sig_thread,NULL,(void *)sigmgr_thread,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(-1);
    }


    clear_global_var();
    write_notify_file(NOTIFY_PATH,SIGUSR2);

#ifndef NVRAM_
    //strcpy(log_base_path,"/home/alan/tool");   //just for test for pc
    //strcpy(log_base_path,argv[1]);
    strcpy(log_base_path,"/tmp");                //for N16
#else
    strcpy(log_base_path,"/tmp");
#endif
    //rootfolder_len = strlen(ROOTFOLDER);
    sprintf(log_path,"%s/Cloud/log",log_base_path);
    my_mkdir("/tmp/Cloud");
    my_mkdir("/tmp/Cloud/log");
    //sprintf(temp_path,"%s/Cloud/temp",mount_path);
    sprintf(general_log,"%s/WebDAV",log_path);
    my_mkdir("/tmp/smartsync");
    my_mkdir("/tmp/smartsync/webdav");
    my_mkdir("/tmp/smartsync/.logs");
    my_mkdir("/tmp/smartsync/webdav/config");
#ifndef NVRAM_
    my_mkdir("/tmp/smartsync/webdav/script");
#ifndef PC
    my_mkdir("/opt/etc/.smartsync");
#endif
#endif
    //my_mkdir_r(temp_path);
    //my_mkdir_r(log_path);  //no mountpath
    //printf("log_path = %s\ntemp_path = %s\ngeneral_log = %s\n",log_path,temp_path,general_log);

    DEBUG("log_path = %s\ngeneral_log = %s\n",log_path,general_log);

    while(detect_process("nvram"))
    {
        sleep(2);
    }
//    create_shell_file();
#ifndef NVRAM_
#ifndef PC
    write_get_nvram_script("cloud_sync",NVRAM_PATH_1,GET_NVRAM_SCRIPT_1);
    write_get_nvram_script("link_internet",NVRAM_PATH_2,GET_NVRAM_SCRIPT_2);
    system(SH_GET_NVRAM_SCRIPT_1);
    sleep(2);
#endif
#endif

    //printf("mount_path = %s\n",mount_path);

    sleep(1);
    read_config();
    //printf("HOST = %s\n",HOST);

    if(no_config == 0)
        run();

    pthread_join(sig_thread,NULL);

    stop_process_clean_up();

    //remove("/tmp/notify/usb/webdav_client");
    //unlink("/tmp/smartsync_app/webdav_client_start");    //unlink in stop_process_clean_up()
//    if(detect_process_file())
//    {
//        DEBUG("webdav did not kill inotify\n");
//    }
//    else
//    {
//        system("killall  -9 inotify &");
//        DEBUG("webdav kill inotify\n");
//    }

/*
    if(!detect_process("asuswebstorage") && !detect_process("ftpclient") && !detect_process("dropbox_client"))
    {
        system("killall  -9 inotify &");
        DEBUG("webdav kill inotify\n");
    }

    else
    {
        DEBUG("webdav did not kill inotify\n");
    }
*/
    DEBUG("stop WebDAV end\n");
    return 1;
}

