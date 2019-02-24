#include <gtkmm.h>
#include "window.h"

int draw(int argc, char* argv[]){
    auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

    Gtk::Window window;
    window.set_default_size(200, 200);

  return app->run(window);
}