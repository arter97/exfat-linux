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

/*----------------------------------------------------------------------*/
/* Debug/Experimental Config                                            */
/*----------------------------------------------------------------------*/
//#define CONFIG_EXFAT_TRACE_IO
//#define CONFIG_EXFAT_TRACE_LOCK /* Trace elapsed time in lock_super(sb) */

#ifndef CONFIG_EXFAT_WRITE_SB_INTERVAL_CSECS
#define CONFIG_EXFAT_WRITE_SB_INTERVAL_CSECS	(dirty_writeback_interval)
#endif

/*----------------------------------------------------------------------*/
/* Default Kconfig                                                      */
/*----------------------------------------------------------------------*/

#ifndef CONFIG_EXFAT_DEFAULT_CODEPAGE /* if Kconfig lacked codepage */
#define CONFIG_EXFAT_DEFAULT_CODEPAGE   437
#endif

#ifndef CONFIG_EXFAT_DEFAULT_IOCHARSET /* if Kconfig lacked iocharset */
#define CONFIG_EXFAT_DEFAULT_IOCHARSET  "utf8"
#endif

#ifndef CONFIG_EXFAT_FAT_MIRRORING /* if Kconfig lacked fat-mirroring option */
#define CONFIG_EXFAT_FAT_MIRRORING /* Write FAT 1, FAT 2 simultaneously */
#endif

#endif /* _EXFAT_CONFIG_H */

/* end of config.h */
