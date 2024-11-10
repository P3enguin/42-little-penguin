# Little Penguin 🐧

![N|Solid](https://i.pinimg.com/originals/a8/c0/1d/a8c01db2df0e9861ff29607b6b448dbf.gif)

This project is designed to introduce you to the fundamentals of kernel modules, device files, and the Linux kernel's inner workings through.
Below is a set of resources that helped me.

## 01:
Bro just clone the kernel and do what u did in ft_linux.

## 02:
The Linux Kernel Module Programming Guide :<br/> 
https://sysprog21.github.io/lkmpg/

## 03:
- Creating a patch  :
    ```git
    git add Makefile
    git commit -m "Modified EXTRAVERSION to add -thor_kernel suffix"
    git format-patch HEAD^ 
    ```

- Applaying patch:
    ```git
    git apply PATCH-FILE-CREATED.patch
    ```

- Understanding linux dev cycle :<br/> 
https://atikraja.hashnode.dev/introduction-to-linux-kernel-development-process


## 04 - 05:
- If you are using udev :<br/> 
https://bootlin.com/doc/legacy/udev/udev.pdf

- Major and minor number :<br/> 
https://embetronicx.com/tutorials/linux/device-drivers/character-device-driver-major-number-and-minor-number/

- Misc device driver :<br/> 
https://embetronicx.com/tutorials/linux/device-drivers/misc-device-driver/

## 06:<br/> 
- Working with linux-next:<br/> 
https://www.kernel.org/doc/man-pages/linux-next.html

## 07:

- debugfs :<br/> 
https://docs.kernel.org/filesystems/debugfs.html

- Might be useful<br/> 
https://embetronicx.com/tutorials/linux/device-drivers/sysfs-in-linux-kernel/


## 09 :
- procfs :<br/> 
https://embetronicx.com/tutorials/linux/device-drivers/procfs-in-linux/

- mnt_namespace  & mounts struct definition :<br/> 
https://elixir.bootlin.com/linux/v6.12-rc6/source/fs/mount.h

- Understanding nsproxy :<br/> 
https://medium.com/@boutnaru/the-linux-kernel-data-structure-journey-struct-nsproxy-b032c71715c5

- seq_file interface :<br/> 
https://docs.kernel.org/filesystems/seq_file.html

- might be useful:<br/> 
https://unix.stackexchange.com/questions/4402/what-is-a-superblock-inode-dentry-and-a-file
