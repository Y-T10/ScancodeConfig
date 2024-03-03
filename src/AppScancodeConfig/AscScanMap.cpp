#include "AscScanMap.hpp"

#include "AscUtility.hpp"

namespace AppScanConf {
    Glib::RefPtr<ScanMapping> ScanMapping::create(const value_type& src, const value_type& dst) noexcept {
        return make_refptr<ScanMapping>(src, dst);
    };

    ScanMapping::ScanMapping(const value_type& src, const value_type& dst) noexcept:
    keySrc(src),
    keyDst(dst){
    };
};