#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T15:27:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/Sudoku.cpp \
    src/SudokuSolver.cpp

HEADERS  += mainwindow.h \
    src/Sudoku.hpp \
    src/SudokuSolver.hpp

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

