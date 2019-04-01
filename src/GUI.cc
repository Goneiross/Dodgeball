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

class MyArea: public Gtk::DrawingArea {
  public:
    MyArea(){};
    virtual ~MyArea(){};
    void clear();
    void draw();
  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  private:
    bool empty;
    void refresh();
};

void MyArea::clear(){
  empty= true;
  refresh();
}
void MyArea::draw(){
  empty= false;
  refresh();
}
void MyArea::refresh(){
  auto win = get_window();
  if(win){
    Gdk::Rectangle r(0,0, get_allocation().get_width(), get_allocation().get_height());
    win->invalidate_rect(r,false);
  }
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
  if(not empty) {
    cr->set_line_width(50.0);

    cr->set_source_rgb(0.92, 0.0, 0.0);
    cr->move_to(50, 50);
    cr->line_to(50, 150);
    cr->stroke();
    cr->move_to(50, 200);
    cr->line_to(50, 300);
    cr->stroke();
    cr->move_to(50, 275);
    cr->line_to(190, 275);
    cr->stroke();
    cr->move_to(73, 175);
    cr->line_to(190, 175);
    cr->stroke();
    cr->move_to(75, 75);
    cr->line_to(190, 75);
    cr->stroke();
  
    cr->move_to(250, 50);
    cr->line_to(250, 300);
    cr->stroke();
    cr->move_to(275, 75);
    cr->line_to(325, 75);
    cr->stroke();
    cr->move_to(275, 175);
    cr->line_to(325, 175);
    cr->stroke();
    cr->arc(325, 125, 50, -M_PI/2, M_PI/2);
    cr->stroke();
  
    cr->move_to(450, 50);
    cr->line_to(450, 150);
    cr->stroke();
    cr->move_to(450, 200);
    cr->line_to(450, 300);
    cr->stroke();
    cr->move_to(450, 75);
    cr->line_to(590, 75);
    cr->stroke();
    cr->move_to(473, 175);
    cr->line_to(575, 175);
    cr->stroke();

    cr->move_to(650, 50);
    cr->line_to(650, 300);
    cr->stroke();
    cr->move_to(650, 275);
    cr->line_to(790, 275);
    cr->stroke();
  } else { std::cout << "Empty !" << std::endl; }
  return true;
}

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
    MyArea m_area;
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

  m_box1.set_size_request(100, 100);

  add(m_box_top);
  m_box_top.pack_start(m_box1);
  m_box_top.pack_start(m_box2);

  m_area.set_size_request(200,200);
  m_box_top.pack_start(m_area);

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

