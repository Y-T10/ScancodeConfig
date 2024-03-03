#pragma once

#include <glibmm/object.h>
#include "CsmViewer.hpp"

namespace AppScanConf {
    struct ScanMapping : public Glib::Object {
        using value_type = CompScanMap::Scancode;
        value_type keySrc;
        value_type keyDst;

        ScanMapping(const value_type& src = 0x00, const value_type& dst = 0x00) noexcept;
    };
};