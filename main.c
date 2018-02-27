/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ramy
 * Title: Limited Space Producer/Consumer solution
 * Created on January 31, 2018, 11:48 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

const unsigned int ARRAY_SIZE = 10;
int data_arr [10];
int CurrentIndex = 0;
int ReadIndex = 0;
int lastGenerated = 0;

sem_t semaphore, delay, block;
pthread_mutex_t mutex;
int n = 0;

int empty = 10;

int consumed1 = 0, consumed2 = 0, consumed3 = 0, consumed4 = 0;
int produced1 = 0, produced2 = 0, produced3 = 0, produced4 = 0;

int producer1lastindex = 0, producer2lastindex = 0, producer3lastindex = 0, producer4lastindex = 0;



void* CleanUp(){
    //I wrote this to clean text files from previous runs.
    fclose(fopen("/home/ramy/os/producer-consumer-limited/producer1.txt", "w"));
    
    fclose(fopen("/home/ramy/os/producer-consumer-limited/consumer1.txt", "w"));
    fclose(fopen("/home/ramy/os/producer-consumer-limited/consumer2.txt", "w"));
    fclose(fopen("/home/ramy/os/producer-consumer-limited/consumer3.txt", "w"));
    fclose(fopen("/home/ramy/os/producer-consumer-limited/consumer4.txt", "w"));
    
    printf("All files are clean now!\n");
}

void combine(char* destination, const char* path1, const char* path2)
{
    if(path1 == NULL && path2 == NULL) {
        strcpy(destination, "");;
    }
    else if(path2 == NULL || strlen(path2) == 0) {
        strcpy(destination, path1);
    }
    else if(path1 == NULL || strlen(path1) == 0) {
        strcpy(destination, path2);
    } 
    else {
        char directory_separator[] = "/";
#ifdef WIN32
        directory_separator[0] = '\\';
#endif
        const char *last_char = path1;
        while(*last_char != '\0')
            last_char++;        
        int append_directory_separator = 0;
        if(strcmp(last_char, directory_separator) != 0) {
            append_directory_separator = 1;
        }
        strcpy(destination, path1);
        if(append_directory_separator)
            strcat(destination, directory_separator);
        strcat(destination, path2);
    }
}

int Produce(){
    int val = 0;
    srand(time(NULL));
    val = rand() % 1000;
    while(val == lastGenerated)
        val = rand() % 1000;
    lastGenerated = val;
  
    return val;
     
}

void* Store(int val){
      
    FILE *f = fopen("/home/ramy/os/producer-consumer-limited/producer1.txt", "a");
    if(f == NULL) printf("failed to open the file");
    else
        fprintf(f, "%d\n", val);
    fclose(f);
    
    data_arr[CurrentIndex % 10] = val;
    CurrentIndex++;
}

int Read(){
 
    FILE * prod = fopen("/home/ramy/os/producer-consumer-limited/producer1.txt", "r");
    int count = 0;
    if ( prod != NULL )
    {
        char line[5];  
        while (fgets(line, sizeof line, prod) != NULL)  
        {
            if (count == ReadIndex)
            {
                fclose(prod);
               
                //printf("%d\t",data_arr[ReadIndex]);
                //ReadIndex = ReadIndex + 1;
                
            }
            else
            {
                count++;
            }
        }
    }
    
     
    int val = data_arr[ReadIndex % 10];
     ReadIndex = ReadIndex + 1;
     return val;
}
 
void* Consume(const char* num, int val){
  
    char result[45];
    const char *d = "/home/ramy/os/producer-consumer-limited/";
    const char* fn = num;
    combine(result, d, fn);
     FILE *f = fopen(result, "a");
                if(f == NULL) printf("failed to open the file");
                else
                fprintf(f, "%d\n", val);
                fclose(f);
    //printf("%d\t",val);
   
    
}

void * CheckBeforeProducing(){
    if(CurrentIndex - consumed1 > 9){
        sem_post(&delay);
            sem_wait(&block);
        }
         
        if(CurrentIndex - consumed2 > 9){
            sem_post(&delay);
            sem_wait(&block);
        }
        if(CurrentIndex - consumed3 > 9){
            sem_post(&delay);
             sem_wait(&block);
        }
        if(CurrentIndex - consumed4 > 9 ){
            sem_post(&delay);
             sem_wait(&block);
        }
}

void * Producer1Thread(){
    
    printf("Producer1 Thread Started!\n");
    while(produced1 < 100000){
        //if(CurrentIndex >= 10000) break;
        CheckBeforeProducing();
         
        int val = Produce();
          sem_wait(&semaphore);
          //critical section
          pthread_mutex_lock(&mutex);
          Store(val);
          producer1lastindex = (CurrentIndex -1) % 10;
          pthread_mutex_unlock(&mutex);
          printf("Producer #1, produced at place %d, value %d\n", producer1lastindex, val);
          produced1++;
          empty--;
          
          //End of critical
          
          n++;
          if(n == 1) sem_post(&delay);
          sem_post(&semaphore);
    }
    
    printf("Producer 1 Produced Items equals %d\n", produced1);
    return NULL;
}
void * Producer2Thread(){
    
    printf("Producer2 Thread Started!\n");
    while(produced2 < 100000){
        //if(CurrentIndex >= 10000) break;
        CheckBeforeProducing();
         
        int val = Produce();
          sem_wait(&semaphore);
          //critical section
           pthread_mutex_lock(&mutex);
          Store(val);
          producer2lastindex = (CurrentIndex -1) % 10;
           pthread_mutex_unlock(&mutex);
           printf("Producer #2, produced at place %d, value %d\n", producer2lastindex, val);
          empty--;
          produced2++;
          //End of critical
          
          n++;
          if(n == 1) sem_post(&delay);
          sem_post(&semaphore);
    }
    
    printf("Producer 2 Produced Items equals %d\n", produced2);
    return NULL;
}
void * Producer3Thread(){
    
    printf("Producer3 Thread Started!\n");
    while(produced3 < 100000){
        //if(CurrentIndex00 >= 10000) break;
        CheckBeforeProducing();
         
        int val = Produce();
          sem_wait(&semaphore);
          //critical section
           pthread_mutex_lock(&mutex);
          Store(val);
          producer3lastindex = (CurrentIndex -1) % 10;
           pthread_mutex_unlock(&mutex);
           printf("Producer #3, produced at place %d, value %d\n", producer3lastindex, val);
          produced3++;
          empty--;
          //End of critical
          
          n++;
          if(n == 1) sem_post(&delay);
          sem_post(&semaphore);
    }
    
    printf("Producer 3 Produced Items equals %d\n", produced3);
    return NULL;
}
void * Producer4Thread(){
    
    printf("Producer4 Thread Started!\n");
    while(produced4 < 100000){
        //if(CurrentIndex >= 10000) break;
        CheckBeforeProducing();
         
        int val = Produce();
          sem_wait(&semaphore);
          //critical section
           pthread_mutex_lock(&mutex);
          Store(val);
          producer4lastindex = (CurrentIndex -1) % 10;
           pthread_mutex_unlock(&mutex);
           printf("Producer #4, produced at place %d, value %d\n", producer4lastindex, val);
          produced4++;
          empty--;
          //End of critical
          
          n++;
          if(n == 1) sem_post(&delay);
          sem_post(&semaphore);
    }
    
    printf("Producer 4 Produced Items equals %d\n", produced4);
    return NULL;
}

void * Consumer1Thread(){
    printf("Consumer 1 Thread Started!\n");
     while(consumed1 < 400000){
        
        if(n == 0) sem_wait(&delay);
        sem_wait(&semaphore);
        //Critical section
        int val = Read();
         n--;
        consumed1++;
        printf("Consumer #1, consumed at place %d, value %d\n", (ReadIndex - 1) % 10, val);
        //End Of critical
        sem_post(&semaphore);
        sem_post(&block);
        Consume("consumer1.txt", val);
        
    }
    printf("Consumer 1 Consumed Items is %d\n", consumed1);
    return NULL;
}
void * Consumer2Thread(){
    printf("Consumer 2 Thread Started!\n");
     while(consumed2 < 400000){
        
        if(n == 0) sem_wait(&delay);
        sem_wait(&semaphore);
        //Critical section
        int val = Read();
         n--;
        consumed2++;
        printf("Consumer #2, consumed at place %d, value %d\n", (ReadIndex - 1) % 10, val);
        //End Of critical
        sem_post(&semaphore);
        sem_post(&block);
        Consume("consumer2.txt", val);
        
    }
    printf("Consumer 2 Consumed Items is %d\n", consumed2);
    return NULL;
}

void * Consumer3Thread(){
    printf("Consumer 3 Thread Started!\n");
     while(consumed3 < 400000){
        
        if(n == 0) sem_wait(&delay);
        sem_wait(&semaphore);
        //Critical section
        int val = Read();
         n--;
        consumed3++;
        printf("Consumer #3, consumed at place %d, value %d\n", (ReadIndex - 1) % 10, val);
        //End Of critical
        sem_post(&semaphore);
        sem_post(&block);
        Consume("consumer3.txt", val);
        
    }
    printf("Consumer 3 Consumed Items is %d\n", consumed3);
    return NULL;
}

void * Consumer4Thread(){
    printf("Consumer 4 Thread Started!\n");
     while(consumed4 < 400000){
        
        if(n == 0) sem_wait(&delay);
        sem_wait(&semaphore);
        //Critical section
        int val = Read();
         n--;
        consumed4++;
        printf("Consumer #4, consumed at place %d, value %d\n", (ReadIndex - 1) % 10, val);
        //End Of critical
        sem_post(&semaphore);
        sem_post(&block);
        Consume("consumer4.txt", val);
        
    }
    printf("Consumer 4 Consumed Items is %d\n", consumed4);
    return NULL;
}



void* MonitorThread(){
    while(1){
        //printf("CurrentIndex is %d", CurrentIndex);
        if(CurrentIndex - ReadIndex > 1000){
           // printf("The difference is: %d at %d Write and %d Read\n",
             //       CurrentIndex - ReadIndex, CurrentIndex, ReadIndex);
        }
        if(CurrentIndex == 10000){
            break;
        }
    }
}

int main(int argc, char** argv) {
  
    CleanUp();
     
    
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semaphore, 0, 0);
    sem_init(&delay, 0, 0);
    sem_init(&block, 0, 0);
     pthread_t c1, c2, c3, c4, p1, p2, p3, p4, monitor;
     sem_post(&semaphore);
     pthread_create(&p1,NULL, Producer1Thread,NULL);
     pthread_create(&p2,NULL, Producer2Thread,NULL);
     pthread_create(&p3,NULL, Producer3Thread,NULL);
     pthread_create(&p4,NULL, Producer4Thread,NULL);
     
     pthread_create(&c1,NULL, Consumer1Thread, NULL);
     pthread_create(&c2,NULL, Consumer2Thread, NULL);
     pthread_create(&c3,NULL, Consumer3Thread, NULL);
     pthread_create(&c4,NULL, Consumer4Thread, NULL);
    // pthread_create(&monitor, NULL, MonitorThread, NULL);
     
    
     pthread_join(p1, NULL);
     pthread_join(p2, NULL);
     pthread_join(p3, NULL);
     pthread_join(p4, NULL);
     
     pthread_join(c1, NULL);
     pthread_join(c2, NULL);
     pthread_join(c3, NULL);
     pthread_join(c4, NULL);
     
     //pthread_join(monitor, NULL);
   
     
    return (EXIT_SUCCESS);
}
