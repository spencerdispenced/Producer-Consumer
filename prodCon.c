#include <stdio.h>
#include <pthread.h>



pthread_mutex_t the_mutex;
pthread_cond_t condRed, condGreen, condBlue, condc, condp;


int redBuffer = 0;
int greenBuffer = 0;
int blueBuffer = 0;
int buffer = 0;



int
isPrime( int n )
{
  int     i;

  for ( i = 2 ; i*i <= n ; i++ )
  {
    if ( n % i == 0 )
    {
      return 0;
    }
  }
  return 1;
}


static int isfib( int i )
{
  static int  x = 1, y = 1;
  int   z;

  if ( i == y )
  {
    z = x + y;
    x = y;
    y = z;
    return 1;
  }
  else
  {
    return 0;
  }

  
}

int is3or5( int i )
{
  if ( i % 3 == 0 ) return 1;
  else if ( i % 5 == 0 )  return 1;
  else      return 0;
}


void* producer(void *ptr) {

  int i;

  for (i = 1 ;; i++) 
  {
    pthread_mutex_lock(&the_mutex);	

    while ((redBuffer != 0) || (greenBuffer != 0) || (blueBuffer != 0))		       
    {
					  
      pthread_cond_wait(&condp, &the_mutex);

    }


      if (isfib(i) == 1)
      {
        greenBuffer = i;
        pthread_cond_signal(&condGreen);  

      }

      else if (isPrime(i) == 1)
      {
        redBuffer = i;
        pthread_cond_signal(&condRed); 

      }


      else if (is3or5(i) == 1)
      {
        blueBuffer = i;
        pthread_cond_signal(&condBlue);  

      }

      else
      {
        buffer = i;
        pthread_cond_signal(&condc);  /* wake up consumer */

      }

    
    pthread_mutex_unlock(&the_mutex);	/* release the buffer */
    
  }
  pthread_exit(0);
}

void* red(void *ptr) { // prime numbers

  int i;

  for (i = 1 ;; i++) 
  {
    pthread_mutex_lock(&the_mutex);	

    while (redBuffer == 0)			// If there is nothing in buffer
    {
				pthread_cond_wait(&condRed, &the_mutex); // wait if nothing
    }

    if (isPrime(redBuffer) == 1)
    {
      printf("\x1B[31m" "%d red\n" "\x1B[0m", redBuffer);
      redBuffer = 0;
      
    }

    redBuffer = 0;
    pthread_cond_signal(&condp);	
    pthread_mutex_unlock(&the_mutex);	
   

  }
  pthread_exit(0);
}



void* green(void *ptr) { //fib numbers

  int i;

  for (i = 1 ;; i++) 
  {
    pthread_mutex_lock(&the_mutex); 

    while (greenBuffer == 0)     
    {
        pthread_cond_wait(&condGreen, &the_mutex); 
    }

    
    printf("\x1B[32m" "%d    Green\n" "\x1B[0m", greenBuffer);
    greenBuffer = 0;

    pthread_cond_signal(&condp);  
    pthread_mutex_unlock(&the_mutex); 
   

  }
  pthread_exit(0);
}

void* blue(void *ptr) { // 3 & 5 numbers
  int i;

  for (i = 1 ;; i++) {
    pthread_mutex_lock(&the_mutex); 

    while (blueBuffer == 0)     
    {
        pthread_cond_wait(&condBlue, &the_mutex); 
    }

    if (is3or5(blueBuffer) == 1)
    {
      printf("\x1B[34m" "%d        blue\n" "\x1B[0m", blueBuffer);
      blueBuffer = 0;
      

    }
    

    blueBuffer = 0;
    pthread_cond_signal(&condp);  
    pthread_mutex_unlock(&the_mutex); 
   

  }
  pthread_exit(0);
}



int main(int argc, char **argv) 
{
  pthread_t pro, redTread, greenTread, blueTread, yellowTread;

  // Initialize the mutex and condition variables
  pthread_mutex_init(&the_mutex, NULL);	
  pthread_cond_init(&condRed, NULL);		
  pthread_cond_init(&condGreen, NULL);    
  pthread_cond_init(&condBlue, NULL); 
  pthread_cond_init(&condc, NULL);   

  pthread_cond_init(&condp, NULL);		

  // Create the threads
  pthread_create(&redTread, NULL, red, NULL);
  pthread_create(&greenTread, NULL, green, NULL);
  pthread_create(&blueTread, NULL, blue, NULL);
  

  pthread_create(&pro, NULL, producer, NULL);

  // Wait for the threads to finish
  pthread_join(redTread, NULL);
  pthread_join(greenTread, NULL);
  pthread_join(blueTread, NULL);

  pthread_join(pro, NULL);

  // Cleanup -- would happen automatically at end of program
  pthread_mutex_destroy(&the_mutex);	
  pthread_cond_destroy(&condRed);		
  pthread_cond_destroy(&condGreen);   
  pthread_cond_destroy(&condBlue); 
  pthread_cond_destroy(&condc);  

  pthread_cond_destroy(&condp);		

  return 0;
}