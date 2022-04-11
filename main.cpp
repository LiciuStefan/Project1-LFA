#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <deque>

using namespace std;

ifstream fin("input.txt");

class Tranzitie
{
    int Nod1;
    int Nod2;
    char litera;
public:
    Tranzitie(int, int, char);
    char get_litera();
    int get_Nod1();
    int get_Nod2();
    friend istream& operator>>(istream&, Tranzitie&);
    friend ostream& operator<<(ostream&, const Tranzitie&);
};

Tranzitie :: Tranzitie(int primul_nod, int al_doilea_nod, char caracter)
{
    this->Nod1 = primul_nod;
    this->Nod2 = al_doilea_nod;
    this->litera = caracter;
}

char Tranzitie :: get_litera()
{
    return this->litera;
}

int Tranzitie :: get_Nod1()
{
    return this->Nod1;
}
int Tranzitie :: get_Nod2()
{
    return this->Nod2;
}

istream& operator>>(istream& in, Tranzitie& ob)
{
    in >> ob.Nod1 >> ob.Nod2 >> ob.litera;
    return in;
}

ostream& operator<<(ostream& os, const Tranzitie& ob)
{
    os << ob.Nod1 << ' ' << ob.Nod2 << ' ' << ob.litera;
    return os;
}

class AFN
{
    int number_of_states;
    int initial_state;
    vector<int>final_states;
    vector<vector<Tranzitie>>transitions;
public:
    AFN(int n, int m)
    {
        number_of_states = n;
        transitions.resize(m);
        initial_state = 0;
    }
    void add_transition(int nod1, int nod2, char c);
    void add_final_state(int nod);
    void set_initial_state(int);
    void print_transitions();
    void check_word(string, int, int, bool&, deque<int>&traseu);
};

void AFN :: add_transition(int nod1, int nod2, char c)
{
    Tranzitie* tranzitie = new Tranzitie(nod1, nod2, c);
    this->transitions[nod1].push_back(*tranzitie);
}

void AFN :: print_transitions()
{
    for (auto i = this->transitions.begin(); i != this->transitions.end(); ++i)
    {
        for (auto j = i->begin(); j != i->end(); ++j)
            cout << *j << ' ';
        cout << '\n';
    }
}

void AFN :: add_final_state(int nod)
{ 
    this->final_states.push_back(nod);
}

void AFN :: set_initial_state(int nod)
{
    this->initial_state = nod;
}

void AFN::check_word(string word, int word_index, int curr_state, bool& OK, deque<int>& traseu)
{
    if (word == "")
    {
        for (int i = 0; i < this->final_states.size(); ++i)
            if (this->final_states[i] == 0)
            {
                traseu.push_back(0);
                OK = true;
                break;
            }
    }

    if (word_index == word.size())
    {

        for (int i = 0; i < this->final_states.size(); ++i)
            if (this->final_states[i] == curr_state)
            {
                if (!traseu.empty() && traseu.back() != curr_state)
                    traseu.push_back(curr_state);
                OK = true;
            }

    }

    for (int p = 0; p < this->transitions[curr_state].size(); ++p)
    {
        if (word[word_index] == this->transitions[curr_state][p].get_litera())
        {
            if (OK == false)
                traseu.push_back(curr_state);
            check_word(word, word_index + 1, this->transitions[curr_state][p].get_Nod2(), OK, traseu);
            if (OK == false and !traseu.empty())
                traseu.pop_back();
        }
    }
}

int main()
{
    int num_of_states, num_of_transitions, nod_plecare, nod_destinatie, initial_state_index, final_state_index, number_of_final_states, number_of_strings;
    bool OK;
    char litera;
    string* word;
    deque<int>traseu;
    AFN* G;
    fin >> num_of_states>>num_of_transitions;
    G = new AFN(num_of_states, num_of_transitions);

    for (int i = 0; i < num_of_transitions; ++i)
    {
        fin >> nod_plecare >> nod_destinatie >> litera;
        G->add_transition(nod_plecare, nod_destinatie, litera);
    }

    fin >> initial_state_index;
    G->set_initial_state(initial_state_index);

    fin >> number_of_final_states;
    for (int i = 0; i < number_of_final_states; ++i)
    {
        fin >> final_state_index;
        G->add_final_state(final_state_index);
    }

    fin >> number_of_strings;

    for (int i = 0; i < number_of_strings; ++i)
    {
        OK = false;
        int word_index = 0;
        word = new string();
     
        fin >> *word;
        G->check_word(*word, word_index, initial_state_index, OK, traseu);
            if (OK)
                cout << "DA\n";
            else cout << "NU";
        while(!traseu.empty())
        {
            cout << traseu.front() << ' ';
            traseu.pop_front();
        }
        cout << '\n';
    }
   
    return 0;
}