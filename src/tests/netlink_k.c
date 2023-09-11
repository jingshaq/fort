#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");

#define GENL_FAMILY_NAME "my_genl_family"
#define GENL_VERSION 1

static struct genl_family my_genl_family = {
    .name = GENL_FAMILY_NAME,
    .version = GENL_VERSION,
};

static int my_genl_echo(struct sk_buff *skb, struct genl_info *info) {
    struct sk_buff *msg;
    void *msg_head;
    int rc = 0;

    msg = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
    if (!msg) {
        rc = -ENOMEM;
        goto out;
    }

    msg_head = genlmsg_put(msg, 0, info->snd_seq, &my_genl_family, 0, info->genlhdr->cmd);
    if (!msg_head) {
        rc = -ENOMEM;
        goto out_free_msg;
    }

    // Copy user data from info->attrs to msg
    rc = nla_put(msg, MY_GENL_ATTR_DATA, info->attrs[MY_GENL_ATTR_DATA], info->attrs[MY_GENL_ATTR_DATA_LEN]);
    if (rc) {
        goto out_free_msg;
    }

    genlmsg_end(msg, msg_head);

    rc = genlmsg_reply(msg, info);

out_free_msg:
    nlmsg_free(msg);
out:
    return rc;
}

static const struct genl_ops my_genl_ops[] = {
    {
        .cmd = MY_GENL_CMD_ECHO,
        .flags = 0,
        .policy = my_genl_policy,
        .doit = my_genl_echo,
    },
};

static struct genl_multicast_group my_genl_mcgrps[] = {};

static int __init my_module_init(void) {
    int rc;

    rc = genl_register_family(&my_genl_family);
    if (rc) {
        pr_err("Failed to register genl family: %d\n", rc);
        return rc;
    }

    rc = genl_register_ops(&my_genl_family, my_genl_ops, ARRAY_SIZE(my_genl_ops));
    if (rc) {
        pr_err("Failed to register genl ops: %d\n", rc);
        genl_unregister_family(&my_genl_family);
        return rc;
    }

    pr_info("Module loaded\n");
    return 0;
}

static void __exit my_module_exit(void) {
    genl_unregister_ops(&my_genl_family, my_genl_ops, ARRAY_SIZE(my_genl_ops));
    genl_unregister_family(&my_genl_family);
    pr_info("Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
