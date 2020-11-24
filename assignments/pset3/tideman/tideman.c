#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void merge_sort_pairs(int start, int end);
void merge_pairs(int start, int middle, int end);
void lock_pairs(void);
bool path(int start, int end);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // loop over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // if name matches
        if (!strcmp(name, candidates[i]))
        {
            // update ranks
            ranks[rank] = i;
            // return true;
            return true;
        }
    }
    // invalid vote
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // loop over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // loop over all ranks
        for (int j = 0; j < candidate_count; j++)
        {
            // others are preferred till current candidate is ranked
            if (ranks[j] != i)
            {
                preferences[ranks[j]][i]++;
            }
            // else break;
            else
            {
                break;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // initialize count to 0
    pair_count = 0;
    // for all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // for all other candidates
        for (int j = 0; j < i; j++)
        {
            // if i is preferred
            if (preferences[i][j] > preferences[j][i])
            {
                // add pair
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // increment count
                pair_count++;
            }
            // if j is preferred
            else if (preferences[j][i] > preferences[i][j])
            {
                // add pair
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                // increment count
                pair_count++;
            }
            // do nothing if tied
            else
            {

            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // sort all
    merge_sort_pairs(0, pair_count);
    return;
}

// sort pairs from index start (inclusive) to end (exclusive)
void merge_sort_pairs(int start, int end)
{
    // if less than 2 items, return
    if (end <= start + 1)
    {
        return;
    }

    // get middle item
    int middle = (start + end) / 2;

    // sort left half
    merge_sort_pairs(start, middle);

    // sort right half
    merge_sort_pairs(middle, end);

    // merge sorted halves
    merge_pairs(start, middle, end);

    return;
}

// merge pairs [start, middle) and [middle, end)
void merge_pairs(int start, int middle, int end)
{
    // temporary array to store merged arrays
    pair merged[end - start];

    // initialize indices
    int i = start, j = middle, k = 0;

    // loop till either index exhausts
    while (i < middle && j < end)
    {
        // if margin of i is more
        if (preferences[pairs[i].winner][pairs[i].loser] >
            preferences[pairs[j].winner][pairs[j].loser])
        {
            // store i
            merged[k].winner = pairs[i].winner;
            merged[k].loser = pairs[i].loser;

            // increment i,k
            i++;
            k++;
        }
        // otherwise
        else
        {
            // store j
            merged[k].winner = pairs[j].winner;
            merged[k].loser = pairs[j].loser;

            // increment j,k
            j++;
            k++;
        }
    }
    // loop until i
    while (i < middle)
    {
        // store i
        merged[k].winner = pairs[i].winner;
        merged[k].loser = pairs[i].loser;

        // increment i,k
        i++;
        k++;
    }
    // loop until j
    while (j < end)
    {
        // store j
        merged[k].winner = pairs[j].winner;
        merged[k].loser = pairs[j].loser;

        // increment j,k
        j++;
        k++;
    }

    // update pairs
    k = 0;
    for (i = start; i < end; i++)
    {
        pairs[i].winner = merged[k].winner;
        pairs[i].loser = merged[k].loser;
        k++;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // go through all pairs
    for (int i = 0; i < pair_count; i++)
    {
        // if no path from loser to winner
        if (!path(pairs[i].loser, pairs[i].winner))
        {
            // add winner to loser in graph
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// returns true if path from start to end in locked graph
bool path(int start, int end)
{
    // initialize queue
    int queue[candidate_count * candidate_count], qsize = 0, qstart = 0;
    queue[qstart + qsize] = start;
    qsize++;

    // while queue is non-empty
    while (qsize > 0)
    {
        // pop from queue
        int current = queue[qstart];
        qstart++;
        qsize--;

        // if end is found, return true
        if (current == end)
        {
            return true;
        }
        // else add neighbors to pair
        else
        {
            // for all candidates
            for (int i = 0; i < candidate_count; i++)
            {
                // if candidate neighbor
                // no need to check visited, graph is acyclic
                if (locked[current][i])
                {
                    // add in queue
                    queue[qstart + qsize] = i;
                    qsize++;
                }
            }
        }
    }

    // here if not found
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        bool winner = true;
        // check for arrows to i
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner = false;
                break;
            }
        }
        // print if winner
        if (winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

