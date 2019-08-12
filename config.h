/*
 *  Copyright (C) 2012-2013 Samsung Electronics Co., Ltd.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _EXFAT_CONFIG_H
#define _EXFAT_CONFIG_H
/*======================================================================*/
/*                                                                      */
/*                        FFS CONFIGURATIONS                            */
/*                  (CHANGE THIS PART IF REQUIRED)                      */
/*                                                                      */
/*======================================================================*/

/*----------------------------------------------------------------------*/
/* Feature Config                                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* Debug/Experimental Config                                            */
/*----------------------------------------------------------------------*/
//#define CONFIG_EXFAT_TRACE_IO
//#define CONFIG_EXFAT_TRACE_LOCK /* Trace elapsed time in lock_super(sb) */

/*----------------------------------------------------------------------*/
/* Defragmentation Config                                               */
/*----------------------------------------------------------------------*/
//#define	CONFIG_EXFAT_DFR
//#define	CONFIG_EXFAT_DFR_PACKING
//#define	CONFIG_EXFAT_DFR_DEBUG

/*----------------------------------------------------------------------*/
/* Config for Kernel equal or newer than 3.7                            */
/*----------------------------------------------------------------------*/
#ifndef CONFIG_EXFAT_WRITE_SB_INTERVAL_CSECS
#define CONFIG_EXFAT_WRITE_SB_INTERVAL_CSECS	(dirty_writeback_interval)
#endif

/*----------------------------------------------------------------------*/
/* Default Kconfig                                                      */
/*----------------------------------------------------------------------*/
/* default mount options                            */
#ifndef CONFIG_EXFAT_DEFAULT_CODEPAGE /* if Kconfig lacked codepage */
#define CONFIG_EXFAT_DEFAULT_CODEPAGE   437
#endif

#ifndef CONFIG_EXFAT_DEFAULT_IOCHARSET /* if Kconfig lacked iocharset */
#define CONFIG_EXFAT_DEFAULT_IOCHARSET  "utf8"
#endif

#ifndef CONFIG_EXFAT_FAT32_SHORTNAME_SEQ /* Shortname ~1, ... ~9 have higher
					  * priority (WIN32/VFAT-like)
					  */
//#define CONFIG_EXFAT_FAT32_SHORTNAME_SEQ
#endif

#ifndef CONFIG_EXFAT_ALIGNED_MPAGE_WRITE
//#define CONFIG_EXFAT_ALIGNED_MPAGE_WRITE
#endif

#ifndef CONFIG_EXFAT_FAT_MIRRORING /* if Kconfig lacked fat-mirroring option */
#define CONFIG_EXFAT_FAT_MIRRORING /* Write FAT 1, FAT 2 simultaneously */
#endif

#ifndef CONFIG_EXFAT_DELAYED_META_DIRTY
//#define CONFIG_EXFAT_DELAYED_META_DIRTY /* delayed DIR/FAT dirty support */
#endif

#ifndef CONFIG_EXFAT_SUPPORT_DIR_SYNC
//#define CONFIG_EXFAT_SUPPORT_DIR_SYNC /* support DIR_SYNC */
#endif

#ifndef CONFIG_EXFAT_CHECK_RO_ATTR
//#define CONFIG_EXFAT_CHECK_RO_ATTR
#endif

#ifndef CONFIG_EXFAT_RESTRICT_EXT_ONLY_SFN
#define CONFIG_EXFAT_RESTRICT_EXT_ONLY_SFN
#endif

#ifndef CONFIG_EXFAT_ALLOW_LOOKUP_LOSSY_SFN
//#define CONFIG_EXFAT_ALLOW_LOOKUP_LOSSY_SFN
#endif

#ifndef CONFIG_EXFAT_DBG_SHOW_PID
//#define CONFIG_EXFAT_DBG_SHOW_PID
#endif

#ifndef CONFIG_EXFAT_VIRTUAL_XATTR
//#define CONFIG_EXFAT_VIRTUAL_XATTR
#endif

#ifndef CONFIG_EXFAT_SUPPORT_STLOG
//#define CONFIG_EXFAT_SUPPORT_STLOG
#endif

#ifndef CONFIG_EXFAT_DEBUG
//{
//#define CONFIG_EXFAT_DEBUG

#ifndef CONFIG_EXFAT_DBG_IOCTL
//#define CONFIG_EXFAT_DBG_IOCTL
#endif

#ifndef CONFIG_EXFAT_DBG_MSG
//#define CONFIG_EXFAT_DBG_MSG
#endif

#ifndef CONFIG_EXFAT_DBG_CAREFUL
//#define CONFIG_EXFAT_DBG_CAREFUL
#endif

#ifndef CONFIG_EXFAT_DBG_BUGON
//#define CONFIG_EXFAT_DBG_BUGON
#endif

#ifndef CONFIG_EXFAT_DBG_WARNON
//#define CONFIG_EXFAT_DBG_WARNON
#endif
//}
#endif /* CONFIG_EXFAT_DEBUG */


#ifndef	CONFIG_EXFAT_TRACE_SB_LOCK
//#define CONFIG_EXFAT_TRACE_SB_LOCK
#endif

#ifndef	CONFIG_EXFAT_TRACE_ELAPSED_TIME
//#define CONFIG_EXFAT_TRACE_ELAPSED_TIME
#endif

#endif /* _EXFAT_CONFIG_H */

/* end of config.h */
