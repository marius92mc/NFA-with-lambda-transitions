/*
 * =====================================================================================
 *
 *       Filename:  lambda_NFA.cpp
 *
 *    Description:  1.Program care simuleaza functionarea unui automat finit nedeterminist cu lambda-tranzitii. Programul citeste (dintr-un fisier sau de la consola) elementele unui automat finit nedeterminist cu lambda-tranzitii oarecare (starile, starea initiala, starile finale, alfabetul de intrare, tranzitiile). Programul permite citirea unui nr oarecare de siruri peste alfabetul de intrarea. Pentru fiecare astfel de sir se afiseaza daca este acceptat sau nu de automat.
 *
 *        Version:  1.0
 *        Created:  03/02/14 14:44:59
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Marius-Constantin Melemciuc 
 *   Organization:  
 *
 * =====================================================================================
 */

/*
   Input
   				numar_varfuri/stari numar_arce
				stare_initiala
				numar_stari_finale
				sir de stari finale
				stare tranzitie/caracter stare'
				stare tranzitie/caracter stare'
				...
				stare tranzitie/caracter stare'
				numar_cuvinte_de_verificat
				cuvant de verificat
				cuvant de verificat
				...

*/

#include <iostream>
#include <cstdio>
#include <vector>
#include <stack>
#include <string.h>
#include <assert.h>

using namespace std;

int state0;

class LambdaNFA
{
	typedef struct nod {
							int y, nr_transition;
							char transition;
					   }NOD;
	vector<NOD> a[100];
	vector<unsigned int> final_states, cycle;
	int N, M, number_final_states, initial_state, count;
	bool word_accepted;
	FILE *in, *out;
public:
	LambdaNFA();
	~LambdaNFA();
	int read();
	int isFinalState(int);
	int numberOfStatesSameTransition(int, char);
	int solve(int, char*);
	int print();
};

LambdaNFA::LambdaNFA()
{
	count = 0;
	in = fopen("lambdanfa.in", "r");
	out = fopen("lambdanfa.out", "w");

	assert(in != NULL);
	assert(out != NULL);                     
}

LambdaNFA::~LambdaNFA()
{
	fclose(in);
	fclose(out);
}

int LambdaNFA::read()
{              // here we read the lambda-NFA
	NOD z;
	int x;
	
	if (fscanf(in, "%d %d %d %d", &N, &M, &initial_state, &number_final_states) != 4)
		return 0;
	
	state0 = initial_state;
	for (int i = 0; i < number_final_states; i++)
	{
		if (fscanf(in, "%d", &x) != 1)
			return 0;
		final_states.push_back(x);
	}
	
	for (int i = 1; i <= M; i++)
	{
		if (fscanf(in, "%d %c %d", &x, &z.transition, &z.y) != 3)
			return 0;
		z.nr_transition = 0;

		a[x].push_back(z);
	}
	return 1;
}

int LambdaNFA::isFinalState(int x)
{
	for (int i = 0; i < final_states.size(); i++)
		if (final_states[i] == x)
			return 1;
	
	return 0;
}

int LambdaNFA::numberOfStatesSameTransition(int state, char transition)
{
	int nr = 0;
	for (int i = 0; i < a[state].size(); i++)
		if (a[state][i].transition == transition)
			nr++;
	return nr;
}

int LambdaNFA::solve(int first_state, char* word)
{				
	stack<int> s;
	int primul, word_index = 0, j, k, word_length = strlen(word), good_to_push;
	
	s.push(first_state);
	while (s.empty() == 0 && !word_accepted)
	{
		primul = s.top();
		k = 0;

		good_to_push = 1;
		int modified_k = 0;
		int same_transition[1000], nr_same_transition = -1; // practic e same transition si/sau lambda-uri
		char simbol[1000];        // same_transition means nodes with the same transition
		
		for (int l = 0; l < a[primul].size() && !word_accepted; l++)
		{
			if (a[primul][l].transition == word[word_index]) 
			{
				nr_same_transition++; 
				same_transition[nr_same_transition] = a[primul][l].y;
				simbol[nr_same_transition] = a[primul][l].transition;
				if (modified_k == 0)
					k = 1;
			}
			else
				if (a[primul][l].transition == '0')
				{
					nr_same_transition++; 
					same_transition[nr_same_transition] = a[primul][l].y;
					simbol[nr_same_transition] = a[primul][l].transition;
					
					k = 1;	
					for (int u = 0; u < cycle.size(); u++)
						if (same_transition[l] == cycle[u])
						{
							for (int t = 0; t < cycle.size(); t++)
							cycle.erase(cycle.begin(), cycle.end());	
							k = 0;  // to pop it from stack	
							modified_k = 1;
						}	
				}
		}

	    if (nr_same_transition == -1)
			return 0;	
		if (k == 1)
		{
			for (int l = 0; l <= nr_same_transition && !word_accepted; l++)
			{			//make cu a l-lea varianta din same_...
				if (strlen(word) != 0)
				{
					if (simbol[l] != '0')
					{	
						if (isFinalState(same_transition[l]) && strlen(word + 1) == 0)
						{
							word_accepted = true;
							return 1;
						}	
						if (!word_accepted)
							if (solve(same_transition[l], word + 1) == 0)
								good_to_push = 0;
					} /* simbol[l] != '0' */
					else
					{
						cycle.push_back(same_transition[l]);
						if (isFinalState(same_transition[l]) && strlen(word) == 0)
						{
							word_accepted = true;
							return 1;
						}
						if (!word_accepted)	
						{
							count++;
							if (count > 10)
								return 0;
							if (solve(same_transition[l], word) == 0)
								good_to_push = 0;
						}
					} /* simbol[l] == '0' */
				} /* strlen(word) != 0 */
				
				if (good_to_push)
					s.push(same_transition[l]);
				if (isFinalState(same_transition[l]) && strlen(word) == 0)
				{
					word_accepted = true;
					return 1;
				}
			} /* for */
		} /* k == 1 */
		else
		{
			s.pop();
			if (modified_k == 1)
				return 0;
		}
	}
	return 0;
}


				
		

int LambdaNFA::print()
{
	int number_of_words;
	char* s;

	s = new char[10];
	
	if (fscanf(in, "%d", &number_of_words) != 1)
		return 0;
	for (int i = 1; i <= number_of_words; i++)
	{
		fscanf(in, "%s", s);

		word_accepted = false;
		count = 0;
		cycle.erase(cycle.begin(), cycle.end());
		solve(state0, s);	

		if (word_accepted)
			fprintf(out, "The word %s is accepted by the NFA.\n", s);
		else
			fprintf(out, "The word %s is not accepted by the NFA.\n", s);
	}

	delete [] s;
	return 1;
}


int main()
{
	LambdaNFA nfa;
	
	if (nfa.read() == 0)
		printf("\nError at reading.");
	
	nfa.print();

	return 0;
}	
