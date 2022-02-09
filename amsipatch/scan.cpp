#include "Header.h"

#define NAUGHTY_STRING "THIS-IS-ABSOLUTELY-A-VIRUS-FUCK-QUICK-DELETE-ME"

struct BigBadVirus {
	BYTE* payload;
	ULONG girth;
};


void scan() {
	HRESULT returnResult = S_OK;
	HAMSICONTEXT amsiContext;
	AMSI_RESULT ismeavirus = AMSI_RESULT_DETECTED;
	HAMSISESSION OnTheSessBoys = nullptr;

	struct BigBadVirus paylo4d;

	paylo4d.payload = (BYTE*)NAUGHTY_STRING;
	paylo4d.girth = strlen(NAUGHTY_STRING);

	ZeroMemory(&amsiContext, sizeof(amsiContext));
	// error handling is over-rated
	returnResult = AmsiInitialize((LPCWSTR)"MyAmsiScanner", &amsiContext);
	returnResult = AmsiOpenSession(amsiContext, &OnTheSessBoys);
	returnResult = AmsiScanBuffer(amsiContext, paylo4d.payload, paylo4d.girth, (LPCWSTR)"SuperDuperScaryVirus", OnTheSessBoys, &ismeavirus);
}