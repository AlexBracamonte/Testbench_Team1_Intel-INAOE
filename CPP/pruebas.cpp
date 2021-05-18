#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <list>
#include <math.h>
#include <bitset>

#define TAM_TOTAL 32

using namespace std;

class Inicializador
{
private:
    string Nombre_Documento;
    string modulo;

public:
    ~Inicializador();
    Inicializador(string nom_in, string mod = "");

    void abrir_archivo(void);
    string Obtener_Nombre_Modulo(void);
    void Obtener_IO();
    string Mapear_Variables(void);
    void asignar_valores(void);
    //string obtener_valores_utt(void);
    string obtener_valores_utt(void);
    void crea_tb(string out_name);

    list<string> lineas;               // Lista de todas las lineas del documento leido
    list<string> entradas;             // Lista de entradas
    list<string> salidas;              // Lista de entradas
    list<string> entradas_solas;       // Lista de entradas, solas en la lista, es decir ["a, b, c, d;", "e, f;"] paso a ser [a, b, c, d, e, f]
    list<string> salidas_solas;        // Lista de variables de salidas
    list<string> entradas_con_valores; // Lista de strings con valores para entradas
    string valores_utt;
};

Inicializador::Inicializador(string nom_in, string mod)
{
    Nombre_Documento = nom_in;
    modulo = mod;
}

Inicializador::~Inicializador()
{
    cout << "Se activo el destructor" << endl;
}

void Inicializador::abrir_archivo()
{
    cout << endl
         << endl
         << "--- abrir_archivo --- " << endl
         << endl;

    string line;
    ifstream design; // Se crea el objeto fichero design

    try
    {
        design.open(Nombre_Documento); // Se abre el documento
        if (design)                    // Se pregunta si existe algo dentro de design
        {

            while (getline(design, line)) // Leemos una linea
                lineas.push_back(line);   // Esa linea se guarda en la lista

            cout << "El resultado es: " << endl;
            for (const auto &i : lineas)
                cout << i << endl;
        }
        else
        {
            cout << "No se pudo abrir el archivo: " << Nombre_Documento << "'" << endl;
            cout << "Es probable que el documento no exista, o no este en la misma carpeta" << endl;
        }
    }
    catch (...)
    {
        cerr << "No se pudo abrir el archivo" << Nombre_Documento << "'" << endl;
        cout << "No se pudo abrir el archivo" << Nombre_Documento << "'" << endl;
    }
}

string Inicializador::Obtener_Nombre_Modulo()
{
    cout << endl
         << endl
         << "--- Obtener_Nombre_Modulo --- " << endl
         << endl;

    if (modulo.compare("") != 0)
    {
        cout << "El usuario ya ingreso un modulo, el nombre es: " << modulo << endl;
    }
    else
    {
        cout << "El usuario no ha ingresado un modulo" << endl;
        // string pattern = "module[\s]+(\S*)[\s]*\([^\)]*\)[\s\S]*"
        smatch match_results;
        regex regexp1("^(module)\\s+(\\w+)");
        for (const auto &linea : lineas)
        {
            regex_search(linea, match_results, regexp1);
            if (match_results.length() > 0)
            {
                cout << "Los resultados son " << match_results[0] << endl;
                modulo = match_results[2];
                cout << endl
                     << endl
                     << "Se encontró esto:" << endl;
                for (auto x : match_results)
                {
                    cout << x << " | ";
                }
            }
        }
    }
    cout << endl
         << "EL nombre del modulo es: " << modulo << endl;
    return modulo;
}

void Inicializador::Obtener_IO()
{
    cout << endl
         << endl
         << "--- Activamos Obtener_IO --- " << endl
         << endl;
    list<string> GPIO;
    GPIO.push_back("input");
    GPIO.push_back("output");
    GPIO.push_back("inout");

    for (const auto &linea : lineas)
    { 
        smatch m;
        regex regexp1("s*(input)\\s+");
        regex_search(linea, m, regexp1);
        if (m.length() > 0)
        {
            string aux = regex_replace(linea, regexp1, "");
            cout << "Aux antes: " << aux << endl;
            for (char letra : aux)
            {
                if (letra == '\t')      // 
                {
                    int lugar = aux.find('\t');
                    cout << "El lugar es. " << lugar << " Y el tamanio es " << aux.size() << endl;
                    aux.erase((lugar), 1);
                }
                if (letra == ')')
                {
                    int lugar = aux.find(')');
                    cout << "El lugar es. " << lugar << " Y el tamanio es " << aux.size() << endl;
                    aux.erase((lugar), 1);
                }
            }
            aux.pop_back();
            aux = aux + ';';
            cout << "\tLa salida total sera ----------------- " << aux << endl;
            cout << "Aux despues: " << aux << endl;
            entradas.push_back(aux);
        }
    }

    for (const auto &linea : lineas)
    {
        smatch m;
        regex regexp1("s*(output)\\s+");
        regex_search(linea, m, regexp1);
        if (m.length() > 0)
        {
            string aux = regex_replace(linea, regexp1, "");
            cout << "Aux antes: " << aux << endl;
            for (char letra : aux)
            {
                if (letra == '\t')
                {
                    int lugar = aux.find('\t');
                    cout << "El lugar es. " << lugar << " Y el tamanio es " << aux.size() << endl;
                    aux.erase((lugar), 1);
                }
                if (letra == ')')
                {
                    int lugar = aux.find(')');
                    cout << "El lugar es. " << lugar << " Y el tamanio es " << aux.size() << endl;
                    aux.erase((lugar), 1);
                }
            }
            aux.pop_back();
            aux = aux + ';';
            cout << "Aux despues: " << aux << endl;
            salidas.push_back(aux);
        }
    }
}

string Inicializador::Mapear_Variables(void)
{
    cout << endl
         << endl
         << "--- Activamos Mapear variables --- " << endl
         << endl;
    list<string> aux = entradas; // ["a, b, c," , "[] d, e, f"]

    for (auto string_de_entradas : entradas) // Por palabra en las entradas
    {
        string aux = "";
        for (auto letra : string_de_entradas) // For de letra en palabra
        {

            if (((letra != ' ') || (letra != ',') || (letra != ';'))) // Creamos la cadena que se tiene que ingresar
            {
                aux = aux + letra;  // ,out_10 out_15; [out_10 out_15]
            }
            if ((letra == ',') || (letra == ';')) // Preguntamos por la existencia de dichos valores
            {
                int lugar = aux.find(','); // Elimina caracter
                if (lugar > 0)
                    aux.erase((lugar), 1); //string.remove()

                lugar = aux.find(';'); // Elimina estos caracteres
                if (lugar > 0)
                    aux.erase((lugar), 1);

                for (int i = 0; i < aux.size(); i++) // Eliminar espacios en blanco
                {
                    if (aux[i] == ' ')
                    {
                        aux.erase(i, 1);
                        i--;
                    }
                }
                cout << "Lo que vamos a ingresar es: " << aux << endl;
                entradas_solas.push_back(aux);  //  aux.append() 
                aux = "";
            }
        }
    }

    for (auto valor : entradas_solas)
    {
        cout << "El valor por entradas es: " << valor << endl;
    }

    aux = salidas;

    for (auto palabra : salidas) // Por palabra en las entradas
    {
        string aux = "";
        for (auto letra : palabra) // For de letra en palabra
        {

            if (((letra != ' ') || (letra != ',') || (letra != ';'))) // Creamos la cadena que se tiene que ingresar
            {
                aux = aux + letra;
            }
            if ((letra == ',') || (letra == ';')) // Preguntamos por la existencia de dichos valores
            {
                int lugar = aux.find(','); // Elimina caracter
                if (lugar > 0)
                    aux.erase((lugar), 1);

                lugar = aux.find(';'); // Elimina estos caracteres
                if (lugar > 0)
                    aux.erase((lugar), 1);

                for (int i = 0; i < aux.size(); i++) // Eliminar espacios en blanco
                {
                    if (aux[i] == ' ')
                    {
                        aux.erase(i, 1);
                        i--;
                    }
                }
                cout << "Lo que vamos a ingresar es: " << aux << endl;
                salidas_solas.push_back(aux);
                aux = "";
            }
        }
    }
    for (auto valor : entradas_solas)
    {
        cout << "El valor por entradas es: " << valor << endl;
    }
    for (auto valor : salidas_solas)
    {
        cout << "El valor por salidas es: " << valor << endl;
    }

    return "aux";
}

void Inicializador::asignar_valores(void)
{
    cout << endl
         << endl
         << "--- Asignar valores a entradas --- " << endl
         << endl;

    int largo = entradas_solas.size();  //  Ejemplo; largo = 6
    int max_valor = pow(2, largo);      // Ejem = 2^6 = 64


// lista[5] = 50

//indice_lista = begin(entradas_solas)
// indice_lista += 5
// * indice_lista = 50

    cout << "El largo es: " << largo << " y la potencia es: " << max_valor << endl;
    for (int numero = 0; numero < max_valor; numero++)
    {
        bitset<TAM_TOTAL> bs2(numero);              // Esta es la linea que convierte el numero a binario
        auto indice_lista = begin(entradas_solas);  // indice lista es un apuntador, que apunta al inicio de la lista 
        string variable = "";                       // variable es un auxiliar con nada
        for (int lugar = 0; lugar < largo; lugar++)
        {
            variable = variable + *indice_lista + "= " + to_string(bs2[lugar]) + "; ";
            indice_lista++;
        }

        cout << numero << " ->: " << bs2 << " -> " << variable << endl;
        variable += " #1";
        cout << numero << " ->: " << bs2 << " ---> " << variable << endl;
        entradas_con_valores.push_back(variable);
    }
}
/*este es el que hizo alex
string Inicializador::obtener_valores_utt(void)
{
    // a, b, c, d, o1, o2, o3
    cout << endl
         << endl
         << "--- Obtener valor de UUT --- " << endl
         << endl;
    string aux = "";
    for (auto valor_entrada : entradas_solas) //(.a(a)
    {
        aux += "." + valor_entrada + "(" + valor_entrada + "), ";
    }
    for (auto valor_entrada : salidas_solas) //(.a(a)
    {
        aux += "." + valor_entrada + "(" + valor_entrada + "), ";
    }
    aux.pop_back();
    aux.pop_back();
    cout << "Res: " << aux << endl;
    valores_utt = aux;
    return aux;
}*/

///////////////////////////////////basico////////////////////

string Inicializador::obtener_valores_utt(void)
{
    // a, b, c, d, o1, o2, o3
    cout << endl
         << endl
         << "--- Obtener valor de UUT --- " << endl
         << endl;
    string aux = "";
    for (auto valor_entrada : entradas_solas) //a,b,c
    {
        aux += valor_entrada ", ";
    }
    for (auto valor_entrada : salidas_solas) //outp
    {
        aux +=  valor_entrada ", ";
    }
    aux.pop_back();
    aux.pop_back();
    cout << "Res222: " << aux << endl;
    valores_utt = aux;
    return aux;
}
/////////////////////////////////////////////////////
void Inicializador::crea_tb(string out_name)
{
    cout << endl
         << endl
         << "--- Imprimiendo TB --- " << endl
         << endl;

    ofstream TB(out_name);
    TB << "`timescale 1ns/1ns  \n\n";
    cout << "`timescale 1ns/1ns  \n\n";

    TB << "module " + modulo + "_TB; \n ";
    cout << "module " + modulo + "_TB; \n ";
    
    for (auto entrada: entradas)
    {
        TB << "\treg " << entrada << "\n";
        cout << "\treg " << entrada << "\n";
    }
    for (auto salida: salidas)
    {
        TB << "\twire " << salida << "\n";
        cout << "\twire " << salida << "\n";
    }
    cout << "\n\n\t" << modulo << " UUT " << "(" << valores_utt << ");\n";
    TB << "\n\n\t" << modulo << " UUT " << "(" << valores_utt << ");\n";

    TB << "\t" << "initial begin\n";
    cout << "\t" << "initial begin\n";
    


    TB << "\t\t" << "$dumpfile(\"" << modulo << ".vcd\");\n";
    TB << "\t\t" << "$dumpvars(1, " << modulo << "_TB);\n";


    
    
  
    for(auto valor: entradas_con_valores)
    {
        cout << "\t\t" << valor << "\n";
        TB << "\t\t" << valor << "\n";
    }

    TB << "\t" << "$finish;\n";
    TB << "\t" << "end\n";
    TB << "endmodule";
    TB.close();
}

int main()
{
    cout << endl
         << endl
         << "\t\t****************" << endl
         << "\t\t*  design.sv   *" << endl
         << "\t\t****************" << endl
         << endl;

    Inicializador test1("design.sv");
    test1.abrir_archivo();
    test1.Obtener_Nombre_Modulo();
    test1.Obtener_IO();
    test1.Mapear_Variables();
    test1.asignar_valores();
    test1.obtener_valores_utt();
    test1.crea_tb("testbench1.sv");

    system("pause");
    return 0;
}
