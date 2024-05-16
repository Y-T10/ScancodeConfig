#pragma once

#include "AsconfRegistry.hpp"

#include "CregHandler.hpp"
#include "CsmCodec.hpp"

const CompScanMap::MappingList AppSacnConf::ReadScancodeMap() noexcept {
    const auto RegKey = CompReg::OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
        KEY_READ
    );
    if (!RegKey) {
        return {};
    }
    const auto RegValue = ReadKeyValueBin(RegKey, TEXT("Scancode Map"));
    if (!RegValue) {
        return {};
    }
    const auto MappingList = CompScanMap::DecodeScancodeMap(*RegValue);
    return MappingList.has_value() ? *MappingList : CompScanMap::MappingList{};
}