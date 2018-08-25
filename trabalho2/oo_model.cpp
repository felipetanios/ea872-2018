#include <vector>

#include "oo_model.hpp"

#include <ncurses.h>

# define K              100.0
# define B              0.1

Corpo::Corpo(float massa, float velocidade, float posicao, float k, float b) {
  this->massa = massa;
  this->velocidade = velocidade;
  this->posicao = posicao;
  this-> k = k;
  this->b = b;
}

void Corpo::update(float nova_velocidade, float nova_posicao) {
  this->velocidade = nova_velocidade;
  this->posicao = nova_posicao;
}

float Corpo::get_massa() {
  return this->massa;
}

float Corpo::get_velocidade() {
  return this->velocidade;
}

float Corpo::get_posicao() {
  return this->posicao;
}

float Corpo::get_k(){
  return this->k;
}

float Corpo::get_b(){
  return this->b;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_massa(),\
                          (*corpos)[k]->get_posicao(),\
                          (*corpos)[k]->get_velocidade(),\
                          (*corpos)[k]->get_k(),\
                          (*corpos)[k]->get_b()
                        );
    this->add_corpo(c);
  }

}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

Fisica::Fisica(ListaDeCorpos *ldc) {
  this->lista = ldc;
}

void Fisica::update(float deltaT) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    float new_acc = (-1)*(*c)[i]->get_k()*(*c)[i]->get_posicao()/(*c)[i]->get_massa() - (*c)[i]->get_b()*(*c)[i]->get_velocidade();
    float new_vel = (*c)[i]->get_velocidade() + (float)deltaT * new_acc/1000;
    float new_pos = (*c)[i]->get_posicao() + (float)deltaT * new_vel/1000;
    (*c)[i]->update(new_vel, new_pos);
  }
}

Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int i;

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  int terminalSizeX, terminalSizeY;
  getmaxyx(stdscr, terminalSizeY, terminalSizeX);
  int offset = this->maxI / 2;
  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    i = offset + (int) ((*corpos_old)[k]->get_posicao()) * \
        (this->maxI / this->maxX);
        if (i < terminalSizeY && i >=0) {
    move(i, k);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
        }
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();
  for (int k=0; k<corpos->size(); k++)
  {
    i = offset + (int) ((*corpos)[k]->get_posicao()) * \
        (this->maxI / this->maxX);
        if (i < terminalSizeY && i >=0) {
    move(i, k);   /* Move cursor to position */
    echochar('*');  /* Prints character, advances a position */

          
        }
    // Atualiza corpos antigos
    (*corpos_old)[k]->update(  (*corpos)[k]->get_velocidade(),\
                               (*corpos)[k]->get_posicao());
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}




