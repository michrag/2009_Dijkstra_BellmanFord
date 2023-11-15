
// Questo programma implementa gli algoritmi di Dijkstra e di Bellman-Ford: il grafo è rappresentato con liste di adiacenza.
// L'algoritmo di Dijkstra utilizza una (inefficiente) coda di min-priorità realizzata con un min-heap binario.

// MinCoda.h

#include <vector>
#include <cmath> // floor
#include <limits> // infinity


class ErroriCoda {};
class CodaVuota: public ErroriCoda {};
class ElementoNonPresente: public ErroriCoda {};
class NuovaKeyMaggiore: public ErroriCoda {};


template<class Tipo>
class MinPriorityQueue
{

    struct handle
    {
        Tipo* pointer;
        double key;
    };

    std::vector<handle> A;

public:
    // 3 costruttori: la coda è inizializzabile in 3 modi diversi
    MinPriorityQueue() {};

    MinPriorityQueue(std::vector<Tipo*> T)
    {
        for(int i = 0; i < T.size(); i++)
        {
            handle tmp = {T[i], std::numeric_limits<double>::infinity()};
            A.push_back(tmp);
        }
    };

    MinPriorityQueue(std::vector<Tipo*> T, std::vector<double> V)
    {
        for(int i = 0; i < T.size(); i++)
        {
            if(i < V.size())
            {
                handle tmp = {T[i], V[i]};
                A.push_back(tmp);
            }
            else
            {
                handle tmp = {T[i], std::numeric_limits<double>::infinity()};
                A.push_back(tmp);
            }
        }

        build_min_heap(A);
    };

    int parent(int i);

    int left(int i);

    int right(int i);

    void min_heapify(int i);

    void build_min_heap(std::vector<handle>& A);

    Tipo* heap_minimum() throw(CodaVuota);

    Tipo* extract_min() throw(CodaVuota);

    void heap_decrease_key(Tipo* ptr, double key) throw(ErroriCoda);
    // versatile ma inefficiente

    void min_heap_insert(Tipo* N, double key) throw(NuovaKeyMaggiore);

    bool is_empty()
    {
        return A.empty();
    }

};


template<class Tipo>
int MinPriorityQueue<Tipo>::parent(int i)
{
    if(i == 0)
    {
        return 0;
    }

    return(int(floor((i - 1) / 2)));
}


template<class Tipo>
int MinPriorityQueue<Tipo>::left(int i)
{
    return(2 * i + 1);
}


template<class Tipo>
int MinPriorityQueue<Tipo>::right(int i)
{
    return(2 * i + 2);
}


template<class Tipo>
void MinPriorityQueue<Tipo>::min_heapify(int i)
{
    int l = left(i);
    int r = right(i);
    int minimo;

    if((l < A.size()) && (A[l].key < A[i].key))
    {
        minimo = l;
    }
    else
    {
        minimo = i;
    }

    if((r < A.size()) && (A[r].key < A[minimo].key))
    {
        minimo = r;
    }

    if(minimo != i)
    {
        handle tmp = A[i];
        A[i] = A[minimo];
        A[minimo] = tmp;
        min_heapify(minimo);
    }
}


template<class Tipo>
void MinPriorityQueue<Tipo>::build_min_heap(std::vector<handle>& A)
{
    for(int i = int(floor(A.size() / 2) - 1); i > -1; i--)
    {
        min_heapify(i);
    }
}


template<class Tipo>
Tipo* MinPriorityQueue<Tipo>::heap_minimum() throw(CodaVuota)
{
    if(is_empty() != true)
    {
        return A.front().pointer;
    }
    else
    {
        throw CodaVuota();
    }
}


template<class Tipo>
Tipo* MinPriorityQueue<Tipo>::extract_min() throw(CodaVuota)
{
    if(is_empty() != true)
    {
        Tipo* min = A.front().pointer;
        A.front() = A.back();
        A.pop_back();
        min_heapify(0);
        return min;
    }
    else
    {
        throw CodaVuota();
    }
}


template<class Tipo>
void MinPriorityQueue<Tipo>::heap_decrease_key(Tipo* ptr, double key)
throw(ErroriCoda)
{
    // Attenzione: sostanziale differenza dalla teoria!
    // Prende un puntatore all'oggetto da modificare e ne ricava l'indice
    // nell'array, di conseguenza questa implementazione di decrease_key costa O(n)
    // anziché O(logn): il costo è dominato dall'operazione di ricerca.

    if(is_empty() == true)
    {
        throw CodaVuota();
    }

    int i = A.size() - 1;

    // parte dal fondo così min_heap_insert esegue immediatamente
    while((A[i].pointer != ptr) && (i > -1))
    {
        i--;
    }

    if(i == -1)
    {
        throw ElementoNonPresente();
    }

    if(A[i].key < key)
    {
        throw NuovaKeyMaggiore();
    }

    A[i].key = key;
    handle tmp;

    while(i > 0 && (A[parent(i)].key > A[i].key))
    {
        tmp = A[i];
        A[i] = A[parent(i)];
        A[parent(i)] = tmp;
        i = parent(i);
    }
}


template<class Tipo>
void MinPriorityQueue<Tipo>::min_heap_insert(Tipo* ptr, double key)
throw(NuovaKeyMaggiore)
{
    handle H = {ptr, std::numeric_limits<double>::infinity()};
    A.push_back(H);

    try
    {
        heap_decrease_key(ptr, key);
    }
    catch(NuovaKeyMaggiore)
    {
        throw;   // impossibile...?!
    }
}

// Fine MinCoda.h
