#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setFixedSize(700, 700); // Dimensioni finestra fisse

    // Blocco l'inserimento di caratteri non numerici
    validator.setRange(1,9);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QWidget *w = ui->gridLayout->itemAtPosition(i, j)->widget();
            QLineEdit* cell = static_cast<QLineEdit*>(w);
            cell->setValidator(&validator);
        }
    }
}

MainWindow::~MainWindow() {
    moves.clear();
    delete ui;
}

void MainWindow::on_pushButton_resolve_clicked()
{
    // Disabilito il bottone risolvi
    ui->pushButton_resolve->setEnabled(false);

    // Setto le celle in sola lettura
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QWidget *w = ui->gridLayout->itemAtPosition(i, j)->widget();
            QLineEdit* cell = static_cast<QLineEdit*>(w);
            cell->setReadOnly(true);
        }
    }

    save_sudoku();

    //Controllo se il sudoku in input è valido
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (sudoku[i][j] != 0 && !valid_cell(i, j)) {
                QMessageBox::warning(this, tr("Errore"),
                                     tr("Sudoku inserito non valido."));
                return;
            }
        }
    }

    if (solve()) {
        print_sudoku();
    }
    else{
        QMessageBox::warning(this, tr("Errore"),
                             tr("Impossibile risolvere il Sudoku."));
        return;
    }

    //Abilito la navigazione tra le mosse
    current_move = moves.cbegin();
    if(current_move != moves.cend()) {
        ui->pushButton_previous->setEnabled(true);
        ui->pushButton_begin->setEnabled(true);
    }
}

void MainWindow::on_pushButton_clear_clicked() {
    // Svuoto pila mosse
    current_move = nullptr;
    moves.clear();

    // Disabilito navigazione mosse
    ui->pushButton_previous->setEnabled(false);
    ui->pushButton_begin->setEnabled(false);
    ui->pushButton_next->setEnabled(false);
    ui->pushButton_end->setEnabled(false);

    // Pulisco griglia e riabilito inserimento
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QWidget* w = ui->gridLayout->itemAtPosition(i, j)->widget();
            QLineEdit* cell = static_cast<QLineEdit*>(w);
            cell->clear();
            cell->setStyleSheet("color: balck");
            cell->setReadOnly(false);
        }
    }

    // Abilito bottone risolvi
    ui->pushButton_resolve->setEnabled(true);
}

void MainWindow::on_pushButton_previous_clicked()
{
    //Rimuovo mossa più recente
    QWidget* w = ui->gridLayout->itemAtPosition(
                current_move->row,
                current_move->column
            )->widget();
    QLineEdit* cell = static_cast<QLineEdit*>(w);
    cell->setText("");

    ++current_move;
    //Se non ci sono mosse precedenti disattivo il bottone.
    if(current_move == moves.cend()) {
        ui->pushButton_previous->setEnabled(false);
        ui->pushButton_begin->setEnabled(false);
    }
    //Abilito bottone next e end
    ui->pushButton_next->setEnabled(true);
    ui->pushButton_end->setEnabled(true);
}

void MainWindow::on_pushButton_begin_clicked()
{
    //Rimuovo tutte le mosse
    while(current_move != moves.cend()) {
        QWidget* w = ui->gridLayout->itemAtPosition(
                    current_move->row,
                    current_move->column
                )->widget();
        QLineEdit* cell = static_cast<QLineEdit*>(w);
        cell->setText("");
        ++current_move;
    }

    //Non ci sono mosse precedenti, disattivo i bottoni.
    ui->pushButton_previous->setEnabled(false);
    ui->pushButton_begin->setEnabled(false);
    //Abilito bottone next e end
    ui->pushButton_next->setEnabled(true);
    ui->pushButton_end->setEnabled(true);
}

void MainWindow::on_pushButton_next_clicked()
{
    --current_move;
    //Se non ci sono mosse successive disattivo il bottone.
    if(current_move == moves.cbegin()) {
        ui->pushButton_next->setEnabled(false);
        ui->pushButton_end->setEnabled(false);
    }
    //Abilito bottone previous e begin
    ui->pushButton_previous->setEnabled(true);
    ui->pushButton_begin->setEnabled(true);

    //Mostro la mossa successiva
    QWidget *w = ui->gridLayout->itemAtPosition(
                current_move->row,
                current_move->column
            )->widget();
    QLineEdit* cell = static_cast<QLineEdit*>(w);
    cell->setText(QString::number(current_move->value));
}

void MainWindow::on_pushButton_end_clicked()
{
    //Mostro tutte le mosse
    while(current_move != moves.cbegin()) {
        --current_move;
        QWidget *w = ui->gridLayout->itemAtPosition(
                    current_move->row,
                    current_move->column
                )->widget();
        QLineEdit* cell = static_cast<QLineEdit*>(w);
        cell->setText(QString::number(current_move->value));
    }

    //Non ci sono mosse successive, disattivo i bottoni.
    ui->pushButton_next->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    //Abilito bottone next e end
    ui->pushButton_previous->setEnabled(true);
    ui->pushButton_begin->setEnabled(true);
}

bool MainWindow::solve() {
    int row, column;
    if (!find_empty_cell(row, column)) {
        // Tutte le celle sono state risolte
        return true;
    }

    sudoku[row][column] = 1;
    while (sudoku[row][column] <= 9) {
        if (valid_cell(row, column) && solve()) {
            moves.append(Move(row, column, sudoku[row][column]));
            return true;
        }
        ++(sudoku[row][column]);
    }

    sudoku[row][column] = 0;
    return false;
}

bool MainWindow::find_empty_cell(int& row, int& column) {
    for (row = 0; row < 9; ++row) {
        for (column = 0; column < 9; ++column) {
            if (sudoku[row][column] == 0) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::save_sudoku(){
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QWidget *w = ui->gridLayout->itemAtPosition(i, j)->widget();
            QLineEdit* cell = static_cast<QLineEdit*>(w);
            if (cell->text().length() > 0) {
                sudoku[i][j] = cell->text().toShort();
            }
            else {
                sudoku[i][j] = 0; //Cella non compilata
            }
        }
    }
}

bool MainWindow::valid_cell(const int& row, const int& column) {
    short number = sudoku[row][column];

    //Controllo riga
    for (int i = 0; i < 9; ++i) {
        if (i != column && sudoku[row][i] == number) {
            return false;
        }
    }

    //Controllo colonna
    for (int i = 0; i < 9; ++i) {
        if (i != row && sudoku[i][column] == number) {
            return false;
        }
    }

    //Controllo casella 3x3
    int box_row = row - (row % 3);
    int box_column = column - (column % 3);
    for (int i = box_row; i < box_row + 3; ++i) {
        for (int j = box_column; j < box_column + 3; ++j) {
            if ((i != row || j != column) && sudoku[i][j] == number) {
                return false;
            }
        }
    }

    return true;
}

void MainWindow::print_sudoku() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QWidget *w = ui->gridLayout->itemAtPosition(i, j)->widget();
            QLineEdit* cell = static_cast<QLineEdit*>(w);
            if(cell->text().isEmpty() || cell->text().toInt() == 0){
                cell->setStyleSheet("color: red");
                cell->setText(QString::number(sudoku[i][j]));
            }
        }
    }
}
