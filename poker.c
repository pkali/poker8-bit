#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <atari.h>
#include <peekpoke.h>

#define randbyte() PEEK(0xD20A)

typedef struct card {
	unsigned char figure;
	unsigned char colour;
} card_;

#include "eval.h"

typedef struct hand_val {
	unsigned char lo;
	unsigned char hi;
} hand_val_;


//GLOBALS
static unsigned char figures[] = "9TJQKA"; //T == 10
static unsigned char colours[] = "CDHS"; // Spades, Hearts, Diamonds, Clubs
static unsigned char colours_print[] = "\x7b\x48\x60\x10"; // Spades, Hearts, Diamonds, Clubs

const unsigned char FN = sizeof figures / sizeof *figures -1;
const unsigned char CN = sizeof colours / sizeof *colours -1;
card_ deck[4*6]; //[CN * FN]; // size of deck - number of colours times number of figures 
short unsigned int used_cards[4*6]; //[CN * FN];
 
unsigned char atari_rand(unsigned char low, unsigned char high)
/*
* returns a random number between low and high (inclusive)
* warning: it can be painfully slow for low (high-low) value
*/
{
	unsigned char result;
	do
	{
		result = randbyte();
	} while (result < low || result > high);
	return result;
}
 
 
// Prompt the user and get the input
unsigned char* getInput()
{
	unsigned char result[81];
	
	putchar('?');
	//putchar(' ');

	fgets(result, sizeof result, stdin);
	
	result[strlen(result) - 1] = '\0';

	return result;
}

void printHand(card_* hand)
{
	unsigned char result[15];
	short unsigned int j=0, i;
	for (i=0;i<5;i++)
	{
		result[j++]=figures[hand[i].figure];
		result[j++]=colours_print[hand[i].colour];
		result[j++]=' ';
	}
	result[j]='\0';
	printf(result);
}
void makeDeck(card_* deck)
{
	short unsigned int i,j,k;
	//unsigned char result[4*6*3]; //[CN*FN*3];
	//create deck
	k=0;
	for (i=0;i<CN;i++)
	{
		for (j=0;j<FN;j++)
		{
			deck[k].colour = i; //colours[i];
			deck[k].figure = j; // figures[j];
			k++;
		}
	}
	for (i=0; i< CN*FN ;i++)
	{
		used_cards[i] = 0;
	}
	/*
	j = 0;
	for (i = 0; i < CN*FN; i++)
	{
		result[j++] = deck[i].figure;
		result[j++] = deck[i].colour;
		result[j++] = ' ';
		//init used_cards table. 0 == unused, 1 - player 1, 2 - player 2 (CPU)
		used_cards[i] = 0;
	}
	result[j]='\0';
	printf(result);
	*/
}

card_ getRandomCard(int player)
/*
*get one card for a given player
*/
{
	short unsigned int card_index;
	do
	{
		card_index = atari_rand(0,FN*CN-1);
	} while (used_cards[card_index] != 0);
	used_cards[card_index] = player;
	return deck[card_index];
}

void randomizeHand(card_* hand, int player)
{
	unsigned int i;
	for (i=0;i<5;i++)
	{
		hand[i] = getRandomCard(player);
	}
}

int compare (const void * a, const void * b)
{

  card_ *orderA = (card_ *)a;
  card_ *orderB = (card_ *)b;

  return ( orderB->figure - orderA->figure );
}



void sortHand(card_* hand)
/*
*sort hand by figures
*/
{
	qsort(hand, 5, sizeof (card_), compare);
}

// Main function
int main(void)
{
	static unsigned char* input;			// User's input
	card_ player1_hand[5];
	card_ player2_hand[5]; //CPU

 
	for (;;)
	{
		//printf("rand: %d\n",atari_rand(10,20));
		makeDeck(deck);
		randomizeHand(player1_hand, 1);
		sortHand(player1_hand);
		printHand(player1_hand);
		analyze_hand(player1_hand);
		print_result();
		
		randomizeHand(player2_hand, 2);
		sortHand(player2_hand);
		
		printHand(player2_hand);
		analyze_hand(player2_hand);
		print_result();
		printf("--------------------------\n");
	}
}