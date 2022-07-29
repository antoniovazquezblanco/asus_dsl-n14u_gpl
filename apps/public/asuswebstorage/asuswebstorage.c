#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <ftw.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <setjmp.h>
#include <curl/curl.h>


//#define IPKG 0

//#ifdef DEBUG
//#include <mcheck.h>
//#endif

//#ifndef IPKG
//#include <bcmnvram.h>
//#include <shutils.h>
//#endif
#include <sys/time.h>
#include "api.h"
#include "function.h"
#include "data.h"
#include "list.h"

#define MAXDATASIZE 1024
#define MYPORT 3567
#define INOTIFY_PORT 3678
#define BACKLOG 100 /* max listen num*/
//#define CONFIG_PATH "/home/gauss/Cloud/Cloud.conf"

#define SEM 0

#define UP_FAIL   10
#define DOWN_FAIL 11
#define SOCKET_FAIL 12
#define RETRY_MAX_TIME 1



int stop_sync_server;
int pre_seq = -10;
Servicegateway sergate;
Aaa aaa;
//int sync_up;
//int sync_down;
int stop_up = 0;
int stop_down = 0 ;
int local_sync;
int server_sync;
int server_modify;
int MySyncFolder;
int MyRecycleID;
char username[256];
char password[256];
//char otp_key[8];
//char captcha[8];
char base_path[256];
sync_item_t from_server_sync_head = NULL;
sync_item_t down_head = NULL;
sync_item_t up_head = NULL;
sync_item_t up_excep_fail = NULL;
sync_item_t download_only_socket_head = NULL;
sync_item_t download_only_modify_head = NULL;
sync_item_t copy_file_list_head = NULL;
int exit_proc = 0;
//pthread_t newthid1,newthid2,newthid3;
pthread_t server_thread,local_thread,socket_thread,download_socket_thread;
int no_config;
int restart_run;
int stop_progress = 0;
int exit_loop = 0;
//int uploading = 0;
//int downloading = 0;
int retry_fail_time = 0;
my_mutex_t my_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
//my_mutex_t wait_sleep_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
my_mutex_t wait_server_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
my_mutex_t wait_socket_mutex = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
pthread_mutex_t mutex_socket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_receve_socket = PTHREAD_MUTEX_INITIALIZER;
int upload_only = 0;
int download_only = 0;
s_tree *s_link = NULL;
Hb_TreeNode *DirRootNode = NULL;
Server_TreeNode  *SRootNode = NULL;
//char uploadfile_path[NORMALSIZE];
//char downloadfile_path[NORMALSIZE];
int server_space_full = 0;
int local_space_full = 0 ;
//int file_iscopying = 0 ;
int has_del_fiel = 0;
int has_socket = 0;
int max_upload_filesize;
struct asus_config cfg;
int no_local_root = 0; /* handle rm sync root dir*/
int mysync;
int copying_file_number =0;
int has_local_socket = 0 ;
int is_server_running = 0;
int IsAccountFrozen = 0;
int IsSyncError = 0;
//int SocketNumber =1;
//int IsNetworkLinkDown;
// token file var
#ifdef IPKG
int disk_change;
int sync_disk_removed;
char disk_info_filename[128];
char record_token_file[256];
char token_filename[256];
#endif

void *SyncServer();
void *SyncLocal();
void sig_handler (int signum);
int cmd_parser(char *command);
//int download_only_cmd_parser(char *command,Socket_cmd *socket_cmd);
int send_action(int type,char *content);
void init_global_var();
void clean_global_var();
void init_sync_folder();
void clean_up();
void run();
int read_config();
void handle_fail_item();
void* sigmgr_thread();

//int update_mount_path_to_nvram();
char *get_mount_path(char *path, int n);
void create_parent_folder_r(char *path);
//int update_mount_path_to_nvram(char *new_sync_path);

#ifdef IPKG
int write_notify_file(char *path, int signal_num);
#endif

int main(int argc, char *argv[])
{
#ifdef DEBUG
    //setenv("MALLOC_TRACE","memlog",1);
    //mtrace();
#endif

//#ifdef IPKG
    //create_start_file();
//#endif

    curl_global_init(CURL_GLOBAL_DEFAULT);
#if 1
    sigset_t bset,oset;
    pthread_t sig_thread;

    //sigfillset(&bset);
    sigemptyset(&bset);
    sigaddset(&bset,SIGUSR1);
    sigaddset(&bset,SIGUSR2);
    sigaddset(&bset,SIGTERM);
    sigaddset(&bset,SIGPIPE);

    if( pthread_sigmask(SIG_BLOCK,&bset,&oset) == -1)
        printf("!! Set pthread mask failed\n");

    if( pthread_create(&sig_thread,NULL,(void *)sigmgr_thread,NULL) != 0)
    {
        printf("thread creation failder\n");
        exit(-1);
    }
#endif


    char error_message[NORMALSIZE];

    memset(error_message,0,sizeof(error_message));

    clean_global_var();

    strcpy(base_path,"/tmp");

    DIR *dp = opendir(base_path);

    if(NULL == dp)
    {
        printf("open %s dir fail,asuswebstorage exit\n",base_path);
        snprintf(error_message,NORMALSIZE,"open %s dir fail,asuswebstorage exit",base_path);
        handle_error(S_OPENDIR_FAIL,error_message);
        exit(-1);
    }

    closedir(dp);

    init_global_var();

    write_log(S_INITIAL,"","");

#ifdef IPKG
    //write_get_nvram_script();
    //system("sh /tmp/smartsync/asuswebstorage/script/asuswebstorage_get_nvram");
    //sleep(2);
    write_get_nvram_script("cloud_sync",NVRAM_PATH_1,GET_NVRAM_SCRIPT_1);
    write_get_nvram_script("link_internet",NVRAM_PATH_2,GET_NVRAM_SCRIPT_2);
    system(SH_GET_NVRAM_SCRIPT_1);
    enter_sleep_time(1000*2000,NULL);
//#else
    //create_shell_file();
#endif


    //signal(SIGUSR1,sig_handler); //add config
    //signal(SIGUSR2,sig_handler); //modify and del config
    //signal(SIGINT,sig_handler);
    //signal(SIGPIPE,sig_handler);
#ifdef IPKG
    write_notify_file(NOTIFY_PATH,SIGUSR2);
#endif

    read_config();

    if(no_config == 0 && !stop_progress)
        run();

    curl_global_cleanup();

#ifdef IPKG
    remove("/tmp/notify/usb/asuswebstorage");

    /*if(!detect_process_file())
    {
           system("killall  -9 inotify &");
           printf("asuswebstorage kill inotify\n");
    }*/

#endif

    printf("stop asuswebstorage end\n");

    return 0;
}

void *SyncServer()
{
    printf("sync server Thread start\n");

    pre_seq = -10 ;
    int cur_seq = 0;
    int sync_fail = 0;

    //while(sync_up == 1)
    while(!exit_loop)
    {
        //printf("Sync Server is running\n");
    	//printf("******local_sync = %d *******\n",local_sync);

        while(local_sync && !exit_loop)
            //usleep(1000*10);
            enter_sleep_time(1000*10,NULL);
        server_sync = 1;

        /*
        if(no_local_root == 1)
        {
            my_mkdir(sync_path);
            send_action(1,sync_path);
            if(upload_only)
                mysync = 1;
            else
                pre_seq = -10;
            no_local_root = 0;
            continue;
        }*/

        if(local_sync == 0 && SocketActionList->head == NULL)
        {
#ifdef IPKG
            if(disk_change)
            {
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                check_disk_change();
            }
#endif

            Changeseq *cs;
            int status;
            cs = getChangeSeq(MySyncFolder);
            if(NULL == cs)
            {
                //enter_sleep_time(2);
                //usleep(1000*100);
                enter_sleep_time(1000*100,NULL);
                check_network_state();
                printf("########server get seqnumber fail########\n");
                continue;
            }
            if(cs->status == 2)
            {
#if SYSTEM_LOG
               write_system_log("GetChageSeq","Auth Fail");
#endif
                my_free(cs);

                status = getToken(username,password,"",0);

                if(status != 0)
                {
                    if( status != S_NETWORK_FAIL)
                        exit_loop = 1;
                    continue;
                }

            }
            else if(cs->status == 0)
            {
                cur_seq = cs->changeseq;
                if(local_sync == 0)
                    handle_fail_item();

                if( cur_seq != pre_seq || sync_fail == 1 || pre_seq == -10)
                {
                    printf("pre seq_number is %d,cur seq number is %d\n",pre_seq,cur_seq);
#ifdef DEBUG
                    printf("#### sync server start#####\n");
#endif

#if SYSTEM_LOG
                    write_system_log("sync server","start");
#endif
                    is_server_running = 1;
                    status = mySync(username,MySyncFolder,sync_path,browse_folder_xml);
                    is_server_running = 0 ;
                    //if(uploading != 1 && server_space_full != 1)
                    //if( server_space_full != 1)
                    //printf("IsSyncError=%d\n",IsSyncError);
                    if(!local_space_full && !server_space_full&&!IsSyncError) //2013/07/22 fix can't always show local sapce is not enough bug
                        write_log(S_SYNC,"","");
                    else if(IsSyncError)
                    {
                        write_log(S_ERROR,"Local synchronization is not entirely successful,failure information,please refer to errlog","");
                        IsSyncError = 0 ;
                    }

#ifdef DEBUG
                    printf("#### sync server end#####\n");
#endif

#if SYSTEM_LOG
                    write_system_log("sync server","end");
#endif
                    if(status == -1)
                    {
                        sync_fail = 1;
                        //sleep(5);
                        //enter_sleep_time(5);
                        //usleep(1000*100);
                        enter_sleep_time(1000*100,NULL);
                    }
                    else
                    {
                        sync_fail = 0;
                    }
                }

                if(sync_fail != 1)
                    pre_seq = cur_seq;

                //printf("#### sync server end final#####\n");

            }
            else
            {
                handle_error(cs->status,"get seqnumber");
            }
            my_free(cs);

        }
        server_sync = 0;
        enter_sleep_time(5,&wait_server_mutex);
        /*
        struct timespec timeout;
        timeout.tv_sec = time(NULL) + 5;
        timeout.tv_nsec = 0;
        pthread_mutex_lock(&(wait_server_mutex.mutex));
        pthread_cond_timedwait(&(wait_server_mutex.cond),&(wait_server_mutex.mutex),&timeout);
        pthread_mutex_unlock(&(wait_server_mutex.mutex));
        */
    }

    printf("stop asuswebstorage server sync\n");

    stop_up = 1;
}

int download_only_add_socket_item(Socket_cmd *socket_cmd)
{

    //printf("download_only_add_socket_item receive socket : %s\n",buf);

    char fullname[512];
    char old_fullname[512];
    //Socket_cmd socket_cmd;
    char cmd_name[32];
    char action[32];

    memset(cmd_name,0,sizeof(cmd_name));
    memset(fullname,0,sizeof(fullname));
    memset(old_fullname,0,sizeof(old_fullname));
    //memset(&socket_cmd,0,sizeof(Socket_cmd));
    memset(action,0,sizeof(action));

    //download_only_cmd_parser(buf,&socket_cmd);

    strcpy(cmd_name,socket_cmd->cmd_name);

    if( !strncmp(cmd_name,"copyfile",strlen("copyfile")) )
    {
        //file_iscopying = 1;
        //copying_file_number++;
        sprintf(fullname,"%s/%s",socket_cmd->path,socket_cmd->filename);
        add_sync_item("copyfile",fullname,copy_file_list_head);
        return 0;
    }
    else if( !strncmp(cmd_name,"rename",strlen("rename")) )
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->newname);
        snprintf(old_fullname,512,"%s/%s",socket_cmd->path,socket_cmd->oldname);
    }
    else if( !strncmp(cmd_name,"move",strlen("move")) )
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->oldname);
        snprintf(old_fullname,512,"%s/%s",socket_cmd->oldpath,socket_cmd->oldname);
    }
    else
    {
        snprintf(fullname,512,"%s/%s",socket_cmd->path,socket_cmd->filename);
    }

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        strcpy(action,"createfile");
        //copying_file_number--;
        //file_iscopying = 0 ;
        struct sync_item* item;

        item = get_sync_item("copyfile",fullname,copy_file_list_head);

        if(item != NULL)
        {
            del_sync_item("copyfile",fullname,copy_file_list_head);
        }
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        strcpy(action,"remove");
        del_download_only_sync_item(action,fullname,download_only_socket_head);
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        strcpy(action,"createfolder");
    }
    else if( strcmp(cmd_name, "rename") == 0 )
    {
        strcpy(action,"rename");
        del_download_only_sync_item(action,old_fullname,download_only_socket_head);
    }
    else if( strcmp(cmd_name, "move") == 0 )
    {
    	strcpy(action,"move");
    	del_download_only_sync_item(action,old_fullname,download_only_socket_head);
    }

    if(from_server_sync_head->next != NULL)
    {
        //printf("download_only_socket_head fu");
        sync_item_t item = get_sync_item(action,fullname,from_server_sync_head);

        if(item != NULL)
        {
            //printf("##### %s %s by asuswebstorage self ######\n",cmd_name,fullname);
            del_sync_item(action,fullname,from_server_sync_head);
            return 0;
        }

    }

    if(strcmp(cmd_name, "rename") == 0 || strcmp(cmd_name, "move") == 0)
    {
        if(test_if_dir(fullname))
        {
            add_all_download_only_socket_list(cmd_name,fullname);
        }
        else
        {
            add_sync_item(cmd_name,fullname,download_only_socket_head);
        }
    }
    else if(strcmp(cmd_name, "createfolder") == 0 || strcmp(cmd_name, "dragfolder") == 0)
    {
        add_sync_item(cmd_name,fullname,download_only_socket_head);
        if(!strcmp(cmd_name, "dragfolder"))
            add_all_download_only_dragfolder_socket_list(fullname);
    }
    else if( strcmp(cmd_name, "createfile") == 0  || strcmp(cmd_name, "dragfile") == 0 || strcmp(cmd_name, "modify") == 0)
    {
        add_sync_item(cmd_name,fullname,download_only_socket_head);
    }

    return 0;
}

int add_socket_item(char *buf)
{
    //printf("add socket[%d]=%s\n",SocketNumber,buf);
    //SocketNumber++;

    SocketActionTmp = malloc (sizeof (struct queue_entry));
    memset(SocketActionTmp,0,sizeof(struct queue_entry));
    snprintf(SocketActionTmp->cmd_name,1024,"%s",buf);

    pthread_mutex_lock(&mutex_socket);
    queue_enqueue(SocketActionTmp,SocketActionList);
    pthread_mutex_unlock(&mutex_socket);

    //printf("add socket end\n");

    return 0;
}

void *SyncLocal()
{
    printf("sync local Thread start\n");

    int sockfd;
    int new_fd;
    int numbytes;
    char buf[MAXDATASIZE];
    int yes = 1;
    //int retry_time = 0;

    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    int sin_size;
    //int i;

    fd_set read_fds;
    fd_set master;
    int fdmax;
    struct timeval timeout;
    int ret;

    FD_ZERO(&read_fds);
    FD_ZERO(&master);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return;
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error lol!");
        return;
    }

    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), sizeof(my_addr.sin_zero)); /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct
                                                         sockaddr))== -1) {
        perror("bind");
        return;
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        return;
    }
    sin_size = sizeof(struct sockaddr_in);    //add by alan

    FD_SET(sockfd,&master);
    fdmax = sockfd;

    //while(sync_down == 1) { /* main accept() loop */
    while(!exit_loop)
    { /* main accept() loop */

        //printf("sync local Thread loop start \n");

        timeout.tv_sec = 0;
        timeout.tv_usec = 1000*100;

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
            //printf("server: got connection from %s\n", \
            //inet_ntoa(their_addr.sin_addr));

            memset(buf, 0, sizeof(buf));

            if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
                perror("recv");
                //exit(1);
                continue;
            }
            //else
            //printf("recv command is %s \n",buf);

            if(buf[strlen(buf)] == '\n')
            {
                buf[strlen(buf)] = '\0';
            }

            close(new_fd);  //add by alan

            pthread_mutex_lock(&mutex_receve_socket);
            receve_socket = 1;
            pthread_mutex_unlock(&mutex_receve_socket);

            //pthread_mutex_lock(&mutex_socket);
            //local_sync = 1;
            add_socket_item(buf);
            //pthread_mutex_unlock(&mutex_socket);

        }
    }

    printf("stop asuswebstorage local sync\n");

    close(sockfd);

    stop_down = 1;

}

void *Socket_Parser(){
    printf("*******Socket_Parser start********\n");
    queue_entry_t socket_execute;
    int status = 0;
    mysync = 1;


    //while(sync_down == 1)
    while(!exit_loop)
    {
        //local_sync = 1;
        //printf("*******Socket_Parser start loop********\n");
        while(server_sync && !exit_loop)
            //usleep(1000*100);
            enter_sleep_time(1000*100,NULL);
        local_sync = 1;


        if(no_local_root == 1)
        {
            my_mkdir(sync_path);
            send_action(1,sync_path);
            if(upload_only)
                mysync = 1;
            else
                pre_seq = -10;
            no_local_root = 0;
            continue;
        }

#ifdef IPKG
        if(disk_change)
        {
            pthread_mutex_lock(&mutex_socket);
            disk_change = 0;
            pthread_mutex_unlock(&mutex_socket);
            check_disk_change();
        }
#endif
        if(upload_only == 1 && mysync == 1)
        {
            //do {
                printf("upload only mysync start\n");
                status = mySync(username,MySyncFolder,sync_path,browse_folder_xml);
                printf("upload only mysync end,status is %d\n",status);
                write_log(S_SYNC,"","");
                if(status == 0)
                    mysync = 0;
                //if(status == -1)
                    //sleep(10);
                    //enter_sleep_time(10);
                    //usleep(1000*100);
                //uploadonly_init_retry_times++;
           // }while(status == -1);

        }

        while(!exit_loop) //if file is copying ,don't exit local sync
        {
            if(upload_only)
            {
                handle_fail_item();
            }

            //printf("######## upload onlyl socket loop########\n");

            //while(SocketActionList->head != NULL && sync_down == 1)
            while(SocketActionList->head != NULL && !exit_loop)
            {
                has_socket = 1;
                socket_execute = SocketActionList->head;
                //status = cmd_parser(socket_execute->cmd_name);
                //if(status == 0)
                if(cmd_parser(socket_execute->cmd_name) == 0)
                {
                    //printf("########will del socket item##########\n");
                    pthread_mutex_lock(&mutex_socket);
                    socket_execute = queue_dequeue(SocketActionList);
                    my_free(socket_execute);
                    //printf("del socket item ok\n");
                    pthread_mutex_unlock(&mutex_socket);
                }
                else
                   printf("######## socket item fail########\n");
                //sleep(2);
                //enter_sleep_time(2);
                //usleep(1000*100);
                enter_sleep_time(1000*100,NULL);
            }

            //if(file_iscopying != 1)
            //if(copying_file_number <= 0 || exit_loop)
            if(copy_file_list_head->next == NULL || exit_loop)
            {
#ifdef DEBUG
                //printf("copy_file_list is blank\n");
#endif
                break;
            }
            //sleep(1);
            //enter_sleep_time(1);
            //usleep(1000*100);
            enter_sleep_time(1000*100,NULL);
        }



        if(has_socket == 1)
        {
            //if(downloading != 1 && !local_space_full && !server_space_full )
            //printf("IsSyncError=%d\n",IsSyncError);
            if(!local_space_full && !server_space_full && !IsSyncError) //2013/07/22 fix can't always show local sapce is not enough bug
                write_log(S_SYNC,"","");
            else if(IsSyncError)
            {
                write_log(S_ERROR,"Local synchronization is not entirely successful,failure information,please refer to errlog","");
                IsSyncError = 0 ;
            }
            has_socket = 0;
        }


       if(!is_server_running)
           has_local_socket = 0;

        //pthread_mutex_lock(&mutex_socket);
        //local_sync = 0;
        //pthread_mutex_unlock(&mutex_socket);

        //printf("*******Socket_Parser start loop end********\n");
        //sleep(5);

        //enter_sleep_time(5);
        pthread_mutex_lock(&mutex_receve_socket);
        receve_socket = 0;
        pthread_mutex_unlock(&mutex_receve_socket);

        local_sync = 0;

        enter_sleep_time(5,&wait_socket_mutex);
        /*
        struct timespec timeout;
        //int seconds
        timeout.tv_sec = time(NULL) + 5;
        timeout.tv_nsec = 0;
        pthread_mutex_lock(&(wait_socket_mutex.mutex));
        pthread_cond_timedwait(&(wait_socket_mutex.cond),&(wait_socket_mutex.mutex),&timeout);
        pthread_mutex_unlock(&(wait_socket_mutex.mutex));
        */

        ///receve_socket = 0;


    }


    printf("stop asuswebstorage Socket_Parser\n");
    stop_down = 1;
}

#if 0
void *Download_Only_Socket_Parser(){
    printf("*******Download_Only_Socket_Parser start********\n");
    queue_entry_t socket_execute;
    int status = 0;
    int mysync = 1;
    int has_socket = 0;

    while(sync_up == 1)
    {
       while(server_sync && !exit_loop)
            //usleep(1000*100);
           enter_sleep_time(1000*100,NULL);
        local_sync = 1;
        //printf("*******Socket_Parser start loop********\n");

        while(1) //don't file when it is copying
        {
            while(SocketActionList->head != NULL && sync_up == 1)
            {
                has_socket = 1;
                socket_execute = SocketActionList->head;
                if(download_only_add_socket_item(socket_execute->cmd_name) == 0)
                {
                    //printf("########will del socket item##########\n");
                    pthread_mutex_lock(&mutex_socket);
                    socket_execute = queue_dequeue(SocketActionList);
                    my_free(socket_execute);
                    //printf("del socket item ok\n");
                    pthread_mutex_unlock(&mutex_socket);
                }
                else
                    printf("########Download_Only_Socket_Parser socket item fail########\n");
                //sleep(1);
                //enter_sleep_time(1);
                //usleep(1000*100);
                enter_sleep_time(1000*100,NULL);
            }

            if(file_iscopying != 1)
                break;

            //sleep(1);
            //enter_sleep_time(1);
            //usleep(1000*100);
            enter_sleep_time(1000*100,NULL);
        }


        //receve_socket = 0;
        pthread_mutex_lock(&mutex_receve_socket);
        receve_socket = 0;
        pthread_mutex_unlock(&mutex_receve_socket);

        //printf("has_socket=%d\n",has_socket);
        if(has_socket == 1)
        {
            write_log(S_SYNC,"","");
            has_socket = 0;
        }

        local_sync = 0;
        //printf("*******Socket_Parser start loop end********\n");

        //enter_sleep_time(5);
        enter_sleep_time(5,NULL);

    }


    printf("stop asuswebstorage Socket_Parser\n");
    stop_down = 1;
}
#endif

/*
void send_broadcast()
{
    int i;
    for(i = 0; i<10; i++)
    {
        pthread_cond_broadcast(&wait_sleep_mutex.cond);
        usleep(100*1000);
    }
}*/

void sig_handler (int signum)
{
    //Getmysyncfolder *gf;

    switch (signum)
    {
    case SIGTERM:
        printf("signal is SIGTERM\n");
        //sync_up = 0;
        //sync_down = 0;
        stop_progress = 1;
        exit_loop = 1;


        pthread_cond_signal(&(wait_server_mutex.cond));
        pthread_cond_signal(&(wait_socket_mutex.cond));
        pthread_cond_signal(&(my_mutex.cond));
#ifdef IPKG
        sync_disk_removed = 0;
        //check_accout_status();
#endif

        //send_broadcast();

        break;
    case SIGUSR1:  // add user
        printf("signal is SIGUSER1\n");
        exit_loop = 1;
        read_config();
        break;
    case SIGUSR2:  // modify user
        printf("signal is SIGUSR2\n");
#ifdef IPKG
        pthread_mutex_lock(&mutex_socket);
        disk_change = 1;
        pthread_mutex_unlock(&mutex_socket);
#endif
        break;
#if 1
    case SIGPIPE:  // delete user
        printf("signal is SIGPIPE\n");
        signal(SIGPIPE, SIG_IGN);
        break;
#endif
    default:
        printf("signal is %d\n",signum);
        signal(signum,SIG_IGN);
        break;

    }
}

unsigned long stat_file(char *filename)
{
    unsigned long size;
    struct stat filestat;
    if( stat(filename,&filestat) == -1)
    {
        perror("stat:");
        printf("stat file stat error:%s file not exist\n",filename);
        return 0;
        //exit(1);
    }
    return  filestat.st_size;

    return size;

}


#if 0
int split_socket_cmd(char *cmd,Socket_cmd *socket_cmd)
{
    //printf("download_only_cmd_parser start\n");
    char temp[512];
    char *ch;
    char split = '\n';

    printf("socet command is %s \n",cmd);

    //if( strstr(cmd,"(conflict)") != NULL )
        //return 0;

    if(!strncmp(cmd,"rmroot",6))
    {
        no_local_root = 1;
        return 0;
    }

    if(strncmp(cmd,"exit",4) == 0)
    {
        printf("exit socket\n");
        return -1;
    }

    if(strstr(cmd,".asus.td"))
    {
        printf("ignore download temp file\n");
        return -1;
    }

    if(!strchr(cmd,split))
    {
        printf("socket cmd is not math cmd\\npath\\nname format\n");
        return -1;
    }



    memset(temp,0,sizeof(temp));

    ch = cmd;
    int i = 0;
    while(*ch != split)    /* change split '@' to '\n' */
    {
        i++;
        ch++;
    }

    memcpy(socket_cmd->cmd_name, cmd, i);

   printf("socket_cmd->cmd_name=%s\n",socket_cmd->cmd_name);

    char *p = NULL;
    ch++;
    i++;

    strcpy(temp,ch);
    p = strchr(temp,split);

    if(p!=NULL)
        strncpy(socket_cmd->path,temp,strlen(temp)- strlen(p));

    p++;
    if(strcmp(socket_cmd->cmd_name, "rename") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(socket_cmd->oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(socket_cmd->newname,p1);
        //printf("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);
    }
    else if(strcmp(socket_cmd->cmd_name, "move") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(socket_cmd->oldpath,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(socket_cmd->oldname,p1);

        //printf("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);
    }
    else
    {
        strcpy(socket_cmd->filename,p);
        //printf("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);
    }

    return 0;

    //printf("download_only_cmd_parser end\n");
}
#endif

#if 1
int split_socket_cmd(char *cmd_in,Socket_cmd *scmd)
{
#ifdef DEBUG
    printf("socket cmd is %s \n",cmd_in);
#endif

    char cmd[1024];
    char *p;
    const char *split = "\n";
    int i=0;


    memset(cmd,0,sizeof(cmd));

    strcpy(cmd,cmd_in);

    if(!strncmp(cmd,"rmroot",6))
    {
        no_local_root = 1;
        return 0;
    }

    if(strncmp(cmd,"exit",4) == 0)
    {
        printf("exit socket\n");
        return -1;
    }

    if(strstr(cmd,".asus.td"))
    {
        printf("ignore download temp file\n");
        return -1;
    }

    if(!strchr(cmd,'\n'))
    {
        printf("socket cmd is not math cmd\\npath\\nname format\n");
        return -1;
    }


    p=strtok(cmd,split);

    while(p!=NULL)
    {
        switch (i)
        {
        case 0 :
            if(!strncmp(p,"rename",6) || !strncmp(p,"move",4))
                strncpy(scmd->cmd_name,p,strlen(p)-1);
            else
                strcpy(scmd->cmd_name,p);
            break;
        case 1:
            //printf("path len =%d\n",strlen(p));
            strcpy(scmd->path,p);
            break;
        case 2:
            if(!strcmp(scmd->cmd_name,"rename"))
                strcpy(scmd->oldname,p);
            else if(!strcmp(scmd->cmd_name,"move"))
                strcpy(scmd->oldpath,p);
            else
                strcpy(scmd->filename,p);
            break;
        case 3:
            if(!strcmp(scmd->cmd_name,"rename"))
                strcpy(scmd->newname,p);
            else if(!strcmp(scmd->cmd_name,"move"))
                strcpy(scmd->oldname,p);
            break;
        default:
            break;
        }
        i++;
        p=strtok(NULL,split);
    }


    if(scmd->cmd_name[0] == '\0' || scmd->path[0] == '\0')
    {
        printf("socket cmd is not math cmd\\npath\\nname format\n");
        return -1;
    }

    //printf("socket splite end\n");

    return 0;
}
#endif

int perform_socket_cmd(Socket_cmd *scmd)
{

    /*
    if(strncmp(cmd,"exit",4) == 0)
    {
        printf("exit socket\n");
        return 0;
    }

    if(strstr(cmd,".asus.td"))
    {
        printf("ignore download temp file\n");
        return 0;
    }

    printf("socet command is %s \n",cmd);
    */

    //local_sync = 1;

    char cmd_name[64];
    char cmd_param[512];
    char path[512];
    char temp[512];
    char filename[NORMALSIZE];
    char fullname[NORMALSIZE];
    char oldname[NORMALSIZE],newname[NORMALSIZE],oldpath[NORMALSIZE];
    char action[64];
    char cmp_name[512];
    //char *ch;
    int parent_ID = -10;
    int entry_ID;
    //int browse_ID;
    Propfind *find = NULL;
    Createfolder *createfolder = NULL;
    Browse *bs = NULL;
    Operateentry *oe = NULL;
    Moveentry *me = NULL;
    struct sync_item *item = NULL;
    int status = -10;
    int res_value = -10;
    int isdelete = 0 ;
    //char  split = '\n';
    //Asusconfig asusconfig;

    memset(cmd_name, 0, sizeof(cmd_name));
    memset(cmd_param, 0, sizeof(cmd_param));
    memset(path,0,sizeof(path));
    memset(temp,0,sizeof(temp));
    memset(oldname,0,sizeof(oldname));
    memset(newname,0,sizeof(newname));
    memset(oldpath,0,sizeof(oldpath));
    memset(fullname,0,sizeof(fullname));
    memset(action,0,sizeof(action));
    memset(cmp_name,0,sizeof(cmd_name));

    /*
    ch = cmd;
    int i = 0;
    while(*ch != split) // change split '@' to '\n'
    {
        i++;
        ch++;
    }

    memcpy(cmd_name, cmd, i);

    char *p = NULL;
    ch++;
    i++;

    strcpy(temp,ch);
    p = strchr(temp,split);

    if(p!=NULL)
        strncpy(path,temp,strlen(temp)- strlen(p));

    p++;
    if(strcmp(cmd_name, "rename") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(oldname,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(newname,p1);
        //printf("cmd_name: [%s],path: [%s],oldname: [%s],newname: [%s]\n",cmd_name,path,oldname,newname);
    }
    else if(strcmp(cmd_name, "move") == 0)
    {
        char *p1 = NULL;

        p1 = strchr(p,split);

        if(p1 != NULL)
            strncpy(oldpath,p,strlen(p)- strlen(p1));

        p1++;

        strcpy(oldname,p1);

        //printf("cmd_name: [%s],path: [%s],oldpath: [%s],oldname: [%s]\n",cmd_name,path,oldpath,oldname);
    }
    else
    {
        strcpy(filename,p);
        //printf("cmd_name: [%s],path: [%s],filename: [%s]\n",cmd_name,path,filename);
    }
    */

    //printf("11\n");
    strcpy(cmd_name,scmd->cmd_name);
    strcpy(path,scmd->path);

    if(!strcmp(cmd_name,"rename"))
    {
        strcpy(oldname,scmd->oldname);
        strcpy(newname,scmd->newname);
    }
    else if(!strcmp(cmd_name,"move"))
    {
        strcpy(oldpath,scmd->oldpath);
        strcpy(oldname,scmd->oldname);
    }
    else
    {
        strcpy(filename,scmd->filename);
    }
    //printf("22\n");

    //add by alan
    if( strcmp(cmd_name, "copyfile") == 0 )
    {
        //file_iscopying = 1;
        //copying_file_number++;
        //  2013/7/25 change copying way
        sprintf(fullname,"%s/%s",path,filename);
        add_sync_item("copyfile",fullname,copy_file_list_head);

        return 0;
    }
    //else if( strcmp(cmd_name, "createfile") == 0)

#if 1

    if( !strcmp(cmd_name,"rename") )
        snprintf(cmp_name,512,"%s/%s",path,newname);
    else
        snprintf(cmp_name,512,"%s/%s",path,filename);

    if( strcmp(cmd_name, "createfile") == 0 )
    {
        strcpy(action,"createfile");
        //file_iscopying = 0;
        //copying_file_number--;
        struct sync_item* item;

        item = get_sync_item("copyfile",cmp_name,copy_file_list_head);

        if(item != NULL)
        {
            del_sync_item("copyfile",cmp_name,copy_file_list_head);
        }
    }
    else if( strcmp(cmd_name, "remove") == 0  || strcmp(cmd_name, "delete") == 0)
    {
        strcpy(action,"remove");
        isdelete = 1;
    }
    else if( strcmp(cmd_name, "createfolder") == 0 )
    {
        strcpy(action,"createfolder");
    }
    else if( strcmp(cmd_name, "rename") == 0 )
    {
        strcpy(action,"rename");
    }


    //printf("cmd_name is %s,cmp_name is %s\n",cmd_name,cmp_name);
    //printf("33\n");
    struct timespec timeout;
    char *up_action = NULL;
    timeout.tv_sec = time(NULL) + 3;
    timeout.tv_nsec = 0;

    if(from_server_sync_head->next != NULL)
    {
    	pthread_mutex_lock(&my_mutex.mutex);

        item = get_sync_item(action,cmp_name,from_server_sync_head);

        //pthread_mutex_unlock(&mutex);
        if(item != NULL)
        {
#ifdef DEBUG
            printf("##### %s %s by asuswebstorage self ######\n",action,cmp_name);
#endif
            //pthread_mutex_lock(&mutex);
            del_sync_item(action,cmp_name,from_server_sync_head);
            pthread_mutex_unlock(&my_mutex.mutex);

            //pthread_mutex_unlock(&mutex);
            //local_sync = 0;
            return 0;
        }

        pthread_mutex_unlock(&my_mutex.mutex);
        item = NULL;
        //}
    }
    //printf("44\n");
    has_local_socket = 1;

    /* first make sure that the token is expired */
    Getmysyncfolder *gf;
    gf = getMySyncFolder(username);

    if(NULL == gf)
    {
        check_network_state();
        return -1;        
    }
    //printf("55\n");
    if(gf->status == S_AUTH_FAIL)
    {
        status = getToken(username,password,"",0);

        if(status != 0)
        {
            if( status != S_NETWORK_FAIL)
                exit_loop = 1;
            return -1;
        }
    }
    //printf("66\n");
    parent_ID = getParentID(path);

    if(parent_ID == -1)
    {
        return -1;
    }
    //printf("77\n");
    if(parent_ID == -2) //parent dir has del from server
    {
        if(isdelete)
            return 0;

        parent_ID = create_server_folder_r(path);
        if(parent_ID == -1)
            return -1;

    }
    //printf("88\n");
#ifdef DEBUG
    printf("###### %s is start ######\n",cmd_name);
    //write_system_log(cmd_name,"start");
#endif

    if( strcmp(cmd_name, "createfile") == 0 || strcmp(cmd_name, "dragfile") == 0 )
    {
        snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
#if TEST
        sleep(30);
        printf("real upload start\n");
#endif
        status = uploadFile(fullname,parent_ID,NULL);

#if TREE_NODE_ENABLE
        modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

        if(status != 0)
        {
#ifdef DEBUG
            printf("upload %s failed\n",fullname);
            //write_system_log("error","uploadfile fail");
#endif
            res_value = handle_upload_fail_code(status,parent_ID,fullname,path);

            return res_value;
        }
    }
    else if( strcmp(cmd_name, "delete") == 0  || strcmp(cmd_name, "remove") == 0 || strcmp(cmd_name, "modify") == 0 ||
             strcmp(cmd_name, "rename") == 0 )
    {
        int isfolder;
        int ischildonly;

        if( strcmp(cmd_name, "rename") == 0 )
            find = checkEntryExisted(username,parent_ID,oldname,"system.unknown");
        else
            find = checkEntryExisted(username,parent_ID,filename,"system.unknown");

        if(NULL == find)
        {
            printf("find prop failed\n");
            return -1;
        }

        if( find->status != 0 )
        {
            handle_error(find->status,"propfind");
            my_free(find);
            return -1;
        }

        entry_ID = find->id;

        //printf("entry id is %d\n",entry_ID);

        if(strcmp(find->type,"system.folder") == 0)
        {
            isfolder = 1;
            ischildonly = 0;
        }
        else if(strcmp(find->type,"system.file") == 0)
        {
            isfolder = 0;
            ischildonly = 0;
        }

        if( strcmp(cmd_name, "rename") == 0 )
        {
            if(strcmp(find->type,"system.notfound") == 0)
            {
                snprintf(fullname,NORMALSIZE,"%s/%s",path,newname);
                res_value = upload_entry(fullname,parent_ID,path);
                return res_value;
            }
            else
            {
#if TEST
                printf("renameEntry() start\n");
                printf("wait 40 start\n");
                sleep(30);
                printf("rename start\n");
#endif
                oe = renameEntry(username,entry_ID,0,newname,isfolder) ;
                //printf("renameEntry() end\n");

#if TREE_NODE_ENABLE
                rename_update_tree(oldname,newname);
#endif

                my_free(find);

                if(NULL == oe)
                {
                    printf("operate rename failed\n");
                    return -1;
                }

                if( oe->status != 0 )
                {
                    handle_error(oe->status,cmd_name);
                    snprintf(fullname,NORMALSIZE,"%s/%s",path,newname);
                    res_value = handle_rename_fail_code(oe->status,parent_ID,fullname,path,isfolder);
                    my_free(oe);
                    return res_value;
                }

                my_free(oe);

            }

        }
        else if( strcmp(cmd_name, "modify") == 0 )
        {

            if(!strcmp(find->type,"system.notfound"))
            {
                printf("del item has not exist server\n");
                //entry_ID = 0 ;
            }

            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            status = uploadFile(fullname,parent_ID,NULL);

#if TREE_NODE_ENABLE
            modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

            if(status != 0)
            {
                printf("upload %s failed\n",fullname);
                res_value = handle_upload_fail_code(status,parent_ID,fullname,path);
                return res_value;
            }
        }
        else
        {
#if TEST
            printf("wait 40 start\n");
            sleep(30);
            printf("del start\n");
#endif
            if(!strcmp(find->type,"system.notfound"))
            {
                printf("del item has not exist server\n");
                return 0;
            }

            oe = removeEntry(username,entry_ID,ischildonly,isfolder);

#if TREE_NODE_ENABLE
            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            modify_tree_node(fullname,DirRootNode,DEL_TREE_NODE);
#endif
            my_free(find);

            if(NULL == oe)
            {
                printf("operate rename failed\n");
                return -1;
            }

            //printf("remove status is %d \n",oe->status);

            if( oe->status != 0 )
            {
                handle_error(oe->status,cmd_name);
                res_value = handle_delete_fail_code(oe->status);
                my_free(oe);
                return res_value;
            }
            my_free(oe);
        }

        /*if( strcmp(cmd_name, "modify") == 0 )
        {

            snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);
            status = uploadFile(fullname,parent_ID,NULL);

#if TREE_NODE_ENABLE
            modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

            if(status != 0)
            {
                printf("upload %s failed\n",fullname);
                res_value = handle_upload_fail_code(status,parent_ID,fullname,path);
                return res_value;
            }
        }*/
    }
    else if(strcmp(cmd_name, "dragfolder") == 0 || strcmp(cmd_name, "createfolder") == 0 )
    {
        snprintf(fullname,NORMALSIZE,"%s/%s",path,filename);

#if TREE_NODE_ENABLE
        modify_tree_node(fullname,DirRootNode,ADD_TREE_NODE);
#endif

#if TEST
        printf("wait 40 start\n");
        sleep(30);
        printf("create folder start\n");
#endif
        createfolder = createFolder(username,parent_ID,0,fullname);


        if(NULL == createfolder)
            return -1;
        else if( createfolder->status != 0 )
        {
            handle_error(createfolder->status,"createfolder");
            res_value = handle_createfolder_fail_code(createfolder->status,parent_ID,path,fullname);
            my_free(createfolder);
            return res_value;
        }
        else
        {
            entry_ID = createfolder->id;
            my_free(createfolder);
            //printf("entry ID is %d\n",entry_ID);
            if(!strcmp(cmd_name,"dragfolder"))
            {
                res_value = sync_all_item(fullname,entry_ID);
                if(res_value = S_UPLOAD_DELETED)
                    return 0;
                else
                    return res_value;
            }

        }
    }
    else if( strcmp(cmd_name, "move") == 0 )
    {
#if TREE_NODE_ENABLE
        char del_name[NORMALSIZE];
        char insert_name[NORMALSIZE];

        memset(del_name,0,sizeof(del_name));
        memset(insert_name,0,sizeof(insert_name));

        snprintf(del_name,NORMALSIZE,"%s/%s",oldpath,oldname);
        snprintf(insert_name,NORMALSIZE,"%s/%s",path,oldname);

        modify_tree_node(del_name,DirRootNode,TREE_DEL_NODE);
        modify_tree_node(insert_name,DirRootNode,ADD_TREE_NODE);

#endif
        int pre_parent_ID;
        int isfolder;

        pre_parent_ID = getParentID(oldpath);
        if(pre_parent_ID == -1)
            return -1;
        //printf("pre parent id is %d\n",pre_parent_ID);

        find = checkEntryExisted(username,pre_parent_ID,oldname,"system.unknown");

        if(NULL == find)
            return -1;

        if( find->status != 0 )
        {
            handle_error(find->status,"propfind");
            my_free(find);
            return -1;
        }
        else
        {
            if(!strcmp(find->type,"system.notfound"))
            {
                snprintf(fullname,NORMALSIZE,"%s/%s",path,oldname);
                res_value = upload_entry(fullname,parent_ID,path);
                return res_value;
            }
            else
            {
                entry_ID = find->id;
                //printf("entry id is %d\n",entry_ID);
                if(strcmp(find->type,"system.folder") == 0)
                {
                    isfolder = 1;
                }
                else if(strcmp(find->type,"system.file") == 0)
                {
                    isfolder = 0;
                }

                my_free(find);

#if TEST
                printf("wait 40 start\n");
                sleep(30);
                printf("move start\n");
#endif
                me = moveEntry(username,entry_ID,oldname,parent_ID,isfolder);

                if(NULL == me)
                {
                    printf("operate move failed\n");
                    return -1;
                }
                else if( me->status != 0 )
                {
                    handle_error(me->status,cmd_name);
                    snprintf(fullname,NORMALSIZE,"%s/%s",path,oldname);
                    res_value = handle_move_fail_code(me->status,path,fullname,parent_ID,oldpath,entry_ID,isfolder);
                    my_free(me);
                    return res_value;
                }

                my_free(me);
            }
        }
    }


    printf("###### %s is ending ######\n",cmd_name);
#if SYSTEM_LOG
    write_system_log(cmd_name,filename);
#endif
    //local_sync = 0;

    return 0;

#endif
}

int cmd_parser(char *command)
{
    Socket_cmd scmd;
    int res;

    memset(&scmd,0,sizeof(scmd));

    res = split_socket_cmd(command,&scmd);

    if(res == -1)
        return 0;

    if(download_only == 1 )
        res = download_only_add_socket_item(&scmd);
    else
        res = perform_socket_cmd(&scmd);

    return res;
}

int send_action(int type, char *content)
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    char str[1024] = {0};
    int port;

    if(type == 1)
        port = 3678;
    else if(type == 2)
        port = MYPORT;

    struct sockaddr_in their_addr; /* connector's address information */


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
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
        return -1;
    }

    //strcpy(str,content);
    sprintf(str,"0@%s",content);
    if (send(sockfd, str, strlen(str), 0) == -1) {
        perror("send");
        return -1;
    }
    else
    {
        printf("send content is %s\n",str);
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        return -1;
    }

    buf[numbytes] = '\0';
    close(sockfd);
    return 0;
}



void clean_global_var()
{
    memset(mount_path,0,sizeof(mount_path));
    memset(base_path, 0, sizeof(base_path));
    memset(cloud_path, 0, sizeof(cloud_path));
    //memset(temp_path, 0, sizeof(temp_path));
    memset(asus_path, 0, sizeof(asus_path));
    memset(log_path, 0, sizeof(log_path));
    memset(xml_path, 0, sizeof(xml_path));
    memset(system_log, 0, sizeof(system_log));
    memset(general_log,0,sizeof(general_log));
    memset(confilicted_log,0,sizeof(confilicted_log));
    memset(temp_file, 0, sizeof(temp_file));

    memset(gateway_xml, 0, sizeof(gateway_xml));
    memset(token_xml, 0, sizeof(token_xml));
    memset(get_info_xml, 0, sizeof(get_info_xml));
    memset(get_sync_folder_xml, 0, sizeof(get_sync_folder_xml));
    memset(get_personal_system_folder_xml,0,sizeof(get_personal_system_folder_xml));
    memset(browse_folder_xml, 0, sizeof(browse_folder_xml));
    memset(propfind_xml, 0, sizeof(propfind_xml));
    memset(create_folder_xml, 0, sizeof(create_folder_xml));
    memset(rename_xml, 0, sizeof(rename_xml));
    memset(move_xml, 0, sizeof(move_xml));
    memset(remove_xml, 0, sizeof(remove_xml));
    memset(update_xml, 0, sizeof(update_xml));
    memset(get_entry_info_xml, 0, sizeof(get_entry_info_xml));
    memset(set_mark_xml, 0, sizeof(set_mark_xml));
    memset(get_change_files_xml, 0, sizeof(get_change_files_xml));
    memset(get_uploads_xml, 0, sizeof(get_uploads_xml));
    memset(get_share_code_xml, 0, sizeof(get_share_code_xml));
    memset(del_share_code_xml, 0, sizeof(del_share_code_xml));
    memset(get_share_entry_xml, 0, sizeof(get_share_entry_xml));
    memset(check_pwd_xml, 0, sizeof(check_pwd_xml));
    memset(cmp_pwd_xml, 0, sizeof(cmp_pwd_xml));
    memset(get_change_seq_xml, 0, sizeof(get_change_seq_xml));
    memset(init_upload_xml, 0, sizeof(init_upload_xml));
    memset(resume_upload_xml, 0, sizeof(resume_upload_xml));
    memset(finish_upload_xml, 0, sizeof(finish_upload_xml));
    memset(get_resize_photo_xml, 0, sizeof(get_resize_photo_xml));
    memset(get_full_txt_xml, 0, sizeof(get_full_txt_xml));
    memset(get_video_snapshot_xml, 0, sizeof(get_video_snapshot_xml));
    memset(trans_excep_file,0,sizeof(trans_excep_file));
}


void init_global_var()
{
    char config_path[256] = {0};
    char script_path[256] = {0};
    char temp_path[256] = {0};
    snprintf(cloud_path,NAMESIZE,"%s/%s",base_path,"smartsync");
    snprintf(log_path,NAMESIZE,"%s/%s",cloud_path,".logs");
    //snprintf(temp_path,NAMESIZE,"%s/%s",cloud_path,"temp");
    snprintf(asus_path,NAMESIZE,"%s/%s",cloud_path,"asuswebstorage");
    snprintf(config_path,256,"%s/config",asus_path);
    snprintf(script_path,256,"%s/script",asus_path);
    snprintf(temp_path,256,"%s/temp",asus_path);
    snprintf(xml_path,NAMESIZE,"%s/xml",temp_path);


    snprintf(general_log,NAMESIZE,"%s/asuswebstorage",log_path);
    snprintf(system_log,NAMESIZE,"%s/system.log",log_path);
    snprintf(temp_file,NAMESIZE,"%s/temp",log_path);
    snprintf(gateway_xml,NAMESIZE,"%s/%s",xml_path,"gateway.xml");
    snprintf(get_user_state_xml,NAMESIZE,"%s/%s",xml_path,"get_user_state.xml");
    snprintf(token_xml,NAMESIZE,"%s/%s",xml_path,"token.xml");
    snprintf(get_info_xml,NAMESIZE,"%s/%s",xml_path,"get_info.xml");
    snprintf(get_sync_folder_xml,NAMESIZE,"%s/%s",xml_path,"get_sync_folder.xml");
    snprintf(get_personal_system_folder_xml,NAMESIZE,"%s/%s",xml_path,"get_personal_system_folder.xml");
    snprintf(browse_folder_xml,NAMESIZE,"%s/%s",xml_path,"browse_folder.xml");
    snprintf(propfind_xml,NAMESIZE,"%s/%s",xml_path,"propfind.xml");
    snprintf(create_folder_xml,NAMESIZE,"%s/%s",xml_path,"create_folder.xml");
    snprintf(rename_xml,NAMESIZE,"%s/%s",xml_path,"rename.xml");
    snprintf(move_xml,NAMESIZE,"%s/%s",xml_path,"move.xml");
    snprintf(remove_xml,NAMESIZE,"%s/%s",xml_path,"remove.xml");
    snprintf(update_xml,NAMESIZE,"%s/%s",xml_path,"update.xml");
    snprintf(get_entry_info_xml,NAMESIZE,"%s/%s",xml_path,"get_entry_info.xml");
    snprintf(set_mark_xml,NAMESIZE,"%s/%s",xml_path,"set_mark.xml");
    snprintf(get_change_files_xml,NAMESIZE,"%s/%s",xml_path,"get_change_files.xml");
    snprintf(get_uploads_xml,NAMESIZE,"%s/%s",xml_path,"get_uploads.xml");
    snprintf(get_share_code_xml,NAMESIZE,"%s/%s",xml_path,"get_share_code.xml");
    snprintf(del_share_code_xml,NAMESIZE,"%s/%s",xml_path,"del_share_code.xml");
    snprintf(get_share_entry_xml,NAMESIZE,"%s/%s",xml_path,"get_share_entry.xml");
    snprintf(check_pwd_xml,NAMESIZE,"%s/%s",xml_path,"check_pwd.xml");
    snprintf(cmp_pwd_xml,NAMESIZE,"%s/%s",xml_path,"cmp_pwd.xml");
    snprintf(get_change_seq_xml,NAMESIZE,"%s/%s",xml_path,"get_change_seq.xml");
    snprintf(init_upload_xml,NAMESIZE,"%s/%s",xml_path,"init_upload.xml");
    snprintf(resume_upload_xml,NAMESIZE,"%s/%s",xml_path,"resume_upload.xml");
    snprintf(finish_upload_xml,NAMESIZE,"%s/%s",xml_path,"finish_upload.xml");
    snprintf(get_resize_photo_xml,NAMESIZE,"%s/%s",xml_path,"get_resize_photo.xml");
    snprintf(get_full_txt_xml,NAMESIZE,"%s/%s",xml_path,"get_full_txt.xml");
    snprintf(get_video_snapshot_xml,NAMESIZE,"%s/%s",xml_path,"get_video_snapshot.xml");
    snprintf(trans_excep_file,128,"%s/%s",log_path,"asuswebstorage_errlog");

    my_mkdir(cloud_path);
    //my_mkdir(temp_path);
    my_mkdir(log_path);
    my_mkdir(asus_path);
    my_mkdir(config_path);
    my_mkdir(script_path);
    my_mkdir(temp_path);
    my_mkdir(xml_path);
    //my_mkdir_r(xml_path);
    //my_mkdir("/tmp/Cloud");

    //remove(general_log);
    remove(trans_excep_file);
}


void init_sync_folder()
{
    //char temp[256];
    //char send_content[NORMALSIZE];
    int status = 0;

    //memset(send_content,0,sizeof(send_content));

    if(test_if_dir_empty(sync_path))
    {
        //printf("sync_path is blank\n");
        //if(sync_up == 1)
        if(!upload_only)
        {
#ifdef DEBUG
            printf("######## init sync folder,please wait......#######\n");
#endif

#if  SYSTEM_LOG
            write_system_log("init","start");
#endif
            //if(sync_up == 1)
            initMyLocalFolder(username,MySyncFolder,sync_path,browse_folder_xml);
#ifdef DEBUG
            printf("######## init sync folder end#######\n");
#endif

#if SYSTEM_LOG
            write_system_log("init","end");
#endif
        }

    }

    //snprintf(send_content,NORMALSIZE,"0@%s",sync_path);
    send_action(1,sync_path);
    //sleep(5);
    //send_action(1,"0@/tmp/mnt/MINIDISK/11");
}

void clean_up()
{
    printf("enter clean up\n");

    remove("/tmp/smartsync/.logs/asuswebstorage");

    check_accout_status();
#if 0
    if( !item_empty(up_head))
    {
        printf("write uncomplete upload item to file\n");
        print_sync_item(up_head,UPLOAD);
    }

    if( !item_empty(down_head))
    {
#if WRITE_DOWN_TEMP_FILE
        remove(down_item_temp_file);
#endif
        printf("write uncomplete download item to file\n");
        print_sync_item(down_head,DOWNLOAD);
    }

    if( !item_empty(up_excep_fail))
    {
        printf("write up excep fail item to file\n");
        print_sync_item(up_excep_fail,UP_EXCEP_FAIL);
    }
#endif

    free_sync_item(up_head);
    free_sync_item(down_head);
    free_sync_item(from_server_sync_head);
    free_sync_item(up_excep_fail);
    queue_destroy(SocketActionList);
    free_sync_item(copy_file_list_head);
    if(download_only == 1)
        free_sync_item(download_only_socket_head);

#if TREE_NODE_ENABLE
    write_tree_to_file(tree_log,DirRootNode);
    free_tree_node(DirRootNode);
#endif

    printf("clean up end !!!\n");
}

void str_to_lower(char *word)
{
    int i;
    int len=strlen(word);
    for(i=0;i<len;i++)
    {
        if(word[i] >='A' && word[i] <= 'Z')
        {
            word[i] += 32;

        }
    }
}

int get_max_upload_filesize(char *username)
{
    Getinfo *gi = NULL;
    int filesize = 0;

    //printf("exit_loop=%d\n",exit_loop);

    while(!exit_loop)
    {
        gi = getInfo(username,sergate.gateway);

        if(NULL == gi)
        {
//#ifdef THREAD_COND_TIMEOUT
            //enter_sleep_time(5);
//#else
            //sleep(5);
//#endif
            //usleep(1000*300);
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }
        if(gi->status != 0)
        {
            handle_error(gi->status,"getinfo");
            my_free(gi);
//#ifdef THREAD_COND_TIMEOUT
            //enter_sleep_time(5);
//#else
            //sleep(5);
//#endif
            //usleep(1000*300);
            enter_sleep_time(1000*300,NULL);
            continue;
        }

        filesize = gi->package.maxfilesize;
        my_free(gi);

        break;
    }

    return filesize;
}

void run()
{
    restart_run = 0;
    int error_time = 0;
    int status;
    int have_error_log = 0;
    int auth_ok = 0;
    int get_sync_id = 0;
    //int removed;

    Getmysyncfolder *gf;
    char *pwd = NULL;
    char pword[256];
    int GetGateOK = 0;
    int AuthFailTimes = 0;
    char temp_key[8] = {0};
    int otp_and_captcha = 0;
    //char new_sync_path[256];
    //char tmp_path[256];

    strcpy(pword,password);
    str_to_lower(pword);
    //printf("pre password is %s,pword is %s\n",password,pword);
    pwd = MD5_string(pword);
    memset(password,0,sizeof(password));
    snprintf(password,256,"%s",pwd);
    my_free(pwd);

    printf("username is %s,pwd is %s,sync path is %s\n",username,password,sync_path);

    memset(&sergate,0,sizeof(Servicegateway));

    while(GetGateOK != 1 && exit_loop != 1)
    {
        if(exit_loop)
            return;

        status = getServiceGateway(username,password,&sergate);//fill Service_geteway struct member info

        if(status == S_NETWORK_FAIL)
        {
           //usleep(1000*500);
           enter_sleep_time(1000*500,NULL);
           check_network_state();
           continue;
        }

        if(status == S_AUTH_FAIL)
        {
            write_log(S_ERROR,"Authentication Failed","");
            return;
        }

        if( sergate.status != 0 )
        {
            handle_error(sergate.status,"gateway");
            return;
        }

        GetGateOK = 1;
    }

#ifdef DEBUG
    printf("getServiceGateway ok\n");
#endif

    if(strlen(cfg.captcha) > 0)
    {
        strcpy(temp_key,cfg.captcha);
        if(strlen(cfg.otp_key) >0)
            otp_and_captcha = 1;
    }
    else if(strlen(cfg.otp_key) > 0)
            strcpy(temp_key,cfg.otp_key);

    //otp_and_captcha = 0;
    //strcpy(temp_key,cfg.otp_key);

    do
    {
        if(exit_loop)
            return;

        if(error_time > 5 && have_error_log != 1)
        {
            write_log(S_ERROR,"Network Connection Failed","");
            have_error_log = 1;
        }

        status = getToken(username,password,temp_key,1);

        if(status !=0)
        {

            if(status == S_NETWORK_FAIL)
            {
                error_time++;
                //usleep(1000*300);
                enter_sleep_time(1000*300,NULL);
                check_network_state();
                continue;

            }
            else
            {
#ifdef IPKG
                char fullname[256] = {0};
                sprintf(fullname,"%s/%s",mount_path,token_filename);
                remove(fullname);
                remove(record_token_file);
#endif            
                if(otp_and_captcha == 1 && status == S_OTP_AUTH_FAIL)
                {
#ifdef DEBUG
                    printf("need captcha and otp\n");
#endif
                    memset(temp_key,0,sizeof(temp_key));
                    //strcpy(temp_key,cfg.captcha);
                    strcpy(temp_key,cfg.otp_key);
                    otp_and_captcha = 0;
                    continue;
                }

                /*if(status == S_AUTH_FAIL)
                {
                    //write_log(S_ERROR,"Authentication Failed","");
                    AuthFailTimes++;
                    printf("AuthFailTimes=%d\n",AuthFailTimes);
                    sleep(1);
                    if(AuthFailTimes >105)
                        return;
                    continue;
                }*/

                return;
            }

            /*if(status == S_AUTH_FAIL)
            {
                write_log(S_ERROR,"Authentication Failed","");
                AuthFailTimes++;
                printf("AuthFailTimes=%d\n",AuthFailTimes);
                sleep(1);
                if(AuthFailTimes >105)
                    return;
                continue;
            }

            if(status == S_OTP_AUTH_FAIL)
            {
                if(strlen(cfg.otp_key)>0)
                {
                    write_log(S_ERROR,"Security Code error.WARNING:If the OTP authentication "
                              "Failures reaches 10 times,OTP will be locked,you must visit the "
                              "official website ,OTP can be used again after unlock it",NULL);
                    //return ;
                }
                else
                {
                    write_log(S_ERROR,"OTP athentication failed.Please input Security Code",NULL);
                    AuthFailTimes++;
                    printf("AuthFailTimes=%d\n",AuthFailTimes);
                    sleep(1);
                    if(AuthFailTimes >105)
                        return;
                    continue;
                    //usleep(1000*500);
                    //continue;
                }
                //return ;
            }

            if(status == S_OTP_ID_LOCKED)
            {
                write_log(S_ERROR,"OTP Services Credential ID is LOCKED state,you must visit "
                          "the official website can be used again after unlock OTP services",NULL);
                return;
            }

            if(status == S_CAPTCHA_FAIL)
            {
                write_log(S_NEEDCAPTCHA,aaa.auxpasswordurl,NULL);
                return;
            }*/
        }



        auth_ok = 1;

    }while(auth_ok != 1 && exit_loop != 1);

#ifdef DEBUG
    printf("GetToken ok\n");
#endif

    do
    {
        if(exit_loop)
            return;

        gf = getMySyncFolder(username);

        if(NULL == gf)
        {
            error_time++;
            //usleep(5000*1000);
            //enter_sleep_time(5);
            //usleep(1000*300);
            enter_sleep_time(1000*300,NULL);
            check_network_state();
            continue;
        }

        if(gf->status != 0)
        {
            error_time ++;
            handle_error(gf->status,"GetMySyncFolderID Fail");
            my_free(gf);
            //usleep(5000*1000);
            //enter_sleep_time(5);
            //usleep(1000*300);
            enter_sleep_time(1000*300,NULL);
            continue;
        }
        get_sync_id = 1 ;
    }while(get_sync_id != 1 && exit_loop != 1);

    //exit_loop = 0;
#ifdef DEBUG
    printf("getMySyncFolder ok\n");
#endif

    max_upload_filesize = get_max_upload_filesize(username);

    status = CheckUserState(username,sergate.gateway);

    if(status == S_ACCOUNT_CLOSE)
           return;

    if(status == S_ACCOUNT_FROZEN)
         IsAccountFrozen = 1;

    MyRecycleID = GetMyRecycleID(username,"MyRecycleBin");

    //printf("MyRecycleID=%d\n",MyRecycleID);

    //max_upload_filesize = 40960;

#ifdef DEBUG
    printf("##### max upload filesize is %dMB ####\n",max_upload_filesize);
#endif

    if(auth_ok == 1)
    {
#if SERVER_TREE
        SRootNode = create_server_treeroot(gf->id);
        browse_to_tree(username,gf->id,browse_folder_xml,SRootNode);
        SearchServerTree(SRootNode);
#endif
        from_server_sync_head = create_head();
        //dragfolder_recursion_head = create_head();
        up_head = create_head();
        down_head =  create_head();
        //socket_list_head = create_head();
        up_excep_fail = create_head();
        copy_file_list_head = create_head();
        if(download_only == 1)
        {
            download_only_socket_head = create_head();
            //download_only_modify_head = create_head();
        }

        if(NULL == from_server_sync_head || NULL == up_head ||
           NULL == up_excep_fail)
            return;

        if(download_only == 1)
        {
            if(NULL == download_only_socket_head )
                return;
        }

        MySyncFolder = gf->id;

        printf("id is %d,MySyncFolder is %d\n",gf->id,MySyncFolder);

        my_free(gf);

        //init_fail_item = 1;

#if WRITE_DOWNLOAD_TEMP_FILE
        if(access(down_item_temp_file,0) == 0)
        {
            parse_trans_item(down_item_temp_file,DOWNLOAD);
            remove(down_item_temp_file);
        }
#endif

#if 0
        if(access(up_item_file,0) == 0)
        {
            parse_trans_item(up_item_file,UPLOAD);

            if(remove(up_item_file) == -1)
            {
                printf("remove %s fail\n",up_item_file);
            }
        }

        if(access(down_item_file,0) == 0)
        {
            parse_trans_item(down_item_file,DOWNLOAD);
            if(remove(down_item_file) == -1)
            {
                printf("remove %s fail\n",down_item_file);
            }
        }

        if(access(up_excep_fail_file,0) == 0)
        {
            init_up_excep_fail(up_excep_fail_file);
        }
#endif

        //init_fail_item = 0 ;

#if TREE_NODE_ENABLE
        if(access(tree_log,0) == 0)
        {
            DirRootNode = read_file_to_tree(tree_log);
            remove(tree_log);
        }
        else
        {
            DirRootNode = create_tree_rootnode(sync_path);
            FindDir(DirRootNode,sync_path);
        }
#endif
        //printf("before write log\n");
        write_log(S_SYNC,"","");//fix local path is blank can't show server space bug

        //if(cfg.sync_disk_exist)
        //    init_sync_folder();
        //create_sync_server_pthread();
        send_action(1,sync_path);

        /*add by alan*/
        SocketActionList = queue_create();

        write_log(S_SYNC,"","");

        if( pthread_create(&local_thread,NULL,(void *)SyncLocal,NULL) != 0)
        {
            printf("thread creation failder\n");
            return;
        }

        //usleep(1000*500);
        enter_sleep_time(1000*500,NULL);

        /*
        if(download_only == 1)
        {
            if( pthread_create(&download_socket_thread,NULL,(void *)Download_Only_Socket_Parser,NULL) != 0)
            {
                printf("thread creation failder\n");
                return;
            }
            usleep(1000*500);
        }
        */
        //else
        //{
            if( pthread_create(&socket_thread,NULL,(void *)Socket_Parser,NULL) != 0)
            {
                printf("thread creation failder\n");
                return;
            }

            //usleep(1000*500);
            enter_sleep_time(1000*500,NULL);

        //}

        if(upload_only != 1)
        {
            if( pthread_create(&server_thread,NULL,(void *)SyncServer,NULL) != 0)
            {
                printf("thread creation failder\n");
                return;
            }

            //usleep(1000*500);
            enter_sleep_time(1000*500,NULL);
        }

        pthread_join(local_thread,NULL);
        //if(download_only)
            //pthread_join(download_socket_thread,NULL);
        //else
        pthread_join(socket_thread,NULL);
        if(!upload_only)
            pthread_join(server_thread,NULL);


    }

    //sleep(2);
    //printf("befor cleap up wait\n");
    //enter_sleep_time(2);
    //usleep(1000*500);
    enter_sleep_time(1000*500,NULL);
    clean_up();

    if(stop_progress != 1)
    {
#ifdef IPKG
        do
        {
            printf("##########enter loop check token file\n");
            if(disk_change)
            {
                sync_disk_removed = check_token_file(&cfg);
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                if(sync_disk_removed == 0)
                {
                    printf("sync disk exists\n");
                }
                else if(sync_disk_removed == 2)
                {
                    printf("sync disk unmount\n");
                }
            }
            else
            {
                //enter_sleep_time(1);;
                //usleep(1000*300);
                enter_sleep_time(1000*300,NULL);
            }

        }while(sync_disk_removed == 2);
        sync_disk_removed = 0;

        //if(removed == 1) //sync disk removed;

        printf("stop_progress is %d \n",stop_progress);
#endif

        if(stop_progress != 1)
        {
            exit_loop = 0;
            if(read_config() != -1)
                run();
        }

    }

}

char  *get_mount_path(char *path , int n)
{
    int i;
    char *m_path = NULL;
    m_path = (char *)malloc(sizeof(char)*NORMALSIZE);
    memset(m_path,0,NORMALSIZE);

    //memset(info_dest_path,'\0',sizeof(info_dest_path));

    char *new_path = NULL;
    new_path = path;

    for(i= 0;i< n ;i++)
    {
        new_path = strchr(new_path,'/');
        if(new_path == NULL)
            break;
        new_path++;
    }

    if( i > 3)
        strncpy(m_path,path,strlen(path)-strlen(new_path)-1);
    else
        strcpy(m_path,path);

    printf("mount path is [%s]\n",m_path);

    //return m_path;
    //strcpy(info_dest_path,new_path);

    return m_path;
}



int read_config()
{
    //char *mount_path;
    char resume_path[NORMALSIZE];
    //char *m_path;
    upload_only = 0;
    download_only = 0;
    int status = 0 ;
    memset(resume_path,0,sizeof(resume_path));

#ifdef IPKG 
    if(create_asuswebstorage_conf_file(CONFIG_PATH) == -1)
    {
        printf("create_asuswebstorage_conf_file fail\n");
        return;
    }
#else
    if(convert_nvram_to_file(CONFIG_PATH) == -1)
    {
        printf("convert_nvram_to_file fail\n");
        return;
    }
#endif

    memset(up_item_file, 0, sizeof(up_item_file));
    memset(down_item_file, 0, sizeof(down_item_file));
#if WRITE_DOWNLOAD_TEMP_FILE
    memset(down_item_temp_file, 0, sizeof(down_item_temp_file));
#endif
    memset(all_local_item_file, 0, sizeof(all_local_item_file));
    memset(up_excep_fail_file,0,sizeof(up_excep_fail_file));
    memset(up_limit_file,0,sizeof(up_limit_file));
    memset(confilicted_log,0,sizeof(confilicted_log));
    memset(tree_log,0,sizeof(tree_log));

    //parse_config(CONFIG_PATH);

//#ifdef IPKG
    memset(&cfg,0,sizeof(struct asus_config));
    parse_config_new(CONFIG_PATH,&cfg);
//#else
    //parse_config_new(CONFIG_PATH,NULL);
//#endif

    if( strlen(username) == 0 )
    {
#ifdef DEBUG
        printf("username is blank ,please input your username and passwrod\n");
#endif

#if SYSTEM_LOG
        write_system_log("error","username is blank ,please input your username and passwrod");
#endif
        no_config = 1;
    }

    /*while(no_config == 1 && exit_loop != 1 )
    {
        //sleep(1);
        //printf("wait for config\n");
        //enter_sleep_time(2);
        //usleep(1000*300);
        enter_sleep_time(1000*300,NULL);

    }*/

    if(no_config)
        return -1;

    if(!no_config)
    {
        //write_log(S_INITIAL,"","");
        //mkdir()
        //printf("sync_path is %s\n",sync_path);

        parse_sync_path(&cfg,sync_path);
        //m_path = get_mount_path(sync_path,4);
        //strcpy(mount_path,m_path);
        //my_free(m_path);

#ifdef IPKG
        memset(record_token_file,0,sizeof(record_token_file));
        memset(token_filename,0,sizeof(token_filename));
        sprintf(token_filename,".__smartsync_0_%s_%s",username,cfg.dir_name);
        sprintf(record_token_file,"/opt/etc/.smartsync/asuswebstorage_%s",cfg.user);
        //get_token_filename(token_filename,sync_path);

        int have_log = 0;
        //struct mounts_info_tag mounts_info;
        //memset(&mounts_info,0,sizeof(mounts_info));

        sync_disk_removed = check_token_file(&cfg);


        printf("disk status is %d \n",sync_disk_removed);
        while(sync_disk_removed == 2) //sync disk unmount
        {
            if(!have_log)
            {
                write_log(S_ERROR,"sync disk unmount","");
                printf("write log end\n");
                have_log = 1;
            }
            if(disk_change)
            {
                printf("enter check tokenf file\n");
                pthread_mutex_lock(&mutex_socket);
                disk_change = 0;
                pthread_mutex_unlock(&mutex_socket);
                sync_disk_removed = check_token_file(&cfg);
            }
            else
               //enter_sleep_time(1);
                //usleep(1000*300);
                enter_sleep_time(1000*300,NULL);
        }
#endif


//#ifndef IPKG



        //get_mounts_info(&mounts_info);
        //check_token_file(&mounts_info,&asus_config);

        /*
        if(sync_disk_removed == 1) //fist start webstorage
        {
          //status = check_record_token_file(record_token_file,&cfg);
          //write_token_file(mount_path,&cfg);

#ifdef IPKG
          memset(record_token_file,0,sizeof(record_token_file));
          sprintf(record_token_file,"/opt/etc/asuswebstorage_%s",cfg.user);
          record_token_to_file(record_token_file,&cfg);
#else
          del_old_token_file(mount_path);
          write_to_nvram(token_filename,NVRAM_TOKENFILE);
#endif
          write_token_file(mount_path,token_filename);
          */
         //if(cfg.sync_disk_exist)
         //{
            if(my_mkdir_r(sync_path) == -1)
            {
               printf("create sync_path=%s fail,asuswebstorage exit\n",sync_path);
               write_log(S_ERROR,"Create sync path fail,please check whether the hard disk is read only mode","");
               exit(-1);
            }

            DIR *dir = opendir(mount_path);
            if(NULL == dir)
            {
                printf("open %s fail,asuswebstorage exit\n");
                exit(-1);
            }
            closedir(dir);

            snprintf(resume_path,NORMALSIZE,"%s/.smartsync/asuswebstorage",mount_path);
            my_mkdir_r(resume_path);

            snprintf(up_item_file,NAMESIZE,"%s/%s_up_item",resume_path,username);
            snprintf(down_item_file,NAMESIZE,"%s/%s_down_item",resume_path,username);
    #if WRITE_DOWNLOAD_TEMP_FILE
            sprintf(down_item_temp_file,"%s/%s_down_temp_item",resume_path,username);
    #endif
            snprintf(all_local_item_file,NAMESIZE,"%s/%s_all_local_item",resume_path,username);
            snprintf(up_excep_fail_file,NAMESIZE,"%s/%s_up_excep_fail",resume_path,username);
            snprintf(up_limit_file,NAMESIZE,"%s/err.log",resume_path,username);
            snprintf(confilicted_log,NAMESIZE,"%s/confilicted.log",resume_path);
            snprintf(tree_log,NAMESIZE,"%s/tree.log",resume_path);
         //}

        //}
        //printf("disk status is %d \n",sync_disk_removed);

//#endif



        //my_free(mount_path);
        no_config = 0 ;
        exit_loop = 0;
    }

    return 0;

}


int retry_all_fail_item(int type)
{
    if(upload_only)
        has_socket = 1;
    struct sync_item *point,*p2;
    Transitem *item;
    char content[NORMALSIZE];
    int retry_time = 1;
    char *filename;
    int status;
    Propfind *find;
    int parentID = -10;
    char path[NORMALSIZE];
    int ok;

    memset(content,0,sizeof(content));
    memset(path,0,sizeof(path));

    if(type == UP_FAIL)
        point = up_head->next;
    //else if(type == DOWN_FAIL)
        //point = down_head->next;
    //else if(type == SOCKET_FAIL)
    //point = socket_list_head->next;

    while(point != NULL)
    {
        p2 = point;
        point = point->next;

        switch (type)
        {
        case UP_FAIL:
            snprintf(content,NORMALSIZE,"%s,%s",p2->action,p2->name);
            item = parse_trans_item_from_buffer(content,UPLOAD);
            struct stat buf;

            if(NULL == item)
            {
                return -1;
            }

            if(item->id > 0)
            {
                if( stat(item->name,&buf) == -1)
                {
                    printf(" %s file does not exist\n",item->name);
                    del_sync_item(p2->action,p2->name,up_head);
                    del_sync_item("up_excep_fail",p2->name,up_excep_fail);
                    return -1;
                }
#ifdef DEBUG
                printf("##### handle upload fail file is %s,parentid id %d,transid is %s#####\n",item->name,item->id,item->transid);
#endif
                while(retry_time)
                {
                    //status = uploadFile(item->name,item->id,item->transid);
                    status = uploadFile(item->name,item->id,NULL);
                    if(status == 0)
                    {
                        ok = 1;
                        del_sync_item(p2->action,p2->name,up_head);
                        break;
                    }
                    else
                        retry_time--;
                }
            }
            else
            {
                del_sync_item(p2->action,p2->name,up_head);
            }
            my_free(item);
            break;
#if 0
        case DOWN_FAIL:
            snprintf(content,NORMALSIZE,"%s,%s",p2->action,p2->name);
            item = parse_trans_item_from_buffer(content,DOWNLOAD);

            if(item->id > 0)
            {
#ifdef DEBUG
                //printf("##### download fail file is %s#####\n",item->name);
                //printf("##### download arg id=%d,name=%s,size=%lld #####\n",item->id,item->name,item->size);
#endif

                filename = parse_name_from_path(item->name);

                if(NULL == filename)
                {
                    printf("parse %s filenam fail\n",item->name);
                    my_free(item);
                    return -1;
                }

                //printf("download filenam is %s\n",filename);

                strncpy(path,item->name,strlen(item->name)-strlen(filename)-1);


                parentID = getParentID(path);

                if(parentID <= 0)
                {
                    printf("obtain %s parent ID is fai\n",item->name);
                    my_free(filename);
                    my_free(item);
                    return -1;
                }

                find = checkEntryExisted(username,parentID,filename,"system.file");

                if(NULL == find)
                {
                    printf("find prop failed\n");
                    my_free(filename);
                    my_free(item);
                    return -1;
                }
                else if(find->status != 0)
                {
                    handle_error(find->status,"profind");
                    my_free(filename);
                    my_free(item);
                    my_free(find);
                    return -1;
                }
                else if(!strcmp(find->type,"system.notfound"))
                {
                    del_sync_item(p2->action,p2->name,down_head);
                    my_free(filename);
                    my_free(item);
                    my_free(find);
                    return 0;
                }
                else if(!strcmp(find->type,"system.file"))
                {
                    if( find->status != 0 )
                    {
                        handle_error(find->status,"propfind");
                        my_free(find);
                        my_free(item);
                        return -1;
                    }

                    //printf("prppfind status=%d,type=%s\n",find->status,find->type);

                    if(!strncmp(find->type,"system.file",sizeof("system.file")))
                    {
                        while(retry_time)
                        {
                            status = downloadFile(item->id,item->name,item->size,0);
                            if(status == 0)
                            {
                                ok = 1;
                                break;
                            }
                            else
                            {
                                printf("##### download %s fail #####\n",item->name);
                                retry_time--;
                            }
                        }
                    }
                }

                my_free(filename);
                my_free(item);
                my_free(find);
            }
            break;
#endif

              case SOCKET_FAIL:
            /*
                  if(cmd_parser(p2->name) == 0)
                  {
                      del_sync_item("socket",p2->name,socket_list_head);
                  */
            break;

              default:
            break;
        }
    }
    ///retry_fail_time == 0;
    return 0;
}

void handle_fail_item()
{
    //retry_fail_time++;
#if 0
    if(!item_empty())
    {
#ifdef DEBUG
        printf("######## handle downoad fail########\n");
#endif
        retry_all_fail_item(DOWN_FAIL);
    }
#endif

    if(!item_empty(up_head))
    {
#ifdef DEBUG
        printf("######## handle up item fail########\n");
#endif
        retry_all_fail_item(UP_FAIL);
    }
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
    sigaddset(&waitset,SIGPIPE);

    //sigfillset(&waitset);

    while (1)  {
        rc = sigwait(&waitset, &sig);
        if (rc != -1) {
            printf("sigwait() fetch the signal - %d\n", sig);
            sig_handler(sig);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}

#ifndef IPKG

/*
int update_mount_path_to_nvram(char *new_sync_path)
{
    char *nv, *nvp, *b;
    char *type, *url ,*user,*pwd,*s_path,*rule, *enable;
    int len;

    //char new_sync_path[256];

    nv = nvp = strdup(nvram_safe_get("clould_sync"));

    if(nv) {
        while ((b = strsep(&nvp, "<")) != NULL) {
            if((vstrsep(b, ">", &type, &user,&pwd,&url,&rule,&s_path,&enable)!=7)) continue;
            if(strlen(user)==0||strlen(pwd)==0) continue;
            printf("%s,%s,%s,%s,%s,%s,%s\n",type,enable,user,pwd,url,rule,s_path);

            nvram_safe_set("cloud_sync",new_sync_path);

        }
        free(nv);
    }
    //fclose(fp);
}
*/
#endif

#ifdef IPKG
int write_notify_file(char *path,int signal_num)
{
    FILE *fp;
    char fullname[64];
    memset(fullname,0,sizeof(fullname));

    //my_mkdir_r(path);
    my_mkdir("/tmp/notify");
    my_mkdir("/tmp/notify/usb");
    sprintf(fullname,"%s/asuswebstorage",path);
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
#endif
