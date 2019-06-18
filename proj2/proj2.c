/**
 * @file proj2.c
 * @brief Project 2 - Iterative calculations
 * Program is divided into 2 parts:
 *				1.part counts logarithm from any number
 *				2.part counts exponential function
 * @author Peter Koprda
 * @date November 2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/**
 * @defgroup Functions
 * @{
 */


/**
 * @brief Taylor polynomial for logarithm
 * @param x Number from which is logarithm calculated
 * @param n Count of iterations
 * @pre x is greater than 0
 */
double taylor_log(double x, unsigned int n)
{
	double result = 0.0;

	if(x > 0 && x < 1)
	/* x is from interval (0,1) */
	{
		double y = 1-x;						
		double power;						
		unsigned int j;						

		for(unsigned int i=1; i<=n; i++)
		/* while i is not greater than n, into function GetPower is stored variable i */
		{
			power = 1.0;
			for(j=i; j>0; j--)
			/* power from variable y */
			{
				power *= y;
			}
			/* fraction added to result */
			result += (((-1) * power) / i);

		}
	}

	else if(x >= 1)
	/* x is greater than or equal to 1 */
	{
		double power;						
		double numerator = (x-1) / x;
		unsigned int j;

		for(unsigned int i=1; i<=n; i++)
		/* loop counts sum of the fractions */
		{
			power = 1.0;
			for(j=i; j>0; j--)
			/* counts numerator that contains (((x-1)/x)^n) */
			{
				power *= numerator;
			}

			/* fraction added to result */
			result += power/i;
		}

	}

	return result;
}

/**
 * @brief Chained fraction for logarithm
 * @param x Number from which is logarithm calculated
 * @param n Count of iterations
 * @return Whole chained fraction
 */
double cfrac_log(double x,unsigned int n)
{
	double z = (x-1) / (x+1);
	double cf = 0.0;

	while(n >= 1)
	{
		cf = (n*n*z*z) / ((2*n+1)-cf);
		n--;
	}

	return (2*z) / (1-cf);
}

/**
 * @brief Calculates exponential function using function taylor_log
 * @param x Power
 * @param y Exponent
 * @param n Count of iterations
 * @pre x is greater than or equal to zero
 * @return Result of for loop increased by one
 */
double taylor_pow(double x,double y,unsigned int n)
{
	if((x >= 0) && (x != NAN) && (x != INFINITY)) 
	/* Power is greater than or equal to 0 */
	{
		double helpIncrement = 1.0;
		double result = 0.0;

		for(unsigned int i=1; i<n; i++)
		{
			helpIncrement *= (y*taylor_log(x,n)) / i;
			result += helpIncrement;					
		}

		return 1+result;
	}

	else
	/* Power is smaller than 0 */
	{
		if(x == NAN)
		{
			return NAN;
		}

		else
		{
			return INFINITY;
		}
	}
}

/**
 * @brief Calculates exponential function using function cfrac_log 
 * @param x Power
 * @param y Exponent
 * @param n Count of iterations
 * @pre x is greater than or equal to zero
 * @return Result of loop increased by one
 **/
double taylorcf_pow(double x, double y, unsigned int n)
{
	if((x >= 0) && (x != NAN) && (x != INFINITY))
	/* Power is greater than or equal to zero */
	{
		double helpIncrement = 1.0;
		double result = 0.0;

		for(unsigned int i=1; i<n; i++)
		{
			helpIncrement *= (y*cfrac_log(x,n)) / i;
			result += helpIncrement;							
		}

		return 1+result;
	}

	else
	/* Power is smaller than 0 */
	{
		if(x == NAN)
		{
			return NAN;
		}

		else
		{
			return INFINITY;
		}
	}	
}

/**
 * @}
 */


int main(int argc,char *argv[])
{
	if(argc < 4 || argc > 5)
	/* number of arguments is wrong */
	{
		fprintf(stderr,"ERROR! Wrong number of arguments!\n");
		return EXIT_FAILURE;
	}

	/** logarithm **/
	if (strcmp(argv[1],"--log")==0 && argc == 4)
	{
		/* log(x) */
		double x;									
		sscanf(argv[2],"%lf",&x);

		/* count of iterations */
		int n;										
		sscanf(argv[3],"%d",&n);			

		if((sscanf(argv[2],"%lf",&x)==0) || sscanf(argv[3],"%d",&n) == 0|| n <= 0)
		/** argument error handling **/
		{
			fprintf(stderr,"ERROR! Wrong argument!\n");
			return EXIT_FAILURE;
		}


		/** printing out logarithms **/

		printf("\tlog(%.5g) = %.12g\n",x,log(x));

		if(isinf(log(x)))
		/* x==0 */
		{
			printf("  cfrac_log(%.5g) = inf\n",x);						
			printf(" taylor_log(%.5g) = inf\n",x);						
		}

		else if (isnan(log(x)))
		/* x is negative number */
		{
			printf("  cfrac_log(%.5g) = -nan\n",x);						
			printf(" taylor_log(%.5g) = -nan\n",x);						
		}

		else
		/* x is positive number */
		{
			printf("  cfrac_log(%.5g) = %.12g\n",x,cfrac_log(x,n));		
			printf(" taylor_log(%.5g) = %.12g\n",x,taylor_log(x,n));	
		}

	}

	/** exponential function **/
	else if(strcmp(argv[1],"--pow")==0 && argc == 5)
	{
		/* power */
		double x;									
		sscanf(argv[2],"%lf",&x);

		/* exponent */
		double y;									
		sscanf(argv[3],"%lf",&y);

		/* count of iterations */
		int n;										
		sscanf(argv[4],"%d",&n);

		if(sscanf(argv[2],"%lf",&x)==0 || sscanf(argv[3],"%lf",&y)==0 || sscanf(argv[4],"%d",&n)==0 || n<=0)
		/* argument error handling */
		{
			fprintf(stderr,"ERROR! Wrong argument!\n");
			return EXIT_FAILURE;
		}


		/* printing out exponential function */

		printf("\t pow(%g,%g) = %.12g\n",x,y,pow(x,y));
		printf("  taylor_pow(%g,%g) = %.12g\n",x,y,taylor_pow(x,y,n));		
		printf("taylorcf_pow(%g,%g) = %.12g\n",x,y,taylorcf_pow(x,y,n));	
	}

	else
	{
		fprintf(stderr,"ERROR! Wrong number of arguments!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
