#include<iostream>
#include<cstdio>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<cstring>
#include <time.h>

#include <cstdlib>
#include <ctime>


using namespace std;

#define number_of_cycle 10
#define number_of_serviceman 3
#define number_of_payment 2
#define arr_size 3
#define sleep_time 3


int ind = 0;
sem_t empty_array;
pthread_mutex_t lock[number_of_serviceman];
pthread_mutex_t lock2;
pthread_mutex_t ind_lock;


void some()
{
    srand((unsigned)time(0));
    int i;
    i = (rand()%3)+1;
    //printf("\n%d\n",i);
    sleep(i);
}



void* repair_shop(void* arg){


            pthread_mutex_lock(&lock2);



        for(int i = 0; i < number_of_serviceman; i++){
        if(i==0)
        {
            pthread_mutex_unlock(&lock2);
            //printf("\nUnlocked\n");
        }

        pthread_mutex_lock(&lock[i]);

        printf("%s started taking service from serviceman %d \n",(char*)arg,i+1);
        some();
        printf("%s finished taking service from serviceman %d \n",(char*)arg,i+1);

        pthread_mutex_unlock(&lock[i]);

        }

        sem_wait(&empty_array);
        //pthread_mutex_lock(&lock2);

        printf("%s started paying the service bill\n",(char*)arg);
        some();
        printf("%s finished paying the service bill\n",(char*)arg);

        //pthread_mutex_unlock(&lock2);
        sem_post(&empty_array);


            /*pthread_mutex_lock(&ind_lock);
            ind++;
            pthread_mutex_unlock(&ind_lock);*/


            pthread_mutex_lock(&lock2);
            for(int i = 0; i < number_of_serviceman; i++)
            {
                pthread_mutex_lock(&lock[i]);
            }

            /*pthread_mutex_lock(&ind_lock);
            ind--;
            pthread_mutex_unlock(&ind_lock);*/
            some();
            printf("%s has departed\n",(char*)arg);
            for(int i = number_of_serviceman-1; i >=0; i--)
            {
                pthread_mutex_unlock(&lock[i]);
            }
            pthread_mutex_unlock(&lock2);
            //printf("%s has departed\n",(char*)arg);

        //pthread_exit((void*)strcat((char*)arg," has departed\n"));
        char str[]="";
        pthread_exit((void*)str);

        //sem_post(&full_array);
}


int main(int argc, char* argv[])
{
    int res;

    res = sem_init(&empty_array,0,arr_size);
    if(res != 0)
    {
        printf("Failed\n");
    }

    for(int i = 0; i < number_of_serviceman; i++)
    {
        res = pthread_mutex_init(&lock[i],NULL);
        if(res != 0)
        {
            printf("Failed\n");
        }
    }

    res = pthread_mutex_init(&lock2,NULL);
    if(res != 0){
        printf("Failed\n");
    }

    res = pthread_mutex_init(&ind_lock,NULL);
    if(res != 0){
        printf("Failed\n");
    }

    pthread_t cycles[number_of_cycle];

    for(int i = 0; i < number_of_cycle; i++)
    {
        char *id = new char[4];
        strcpy(id,to_string(i+1).c_str());

        res = pthread_create(&cycles[i],NULL,repair_shop,(void *)id);

        if(res != 0){
            printf("Thread creation failed\n");
        }
    }

    for(int i = 0; i < number_of_cycle; i++)
    {
        void *result;
        pthread_join(cycles[i],&result);
        printf("%s",(char*)result);
    }

    // destroy er serial change hole , final result ki change hobe??

    res = sem_destroy(&empty_array);
    if(res != 0){
        printf("Failed\n");
    }

    res = pthread_mutex_destroy(&lock2);
    if(res != 0){
        printf("Failed\n");
    }

    res = pthread_mutex_destroy(&ind_lock);
    if(res != 0){
        printf("Failed\n");
    }

    for(int i = 0; i < number_of_serviceman; i++)
    {
        res = pthread_mutex_destroy(&lock[i]);
        if(res != 0)
        {
            printf("Failed\n");
        }
    }

    return 0;

}
