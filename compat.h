// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2012-2013 Samsung Electronics Co., Ltd.
 */

#ifndef _EXFAT_COMPAT_H
#define _EXFAT_COMPAT_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
#error "This driver doesn't support v5.8+, " \
       "please use the included driver from your kernel"
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0)
#error "This driver doesn't support kernel versions lower than v4.9, " \
       "please use the driver from https://github.com/arter97/exfat-linux/tree/old"
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 16, 0)
#include <linux/iversion.h>
#else
#define inode_inc_iversion(inode) (inode->i_version++)
#define inode_query_iversion(inode) (inode->i_version)
#define inode_eq_iversion(inode, version) (inode->i_version == version)
#define inode_peek_iversion_raw(inode) (inode->i_version)
#define inode_set_iversion(inode, val) (inode->i_version = val)
#endif

/* MS flags were renamed to SB on v4.15 */
#ifndef SB_NODIRATIME
#define SB_NODIRATIME MS_NODIRATIME
#endif

#ifndef SB_RDONLY
#define SB_RDONLY MS_RDONLY
#endif

#ifndef SB_SYNCHRONOUS
#define SB_SYNCHRONOUS MS_SYNCHRONOUS
#endif

#ifndef sb_rdonly
#define sb_rdonly(sb) ((sb)->s_flags & SB_RDONLY)
#endif

#endif /* _EXFAT_COMPAT_H */
