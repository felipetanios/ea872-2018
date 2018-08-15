#include <vector>

#include "oo_model.hpp"
#include <cmath>
#include <iostream>

# define K              10.0

Corpo::Corpo(float massa, float velocidade, float posicao) {
  this->massa = massa;
  this->velocidade = velocidade;
  this->posicao = posicao;
  this->posicao_inicial = posicao;
  this->w = sqrt(K/massa);
}

void Corpo::update(float nova_velocidade, float nova_posicao) {
  this->velocidade = nova_velocidade;
  this->posicao = nova_posicao;
}

float Corpo::get_massa() {
  return this->massa;
}

float Corpo::get_aceleracao() {
  return this->aceleracao;
}

float Corpo::get_velocidade() {
  return this->velocidade;
}

float Corpo::get_posicao() {
  return this->posicao;
}

float Corpo::get_posicao_inicial() {
  return this->posicao_inicial;
}

float Corpo::get_constante_elastica() {
  return this->constante_elastica;
}

float Corpo::get_fator_amortecimento() {
  return this->fator_amortecimento;
}

float Corpo::get_w(){
  return this->w;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
  std::cout << "Agora tenho: " << this->corpos->size() << " elementos" << std::endl;
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

Fisica::Fisica(ListaDeCorpos *ldc) {
  this->lista = ldc;
}

void Fisica::update(float deltaT) {
  static float totalTime = 0;
  totalTime += deltaT;
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    float w = (*c)[i]->get_w();
    float A0 = (*c)[i]->get_posicao_inicial();
    float new_vel = (*c)[i]->get_velocidade() + ((float)deltaT/1000) * (-1)*A0 * pow(w,2) * cos(w * totalTime);
    float new_pos = (*c)[i]->get_posicao() + (float)deltaT * new_vel/1000;
    (*c)[i]->update(new_vel, new_pos);
  }
}

// x = x0 + v0t+ at^2/2
// v = v0 + at
// dv/dt = a = cte
// dv = a *dt
// a = cte


// x = A0 cos(wt)
// v = -A0w sin(wt)
// dx = -A0sin(wt) + dt
// dv/dt = a = -A0wˆ2 cos(wt)
// dv= -A0* w^2 * cos(wt) * dt


// x = A0*e^(-at)*cos(wt+phi)
// a = -b/2m
// w = (1/2*pi)sqrt(k/m)

