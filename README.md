# ECEN5013-HW2
Kernel Module, Kernel Call, HW1 Unit Tests

## Maintained By:
[Chase E Stewart](chst9148@colorado.edu)

## Overview:
This repository holds an example kernel module that implements a logging 500msec timer, unit tests for the data structures in HW1, and a system call to be compiled into the Ubuntu 16.04 kernel that does a int32_t buffer sort.

The C libraries follow the CU Boulder Embedded Systems Engineering C-Coding and HDL Style Guideline.

## Usage:

#### Syscall
It will take some work to install the system call into your Ubuntu 16.04 computer. The gist of the installation is the following
1) wget and unzip the latest (or your favorite) linux kernel - I used 4.12.10.
2) create a top-level folder, add the syscall into this folder, and place the relative path into the `core-y += ...` folder.
3) add the following line into kernel/arch/x86/syscalls/syscall_64.tbl:
```bash
333   common  sort          sys_sort

```
4) build the kernel:
```bash
cd path\to\linux
cp /boot/config-`uname -r`* .config
make defconfig
make
sudo make modules_install install
```
5) call `sudo reboot`

#### Kernel Module
For the kernel module, do the following:
1) run `cd kernel_module/` and then run `make all`.
2) run `cp kernel_module /lib/modules/<your kernel version>/linux/drivers/char`
3) run `sudo depmod -a`
4) run `insmod path/to/example_mod.ko`
5) run `modinfo example_mod`
6) run `modprobe -r example_mod`
7) check your syslog

#### Unit Tests
For the unit test, simply cd to the folder, run `make all` and then run `./unit_test`
