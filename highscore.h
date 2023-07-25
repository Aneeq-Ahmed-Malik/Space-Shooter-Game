#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class HighScore
{
private:
    string *names;
    string *scores;
    fstream myfile;
    int record_count;
    string fileName;

public:
    HighScore();

    void record_Input(string name, string score);
    string get_data();

    string *getName();
    string *getScore();
    int getCount();
};

HighScore::HighScore()
{
    names = new string[3];
    scores = new string[3];
    this->fileName = "scores.txt";
    myfile.open(this->fileName, ios::app | ios::in);
    myfile.close();
    record_count = 0;
}

void HighScore::record_Input(string name, string score)
{
    string data = get_data(), temp, data2 = data;
    this->record_count = this->getCount();
    ofstream myfile;

    myfile.open(this->fileName, ios::out);

    int index = this->record_count, count = 0, len = 0;

    for (int i = 0; i < this->record_count; i++)
    {
        len = 0;
        while (data[len] != '\n')
        {
            temp.push_back(data[len]);
            len++;
        }
        data = &data[len + 1];
        if (stoi(temp.substr(temp.find(",") + 1)) < stoi(score))
        {
            index = i;
            break;
        }
        temp.erase();
    }
    temp.erase();

    for (int i = 0; i <= this->record_count; i++)
    {
        if (i == index)
            myfile << name << "," << score << endl;
        else
        {
            len = 0;
            while (data2[len] != '\n')
            {
                temp.push_back(data2[len]);
                len++;
            }
            myfile << temp << "\n";
            data2 = &data2[len + 1];
            temp.erase();
        }
    }

    myfile.close();
}

string HighScore::get_data()
{

    string temp, s;
    myfile.open(this->fileName, ios::in | ios::app);

    myfile.seekg(0);
    myfile.seekp(0);

    int i = 0;

    while (myfile >> s)
    {
        temp += s;
        temp += '\n';
    }

    myfile.close();
    return temp;
}

string *HighScore::getName()
{
    string data = get_data();
    int len = 0;

    for (int i = 0; i < 3; i++)
    {
        len = 0;
        this->names[i].erase();
        while (data[len] != '\n')
        {
            this->names[i].push_back(data[len]);
            len++;
        }
        data = &data[len + 1];
        this->names[i] = this->names[i].substr(0, this->names[i].find_first_of(','));
    }
    return this->names;
}

string *HighScore::getScore()
{
    string data = get_data();
    int len = 0;

    for (int i = 0; i < 3; i++)
    {
        len = 0;
        while (data[len] != '\n')
        {
            this->scores[i].push_back(data[len]);
            len++;
        }
        data = &data[len + 1];

        this->scores[i] = this->scores[i].substr(this->scores[i].find_first_of(',') + 1);
    }
    return this->scores;
}

int HighScore::getCount()
{
    myfile.open(this->fileName, ios::in);
    int i = 0;
    string temp;
    while (!myfile.eof())
    {
        i++;
        myfile.ignore(256, '\n');
    }
    return i - 1;
}
