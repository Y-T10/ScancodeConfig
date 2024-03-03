#include <cstdint>
#include <gtkmm.h>
#include <memory>

#include "AscScanMap.hpp"

using namespace AppScanConf;

struct MainWindow : public Gtk::Window {
    MainWindow();

    Gtk::ColumnView view;
    Glib::RefPtr<Gtk::ColumnViewColumn> colSrc;
    Glib::RefPtr<Gtk::ColumnViewColumn> colDst;
    Glib::RefPtr<Gio::ListModel> map;
};

const Glib::RefPtr<Gio::ListModel> CreateSampleList() {
    auto newList = Gio::ListStore<ScanMapping>::create();
    for(uint16_t i = 0; i < 5; i++) {
        newList->append(ScanMapping::create(i, 0x00));
    }
    return newList;
}

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

int main(int argc, char* argv[]) {
    Glib::set_init_to_users_preferred_locale(false);
    const auto app = Gtk::Application::create("gtkmm.win.scanmap");
    return app->make_window_and_run<MainWindow>(argc, argv);
}
