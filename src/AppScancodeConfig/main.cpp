#include <gtkmm.h>

struct MainWindow : public Gtk::Window {
    MainWindow();
};

MainWindow::MainWindow() {
    set_title("Scancode Mapping");
    set_default_size(800, 600);
};

int main(int argc, char* argv[]) {
    Glib::set_init_to_users_preferred_locale(false);
    const auto app = Gtk::Application::create("gtkmm.win.scanmap");
    return app->make_window_and_run<MainWindow>(argc, argv);
}