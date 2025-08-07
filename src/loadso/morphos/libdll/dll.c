/*
** This file contains the runtime usable DLL functions, like LoadLibrary, GetProcAddress etc.
*/

#include "SDL_internal.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <dos/dostags.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/alib_protos.h>
#include "dll.h"
#include "dll_intern.h"

struct List dllOpenedDLLs;
char dllError[256] = {0};
int errorclear = TRUE;
int cleanupflag = FALSE;

static void dllSetError(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(dllError, sizeof(dllError), fmt, ap); 
	va_end(ap);
	errorclear = FALSE;
	D("[DLL] DLL error: %s", dllError);
}

void dllCleanup()
{
	dll_tInstance *n;

	D("[DLL] dllCleanup()");

	while ((n = (dll_tInstance *)RemHead((struct List *)&dllOpenedDLLs)))
	{
		dllInternalFreeLibrary(n);
	}
}

void *dllLoadLibrary(const char *filename, const char *portname)
{
	D("[DLL] dllLoadLibrary(%s, %s)", filename, portname);

	return dllInternalLoadLibrary(filename, portname, 1);
}

void *dllInternalLoadLibrary(const char *filename, const char *portname, int raiseusecount)
{
	struct MsgPort *myport = NULL;
	dll_tInstance *n = NULL;
	dll_tMessage msg, *reply;
	BPTR handle;
	//char buffer[1024];
	char *buffer = NULL;

	D("[DLL] dllInternalLoadLibrary(%s, %s, %d)", filename, portname, raiseusecount);

	if (!cleanupflag)
	{
		NewList(&dllOpenedDLLs);
		dllOpenedDLLs.lh_Type = NT_USER;

		if (atexit((void *)dllCleanup))
		{
			dllSetError("can't install exit handler");
			goto failed;
		}
		else
		{
			cleanupflag = TRUE;
		}
	}

	if (!filename)
	{
		dllSetError("empty file name");
		goto failed;
	}

	if (!(handle = Open(filename, MODE_OLDFILE)))
	{
		char dosErrorMsg[80];
		Fault(IoErr(), NULL, dosErrorMsg, sizeof(dosErrorMsg));
		dllSetError("can't open %s: %s", filename, dosErrorMsg);
		goto failed;
	}

	Close(handle);

	if (!portname)
	{
		portname = filename;
	}

	// Search for already opened DLLs
	if ((n = (dll_tInstance *)FindName(&dllOpenedDLLs, portname)))
	{
		if (raiseusecount)
		{
			if (n->node.ln_Pri < 127)
			{
				n->node.ln_Pri++;
				D("[DLL] increased internal open count of %s to %d", n->node.ln_Name, n->node.ln_Pri);
			}
			else
			{
				dllSetError("can't open %s: reached max usage count (127)", filename);
				// we can't do a goto failed here, because it would free the node
				return NULL;
			}
		}

		D("[DLL] found %s in dllOpenedDLLs", portname);

		return n;
	}

	// Not opened yet, create a new node
	if (!(n = malloc(sizeof(dll_tInstance) + strlen(portname))))
	{
		dllSetError("can't allocate %d bytes for a new dllOpenedDLL node", sizeof(dll_tInstance));
		goto failed;
	}

	memset(n, 0, sizeof(dll_tInstance));
	//strncpy(n->name, portname, sizeof(n->name));
	strcpy(n->name, portname);
	n->node.ln_Name = n->name;
	n->node.ln_Pri = 1; // usecount
	n->node.ln_Type = NT_USER;

	// create a reply port
	if (!(myport = CreateMsgPort()))
	{
		dllSetError("can't create the message port");
		goto failed;
	}

	// if the DLL has not been started already...
	if (!(n->dllPort = FindPort(portname)))
	{
		int i;
		int bufsize;

		D("[DLL] launching '%s'\n", filename);

		if (!(handle = Open("CON:0/0/800/600/DLL_OUTPUT/AUTO/CLOSE/WAIT", MODE_NEWFILE)))
		//snprintf(buffer, sizeof(buffer), "CON:////%s output/AUTO/CLOSE/WAIT", FilePart(filename));

		//if (!(handle = Open(buffer, MODE_NEWFILE)))
		{
			dllSetError("can't open output stream");
			goto failed;
		}

		if (!(buffer = malloc(strlen(filename) + strlen(portname) + 6)))
		{
			dllSetError("can't allocate command line string");
			goto failed;
		}

		bufsize = strlen(filename) + strlen(portname) + 6;
		i = snprintf(buffer, strlen(filename) + strlen(portname) + 6, "\"%s\" \"%s\"", filename, portname);

		// check if the string was truncated
		if (!(i > 0 && i < bufsize))
		{
			dllSetError("command line string was truncated (%d - %d)", i, bufsize);
			goto failed;
		}

		if (SystemTags(buffer,
			SYS_Asynch, TRUE,
			SYS_Output, handle,
			SYS_Input,  NULL,
			NP_StackSize, 10000, 
			TAG_DONE) < 0)
		{
			dllSetError("SystemTags failed");
			goto failed;
		}

		// wait for 10 secs, and check every half sec
		for (i = 0; i < 20; i++)
		{
			if ((n->dllPort = FindPort(portname)))
			{
				break;
			}
			D("[DLL] Delaying...");
			Delay(25);
		}
	}

	if (!n->dllPort)
	{
		dllSetError("can't find the message port: %s", portname);
		goto failed;
	}

	memset(&msg, 0, sizeof(msg));
	msg.dllMessageType = DLLMTYPE_Open;
	msg.dllMessageData.dllOpen.StackType = DLLSTACK_DEFAULT;
	msg.Message.mn_ReplyPort = myport;

	PutMsg(n->dllPort, (struct Message *)&msg);
	WaitPort(myport);

	if (!(reply = (dll_tMessage *)GetMsg(myport)))
	{
		dllSetError("didn't get a reply message");
		goto failed;
	}

	if (reply->dllMessageData.dllOpen.ErrorCode != DLLERR_NoError)
	{
		dllSetError("DLL initialization error");
		goto failed;
	}

	DeleteMsgPort(myport);
	AddTail(&dllOpenedDLLs, (struct Node *)n);

	D("[DLL] added a new node for %s", portname);

	return n;

failed:
	if (myport)
	{
		DeleteMsgPort(myport);
	}

	if (n)
	{
		free(n);
	}

	if (buffer)
	{
		free(buffer);
	}

	return NULL;
}

int dllFreeLibrary(void *hinst)
{
	dll_tInstance *n;
	D("[DLL] dllFreeLibrary()");

	if (!(n = (dll_tInstance *)hinst))
	{
		dllSetError("NULL DLL instance");
		return 1;
	}

	n->node.ln_Pri--;
	D("[DLL] decreased internal open count of %s to %d", n->node.ln_Name, n->node.ln_Pri);

	if (n->node.ln_Pri <= 0)
	{
		Remove((struct Node *)n);
		return dllInternalFreeLibrary(n);
	}

	return 0;
}

int dllInternalFreeLibrary(dll_tInstance *n)
{
	dll_tMessage msg, *reply;
	struct MsgPort *myport;
	D("[DLL] dllInternalFreeLibrary()");

	if (!(myport = CreateMsgPort()))
	{
		dllSetError("can't create the message port");	
		return 1;
	}

	D("[DLL] sending close message to %s", n->node.ln_Name);
	memset(&msg, 0, sizeof(msg));
	msg.dllMessageType = DLLMTYPE_Close;
	msg.Message.mn_ReplyPort = myport;

	if (FindPort(n->node.ln_Name) == n->dllPort)
	{
		PutMsg(n->dllPort, (struct Message *)&msg);
		while(!(reply = (dll_tMessage *)GetMsg(myport)))
		{
			Delay(2);
			if (FindPort(n->node.ln_Name) != n->dllPort)
			{
				break;
			}
		}
	}
	else
	{
		dllSetError("can't find message port: %s", n->node.ln_Name);
		DeleteMsgPort(myport);
		return 1;
	}

	DeleteMsgPort(myport);
	free(n);
	
	return 0;
}

void *dllGetProcAddress(void *hinst, const char *name)
{
	dll_tMessage msg, *reply;
	struct MsgPort *myport;
	dll_tInstance *n;
	void *sym;
	D("[DLL] dllGetProcAddress()");

	if (!(n = (dll_tInstance *)hinst))
	{
		dllSetError("NULL DLL instance");
		return NULL;
	}

	if (!name)
	{
		dllSetError("empty symbol name");
		return NULL;
	}

	if (!(myport = CreateMsgPort()))
	{
		dllSetError("can't create the message port");
		return NULL;
	}

	memset(&msg, 0, sizeof(msg));
	msg.dllMessageType = DLLMTYPE_SymbolQuery;
	msg.dllMessageData.dllSymbolQuery.StackType = DLLSTACK_DEFAULT;
	msg.dllMessageData.dllSymbolQuery.SymbolName = (char *)name;
	msg.dllMessageData.dllSymbolQuery.SymbolPointer = &sym;
	msg.Message.mn_ReplyPort = myport;

	PutMsg(n->dllPort, (struct Message *)&msg);
	WaitPort(myport);
	reply = (dll_tMessage *)GetMsg(myport);

	DeleteMsgPort(myport);
	
	if (reply)
	{
		return(sym);
	}

	dllSetError("can't find symbol: %s", name);
	return NULL;
}

char *dllGetLastError(void)
{
	if (errorclear)
	{
		return NULL;
	}

	errorclear = TRUE;

	return dllError;
}
