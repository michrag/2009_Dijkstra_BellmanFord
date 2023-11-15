// ClasseNodo.h

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <limits> // infinity
#include "MinCoda.h"


class Nodo
{
    std::string nome;
    double peso; // peso dalla sorgente
    struct NodoAdiacente
    {
        Nodo* ptr;
        double peso_arco;
    };
    std::vector<NodoAdiacente> lista_di_adiacenza;
    Nodo* predecessore; // per ricostruire il cammino di peso minimo

public:
    Nodo() {};

    Nodo(std::string name)
    {
        nome = name;
    }

    std::string get_nome() const
    {
        return nome;
    }

    void stampa_nome() const
    {
        std::cout << nome;
    }

    void aggiungi_nodo_adiacente(Nodo* n, double p);

    void stampa_nodi_adiacenti() const;

    void modifica_peso(double dv)
    {
        peso = dv;
    }

    double get_peso() const
    {
        return peso;
    }

    void assegna_pred(Nodo* pr)
    {
        predecessore = pr;
    }

    Nodo* get_pred() const
    {
        return predecessore;
    }

    void relax(MinPriorityQueue<Nodo>* coda_ptr) throw(ErroriCoda);
    // versione per Dijkstra

    void relax(); // versione per Bellman-Ford

    bool check(); // usato da Bellman-Ford

    void stampa_cammino_minimo();
};


void Nodo::aggiungi_nodo_adiacente(Nodo* n, double p)
{
    NodoAdiacente nodo_ad = {n, p};
    lista_di_adiacenza.push_back(nodo_ad);
}


void Nodo::stampa_nodi_adiacenti() const
{
    std::cout << "\nI seguenti nodi sono adiacenti a " << nome << ", con peso:\n";

    for(int i = 0; i < lista_di_adiacenza.size(); i++)
    {
        lista_di_adiacenza[i].ptr->stampa_nome();
        std::cout << " " << lista_di_adiacenza[i].peso_arco << std::endl;
    }
}


void Nodo::relax(MinPriorityQueue<Nodo>* coda_ptr) throw(ErroriCoda)
{
    double dv; // peso del nodo v adiacente a questo nodo (u)
    double du = peso; // peso di questo nodo (u) dalla sorgente
    double wuv; // peso dell'arco (u,v)

    // relax si "assume la responsabilità" di esaminare tutti i nodi adiacenti
    for(int i = 0; i < lista_di_adiacenza.size(); i++)
    {
        dv = lista_di_adiacenza[i].ptr->get_peso();
        wuv = lista_di_adiacenza[i].peso_arco;

        if((dv > (du + wuv)))
        {
            dv = du + wuv;
            lista_di_adiacenza[i].ptr->modifica_peso(dv);

            try
            {
                coda_ptr->heap_decrease_key(lista_di_adiacenza[i].ptr, dv);
            }
            // gli errori sollevabili da heap_decrease_key sono impossibili ma disastrosi
            catch(ErroriCoda)
            {
                throw;
            }

            lista_di_adiacenza[i].ptr->assegna_pred(this);
        }
    }
}


void Nodo::relax()  // versione per Bellman-Ford
{
    double dv; // peso del nodo v adiacente a questo nodo (u)
    double du = peso; // peso di questo nodo (u) dalla sorgente
    double wuv; // peso dell'arco (u,v)

    // relax si "assume la responsabilità" di esaminare tutti i nodi adiacenti
    for(int i = 0; i < lista_di_adiacenza.size(); i++)
    {
        dv = lista_di_adiacenza[i].ptr->get_peso();
        wuv = lista_di_adiacenza[i].peso_arco;

        if((dv > (du + wuv)))
        {
            dv = du + wuv;
            lista_di_adiacenza[i].ptr->modifica_peso(dv);
            lista_di_adiacenza[i].ptr->assegna_pred(this);
        }
    }
}


bool Nodo::check()  // controlla se esiste un ciclo di peso negativo
{
    double dv; // peso del nodo v adiacente a questo nodo (u)
    double du = peso; // peso di questo nodo (u) dalla sorgente
    double wuv; // peso dell'arco (u,v)

    // check si "assume la responsabilità" di esaminare tutti i nodi adiacenti
    for(int i = 0; i < lista_di_adiacenza.size(); i++)
    {
        dv = lista_di_adiacenza[i].ptr->get_peso();
        wuv = lista_di_adiacenza[i].peso_arco;

        if((dv > (du + wuv)))
        {
            return false;
        }
    }

    return true;
}


void Nodo::stampa_cammino_minimo()
{
    double m = get_peso();

    if(m == (std::numeric_limits<double>::infinity()))
    {
        std::cout << "Il nodo ";
        stampa_nome();
        std::cout << " e' irraggiungibile dalla sorgente!" << std::endl;
    }

    else
    {
        std::cout << "Il cammino minimo dalla sorgente a ";
        stampa_nome();
        std::cout << " costa " << m << " ed e':" << std::endl;

        std::stack<Nodo*> pila;
        Nodo* ptr = this;

        while(ptr != 0)
        {
            pila.push(ptr);
            ptr = ptr->get_pred();
        }

        while(pila.empty() == false)
        {
            ptr = pila.top();
            ptr->stampa_nome();

            if(ptr != this)
            {
                std::cout << "->";
            }
            else
            {
                std::cout << std::endl;
            }

            pila.pop();
        };
    }
}

// Fine ClasseNodo.h
