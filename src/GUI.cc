/*!
  \file   GUI.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "GUI" module implementation
*/

#include "GUI.h"
#include "define.h"
#include "simulation.h"
#include <glibmm/main.h>
#include <gtkmm.h>
#include <iostream>

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "ball.h"
#include "map.h"
#include "player.h"
#endif

using namespace Gtk;

class MyArea : public Gtk::DrawingArea {
  public:
    MyArea(PlayerMap* p, BallMap* b, Map* m) {
        players = p;
        balls = b;
        mainMap = m;
    };
    virtual ~MyArea(){};
    void clear();
    void draw();
    PlayerMap* players;
    BallMap* balls;
    Map* mainMap;

  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  private:
    bool empty;
    void refresh();
};

void MyArea::clear() {
    empty = true;
    refresh();
}

void MyArea::draw() {
    empty = false;
    refresh();
}

void MyArea::refresh() {
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    if (not empty) {
        Gtk::Allocation allocation = get_allocation();
        int width = allocation.get_width();
        int height = allocation.get_height();
        int lesser = MIN(width, height);

        int xc, yc;
        xc = width / 2;
        yc = height / 2;
        int nbPlayer = players->getNb();
        cr->save();
        for (int p = 0; p < nbPlayer; p++) {
            Player* player = players->getPlayer(p);
            player->setGX(width / 2 + player->getX()); // PUT ELSEWHERE !
            player->setGY(height / 2 - player->getY());
            if (player->getTimeTouched() >= 4) { // Rename var //PUT IN A FUNCTION !!!
                cr->set_source_rgba(0.0, 1, 0.0, 1);
            } else if (player->getTimeTouched() == 3) {
                cr->set_source_rgba(1, 1, 0.0, 1);
            } else if (player->getTimeTouched() == 2) {
                cr->set_source_rgba(1.0, 1, 0.0, 1);
            } else {
                cr->set_source_rgba(1.0, 0.0, 0.0, 1);
            }
            cr->arc(player->getGX(), player->getGY(),
                    (player->getRadius() / SIDE) * lesser, 0.0, 2.0 * M_PI);
            cr->fill_preserve();
            cr->stroke();
            cr->set_source_rgba(0.0, 0.0, 1.0, 1);
            cr->arc(player->getGX(), player->getGY(),
                    (player->getRadius() / SIDE) * lesser, 0.0,
                    (player->getCount() / MAX_COUNT) * (2 * M_PI));
            std::cout << (player->getCount() / MAX_COUNT) * (2 * M_PI) << std::endl;
            cr->stroke();
            player = nullptr;
        }
        cr->restore();

        int nbBall = balls->getNb();
        cr->save();
        cr->set_source_rgba(0.0, 0.0, 1, 1);
        for (int b = 0; b < nbBall; b++) {
            Ball* ball = balls->getBall(b);

            ball->setGX(width / 2 + ball->getX()); // PUT ELSEWHERE !
            ball->setGY(height / 2 - ball->getY());

            cr->arc(ball->getGX(), ball->getGY(), (ball->getRadius() / SIDE) * lesser,
                    0.0, 2.0 * M_PI);
            cr->fill_preserve();
            cr->stroke();
            ball = nullptr;
        }
        cr->restore();

        int nbObstacle = mainMap->getNb();
        cr->save();
        cr->set_source_rgba(1.0, 0.0, 0.0, 1);
        for (int o = 0; o < nbObstacle; o++) {
            Obstacle* obstacle = mainMap->getObstacle(o);
            double side = obstacle->getSide();

            obstacle->setGX(width / 2 + obstacle->getX()); // PUT ELSEWHERE !
            obstacle->setGY(height / 2 - obstacle->getY());
            cr->move_to(obstacle->getGX() - side / 2, obstacle->getGY() - side / 2);
            cr->rel_line_to(side, 0);
            cr->rel_line_to(0, side);
            cr->rel_line_to(-side, 0);
            cr->rel_line_to(0, -side);
            cr->fill_preserve();
            cr->stroke();
            obstacle = nullptr;
        }
        cr->restore();
    } else {
        std::cout << "Empty !" << std::endl;
    }
    return true;
}

class GUI : public Window {
  public:
    GUI(PlayerMap* players, BallMap* balls, Map* mainMap);
    virtual ~GUI();

  protected: // Or private ?
    void on_button_clicked_exit();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_start();
    void on_button_clicked_step();
    bool on_timeout();
    bool timer_added;
    bool disconnect;
    const int timeoutValue;

    Box m_box_top, m_box1, m_box2;
    Button m_button_exit;
    Button m_button_open;
    Button m_button_save;
    Button m_button_start;
    Button m_button_step;
    Label m_label_status;
    MyArea m_area;
};

GUI::GUI(PlayerMap* p, BallMap* b, Map* m)
    : m_box_top(Gtk::ORIENTATION_VERTICAL), m_box1(Gtk::ORIENTATION_HORIZONTAL, 10),
      m_box2(Gtk::ORIENTATION_HORIZONTAL, 10), m_button_exit("Exit"),
      m_button_open("Open"), m_button_save("Save"), m_button_start("Start"),
      m_button_step("Step"), m_label_status("No game to run"), m_area(p, b, m),
      timer_added(false), disconnect(false), timeoutValue(DELTA_T * 1000) {

    set_title("DodgeBall");
    set_border_width(0);

    m_box1.set_size_request(10, 10);

    add(m_box_top);
    m_box_top.pack_start(m_box1, false, false);
    m_box_top.pack_start(m_box2);

    m_area.set_size_request(SIDE, SIDE);
    m_box2.pack_start(m_area);

    m_box1.pack_start(m_button_exit, false, false);
    m_box1.pack_start(m_button_open, false, false);
    m_box1.pack_start(m_button_save, false, false);
    m_box1.pack_start(m_button_start, false, false);
    m_box1.pack_start(m_button_step, false, false);
    m_box1.pack_start(m_label_status, false, false);

    m_button_exit.signal_clicked().connect(
        sigc::mem_fun(*this, &GUI::on_button_clicked_exit));
    m_button_open.signal_clicked().connect(
        sigc::mem_fun(*this, &GUI::on_button_clicked_open));
    m_button_save.signal_clicked().connect(
        sigc::mem_fun(*this, &GUI::on_button_clicked_save));
    m_button_start.signal_clicked().connect(
        sigc::mem_fun(*this, &GUI::on_button_clicked_start));
    m_button_step.signal_clicked().connect(
        sigc::mem_fun(*this, &GUI::on_button_clicked_step));

    show_all_children();

    maximize();
}

GUI::~GUI() {}

void GUI::on_button_clicked_exit() { hide(); }

void GUI::on_button_clicked_open() {
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_ACCEPT);
    int result = dialog.run();
    switch (result) {
    case (Gtk::RESPONSE_ACCEPT): {
        std::string inputFile = dialog.get_filename();
        bool success =
            initialization(inputFile, m_area.players, m_area.mainMap, m_area.balls);
        if (success) {
            m_area.clear();
            m_area.draw();
            m_label_status.set_label("Game ready to run");
        } else {
            m_area.clear();
            m_label_status.set_label("No game to run");
        }
        break;
    }
    case (Gtk::RESPONSE_CANCEL): {
        break;
    }
    default: { break; }
    }
}

void GUI::on_button_clicked_save() {
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_ACCEPT);
    int result = dialog.run();
    switch (result) {
    case (Gtk::RESPONSE_ACCEPT): {
        std::string filename = dialog.get_filename();
        save(filename, m_area.mainMap->getLNb(), m_area.players, m_area.mainMap,
             m_area.balls);
        break;
    }
    case (Gtk::RESPONSE_CANCEL): {
        break;
    }
    default: { break; }
    }
}

void GUI::on_button_clicked_start() {
    if (not timer_added) {
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &GUI::on_timeout),
                                       timeoutValue);

        timer_added = true;
        m_button_start.set_label("Stop");
    } else {
        disconnect = true;
        timer_added = false;
        m_button_start.set_label("Start");
    }
}

void GUI::on_button_clicked_step() {
    update(m_area.balls, m_area.players);
    check(m_area.balls, m_area.players);
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}
bool GUI::on_timeout() {
    if (disconnect) {
        disconnect = false;

        return false;
    } else {
        update(m_area.balls, m_area.players);
        check(m_area.balls, m_area.players);
        auto win = get_window();
        if (win) {
            Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                             get_allocation().get_height());
            win->invalidate_rect(r, false);
        }
        return true;
    }
}

int draw(PlayerMap* players, BallMap* balls, Map* mainMap) {
    auto app = Application::create();

    GUI mainWindow(players, balls, mainMap);
    app->run(mainWindow);
    return 0;
}