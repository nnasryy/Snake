#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario
{
public:
    Usuario();
    Usuario(std::string nombreInicial);
    virtual ~Usuario();

    std::string getNombre() const;
    void setNombre(std::string nuevoNombre);

    std::string getContrasena() const;
    void setContrasena(std::string nuevaContrasena);
    bool verificarContrasena(std::string intento) const;
    static bool esContrasenaValida(std::string contrasena);
    virtual void mostrarInfo() const;

protected:
    std::string nombre;
    std::string contrasena;
};

#endif