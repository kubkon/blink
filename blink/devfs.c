/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2023 Trung Nguyen                                                  │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "blink/devfs.h"

#include <stdlib.h>
#include <string.h>

#include "blink/errno.h"
#include "blink/hostfs.h"
#include "blink/log.h"

#ifndef DISABLE_VFS

static int DevfsInit(const char *source, u64 flags, const void *data,
                     struct VfsDevice **device, struct VfsMount **mount) {
  int ret;
  if (source == NULL) {
    return efault();
  }
  if (*source == '\0') {
    source = "/dev";
  }
  VFS_LOGF("real devfs not implemented, delegating to hostfs");
  if ((ret = HostfsInit(source, flags, data, device, mount)) != -1) {
    (*device)->ops = &g_devfs.ops;
  }
  return ret;
}

static int DevfsReadmountentry(struct VfsDevice *device, char **spec,
                               char **type, char **mntops) {
  *spec = strdup("none");
  if (*spec == NULL) {
    return enomem();
  }
  *type = strdup("devtmpfs");
  if (type == NULL) {
    free(*spec);
    return enomem();
  }
  *mntops = NULL;
  return 0;
}

struct VfsSystem g_devfs = {.name = "devfs",
                            .nodev = true,
                            .ops = {
                                .Init = DevfsInit,
                                .Freeinfo = HostfsFreeInfo,
                                .Freedevice = HostfsFreeDevice,
                                .Finddir = HostfsFinddir,
                                .Readmountentry = DevfsReadmountentry,
                                .Readlink = HostfsReadlink,
                                .Mkdir = HostfsMkdir,
                                .Mkfifo = HostfsMkfifo,
                                .Open = HostfsOpen,
                                .Access = HostfsAccess,
                                .Stat = HostfsStat,
                                .Fstat = HostfsFstat,
                                .Chmod = HostfsChmod,
                                .Fchmod = HostfsFchmod,
                                .Chown = HostfsChown,
                                .Fchown = HostfsFchown,
                                .Ftruncate = HostfsFtruncate,
                                .Link = HostfsLink,
                                .Unlink = HostfsUnlink,
                                .Read = HostfsRead,
                                .Write = HostfsWrite,
                                .Pread = HostfsPread,
                                .Pwrite = HostfsPwrite,
                                .Readv = HostfsReadv,
                                .Writev = HostfsWritev,
                                .Preadv = HostfsPreadv,
                                .Pwritev = HostfsPwritev,
                                .Seek = HostfsSeek,
                                .Fsync = HostfsFsync,
                                .Fdatasync = HostfsFdatasync,
                                .Flock = HostfsFlock,
                                .Fcntl = HostfsFcntl,
                                .Ioctl = HostfsIoctl,
                                .Dup = HostfsDup,
#ifdef HAVE_DUP3
                                .Dup3 = HostfsDup3,
#endif
                                .Poll = HostfsPoll,
                                .Opendir = HostfsOpendir,
#ifdef HAVE_SEEKDIR
                                .Seekdir = HostfsSeekdir,
                                .Telldir = HostfsTelldir,
#endif
                                .Readdir = HostfsReaddir,
                                .Rewinddir = HostfsRewinddir,
                                .Closedir = HostfsClosedir,
                                .Bind = HostfsBind,
                                .Connect = HostfsConnect,
                                .Connectunix = HostfsConnectUnix,
                                .Accept = HostfsAccept,
                                .Listen = HostfsListen,
                                .Shutdown = HostfsShutdown,
                                .Recvmsg = HostfsRecvmsg,
                                .Sendmsg = HostfsSendmsg,
                                .Recvmsgunix = HostfsRecvmsgUnix,
                                .Sendmsgunix = HostfsSendmsgUnix,
                                .Getsockopt = HostfsGetsockopt,
                                .Setsockopt = HostfsSetsockopt,
                                .Getsockname = HostfsGetsockname,
                                .Getpeername = HostfsGetpeername,
                                .Rename = HostfsRename,
                                .Utime = HostfsUtime,
                                .Futime = HostfsFutime,
                                .Symlink = HostfsSymlink,
                                .Mmap = HostfsMmap,
                                .Munmap = HostfsMunmap,
                                .Mprotect = HostfsMprotect,
                                .Msync = HostfsMsync,
                                .Pipe = NULL,
#ifdef HAVE_PIPE2
                                .Pipe2 = NULL,
#endif
                                .Socket = NULL,
                                .Socketpair = NULL,
                                .Tcgetattr = NULL,
                                .Tcsetattr = NULL,
                                .Tcflush = NULL,
                                .Tcdrain = NULL,
                                .Tcsendbreak = NULL,
                                .Tcflow = NULL,
                                .Tcgetsid = NULL,
                                .Tcgetpgrp = NULL,
                                .Tcsetpgrp = NULL,
#ifdef HAVE_SOCKATMARK
                                .Sockatmark = NULL,
#endif
                                .Fexecve = NULL,
                            }};

#endif /* DISABLE_VFS */
