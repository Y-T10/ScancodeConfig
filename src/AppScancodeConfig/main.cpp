#include <gtkmm.h>

#include "AscWindowMapping.hpp"

using namespace AppScanConf;

int main(int argc, char* argv[]) {
    Glib::set_init_to_users_preferred_locale(false);
    const auto app = Gtk::Application::create("gtkmm.win.scanmap");
    return app->make_window_and_run<MainWindow>(argc, argv);
}
