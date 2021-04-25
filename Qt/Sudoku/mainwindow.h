#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIntValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_resolve_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_previous_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_begin_clicked();

    void on_pushButton_end_clicked();

private:
    QIntValidator validator;

    short sudoku[9][9]; ///< Matrice contenente il Sudoku

    /**
     * @brief Struct contenente una mossa.
     */
    struct Move {
        int row;
        int column;
        short value;

        Move(): row(0), column(0), value(0) {}
        Move(int r, int c, int v): row(r), column(c), value(v) {}
    };
    QVector<Move> moves; ///< Mosse effettuate dal solver
    QVector<Move>::const_iterator current_move;

    Ui::MainWindow *ui;

    /**
     * @brief Funzione che risolve il Sudoku in modo ricorsivo.
     * @return true se il sudoku è stato risolto, false altrimenti.
     */
    bool solve();

    /**
     * @brief Salva i valori inseriti tramite UI nella variabile sudoku.
     */
    void save_sudoku();

    /**
     * @brief Funzione che controlla la validità di una cella nel Sudoku.
     * @param row Riga della cella da validare
     * @param column Colonna della cella da validare
     * @return true se la cella rispetta le regole del Sudoku, false altrimenti.
     */
    bool valid_cell(const int& row, const int& column);

    /**
     * @brief Mostra il contenuto di Sudoku tramite UI.
     * Mostra nell'interfaccia grafica i valori ottenuti dal solver con il
     * colore rosso.
     */
    void print_sudoku();

    /**
     * @brief Trova una cella vuota nel sudoku.
     * I parametri row e column vengono usati come output se viene trovata una
     * cella vuota.
     *
     * @param row Riga della cella eventualmente trovata.
     * @param column Colonna della cella eventualmente trovata
     * @return true se è stata trovata una cella vuota, false altrimenti.
     */
    bool find_empty_cell(int& row, int& column);
};
#endif // MAINWINDOW_H
