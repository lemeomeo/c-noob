/*
 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
 * void *(*start_routine) (void *), void *arg);
*/
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# define n_thread 11

int grid[9][9] = {
	{ 6, 5, 3, 1, 2, 8, 7 ,9 ,4 },
	{ 1, 7, 4, 3, 5, 9, 6, 8, 2 },
	{ 9, 2, 8, 4, 6, 7, 5, 3, 1 },
	{ 2, 8, 6, 5, 1, 4, 3, 7, 9 },
	{ 3, 9, 1, 7, 8, 2, 4, 5, 6 },
	{ 5, 4, 7, 6, 9, 3, 2, 1, 8 },
	{ 8, 6, 5, 2, 3, 1, 9, 4, 7 },
	{ 4, 1, 2, 9, 7, 5, 8, 6, 3 },
	{ 7, 3, 9, 8, 4, 6, 1, 2, 5 },
}; // sudoku is shared by the threads

int check[n_thread] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
// t1, t2, t3, t4, t5, t6, t7, t8, t9, tr, tc;

void *checksquare(void *param);
void *checkCol(void *param);
void *checkRow(void *param);

/* structure for passing data to threads */
typedef struct
{
int row;
int col;
} parameters;

void *checksquare(void *param)
{
	parameters *p = (parameters *) param;
	int row = p->row;
	int col = p->col;
	int valid_square[10] = {0};
	for (int i=row; i<row+3; ++i)
	{
		for (int j=col; j<col+3; ++j)
		{
			int value = grid[i][j];
			if (valid_square[value] != 0)
			{
				if (row == 0 && col == 0)
				{
					check[0] = 0;
				}
				else if(row == 0 && col == 3)
				{
					check[1] = 0;
				}
				else if(row == 0 && col == 6)
				{
					check[2] = 0;
				}
				else if(row == 3 && col == 0)
				{
					check[3] = 0;
				}
				else if(row == 3 && col == 3)
				{
					check[4] = 0;
				}
				else if(row == 3 && col == 6)
				{
					check[5] = 0;
				}
				else if(row == 6 && col == 0)
				{
					check[6] = 0;
				}
				else if(row == 6 && col == 3)
				{
					check[7] = 0;
				}
				else if(row == 6 && col == 6)
				{
					check[8] = 0;
				}
			}
			else
			{
				valid_square[value] = 1;
			}
		}	
	}

	for (int i=0; i < 9; i++)
	{
		if (check[i] != 0)
		{
			check[i] = 1;
		}
	}

}

void *checkCol(void *param)
{
	parameters *p = (parameters *) param;
	int row = p->row;
	int col = p->col;
	for (int i=col; i < 9; i++)
	{
		int valid_col[10] = {0};
		for (int j=row; j < 9; j++)
		{
			int value = grid[j][i];
			// a duplicate, not satisfy
			if (valid_col[value] != 0)
			{
				check[10] = 0;
			}
			else // no duplicate appears yet, set to 1
			{
				valid_col[value] = 1;
			}
		}
	}
	// if no duplicate in value_col array 
	if (check[10] != 0)
	{
		check[10] = 1;
	}

}

void *checkRow(void *param)
{
	parameters *p = (parameters *) param;
	int row = p->row;
	int col = p->col;
	for (int i=row; i < 9; i++)
	{
		int valid_row[10] = {0};
		for (int j=col; j < 9; j++)
		{
			int value = grid[i][j];
			// a duplicate, not satisfy
			if (valid_row[value] != 0)
			{
				check[9] = 0;
			}
			else // no duplicate appears yet, set to 1
			{
				valid_row[value] = 1;
			}
		}
	}
	// if no duplicate in value_col array 
	if (check[9] != 0)
	{
		check[9] = 1;
	}
}

int main()
{
	printf("CS149 Sudoku from Le Dao\n\n");

	for(int i=0; i < 9; i++)
	{
		for (int j=0; j<9; j++)
		{
			printf("%d ", grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	pthread_t threads[n_thread-2];
	// t1, t2, t3, t4, t5, t6, t7, t8, t9
	parameters *params[n_thread-2];
	int counter = 0;

	for (int i=0; i < 7; i = i+3)
	{
		for (int j=0; j< 7; j = j+3)
		{
			parameters *p = (parameters *) malloc(sizeof(parameters));
			p->row = i;
			p->col = j;
			params[counter] = p;
			pthread_create(&threads[counter], NULL, checksquare, p);
			counter++;
		}

	}

	pthread_t tr, tc;

	parameters *p0 = (parameters *) malloc(sizeof(parameters));
	p0->row = 0;
	p0->col = 0;

	// parameters *p1 = (parameters *) malloc(sizeof(parameters));
	// p1->row = 0;
	// p1->col = 0;

	// parameters *p2 = (parameters *) malloc(sizeof(parameters));
	// p2->row = 0;
	// p2->col = 3;	

	// parameters *p3 = (parameters *) malloc(sizeof(parameters));
	// p3->row = 0;
	// p3->col = 6;

	// parameters *p4 = (parameters *) malloc(sizeof(parameters));
	// p4->row = 3;
	// p4->col = 0;

	// parameters *p5 = (parameters *) malloc(sizeof(parameters));
	// p5->row = 3;
	// p5->col = 3;

	// parameters *p6 = (parameters *) malloc(sizeof(parameters));
	// p6->row = 3;
	// p6->col = 6;

	// parameters *p7 = (parameters *) malloc(sizeof(parameters));
	// p7->row = 6;
	// p7->col = 0;

	// parameters *p8 = (parameters *) malloc(sizeof(parameters));
	// p8->row = 6;
	// p8->col = 3;

	// parameters *p9 = (parameters *) malloc(sizeof(parameters));
	// p9->row = 6;
	// p9->col = 6;

	// create the threads
	// pthread_create(&t1, NULL, checksquare, p1);
	// pthread_create(&t2, NULL, checksquare, p2);
	// pthread_create(&t3, NULL, checksquare, p3);
	// pthread_create(&t4, NULL, checksquare, p4);
	// pthread_create(&t5, NULL, checksquare, p5);
	// pthread_create(&t6, NULL, checksquare, p6);
	// pthread_create(&t7, NULL, checksquare, p7);
	// pthread_create(&t8, NULL, checksquare, p8);
	// pthread_create(&t9, NULL, checksquare, p9);
	pthread_create(&tc, NULL, checkCol, p0);
	pthread_create(&tr, NULL, checkRow, p0);

	for (int i=0; i < 9; i++)
	{
		pthread_join(threads[i], NULL);
		free(params[i]);
	}
	// pthread_join(t1, NULL);
	// pthread_join(t2, NULL);
	// pthread_join(t3, NULL);
	// pthread_join(t4, NULL);
	// pthread_join(t5, NULL);
	// pthread_join(t6, NULL);
	// pthread_join(t7, NULL);
	// pthread_join(t8, NULL);
	// pthread_join(t9, NULL);
	pthread_join(tc, NULL);
	pthread_join(tr, NULL);

	free(p0);
	// free(p1);
	// free(p2);
	// free(p3);
	// free(p4);
	// free(p5);
	// free(p6);
	// free(p7);
	// free(p8);
	// free(p9);

	for (int i=0; i < 11; i++)
	{
		if (check[i] == 0)
		{
			printf("This is not a correct solution.\n");
			exit(0);
		}
	}
	printf("This is the correct solution.\n");
	return 0;
}