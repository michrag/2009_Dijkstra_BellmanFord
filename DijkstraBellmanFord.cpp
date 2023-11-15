// DijkstraBellmanFord.cpp

#include <iostream>
#include <vector>
#include <stack>
#include <limits> // infinity
#include "ClasseNodo.h"

void Initialize_Single_Source(std::vector<Nodo>* V, int n, Nodo* s)
{
    for(int i = 0; i < n; i++)
    {
        (*V)[i].modifica_peso(std::numeric_limits<double>::infinity());
        (*V)[i].assegna_pred(0);
    }

    s->modifica_peso(0);
    std::cout << "\nIl nodo sorgente e' ";
    s->stampa_nome();
    std::cout << std::endl;
}


void Dijkstra(std::vector<Nodo>* V, Nodo* s) throw(ErroriCoda)
{

    Initialize_Single_Source(V, V->size(), s);

    std::stack<Nodo*> S; // inutile, presente solo per "fedelt�" allo pseudocodice

    MinPriorityQueue<Nodo> Q;

    for(int i = 0; i < (V->size()); i++)
    {
        Q.min_heap_insert(&((*V)[i]), (*V)[i].get_peso());
    }

    Nodo* u;

    while(Q.is_empty() == false)
    {
        try
        {
            u = Q.extract_min();
        }
        catch(CodaVuota)
        {
            break;   // evento impossibile ma non disastroso
        }

        try
        {
            u->relax(&Q);
        }
        catch(ErroriCoda)
        {
            throw;
        }

        S.push(u);
        u->stampa_cammino_minimo();
    }
}


bool BellmanFord(std::vector<Nodo>* V, Nodo* s)
{
    bool no_cicli_negativi = true;

    Initialize_Single_Source(V, V->size(), s);

    for(int i = 0; i < ((V->size()) - 1); i++)
        for(int j = 0; j < (V->size()); j++)
        {
            (*V)[j].relax();    // sono esaminati tutti gli archi
        }

    int k = 0;

    while((k < (V->size())) && (no_cicli_negativi == true))
    {
        no_cicli_negativi = (*V)[k].check(); // sono esaminati tutti gli archi
        k++;
    }

    return no_cicli_negativi;
}


// da qui fino alla fine è realizzato l'input / output

bool search(std::vector<Nodo>* V, std::string nome, int* i)
{
    // serve in fase di input da tastiera

    bool found = false;

    while((*i < V->size()) && (found == false))
    {
        if(nome == (*V)[*i].get_nome())
        {
            found = true;
        }
        else
        {
            (*i)++;
        }
    }

    return found;
}


void stampa_errori(int i)
{
    if(i == 1)
    {
        std::cout << "Input non accettato: non e' piu' possibile aggiungere nodi!\n";
    }

    if(i == 2)
    {
        std::cout << "Input non accettato: puoi aggiungere ancora un solo altro nodo!\n";
    }

    if(i == 3)
    {
        std::cout << "Numero nodi raggiunto. Da ora impossibile aggiungere nuovi nodi!\n";
    }
}


int main()
{

    std::cout << "Questo programma implementa gli algoritmi di Dijkstra e di Bellman-Ford\n";

    int size;

    do
    {
        std::cout << "Quanti nodi ha il grafo? (Almeno 2!)\n>";
        std::cin >> size;
    }
    while(size < 2);

    std::vector<Nodo> Vec(size);

    std::string nome1, nome2, sorgente;
    double p; // peso arco
    bool exit = false;
    bool found1, found2;
    int i1, i2;
    int i = 0;
    bool dij = true; // false <=> Dijkstra non pu� essere eseguito

    std::cout << "Inserire gli archi del grafo con la seguente sintassi:\n\nnodo1 nodo2 peso_arco(nodo1, nodo2)\n";
    std::cout << "\nAttenzione alla distinzione maiuscole / minuscole: e' case-sensitive\n";
    std::cout << "Per terminare l'input inserire \"0 0 0\"\n";

    do
    {
        std::cin >> nome1 >> nome2 >> p;

        if(nome1 == "0" && nome2 == "0" && p == 0)
        {
            if(i == size)
            {
                exit = true;
            }
            else
            {
                std::cout << "Input non accettato: mancano ancora " << (size - i) << " nodi!\n";
            }
        }

        else
        {
            if(p < 0)
            {
                dij = false;
            }

            // Dijkstra non pu� essere eseguito su un grafo con archi di peso negativo

            i1 = 0;
            i2 = 0;
            found1 = false;
            found2 = false;

            found1 = search(&Vec, nome1, &(i1));
            found2 = search(&Vec, nome2, &(i2));

            if(found1 == false && found2 == false)
            {
                if(i == size)
                {
                    stampa_errori(1);
                }

                if(i == (size - 1))
                {
                    stampa_errori(2);
                }

                if(i < (size - 1))
                {
                    Vec[i] = (nome1);
                    Vec[i + 1] = (nome2);
                    Vec[i].aggiungi_nodo_adiacente((&Vec[i + 1]), p);
                    i = i + 2;

                    if(i == size)
                    {
                        stampa_errori(3);
                    }
                }
            }

            if(found1 == true && found2 == false)
            {
                if(i == size)
                {
                    stampa_errori(1);
                }
                else
                {
                    Vec[i] = (nome2);
                    Vec[i1].aggiungi_nodo_adiacente((&Vec[i]), p);
                    i++;

                    if(i == size)
                    {
                        stampa_errori(3);
                    }
                }
            }

            if(found1 == false && found2 == true)
            {
                if(i == size)
                {
                    stampa_errori(1);
                }
                else
                {
                    Vec[i] = (nome1);
                    Vec[i].aggiungi_nodo_adiacente((&Vec[i2]), p);
                    i++;

                    if(i == size)
                    {
                        stampa_errori(3);
                    }
                }
            }

            if(found1 == true && found2 == true)
            {
                Vec[i1].aggiungi_nodo_adiacente((&Vec[i2]), p);
            }

        }
    }
    while(exit == false);

    for(int i = 0; i < size; i++)
    {
        Vec[i].stampa_nodi_adiacenti();
    }

    int k; // posizione nell'array del nodo sorgente
    bool found = false;

    do
    {
        std::cout << "\nQual e' il nodo sorgente?\n>";
        std::cin >> sorgente;
        k = 0;
        found = search(&Vec, sorgente, (&k));

        if(found == false)
        {
            std::cout << "Il nodo sorgente deve essere uno tra quelli immessi!\n";
        }
    }
    while(found == false);

    std::cout << "\nProvo ad eseguire Dijkstra...\n";

    if(dij == true)
    {
        try
        {
            Dijkstra(&Vec, &Vec[k]);
        }
        catch(ErroriCoda)
        {
            std::cerr << "\nErrore disastroso durante l'esecuzione di Dijkstra!\n";
        }
    }
    else
    {
        std::cout << "Non posso eseguire Dijkstra: c'e' almeno un arco di peso negativo!\n";
    }

    std::cout << "\nProvo ad eseguire Bellman-Ford...\n";
    bool bford = BellmanFord(&Vec, &Vec[k]);

    if(bford == true)
    {
        for(int i = 0; i < size; i++)
        {
            Vec[i].stampa_cammino_minimo();
        }
    }
    else
    {
        std::cout << "Non posso eseguire Bellman-Ford:\nil grafo contiene almeno un ciclo di peso negativo raggiungibile dalla sorgente!\n";
    }

    //system("pause");
}

// Fine DijkstraBellmanFord.cpp
