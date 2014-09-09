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

//cuts the portion of a string after its first space
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

//fills a arrary of char arrays with the names of the players by calling the parseBack function to format
void playerRoster(std::ifstream & file1, std::ifstream & file2, char**& names, int players1, int players2)
{
    string buffer;
    int size = 0;

    while(size<players1)
    {
        getline(file1, buffer);
        parseBack(buffer);
        names[size] = new char[buffer.length()+1];
        strcpy(names[size],buffer.c_str());
        size++;
    }
    while(size< players1 + players2)
    {
        getline(file2, buffer);
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

//goes through the match results file and seperates the person tagged, the tagger, and the location they were tagged
//and passes those values to the pointsArray and tagArray classes
void tagPoints(std::ifstream & file, int** tags, int* points,int players1)
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

        parseBack(buffer);
        placeHolder = parseFront(buffer);
        target = atoi(placeHolder.c_str());

        parseBack(buffer);
        parseBack(buffer);
        placeHolder = parseFront(buffer);
        location = atoi(placeHolder.c_str());

        pointsArray(points, tagger-1, location);
        tagArray(tags, tagger-1, target-1, players1);
    }

}

//prints the results in low verbosity
void printSim(int* points, string name1, string name2, int players1, int players2)
{
    int points1=0, points2 = 0;
    for(int i = 0; i < players1; i++)
        points1+= points[i];
    for(int i = players1; i < players1+players2; i++)
        points2+= points[i];

    cout << name1 << ": " << points1 << " points"<< endl;
    cout << name2 << ": " << points2 << " points\nOverall Winners: ";

    if (points1 > points2)
        cout << name1 << "\n\n";
    else
        cout << name2 << "\n\n";
}

//prints the results in medium verbosity
void printMed(char**& names, int** tags, int* points, string name1, string name2, int players1, int players2)
{
    int points1=0, points2 = 0;
    for(int i = 0; i < players1; i++)
        points1+= points[i];
    for(int i = players1; i < players1+players2; i++)
        points2+= points[i];

    int tally;
    cout << name1 << "\n\n";
    for(int i=0; i < players1; i++)
    {
     tally = 0;
     for(int k = 0; k < players2; k++)
     {
         if(tags[i][k] > 0)
             tally = tally + tags[i][k];
     }
     cout << names[i]<< " had a total of " << tally << " tags" << endl;
    }

    cout << "\n"<< name2 << "\n\n";
    for(int i=players1; i < players1+players2; i++)
    {
     tally = 0;
     for(int k = 0; k < players1; k++)
     {
         if(tags[i][k] > 0)
             tally = tally + tags[i][k];
     }
     cout << names[i]<< " had a total of " << tally << " tags" << endl;
    }

    int bestScore = 0;
    for(int i = 1; i < players1; i++)
    {
        if(points[i] > points[i-1])
            bestScore = i;
    }
    cout << "\nBest score for " << name1 << ": " << names[bestScore] << " (" << points[bestScore] << " points)" << endl;

    bestScore = players1;
    for(int i = players1+1; i < players1+players2; i++)
    {
        if(points[i] > points[i-1])
            bestScore = i;
    }
    cout << "Best score for " << name2 << ": " << names[bestScore] << " (" << points[bestScore] << " points)" << endl;



    cout << name1 << ": " << points1 << " points"<< endl;
    cout << name2 << ": " << points2 << " points\nOverall Winners: ";

    if (points1 > points2)
        cout << name1 << "\n\n";
    else
        cout << name2 << "\n\n";
}

int main()
{

    char** playerNames = NULL;

    string line, teamName1, teamName2, fileName1, fileName2, fileMatch, verbosity;
    int players1, players2;

    cout << "Team 1 textfile" << endl;
    cin >>  fileName1 ;
    cout << "Team 2 textfile" << endl;
    cin >>  fileName2 ;
    cout << "Match textfile" << endl;
    cin >>  fileMatch ;
    cout << "Verbosity" << endl;
    cin >>  verbosity ;

    cout << "\n\n";

    ifstream file1(fileName1.c_str());
    ifstream file2(fileName2.c_str());

    getTeam(file1, teamName1, players1);
    getTeam(file2, teamName2, players2);

    playerNames = new char*[players1+players2];

    playerRoster(file1, file2, playerNames, players1, players2);

    file1.close();
    file2.close();

    int** tags = NULL;
    initializeArray(tags, players1, players2);

    int points[players1+players2];
    initializeArray(points, players1, players2);

    file1.open(fileMatch.c_str());

    tagPoints(file1, tags, points, players1);

    file1.close();

    if(verbosity.compare("vlow") == 0)
    {
    printSim(points, teamName1, teamName2, players1, players2);
    }
    else if(verbosity.compare("vmed") == 0)
    {
    printMed(playerNames, tags, points, teamName1, teamName2, players1, players2);
    }
    else if(verbosity.compare("vhigh")==0)
        cout<< "I wish I had started this earlier. hopefully i got around to commenting, if not i am so sorry\n";
    return 0;
}

