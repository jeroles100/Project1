//General Overview: The program opens the three input files, and transfers all the necessary information into three parallel arrays,
//one for names of players, one for points to tally points that player earned, and one that holds the number of taggs that player has on each opponent
//I then work with the three arrays to print at the 3 verbosity levels using a different function for each

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h> //atoi
#include <vector>

using namespace std;

//returns the name of the neam and the number of players in a team
void getTeam(std::ifstream & file, string &name, int &players)
{
    string temp;
    getline( file, name);
    getline(file, temp);
    players = atoi(temp.c_str());

}

//cuts the portion of a string before its first space
void parseBack(string & parse)
{
    size_t pos;
    pos = parse.find(" ");
    parse = parse.substr(pos+1);
}


//returns the portion of a string before its first space
string parseFront(string parse)
{
    size_t pos;
    pos = parse.find(" ");
    parse = parse.substr(0, pos);
    return parse;
}

//fills a arrary of char arrays with the names of the players by calling the parseBack function to format the lines of the input file
//and places the names of all players in one array. Also fills the array of player ids with the corresponding id.
void playerRoster(std::ifstream & file1, std::ifstream & file2, char**& names, int*& ids, int players1, int players2)
{
    string buffer;
    string temp;
    int size = 0;

    while(size<players1)
    {
        getline(file1, buffer);
        temp = parseFront(buffer);
        ids[size]=atoi(temp.c_str());
        parseBack(buffer);
        names[size] = new char[buffer.length()+1];
        strcpy(names[size],buffer.c_str());
        size++;
    }
    while(size< players1 + players2)
    {
        getline(file2, buffer);
        temp = parseFront(buffer);
        ids[size]=atoi(temp.c_str());
        parseBack(buffer);
        names[size] = new char[buffer.length()+1];
        strcpy(names[size],buffer.c_str());
        size++;
    }

}

//initializes the array so all values will be zero
void initializeArray(int* points, int players1, int players2)
{
    int size = 0;
    while(size <players1+ players2)
    {
        points[size]= 0;
        size++;
    }
}

//initializes the double array so all values will be zero
void initializeArray(int**& tags, int players1, int players2)
{
    tags = new int*[players1+players2];
    int size = 0, initial=0;

    while(size<players1)
    {
        initial = 0;
        tags[size]= new int[players2];
        while(initial < players2)
        {
            tags[size][initial] = 0;
            initial++;
        }
        size++;

    }
    while(size<players1+players2)
    {
        initial = 0;
        tags[size]= new int[players1];
        while(initial < players1)
        {
            tags[size][initial] = 0;
            initial++;
        }
        size++;
    }

}

//adds the points earned from hitting a specific location to the corresponding element of the
//points array that runs parallel to the array of player names
void pointsArray(int* points, int tagger, int location)
{
    switch(location)
    {
        case 1 :
            points[tagger]+=5;
            break;
        case 2 :
            points[tagger]+=8;
            break;
        case 3 :
            points[tagger]+=10;
            break;
        case 4 :
            points[tagger]+=15;
            break;
        default :
            break;

    }

}

//adds a point for tagging someone to the corresponding element of the tag array
//that runs parallel to the array of player names/ and designates which opponent was tagged
void tagArray(int**& tags, int tagger, int target, int players1)
{
    if(target > players1-1)
        target = target - players1;

    tags[tagger][target]++;
}

//looks up the location of the arrays that contains the info for the person with the given id#(for the of chance that id #s are randomized since it wasn't specified)
void idSearch(int & id, int*& idArray, int size)
{
    bool exit = true;
    int counter= 0;

    while(exit)
    {
        if(idArray[counter]==id)
        {
            id = counter;
            exit = false;
        }
        if(counter > size) // this should only be the case if the user gives bad input
        {
            exit = false;
        }
        counter++;
    }

}

//goes through the match results file and seperates the person tagged, the tagger, and the location they were tagged
//and passes those values to the pointsArray and tagArray classes
void tagPoints(std::ifstream & file, int** tags, int* points,int*& ids, int& players1, int& players2)
{
    string buffer;
    string placeHolder;
    int tagger;
    int target;
    int location;

    getline(file, buffer);
    int totalTags = atoi(buffer.c_str());

    for(int i=0; i <totalTags; i++)
    {
        getline(file, buffer);
        placeHolder = parseFront(buffer);
        tagger = atoi(placeHolder.c_str());
        idSearch(tagger, ids, players1+players2);

        parseBack(buffer);
        placeHolder = parseFront(buffer);
        target = atoi(placeHolder.c_str());
        idSearch(target, ids, players1+players2);

        parseBack(buffer);
        parseBack(buffer);
        placeHolder = parseFront(buffer);
        location = atoi(placeHolder.c_str());

        pointsArray(points, tagger, location);
        tagArray(tags, tagger, target, players1);
    }

}

//tallys the total number of tags a player had against all its opponents
int tallyTags(int**& tags, int& position, int& opponents)
{
    int tally = 0;
    for(int k = 0; k < opponents; k++)
    {
        if(tags[position][k] > 0)
            tally = tally + tags[position][k];
    }
    return tally;
}

//prints the results in low verbosity
void printSim(std::ofstream & output, int* points, string& name1, string& name2, int& players1, int& players2)
{
    int points1=0, points2 = 0;
    for(int i = 0; i < players1; i++)
        points1+= points[i];
    for(int i = players1; i < players1+players2; i++)
        points2+= points[i];

    output << name1 << ": " << points1 << " points"<< endl;
    output << name2 << ": " << points2 << " points\nOverall Winners: ";

    if (points1 > points2)
        output << name1 << "\n\n";
    else
        output << name2 << "\n\n";
}

//prints the results in medium verbosity calls the printSim method to print overlapping lines
void printMed(std::ofstream & output, char**& names, int** tags, int* points, string& name1, string& name2, int& players1, int& players2)
{

    output << name1 << "\n";
    for(int i=0; i < players1; i++)
    {
    output << "\t" << names[i]<< " had a total of " << tallyTags(tags, i, players2) << " tags" << endl;
    }

    output << "\n"<< name2 << "\n";
    for(int i=players1; i < players1+players2; i++)
    {
     output << "\t" << names[i]<< " had a total of " << tallyTags(tags, i, players1) << " tags" << endl;
    }

    int bestScore = 0;
    for(int i = 1; i < players1; i++)
    {
        if(points[i] > points[bestScore])
            bestScore = i;

    }
    output << "\nBest score for " << name1 << ": " << names[bestScore] << " (" << points[bestScore]  << " points)" << endl;

    bestScore = 0;
    for(int i = players1; i < players1+players2; i++)
    {
        if(points[i] > points[bestScore])
            bestScore = i;

    }
    output <<  "Best score for " << name2 << ": " << names[bestScore] << " (" << points[bestScore] <<  " points)" << endl;

    printSim(output, points, name1, name2, players1, players2);

}

//Goes through the array of names and prints out how many times each player tagged each opponent, by also going throught the parallel tag array
void tagPrinter(std::ofstream & output, char**& names, int** tags, int endPrint, int columns, int startPrint)
{
    int startTag= 0;
    if(startPrint==0)
        startTag = endPrint;
    for(int i = startPrint; i < endPrint ; i++)
    {
        for(int k = 0; k < columns ; k++)
        {
            output <<"\t" << names[i] << " tagged " << names[startTag] << " " << tags[i][k] << " times" << endl;
            startTag++;
        }
        if(startPrint== 0)
            startTag = endPrint;
        else
            startTag = 0;
        output << "\t" << names[i] << " had a total of " << tallyTags(tags, i, columns) << " tags" << endl;
    }

}

//prints out in high verbosity and calls the tag printer class to help print
void printHigh(std::ofstream & output, char**& names, int** tags, int* points, string name1, string name2, int players1, int players2)
{
    int points1=0, points2 = 0;
    for(int i = 0; i < players1; i++)
        points1+= points[i];
    for(int i = players1; i < players1+players2; i++)
        points2+= points[i];
    output << name1 << "\n";
    tagPrinter(output, names, tags, players1, players2, 0);
    output << name1 << ": " << points1 << " points\n\n"<< endl;
    output << name2 << "\n";
    tagPrinter(output, names, tags, players1+players2, players1, players1);
    output << name2 << ": " << points2 << " points\n\nOverall Winners: ";

    if (points1 > points2)
        output << name1 << "\n\n";
    else
        output << name2 << "\n\n";
}

int main(int argc, char*argv[])
{
    if(argc != 6)
    {
        cout<< " ERROR => Invalid number of arguments. Usage is:\n./a.out <TeamA> <TeamB> <Output> <vlow|vmed|vhigh>\n\n"<< argv[0] << endl;

    }
    else
    {

        char** playerNames = NULL;
        int* playerIds = NULL;

        string teamName1, teamName2;
        int players1, players2;

       ifstream file1(argv[1]);
       ifstream file2(argv[2]);

       getTeam(file1, teamName1, players1);
       getTeam(file2, teamName2, players2);

       playerNames = new char*[players1+players2];
       playerIds = new int[players1+players2];

       playerRoster(file1, file2, playerNames, playerIds, players1, players2);

       file1.close();
       file2.close();

       int** tags = NULL;
       initializeArray(tags, players1, players2);

       int points[players1+players2];
       initializeArray(points, players1, players2);

       file1.open(argv[3]);
       ofstream output(argv[4]);

       tagPoints(file1, tags, points,playerIds, players1, players2);

       file1.close();

       if(strcmp(argv[5],"vlow") == 0)
       {
          printSim(output, points, teamName1, teamName2, players1, players2);
       }
       else if(strcmp(argv[5],"vmed") == 0)
       {
          printMed(output, playerNames, tags, points, teamName1, teamName2, players1, players2);
       }
       else if(strcmp(argv[5],"vhigh")==0)
          printHigh(output, playerNames, tags, points, teamName1, teamName2, players1, players2);

       output.close();
       return 0;
    }
}

