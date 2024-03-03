#pragma once

#include <glibmm/objectbase.h>
#include <glibmm/refptr.h>

namespace AppScanConf {
    template<class _T, class ...Args>
    Glib::RefPtr<_T> make_refptr(Args&& ...args) noexcept {
        static_assert(std::is_base_of_v<Glib::ObjectBase, _T>);
        return Glib::make_refptr_for_instance(new _T(args...));
    }
}