#pragma once

#include <QMainWindow>

#include "ui_editor.h"

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
    Q_OBJECT
public:
    EditorWindow() {
        setupUi(this);

        show(); 
    }
};
