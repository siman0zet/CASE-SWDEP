#include "CForeignRow.h"

void CForeignRow::setForeignTable(int id, int frid)
{
    foreign_table_id = id;
    foreign_row_id = frid;
}
