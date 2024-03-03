#pragma once

#include <gtkmm/columnview.h>
#include <gtkmm/window.h>

namespace AppScanConf {
    struct MainWindow : public Gtk::Window {
        MainWindow();

        Gtk::ColumnView view;
        Glib::RefPtr<Gtk::ColumnViewColumn> colSrc;
        Glib::RefPtr<Gtk::ColumnViewColumn> colDst;
        Glib::RefPtr<Gio::ListModel> map;
    };
}