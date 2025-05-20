#ifndef LCD_TASK_H_
#define LCD_TASK_H_

#include "global.h"

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);

void lcd_task_init(void);

#endif // LCD_TASK_H