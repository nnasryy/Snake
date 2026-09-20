#include "usuario.h"
#include <iostream>

Usuario::Usuario()
{
    nombre = "";
}

Usuario::Usuario(std::string nombreInicial)
{
    nombre = nombreInicial;
}

Usuario::~Usuario() {}

std::string Usuario::getNombre() const { return nombre; }
void Usuario::setNombre(std::string nuevoNombre) { nombre = nuevoNombre; }

void Usuario::mostrarInfo() const
{
    std::cout << "Usuario: " << nombre << std::endl;
}