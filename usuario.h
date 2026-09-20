#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario
{
public:
    Usuario();
    Usuario(std::string nombreInicial);
    virtual ~Usuario(); // destructor virtual: buena práctica al tener herencia

    std::string getNombre() const;
    void setNombre(std::string nuevoNombre);

    virtual void mostrarInfo() const; // se puede sobreescribir en clases hijas

protected:
    std::string nombre; // protected: las clases hijas pueden acceder directo
};

#endif