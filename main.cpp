#include <iostream> //std::cout
#include <cassert> //assert
#include <string> //std::string

#include "digraph.h"

/**
 * @brief Funtore di uguaglianza tra int
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

    Person():_eyes_color('B') {}

    Person( const std::string& first_name,
            const std::string& last_name,
            char eyes_color):
        _first_name(first_name),
        _last_name(last_name),
        _eyes_color(eyes_color) {}
};

/**
 * @brief Funtore di uguaglianza tra Person.
 */
struct Person_equal {
    bool operator()(const Person& a, const Person& b) const {
        return (a._first_name == b._first_name &&
            a._last_name == b._last_name);
    }
};

/**
 * @brief Funzione helper che genera un digraph di interi popolato.
 * 
 * Il grafo risultante rispecchia quello presente nella traccia del progetto. 
 */
Digraph<int, Int_equal> test_helper_int() {
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
Digraph<Person, Person_equal> test_helper_person() {
    Digraph<Person, Person_equal> g;
    Person p[3] = {
        {"Alice", "Shrdlu", 'G'},
        {"Bob", "Etaoin", 'A'},
        {"Carl", "Thug", 'B'}
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
void constructor_test() {
    Digraph<int, Int_equal>* graph_int = nullptr;
    Digraph<Person, Person_equal>* graph_person = nullptr;

    //Costruttore di default
    graph_int = new Digraph<int, Int_equal> ();
    assert(graph_int->nodesNumber() == 0);
    assert(graph_int->edgesNumber() == 0);
    delete graph_int;
    graph_int = nullptr;

    graph_person = new Digraph<Person, Person_equal> ();
    assert(graph_person->nodesNumber() == 0);
    assert(graph_person->edgesNumber() == 0);
    delete graph_person;
    graph_person = nullptr;

    Digraph<int, Int_equal> tmp_int = test_helper_int();
    Digraph<Person, Person_equal> tmp_person = test_helper_person();

    //Costruttore di copia
    graph_int = new Digraph<int, Int_equal> (tmp_int);
    assert(graph_int->nodesNumber() == tmp_int.nodesNumber());
    assert(graph_int->edgesNumber() == tmp_int.edgesNumber());
    assert(graph_int->exists(1) == tmp_int.exists(1));
    assert(graph_int->exists(-1) == tmp_int.exists(-1));
    assert(graph_int->hasEdge(1,2) == tmp_int.hasEdge(1,2));
    assert(graph_int->hasEdge(2,1) == tmp_int.hasEdge(2,1));
    //std::cout << (*graph_int) << std::endl;
    
    delete graph_int;
    graph_int = nullptr;

    graph_person = new Digraph<Person, Person_equal> (tmp_person);
    assert(graph_person->nodesNumber() == tmp_person.nodesNumber());
    assert(graph_person->edgesNumber() == tmp_person.edgesNumber());
    //std::cout << (*graph_person) << std::endl;
    
    delete graph_person;
    graph_person = nullptr;
}

/**
 * @brief Test assegnamento tra Digraph.
 */
void assignment_test() {
    Digraph<int, Int_equal> g1;
    Digraph<int, Int_equal> g2;

    g1 = test_helper_int();
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

    g3 = test_helper_person();
    g4 = g3;
    assert(g3.nodesNumber() == g4.nodesNumber());
    assert(g3.edgesNumber() == g4.edgesNumber());
    //std::cout << g3 << std::endl;
    //std::cout << g4 << std::endl;
}

/**
 * @brief Test modifiche contenuto
 */
void edit_test() {
    Digraph<int, Int_equal> g1 = test_helper_int();
    assert(g1.nodesNumber() == 6);
    assert(g1.edgesNumber() == 8);

    for(int i = 0; i < 200; ++i) {
        g1.addNode(-i);
    }
    assert(g1.nodesNumber() == 206);

    for(int i = 0; i < 200; i += 2) {
        for(int j = 1; j < 200; j += 2) {
            g1.addEdge(-i, -j);
        }
    }
    assert(g1.edgesNumber() == 10008);

    Digraph<Person, Person_equal> g2 = test_helper_person();

    assert(g2.nodesNumber() == 3);
    assert(g2.edgesNumber() == 4);

    g2.addNode(Person("Mario", "Rossi", 'V'));
    assert(g2.nodesNumber() == 4);

    g2.addEdge(Person("Alice", "Shrdlu", 'G'), Person("Mario", "Rossi", 'V'));
    assert(g2.edgesNumber() == 5);

    //TODO test removes
}

int main()
{
#ifndef NDEBUG
    constructor_test();
    std::cout << "Test costruttori completati con successo." << std::endl;

    assignment_test();
    std::cout << "Test assegnamento completati con successo." << std::endl;

    edit_test();
    std::cout <<
        "Test modifiche al contenuto del Digraph completate con successo." <<
        std::endl;

    //todo test constness

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Test completati con successo." << std::endl;
#endif

    return 0;
}

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
std::ostream &operator<<(std::ostream &os, const Person& p) {
	os << p._first_name;
	return os;
}