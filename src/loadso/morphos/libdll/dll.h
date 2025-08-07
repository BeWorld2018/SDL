#ifndef __DLL_H
#define __DLL_H

/************************************************************
 * External structures
 ************************************************************/

typedef struct dll_sExportSymbol
{
	void *SymbolAddress;
	char *SymbolName;
} dll_tExportSymbol;

typedef struct dll_sImportSymbol
{
	void **SymbolPointer;
	char *SymbolName;
	char *DLLFileName;
	char *DLLPortName;
} dll_tImportSymbol;

#ifdef __cplusplus
extern "C" {
#endif

void *dllLoadLibrary(const char *name, const char *portname);
int dllFreeLibrary(void *hinst);
void *dllGetProcAddress(void *hinst, const char *name);
char *dllGetLastError(void);
//int dllImportSymbols(void);

int     DLL_Init(void);
void    DLL_DeInit(void);

#ifdef __cplusplus
}
#endif

/*
 * Prototypes for DLL implementations
 */

extern dll_tExportSymbol DLL_ExportSymbols[];
extern dll_tImportSymbol DLL_ImportSymbols[];

#endif
