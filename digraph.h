#ifndef digraph_h
#define digraph_h

#include <algorithm> // std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include <cassert>  //assert


/**
 * Classe che implementa un grafo orientato con nodi di tipo T.
 * Gli archi vengono memorizzati tramite matrice di adiacenza.
 * L'uguaglianza tra due dati di tipo T è verificata usando un funtore di
 * uguaglianza E.
 * 
 * @brief Grafo orientato contenente nodi di tipo T.
 * 
 * @param T tipo dei nodi
 * @param E funtore di uguaglianza tra due nodi
 */

template <typename T, typename E>
class Digraph {
    T* _nodes; ///< Puntatore ad array contenente i nodi
    unsigned int _nodes_number; ///< Numero di nodi
    bool** _adjacency_matrix; ///< Matrice di adiacenza

    E _equal;  ///< Istanza del funtore di uguaglianza 

    void clear() {
        delete[] _nodes;
        _nodes = nullptr;

        for(unsigned int i = 0; i < _nodes_number; ++i) {
            delete[] _adjacency_matrix[i];
            _adjacency_matrix[i] = nullptr;
        }
        delete[] _adjacency_matrix;
        _adjacency_matrix = nullptr;

        _nodes_number = 0;
    }

    void clear_temps(T*& tmp_nodes, unsigned int tmp_nodes_number,
            bool**& tmp_matrix) const {
        delete[] tmp_nodes;
        tmp_nodes = nullptr;

        for(unsigned int i = 0; i < tmp_nodes_number; ++i) {
            delete[] tmp_matrix[i];
            tmp_matrix[i] = nullptr;
        }
        delete[] tmp_matrix;
        tmp_matrix = nullptr;
    }

    unsigned int nodeIndex(const T& u) const {
        assert(exists(u));

        for(unsigned int i=0; i < _nodes_number; ++i) {
            if(_equal(_nodes[i], u)) {
                return i;
            }
        }

        //nodo non trovato, in caso la assert venga ignorata dal compilatore
        return _nodes_number;
    }

    void setEdge(const T& u, const T& v) {
        assert(exists(u));
        assert(exists(v));

        unsigned int u_ind = nodeIndex(u);
        unsigned int v_ind = nodeIndex(v);

        _adjacency_matrix[u_ind][v_ind] = !_adjacency_matrix[u_ind][v_ind];
    }

public:

    /**
     * Costruttore di default, inizializza un grafo vuoto senza nodi e senza
     * archi.
     * 
     * @brief Costruttore default, genera un grafo vuoto
     * @post _nodes = nullptr
     * @post _nodes_number = 0
     * @post _adjacency_matrix = nullptr
     */
    Digraph():_nodes(nullptr), _nodes_number(0), _adjacency_matrix(nullptr) {}

    Digraph(const Digraph& other):
            _nodes(nullptr), _nodes_number(0), _adjacency_matrix(nullptr) {
        try{
            for(unsigned int i=0; i<other._nodes_number; ++i) {
                addNode(other._nodes[i]);
            }
        } catch(...) {
            clear();
            throw;
        }

        for(unsigned int i=0; i<_nodes_number; ++i) {
            for(unsigned int j=0; j<_nodes_number; ++j) {
                _adjacency_matrix[i][j] = other._adjacency_matrix[i][j];
            }
        }
    }

    ~Digraph() {
        this->clear();
    }

    Digraph& operator= (const Digraph& other) {
        //TODO
        return this;
    }

    void addNode(const T& node) {
        assert(!exists(node));

        T* old_nodes = _nodes;
        bool** old_matrix = _adjacency_matrix;

        try {
            _nodes = new T[_nodes_number + 1];
        } catch(...) {
            _nodes = old_nodes;
            throw;
        }

        try {
            _adjacency_matrix = new bool*[_nodes_number + 1];
        } catch(...) {
            delete[] _nodes;
            _nodes = old_nodes;
            _adjacency_matrix = old_matrix;
            throw;
        }

        for(unsigned int i = 0; i < _nodes_number+1; ++i) {
            try{
                _adjacency_matrix[i] = new bool[_nodes_number+1];
            } catch(...) {
                delete[] _nodes;
                _nodes = old_nodes;
                for(unsigned int j = 0; j < i; ++j) {
                    delete[] _adjacency_matrix[j];
                    _adjacency_matrix[j] = nullptr;
                }
                delete _adjacency_matrix;
                _adjacency_matrix = old_matrix;
                throw;
            }
        }

        for(unsigned int i = 0; i<_nodes_number; ++i) {
            _nodes[i] = old_nodes[i];
        }
        _nodes[_nodes_number] = node;

        for(unsigned int i = 0; i<_nodes_number; ++i) {
            for(unsigned int j = 0; j<_nodes_number; ++j) {
                _adjacency_matrix[i][j] = old_matrix[i][j];
            }
        }
        for(unsigned int i = 0; i<_nodes_number+1; ++i) {
            _adjacency_matrix[i][_nodes_number] = false;
            _adjacency_matrix[_nodes_number][i] = false;
        }

        clear_temps(old_nodes, _nodes_number, old_matrix);

        ++_nodes_number;
    }

    void removeNode(const T& node) {
        assert(exists(node));

        unsigned int node_index = nodeIndex(node);
        T* old_nodes = _nodes;
        bool** old_matrix = _adjacency_matrix;

        try {
            _nodes = new T[_nodes_number - 1];
        } catch(...) {
            _nodes = old_nodes;
            throw;
        }

        try {
            _adjacency_matrix = new bool*[_nodes_number - 1];
        } catch(...) {
            delete[] _nodes;
            _nodes = old_nodes;
            _adjacency_matrix = old_matrix;
            throw;
        }

        for(unsigned int i = 0; i < _nodes_number-1; ++i) {
            try{
                _adjacency_matrix[i] = new bool[_nodes_number - 1];
            } catch(...) {
                delete[] _nodes;
                _nodes = old_nodes;
                for(unsigned int j = 0; j < i; ++j) {
                    delete[] _adjacency_matrix[j];
                    _adjacency_matrix[j] = nullptr;
                }
                delete _adjacency_matrix;
                _adjacency_matrix = old_matrix;
                throw;
            }
        }

        for(unsigned int i = 0; i<_nodes_number-1; ++i) {
            if(node_index > i) {
                _nodes[i] = old_nodes[i];
            }
            else{
                _nodes[i] = old_nodes[i+1];
            }
        }

        for(unsigned int i = 0; i<_nodes_number-1; ++i) {
            for(unsigned int j = 0; j < _nodes_number-1; ++j) {
                unsigned int k = i, l = j;
                if(k >= node_index) {
                    ++k;
                }
                if(l >= node_index) {
                    ++l;
                }
                _adjacency_matrix[i][j] = old_matrix[k][l];
            }
        }

        clear_temps(old_nodes, _nodes_number, old_matrix);

        --_nodes_number;
    }

    void addEdge(const T& u, const T& v) {
        assert(!hasEdge(u, v));

        setEdge(u, v);
    }

    void removeEdge(const T& u, const T& v) {
        assert(hasEdge(u, v));

        setEdge(u, v);
    }

    unsigned int nodesNumber() const {
        return _nodes_number;
    }

    bool exists(const T& u) const {
        for(unsigned int i=0; i < _nodes_number; ++i) {
            if(_equal(_nodes[i], u)) {
                return true;
            }
        }
        return false;
    }

    bool hasEdge(const T& u, const T& v) const {
        if(!exists(u) || !exists(v)) {
            return false;
        }

        unsigned int u_index = nodeIndex(u);
        unsigned int v_index = nodeIndex(v);

        return _adjacency_matrix[u_index][v_index];
    }

    typedef const T* const_iterator;

    /**
     * Ritorna l'iteratore all'inizio della sequenza di nodi
     */
	const_iterator begin() const {
		return _nodes;
	}
	
	/**
     * Ritorna l'iteratore alla fine della sequenza di nodi
     */
	const_iterator end() const {
		return _nodes + _nodes_number;
	}

}; //class Digraph

/**
	Ridefinizione dell'operatore di stream per la stampa del contenuto del
    Digraph

	@param os Stream di output
	@param ol Digraph da stampare
	@return Reference allo stream di output
*/
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, const Digraph<T,E> &digraph) {
	typename Digraph<T,E>::const_iterator i_row, i_column;

	for(i_row  = digraph.begin(); i_row != digraph.end(); ++i_row) {
        os << "\t" << *i_row;        
    }

    for(i_column  = digraph.begin(); i_column != digraph.end(); ++i_column) {
        os << std::endl;
        os << *i_column;
        for(i_row  = digraph.begin(); i_row != digraph.end(); ++i_row) {
            os << "\t" << digraph.hasEdge(*i_row, *i_column);
        }
    }

	return os;
}

#endif //digraph_h