// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <linux/rbtree.h>
#include <../fs/mount.h>
#include <linux/mount.h>

static void get_full_path(struct seq_file *seq, struct path *path)
{
	char *buf;
	char *path_str;

	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!buf)
		return;
	path_str = d_path(path, buf, PATH_MAX);
	if (!IS_ERR(path_str))
		seq_printf(seq, "%s", path_str);
	kfree(buf);
}

static int print_mounts(struct seq_file *seq, void *ptr)
{
	struct rb_root mounts;
	struct mount *mnt, *tmp;
	struct super_block *sb;
	struct path path;

	/* Since Kernel 6.8, the mounts have been stored as a red-black tree. */
	mounts = current->nsproxy->mnt_ns->mounts;
	rbtree_postorder_for_each_entry_safe(mnt, tmp, &mounts, mnt_node) {
		sb = mnt->mnt.mnt_sb;
		path.mnt = &mnt->mnt;
		path.dentry = mnt->mnt.mnt_root;
		seq_printf(seq, "%s ", sb->s_id);
		get_full_path(seq, &path);
		seq_puts(seq, "\n");
	}
	return 0;
}

static int open_proc(struct inode *inode, struct file *file)
{
	return single_open(file, print_mounts, NULL);
}

static const struct proc_ops  proc_fops = {
	.proc_open = open_proc,
	.proc_read = seq_read,          // seq_read will handle reading data
	.proc_lseek = seq_lseek,        // seq_lseek for seeking in the seq file
	.proc_release = single_release, // single_release to release resources
};

int init_module(void)
{
	struct  proc_dir_entry *proc_entry;

	proc_entry = proc_create("mymounts", 0444, NULL, &proc_fops);
	if (!proc_entry)
		return -ENOMEM;
	pr_info("mymounts created successfully");
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry("mymounts", NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("penguu");
MODULE_DESCRIPTION("Mount points listing module");
