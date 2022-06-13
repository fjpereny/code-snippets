/*
QTableWdiget Code Snippet
cut_copy_paste_delete.cpp
Adds cut / copy / paste / delete functionality to QTableWidget cells.

Copyright (C) 2022  Frank Pereny

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


void TabDialog::keyPressEvent(QKeyEvent *event)
{
    // Copy
    // This code captures the cells text values and puts them to clipboard.
    // Data is tab spaced for pasting into spreadsheet applications such as LibreOffice Calc.
    if ((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_C))
    {
        // Check to ensure selection exists.
        QItemSelectionModel *selection_model = ui.tableWidget->selectionModel();
        if (selection_model)
        {
            QModelIndexList indexList = selection_model->selectedIndexes();
            if (indexList.count() <= 0)
            {
                return;
            }
        }
        else
        {
            return;
        }

        QString copied_text = "";
        QList<QTableWidgetItem *> list = ui.tableWidget->selectedItems();

        int start_row = list[0]->row();
        int end_row = list[0]->row();
        int start_col = list[0]->column();
        int end_col = list[0]->column();
        for(QTableWidgetItem *item : list)
        {
            if (item->row() < start_row)
            {
                start_row = item->row();
            }
            if (item->row() > end_row)
            {
                end_row = item->row();
            }
            if (item->column() < start_col)
            {
                start_col = item->column();
            }
            if (item->column() > end_col)
            {
                end_col = item->column();
            }
        }

        for(int row=start_row; row <= end_row; ++row)
        {
            for(int col=start_col; col <= end_col; ++col)
            {
                QTableWidgetItem *item = ui.tableWidget->item(row, col);
                copied_text += item->text();
                if (col < ui.tableWidget->columnCount() - 1)
                {
                    copied_text += "\t";
                }
            }
            copied_text += "\n";
        }
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(copied_text);
    }

    // Cut
    if ((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_X))
    {
        // Check to ensure selection exists.
        QItemSelectionModel *selection_model = ui.tableWidget->selectionModel();
        if (selection_model)
        {
            QModelIndexList indexList = selection_model->selectedIndexes();
            if (indexList.count() <= 0)
            {
                return;
            }
        }
        else
        {
            return;
        }

        QString copied_text = "";
        QList<QTableWidgetItem *> list = ui.tableWidget->selectedItems();

        int start_row = list[0]->row();
        int end_row = list[0]->row();
        int start_col = list[0]->column();
        int end_col = list[0]->column();
        for(QTableWidgetItem *item : list)
        {
            if (item->row() < start_row)
            {
                start_row = item->row();
            }
            if (item->row() > end_row)
            {
                end_row = item->row();
            }
            if (item->column() < start_col)
            {
                start_col = item->column();
            }
            if (item->column() > end_col)
            {
                end_col = item->column();
            }
        }

        for(int row=start_row; row <= end_row; ++row)
        {
            for(int col=start_col; col <= end_col; ++col)
            {
                QTableWidgetItem *item = ui.tableWidget->item(row, col);
                copied_text += item->text();
                item->setText("");
                if (col < ui.tableWidget->columnCount() - 1)
                {
                    copied_text += "\t";
                }
            }
            copied_text += "\n";
        }
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(copied_text);
    }

    // Paste
    // Allows pasting of cell data from spreadsheet applications such as LibreOffice Calc.
    if ((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_V))
    {
        QClipboard *clipboard = QGuiApplication::clipboard();
        QString pasted_text = clipboard->text();

#if (QT_VERSION>=QT_VERSION_CHECK(5,14,0))
        QList<QString> rows = pasted_text.split('\n', Qt::SkipEmptyParts);
#else
        QList<QString> rows = pasted_text.split('\n', QString::SkipEmptyParts);
#endif

        // Get indexes of currently selected cells.
        int starting_row = 0;
        int starting_col = 0;
        QItemSelectionModel *selection_model = ui.tableWidget->selectionModel();
        if (selection_model)
        {
            QModelIndexList indexList = selection_model->selectedIndexes();
            if (indexList.count() > 0)
            {
                starting_row = selection_model->selectedIndexes().begin()->row();
                starting_col = selection_model->selectedIndexes().begin()->column();
            }
        }

        int cur_row = starting_row;
        int cur_col = starting_col;
        for(const QString &row : rows)
        {
            if (cur_row >= ui.tableWidget->rowCount())
            {
                ui.tableWidget->setRowCount(ui.tableWidget->rowCount() + 1);
                ui.spinBoxRows->setValue(ui.spinBoxRows->value() + 1);
            }

            QList<QString> cols = row.split('\t');
            for(const QString &col : cols)
            {
                if (cur_col >= ui.tableWidget->columnCount())
                {
                    ui.tableWidget->setColumnCount(ui.tableWidget->columnCount() + 1);
                    ui.spinBoxColumns->setValue(ui.spinBoxColumns->value() + 1);
                }

                QTableWidgetItem *item = ui.tableWidget->item(cur_row, cur_col);
                if (item) // Checks if item is a null pointer (item doesn't exist)
                {
                    item->setText(col);
                    ++cur_col;
                }
                else
                {
                    // Adds new item if copied size is larger than table size.
                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    newItem->setText(col);
                    QBrush brush = QBrush(Qt::red);
                    newItem->setBackground(brush);
                    ui.tableWidget->setItem(cur_row, cur_col, newItem);
                }
            }
            ++cur_row;
            cur_col = starting_col;
        }
    }

    // Delete Cell Content
    // This feature allows cells to be deleted by selecting and clicking the delete key.
    if (event->key() == Qt::Key_Delete)
    {
        QList<QTableWidgetItem *> list = ui.tableWidget->selectedItems();
        for(QTableWidgetItem* i : list)
        {
            i->setText("");
        }
    }
}