///usr/bin/g++
#include <iostream>
#include <vector>
#include "CombinationIterator.h"
#include "Graph.h"
#include "Solution.h"
#include "Process.h"
#include "mpi/mpi.h"
#include <stdio.h>

using namespace std;
#define MESSAGE_LENGTH 10 // staci
// zadost o praci
#define WORK_REQUEST 0
// master posle dalsi praci
#define MORE_WORK 1
// ukonceni prace
#define END_WORK 3
int message[MESSAGE_LENGTH]; // tohle chce jeste asi poresit

Process::Process(Graph * graph, int currentProcess, int processCount)
{
    this->graph = graph;
    this->isMaster = (currentProcess == 0);
    this->currentProcess = currentProcess;
    this->processCount = processCount;
    iter = 0;
    stopped = false;
    bestSolutionPossible = graph->getEdgeCount() % 2;
    myBestPrice = -1;
    this->processWorkUnit = 1; // lze rozsirovat podle efektivity. Znamena jedna hrana.
    this->processedWorkUnitCounter = 0; // master si pocita, kolik mu chybi jeste prelozdelit prace              
    this->solutionInIntMessageLenght = this->graph->getEdgeCount() / 32;
    this->solutionInIntMessageLenght += ((this->graph->getEdgeCount() % 32) > 0) ? 1: 0; // kolik intu je potreba na preneseni celeho reseni
}

void
Process::run()
{    
    // buffer pro MPI funkce
    MPI_Barrier(MPI_COMM_WORLD); // ceka se na nastartovani vsech procesu

    if (isMaster) 
    {
        // Pocatecni spravedlive rozdeleni prace vsem procesum
        for (int i = 1; i < this->processCount; i++) 
        {
            constructMoreWorkMessage();                
            MPI_Send(message, MESSAGE_LENGTH, MPI_INT, i, MORE_WORK, MPI_COMM_WORLD); // kazdemu slave poslu jeho zakladnik balik prace            
            if (processedWorkUnitCounter == this->graph->getEdgeCount()) break; // dal nerozdeluji, nektere procesy mohou zustat nevyuzity
        }
        
        int stoppedProcessesCounter = 0; // az zastavim vsechny ostatni procesy, mohu zastavit i sebe                
        while (true) // cekam na prichozi zpravy (proces 0)
        {
            MPI_Recv(&message, MESSAGE_LENGTH, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);            
            switch (status.MPI_TAG)
            {
                case WORK_REQUEST:
                    if (myBestPrice ==  - 1 || myBestPrice > message[0])
                    {
                        myBestPrice = message[0];                        
                        //TODO
                        //myBestSolution = new Solution(message)                        
                        // zpetna konveze pole intu do solution
                    }
                    if (processedWorkUnitCounter < this->graph->getEdgeCount() && myBestPrice != bestSolutionPossible) // mam jeste nejakou praci na pocitani ?
                    {
                        constructMoreWorkMessage();
                        MPI_Send(message, MESSAGE_LENGTH, MPI_INT, status.MPI_SOURCE , MORE_WORK, MPI_COMM_WORLD); // slave poslu zakladni balik prace            
                    }
                    else
                    {
                        MPI_Send(message, MESSAGE_LENGTH, MPI_INT, status.MPI_SOURCE, END_WORK, MPI_COMM_WORLD);
                        stoppedProcessesCounter++;
                    }
                    break;
                // mohou byt dalsi cases treba posilani mezivypoctu                        
            }            
            if (stoppedProcessesCounter == this->processCount - 1) break; // vsem procesum jsem poslal priznak zastaveni
        }        
    }
    else if (!isMaster) 
    {
        while (!stopped) 
        {          
            // blokacni zprava, cekam na instrukce od hlavniho procesu. Do statusu se nahraji zakladni udaje o charakteru zpravy.
            // zprava muze mit 
            MPI_Recv(&message, MESSAGE_LENGTH, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // checkMessages
            switch (status.MPI_TAG)
            {
                case MORE_WORK:
                {
                    int startEdge = message[0];
                    int endEdge = message[1];
                    printf("Proces %d: Od procesu %d prisel pokyn ke zpracovani useku %d az %d.", this->currentProcess, status.MPI_SOURCE, startEdge, endEdge);
                    EdgeCombination * c = new EdgeCombination(this->graph->getEdgeCount(), startEdge);
                    EdgeCombination * max = new EdgeCombination(this->graph->getEdgeCount(), endEdge);
                    iter = new CombinationIterator(c, max);
                    checkSolution(iter); // pocita se nove nejlepsi reseni
                    this->updateMessageWithBestSolution(); // aktualizuje se nejelepsi reseni                    
                    MPI_Send(message, MESSAGE_LENGTH, MPI_INT, 0, WORK_REQUEST, MPI_COMM_WORLD); // poslu procesu 0 svoje dosavadni nejlepsi skore a zadost o dalsi praci                    
                    break;
                }
                case END_WORK:
                {
                    this->stopped = true;
                    break;                    
                }
            }
        }
    }
    // master tiskne, jen kdyz nebylo reseni naleznuto
    if (isMaster) 
    {
        // print solution
        if (myBestPrice == -1) 
        {
            cout << "Reseni neexistuje" << endl;
        }
        else
        {
            myBestSolution->print();
        }
    }
}

void
Process::checkSolution(CombinationIterator * it)
{
    do 
    {
        // create solution: graph is shared instance, combination instance is cloned
        Solution * s = new Solution(this->graph, it->getCurrent()->clone());

        // triangle test and price test
        if (this->graph->testTriangleOk(it->getCurrent()) && (myBestPrice == -1 || myBestPrice > s->getPrice())) {
            // update data
            myBestPrice = s->getPrice();
            delete myBestSolution;
            myBestSolution = s;

            // if price is best possible then quit algorithm
            if (myBestPrice == bestSolutionPossible) {
                break;
            }
        }
        else {
            // delete current solution instance
            delete s;
        }

        // prepni na dalsi
        it->next();
    }
    while (it->hasNext());
}
void Process::constructMoreWorkMessage()
{
    // intervalovy balik ke zpracovani posilany procesu procesu                  
    message[0] = this->processedWorkUnitCounter; // startEdge
    processedWorkUnitCounter += processWorkUnit;

    if (processedWorkUnitCounter > this->graph->getEdgeCount()) // kouknu, jestli jsem se nedostal za hranu reseni
    {
        processedWorkUnitCounter = this->graph->getEdgeCount(); // upravim na pocet hran reseni;
    }
    message[1] = processedWorkUnitCounter; // endEdge   
}
void Process::updateMessageWithBestSolution()
{
    message[0] = this->myBestPrice;
    message[1] = this->solutionInIntMessageLenght;
    // TODO
    // konverzi predelat reseni (1|0|0|1|1) na int + vymyslet zarazku. V pripade reseni, ktere se do jednoho intu nevejde potreba vice intu
}

void
Process::checkMessages()
{
    // TODO
    // zkontroluj NEWBEST a pripadne se ukonci (v pripade nalezeni reseni s cenou bestPricePossible)
    // pokud jsi slave, zkontroluj SENDJOB
    // pokud jsi slave, cekej na JOB nebo na STOP (blokujici receive)
}

void
Process::sendMessages() const
{
    // TODO
    // zkontroluj, jestli nemuzes poslat NEWBEST (pokud je bestPricePossible, tak uz nic dalsiho neposilej)
    // pokud jsi slave, posli END nebo STATE (podle toho, jestli mas praci)
    // pokud jsi master a nekdo chtel praci, posli nekomu SENDJOB
    // pokud jsi master a vse je hotovo, posli prikazy STOP vsem slaveum	
}
