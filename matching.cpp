// CMP201 AX1.8 The dog adoption problem.
// Author: William Kavanagh
// Partial Framework

// Solved by Sam Collier - 2100259

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

// helper function
// takes a vector of vectors, a person to index the row and then two dogs 1 and 2 to index the column.
// if the value of dog 1 comes BEFORE dog 2 in the person row of their preference, return true, else, return false.
// Hint: Use find(iterator, iterator, value).
bool personPrefersNewDog(std::vector<std::vector<int>> personPref, int person, int dog1, int dog2)
{
	// Assessed, 1 mark.
	// which ever dog comes first in the person's preference list is the dog they prefer.
	for(int i = 0; i < personPref[person].size(); i++)
	{
		if(personPref[person][i] == dog1)
		{
			return true;
		}
		else if(personPref[person][i] == dog2)
		{
			return false;
		}
	}
	return false;
}

// helper function
// takes a map of matches and returns the dog (i.e. the key) of the pair with the passed person as the value.
int dogMatchedToPerson(int person, std::map<int,int> matches)
{
	// Assessed, 1 mark.
	for(std::pair<int,int> pair : matches)
	{
		if(pair.second == person)
		{
			return pair.first;
		}
	}
	return -1;
}

// helper function
// returns true if there is a dog that has not proposed to a person
bool thereIsADogWhoHasNotProposedToAPerson(const std::map<int,std::map<int,int>>& dogProposals)
{
	for(int i=0; i < dogProposals.size(); i++)
	{
		for(int j=0; j < dogProposals.at(i).size(); j++)
		{
			if(dogProposals.at(i).at(j) == 0)
			{
				return true;
			}
		}
	}
	return false;
}


std::map<int, int> stableMarriage(std::vector<std::vector<int>> dogPref, std::vector<std::vector<int>> personPref)
{
	// Assessed, 3 marks.
	
	// Initialise all dogs and people to be free.

	std::vector<bool> cuffedDogs;
	for(int i=0; i<personPref[0].size(); i++)
	{
		cuffedDogs.push_back(false);
	}

	std::vector<bool> cuffedPeople;
	for(int i=0; i<dogPref[0].size(); i++)
	{
		cuffedPeople.push_back(false);
	}

	// create counts of how many proposals each dog has made.
	std::map<int,std::map<int,int>> dogProposals;
	for(int i=0; i < personPref[0].size(); i++)
	{
		for(int j = 0; j < dogPref[0].size(); j++)
		{
			dogProposals[i][j] = 0;
		}
	}

	// create a map of matches.
	std::map<int,int> matches;
	
	// while a dog is free and hasn't proposed to everyone yet.
	while(std::find(cuffedDogs.begin(),cuffedDogs.end(),false) != cuffedDogs.end()
			&& thereIsADogWhoHasNotProposedToAPerson(dogProposals))
	{
		// find the next free dog who still has a person to propose to
		int freeDog = std::find(cuffedDogs.begin(),cuffedDogs.end(),false) - cuffedDogs.begin();
		
		// find the next person they haven't proposed to.
		int nextPerson = -1;
		for(int person = 0; person < cuffedPeople.size(); person++)
		{
			if(dogProposals[freeDog][person] == 0)
			{
				nextPerson = person;
				break;
			}
		}
		
		// Dog proposes to person -- increment proposal.
		dogProposals[freeDog][nextPerson]++;

		// if the person is free..
		if(!cuffedPeople[nextPerson])
		{
			// person is not matched, match dog and person
			matches[freeDog] = nextPerson;
			cuffedPeople[nextPerson] = true;
			cuffedDogs[freeDog] = true;
		}
		else
		{
			// person is already matched to dog2 (use helper function).
			int dog2 = dogMatchedToPerson(nextPerson,matches);
			if(personPrefersNewDog(personPref,nextPerson,freeDog,dog2))
			{
				// person prefers new dog, dog2 is unmatched, dog1 and person make new match.
				cuffedDogs[dog2] = false;
				matches[freeDog] = nextPerson;
				cuffedDogs[freeDog] = true;
			}
		}
	}
	return matches;
}

// helper to check if a marriage is stable or not
bool isStableMarriage(const std::map<int,int>& matches, std::vector<std::vector<int>> dogPref, std::vector<std::vector<int>> personPref)
{
	for(int dog = 0; dog < personPref[0].size(); dog++)
	{
		// loop through each preferred person before the dog's match
		int person = matches.at(dog); // get current dog's matched person. This is the index we stop before
		int personIdx = std::find(personPref[dog].begin(),personPref[dog].end(),person) - personPref[dog].begin();
		// all people perferred to dog's match
		for(int i = 0; i < personIdx; i++)
		{
			int preferredPerson = personPref[dog][i];
			int theirDog = matches.at(preferredPerson);
			// if the other person prefers the dog to their own, return false.
			if(personPrefersNewDog(personPref,preferredPerson,dog,theirDog))
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	// Test 1, from lecture 13A
	std::vector<std::vector<int>> dogPref = {
		{1, 2, 3, 4, 0},
		{2, 3, 4, 1, 0},
		{2, 3, 1, 0, 4},
		{3, 2, 1, 0, 4},
		{2, 3, 0, 1, 4}
	};

	std::vector<std::vector<int>> peoplePref = {
		{1, 3, 2, 4, 0},
		{2, 3, 1, 4, 0},
		{2, 4, 1, 0, 3},
		{1, 2, 3, 0, 4},
		{2, 3, 0, 1, 4}
	};

	std::map<int, int> matches = stableMarriage(dogPref, peoplePref);

	std::cout << "Correct result should see matches of D0-P4, D1-P3, D2-P2, D3-P1 and D4-P0.\n";

	for (auto d : matches)
	{
		std::cout << d.first << " was matched with " << d.second << ", ";
	}

	std::cout << "\nThis is " << (isStableMarriage(matches, dogPref, peoplePref) ? "NOT " : "") << "a stable marriage.\n";
	std::cout << "\n";

	// Test 2
	dogPref = {
		{0, 2, 4, 6, 7, 1, 3, 5},
		{1, 3, 2, 0, 4, 5, 6, 7},
		{1, 2, 0, 4, 3, 6, 5, 7},
		{3, 1, 2, 4, 5, 6, 7, 0},
		{2, 0, 3, 1, 5, 7, 6, 4},
		{2, 1, 3, 0, 6, 7, 4, 5},
		{0, 2, 1, 4, 3, 7, 5, 6},
		{2, 3, 0, 1, 5, 7, 6, 4}
	};

	peoplePref = {
		{6, 7, 3, 1, 2, 4, 5, 0},
		{2, 0, 3, 1, 5, 7, 6, 4},
		{2, 1, 3, 4, 5, 0, 6, 7},
		{6, 0, 4, 2, 7, 1, 3, 5},
		{1, 0, 2, 3, 4, 5, 6, 7},
		{0, 4, 3, 5, 6, 7, 1, 2},
		{0, 3, 7, 5, 2, 1, 4, 6},
		{2, 3, 0, 1, 5, 7, 6, 4} 
	};

	matches = stableMarriage(dogPref, peoplePref);

	std::cout << "we are expecting: D0-P4, D1-P2, D2-P1, D3-P5, D4-P3, D5-P6, D6-P0, D7-P7:\n";

	for (auto d : matches)
	{
		std::cout << d.first << " was matched with " << d.second << ", ";
	}

	std::cout << "\nThis is " << (isStableMarriage(matches, dogPref, peoplePref) ? "NOT " : "") << "a stable marriage.\n";

	return 0;
}
