#ifndef digraph_h
#define digraph_h

#include <algorithm> // std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include <cassert> //assert
#include <ostream> // std::ostream


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
    T* _nodes; ///< Puntatore ad array contenente gli identificativi dei nodi
    unsigned int _nodes_number; ///< Numero di nodi
    bool** _adjacency_matrix; ///< Matrice di adiacenza
    unsigned int _edges_number; ///< Numero di archi

    E _equal;  ///< Istanza del funtore di uguaglianza

    /**
     * @brief Costruttore che inizializza un Digraph di dimensione specificata.
     * 
     * Costruttore PRIVATE che inizializza un Digraph avente il numero di nodi
     * specificato e privo di archi.
     * 
     * @param nodes_number Numero di nodi.
     * @post _nodes_number = nodes_number
     * @post _edges_number = 0
     * @throw eccezione di allocazione della memoria
     */
    explicit Digraph(const unsigned int& nodes_number): _nodes(nullptr),
            _nodes_number(0), _adjacency_matrix(nullptr), _edges_number(0) {
        
        if(nodes_number == 0) {
            return;
        }

        try {
            _nodes = new T[nodes_number];

            _adjacency_matrix = new bool*[nodes_number];
            for(unsigned int i = 0; i < nodes_number; ++i) {
                _adjacency_matrix[i] = nullptr;
            }
            for(unsigned int i = 0; i < nodes_number; ++i) {
                _adjacency_matrix[i] = new bool[nodes_number];
            }
        } catch(...) {
            clear();
            throw;
        }

        _nodes_number = nodes_number;
        for(unsigned int i = 0; i < nodes_number; ++i) {
            for(unsigned int j = 0; j < nodes_number; ++j) {
                _adjacency_matrix[i][j] = false;
            }
        }
    }

    /**
     * @brief Funzione che dealloca tutte le risorse allocate dinamicamente
     * 
     * La funzione mantiene la consistenza dell'oggetto. 
     */
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
        _edges_number = 0;
    }

    /**
     * @brief funzione che ritorna la posizione del nodo
     * 
     * @param u Nodo sorgende da cercare.
     * @return Posizione di u in _nodes, _nodes_number se u non presente.
     */
    unsigned int nodeIndex(const T& u) const {
        for(unsigned int i=0; i < _nodes_number; ++i) {
            if(_equal(_nodes[i], u)) {
                return i;
            }
        }
        return _nodes_number;
    }

    /**
     * @brief funzione che inverte lo stato dell'arco indicato.
     * 
     * Se l'arco esiste viene eliminato.
     * Se l'arco non esiste viene creato.
     * 
     * @param u Nodo sorgende dell'arco.
     * @param v Nodo destinazione dell'arco.
     */
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
     * @post _edges_number = 0
     */
    Digraph():_nodes(nullptr), _nodes_number(0), _adjacency_matrix(nullptr),
            _edges_number(0) {}

    /**
     * @brief Costruttore di copia.
     * 
     * @param other Digraph da copiare
     * @throw eccezione di allocazione della memoria
     */
    Digraph(const Digraph& other): _nodes(nullptr), _nodes_number(0),
            _adjacency_matrix(nullptr), _edges_number(0) {        
        Digraph tmp(other._nodes_number);

        for(unsigned int i = 0; i < tmp._nodes_number; ++i) {
            tmp._nodes[i] = other._nodes[i];
        }

        for(unsigned int i=0; i < tmp._nodes_number; ++i) {
            for(unsigned int j=0; j < tmp._nodes_number; ++j) {
                tmp._adjacency_matrix[i][j] = other._adjacency_matrix[i][j];
            }
        }
        tmp._edges_number = other._edges_number;

        swap(tmp);
    }

    /**
     * @brief Distruttore.
     * 
     * Dealloca tutte le risorse allocate dinamicamente.
     * 
     * @post _nodes = nullptr
     * @post _nodes_number = 0
     * @post _adjacent_matrix = nullptr
     * @post _edges_number = 0
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
        if(this != &other) {
            Digraph tmp(other);
            this->swap(tmp);
        }
        return *this;
    }

    /**
     * @brief Swap tra *this e il Digraph passato.
     * 
     * @param other Digraph con cui eseguire la swap
     */
    void swap(Digraph& other) {
        std::swap(_nodes, other._nodes);
        std::swap(_nodes_number, other._nodes_number);
        std::swap(_adjacency_matrix, other._adjacency_matrix);
        std::swap(_edges_number, other._edges_number);
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
        return _edges_number;
    }

    /**
     * @brief Inserimento del nodo nel grafo.
     * 
     * Il nodo viene inserito privo di archi entranti e uscenti (nodo isolato).
     * 
     * @param node Nodo da inserire.
     * @pre !exists(node)
     * @post _nodes_number = _nodes_number + 1
     * @throw Eccezione di allocazione di memoria.
     */
    void addNode(const T& node) {
        assert(!exists(node));

        Digraph tmp(_nodes_number + 1);

        for(unsigned int i = 0; i < _nodes_number; ++i) {
            tmp._nodes[i] = _nodes[i];
        }
        tmp._nodes[_nodes_number] = node;

        for(unsigned int i = 0; i < _nodes_number; ++i) {
            for(unsigned int j = 0; j < _nodes_number; ++j) {
                tmp._adjacency_matrix[i][j] = _adjacency_matrix[i][j];
            }
        }
        for(unsigned int i = 0; i < _nodes_number; ++i) {
            tmp._adjacency_matrix[i][_nodes_number] = false;
            tmp._adjacency_matrix[_nodes_number][i] = false;
        }
        tmp._edges_number = _edges_number;

        this->swap(tmp);
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

        Digraph tmp(_nodes_number - 1);
        unsigned int node_index = nodeIndex(node);

        for(unsigned int i = 0; i < tmp._nodes_number; ++i) {
            if(node_index > i) {
                tmp._nodes[i] = _nodes[i];
            }
            else{
                tmp._nodes[i] = _nodes[i+1];
            }
        }

        for(unsigned int i = 0; i < tmp._nodes_number; ++i) {
            for(unsigned int j = 0; j < tmp._nodes_number; ++j) {
                unsigned int k = i, l = j;
                if(k >= node_index) {
                    ++k;
                }
                if(l >= node_index) {
                    ++l;
                }
                tmp._adjacency_matrix[i][j] = _adjacency_matrix[k][l];
                
                if(tmp._adjacency_matrix[i][j]) {
                    ++tmp._edges_number;
                }
            }
        }

        this->swap(tmp);
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

        ++_edges_number;
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

        --_edges_number;
    }

    /**
     * @brief Determina l'esistenza di un nodo nel grafo. 
     * 
     * @param u Nodo da cercare.
     * @return true se il nodo è presente nel grafo, false altrimenti.
     */
    bool exists(const T& u) const {
        return nodeIndex(u) != _nodes_number;
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

    /**
     * @brief Iteratore costante per la classe Digraph.
     * 
     * Itera sugli identificativi dei nodi presenti nel grafo.
     */
    class const_iterator {
		const T* _ptr;

		friend class Digraph;

		/**
         * Costruttore privato di inizializzazione usato dalla classe container
         */
        explicit const_iterator(const T* node) : _ptr(node) {}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;

        /**
         * @brief Costruttore di default.
         */
		const_iterator() : _ptr(nullptr) {}
		
        /**
         * @brief Costruttore di copia.
         */
		const_iterator(const const_iterator& other) : _ptr(other._ptr) {}

        /**
         * @brief Operatore di assegnamento.
         */
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
		
		/**
         * @brief Operatore di iterazione pre-incremento (++i)
         */
		const_iterator& operator++() {
			++_ptr;
			return *this;
		}

		/**
         * @brief Operatore di iterazione post-incremento (i++)
         */
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			++_ptr;
			return tmp;
		}

		/**
         * @brief Operatore di uguaglianza
         */
		bool operator==(const const_iterator& other) const {
			return (_ptr == other._ptr);
		}
		
		/**
         * @brief Operatore di disuguaglianza
         */
		bool operator!=(const const_iterator& other) const {
			return (_ptr != other._ptr);
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
 * @param digraph Digraph da inviare
 * @return Reference allo stream di output
 */
template <typename T, typename E>
std::ostream& operator<<(std::ostream& os, const Digraph<T,E>& digraph) {
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