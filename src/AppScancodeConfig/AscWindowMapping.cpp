#include "AscWindowMapping.hpp"

#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/noselection.h>
#include <giomm/liststore.h>
#include <gtkmm/label.h>

#include "AscScanMap.hpp"

namespace {
    using namespace AppScanConf;
    const Glib::RefPtr<Gio::ListModel> CreateSampleList() {
        auto newList = Gio::ListStore<ScanMapping>::create();
        for(uint16_t i = 0; i < 5; i++) {
            newList->append(ScanMapping::create(i, 0x00));
        }
        return newList;
    }
}

namespace AppScanConf {
MainWindow::MainWindow() {
    set_title("Scancode Mapping");
    set_default_size(800, 600);
    map = CreateSampleList();
    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem>& list_item){
        Gtk::Label* label = new Gtk::Label("empty");
        list_item->set_child(*label);
    });
    factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem>& list_item){
        auto pair = std::dynamic_pointer_cast<ScanMapping>(list_item->get_item());
        auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
        if (!label || !pair) {
            return;
        }
        label->set_text(Glib::ustring::format(std::hex, pair->keySrc));
    });
    colSrc = Gtk::ColumnViewColumn::create("変換元", factory);
    colDst = Gtk::ColumnViewColumn::create("変換先", factory);
    view.append_column(colSrc);
    view.append_column(colDst);
    view.set_model(Gtk::NoSelection::create(map));
    set_child(view);
};
}