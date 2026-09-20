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
bool Usuario::esContrasenaValida(std::string contrasena)
{
    if (contrasena.length() < 6) return false;

    bool tieneLetra = false;
    bool tieneNumero = false;
    for (char c : contrasena) {
        if (isalpha(static_cast<unsigned char>(c))) tieneLetra = true;
        if (isdigit(static_cast<unsigned char>(c))) tieneNumero = true;
    }
    return tieneLetra && tieneNumero;
}
void Usuario::setContrasena(std::string nuevaContrasena) { contrasena = nuevaContrasena; }
bool Usuario::verificarContrasena(std::string intento) const { return contrasena == intento; }
std::string Usuario::getContrasena() const
{
    return contrasena;
}

void Usuario::mostrarInfo() const
{
    std::cout << "Usuario: " << nombre << std::endl;
}