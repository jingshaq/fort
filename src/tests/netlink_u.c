#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/genetlink.h>

#define GENL_FAMILY_NAME "my_genl_family"
#define GENL_VERSION 1
#define MY_GENL_CMD_ECHO 0
#define MY_GENL_ATTR_DATA 1
#define MY_GENL_ATTR_DATA_LEN 2

struct nl_msg {
    struct nlmsghdr n;
    struct genlmsghdr g;
    char buf[256];
};

int main() {
    int sockfd;
    struct sockaddr_nl sa;
    struct nl_msg msg;
    char recv_buf[256];

    sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = 0;
    sa.nl_pid = getpid();

    if (bind(sockfd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    memset(&msg, 0, sizeof(msg));
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = GENL_ID_GENERATE;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.g.cmd = CTRL_CMD_GETFAMILY;
    msg.g.version = GENL_VERSION;
    strcpy(msg.buf, GENL_FAMILY_NAME);

    if (send(sockfd, &msg, msg.n.nlmsg_len, 0) == -1) {
        perror("send");
        close(sockfd);
        return 1;
    }

    memset(recv_buf, 0, sizeof(recv_buf));
    if (recv(sockfd, recv_buf, sizeof(recv_buf), 0) == -1) {
        perror("recv");
        close(sockfd);
        return 1;
    }

    struct nlattr *tb[GENL_HDRLEN];
    struct genlmsghdr *genl = (struct genlmsghdr*)recv_buf;
    struct nlattr *attrs = GENLMSG_DATA(genl);

    if (nla_parse(tb, GENL_HDRLEN, attrs, genl->genlmsg_len - GENL_HDRLEN, NULL) < 0) {
        fprintf(stderr, "nla_parse failed\n");
        close(sockfd);
        return 1;
    }

    if (tb[CTRL_ATTR_FAMILY_ID]) {
        msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
        msg.n.nlmsg_type = tb[CTRL_ATTR_FAMILY_ID]->nla_type;
        msg.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
        msg.g.cmd = MY_GENL_CMD_ECHO;

        strcpy(msg.buf, "Hello from user space!");

        if (send(sockfd, &msg, msg.n.nlmsg_len, 0) == -1) {
            perror("send");
            close(sockfd);
            return 1;
        }
    } else {
        fprintf(stderr, "Failed to get family ID\n");
        close(sockfd);
        return 1;
    }

    close(sockfd);
    return 0
