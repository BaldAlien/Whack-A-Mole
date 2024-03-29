#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NUC1xx.h"
#include "Seven_Segment.h"
#include "scankey.h"
#include "LCD_Driver.h"
#define MOLE_SPEED_MAX 1500
#define MOLE_SPEED_CHANGE_RATE 0.97

unsigned char DisplayBuf[8 * 128];

unsigned char Img_mole_nor[40 * 4] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xF0, 0x70, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFD, 0xFF, 0xFF, 0xE7, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xFF, 0xFF, 0xFC, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x60, 0x20, 0x60, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xF3, 0xF3, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0x20, 0x60, 0x60, 0x40, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x03, 0x06, 0x06, 0x04, 0x0C, 0x0C, 0x0C, 0x08, 0x1D, 0x1F, 0x3F, 0x3F, 0x3F, 0x7F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x7F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1D, 0x08, 0x08, 0x0C, 0x0C, 0x0C, 0x04, 0x06, 0x07, 0x01, 0x00, 0x00};

unsigned char Img_mole_die[40 * 4] = {
	0x00, 0x00, 0x00, 0x00, 0x50, 0x68, 0x88, 0x84, 0x04, 0x02, 0x04, 0xE0, 0xE6, 0xE0, 0xC2, 0xE4, 0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 0x50, 0x04, 0x08, 0x08, 0x50, 0x30, 0x00, 0x00, 0x20, 0xE0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1, 0xF8, 0xDF, 0xDE, 0xE9, 0xF3, 0xE5, 0xDE, 0xFF, 0xFF, 0xDE, 0xE5, 0xFB, 0xF3, 0xCC, 0xDF, 0xFC, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x60, 0x60, 0x60, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xF3, 0xF3, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x60, 0x60, 0x60, 0x40, 0xC0, 0x40, 0xC0, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x03, 0x06, 0x04, 0x04, 0x0C, 0x0C, 0x08, 0x18, 0x1D, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x7F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1B, 0x08, 0x08, 0x0C, 0x0C, 0x04, 0x06, 0x06, 0x03, 0x03, 0x00, 0x00};

unsigned char Img_mole_hole[40 * 4] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x60, 0x20, 0x60, 0x20, 0x70, 0x30, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x30, 0x30, 0x30, 0x30, 0x60, 0x20, 0x60, 0x60, 0x40, 0xE0, 0xC0, 0x80, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x03, 0x06, 0x06, 0x04, 0x0C, 0x0C, 0x0C, 0x08, 0x1C, 0x1C, 0x3C, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x38, 0x3C, 0x1C, 0x1C, 0x08, 0x08, 0x0C, 0x0C, 0x0C, 0x04, 0x06, 0x07, 0x01, 0x00, 0x00};

unsigned char Nuvoton[128 * 8] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void InitData(void);
int8_t GetRand(void);
void LoadBuf(void);
void LoadImg(uint8_t Img_num, uint8_t Img_location);
extern void Initial_panel(void);
extern void clr_all_panel(void);
extern void draw_LCD(unsigned char *buffer);
void seg_display(int8_t res, int16_t *seg_count, int8_t *seg_state);

void InitData(void)
{
	uint8_t i;
	for (i = 1; i <= 6; i++)
	{
		LoadImg(0, i);
	}
}

int8_t GetRand(void)
{
	return rand() % 6 + 1;
}

void LoadIndex(void)
{
	Initial_panel();
	clr_all_panel();
	print_lcd(1, "Whack-A-Mole!");
	print_lcd(2, "Start ----> 1");
}
void Loadres(uint8_t res)
{
	char res_text[15] = "Your Score:";
	Initial_panel();
	clr_all_panel();
	print_lcd(0, "Game Over");
	sprintf(res_text + 11, "%d", res);
	print_lcd(1, res_text);
	print_lcd(2, "Continue? ---> 1");
	print_lcd(3, "BackToHome --> 2");
}

void LoadBuf(void)
{
	uint8_t x, y;
	for (y = 0; y < 128; y++)
	{
		for (x = 0; x < 8; x++)
		{
			DisplayBuf[y * 8 + x] = Nuvoton[y + x * 128];
		}
	}
}
void LoadImg(uint8_t Img_num, uint8_t Img_location)
{
	uint8_t x, y;

	switch (Img_num)
	{
	case 0:
		for (y = 0; y < 4; y++)
		{
			for (x = 0; x < 40; x++)
			{
				Nuvoton[y * 128 + 128 * 4 * (Img_location / 4) + 40 * ((Img_location - 1) % 3) + 2 * ((Img_location - 1) % 3 + 1) + x] = Img_mole_hole[y * 40 + x];
			}
		}
		break;
	case 1:
		for (y = 0; y < 4; y++)
		{
			for (x = 0; x < 40; x++)
			{
				Nuvoton[y * 128 + 128 * 4 * (Img_location / 4) + 40 * ((Img_location - 1) % 3) + 2 * ((Img_location - 1) % 3 + 1) + x] = Img_mole_nor[y * 40 + x];
			}
		}
		break;
	case 2:
		for (y = 0; y < 4; y++)
		{
			for (x = 0; x < 40; x++)
			{
				Nuvoton[y * 128 + 128 * 4 * (Img_location / 4) + 40 * ((Img_location - 1) % 3) + 2 * ((Img_location - 1) % 3 + 1) + x] = Img_mole_die[y * 40 + x];
			}
		}
		break;
	default:
		break;
	}
}

void seg_display(int8_t res, int16_t *seg_count, int8_t *seg_state)
{

	int8_t digit;
	int8_t value = res;
	int8_t bit = 0;

	if (*seg_count)
	{
		*seg_count = *seg_count - 1;
	}
	else
	{
		while (value)
		{
			bit++;
			value /= 10;
		}
		*seg_count = 1;
		*seg_state = *seg_state + 1;
		if (*seg_state > bit - 1)
		{
			*seg_state = 0;
		}
		value = res;

		switch (*seg_state)
		{
		case 0:
			digit = value % 10;
			close_seven_segment();
			show_seven_segment(0, digit);
			break;
		case 1:
			digit = value / 10 % 10;
			close_seven_segment();
			show_seven_segment(1, digit);
			break;
		case 2:
			digit = value / 100 % 10;
			close_seven_segment();
			show_seven_segment(2, digit);
			break;
		case 3:
			digit = value / 1000 % 10;
			close_seven_segment();
			show_seven_segment(3, digit);
			break;
		default:
			break;
		}
	}
}

/*----------------------------------------------------------------------------
  MAIN function
  ----------------------------------------------------------------------------*/
int32_t main(void)
{
	int8_t number;
	float mole_speed = MOLE_SPEED_MAX;	  //地鼠刷新速度
	int8_t mole_state = 1;				  //地鼠状态，1代表活的，0代表死的
	int16_t mole_rand_count = mole_speed; //地鼠刷新计时器
	int16_t seg_count = 2;
	int8_t seg_state = 0;
	int8_t mole_now = 1; //获得一只地鼠
	int8_t mole_now_get;
	int8_t state = 0;
	int8_t res = 0; //成绩，打中的地鼠

	UNLOCKREG();
	SYSCLK->PWRCON.XTL32K_EN = 1;
	SYSCLK->PWRCON.XTL12M_EN = 1; //Enable 12Mhz and set HCLK->12Mhz
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();

	OpenKeyPad();
	close_seven_segment();

	LoadIndex();
	while (1)
	{
		switch (state)
		{
		case 0:
		{
			number = Scankey(); //读取键盘
			if (number == 1)
			{

				state = 1;	//进入游戏状态
				InitData(); //游戏初始化数据

				res = 0;
				mole_speed = MOLE_SPEED_MAX; //设置地鼠刷新速度

				mole_now = GetRand();		  //刷新地鼠
				LoadImg(1, mole_now);		  //加载地鼠图片
				mole_state = 1;				  //活的地鼠
				mole_rand_count = mole_speed; //地鼠刷新时间重置

				LoadBuf();
				draw_LCD(DisplayBuf); // render to LCD
			}
			break;
		}
		case 1:
		{

			seg_display(res, &seg_count, &seg_state);
			if (mole_rand_count) //判断地鼠是否要刷新
			{
				mole_rand_count--; //不用刷新，计数-1
			}
			else
			{
				if (mole_state == 1) //超时没打中
				{
					state = 2; //游戏结束
					clr_all_panel();
					Loadres(res); //加载游戏结束界面
					break;
				}
				LoadImg(0, mole_now); //刷走上一只地鼠
				do
				{
					mole_now_get = GetRand();		//刷新地鼠
				} while (mole_now_get == mole_now); //与上一只地鼠不重复
				mole_now = mole_now_get;

				LoadImg(1, mole_now);		  //加载地鼠图片
				mole_state = 1;				  //活的地鼠
				mole_rand_count = mole_speed; //地鼠刷新时间重置

				LoadBuf();
				draw_LCD(DisplayBuf); // 刷新LCD
			}

			number = Scankey(); //读取键盘

			if ((mole_now == (number - 3)) && mole_state) //如果刚好打中当前地鼠
			{
				LoadImg(2, mole_now);				  //地鼠图变为死的
				mole_state = 0;						  //地鼠标记为死的
				res++;								  //分数+1
				mole_speed *= MOLE_SPEED_CHANGE_RATE; //加快速度

				LoadBuf();
				draw_LCD(DisplayBuf); // 刷新LCD
			}
			if (number == 3)
			{
				state = 2; //游戏结束
				Loadres(res); //加载游戏结束界面
				break;
			}
			break;
		}
		case 2:
		{

			seg_display(res, &seg_count, &seg_state);

			number = Scankey(); //读取键盘
			if (number == 1)
			{
				state = 1;
				InitData();
				res = 0;
				mole_speed = MOLE_SPEED_MAX;

				mole_now = GetRand();		  //刷新地鼠
				LoadImg(1, mole_now);		  //加载地鼠图片
				mole_state = 1;				  //活的地鼠
				mole_rand_count = mole_speed; //地鼠刷新时间重置

				LoadBuf();
				draw_LCD(DisplayBuf); // 刷新LCD
				break;
			}
			if (number == 2)
			{
				state = 0;
				close_seven_segment();
				LoadIndex();
				break;
			}
		}
		default:
			break;
		}
	}
}
