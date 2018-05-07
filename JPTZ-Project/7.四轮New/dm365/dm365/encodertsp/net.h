#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
static int getIfaceName(char *iface_name, int len)
{
    int r = -1;
    int flgs, ref, use, metric, mtu, win, ir;
    unsigned long int d, g, m;    
    char devname[20];
    FILE *fp = NULL;

    if((fp = fopen("/proc/net/route", "r")) == NULL) {
        perror("fopen error!\n");
        return -1;
    }

    if (fscanf(fp, "%*[^\n]\n") < 0) {
        fclose(fp);
        return -1;
    }

    while (1) {
        r = fscanf(fp, "%19s%lx%lx%X%d%d%d%lx%d%d%d\n",
                 devname, &d, &g, &flgs, &ref, &use,
                 &metric, &m, &mtu, &win, &ir);
        if (r != 11) {
            if ((r < 0) && feof(fp)) {
                break;
            }
            continue;
        }

        strncpy(iface_name, devname, len);
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return -1;
}

static int getIpAddress(char *iface_name, char *ip_addr, int len)
{
    int sockfd = -1;
    struct ifreq ifr;
    struct sockaddr_in *addr = NULL;

    memset(&ifr, 0, sizeof(struct ifreq));
    strcpy(ifr.ifr_name, iface_name);
    addr = (struct sockaddr_in *)&ifr.ifr_addr;
    addr->sin_family = AF_INET;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket error!\n");
        return -1;
    }
    
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) == 0) {
        strncpy(ip_addr, inet_ntoa(addr->sin_addr), len);
        close(sockfd);
        return 0;
    }

    close(sockfd);

    return -1;
}

void test_net_address(void)
{
    //struct in_addr    intaddr;
    char iface_name[20];
	char ip_addr[15];

	memset(iface_name, 0, 20);
	memset(ip_addr, 0, 15);

    if(getIfaceName(iface_name, sizeof(iface_name)) < 0) {
        printf("get interface name error!\n");
        return -1;
    }

	printf("iface name:%s\n", iface_name);

    if(getIpAddress(iface_name, ip_addr, 15) < 0) {
        printf("get interface ip address error!\n");
        return -1;
    }
    
    printf("address:%s\n", ip_addr);
}
