#include <gtkmm.h>
#include "GUI.h"

using namespace Gtk;

int draw(int argc, char* argv[]){
    auto app =
    Application::create(argc, argv,
      "org.gtkmm.examples.base");

    Window window;
    window.set_default_size(400, 400);

  return app->run(window);
}