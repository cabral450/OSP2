//Daniel Sze
//Robert Carneiro
#include "main.h"
#include "memory.h"

int main(int argc, char *argv[])
{
	if(argc < 2 || argc > 3) err("Invalid arguments\n");

	std::string fname1=argv[1];
	std::ifstream fd(fname1.c_str());
	std::string line;
	std::vector<std::string> lines;
	memory m;

	if (fd != NULL){
		while (std::getline(fd, line)){
			lines.push_back(line);
		}

        parsePhys(lines, m);

        fd.close();
	} else {
        err("Invalid file\n");
    }

    line.clear();
    lines.clear();
	std::string fname2=argv[1];
	fd.open(fname2.c_str());

	if (fd != NULL){
		while (std::getline(fd, line)){
			lines.push_back(line);
		}

        //parseVirtual(lines, m);

        fd.close();
	} else {
        err("Invalid file\n");
    }

	//for(int i=0;i<3;++i)
		//TBD(m, i);

	return 0;
}

void parsePhys(std::vector<std::string> lines, memory& m){
	bool first = true;
	//int numProcesses = 0;
	  
	for (unsigned int i = 0; i < lines.size(); ++i){
		if (lines[i][0] == '#')
			continue;
		if (lines[i].empty())
			continue;
		if (first){
			first = false;
			continue;
		}

		char* parseString = new char [lines[i].length()+1];
        std::strcpy(parseString, lines[i].c_str()); 

		char* splitText = strtok(parseString, " ");
		char procName = splitText[0];

        splitText = strtok(NULL, " ");
		int memSize = atoi(splitText);

		std::vector<std::string> paramList;
        splitText = strtok(NULL, " ");

		while (splitText != NULL){
			std::string convText(splitText);
			paramList.push_back(convText);
			splitText = strtok(NULL, " ");
		}

		process p(procName, memSize);
		
		for (unsigned int j = 0; j < paramList.size(); j++){
		   std::size_t index = paramList[j].find('/'); 
		   int arrival = atoi(paramList[j].substr(0, index).c_str());
		   int duration = atoi(paramList[j].substr(index+1).c_str());
		   burst b(arrival, duration);
		   p.bursts.push_back(b);
		}  
        
        m.processList.push_back(p);
        delete(parseString);
	}
}

void TBD(memory m, int algoFlag)
{
	int timeElapsed=0;

	std::cout<<"time "<<timeElapsed<<"ms: ";
	if(algoFlag == memory::NEXTFIT)
		std::cout<<"Simulator started (Contiguous -- Next-Fit)"<<std::endl;
	else if(algoFlag == memory::BESTFIT)
		std::cout<<"Simulator started (Contiguous -- Best-Fit)"<<std::endl;
	else if(algoFlag == memory::WORSTFIT)
		std::cout<<"Simulator started (Contiguous -- Worst-Fit)"<<std::endl;

	while(!m.isFinished(timeElapsed))
	{
		//get the next event
		int event=0;//0 for arrival, 1 for exit/end of duration
		process p;
		bool success=m.nextEvent(timeElapsed, event, p);

		if(success)
		{
			if(event == 0)
			{
				//add the process to the memory
				std::cout<<"time "<<timeElapsed<<"ms: ";
				std::cout<<"Process "<<p.processName<<" arrived (requires "<<p.memSize<<" frames)"<<std::endl;

				m.addProcess(p, algoFlag);

				std::cout<<"time "<<timeElapsed<<"ms: ";
				std::cout<<"Placed process "<<p.processName<<":"<<std::endl;
			}
			else if(event == 1)
			{
				//remove the process from the memory
				std::cout<<"time "<<timeElapsed<<"ms: ";
				std::cout<<"Process "<<p.processName<<" removed:"<<std::endl;
			}
		}
		else
		{
			//defragment if the memory isnt full
			//m.defragment();
		}

		m.print();
	}

	std::cout<<"time "<<timeElapsed<<"ms: ";
	if(algoFlag == memory::NEXTFIT)
		std::cout<<"Simulator ended (Contiguous -- Next-Fit)"<<std::endl;
	else if(algoFlag == memory::BESTFIT)
		std::cout<<"Simulator ended (Contiguous -- Best-Fit)"<<std::endl;
	else if(algoFlag == memory::WORSTFIT)
		std::cout<<"Simulator ended (Contiguous -- Worst-Fit)"<<std::endl;
}

std::string intTOstring(int number)
{
   std::stringstream ss;
   ss<<number;
   return ss.str();
}

std::string floatTOstring(float number)
{
   std::stringstream ss;
   ss<<number;
   return ss.str();
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
