/*********************************************************
 * From C PROGRAMMING: A MODERN APPROACH, Second Edition *
 * By K. N. King                                         *
 * Copyright (c) 2008, 1996 W. W. Norton & Company, Inc. *
 * All rights reserved.                                  *
 * This program may be freely distributed for class use, *
 * provided that this copyright notice is retained.      *
 *********************************************************/

/* poker.c (Chapter 10, page 233) */
/* Classifies a poker hand */

#include <stdbool.h>   /* C99 only */
#include <stdio.h>
#include <stdlib.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS 5

typedef struct card {
	unsigned char figure;
	unsigned char colour;
} card_;



/* external variables */
short int num_in_rank[NUM_RANKS];
short int num_in_suit[NUM_SUITS];
bool straight, flush, four, three;
short int pairs;   /* can be 0, 1, or 2 */

/* prototypes */
/*
void read_cards(void);
void analyze_hand(void);
void print_result(void);
*/

/**********************************************************
 * main: Calls read_cards, analyze_hand, and print_result *
 *       repeatedly.                                      *
 **********************************************************/
/*
int main(void)
{
  for (;;) {
    read_cards();
    analyze_hand();
    print_result();
  }
}
*/
/**********************************************************
 * read_cards: Reads the cards into the external          *
 *             variables num_in_rank and num_in_suit;     *
 *             checks for bad cards and duplicate cards.  *
 **********************************************************/
/*
void read_cards(void)
{
  bool card_exists[NUM_RANKS][NUM_SUITS];
  char ch, rank_ch, suit_ch;
  int rank, suit;
  bool bad_card;
  int cards_read = 0;

  for (rank = 0; rank < NUM_RANKS; rank++) {
    num_in_rank[rank] = 0;
    for (suit = 0; suit < NUM_SUITS; suit++)
      card_exists[rank][suit] = false;
  }

  for (suit = 0; suit < NUM_SUITS; suit++)
    num_in_suit[suit] = 0;

  while (cards_read < NUM_CARDS) {
    bad_card = false;

    printf("Enter a card: ");

    rank_ch = getchar();
    switch (rank_ch) {
      case '0':           exit(EXIT_SUCCESS);
      case '2':           rank = 0; break;
      case '3':           rank = 1; break;
      case '4':           rank = 2; break;
      case '5':           rank = 3; break;
      case '6':           rank = 4; break;
      case '7':           rank = 5; break;
      case '8':           rank = 6; break;
      case '9':           rank = 7; break;
      case 't': case 'T': rank = 8; break;
      case 'j': case 'J': rank = 9; break;
      case 'q': case 'Q': rank = 10; break;
      case 'k': case 'K': rank = 11; break;
      case 'a': case 'A': rank = 12; break;
      default:            bad_card = true;
    }

    suit_ch = getchar();
    switch (suit_ch) {
      case 'c': case 'C': suit = 0; break;
      case 'd': case 'D': suit = 1; break;
      case 'h': case 'H': suit = 2; break;
      case 's': case 'S': suit = 3; break;
      default:            bad_card = true;
    }

    while ((ch = getchar()) != '\n')
      if (ch != ' ') bad_card = true;

    if (bad_card)
      printf("Bad card; ignored.\n");
    else if (card_exists[rank][suit])
      printf("Duplicate card; ignored.\n");
    else {
      num_in_rank[rank]++;
      num_in_suit[suit]++;
      card_exists[rank][suit] = true;
      cards_read++;
    }
  }
}
*/
/**********************************************************
 * analyze_hand: Determines whether the hand contains a   *
 *               straight, a flush, four-of-a-kind,       *
 *               and/or three-of-a-kind; determines the   *
 *               number of pairs; stores the results into *
 *               the external variables straight, flush,  *
 *               four, three, and pairs.                  *
 **********************************************************/
void analyze_hand(card_* hand)
{
  short int num_consec = 0;
  short int rank, suit, i;

  straight = false;
  flush = false;
  four = false;
  three = false;
  pairs = 0;

  //convert hand into num_in_suit format
  for (rank = 0; rank < NUM_RANKS; rank++)
    num_in_rank[rank] = 0;
  for (suit = 0; suit < NUM_SUITS; suit++)
    num_in_suit[suit] = 0;

	for (i=0;i<5;i++)
	{
		rank = hand[i].figure;
		suit = hand[i].colour;
		num_in_rank[rank]++;
		num_in_suit[suit]++;
	
	}
  
  /* check for flush */
  for (suit = 0; suit < NUM_SUITS; suit++)
    if (num_in_suit[suit] == NUM_CARDS)
      flush = true;

  /* check for straight */
  rank = 0;
  while (num_in_rank[rank] == 0) rank++;
  for (; rank < NUM_RANKS && num_in_rank[rank] > 0; rank++)
    num_consec++;
  if (num_consec == NUM_CARDS) 
  {
    straight = true;
    return;
  }

  /* check for 4-of-a-kind, 3-of-a-kind, and pairs */
  for (rank = 0; rank < NUM_RANKS; rank++) {
    if (num_in_rank[rank] == 4) four = true;
    if (num_in_rank[rank] == 3) three = true;
    if (num_in_rank[rank] == 2) pairs++;
  }
}

/**********************************************************
 * print_result: Prints the classification of the hand,   *
 *               based on the values of the external      *
 *               variables straight, flush, four, three,  *
 *               and pairs.                               *
 **********************************************************/
void print_result(void)
{
  if (straight && flush) printf("Straight flush");
  else if (four)         printf("Four of a kind");
  else if (three &&
           pairs == 1)   printf("Full house");
  else if (flush)        printf("Flush");
  else if (straight)     printf("Straight");
  else if (three)        printf("Three of a kind");
  else if (pairs == 2)   printf("Two pairs");
  else if (pairs == 1)   printf("Pair");
  else                   printf("High card");

  printf("\n\n");
}


/* python evaluator by http://stackoverflow.com/users/1355221/dansalmo 
def poker(hands):
    scores = [(i, score(hand.split())) for i, hand in enumerate(hands)]
    winner = sorted(scores , key=lambda x:x[1])[-1][0]
    return hands[winner]

def score(hand):
    ranks = '23456789TJQKA'
    rcounts = {ranks.find(r): ''.join(hand).count(r) for r, _ in hand}.items()
    score, ranks = zip(*sorted((cnt, rank) for rank, cnt in rcounts)[::-1])
    if len(score) == 5:
        if ranks[0:2] == (12, 3): #adjust if 5 high straight
            ranks = (3, 2, 1, 0, -1)
        straight = ranks[0] - ranks[4] == 4
        flush = len({suit for _, suit in hand}) == 1
        '''no pair, straight, flush, or straight flush'''
        score = ([1, (3,2,0)], [(3,2,1), 5])[flush][straight]
    return score, ranks

 >>> poker(['8C TS KC 9H 4S', '7D 2S 5D 3S AC', '8C AD 8D AC 9C', '7C 5H 8D TD KS'])
 '8C AD 8D AC 9C'
 */
 
 /* full algo by Nick
 Once you get down to 5 cards, you first take a histogram of the card ranks. That is, for each rank in the hand, count how often it appears. Sort the histogram by the count backwards (high values first).

If the histogram counts are 4 and 1, then the hand is quads.

If the histogram counts are 3 and a 2, then the hand is a boat.

If the histogram counts are 3, 1 and 1, then the hand is a set.

If the histogram counts are 2, 2 and 1, then the hand is two pair.

If the histogram has 4 ranks in it, then the hand is one pair.

Next, check to see if the hand is a flush. You do this by iterating through the cards to see if the suit of a card is the same as its neighbor. If not, then the hand is not a flush. Don't return a result yet, just note whether or not it's a flush.

Next, check for straights. Do this by sorting the list of cards. Then subtract the rank of the bottom card from the top card. If you get 4, it's a straight. At this point, you must also check either for the wheel or for broadway, depending on whether your sort puts the ace at the top or bottom. I would expect most folks to put the ace at the top of the sort, since it is usually a high card. So to check for the wheel, check to see if the top card is an ace and the 2nd to top card is a 5. If so, then it is the wheel.

If the hand is a straight and a flush, it's a straight-flush. Otherwise if it's one or the other, you can return that.

If we haven't matched the hand by now, it's High Card.

Once you know what the hand is, it is fairly straightforward to compare two hands of the same type. For straights, you simply compare the top card. For flushes, you iterate down through a reverse sort of the ranks until you find a higher card or run out. For the histogram-based hands, you start at the beginning of the reverse-sorted count list and compare the ranks (two pair is tricky - you must always evaluate the higher of the two pairs for each hand first - the histogram won't help, then the lower pair, then the kicker).

Checking for a low (for high/low or Razz) is a little easier. Sort the hand by rank. Iterate through the list. If any card is bigger than an 8 or is the same rank as it's neighbor, it's not a low hand.
POSTED BY NICK AT 10:16 AM

*/
