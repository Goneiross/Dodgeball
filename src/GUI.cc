/*!
  \file   GUI.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "GUI" module implementation
*/

#include <gtkmm.h>
#include "GUI.h"
#include <iostream>

using namespace Gtk;

class GUI: public Window {
  public:
    GUI();
    virtual ~GUI(); 
  protected: //Or private ?
    void on_button_clicked_exit();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_start();
    void on_button_clicked_step();
    Gtk::Box m_box_top, m_box1, m_box2;
    Button m_button_exit;
    Button m_button_open;
    Button m_button_save;
    Button m_button_start;
    Button m_button_step;
    Label m_label_status;
};

GUI::GUI(): 
  m_box_top(Gtk::ORIENTATION_VERTICAL),
  m_box1(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_box2(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_button_exit("Exit"),
  m_button_open("Open"),
  m_button_save("Save"),
  m_button_start("Start"),
  m_button_step("Step"),
  m_label_status("Initiaization") {
  
  set_title("DodgeBall");
  set_border_width(10);
  maximize();

  add(m_box_top);
  m_box_top.pack_start(m_box1);
  m_box_top.pack_start(m_box2);

  m_box1.pack_start(m_button_exit);
  m_box1.pack_start(m_button_open);
  m_box1.pack_start(m_button_save);
  m_box1.pack_start(m_button_start);
  m_box1.pack_start(m_button_step);
  m_box1.pack_start(m_label_status);

  m_button_exit.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_exit));
  m_button_open.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_open));
  m_button_save.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_save));
  m_button_start.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_start));
  m_button_step.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_step));
   
  show_all_children();
}
GUI::~GUI(){}
void GUI::on_button_clicked_exit(){ exit(0); }
void GUI::on_button_clicked_open(){ exit(0); }
void GUI::on_button_clicked_save(){ exit(0); }
void GUI::on_button_clicked_start(){ exit(0); }
void GUI::on_button_clicked_step(){ exit(0); }

int draw(){
    auto app = Application::create();

    GUI mainWindow;

  return app->run(mainWindow);
}

