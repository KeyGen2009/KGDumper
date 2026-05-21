/*						 KGDumper for Youtube 
						Tutorial by KeyGen2009         
				  Nota: Activar siempre juego de caracteres multibytes          */


#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <conio.h>

#include <fstream> //Para crear archivos


using namespace std;

//Variables del proceso
DWORD pid;
HANDLE hp;

#include "KGModulos.h"
#include "kg_buscar_patrones.h"



//Modulos del juego
ModuloInfo client;
ModuloInfo engine;

int main()
{

	//Patron  y mascara de la funcion XRAY
	const char* patron =
		"\x00\xC0"
		"\x4C\x8B\xA4\x24\xC8\x00\x00\x00"
		"\x48\x8B\xB4\x24\xC0\x00\x00\x00"
		"\x48\x8B\x9C\x24\xD0\x00\x00\x00";  //25

	const char* mascara = "?xxxxxxxxxxxxxxxxxxxxxxxxx";


	//Abrir el proceso
	hp = kg_abrir_proceso("cs2.exe", PROCESS_ALL_ACCESS);

	//Cargar las direcciones y tamaños de los modulos
	client = kg_obtener_modulo_info(pid, L"client.dll");

	//Crear un espacio en memoria para la imagen virtual de client.dll
	BYTE* buffer = new BYTE[client.size];

	//Comproba la lectura total del modulo client.dll
	SIZE_T bytes_leidos;

	//Cargar imagen el el buffer
	ReadProcessMemory(hp, (LPBYTE)client.base, buffer, client.size, &bytes_leidos);

	if (bytes_leidos == client.size)
	{
		cout << "Se cargo la imagen de client.dll correctamente!" << endl;
	}
	else {
		cout << "No se pudo cargar en su totalidad la imagen!" << endl;
	}

	//Pedimos a la funcion que busque el patron y nos devuelva el offset
	uintptr_t XRAY_TKB = BuscarPatrones(buffer, client.size, patron, mascara);
	cout << "La direccion de XRAY es: client.dll + " << hex << XRAY_TKB << endl;

	//Crear nuestro archivo
	ofstream cfg("kg_dumperCS2.h");
	cfg << "/*    KG Dumper CS2 Anti UPDATES */ \n" << endl;
	cfg << "uintptr_t XRAY = 0x" << hex << XRAY_TKB << ";" << endl;
	cfg.close();
	cout << "Archivo escrito con exito!" << endl;

}