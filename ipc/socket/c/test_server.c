
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    int main(void)
    {
        //create socket
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd==-1)
        {
            perror("socket\n");
            exit(-1);
        }
        printf("socket fd=%d\n",fd);

        //build connection address
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(6666);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        int r;
        r = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
        if(r==-1)
        {
            perror("bind");
            close(fd);
            exit(-1);
        }
        printf("bind address successful!\n");
        //accept or send message
        char buf[255];
        struct sockaddr_in from;
        socklen_t len;
        len = sizeof(from);
        while(1)
        {
            r = recvfrom(fd,buf,sizeof(buf)-1,0,(struct sockaddr*)&from,&len);
            if(r>0)
            {
                buf[r]=0;
                printf("The message from %s is:%s\n",inet_ntoa(from.sin_addr),buf);
            }
            else
            {
                break;
            }
        }
        //close socket
        close(fd);
        return 0;
    }
