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

/************************************************************************/
/*                                                                      */
/*  PROJECT : exFAT & FAT12/16/32 File System                           */
/*  FILE    : exfat_api.c                                               */
/*  PURPOSE : exFAT volume lock layer                                   */
/*                                                                      */
/************************************************************************/

/*----------------------------------------------------------------------*/
/*  Internal structures                                                 */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Constant & Macro Definitions                                        */
/*----------------------------------------------------------------------*/
static DEFINE_MUTEX(_lock_core);

/*----------------------------------------------------------------------*/
/*  Global Variable Definitions                                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Local Variable Definitions                                          */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Local Function Declarations                                         */
/*----------------------------------------------------------------------*/

/*======================================================================*/
/*  Global Function Definitions                                         */
/*    - All functions for global use have same return value format,     */
/*      that is, 0 on success and minus error number on                 */
/*      various error condition.                                        */
/*======================================================================*/

/*----------------------------------------------------------------------*/
/*  exFAT Filesystem Init & Exit Functions                              */
/*----------------------------------------------------------------------*/

static s32 fsapi_init(void)
{
	return fscore_init();
}

static s32 fsapi_shutdown(void)
{
	return fscore_shutdown();
}

/*----------------------------------------------------------------------*/
/*  Volume Management Functions                                         */
/*----------------------------------------------------------------------*/

/* mount the file system volume */
static s32 fsapi_mount(struct super_block *sb)
{
	s32 err;

	/* acquire the core lock for file system ccritical section */
	mutex_lock(&_lock_core);

	err = meta_cache_init(sb);
	if (err)
		goto out;

	err = fscore_mount(sb);
out:
	if (err)
		meta_cache_shutdown(sb);

	/* release the core lock for file system critical section */
	mutex_unlock(&_lock_core);

	return err;
}

/* unmount the file system volume */
static s32 fsapi_umount(struct super_block *sb)
{
	s32 err;

	/* acquire the core lock for file system ccritical section */
	mutex_lock(&_lock_core);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_umount(sb);
	meta_cache_shutdown(sb);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));

	/* release the core lock for file system critical section */
	mutex_unlock(&_lock_core);

	return err;
}

/* get the information of a file system volume */
static s32 fsapi_statfs(struct super_block *sb, VOL_INFO_T *info)
{
	FS_INFO_T *fsi = &(EXFAT_SB(sb)->fsi);

	/* check the validity of pointer parameters */
	ASSERT(info);

	if (fsi->used_clusters == (u32) ~0) {
		s32 err;

		mutex_lock(&(EXFAT_SB(sb)->s_vlock));
		err = fscore_statfs(sb, info);
		mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
		return err;
	}

	info->ClusterSize = fsi->cluster_size;
	info->NumClusters = fsi->num_clusters - 2; /* clu 0 & 1 */
	info->UsedClusters = fsi->used_clusters + fsi->reserved_clusters;
	info->FreeClusters = info->NumClusters - info->UsedClusters;

	return 0;
}

/* synchronize a file system volume */
static s32 fsapi_sync_fs(struct super_block *sb, s32 do_sync)
{
	s32 err;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_sync_fs(sb, do_sync);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

static s32 fsapi_set_vol_flags(struct super_block *sb, u16 new_flag, s32 always_sync)
{
	s32 err;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_set_vol_flags(sb, new_flag, always_sync);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/*----------------------------------------------------------------------*/
/*  File Operation Functions                                            */
/*----------------------------------------------------------------------*/

/* lookup */
static s32 fsapi_lookup(struct inode *inode, u8 *path, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid && path);

	if (unlikely(!strlen(path)))
		return -EINVAL;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_lookup(inode, path, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* create a file */
static s32 fsapi_create(struct inode *inode, u8 *path, u8 mode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid && path);

	if (unlikely(!strlen(path)))
		return -EINVAL;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_create(inode, path, mode, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* read the target string of symlink */
static s32 fsapi_read_link(struct inode *inode, FILE_ID_T *fid, void *buffer, u64 count, u64 *rcount)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid && buffer);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_read_link(inode, fid, buffer, count, rcount);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* write the target string of symlink */
static s32 fsapi_write_link(struct inode *inode, FILE_ID_T *fid, void *buffer, u64 count, u64 *wcount)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid && buffer);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_write_link(inode, fid, buffer, count, wcount);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* resize the file length */
static s32 fsapi_truncate(struct inode *inode, u64 old_size, u64 new_size)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	TMSG("%s entered (inode %p size %llu)\n", __func__, inode, new_size);
	err = fscore_truncate(inode, old_size, new_size);
	TMSG("%s exitted (%d)\n", __func__, err);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* rename or move a old file into a new file */
static s32 fsapi_rename(struct inode *old_parent_inode, FILE_ID_T *fid,
		struct inode *new_parent_inode, struct dentry *new_dentry)
{
	s32 err;
	struct super_block *sb = old_parent_inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_rename(old_parent_inode, fid, new_parent_inode, new_dentry);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* remove a file */
static s32 fsapi_remove(struct inode *inode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_remove(inode, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* get the information of a given file */
static s32 fsapi_read_inode(struct inode *inode, DIR_ENTRY_T *info)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	TMSG("%s entered (inode %p info %p\n", __func__, inode, info);
	err = fscore_read_inode(inode, info);
	TMSG("%s exited (err:%d)\n", __func__, err);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* set the information of a given file */
static s32 fsapi_write_inode(struct inode *inode, DIR_ENTRY_T *info, int sync)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	TMSG("%s entered (inode %p info %p sync:%d\n",
			__func__, inode, info, sync);
	err = fscore_write_inode(inode, info, sync);
	TMSG("%s exited (err:%d)\n", __func__, err);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* return the cluster number in the given cluster offset */
static s32 fsapi_map_clus(struct inode *inode, u32 clu_offset, u32 *clu, int dest)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(clu);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	TMSG("%s entered (inode:%p clus:%08x dest:%d\n",
				__func__, inode, *clu, dest);
	err = fscore_map_clus(inode, clu_offset, clu, dest);
	TMSG("%s exited (clu:%08x err:%d)\n", __func__, *clu, err);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/*----------------------------------------------------------------------*/
/*  Directory Operation Functions                                       */
/*----------------------------------------------------------------------*/

/* create(make) a directory */
static s32 fsapi_mkdir(struct inode *inode, u8 *path, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid && path);

	if (unlikely(!strlen(path)))
		return -EINVAL;

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_mkdir(inode, path, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* read a directory entry from the opened directory */
static s32 fsapi_readdir(struct inode *inode, DIR_ENTRY_T *dir_entry)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(dir_entry);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_readdir(inode, dir_entry);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* remove a directory */
static s32 fsapi_rmdir(struct inode *inode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid);

	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_rmdir(inode, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* unlink a file.
 * that is, remove an entry from a directory. BUT don't truncate
 */
static s32 fsapi_unlink(struct inode *inode, FILE_ID_T *fid)
{
	s32 err;
	struct super_block *sb = inode->i_sb;

	/* check the validity of pointer parameters */
	ASSERT(fid);
	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	err = fscore_unlink(inode, fid);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return err;
}

/* reflect the internal dirty flags to VFS bh dirty flags */
static s32 fsapi_cache_flush(struct super_block *sb, int do_sync)
{
	mutex_lock(&(EXFAT_SB(sb)->s_vlock));
	fcache_flush(sb, do_sync);
	dcache_flush(sb, do_sync);
	mutex_unlock(&(EXFAT_SB(sb)->s_vlock));
	return 0;
}

static u32 fsapi_get_au_stat(struct super_block *sb, s32 mode)
{
	/* volume lock is not required */
	return fscore_get_au_stat(sb, mode);
}

/* clear extent cache */
static void fsapi_invalidate_extent(struct inode *inode)
{
	/* Volume lock is not required,
	 * because it is only called by evict_inode.
	 * If any other function can call it,
	 * you should check whether volume lock is needed or not.
	 */
	extent_cache_inval_inode(inode);
}

/* check device is ejected */
static s32 fsapi_check_bdi_valid(struct super_block *sb)
{
	return fscore_check_bdi_valid(sb);
}

/* end of exfat_api.c */
