// Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project.h"

#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <cstdio>
#include<conio.h>


#define MAX_LOADSTRING 100
#define IMAGE_VIEW 1 //poza de background
#define EDITCONTROL01 10 //textbox
#define BUTTON_01 11 //button 01 antonim
#define BUTTON_02 12 //button 02 sinonim
#define BUTTON_03 13 //button 03 definitie
#define BUTTON_04 15 //button help
#define STATIC 14 //static id

typedef struct treenode{

	int pref; //-1-->st;
			  //1-->dr;

	wchar_t cuvant[20];

	wchar_t antonim[20];

	wchar_t sinonim[20];

	wchar_t def[200];

	int echilibru;

	treenode* st;
	treenode* dr;

}treenode;

treenode* rad3;//definitii
treenode* rad2;//antonime
treenode* rad; // sinonime
treenode* aux;

int maxim(int a1, int b1){
	if (a1 > b1)
		return a1;
	else return b1;
}


int minim(int a1, int b1){
	if (a1 < b1)
		return a1;
	else return b1;
}
//
//
int inaltime(treenode *rad){
	if (rad == NULL)
		return -1;
	return maxim(inaltime(rad->st), inaltime(rad->dr)) + 1;
}

void factor_echilibru(treenode *rad){
	rad->echilibru = inaltime(rad->dr) - inaltime(rad->st);
}
//
void factor_echilibru_parcurgere(treenode *rad){

	if (rad == NULL){
		return;
	}
	else{
		factor_echilibru_parcurgere(rad->st);
		factor_echilibru_parcurgere(rad->dr);
	}

	factor_echilibru(rad);

}

treenode *rotire_dreapta(treenode *rad){
	treenode *p1;
	p1 = rad->st;
	rad->st = p1->dr;
	p1->dr = rad;
	factor_echilibru(rad);
	factor_echilibru(p1);
	rad = p1;
	return p1;
}
//
treenode *rotire_stanga(treenode *rad){
	treenode *p1 = rad->dr;
	rad->dr = p1->st;
	p1->st = rad;
	factor_echilibru(p1);
	factor_echilibru(rad);
	rad = p1;
	return p1;
}

//
treenode *rotire_d_dreapta(treenode *rad){
	rad->st = rotire_stanga(rad->st);
	rad = rotire_dreapta(rad);
	return rad;
}
//
treenode *rotire_d_stanga(treenode *rad){
	rad->dr = rotire_dreapta(rad->dr);
	rad = rotire_stanga(rad);
	return rad;
}
//
treenode* echilibrare(treenode* rad){
	treenode *p1;
	factor_echilibru(rad);
	if (rad->echilibru == -2)//dezechilibru este pe stanga
	{
		p1 = rad->st;
		if (p1->echilibru == 1)//dezechilibru este in subarborele drept al radacinii si facem rotatie dubla la dreapta
			rad = rotire_d_dreapta(rad);
		else //rotatie simpla la dreapta
			rad = rotire_dreapta(rad);
	}
	else if (rad->echilibru == 2){//(hd-hs) dezechilibru este pe dreapta
		p1 = rad->dr;
		if (p1->echilibru == -1)//dezechilibrul este in subarborele stang al radacinii
			rad = rotire_d_stanga(rad);
		else//dezechilibrul este in subarborele drept si se face rotatie simpla la stanga
			rad = rotire_stanga(rad);
	}
	return rad;
}
//
treenode* inserare_rec(treenode* rad, wchar_t *cuvant, wchar_t *informatie, int optiune){

	treenode* p1;

	if (rad == 0){
		p1 = (treenode*)malloc(sizeof(treenode));
		wcscpy(p1->cuvant, cuvant);
		if (optiune == 1)
			wcscpy(p1->antonim, informatie);
		if (optiune == 2)
			wcscpy(p1->sinonim, informatie);
		if (optiune == 3)
			wcscpy(p1->def, informatie);
		p1->st = 0;
		p1->dr = 0;
		p1->pref = 0;
		p1->echilibru = 0;
		return p1;
	}
	else {


		if (wcscmp(cuvant, rad->cuvant)<0){

			rad->st = inserare_rec(rad->st, cuvant, informatie, optiune);
		}
		else if (wcscmp(cuvant, rad->cuvant)>0){
			rad->dr = inserare_rec(rad->dr, cuvant, informatie, optiune);
		}
		else printf("Cuvantul exista");
	}
	return rad;
}
//

treenode* cautare(treenode* rad, wchar_t*string)
{
	treenode *p1;
	if (rad == 0) return 0;
	p1 = rad;

	while (p1 != 0)
	{

		if (wcscmp(string, p1->cuvant) == 0){
			return p1;
		}
		else if (wcscmp(string, p1->cuvant)<0){
			p1 = p1->st;
		}
		else p1 = p1->dr;
	}

	return 0; /* nu exista nod de cheie key */
}
//
void cautare_pozitie(treenode* rad, treenode* c1)  // CAUTAM POZITIA UNDE TREBUIE INTRODUS stringul in arborele auxiliar
// si adaugam 
{

	if (wcscmp(c1->cuvant, rad->cuvant) < 0){
		if (rad->st == 0){
			rad->st = c1;
			return;
		}
		else cautare_pozitie(rad->st, c1);
	}

	if (wcscmp(c1->cuvant, rad->cuvant)>0){
		if (rad->dr == 0){
			rad->dr = c1;
			return;
		}
		else cautare_pozitie(rad->dr, c1);

	}

	return;
}

//
void functie(treenode* rad, treenode *rad_arbore_original){ // unire restul cailor 

	treenode* c1; //cautat


	if (rad == 0)
		return;


	functie(rad->st, rad_arbore_original);// parcurgem in stanga

	functie(rad->dr, rad_arbore_original);//parcurgem in dreapta


	//
	_getch();
	printf("cautam daca trebuie sa unim la din arborele original ( %s )\n\n", rad->cuvant);

	_getch();
	//


	c1 = cautare(rad_arbore_original, rad->cuvant);


	//CONDITIE GRESITA DE CAUTAT ALTA 
	if ((c1->st) && (c1->pref != -1)){
		printf("\n\ncautam pozitie pt:%s\n\n", c1->st->cuvant);
		cautare_pozitie(rad, c1->st);
		c1->st = NULL;  // stergem calea din arborele original 
	}

	if ((c1->dr) && (c1->pref != 1)){
		printf("\n\ncautam pozitie pt:%s\n\n", c1->dr->cuvant);
		cautare_pozitie(rad, c1->dr);
		c1->dr = NULL;// stergem calea din arborele original 
	}

	if (c1->pref == 0) // sa nu se mai duca si in dreapta
		return;

	if (c1->pref == -1)  // daca s a dus pe o cale sa nu se mai duca si pe cealalta 
		return;

}

treenode* extragere_cale_preferata(treenode*rad, treenode*aux, int optiune){ // SI echilibrare

	treenode *p1;

	aux = (treenode*)malloc(sizeof(treenode));
	wcscpy(aux->cuvant, rad->cuvant);
	wcscpy(aux->antonim, rad->antonim);
	aux->st = 0;
	aux->dr = 0;
	aux->pref = 0;
	aux->echilibru = 0;
	aux->pref = rad->pref;

	p1 = rad;

	//copiem calea drumul catre pref in aux Si echilibram 
	while (p1 != NULL){
		if (p1->pref == 0){
			break;
		}

		if (p1->pref == 1){
			inserare_rec(aux, p1->dr->cuvant, p1->dr->antonim, optiune);
			p1 = p1->dr;
			factor_echilibru_parcurgere(aux);
			aux = echilibrare(aux);
		}
		else
		{
			inserare_rec(aux, p1->st->cuvant, p1->st->antonim, optiune);
			p1 = p1->st;
			factor_echilibru_parcurgere(aux);
			aux = echilibrare(aux);

		}

	}

	functie(aux, rad);

	return aux;
}

void update(treenode *rad, wchar_t *cuvant){


	if (rad == NULL){
		printf("\nCuvantul nu exista");
		_getch();
		exit(1);
	}

	if (wcscmp(cuvant, rad->cuvant) == 0){ // daca gaseste
		printf("%s != %s", rad->cuvant, rad->antonim);
		return;
	}


	if (wcscmp(cuvant, rad->cuvant) < 0)
		update(rad->st, cuvant);
	else
		update(rad->dr, cuvant);



	if (wcscmp(cuvant, rad->cuvant) < 0)
		rad->pref = -1;
	else
		rad->pref = 1;

}

treenode* cautare_CUVANT(treenode* rad, wchar_t *cuvant, int optiune){

	treenode *p1;
	p1 = rad;

	while (p1){
		if (wcscmp(cuvant, p1->cuvant) < 0)//st
			p1 = p1->st;
		else if (wcscmp(cuvant, p1->cuvant)>0)
			p1 = p1->dr;
		else
		{
			if (optiune == 1)
				return p1; //(" %s ", p1->antonim);

		}///


	}

	return NULL;
}

//CITIREA DIN FISIER A INFORMATIILOR IN ARBORE
void citire_din_fisier1(){

	FILE *f1,*f2,*f3;
	wchar_t *cuvant;
	wchar_t *informatie;
	wchar_t *linie;
	wchar_t *p1;
	int i1 = 0;

	cuvant=(wchar_t*)malloc(sizeof(wchar_t)* 20);
	informatie = (wchar_t*)malloc(sizeof(wchar_t)* 150);
	linie = (wchar_t*)malloc(sizeof(wchar_t)* 100);
	
		f1 = fopen("antonime.txt", "r");
		f2 = fopen("sinonime.txt", "r");
		f3 = fopen("definitii.txt", "r");


	while (!feof(f1)){ // Creem arobrii
		fgetws(linie, 100, f1);

		p1 = wcstok(linie, L"?\n");
		while (p1 != NULL){
			if (i1 % 2 == 0)
				wcscpy(cuvant, p1);
			else
				wcscpy(informatie, p1);

			i1++;
			p1 = wcstok(NULL, L"?\n");
		}
		rad = inserare_rec(rad, cuvant, informatie, 1);
	}

	
	while (!feof(f2)){ // Creem arobrii
		fgetws(linie, 100, f2);

		p1 = wcstok(linie, L"?\n");
		while (p1 != NULL){
			if (i1 % 2 == 0)
				wcscpy(cuvant, p1);
			else
				wcscpy(informatie, p1);

			i1++;
			p1 = wcstok(NULL, L"?\n");
		}
			rad2 = inserare_rec(rad2, cuvant, informatie, 2);
	}


	while (!feof(f3)){ // Creem arobrii
		fgetws(linie, 100, f3);

		p1 = wcstok(linie, L"?\n");
		while (p1 != NULL){
			if (i1 % 2 == 0)
				wcscpy(cuvant, p1);
			else
				wcscpy(informatie, p1);

			i1++;
			p1 = wcstok(NULL, L"?\n");
		}
			rad3 = inserare_rec(rad3, cuvant, informatie, 3);
	}

	free(cuvant);
	free(informatie);
	free(linie);
	fclose(f1);



}

void citire_din_fisier2(){

	FILE *f2;
	wchar_t *cuvant2;
	wchar_t *informatie2;
	wchar_t *linie2;
	wchar_t *p2;
	int i2 = 0;

	cuvant2 = (wchar_t*)malloc(sizeof(wchar_t)* 20);
	informatie2 = (wchar_t*)malloc(sizeof(wchar_t)* 100);
	linie2 = (wchar_t*)malloc(sizeof(wchar_t)* 100);

	
	f2 = fopen("sinonime.txt", "r");



	while (!feof(f2)){ // Creem arobrii
		fgetws(linie2, 100, f2);

		p2 = wcstok(linie2, L"?\n");
		while (p2 != NULL){
			if (i2 % 2 == 0)
				wcscpy(cuvant2, p2);
			else
				wcscpy(informatie2, p2);

			i2++;
			p2 = wcstok(NULL, L"?\n");
		}
		rad2 = inserare_rec(rad2, cuvant2, informatie2, 2);
	}
	free(cuvant2);
	free(informatie2);
	free(linie2);
	fclose(f2);


	}

void citire_din_fisier3(){

	FILE *f3;
	wchar_t *cuvant3;
	wchar_t *informatie3;
	wchar_t *linie3;
	wchar_t *p3;
	int i3 = 0;

	cuvant3 = (wchar_t*)malloc(sizeof(wchar_t)* 20);
	informatie3 = (wchar_t*)malloc(sizeof(wchar_t)* 200);
	linie3 = (wchar_t*)malloc(sizeof(wchar_t)* 200);


	f3 = fopen("definitii.txt", "r");



	while (!feof(f3)){ // Creem arobrii
		fgetws(linie3, 100, f3);

		p3 = wcstok(linie3, L"?\n");
		while (p3 != NULL){
			if (i3 % 2 == 0)
				wcscpy(cuvant3, p3);
			else
				wcscpy(informatie3, p3);

			i3++;
			p3 = wcstok(NULL, L"?\n");
		}
		rad3 = inserare_rec(rad3, cuvant3, informatie3, 3);
	}
	free(cuvant3);
	free(informatie3);
	free(linie3);
	fclose(f3);


}

void refacere_arbore(treenode*rad,wchar_t*string,int optiune){


	        update(rad, string);
	
			aux = extragere_cale_preferata(rad, aux, optiune);
	
			rad = aux;// SCHIMBAM ARBORII
	
			aux = NULL;

}



///////////<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
bool alegeAcum;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable


	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		szWindowClass,
		L"DICTIONAR",
	    WS_OVERLAPPED,//de marit imaginea
		CW_USEDEFAULT, CW_USEDEFAULT, 1100,600,NULL, NULL, hInstance, NULL);   //dim chenar

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

HWND hImageView; //poza de fundal
HWND TextBox01;
HWND BUTTON01;//buton antonim
HWND BUTTON02;//buton sinonim
HWND BUTTON03;//button definite
HWND BUTTON04;//button help
HWND STATIC01;//static text


int sem = 0;
treenode *cautat;
wchar_t cuvant[20];
int alegere_about;
wchar_t alfabet[200];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	//Copiere date din fisierul text:
	if (sem == 0){
		citire_din_fisier1();
		citire_din_fisier2();
		citire_din_fisier3();
		sem++;
	}
	////





	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{


	case WM_CREATE:{
					   //background
					   HBITMAP hImage = (HBITMAP)LoadImage(NULL, L"image.bmp", IMAGE_BITMAP, 1140, 545, LR_LOADFROMFILE);
					   hImageView = CreateWindowEx(NULL, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0,0, 500, 600, hWnd, (HMENU)IMAGE_VIEW, GetModuleHandle(NULL), NULL);
					   SendMessage(hImageView, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);

					   //TextBox
					   LPCWSTR TextBox_ID = L"EDIT";
					   LPCWSTR TextBox_text = L"";
					   TextBox01 = CreateWindowEx(NULL, TextBox_ID, TextBox_text, WS_BORDER | WS_VISIBLE | WS_CHILD, 20, 100, 300, 20, hWnd, (HMENU)EDITCONTROL01, NULL, NULL);//textbox 01
					   //TextBox_end

					   //BUTTON antonim
					   BUTTON01 = CreateWindow(L"BUTTON", L"ANTONIM", BS_DEFPUSHBUTTON | WS_BORDER | WS_VISIBLE | WS_CHILD, 80, 180, 80, 30, hWnd,(HMENU)BUTTON_01, NULL, NULL);
					   //button antonim_end

					   //BUTTON sinonim
					   BUTTON02 = CreateWindow(L"BUTTON", L"SINONIM", BS_DEFPUSHBUTTON | WS_BORDER | WS_VISIBLE | WS_CHILD,250, 180, 80, 30, hWnd, (HMENU)BUTTON_02, NULL, NULL);
					   //button sinonim_end

					   //BUTTON definitie
					   BUTTON03 = CreateWindow(L"BUTTON", L"DEFINITIE", BS_DEFPUSHBUTTON | WS_BORDER | WS_VISIBLE | WS_CHILD, 410,180, 80, 30, hWnd, (HMENU)BUTTON_03, NULL, NULL);
					   //button definitie_end

					   //BUTTON help
					   BUTTON04 = CreateWindow(L"BUTTON", L"HELP?", BS_DEFPUSHBUTTON | WS_BORDER | WS_VISIBLE | WS_CHILD, 800, 450, 80, 30, hWnd, (HMENU)BUTTON_04, NULL, NULL);
					   //button help_end

					   //Static_Text
					   LPCWSTR Static_ID = L"STATIC";
					   STATIC01 = CreateWindowEx(NULL, Static_ID, L"", WS_BORDER | WS_VISIBLE | WS_CHILD, 510, 285, 400, 100, hWnd, (HMENU)STATIC, NULL, NULL);
					   HFONT hFont = CreateFont(30, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
					   SendMessage(STATIC01, WM_SETFONT, WPARAM(hFont), TRUE);

					   //Static_Text_End


	}break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{

		case BUTTON_01://Buton ANTONIM

			
			GetWindowText(TextBox01, cuvant, 20);

			cautat=cautare_CUVANT(rad, cuvant, 1);
	
			if (cautat!=NULL)
			SetWindowText(STATIC01,cautat->antonim);
			else
			SetWindowText(STATIC01, L"Nu exista antonim pentru acest cuvant");

			break;

		
		case BUTTON_02://Buton SINONIM

			cautat = NULL;
			
			GetWindowText(TextBox01, cuvant, 20);

			cautat = cautare_CUVANT(rad2, cuvant, 1);

		if (cautat != NULL)
				SetWindowText(STATIC01, cautat->sinonim);
			else
				SetWindowText(STATIC01, L"Nu exista sinonim pentru acest cuvant");

			break;


		case BUTTON_03://Buton DEFINITIE

			cautat = NULL;

			GetWindowText(TextBox01, cuvant, 20);

			cautat = cautare_CUVANT(rad3, cuvant, 1);

			if (cautat != NULL)
				SetWindowText(STATIC01, cautat->def);
			else
				SetWindowText(STATIC01, L"Nu exista definitie pentru acest cuvant");

			break;


		case BUTTON_04:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		
		switch (wParam){

		case a:
		
			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
					FILE *ff;
				ff = fopen("faa.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsa.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)"daaaaa");
				ff = fopen("fda.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;

		case b:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fab.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsb.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdb.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case c:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fac.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsc.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdc.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case d:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fad.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsd.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdd.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case e:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fae.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fse.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
	

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fde.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case f:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("faf.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsf.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}


			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdf.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case g:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fag.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsg.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdg.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case h:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fah.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsh.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdh.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;

		case i:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fai.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsi.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdi.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;

		case j:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("faj.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsj.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdj.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case k:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fak.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsk.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdk.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case l:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fal.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsl.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			
			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdl.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case m:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fam.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsm.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdm.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case n:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fan.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsn.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdn.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case o:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fao.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fso.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		
			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdo.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case p:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fap.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsp.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdp.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case q:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("faq.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsq.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdq.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case r:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("far.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsr.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdr.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;




		case s:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fas.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fss.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fds.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case t:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fat.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fst.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdt.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case u:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fau.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsu.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}


			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdu.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case v:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fav.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsv.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdv.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case w:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("faw.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsw.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdw.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



		case x:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fax.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsx.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdx.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case y:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fay.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsy.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
		

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdy.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;


		case z:

			if (alegere_about == 1){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("faz.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);
				}
				fclose(ff);
			}

			if (alegere_about == 2){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fsz.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			

			if (alegere_about == 3){
				SendMessage(GetDlgItem(hDlg, LISTA01), LB_RESETCONTENT, 0, 0);
				FILE *ff;
				ff = fopen("fdz.txt", "r");
				while (!feof(ff)){
					fgetws(alfabet, 30, ff);
					SendMessage(GetDlgItem(hDlg, LISTA01), LB_ADDSTRING, 0, (LPARAM)alfabet);

				}
				fclose(ff);
			}
			break;



			////////////////////
		case antonime_help:
				alegere_about = 1;
			break;

		case sinonime_help:
			alegere_about = 2;
			break;

		case definitie_help:
				alegere_about = 3;
			break;


		case ww: //Update button
			wchar_t cuvant_pref[30];

			GetWindowText(GetDlgItem(hDlg, IDC_EDIT2), cuvant_pref, 20);


			if (alegere_about == 1)
				refacere_arbore(rad,cuvant_pref, 1);

			if (alegere_about == 2)
				refacere_arbore(rad2, cuvant_pref, 2);


			if (alegere_about ==3)
				refacere_arbore(rad3, cuvant_pref, 3);


			break;

			/// Pentru butonul de upload

		

		}



		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	

		
	return (INT_PTR)FALSE;
}

