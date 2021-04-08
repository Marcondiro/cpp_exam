#include <iostream> //std::cout std::cerr
#include <cassert> //assert
#include <string> //std::string
#include <ostream> // std::ostream

#include "digraph.h"

/**
 * @brief Funtore di uguaglianza tra int
 * 
 * Usa l'operatore == tra interi.
 */
struct Int_equal {
    bool operator()(const int& a, const int& b) const {
        return a == b;
    }
};

/**
 * @brief Struct custom utilizzata per testare Digraph
 */
struct Person {
    std::string _first_name;
    std::string _last_name;
    char _eyes_color;

    Person() : _eyes_color('B') {}

    Person(const std::string& first_name,
            const std::string& last_name,
            char eyes_color) :
        _first_name(first_name),
        _last_name(last_name),
        _eyes_color(eyes_color) {}
};

/**
 * @brief Funtore di uguaglianza tra Person.
 * 
 * Usa l'operatore == tra stringhe. In particolare verifica l'ugaglianza tra
 * _fist_name e _last_name delle due istanze.
 */
struct Person_equal {
    bool operator()(const Person& a, const Person& b) const {
        return (a._first_name == b._first_name &&
            a._last_name == b._last_name);
    }
};

/**
 * @brief Invia Person sullo stream
 * 
 * Ridefinizione dell'operatore di stream per l'invio del solo nome di Person
 * sullo stream.
 * 
 * @param os Stream di output
 * @param p Person da inviare
 * @return Reference allo stream di output
 */
std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p._first_name;
    return os;
}

/**
 * @brief Funzione helper che genera un digraph di interi popolato.
 * 
 * Il grafo risultante rispecchia quello presente nella traccia del progetto. 
 */
Digraph<int, Int_equal> testHelperInt() {
    Digraph<int, Int_equal> g;

    for(int i = 1; i <= 6; ++i) {
        g.addNode(i);
    }

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 6);
    g.addEdge(5, 5);
    g.addEdge(5, 6);

    return g;
}

/**
 * @brief Funzione helper che genera un digraph di Person popolato.
 */
Digraph<Person, Person_equal> testHelperPerson() {
    Digraph<Person, Person_equal> g;
    Person p[3] = {
        Person("Alice", "Shrdlu", 'G'),
        Person("Bob", "Etaoin", 'A'),
        Person("Carl", "Thug", 'B')
    };

    for(int i = 0; i < 3; ++i) {
        g.addNode(p[i]);
    }

    g.addEdge(p[0], p[1]);
    g.addEdge(p[1], p[0]);
    g.addEdge(p[2], p[0]);
    g.addEdge(p[2], p[1]);

    return g;
}

/**
 * @brief Test costruttori di Digraph.
 */
void constructorTest() {
    Digraph<int, Int_equal>* graph_int = nullptr;
    Digraph<Person, Person_equal>* graph_person = nullptr;

    // Costruttore di default
    try {
        graph_int = new Digraph<int, Int_equal> ();
    } catch (...) {
        std::cerr << "Errore durante l'allocazione del Digraph di interi, "
                  << "test costruttore di default fallito." << std::endl;
        throw;
    }
    assert(graph_int->nodesNumber() == 0);
    assert(graph_int->edgesNumber() == 0);
    delete graph_int; //test ~Digraph();
    graph_int = nullptr;

    try {
        graph_person = new Digraph<Person, Person_equal> ();
    } catch (...) {
        std::cerr << "Errore durante l'allocazione del Digraph di Person, "
                  << "test costruttore di default fallito." << std::endl;
        throw;
    }
    assert(graph_person->nodesNumber() == 0);
    assert(graph_person->edgesNumber() == 0);
    delete graph_person; //test ~Digraph();
    graph_person = nullptr;

    Digraph<int, Int_equal> tmp_int = testHelperInt();
    Digraph<Person, Person_equal> tmp_person = testHelperPerson();

    // Costruttore di copia
    try {
        graph_int = new Digraph<int, Int_equal> (tmp_int);
    } catch (...) {
        std::cerr << "Errore durante l'allocazione del Digraph di interi, "
                  << "test costruttore di copia fallito." << std::endl;
        throw;
    }
    assert(graph_int->nodesNumber() == tmp_int.nodesNumber());
    assert(graph_int->edgesNumber() == tmp_int.edgesNumber());
    assert(graph_int->exists(1) == tmp_int.exists(1));
    assert(graph_int->exists(-1) == tmp_int.exists(-1));
    assert(graph_int->hasEdge(1,2) == tmp_int.hasEdge(1,2));
    assert(graph_int->hasEdge(2,1) == tmp_int.hasEdge(2,1));
    //std::cout << (*graph_int) << std::endl;
    delete graph_int; //test ~Digraph();
    graph_int = nullptr;

    try {
        graph_person = new Digraph<Person, Person_equal> (tmp_person);
    } catch (...) {
        std::cerr << "Errore durante l'allocazione del Digraph di Person, " <<
            "test costruttore di copia fallito." << std::endl;
        throw;
    }
    assert(graph_person->nodesNumber() == tmp_person.nodesNumber());
    assert(graph_person->edgesNumber() == tmp_person.edgesNumber());
    //std::cout << (*graph_person) << std::endl;
    delete graph_person; //test ~Digraph();
    graph_person = nullptr;

    // Test accesso a costruttore privato
    //Digraph<Person, Person_equal> g(100);
}

/**
 * @brief Test assegnamento tra Digraph.
 */
void assignmentTest() {
    Digraph<int, Int_equal> g1;
    Digraph<int, Int_equal> g2;

    g1 = testHelperInt();
    g2 = g1;
    assert(g1.nodesNumber() == g2.nodesNumber());
    assert(g1.edgesNumber() == g2.edgesNumber());
    assert(g1.exists(1) == g2.exists(1));
    assert(g1.exists(-1) == g2.exists(-1));
    assert(g1.hasEdge(1,2) == g2.hasEdge(1,2));
    assert(g1.hasEdge(2,1) == g2.hasEdge(2,1));
    //std::cout << g1 << std::endl;
    //std::cout << g2 << std::endl;

    Digraph<Person, Person_equal> g3;
    Digraph<Person, Person_equal> g4;

    g3 = testHelperPerson();
    g4 = g3;
    assert(g3.nodesNumber() == g4.nodesNumber());
    assert(g3.edgesNumber() == g4.edgesNumber());
    //std::cout << g3 << std::endl;
    //std::cout << g4 << std::endl;
}

/**
 * @brief Test modifiche contenuto
 */
void editTest() {
    Digraph<int, Int_equal> g1;

    for(int i = 0; i < 200; ++i) {
        g1.addNode(i);
    }
    assert(g1.nodesNumber() == 200);
    //g1.addNode(0); // Test add nodo già esistente

    for(int i = 0; i < 200; i += 2) {
        for(int j = 1; j < 200; j += 2) {
            g1.addEdge(i, j);
        }
    }
    assert(g1.edgesNumber() == 10000);
    //g1.addEdge(0, 1); // Test add arco già esistente

    //g1.removeEdge(1, 0); // Test remove arco inesistente
    for(int i = 0; i < 200; i += 4) {
        for(int j = 1; j < 200; j += 4) {
            g1.removeEdge(i, j);
        }
    }
    assert(g1.edgesNumber() == 7500);

    //g1.removeNode(-1); // Test remove nodo inesistente
    for(int i = 0; i < 100; ++i) {
        g1.removeNode(i);
    }
    assert(g1.nodesNumber() == 100);
    assert(g1.edgesNumber() == 1875);

    for(int i = 0; i < 100; ++i) {
        assert(!g1.exists(i));
    }
    for(int i = 100; i < 200; ++i) {
        assert(g1.exists(i));
    }

    for(int i = 100; i < 200; ++i) {
        for(int j = 100; j < 200; ++j) {
            if((i % 2) == 0  && ((j-1) % 2) == 0 &&
                    ((i % 4) != 0 || ((j-1) % 4) != 0)) {
                assert(g1.hasEdge(i, j));
            }
            else {
                assert(!g1.hasEdge(i, j));
            }
        }
    }

    Digraph<Person, Person_equal> g2(testHelperPerson());

    g2.addNode(Person("Mario", "Rossi", 'V'));
    assert(g2.nodesNumber() == 4);
    //g2.addNode(Person("Mario", "Rossi", 'B')); // Test add nodo già esistente

    g2.addEdge(Person("Alice", "Shrdlu", 'G'), Person("Mario", "Rossi", 'V'));
    assert(g2.edgesNumber() == 5);

    g2.removeEdge(Person("Alice", "Shrdlu", 'G'),Person("Bob", "Etaoin", 'A'));
    g2.removeEdge(Person("Bob", "Etaoin", 'A'),Person("Alice", "Shrdlu", 'G'));
    assert(g2.edgesNumber() == 3);

    g2.removeNode(Person("Bob", "Etaoin", 'A'));
    g2.removeNode(Person("Alice", "Shrdlu", 'G'));
    assert(g2.nodesNumber() == 2);
    assert(g2.edgesNumber() == 0);

    assert(!g2.exists(Person("Alice", "Shrdlu", 'G')));
    assert(g2.exists(Person("Mario", "Rossi", 'V')));

    assert(!g2.hasEdge(Person("Mario", "Rossi", 'V'), 
        Person("Carl", "Thug", 'B')));
}

/**
 * @brief Test iteratori
 * 
 * Viene testato di conseguenza anche il metodo swap.
 */
void iteratorTest() {
    Digraph<Person, Person_equal> g1(testHelperPerson());
    Digraph<Person, Person_equal>::const_iterator i = g1.begin(),ie = g1.end();

    // Test operatore * e ->
    Digraph<Person, Person_equal> tmp(g1);
    for(i = g1.begin(), ie = g1.end(); i != ie; ++i) {
        tmp.removeNode(*i);
        //std::cout << (*i) <<std::endl;
        /*
        std::cout << i->_first_name << " " <<
            i->_last_name << " " <<
            i->_eyes_color << std::endl;
        */
    }
    assert(tmp.nodesNumber() == 0); // L'iteratore itera su tutti i nodi

    // Test pre incremento e assegnamento
    i = g1.begin();
    Digraph<Person, Person_equal>::const_iterator i_tmp;
    for(unsigned int j = 0; j < g1.nodesNumber(); ++j) {
        assert(i != ie);
        i_tmp = i;
        ++i_tmp;
        assert(i_tmp == ++i);
    }
    assert(i == ie);

    // Test post incremento e copy constructor
    i = g1.begin();
    for(unsigned int j = 0; j < g1.nodesNumber(); ++j) {
        Digraph<Person, Person_equal>::const_iterator tmp(i);
        
        assert(i != ie);
        assert(tmp == i++);
    }
    assert(i == ie);

    // Test const_iterator constness
    i = g1.begin();
    //(*i) = Person();
    //i->_first_name = "!!";
}

/**
 * @brief Test constness dei metodi
 */
void constnessTest() {
    const Digraph<Person, Person_equal> g(testHelperPerson()), tmp;

    //g = testHelperPerson();
    //g.addEdge(Person("Bob", "Etaoin", 'A'), Person("Carl", "Thug", 'B'));
    //g.addNode(Person("Mario", "Rossi", 'V'));
    g.begin();
    g.edgesNumber();
    g.end();
    g.exists(Person("Mario", "Rossi", 'V'));
    g.hasEdge(Person("Alice", "Shrdlu", 'G'), Person("Bob", "Etaoin", 'A'));
    g.nodesNumber();
    //g.removeEdge(Person("Bob", "Etaoin", 'A'), Person("Carl", "Thug", 'B'));
    //g.removeNode(Person("Mario", "Rossi", 'V'));
    //g.swap(tmp);
}

int main() {

#ifndef NDEBUG
    constructorTest();
    std::cout << "Test costruttori completati con successo." << std::endl;

    assignmentTest();
    std::cout << "Test assegnamento completati con successo." << std::endl;

    editTest();
    std::cout
        << "Test modifiche al contenuto del Digraph completate con successo."
        << std::endl;

    iteratorTest();
    std::cout << "Test iteratori completati con successo." << std::endl;
    
    constnessTest();
    std::cout << "Test constness completati con successo." << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Test completati con successo." << std::endl;
#endif

    return 0;
}