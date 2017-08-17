#include <stdio.h>

#define uint8_t unsigned char
#define uint16_t unsigned int
#define MAX_TIMERS 5

int suma(int a, int b)
{
    return a+b;
}

int resta(int a, int b)
{
    return a-b;
}

int mult(int a, int b)
{
    return a*b;
}

typedef struct
{
    uint16_t value;
    uint16_t period;
    void (*callback)(void);
    uint8_t enabled;
    uint8_t reload;
} timer_t;

/*Create a Timer scheduler*/
timer_t timers[5];
uint8_t timers_used=0;

uint8_t init_timers(void)
{
    /*Find an empty slot*/
    uint8_t i;
    for(i=0;i<MAX_TIMERS;i++)
    {
        timers[i].enabled=0;
    }
}

/*Return 1 when create timer, 0 if fail*/
uint8_t create_timer(uint16_t period, uint16_t start, uint8_t reload,void (*callback)(void))
{
    
    int retval=0;
    if(timers_used < MAX_TIMERS)
    {
        /*Find an empty slot*/
        uint8_t i;
        uint8_t found=0;
        for(i=0;i<MAX_TIMERS;i++)
        {
            if(timers[i].enabled==0)
            {
                found=1;
                break;
            }
        }
        
        /*Check if timer has been found*/
        if(found!=0)
        {
            timers[i].period=period;
            timers[i].value=start;
            timers[i].callback=callback;
            timers[i].enabled=1;
            timers[i].reload=reload;
            retval=1;
        }        
    }
    
    return retval;
    
}


void run_scheduler(void)
{

    uint8_t i;
    /*Loop thru the timers*/
    for(i=0;i<MAX_TIMERS;i++)
    {
        if(timers[i].enabled!=0)
        {
            /*Increment timer*/
            timers[i].value++;
            
            /*Check for overflow*/
            if(timers[i].value>=timers[i].period) 
            {
                /*Callack*/
                timers[i].callback();
                
                /*Reset timer*/
                timers[i].value=0;
                
                /*Disable timer if we will not reload*/
                if(timers[i].reload==0)
                {
                    timers[i].enabled=0;
                }
            }
        }
    }
    
}


void ding(void)
{
    printf("Ding\n");
}

void dong(void)
{
    printf("Dong\n");
}

int main()
{
    int a;
    int b;
    int (*func)(int,int);
    
    a=8;
    b=2;
    printf("Testing Function Pointers\n");
    
    func=suma;
    printf("Testing Suma %d + %d = %d: \n",a,b,func(a,b));
    
    func=resta;
    printf("Testing Resta %d - %d = %d: \n",a,b,func(a,b));    
    
    func=mult;
    printf("Testing Mult %d x %d = %d: \n",a,b,func(a,b));    

    int rv;
    rv=create_timer(100,0,1,dong);
    if(rv==0) {
        printf("Error Creating timer\n");
    }
    
    rv=create_timer(50,0,1,ding);
    if(rv==0) {
        printf("Error Creating timer\n");
    }    
    
    /*Loop some time to test the scheduler*/
    uint16_t loop;
    for(loop=0;loop<1000;loop++)
    {
        run_scheduler();
    }

    return 0;
}

