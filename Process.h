#pragma once
#include <vector>
#include "CombinationIterator.h"
#include "Graph.h"
#include "Solution.h"

using namespace std;

/**
 * Process class
 *
 *   Workflow
 *   --------
 *   Master s cislem 0 se stara o komunikaci a prerozdelovani zprav. Slavey jen posilaji masteru
 *   svuj stav a reaguji na prikazy. Mezi sebou komunikuji pouze zpravami NEWBEST a JOB.
 *
 *   Procesy udelaji kus prace (zkontroluji TIMEOUT nebo MASTER_TIMEOUT kombinaci) v cyklu a potom
 *   zkontroluji a poslou zpravy (v tomto poradi, aby se tolik neposilaly neaktualni veci).
 *
 *   Master si drzi seznam slaveu a vi, co ktery dela za praci.
 *
 *   - Master pri inicializaci spocita nejake prvotni rozdeleni prace a posle prikazy JOB slaveum.
 *   - Slavey po skonceni cyklu posilaji:
 *     - Zpravu NEWBEST, pokud nasly nove maximum.
 *     - Pokud maji praci, poslou masteru STATE, pokud nemaji, poslou mu END.
 *   - Master se pri prijmu zpravy END koukne, komu zbyva nejvic prace. Tomu procesoru posle zpravu
 *     SENDJOB s parametrem, kteremu procesu ma praci poslat pomoci prikazu JOB.
 *   - Pokud je vypocet u konce a nebylo nalezeno trivialne nejlepsi mozne reseni (s cenou 0 nebo 1),
 *     master rozesle prikaz STOP slaveum.
 *
 *   Protokol zprav
 *   --------------
 *   Prvni radek je nazev prikazu a zbytek jsou data.
 *
 *   - NEWBEST\n2\nYYRRY
 *     - nová nejlepší kombinace je {YELLOW, YELLOW, RED, RED, YELLOW} a ma cenu 2
 *     - broadcast
 *     - pri prijmu zaktualizuju svuj stav
 *     - pokud je newbest nejlepsi mozne reseni, proces se ukonci
 *   - END\n2\nYYRRY
 *     - nemam praci a nejlepsi kombinace je {YELLOW, YELLOW, RED, RED, YELLOW} a ma cenu 2
 *     - poslat masteru, mozna se zmeni s prerozdelovanim
 *   - JOB\nRRR\nYYY
 *     - posilam praci od RRR do YYY
 *     - root posila na zacatku + pri prerozdelovanim
 *   - STATE\nRRR\nYYY
 *     - posila slave masteru, ze jeho aktualni kombinace je RRR a max je YYY, aby mel prehled
 *   - SENDJOB\n4
 *     - posila master slaveu, aby poslal slaveu c. 4 nejakou praci
 *   - STOP
 *     - posila master slaveum, aby se zastavily
 */
class Process
{
public:
	Process(Graph * graph, int processNum, int processCount);
	
	void run();
	void checkMessages();
	void checkSolution(EdgeCombination * c);
	void sendMessages() const;
	void sendJobMessage(int, CombinationIterator *) const;
	void receiveJobMessage();
	
	// kolik cyklu ma probehnout pred kontrolou zprav
	static const int TIMEOUT = 100;
	static const int MASTER_TIMEOUT = 50;

	static const int CMD_JOB = 1;
	static const int CMD_NEWBEST = 2;
	static const int CMD_STATE = 3;
	static const int CMD_END = 4;
	static const int CMD_SENDJOB = 5;
	static const int CMD_STOP = 6;

private:
	int processCount;
	int processNum;
	bool isMaster;
	bool stopped;
	Graph * graph;
	Solution * myBestSolution;
	CombinationIterator * iter;
	int bestSolutionPossible;
	int myBestPrice;	
};
