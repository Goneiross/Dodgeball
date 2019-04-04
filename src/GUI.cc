/*!
  \file   GUI.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "GUI" module implementation
*/

#include <gtkmm.h>
#include <glibmm/main.h>
#include "GUI.h"
#include <iostream>
#include "simulation.h"
#include "define.h"

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "player.h"
#include "ball.h"
#include "map.h"
#endif

using namespace Gtk;

class MyArea: public Gtk::DrawingArea {
  public:
    MyArea(PlayerMap* p, BallMap* b, Map* m){
      players = p;
      balls = b;
      mainMap = m;
    };
    virtual ~MyArea(){};
    void clear();
    void draw();
  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  private:
    bool empty;
    void refresh();
    PlayerMap* players;
    BallMap* balls;
    Map* mainMap;
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
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    const int lesser = MIN(width, height);

    int xc, yc;
    xc = width / 2;
    yc = height / 2;
    
    int nbPlayer = players->getNb();
    for (int p = 0; p < nbPlayer; p++){
      Player* player = players->getPlayer(p);

      player->setGX(width / 2 + player->getX()); //PUT ELSEWHERE !
      player->setGY(height / 2 - player->getY());
      cr->save();
      cr->arc(player->getGX(), player->getGY(), (player->getRadius() / SIDE) * lesser, 0.0, 2.0 * M_PI); // full circle
      cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // partially translucent
      cr->fill_preserve();
      cr->restore();  // back to opaque black
      cr->stroke();
    }


    
  } else { std::cout << "Empty !" << std::endl; }
  return true;
}

class GUI: public Window {
  public:
    GUI(PlayerMap* players, BallMap* balls, Map* mainMap);
    virtual ~GUI(); 
  protected: //Or private ?
    void on_button_clicked_exit();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_start();
    void on_button_clicked_step();
    bool on_timeout();
    Box m_box_top, m_box1, m_box2;
    Button m_button_exit;
    Button m_button_open;
    Button m_button_save;
    Button m_button_start;
    Button m_button_step;
    Label m_label_status;
    MyArea m_area;
};

GUI::GUI(PlayerMap* players, BallMap* balls, Map* mainMap): 
  m_box_top(Gtk::ORIENTATION_VERTICAL),
  m_box1(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_box2(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_button_exit("Exit"),
  m_button_open("Open"),
  m_button_save("Save"),
  m_button_start("Start"),
  m_button_step("Step"),
  m_label_status("Initiaization"),
  m_area(players, balls, mainMap){
  
  set_title("DodgeBall");
  set_border_width(0);

  m_box1.set_size_request(10, 10);

  add(m_box_top);
  m_box_top.pack_start(m_box1, false, false);
  m_box_top.pack_start(m_box2);

  m_area.set_size_request(200,200);
  m_box2.pack_start(m_area);

  m_box1.pack_start(m_button_exit, false, false);
  m_box1.pack_start(m_button_open, false, false);
  m_box1.pack_start(m_button_save, false, false);
  m_box1.pack_start(m_button_start, false, false);
  m_box1.pack_start(m_button_step, false, false);
  m_box1.pack_start(m_label_status, false, false);

  m_button_exit.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_exit));
  m_button_open.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_open));
  m_button_save.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_save));
  m_button_start.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_start));
  m_button_step.signal_clicked().connect(sigc::mem_fun(*this,&GUI::on_button_clicked_step));
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &GUI::on_timeout), 1000 );
   
  show_all_children();

  maximize();
}

GUI::~GUI(){}
void GUI::on_button_clicked_exit(){ hide(); }
void GUI::on_button_clicked_open(){ exit(0); }
void GUI::on_button_clicked_save(){ save(mainMap->getLNb(), players, mainMap, balls); }
void GUI::on_button_clicked_start(){ exit(0); }
void GUI::on_button_clicked_step(){ exit(0); }
bool GUI::on_timeout()
{
    // force our program to redraw the entire clock.
    auto win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}

int draw(PlayerMap* players, BallMap* balls, Map* mainMap){
    auto app = Application::create();

    GUI mainWindow(players, balls, mainMap);
    app->run(mainWindow);
  return 0;
}