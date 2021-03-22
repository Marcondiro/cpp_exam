# Grafo Orientato: scelte implementative
## Progetto d'esame del corso di **Programmazione C++** appello di aprile a.a. 2020-2021 UniMiB
### *Marco Cavenati 845899 m.cavenati1@campus.unimib.it*

---

## 1. Classe Digraph
La classe deve implementare un **grafo orienatato**.
Dato che nella traccia del progeto è indicato che
>I nodi  sono  rappresentati  da  un generico identificativo

la classe è definita **templata**, il parametro `T` definisce il tipo di dato degli identificativi dei nodi.  
Inoltre, deve essere possibile determinare l'uguaglianza tra due identificativi. Al fine di mantenere la classe il più generica possibile e non obbligare l'utente ad implementare l'operatore di confronto `==` per il tipo T, viene utilizzato un funtore templato `E`.  
Conseguentemente a queste scelte la classe è definita interamente nel file digraph.h.

### Attributi
Internamente alla classe il grafo viene rappresentato utilizzando:
- `_nodes` Un **array dinamico** di tipo `T` contenente gli identificativi dei nodi, esso rappresente l'intestazione delle righe e delle colonne della matrice di adiacenza.
- `_adj_matrix` Un **array bidimensionale dinamico** di tipo `bool` che rappresenta il contenuto della matrice di adiacenza.

Questa scelta è stata fatta per rispettare la traccia:
> Il  grafo  deve  essere  implementato mediante matrici di adiacenza come in figura. Non possono essere usate liste.

Altri attributi della classe sono
- `_nodes_nmumber` Numero dei nodi
- `_edges_number` Numero degli archi
- `_equal` Istanza del funtore di uguaglianza

Il numero degli archi poteva essere ricalcolato all'occorrenza, ma dato che il suo salvataggio non comporta un eccessivo utilizzo di risorse sia per lo spazio in memoria sia per l'aggiornamento del dato, ho scelto di definire un attributo della classe.

### Metodi
Nella sezione `private` della classe ho definito i seguenti metodi:
- Un **costruttore** che ha come parametro il numero di nodi del grafo.  
La definizione di questo costruttore si è resa necessaria durante lo sviluppo dei metodi `addNode()` e `removeNode()` in quanto in entrambi i metodi è necessario allocare un nuovo grafo con matrice di adiacenza di dimensioni differenti da quella corrente del grafo.  
Questo costruttore è l'unico metodo della classe che alloca risorse sullo heap, rendendo più facile la gestione della memoria.  
Il metodo è definito privato perchè l'array degli identificativi istanziato non viene inizializzato, se questo costruttore fosse accessibile all'esterno si otterrebbe un Digraph **non consistente**. I metodi della classe che utilizzano questo costruttore **devono** occuparsi dell'inizializzazione dell'array degli identificativi.  
In caso di fallimento dell'allocazione di memoria il costruttore si occupa di ripristinare uno stato coerente della memoria e dell'istanza di Digraph tramite il metodo clear() e rilancia l'eccezione al chiamante.  
Ho utilizzato la keyword explicit per evitare di utilizzare il costruttore in modo implicito per errore.
- Il metodo `clear` si occupa della deallocazione di tutte le risorse della classe allocate dinamicamente. Al termine della sua esecuzione si ottiene un grafo vuoto, senza archi e nodi, ma l'oggetto risulta comunque coerente perchè il metodo aggiorna anche gli attributi `_nodes_nmumber` e `_edges_number`.
- Il metodo `nodeIndex` è un metodo privato che prende in input un oggetto di tipo `T` e ritorna la sua posizione all'interno dell'array `_nodes` sfruttando il funtore `_equal` per fare i confronti. Questa informazione è utile in diversi metodi pubblici della classe Digraph. Il metodo è definito privato in quanto questa informazione non ha utilità all'esterno della classe stessa.  
Nel caso il nodo non fosse presente nel grafo viene restituito il valore di `_nodes_number`. Il valore di ogni indice valido sarà minore di `_nodes_number`, questo ci permette di utilizzare il suo valore per indicare che il nodo non è presente.
- Il metodo `setEdge` si è reso utile durante lo sviluppo dei metodi `addEdge` e `removeEdge`. Questo metodo evita di avere codice duplicato nei due metodi. Il metodo prende in input due vertici che identificano un arco e se tale arco esiste viene eliminato, se non esiste viene creato.

Nella sezione `public` della classe ho definito i seguenti metodi:
- Il **costruttore** di default che istanzia un grafo vuoto.
- Il **costruttore** di copia. Esso sfrutta al suo interno il costruttore privato per generare un grafo di dimensione pari a quella del grafo da copiare e successivamente effettua la copia di nodi e archi.
- Il **distruttore** semplicemente richiama il metodo `clear`.
- L'operatore di assegnamento `operator=`, dopo aver fatto il controllo dell'autoassegnamento, sfrutta il costruttore di copia e il meotodo `swap` per effettuare l'assegnamento.
- Il metodo `swap` effettua la swap membro a membro tra this e un Digraph passato per parametro. Questo metodo è molto utile anche all'interno della classe stessa.
- Il metodo `nodesNumber` restituisce il numero di nodi del grafo.
- Il metodo `edgesnumber` restituisce il numero di archi del grafo.
- Il metodo `addNode` permette di aggiungere un nodo al grafo, se non già presente. Utilizza al suo interno il costruttore privato per istanziare un grafo con un nodo in più del grafo corrente (ovvero con una matrice di adiacenza con una riga e una colonna in più). Dopodichè copia tutti i dati del grafo corrente aggiungendo il nuovo nodo "in coda". Il nodo viene inserito come nodo isolato, privo di archi entranti e uscenti.
- Il metodo `removeNode` permette di rimuovere un nodo dal grafo, se presente. Utilizza al suo interno il costruttore privato per istanziare un grafo con un nodo in meno del grafo corrente. Dopodichè copia tutti i dati del grafo corrente rimuovendo il nodo scelto e i relativi archi. In questo metodo è necessario ricalcolare il numero degli archi.
- Il metodo `addEdge` aggiunge un arco al grafo, se non già presente. Viene sfruttato il metodo privato `setEdge`.
- Il metodo `removeEdge` rimuove un arco dal grafo, se presente. Viene sfruttato il metodo privato `setEdge`.
- Il metodo `exists` è un metodo booleano che ritorna true se il nodo passato in input è presente nel grafo. La sua implementazione è molto semplice perchè sfrutta il metodo privato `nodeIndex`.
- Il metodo `hasEdge` è un metodo booleano che ritorna true se l'arco passato in input è presente nel grafo, consultando la matrice di adiacenza. Anch'esso sfrutta il metodo `nodeIndex`.

I controlli di validità dei parametri  in input vengono implementati tramite asserzioni.

Come da specifiche ho implementato un **iteratore costante** di tipo **forward**. Questo iteratore è di fatto un wrapper di un puntatore costante a `T` che ne restringe le funzionalità a quelle richieste.
- Il metodo `begin` ritorna un iteratore all'inizio della sequenza di nodi sfruttando il costruttore privato di const_iterator.
- Il metodo `end` similmente a `begin` ritorna un iteratore alla fine della sequenza di nodi.

Al fine di facilitare le operazione di testing e debug ho ridefinito l'operatore di stream `<<` che permette di stampare un oggetto Digraph formattato similmente alla matrice di adiacenza rappresentata nella traccia del progetto.

## 2. Tests
Nel file `main.cpp` sono stati definiti diversi test per la classe Digraph, raggruppati in funzioni globali.

Digraph è stata testa con nodi di tipo int e nodi di tipo Person, una struct custom. Per entrambi i tipi di dato è stato definito un funtore di uguaglianza.

I test fanno uso di asserzioni, lo standard output viene utilizzato per comunicare il superamento di un blocco di test.

Nello specifico i blocchi di test implementati sono:
- `constructorTest` testa i costruttori.
- `assignmentTest` testa l'operatore di assegnamento.
- `editTest` è il set di test più corposo che verifica la correttezza delle operazioni di aggiunta/rimozione di nodi e archi nel grafo.
- `iteratorTest` testa l'iteratore.
- `constnessTest` verifica la corretta definizione dei metodi come const o non const.

La coerenza delle dimensioni `nodesNumber()` e `edgesNumber` viene verificata in diversi blocchi di test.

## 3. Doxyfile
Il file Doxyfile è molto semplice e permette di generare documentazione HTML sfruttando i commenti del codice.

## 4. Makefile
Per la compilazione utilizzo l'opzione `-std=c++0x` di g++ per motivi di compatibilità con il server di test.  
Il Makefile è stato arricchito con alcune funzioni `PHONY` utili durante lo sviluppo:
- `clear` elimina i file `.o` e `.exe` prodotti dal compilatore
- `docs` richiama doxygen per la creazione della documentazione
- `run` esegue direttamente il `main.exe` al termine della compilazione
- `run_v` esegue **valgrind** per analizzare il main.exe dopo la compilazione.