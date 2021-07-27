#include "AssetLoader.hpp"

int Me::AssetLoader::Initialize() {
    return 0;
}

void Me::AssetLoader::Finalize() {
    m_strSearchPath.clear();
}

void Me::AssetLoader::Tick() {

}

bool Me::AssetLoader::AddSearchPath(const char *path) {
    std::vector<std::string>::iterator src = m_strSearchPath.begin();

    while (src != m_strSearchPath.end()) {
        if (!(*src).compare(path)) {
            return true;
        }
        src++;
    }

    m_strSearchPath.push_back(path);
    return true;
}

bool Me::AssetLoader::RemoveSearchPath(const char *path) {
    std::vector<std::string>::iterator src = m_strSearchPath.begin();

    while (src != m_strSearchPath.end()) {
        if (!(*src).compare(path)) {
            m_strSearchPath.erase(src);
            return true;
        }
        src++;
    }

    return true;
}

bool Me::AssetLoader::FileExists(const char *filePath) {
    AssetFilePtr fp = OpenFile(filePath, ME_OPEN_BINARY);
    if (fp != nullptr) {
        CloseFile(fp);
        return true;
    }
    return false;
}

Me::AssetLoader::AssetFilePtr Me::AssetLoader::OpenFile(const char *name, Me::AssetLoader::AssetOpenMode mode) {
    FILE *fp = nullptr;
    std::string upPath;
    std::string fullPath;

    for (int32_t i = 0; i < 10; ++i) {
        std::vector<std::string>::iterator src = m_strSearchPath.begin();
        bool looping = true;
        while (looping) {
            fullPath.assign(upPath); // reset to current upPath.
            if (src != m_strSearchPath.end()) {
                fullPath.append(*src);
                fullPath.append("/Asset/");
                src++;
            } else {
                fullPath.append("Asset/");
                looping = false;
            }
            fullPath.append(name);
#ifdef _DEBUG
            fprintf(stderr, "Trying to open %s\n", fullPath.c_str());
#endif
            switch (mode) {
                case ME_OPEN_TEXT:
                    fp = fopen(fullPath.c_str(), "r");
                    break;
                case ME_OPEN_BINARY:
                    fp = fopen(fullPath.c_str(), "rb");
                    break;
            }

            if (fp) {
                return (AssetFilePtr) fp;
            }
        }
        upPath.append("../");
    }
    return nullptr;
}

Me::Buffer Me::AssetLoader::SyncOpenAndReadText(const char *filePath) {
    AssetFilePtr fp = OpenFile(filePath, ME_OPEN_TEXT);
    Buffer *pBuff = nullptr;

    if (fp) {
        size_t length = GetSize(fp);

        pBuff = new Buffer(length + 1);
        fread(pBuff->m_pData, length, 1, static_cast<FILE *>(fp));
        pBuff->m_pData[length] = '\0';

        CloseFile(fp);

#ifdef _DEBUG
        fprintf(stderr, "Read file '%s', %d bytes\n", filePath, (int) length);
#endif
    } else {
        fprintf(stderr, "Error opening file '%s'\n", filePath);
        pBuff = new Buffer();
    }

    return *pBuff;
}

Me::Buffer Me::AssetLoader::SyncOpenAndReadBinary(const char *filePath) {
    AssetFilePtr fp = OpenFile(filePath, ME_OPEN_BINARY);
    Buffer *pBuff = nullptr;

    if (fp) {
        size_t length = GetSize(fp);

        pBuff = new Buffer(length);
        fread(pBuff->m_pData, length, 1, static_cast<FILE *>(fp));
        pBuff->m_pData[length] = '\0';

        CloseFile(fp);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", filePath);
        pBuff = new Buffer();
    }

#ifdef DEBUG
    fprintf(stderr, "Read file '%s', %d bytes\n", filePath, length);
#endif

    return *pBuff;
}

size_t Me::AssetLoader::SyncRead(Me::AssetLoader::AssetFilePtr const &fp, Me::Buffer &buf) {
    size_t sz;
    if (!fp) {
        fprintf(stderr, "null file descriptor\n");
        return 0;
    }

    sz = fread(buf.m_pData, buf.m_szSize, 1, static_cast<FILE *>(fp));

#ifdef DEBUG
    fprintf(stderr, "Read file '%s', %d bytes\n", filePath, length);
#endif

    return sz;
}

void Me::AssetLoader::CloseFile(Me::AssetLoader::AssetFilePtr &fp) {
    fclose((FILE *) fp);
    fp = nullptr;
}

size_t Me::AssetLoader::GetSize(Me::AssetLoader::AssetFilePtr const &fp) {
    FILE *_fp = static_cast<FILE *>(fp);

    long pos = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    size_t length = ftell(_fp);
    fseek(_fp, pos, SEEK_SET);

    return length;
}

int32_t Me::AssetLoader::Seek(Me::AssetLoader::AssetFilePtr fp, long offset, Me::AssetLoader::AssetSeekBase where) {
    return fseek(static_cast<FILE * >(fp), offset, static_cast<int>(where));
}
