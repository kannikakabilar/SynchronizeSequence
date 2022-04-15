#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int * seq(int n, int a, int b){
    /* Given the first 2 instance a, and b, calculate the nth 
    and n-1th value in the  same format as the fibonacci sequence */
    int *ret = malloc(8);
    if(n == 0){
        ret[0] = a;
        ret[1] = 1;
        return ret;
    }else if(n == 1){
        ret[0] = b;
        ret[1] = a;
        return ret;
    }else{
        int i, c;
        for(i = 2; i <= n; i++){
            c = a + b;
            a = b;
            b = c;
        }
        ret[0] = b;
        ret[1] = a;
        return ret;
    }
}

int main(void) {
    int nth;
    printf("Enter the order of the sequence to be calculated: ");
    scanf("%d", &nth);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipeid");
        exit(1);
    }
    
    int result = fork();
  
    if (result == 0) {
        /* Child process will calculate the Lucas' sequence
        and send it to parent through pipefd */
        
        if (close(pipefd[0]) == -1) {
            perror("read closed abnormally at child");
            exit(1);
        }
        
        int *lucas = malloc(8);
        lucas = seq(nth, 2, 1);
        
        if (write(pipefd[1], &lucas[0], sizeof(lucas[0])) == -1) {
            perror("write does not work properly at child");
            exit(1);
        }
        if (write(pipefd[1], &lucas[1], sizeof(lucas[1])) == -1) {
            perror("write does not work properly at child");
            exit(1);
        }
        
        if (close(pipefd[1]) == -1) {
            perror("write closed abnormally at child");
            exit(1);
        }
        return 0;
        
    }
    /* Parent process will calculate the Fibonacci sequence
       and sums with Lucas sequence read through pipefd */

    close(pipefd[1]);
    /*
    // Another program can be synchronized with the calculation  
    // of the sequence order by using the below code
    int sub = fork();
    if (sub == 0 && dup2(pipefd[0], fileno(stdin)) >= 0) {
        execl("./sub_program", sub_program, NULL);
    }
    */
    int *fib = malloc(8);
    fib = seq(nth, 0, 1);
    float fib_ratio = (float)fib[0]/(float)fib[1];
    printf("Fibonacci sequence of order %d is: %d\n", nth, fib[0]);
    printf("Fibonacci Golden Ratio of order %d is: %9.9f\n", nth, fib_ratio);

    int luc_curr, luc_prev;
    if (read(pipefd[0], &luc_curr, sizeof(luc_curr)) == -1) {
            perror("write does not work properly at parent");
            exit(1);
    }
    if (read(pipefd[0], &luc_prev, sizeof(luc_prev)) == -1) {
        perror("write does not work properly at parent");
        exit(1);
    }
    float luc_ratio = (float)luc_curr/(float)luc_prev;
    printf("Lucas sequence of order %d is: %d\n", nth, luc_curr);
    printf("Lucas Golden Ratio of order %d is: %9.9f\n", nth, luc_ratio);
    printf("The sum of Fibonacci and Lucas at order %d is: %d\n", nth, fib[0]+luc_curr);
    
    if (close(pipefd[0]) == -1) {
        perror("read closed abnormally at parent");
        exit(1);
    }
    
    int status;
    if (wait(&status) == -1) {
        perror("wait");
        exit(1);
    }
    
    return 0;
}

