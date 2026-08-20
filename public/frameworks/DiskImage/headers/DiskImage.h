// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_DI_DISKIMAGE_H
#define FW_HEADERS_DI_DISKIMAGE_H

#include <SystemKit/System.h>

#ifndef __DISK_IMAGE_CDROM__
#define kDISectorSz (512)
#else
#define kDISectorSz (2048)
#endif  // __DISK_IMAGE_CDROM__

#define kDIMinDiskSz mib_cast(1)
#define kDIDefaultOutputName "disk.eimg"
#define kDIDefaultDiskName "Disk"
#define kDISuccessStatus (0)
#define kDIFailureStatus (1)

#define kDIDiskNameLen (16)
#define kDIOutNameLen (256)

namespace DI {
  
/// @brief Disk Image file structure.
/// @param disk_name Disk partition name.
/// @param sector_sz Disk sector_sz.
/// @param block_cnt Disk block count.
/// @param disk_sz Disk size.
/// @param out_name Output file name.
struct DI_DISK_IMAGE _FINAL {
  Char   disk_name[kDIDiskNameLen] = kDIDefaultDiskName;
  SInt32 sector_sz                 = kDISectorSz;
  SInt32 block_cnt                 = 0;
  SizeT  disk_sz                   = kDIMinDiskSz;
  Char   out_name[kDIOutNameLen]   = kDIDefaultOutputName;
  SInt32 fs_version                = 0UL;
};

/// @brief Format with an EPM partition.
/// @param img disk image structure.
/// @return Status code upon completion.
SInt32 DIFormatPartitionEPM(struct DI_DISK_IMAGE& img);

/// @brief NeFS format over EPM.
/// @param img disk image structure.
/// @return Status code upon completion.
SInt32 DIFormatFilesystemNeFS(struct DI_DISK_IMAGE& img);

/// @brief OpenHeFS format over EPM.
/// @param img disk image structure.
/// @return Status code upon completion.
SInt32 DIFormatFilesystemOpenHeFS(struct DI_DISK_IMAGE& img);

}  // namespace DI

#endif
