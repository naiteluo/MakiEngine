#pragma once

#include <cstdio>
#include <string>
#include <utility>
#include <vector>
#include "IRuntimeModule.hpp"
#include "Buffer.hpp"

namespace Me {
    class AssetLoader : public IRuntimeModule {
    public:
        virtual ~AssetLoader() {};

        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

        typedef void *AssetFilePtr;

        enum AssetOpenMode {
            ME_OPEN_TEXT = 0, /// Open In Text Mode
            ME_OPEN_BINARY = 1, /// Open In Binary Mode
        };

        enum AssetSeekBase {
            ME_SEEK_SET = 0, /// SEEK_SET
            ME_SEEK_CUR = 1, /// SEEK_CUR
            ME_SEEK_END = 2, /// SEEK_END
        };

        bool AddSearchPath(const char *path);

        bool RemoveSearchPath(const char *path);

        bool FileExists(const char *filePath);

        AssetFilePtr OpenFile(const char *name, AssetOpenMode mode);

        Buffer SyncOpenAndReadText(const char *filePath);

        Buffer SyncOpenAndReadBinary(const char *filePath);

        size_t SyncRead(const AssetFilePtr &fp, Buffer &buf);

        void CloseFile(AssetFilePtr &fp);

        size_t GetSize(const AssetFilePtr &fp);

        int32_t Seek(AssetFilePtr fp, long offset, AssetSeekBase where);

        inline std::string SyncOpenAndReadTextFileToString(const char *fileName) {
            std::string result;
            Buffer buffer = SyncOpenAndReadText(fileName);
            char *content = reinterpret_cast<char *>(buffer.m_pData);

            if (content) {
                result = std::string(std::move(content));
            }

            return result;
        }

    private:
        std::vector<std::string> m_strSearchPath;
    };

    extern AssetLoader *g_pAssetLoader;
}