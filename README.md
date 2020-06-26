# exfat-linux

This __exFAT filesystem module for Linux kernel__ is a backport of the latest Linux mainline's exFAT drivers by Samsung.

This project can be used for everyday Linux users by simply doing `make && make install`. Ubuntu users can simply add a PPA and start using it, without even downloading the code. This can also be directly dropped-in to an existing Linux kernel source for building the filesystem drivers inline, which should be useful for Android kernel developers.

**exfat-linux** has been tested with all major LTS kernels ranging from v4.9 to v5.4 and the ones Canonical uses for Ubuntu: `v4.9`, `v4.14`, `v4.19`, `v5.4` and `v4.15`, `v5.3`, and `v5.6`.

It's also been tested with `x86(i386)`, `x86_64(amd64)`, `arm32(AArch32)` and `arm64(AArch64)`.

Linux kernels since `v5.4` includes an exFAT driver, but it is an extremely outdated version from 2016. This was later revised by Samsung directly with `v5.7`.

People on `v5.7` kernel or higher can just use the bundled exFAT drivers.

People on `v5.4+` are highly recommended to use this drivers.

Support for kernel versions lower than `v4.9` were dropped for easier maintenance. For people interested in exFAT support for said kernels, please use the [old branch](https://github.com/arter97/exfat-linux/tree/old). It still works nicely and it's actively being shipped to production smartphones.

exfat-linux is planned to be maintained until Android devices with `v5.7+` LTS kernel become more common.

## Disclaimer

#### ● Original authorship and copyright: Samsung

#### ● Maintainer of exfat-linux: Park Ju Hyung([arter97](https://twitter.com/arter97))

## Using exfat-linux

### ● Ubuntu PPA

If you're an Ubuntu user, you can simply add a [PPA repository](https://launchpad.net/~arter97/+archive/ubuntu/exfat-linux) and start using the exFAT module.

Ubuntu will handle upgrades automatically as well.

1. Add the exfat-linux repository

   ```
   sudo add-apt-repository ppa:arter97/exfat-linux
   sudo apt update
   ```

2. Install the module

   `sudo apt install exfat-dkms`

This will use DKMS(Dynamic Kernel Module Support) and automatically build exFAT module for your current Ubuntu installation.

### ● Manually installing the module

1. Download the code

   ```
   git clone https://github.com/arter97/exfat-linux
   cd exfat-linux
   ```

2. Build

   `make`

3. Install

   `sudo make install`

This will install the module to your __currently running kernel__.

__If you're running a `v5.4+` kernel, it is highly recommended to reboot at this point to prevent the existing staging exFAT drivers to load.__

4. And finally load

   `sudo modprobe exfat`

If you upgrade the kernel, you'll have to repeat this process.

If you want to update **exfat-linux** to the latest version, you'll have to repeat this process.

### ● Merging the drivers to existing Linux kernel source

If you're using `git`, using `git subtree` or `git submodule` is highly recommended.

1. Add this repository to `fs/exfat`

2. Modify `fs/Kconfig`

```
 menu "DOS/FAT/NT Filesystems"

 source "fs/fat/Kconfig"
+source "fs/exfat/Kconfig"
 source "fs/ntfs/Kconfig"
 endmenu
```

3. Modify `fs/Makefile`

```
 obj-$(CONFIG_FAT_FS)    += fat/
+obj-$(CONFIG_EXFAT_FS)  += exfat/
 obj-$(CONFIG_BFS_FS)    += bfs/
```

And you're good to go!

## Benchmarks

For reference, existing exFAT implementations were tested and compared on a server running Ubuntu 16.04 with Linux kernel 4.14 under a contained virtual machine.

Linux 4.14 was used as higher LTS kernels don't work with [exfat-nofuse] at the time of testing.

__The new base backported from mainline is not benchmarked yet.__

### ● Ramdisk

#### fio sequential I/O

| Implementation   | Base   | Read         | Write        |
| ---------------  | ------ | ------------ | ------------ |
| **exfat-linux**  | 2.2.0  |    7042 MB/s |    2173 MB/s |
| [exfat-nofuse]   | 1.2.9  |    6849 MB/s |    1961 MB/s |
| [exfat-fuse]     | N/A    |    3097 MB/s |    1710 MB/s |
| ext4             | N/A    |    7352 MB/s |    3333 MB/s |

#### fio random I/O

| Implementation   | Base   | Read         | Write        |
| ---------------  | ------ | ------------ | ------------ |
| **exfat-linux**  | 2.2.0  |     760 MB/s |    2222 MB/s |
| [exfat-nofuse]   | 1.2.9  |     760 MB/s |    2160 MB/s |
| [exfat-fuse]     | N/A    |     1.7 MB/s |     1.6 MB/s |
| ext4             | N/A    |     747 MB/s |    2816 MB/s |

### ● NVMe device

#### fio sequential I/O

| Implementation   | Base   | Read         | Write        |
| ---------------  | ------ | ------------ | ------------ |
| **exfat-linux**  | 2.2.0  |    1283 MB/s |    1832 MB/s |
| [exfat-nofuse]   | 1.2.9  |    1285 MB/s |    1678 MB/s |
| [exfat-fuse]     | N/A    |     751 MB/s |    1464 MB/s |
| ext4             | N/A    |    1283 MB/s |    3356 MB/s |

#### fio random I/O

| Implementation   | Base   | Read         | Write        |
| ---------------  | ------ | ------------ | ------------ |
| **exfat-linux**  | 2.2.0  |      26 MB/s |    1885 MB/s |
| [exfat-nofuse]   | 1.2.9  |      24 MB/s |    1827 MB/s |
| [exfat-fuse]     | N/A    |     1.6 MB/s |     1.6 MB/s |
| ext4             | N/A    |      29 MB/s |    2821 MB/s |

[exfat-fuse]: https://github.com/relan/exfat

## Mount options

* uid
* gid
* umask
* dmask
* fmask
* allow_utime
* iocharset
* quiet
* time_offset

  * Please refer to the [vfat](https://github.com/torvalds/linux/blob/master/Documentation/filesystems/vfat.txt)'s documentation.

* errors=continue

  * Keep going on a filesystem error.

* errors=panic

  * Panic and halt the machine if an error occurs.

* errors=remount-ro

  * Remount the filesystem read-only on an error.

* discard

  * Enable the use of discard/TRIM commands to ensure flash storage doesn't run out of free blocks. This option may introduce latency penalty on file removal operations.

## Enjoy!
