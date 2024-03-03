#include "AscScanMap.hpp"

namespace AppScanConf {
    ScanMapping::ScanMapping(const value_type& src, const value_type& dst) noexcept:
    keySrc(src),
    keyDst(dst){
    };
};