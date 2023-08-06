#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Trie structure
struct Trie
{	
    struct Trie *children[26];
    bool isEndOfWord;
    int count;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (pTrie->children[index] == NULL) {
            pTrie->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            for (int j = 0; j < 26; j++) {
                pTrie->children[index]->children[j] = NULL;
            }
            pTrie->children[index]->isEndOfWord = false;
            pTrie->children[index]->count = 0;
        }
        pTrie = pTrie->children[index];
    }
    pTrie->isEndOfWord = true;
    pTrie->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
     for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (pTrie->children[index] == NULL) {
            return 0;
        }
        pTrie = pTrie->children[index];
    }
    if (pTrie->isEndOfWord) {
        return pTrie->count;
    } else {
        return 0;
    }
}

void deallocateTrieNodes(struct Trie *pTrie) {
    // Base case
    if (pTrie == NULL) {
        return;
    }

    // Recursively deallocate child nodes
    for (int i = 0; i < 26; i++) {
        deallocateTrieNodes(pTrie->children[i]);
    }

    // Free the current node
    free(pTrie);
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) {
        return NULL;
    }

    // Deallocate all nodes recursively starting from the root
    deallocateTrieNodes(pTrie);

    // Set the root pointer to NULL (optional)
    pTrie = NULL;

    return pTrie;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (newTrie != NULL) {
        for (int i = 0; i < 26; i++) {
            newTrie->children[i] = NULL;
        }
        newTrie->isEndOfWord = false;
        newTrie->count = 0;
    }
    return newTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char ***pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    // Count the number of words in the dictionary file
    int numWords = 0;
    char word[100]; // Assuming maximum word length is 99 characters

    while (fscanf(file, "%s", word) == 1)
    {
        numWords++;
    }

    // Allocate memory for the array of pointers to words
    *pInWords = (char **)malloc(numWords * sizeof(char *));

    // Rewind the file pointer to read words again
    fseek(file, 0, SEEK_SET);

    // Read words into the array
    for (int i = 0; i < numWords; i++)
    {
        fscanf(file, "%s", word);

        // Allocate memory for each word and copy it to the array
        (*pInWords)[i] = (char *)malloc((strlen(word) + 1) * sizeof(char));
        strcpy((*pInWords)[i], word);
    }

    fclose(file);

    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}