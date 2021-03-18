#ifndef digraph_h
#define digraph_h

#include <algorithm> // std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include <cassert>  //assert


/**
 * @brief Grafo orientato contenente nodi di tipo T.
 * 
 * Classe che implementa un grafo orientato con nodi di tipo T.
 * Gli archi vengono memorizzati tramite matrice di adiacenza.
 * L'uguaglianza tra due dati di tipo T è verificata usando un funtore di
 * uguaglianza E.
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
     * @brief Costruttore default, genera un grafo vuoto.
     * 
     * Inizializza un grafo vuoto senza nodi e senza archi.
     * 
     * @post _nodes = nullptr
     * @post _nodes_number = 0
     * @post _adjacency_matrix = nullptr
     */
    Digraph():_nodes(nullptr), _nodes_number(0), _adjacency_matrix(nullptr) {}

    /**
     * @brief Costruttore di copia.
     * 
     * @param other Digraph da copiare
     * @throw eccezione di allocazione della memoria
     */
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

    /**
     * @brief Distruttore.
     * 
     * Dealloca tutte le risorse allocate dinamicamente.
     * 
     * @post _nodes = nullptr
     * @post _nodes_number = 0
     * @post _adjacent_matrix = nullptr
     */
    ~Digraph() {
        clear();
    }

    /**
     * @brief Operatore di assegnamento.
     * 
     * @param other Digraph da assegnare.
	 * @return reference al Digraph this.
	 * @throw Eccezione di allocazione della memoria.
     */
    Digraph& operator= (const Digraph& other) {
        Digraph* tmp = new Digraph(other);
        
        std::swap(_nodes, tmp->_nodes);
        std::swap(_nodes_number, tmp->_nodes_number);
        std::swap(_adjacency_matrix, tmp->_adjacency_matrix);

        return *this;
    }

    /**
     * @brief Ritorna il numero di nodi nel grafo.
     * 
     * @return  Numero di nodi.
	 */
    unsigned int nodesNumber() const {
        return _nodes_number;
    }

    /**
     * @brief Ritorna il numero di archi nel grafo.
     * 
     * @return  Numero di archi.
	 */
    unsigned int edgesNumber() const {
        unsigned int edges = 0;
        for(unsigned int i=0; i < _nodes_number; ++i) {
            for(unsigned int j=0; j < _nodes_number; ++j) {
                if(_adjacency_matrix[i][j]) {
                    ++edges;
                }
            }
        }

        return edges;
    }

    /**
     * @brief Inserimento del nodo nel grafo.
     * 
     * Il nodo viene inserito privo di archi entranti e uscenti (nodo isolato).
     * 
     * @param node Nodo da inserire.
     * @post _nodes_number = _nodes_number + 1
     * @throw Eccezione di allocazione di memoria.
     */
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

    /**
     * @brief Eliminazione del nodo dal grafo.
     * 
     * Il nodo e tutti i relativi archi entranti o uscenti vengono rimossi dal
     * grafo.
     * 
     * @param node Nodo da rimuovere.
     * @pre exists(node)
     * @post _nodes_number = _nodes_number - 1
     * @throw Eccezione di allocazione di memoria.
     */
    void removeNode(const T& node) {
        assert(exists(node));

        if(_nodes_number == 1) {
            clear();
            return;
        }

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

    /**
     * @brief Aggiunta dell'arco al grafo.
     * 
     * @param u Nodo sorgende dell'arco da aggiungere.
     * @param v Nodo destinazione dell'arco da aggiungere.
     * @pre exists(u)
     * @pre exists(v)
     * @pre !hasEdge(u, v)
     * @post hasEdge(u, v)
     */
    void addEdge(const T& u, const T& v) {
        assert(!hasEdge(u, v));

        setEdge(u, v);
    }

    /**
     * @brief Eliminazione dell'arco dal grafo.
     * 
     * @param u Nodo sorgende dell'arco da rimuovere.
     * @param v Nodo destinazione dell'arco da rimuovere.
     * @pre exists(u)
     * @pre exists(v)
     * @pre hasEdge(u, v)
     * @post !hasEdge(u, v)
     */
    void removeEdge(const T& u, const T& v) {
        assert(hasEdge(u, v));

        setEdge(u, v);
    }

    /**
     * @brief Determina l'esistenza di un nodo nel grafo. 
     * 
     * @param u Nodo da cercare.
     * @return true se il nodo è presente nel grafo, false altrimenti.
     */
    bool exists(const T& u) const {
        for(unsigned int i=0; i < _nodes_number; ++i) {
            if(_equal(_nodes[i], u)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Determina l'esistenza di un arco nel grafo. 
     * 
     * @param u Nodo sorgende dell'arco da cercare.
     * @param v Nodo destinazione dell'arco da cercare.
     * @pre exists(u)
     * @pre exists(v)
     * @return true se l'arco è presente nel grafo, false altrimenti.
     */
    bool hasEdge(const T& u, const T& v) const {
        assert(exists(u));
        assert(exists(v));

        unsigned int u_index = nodeIndex(u);
        unsigned int v_index = nodeIndex(v);

        return _adjacency_matrix[u_index][v_index];
    }

    class const_iterator {
		const T* _ptr;

        // La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class Digraph;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		explicit const_iterator(const T* node) : _ptr(node) {}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;

		const_iterator() : _ptr(nullptr) {}
		
		const_iterator(const const_iterator& other) : _ptr(other._ptr) {}

		const_iterator& operator=(const const_iterator& other) {
			_ptr = other._ptr;
			return *this;
		}

		/**
         * @brief Ritorna il dato riferito dall'iteratore (dereferenziamento).
         */
		reference operator*() const {
			return *_ptr;
		}

		/**
         * @brief Ritorna il puntatore al dato riferito dall'iteratore.
         */
		pointer operator->() const {
			return _ptr;
		}
		
		// Operatore di iterazione post-incremento (i++)
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			++_ptr;
			return tmp;
		}

		// Operatore di iterazione pre-incremento (++i)
		const_iterator& operator++() {
			++_ptr;
			return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (_ptr == other._ptr);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return !(*this == other);
		}
	}; // classe const_iterator

    /**
     * @brief Ritorna l'iteratore all'inizio della sequenza di nodi
     * 
     * @return const_iterator all'inizio della sequenza di nodi
     */
	const_iterator begin() const {
		return const_iterator(_nodes);
	}
	
	/**
     * @brief Ritorna l'iteratore alla fine della sequenza di nodi
     * 
     * @return const_iterator alla fine della sequenza di nodi
     */
	const_iterator end() const {
		return const_iterator(_nodes + _nodes_number);
	}

}; //class Digraph

/**
 * @brief Invia il Digraph sullo stream
 * 
 * Ridefinizione dell'operatore di stream per l'invio del contenuto del
 * Digraph sullo stream rappresentato come matrice di adiacenza.
 * 
 * @param os Stream di output
 * @param ol Digraph da inviare
 * @return Reference allo stream di output
 */
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, const Digraph<T,E> &digraph) {
	typename Digraph<T,E>::const_iterator i_row, i_column;

	for(i_column  = digraph.begin(); i_column != digraph.end(); ++i_column) {
        os << "\t" << *i_column;        
    }

    for(i_row  = digraph.begin(); i_row != digraph.end(); ++i_row) {
        os << std::endl;
        os << *i_row;
        for(i_column  = digraph.begin(); i_column != digraph.end(); ++i_column)
        {
            os << "\t" << digraph.hasEdge(*i_row, *i_column);
        }
    }

	return os;
}

#endif //digraph_h