#include "DiskUtils.h"
#include <shellapi.h>
#include <memory>

bool DiskUtils::GetLogicalDriveList(std::vector<DString>& driveList)
{
    const int32_t maxBufLen = 1022;
    DString::value_type tempBuf[maxBufLen + 2];
    DWORD dwSize = ::GetLogicalDriveStrings(maxBufLen, tempBuf);
    if (dwSize == 0) {
        return false;
    }
    const DString::value_type* driveStr = tempBuf;
    std::unique_ptr<DString::value_type> spBuf;
    if (dwSize > maxBufLen) {
        DString::value_type* szBuf = new DString::value_type[dwSize + 2];
        spBuf.reset(szBuf);
        DWORD dwRetSize = ::GetLogicalDriveStrings(dwSize, szBuf);
        if ( (dwRetSize == 0) || (dwRetSize > dwSize) )
        {
            return false;
        }
        szBuf[dwRetSize] = 0;
        szBuf[dwRetSize + 1] = 0;
        
        dwSize = dwRetSize;
        driveStr = szBuf;
    }
    else {
        tempBuf[dwSize] = 0;
        tempBuf[dwSize + 1] = 0;
    }

    DWORD begin = 0;
    for (DWORD i = 0; i < dwSize; ++i) {
        if (driveStr[i] == _T('\0') && begin != i) {
            driveList.push_back(driveStr + begin);
            begin = i + 1;
        }
    }
    return true;
}

bool DiskUtils::GetLogicalDriveInfo(const DString& driveString, DiskInfo& diskInfo)
{ 
    HMODULE hShell32Dll = ::LoadLibraryW(_T("Shell32.dll"));
    ASSERT(hShell32Dll != NULL);
    if (hShell32Dll == NULL) {
        return false;
    }

    typedef DWORD_PTR (*PFNSHGetFileInfo)( LPCWSTR pszPath, DWORD dwFileAttributes, SHFILEINFOW * psfi,
                                           UINT cbFileInfo, UINT uFlags);

    PFNSHGetFileInfo pfnSHGetFileInfo = (PFNSHGetFileInfo)::GetProcAddress(hShell32Dll, "SHGetFileInfoW");
    ASSERT(pfnSHGetFileInfo != NULL);

    if (pfnSHGetFileInfo == NULL) {
        return false;
    }

    DiskInfo currentDiskInfo;
    SHFILEINFO shellInfo = {0};
    DWORD_PTR result = pfnSHGetFileInfo(driveString.c_str(),  0, &shellInfo, sizeof(shellInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME| SHGFI_TYPENAME);
    if (result == 0) {
        return false;
    }
    currentDiskInfo.m_volumeName = shellInfo.szDisplayName;
    currentDiskInfo.m_volumeType = shellInfo.szTypeName;
    
    DString::value_type volumeNameBuffer[MAX_PATH + 1] = {0};
    DWORD volumeNameSize = MAX_PATH;
    DWORD volumeSerialNumber = 0;
    DWORD maximumComponentLength = 0;
    DWORD fileSystemFlags = 0;
    DString::value_type fileSystemNameBuffer[MAX_PATH + 1] = {0};
    DWORD fileSystemNameSize = MAX_PATH;

    if (::GetVolumeInformation( driveString.c_str(), 
                                volumeNameBuffer, 
                                volumeNameSize, 
                                &volumeSerialNumber, 
                                &maximumComponentLength, 
                                &fileSystemFlags, 
                                fileSystemNameBuffer, 
                                fileSystemNameSize) != FALSE) {
        fileSystemNameBuffer[MAX_PATH] = 0;
        currentDiskInfo.m_fileSystem = fileSystemNameBuffer;
        currentDiskInfo.m_hasFileSystem = true;
        
        DWORD dSectorsPerCluster = 0;
        DWORD dBytesPerSector = 0;
        DWORD dNumberOfFreeClusters = 0;
        DWORD dTotalNumberOfClusters = 0;
        if (::GetDiskFreeSpace(driveString.c_str(), &dSectorsPerCluster, &dBytesPerSector, &dNumberOfFreeClusters, &dTotalNumberOfClusters)) {
            currentDiskInfo.m_clusterBytes = dSectorsPerCluster * dBytesPerSector;
            currentDiskInfo.m_totalBytes   = ((uint64_t)dTotalNumberOfClusters) * dSectorsPerCluster * dBytesPerSector;
            currentDiskInfo.m_freeBytes    = ((uint64_t)dNumberOfFreeClusters)  * dSectorsPerCluster * dBytesPerSector;
        }
    }
    else {
        currentDiskInfo.m_clusterBytes = 0;
        currentDiskInfo.m_totalBytes = 0;
        currentDiskInfo.m_freeBytes = 0;
        currentDiskInfo.m_hasFileSystem = false;
    }
    diskInfo = currentDiskInfo;
    return true;
}

DString DiskUtils::GetDriveFromDirectoryPath(DString path) 
{
    if ( (path.size() < 2)) {
        return DString();
    }
    if ( (path[1] != _T(':'))) {
        return DString();
    }    
    return path.substr(0, 3);
}

DString DiskUtils::GetMaxFreeSpaceLocalDisk()
{
    uint64_t freeBytes = 0;
    DString maxFreedrive;
    DString defaultPath;
    std::vector<DString> driveList;
    GetLogicalDriveList(driveList);
    for (size_t i = 0; i < driveList.size(); ++i) {
        DString drive = driveList[i];
        UINT uType = ::GetDriveType(drive.c_str());
        if (uType != DRIVE_FIXED) {
            //只需要本地磁盘
            continue;
        }

        DiskInfo diskInfo;
        bool bRet = GetLogicalDriveInfo(drive, diskInfo);
        if (!bRet) {
            continue;
        }

        if (diskInfo.m_freeBytes > freeBytes) {
            freeBytes = diskInfo.m_freeBytes;
            maxFreedrive = drive;
        }
    }
    return maxFreedrive;
}

uint64_t DiskUtils::GetFreeDiskSpace(const DString& fullDirectory)
{
    uint64_t freeSize = 0;
    DString drive = GetDriveFromDirectoryPath(fullDirectory);
    if (!drive.empty()) {
        DiskInfo diskInfo;
        GetLogicalDriveInfo(drive, diskInfo);
        freeSize = diskInfo.m_freeBytes;
    }
    return freeSize;
}
