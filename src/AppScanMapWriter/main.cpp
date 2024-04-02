
#include "CregAdmin.hpp"
#include "CproHandle.hpp"
#include "CregHandler.hpp"
#include "CregTypes.hpp"
#include "CprocPipe.hpp"
#include "CsmCodec.hpp"
#include "CsmViewer.hpp"

#include <cstdint>
#include <cstdio>
#include <vector>

const CmpProc::object_handle OpenNamedPipe(const CompReg::win32str& pipeName) noexcept {
    const auto Pipe = CreateFile(
        pipeName.c_str(),
        GENERIC_READ, 0,
        NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (Pipe == INVALID_HANDLE_VALUE) {
        return nullptr;
    }
    return CmpProc::object_handle(Pipe);
}

const std::expected<CompScanMap::MappingList, DWORD> GetMappingFromPipe(const CompReg::win32str& pipeName) noexcept {
    // TODO: UNICODEにも対応する
    const auto Pipe = OpenNamedPipe(pipeName);
    if (!Pipe) {
        return std::unexpected{GetLastError()};
    }

    const auto Data = CmpProc::ReadPipe(Pipe);
    if (!Data) {
        return std::unexpected{Data.error()};
    }

    const auto ToMap = [](const std::vector<uint8_t>& bin){
        const size_t MapSize = bin.size() / sizeof(CompScanMap::MappingList::value_type);
        auto map = CompScanMap::MappingList(MapSize, CompScanMap::ScanMapping{});
        memcpy(map.data(), bin.data(), bin.size());
        return map;
    };

    return ToMap(*Data);
}

int main(int argc, char* argv[]) {
    const auto resutl = CompReg::IsElevated();
    if (!resutl) {
        return GetLastError();
    }

    if (!resutl.value()) {
        return 1;
    }

    if (argc < 2) {
        return 2;
    }

    const auto SrcMapping = GetMappingFromPipe(argv[1]);
    if (!SrcMapping) {
        return SrcMapping.error();
    }
    const auto ScanMapValue = CompScanMap::EncodeScancodeMap(*SrcMapping);
    if (!ScanMapValue) {
        return 1;
    }

    const auto ScanMapKey = CompReg::OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
        KEY_SET_VALUE
    );
    if (!ScanMapKey) {
        return GetLastError();
    }
    WriteKeyValueBin(ScanMapKey, TEXT("Scancode Map"), *ScanMapValue);

    return 0;
}