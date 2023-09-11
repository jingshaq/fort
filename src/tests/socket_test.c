#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/net.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/syscalls.h>

static struct nf_hook_ops nfho;
asmlinkage long (*original_socketcall)(int call, unsigned long *args);

// Hook函数
unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    pid_t pid = task_tgid_nr(current);

    if (!skb)
        return NF_ACCEPT;

    ip_header = ip_hdr(skb);
    if (ip_header->protocol == IPPROTO_TCP) {
        tcp_header = tcp_hdr(skb);
        printk(KERN_INFO "Process PID: %d, Source PID: %d, Source IP: %pI4, Source Port: %d\n",
               pid, task_pid_nr(current), &ip_header->saddr, ntohs(tcp_header->source));
        printk(KERN_INFO "Dest IP: %pI4, Dest Port: %d\n", &ip_header->daddr, ntohs(tcp_header->dest));
    }

    return NF_ACCEPT;
}

asmlinkage long hook_socketcall(int call, unsigned long *args) {
    long ret;
    
    if (call == SYS_SOCKET) {
        printk(KERN_INFO "Socket call intercepted: SYS_SOCKET\n");
    } else if (call == SYS_CONNECT) {
        printk(KERN_INFO "Socket call intercepted: SYS_CONNECT\n");
    }

    ret = original_socketcall(call, args);
    
    return ret;
}

static int __init mymodule_init(void) {
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho);

    // Hook socketcall
    original_socketcall = (void *)syscall_table[__NR_socketcall];
    syscall_table[__NR_socketcall] = hook_socketcall;

    return 0;
}

static void __exit mymodule_exit(void) {
    nf_unregister_net_hook(&init_net, &nfho);

    // Restore original socketcall
    syscall_table[__NR_socketcall] = original_socketcall;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Socket Hooking Module");
